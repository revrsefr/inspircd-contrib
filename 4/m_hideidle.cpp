/*
 * InspIRCd -- Internet Relay Chat Daemon
 *
 *   Copyright (C) 2010-2012 Attila Molnar <attilamolnar@hush.com>
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

/// $ModAuthor: Attila Molnar <attilamolnar@hush.com>
/// $ModDepends: core 4
/// $ModDesc: Provides the +a usermode that hides idle and signon time in WHOIS from non-opers


#include "inspircd.h"
#include "modules/whois.h"

class ModuleHideIdle final
	: public Module
	, public Whois::LineEventListener
{
private:
	SimpleUserMode hideidle;

public:
	ModuleHideIdle()
		: Module(VF_NONE, "Provides the +a usermode that hides idle and signon time in WHOIS from non-opers")
		, Whois::LineEventListener(this)
		, hideidle(this, "hideidle", 'a')
	{
	}

	ModResult OnWhoisLine(Whois::Context& whois, Numeric::Numeric& numeric) override
	{
		if (numeric.GetNumeric() != RPL_WHOISIDLE)
			return MOD_RES_PASSTHRU;

		if (whois.GetSource() == whois.GetTarget())
			return MOD_RES_PASSTHRU;

		if (!whois.GetTarget()->IsModeSet(hideidle))
			return MOD_RES_PASSTHRU;

		if (!whois.GetSource()->HasPrivPermission("users/auspex"))
			return MOD_RES_DENY;

		return MOD_RES_PASSTHRU;
	}
};

MODULE_INIT(ModuleHideIdle)
