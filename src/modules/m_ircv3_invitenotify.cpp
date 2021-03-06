/*
 * InspIRCd -- Internet Relay Chat Daemon
 *
 *   Copyright (C) 2015 Attila Molnar <attilamolnar@hush.com>
 *
 * This file is part of InspIRCd.  InspIRCd is free software: you can
 * redistribute it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, version 2.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "inspircd.h"
#include "modules/cap.h"

class ModuleIRCv3InviteNotify : public Module
{
	Cap::Capability cap;

 public:
	ModuleIRCv3InviteNotify()
		: cap(this, "invite-notify")
	{
	}

	void OnUserInvite(User* source, User* dest, Channel* chan, time_t expiry, unsigned int notifyrank, CUList& notifyexcepts) override
	{
		std::string msg = "INVITE ";
		msg.append(dest->nick).append(1, ' ').append(chan->name);
		const Channel::MemberMap& users = chan->GetUsers();
		for (Channel::MemberMap::const_iterator i = users.begin(); i != users.end(); ++i)
		{
			User* user = i->first;
			// Skip members who don't use this extension or were excluded by other modules
			if ((!cap.get(user)) || (notifyexcepts.count(user)))
				continue;

			Membership* memb = i->second;
			// Check whether the member has a high enough rank to see the notification
			if (memb->getRank() < notifyrank)
				continue;

			// Send and add the user to the exceptions so they won't get the NOTICE invite announcement message
			user->WriteFrom(source, msg);
			notifyexcepts.insert(user);
		}
	}

	void Prioritize() override
	{
		// Prioritize after all modules to see all excepted users
		ServerInstance->Modules.SetPriority(this, I_OnUserInvite, PRIORITY_LAST);
	}

	Version GetVersion() override
	{
		return Version("Provides the invite-notify IRCv3.2 extension", VF_VENDOR);
	}
};

MODULE_INIT(ModuleIRCv3InviteNotify)
