/**
 * ExpansionCOTAirdropModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCOTAirdropModule: ExpansionCOTModuleBase
{
	void ExpansionCOTAirdropModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.Airdrop.View" );
		GetPermissionsManager().RegisterPermission( "Expansion.Airdrop.Update" );
	}

	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.Airdrop.View" );
	}

	override string GetLayoutRoot()
	{
		return "DayZExpansion/GUI/layouts/COT/airdrop/form.layout";
	}

	override string GetTitle()
	{
		return "[EX] Airdrop Settings";
	}

	override string GetIconName()
	{
		return "A";
	}

	override bool ImageIsIcon()
	{
		return false;
	}

	override string GetWebhookTitle()
	{
		return "Expansion Airdrop Settings";
	}

	override ref ExpansionSettingBase GetSettingsInstance()
	{
		return GetExpansionSettings().GetAirdrop();
	}

	override void OnRegisterSettings()
	{
		CreateToggle( "ServerMarkerOnDropLocation", "Server Marker On Drop Location", "", "" );
		CreateToggle( "Server3DMarkerOnDropLocation", "Server 3D Marker On Drop Location", "", "" );
		CreateToggle( "ShowAirdropTypeOnMarker", "Show Airdrop Type On Marker", "", "" );
		CreateToggle( "HeightIsRelativeToGroundLevel", "Height Is Relative To Ground Level", "", "" );
		
		CreateSlider( "Height", "Height", "", "", 100.0, 600.0, 10.0);
		CreateSlider( "FollowTerrainFraction", "Terrain Follow", "", "", 0.1, 2.0, 0.1);
		CreateSlider( "Speed", "Speed", "", "", 50.0, 2000.0, 25.0);
		CreateSlider( "Radius", "Radius", "", "", 1.0, 100.0, 1.0);
		CreateSlider( "InfectedSpawnRadius", "Zeds Spawn Radius", "", "", 10.0, 100.0, 10.0);
		//! Doesn't work
		//CreateSlider( "ItemCount", "Item Count", "", "", 5.0, 75.0, 5.0);
	}

	override int GetRPCMin()
	{
		return ExpansionCOTAirDropModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionCOTAirDropModuleRPC.COUNT;
	}

	override int GetRPCUpdate()
	{
		return ExpansionCOTAirDropModuleRPC.Update;
	}

	override void OnSend( ref ExpansionSettingBase setting )
	{
		if ( GetGame().IsClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			setting.OnSend( rpc );
			rpc.Send( NULL, ExpansionCOTAirDropModuleRPC.Update, true, NULL );
		} else if ( IsMissionOffline() )
		{
			GetSettingsInstance().Update( setting );
		}
	}

	override void OnSend_RPC( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( !IsMissionHost() )
			return;

		ref ExpansionAirdropSettings setting = new ExpansionAirdropSettings();
		if ( !setting.OnRecieve( ctx ) )
			return;

		JMPlayerInstance inst;
		if ( !GetPermissionsManager().HasPermission( "Expansion.Airdrop.Update", senderRPC, inst ) )
			return;

		UpdateServer( setting, inst );
	}
};