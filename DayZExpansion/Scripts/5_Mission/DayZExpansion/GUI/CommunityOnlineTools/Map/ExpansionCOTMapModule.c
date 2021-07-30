/**
 * ExpansionCOTMapModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCOTMapModule: ExpansionCOTModuleBase
{
	void ExpansionCOTMapModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.Map.View" );
		GetPermissionsManager().RegisterPermission( "Expansion.Map.Update" );
	}

	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.Map.View" );
	}

	override string GetLayoutRoot()
	{
		return "DayZExpansion/GUI/layouts/COT/map/form.layout";
	}

	override string GetTitle()
	{
		return "[EX] Map Settings";
	}

	override string GetIconName()
	{
		return "M";
	}

	override bool ImageIsIcon()
	{
		return false;
	}

	override string GetWebhookTitle()
	{
		return "Expansion Map Settings";
	}

	override ExpansionSettingBase GetSettingsInstance()
	{
		return GetExpansionSettings().GetMap();
	}

	override void OnRegisterSettings()
	{
		CreateToggle( "EnableMap", "Enable Expansion Map", "", "" );
		CreateToggle( "UseMapOnMapItem", "Use Expansion Map On Map Item", "", "" );
		
		array< string > lamp_types = { "Disabled", "Enabled", "Require Compass" };
		CreateEnum( "ShowPlayerPosition", lamp_types, "Show Player Position", "", "" );
		CreateToggle( "ShowMapStats", "Show Map Stats (XYZ)", "", "" );

		CreateToggle( "NeedMapItemForKeyBinding", "Require Map Item", "", "" );
		CreateToggle( "CanOpenMapWithKeyBinding", "Can Open Map With Key Binding", "", "" );

		CreateToggle( "EnableHUDGPS", "Enable HUD GPS", "", "" );
		CreateToggle( "NeedGPSItemForKeyBinding", "Require GPS Item", "", "" );
		
		CreateToggle( "NeedPenItemForCreateMarker", "Need Pen Item For Create Marker", "", "" );
		CreateToggle( "NeedGPSItemForCreateMarker", "Need GPS Item For Create Marker", "", "" );
		CreateToggle( "CanCreateMarker", "Can Create Marker", "", "" );
		CreateToggle( "CanCreate3DMarker", "Can Create 3D Marker", "", "" );
		CreateToggle( "ShowDistanceOnPersonalMarkers", "Show Distance On Personal Markers", "", "" );		

		CreateToggle( "EnableServerMarkers", "Enable Server Markers", "", "" );
		CreateToggle( "ShowNameOnServerMarkers", "Show Name Of Server Markers", "", "" );
		CreateToggle( "ShowDistanceOnServerMarkers", "Show Distance Of Server Markers", "", "" );
		
		CreateToggle( "EnableHUDCompass", "Enable/Disable compass HUD.", "", "" );
		CreateToggle( "NeedCompassItemForHUDCompass", "Requires compass item to display compass HUD", "", "" );
		CreateToggle( "NeedGPSItemForHUDCompass", "Requires GPS item to display compass HUD", "", "" );
	}

	override int GetRPCMin()
	{
		return ExpansionCOTMapModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionCOTMapModuleRPC.COUNT;
	}

	override int GetRPCUpdate()
	{
		return ExpansionCOTMapModuleRPC.Update;
	}

	override void OnSend(  ExpansionSettingBase setting )
	{
		if ( GetGame().IsClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			setting.OnSend( rpc );
			rpc.Send( NULL, ExpansionCOTMapModuleRPC.Update, true, NULL );
		} else if ( IsMissionOffline() )
		{
			GetSettingsInstance().Update( setting );
		}
	}

	override void OnSend_RPC( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( !IsMissionHost() )
			return;

		ExpansionMapSettings setting = new ExpansionMapSettings();
		if ( !setting.OnRecieve( ctx ) )
			return;

		JMPlayerInstance inst;
		if ( !GetPermissionsManager().HasPermission( "Expansion.Map.Update", senderRPC, inst ) )
			return;

		UpdateServer( setting, inst );
	}
};