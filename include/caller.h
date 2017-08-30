/*
 * InspIRCd -- Internet Relay Chat Daemon
 *
 *   Copyright (C) 2009 Daniel De Graaf <danieldg@inspircd.org>
 *   Copyright (C) 2007 Craig Edwards <craigedwards@brainbox.cc>
 *   Copyright (C) 2012 Adam <Adam@anope.org>
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

template<typename ReturnType, typename... Args> class CoreExport Handler : public classbase
{
 public:
	virtual ~Handler() = default;
	virtual ReturnType Call(Args...) = 0;
};

template<typename ReturnType, typename... Args> class CoreExport Caller
{
 public:
	Handler<ReturnType, Args...>* target;

	Caller(Handler<ReturnType, Args...>* initial) : target(initial) { }
	virtual ~Caller() = default;

	virtual ReturnType operator()(const Args&... params)
	{
		return this->target->Call(params...);
	}
};

#define DEFINE_HANDLER(NAME, RETURN, ...) \
	class CoreExport NAME final : public Handler<RETURN, ##__VA_ARGS__> { public: NAME() = default; virtual RETURN Call(__VA_ARGS__) override final; }
