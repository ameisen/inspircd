/*
 * InspIRCd -- Internet Relay Chat Daemon
 *
 *   Copyright (C) 2009 Daniel De Graaf <danieldg@inspircd.org>
 *   Copyright (C) 2008 Thomas Stagner <aquanight@inspircd.org>
 *   Copyright (C) 2007-2008 Craig Edwards <craigedwards@brainbox.cc>
 *   Copyright (C) 2007 Robin Burchell <robin+git@viroteck.net>
 *   Copyright (C) 2007 Dennis Friis <peavey@inspircd.org>
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


#pragma once

#include "inspircd.h"
#include "event.h"
#include "modules/dns.h"
#include "servercommand.h"
#include "commands.h"
#include "protocolinterface.h"

/** If you make a change which breaks the protocol, increment this.
 * If you  completely change the protocol, completely change the number.
 *
 * IMPORTANT: If you make changes, document your changes here, without fail:
 * http://wiki.inspircd.org/List_of_protocol_changes_between_versions
 *
 * Failure to document your protocol changes will result in a painfully
 * painful death by pain. You have been warned.
 */
const long ProtocolVersion = 1205;
const long MinCompatProtocol = 1202;

/** Forward declarations
 */
class SpanningTreeUtilities;
class CacheRefreshTimer;
class TreeServer;
class Link;
class Autoconnect;

/** This is the main class for the spanningtree module
 */
class ModuleSpanningTree : public Module
{
	/** Client to server commands, registered in the core
	 */
	CommandRConnect rconnect;
	CommandRSQuit rsquit;
	CommandMap map;

	/** Server to server only commands, not registered in the core
	 */
	SpanningTreeCommands commands;

	/** Next membership id assigned when a local user joins a channel
	 */
	Membership::Id currmembid;

	/** The specialized ProtocolInterface that is assigned to ServerInstance->PI on load
	 */
	SpanningTreeProtocolInterface protocolinterface;

	/** Event provider for our events
	 */
	Events::ModuleEventProvider eventprov;

 public:
	dynamic_reference<DNS::Manager> DNS;

	ServerCommandManager CmdManager;

	/** Set to true if inside a spanningtree call, to prevent sending
	 * xlines and other things back to their source
	 */
	bool loopCall;

	/** Constructor
	 */
	ModuleSpanningTree();
	void init() override;

	/** Shows /LINKS
	 */
	void ShowLinks(TreeServer* Current, User* user, int hops);

	/** Handle LINKS command
	 */
	void HandleLinks(const std::vector<std::string>& parameters, User* user);

	/** Handle SQUIT
	 */
	ModResult HandleSquit(const std::vector<std::string>& parameters, User* user);

	/** Handle remote WHOIS
	 */
	ModResult HandleRemoteWhois(const std::vector<std::string>& parameters, User* user);

	/** Connect a server locally
	 */
	void ConnectServer(Link* x, Autoconnect* y = nullptr);

	/** Connect the next autoconnect server
	 */
	void ConnectServer(Autoconnect* y, bool on_timer);

	/** Check if any servers are due to be autoconnected
	 */
	void AutoConnectServers(time_t curtime);

	/** Check if any connecting servers should timeout
	 */
	void DoConnectTimeout(time_t curtime);

	/** Handle remote VERSON
	 */
	ModResult HandleVersion(const std::vector<std::string>& parameters, User* user);

	/** Handle CONNECT
	 */
	ModResult HandleConnect(const std::vector<std::string>& parameters, User* user);

	/** Display a time as a human readable string
	 */
	static std::string TimeToStr(time_t secs);

	const Events::ModuleEventProvider& GetEventProvider() const { return eventprov; }

	/**
	 ** *** MODULE EVENTS ***
	 **/

	ModResult OnPreCommand(std::string &command, std::vector<std::string>& parameters, LocalUser *user, bool validated, const std::string &original_line) override;
	void OnPostCommand(Command*, const std::vector<std::string>& parameters, LocalUser* user, CmdResult result, const std::string& original_line) override;
	void OnUserConnect(LocalUser* source) override;
	void OnUserInvite(User* source, User* dest, Channel* channel, time_t timeout, unsigned int notifyrank, CUList& notifyexcepts) override;
	ModResult OnPreTopicChange(User* user, Channel* chan, const std::string& topic) override;
	void OnPostTopicChange(User* user, Channel* chan, const std::string &topic) override;
	void OnUserMessage(User* user, void* dest, int target_type, const std::string& text, char status, const CUList& exempt_list, MessageType msgtype) override;
	void OnBackgroundTimer(time_t curtime) override;
	void OnUserJoin(Membership* memb, bool sync, bool created, CUList& excepts) override;
	void OnChangeHost(User* user, const std::string &newhost) override;
	void OnChangeName(User* user, const std::string &gecos) override;
	void OnChangeIdent(User* user, const std::string &ident) override;
	void OnUserPart(Membership* memb, std::string &partmessage, CUList& excepts) override;
	void OnUserQuit(User* user, const std::string &reason, const std::string &oper_message) override;
	void OnUserPostNick(User* user, const std::string &oldnick) override;
	void OnUserKick(User* source, Membership* memb, const std::string &reason, CUList& excepts) override;
	void OnPreRehash(User* user, const std::string &parameter) override;
	void ReadConfig(ConfigStatus& status) override;
	void OnOper(User* user, const std::string &opertype) override;
	void OnAddLine(User *u, XLine *x) override;
	void OnDelLine(User *u, XLine *x) override;
	ModResult OnStats(Stats::Context& stats) override;
	ModResult OnSetAway(User* user, const std::string &awaymsg) override;
	void OnLoadModule(Module* mod) override;
	void OnUnloadModule(Module* mod) override;
	ModResult OnAcceptConnection(int newsock, ListenSocket* from, irc::sockets::sockaddrs* client, irc::sockets::sockaddrs* server) override;
	void OnMode(User* source, User* u, Channel* c, const Modes::ChangeList& modes, ModeParser::ModeProcessFlag processflags, const std::string& output_mode) override;
	CullResult cull() override;
	~ModuleSpanningTree();
	Version GetVersion() override;
	void Prioritize() override;
};
