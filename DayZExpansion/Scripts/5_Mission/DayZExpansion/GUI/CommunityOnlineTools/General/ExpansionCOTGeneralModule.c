/**
 * ExpansionCOTGeneralModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCOTGeneralModule: ExpansionCOTModuleBase
{
	void ExpansionCOTGeneralModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.General.View" );
		GetPermissionsManager().RegisterPermission( "Expansion.General.Update" );
	}

	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.General.View" );
	}

	override string GetLayoutRoot()
	{
		return "DayZExpansion/GUI/layouts/COT/general/form.layout";
	}

	override string GetTitle()
	{
		return "[EX] General Settings";
	}

	override string GetIconName()
	{
		return "G";
	}

	override bool ImageIsIcon()
	{
		return false;
	}

	override string GetWebhookTitle()
	{
		return "Expansion General Settings";
	}

	override ref ExpansionSettingBase GetSettingsInstance()
	{
		return GetExpansionSettings().GetGeneral();
	}

	override void OnRegisterSettings()
	{
		CreateToggle( "PlayerLocationNotifier", "Player Location", "", "" );
		CreateToggle( "EnableGlobalChat", "Globalchat", "", "" );
		CreateToggle( "EnablePartyChat", "Partychat", "", "" );
		CreateToggle( "EnableTransportChat", "TransportChat", "", "" );
		CreateToggle( "EnableGravecross", "Gravecross", "", "" );
		CreateToggle( "GravecrossDeleteBody", "Delete Body On Gravecross", "", "" );
		CreateSlider( "GravecrossTimeThreshold", "Gravecross Threshold", "", "", 0.0, 2000.0, 60.0 );
		array< string > lamp_types = { "Disable", "Generators", "Always On", "Always On Everywhere" };
		CreateEnum( "EnableLamps", lamp_types, "Lamps", "", "" );
		CreateToggle( "EnableGenerators", "Generators", "", "" );
		CreateToggle( "EnableLighthouses", "Lighthouses", "", "" );
		CreateToggle( "EnableHUDGPS", "GPS on HUD", "", "" );
		CreateToggle( "NeedGPSItemForKeyBinding", "Need GPS Item Keybinding", "", "" );
		CreateToggle( "NeedMapItemForKeyBinding", "Need Map Item Keybinding", "", "" );
		CreateToggle( "EnableHUDNightvisionOverlay", "Nightvision HUD Overlay", "", "" );
		CreateToggle( "DisableMagicCrosshair", "DayZ SA Alpha Aiming", "", "" );
		CreateToggle( "EnablePlayerTags", "Player Tags", "", "" );
		CreateSlider( "PlayerTagViewRange", "Player Tag View Range", "", "", 0.0, 100.0, 1.0 );
		CreateToggle( "EnablePlayerList", "Player List", "", "" );
		CreateToggle( "EnableAutoRun", "Auto Run", "", "" );
		CreateToggle( "UnlimitedStamina", "Unlimited Stamina", "", "" );
		CreateToggle( "UseCustomMappingModule", "Use CustomMapping Module", "", "" );
		CreateToggle( "Mapping.BuildingInteriors", "Building Interiors", "", "" );
		CreateToggle( "Mapping.BuildingIvys", "Building Ivys", "", "" );
	}

	override int GetRPCMin()
	{
		return ExpansionCOTGeneralModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionCOTGeneralModuleRPC.COUNT;
	}

	override int GetRPCUpdate()
	{
		return ExpansionCOTGeneralModuleRPC.Update;
	}

	override void OnSend( ref ExpansionSettingBase setting )
	{
		if ( GetGame().IsClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			setting.OnSend( rpc );
			rpc.Send( NULL, ExpansionCOTGeneralModuleRPC.Update, true, NULL );
		} else if ( IsMissionOffline() )
		{
			GetSettingsInstance().Update( setting );
		}
	}

	override void OnSend_RPC( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( !IsMissionHost() )
			return;

		ref ExpansionGeneralSettings setting = new ExpansionGeneralSettings();
		if ( !setting.OnRecieve( ctx ) )
			return;

		JMPlayerInstance inst;
		if ( !GetPermissionsManager().HasPermission( "Expansion.General.Update", senderRPC, inst ) )
			return;

		UpdateServer( setting, inst );
	}
};