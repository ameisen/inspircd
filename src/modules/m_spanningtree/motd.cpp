/*       +------------------------------------+
 *       | Inspire Internet Relay Chat Daemon |
 *       +------------------------------------+
 *
 *  InspIRCd: (C) 2002-2008 InspIRCd Development Team
 * See: http://www.inspircd.org/wiki/index.php/Credits
 *
 * This program is free but copyrighted software; see
 *            the file COPYING for details.
 *
 * ---------------------------------------------------
 */

#include "inspircd.h"
#include "xline.h"

#include "m_spanningtree/treesocket.h"
#include "m_spanningtree/treeserver.h"
#include "m_spanningtree/utils.h"

/* $ModDep: m_spanningtree/utils.h m_spanningtree/treeserver.h m_spanningtree/treesocket.h */



/** remote MOTD. leet, huh? */
bool TreeSocket::Motd(const std::string &prefix, std::deque<std::string> &params)
{
	if (params.size() > 0)
	{
		if (InspIRCd::Match(this->Instance->Config->ServerName, params[0]))
		{
			/* It's for our server */
			string_list results;
			User* source = this->Instance->FindNick(prefix);

			if (source)
			{
				std::deque<std::string> par;
				par.push_back(prefix);
				par.push_back("");

				if (!Instance->Config->MOTD.size())
				{
					par[1] = std::string("::")+Instance->Config->ServerName+" 422 "+source->nick+" :Message of the day file is missing.";
					Utils->DoOneToOne(this->Instance->Config->GetSID(), "PUSH",par, source->server);
					return true;
				}

				par[1] = std::string("::")+Instance->Config->ServerName+" 375 "+source->nick+" :"+Instance->Config->ServerName+" message of the day";
				Utils->DoOneToOne(this->Instance->Config->GetSID(), "PUSH",par, source->server);

				for (unsigned int i = 0; i < Instance->Config->MOTD.size(); i++)
				{
					par[1] = std::string("::")+Instance->Config->ServerName+" 372 "+source->nick+" :- "+Instance->Config->MOTD[i];
					Utils->DoOneToOne(this->Instance->Config->GetSID(), "PUSH",par, source->server);
				}

				par[1] = std::string("::")+Instance->Config->ServerName+" 376 "+source->nick+" :End of message of the day.";
				Utils->DoOneToOne(this->Instance->Config->GetSID(), "PUSH",par, source->server);
			}
		}
		else
		{
			/* Pass it on */
			User* source = this->Instance->FindNick(prefix);
			if (source)
				Utils->DoOneToOne(prefix, "MOTD", params, params[0]);
		}
	}
	return true;
}

