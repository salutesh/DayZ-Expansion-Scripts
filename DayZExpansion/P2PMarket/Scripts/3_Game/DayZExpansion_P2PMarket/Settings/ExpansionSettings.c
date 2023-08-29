/**
 * ExpansionSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ExpansionSettings
{
	static ref ScriptInvoker SI_P2PMarket = new ScriptInvoker();

	override void Init()
	{
		super.Init();

		Init(ExpansionP2PMarketSettings);
	}

	override bool OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_4(ExpansionTracing.SETTINGS, this, "OnRPC").Add(sender).Add(target).Add(rpc_type).Add(ctx);
#endif

		if (super.OnRPC(sender, target, rpc_type, ctx))
			return true;

		if ( rpc_type <= ExpansionSettingsRPC.INVALID || rpc_type >= ExpansionSettingsRPC.COUNT )
			return false;

		switch ( rpc_type )
		{
			case ExpansionSettingsRPC.P2PMarket:
			{
				Receive(ExpansionP2PMarketSettings, ctx);
				return true;
			}
		}

		return false;
	}

	ExpansionP2PMarketSettings GetP2PMarket(bool checkLoaded = true)
	{
		return ExpansionP2PMarketSettings.Cast(Get(ExpansionP2PMarketSettings, checkLoaded));
	}
};
