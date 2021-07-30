/**
 * ExpansionCOTTerritoryModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCOTTerritoryModule: ExpansionCOTModuleBase
{
	void ExpansionCOTTerritoryModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.Territory.View" );
		GetPermissionsManager().RegisterPermission( "Expansion.Territory.Update" );
	}

	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.Territory.View" );
	}

	override string GetLayoutRoot()
	{
		return "DayZExpansion/GUI/layouts/COT/Territory/form.layout";
	}

	override string GetTitle()
	{
		return "[EX] Territory Settings";
	}

	override string GetIconName()
	{
		return "T";
	}

	override bool ImageIsIcon()
	{
		return false;
	}

	override string GetWebhookTitle()
	{
		return "Expansion Territory Settings";
	}

	override ExpansionSettingBase GetSettingsInstance()
	{
		return GetExpansionSettings().GetTerritory();
	}

	override void OnRegisterSettings()
	{
		CreateToggle( "EnableTerritories", "Enable Territories", "", "" );
		CreateToggle( "UseWholeMapForInviteList", "Use Whole Map For Invite List", "", "" );

		CreateSlider( "TerritorySize", "Territory Size", "", "", 0.0, 500.0, 1.0);
		CreateSlider( "TerritoryPerimeterSize", "Territory Perimeter Size", "", "", 0.0, 500.0, 1.0);

		CreateSlider( "MaxMembersInTerritory", "Max Members In Territory", "", "", 0.0, 50.0, 1.0);
		CreateSlider( "MaxTerritoryPerPlayer", "Max Territory Per Player", "", "", 0.0, 50.0, 1.0);
	}

	override int GetRPCMin()
	{
		return ExpansionCOTTerritoryModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionCOTTerritoryModuleRPC.COUNT;
	}

	override int GetRPCUpdate()
	{
		return ExpansionCOTTerritoryModuleRPC.Update;
	}

	override void OnSend(  ExpansionSettingBase setting )
	{
		if ( GetGame().IsClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			setting.OnSend( rpc );
			rpc.Send( NULL, ExpansionCOTTerritoryModuleRPC.Update, true, NULL );
		} else if ( IsMissionOffline() )
		{
			GetSettingsInstance().Update( setting );
		}
	}

	override void OnSend_RPC( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( !IsMissionHost() )
			return;

		ExpansionTerritorySettings setting = new ExpansionTerritorySettings();
		if ( !setting.OnRecieve( ctx ) )
			return;

		JMPlayerInstance inst;
		if ( !GetPermissionsManager().HasPermission( "Expansion.Territory.Update", senderRPC, inst ) )
			return;

		UpdateServer( setting, inst );
	}
};