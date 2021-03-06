/*
 * InspIRCd -- Internet Relay Chat Daemon
 *
 *   Copyright (C) 2009 Daniel De Graaf <danieldg@inspircd.org>
 *   Copyright (C) 2008 Robin Burchell <robin+git@viroteck.net>
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

class ModuleQuietBan : public Module
{
 public:
	Version GetVersion() override
	{
		return Version("Implements extban +b m: - mute bans",VF_OPTCOMMON|VF_VENDOR);
	}

	ModResult OnUserPreMessage(User* user, void* dest, int target_type, std::string& text, char status, CUList& exempt_list, MessageType msgtype) override
	{
		if (!user->as<LocalUser>() || target_type != TYPE_CHANNEL)
			return MOD_RES_PASSTHRU;

		Channel* chan = static_cast<Channel*>(dest);
		if (chan->GetExtBanStatus(user, 'm') == MOD_RES_DENY && chan->GetPrefixValue(user) < VOICE_VALUE)
		{
			user->WriteNumeric(ERR_CANNOTSENDTOCHAN, chan->name, "Cannot send to channel (you're muted)");
			return MOD_RES_DENY;
		}

		return MOD_RES_PASSTHRU;
	}

	void On005Numeric(std::map<std::string, std::string>& tokens) override
	{
		tokens["EXTBAN"].push_back('m');
	}
};

MODULE_INIT(ModuleQuietBan)
