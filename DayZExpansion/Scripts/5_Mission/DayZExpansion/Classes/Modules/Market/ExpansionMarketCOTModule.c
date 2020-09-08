/**
 * ExpansionMarketCOTModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketCOTModule: JMRenderableModuleBase
{
	void ExpansionMarketCOTModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.Market" );

		GetPermissionsManager().RegisterPermission( "Expansion.Market.View" );
	}

	override void OnMissionLoaded()
	{
		super.OnMissionLoaded();
	}

	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.Market.View" );
	}

	override string GetLayoutRoot()
	{
		return "DayZExpansion/GUI/layouts/COT/market_form.layout";
	}

	override string GetTitle()
	{
		return "[EX] Market Management";
	}
	
	override string GetIconName()
	{
		return "M";
	}

	override bool ImageIsIcon()
	{
		return false;
	}
	
	override int GetRPCMin()
	{
		return ExpansionMarketModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionMarketModuleRPC.COUNT;
	}

	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	{
		switch ( rpc_type )
		{
		case ExpansionMarketModuleRPC.GiveMoney:
		case ExpansionMarketModuleRPC.ReserveMoney:
		case ExpansionMarketModuleRPC.DeleteMoney:
		case ExpansionMarketModuleRPC.UnlockMoney:
		//case ExpansionMarketModuleRPC.Load:
		//	RPC_Load( ctx, sender, target );
			break;
		}
	}
}