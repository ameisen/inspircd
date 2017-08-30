/*
 * InspIRCd -- Internet Relay Chat Daemon
 *
 *   Copyright (C) 2009 Daniel De Graaf <danieldg@inspircd.org>
 *   Copyright (C) 2004, 2008-2009 Craig Edwards <craigedwards@brainbox.cc>
 *   Copyright (C) 2007 Dennis Friis <peavey@inspircd.org>
 *   Copyright (C) 2005, 2007 Robin Burchell <robin+git@viroteck.net>
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

// Temporary hack to get around a warning being thrown by a Microsoft header.
#if _MSC_VER == 1911
#	pragma warning( disable : 4244 )  
	#include <xstring>
#	pragma warning( default : 4244 )  
#endif

#include "inspircd.h"
#include "modules/exemption.h"

typedef insp::flat_map<irc::string, irc::string> censor_t;

/** Handles usermode +G
 */
class CensorUser : public SimpleUserModeHandler
{
 public:
	CensorUser(Module* Creator) : SimpleUserModeHandler(Creator, "u_censor", 'G') { }
};

/** Handles channel mode +G
 */
class CensorChannel : public SimpleChannelModeHandler
{
 public:
	CensorChannel(Module* Creator) : SimpleChannelModeHandler(Creator, "censor", 'G') { }
};

class ModuleCensor : public Module
{
	CheckExemption::EventProvider exemptionprov;
	censor_t censors;
	CensorUser cu;
	CensorChannel cc;

 public:
	ModuleCensor()
		: exemptionprov(this)
		, cu(this)
		, cc(this)
	{
	}

	// format of a config entry is <badword text="shit" replace="poo">
	ModResult OnUserPreMessage(User* user, void* dest, int target_type, std::string& text, char status, CUList& exempt_list, MessageType msgtype) override
	{
		if (!user->as<LocalUser>())
			return MOD_RES_PASSTHRU;

		bool active = false;

		if (target_type == TYPE_USER)
			active = ((User*)dest)->IsModeSet(cu);
		else if (target_type == TYPE_CHANNEL)
		{
			Channel* c = (Channel*)dest;
			active = c->IsModeSet(cc);
			ModResult res;
			FIRST_MOD_RESULT_CUSTOM(exemptionprov, CheckExemption::EventListener, OnCheckExemption, res, (user, c, "censor"));

			if (res == MOD_RES_ALLOW)
				return MOD_RES_PASSTHRU;
		}

		if (!active)
			return MOD_RES_PASSTHRU;

		irc::string text2 = text.c_str();
		for (censor_t::iterator index = censors.begin(); index != censors.end(); index++)
		{
			if (text2.find(index->first) != irc::string::npos)
			{
				if (index->second.empty())
				{
					user->WriteNumeric(ERR_WORDFILTERED, ((target_type == TYPE_CHANNEL) ? ((Channel*)dest)->name : ((User*)dest)->nick), index->first.c_str(), "Your message contained a censored word, and was blocked");
					return MOD_RES_DENY;
				}

				stdalgo::string::replace_all(text2, index->first, index->second);
			}
		}
		text = text2.c_str();
		return MOD_RES_PASSTHRU;
	}

	void ReadConfig(ConfigStatus& status) override
	{
		/*
		 * reload our config file on rehash - we must destroy and re-allocate the classes
		 * to call the constructor again and re-read our data.
		 */
		censors.clear();

		ConfigTagList badwords = ServerInstance->Config->ConfTags("badword");
		for (ConfigIter i = badwords.first; i != badwords.second; ++i)
		{
			ConfigTag* tag = i->second;
			std::string str = tag->getString("text");
			irc::string pattern(str.c_str());
			str = tag->getString("replace");
			censors[pattern] = irc::string(str.c_str());
		}
	}

	Version GetVersion() override
	{
		return Version("Provides user and channel +G mode",VF_VENDOR);
	}

};

MODULE_INIT(ModuleCensor)
