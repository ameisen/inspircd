/*
 * InspIRCd -- Internet Relay Chat Daemon
 *
 *   Copyright (C) 2004, 2008 Craig Edwards <craigedwards@brainbox.cc>
 *   Copyright (C) 2007 Dennis Friis <peavey@inspircd.org>
 *   Copyright (C) 2007 Robin Burchell <robin+git@viroteck.net>
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
#include "modules/exemption.h"

/** Handles channel mode +S
 */
class ChannelStripColor : public SimpleChannelModeHandler
{
 public:
	ChannelStripColor(Module* Creator) : SimpleChannelModeHandler(Creator, "stripcolor", 'S') { }
};

/** Handles user mode +S
 */
class UserStripColor : public SimpleUserModeHandler
{
 public:
	UserStripColor(Module* Creator) : SimpleUserModeHandler(Creator, "u_stripcolor", 'S') { }
};


class ModuleStripColor : public Module
{
	CheckExemption::EventProvider exemptionprov;
	ChannelStripColor csc;
	UserStripColor usc;

 public:
	ModuleStripColor()
		: exemptionprov(this)
		, csc(this)
		, usc(this)
	{
	}

	void On005Numeric(std::map<std::string, std::string>& tokens) override
	{
		tokens["EXTBAN"].push_back('S');
	}

	ModResult OnUserPreMessage(User* user, void* dest, int target_type, std::string& text, char status, CUList& exempt_list, MessageType msgtype) override
	{
		if (!user->as<LocalUser>())
			return MOD_RES_PASSTHRU;

		bool active = false;
		if (target_type == TYPE_USER)
		{
			User* t = (User*)dest;
			active = t->IsModeSet(usc);
		}
		else if (target_type == TYPE_CHANNEL)
		{
			Channel* t = (Channel*)dest;
			ModResult res;
			FIRST_MOD_RESULT_CUSTOM(exemptionprov, CheckExemption::EventListener, OnCheckExemption, res, (user, t, "stripcolor"));

			if (res == MOD_RES_ALLOW)
				return MOD_RES_PASSTHRU;

			active = !t->GetExtBanStatus(user, 'S').check(!t->IsModeSet(csc));
		}

		if (active)
		{
			InspIRCd::StripColor(text);
		}

		return MOD_RES_PASSTHRU;
	}

	void OnUserPart(Membership* memb, std::string& partmessage, CUList& except_list) override
	{
		User* user = memb->user;
		Channel* channel = memb->chan;

		if (!user->as<LocalUser>())
			return;

		if (channel->GetExtBanStatus(user, 'S').check(!user->IsModeSet(csc)))
		{
			ModResult res;
			FIRST_MOD_RESULT_CUSTOM(exemptionprov, CheckExemption::EventListener, OnCheckExemption, res, (user, channel, "stripcolor"));

			if (res != MOD_RES_ALLOW)
				InspIRCd::StripColor(partmessage);
		}
	}

	Version GetVersion() override
	{
		return Version("Provides channel +S mode (strip ansi color)", VF_VENDOR);
	}

};

MODULE_INIT(ModuleStripColor)
