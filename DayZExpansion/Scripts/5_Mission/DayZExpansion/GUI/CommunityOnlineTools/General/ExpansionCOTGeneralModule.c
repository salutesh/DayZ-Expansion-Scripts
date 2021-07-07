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

		CreateToggle( "DisableShootToUnlock", "Disable Shooting Doors to unlock them", "", "" );
		
		CreateToggle( "EnableGravecross", "Gravecross", "", "" );
		CreateToggle( "GravecrossDeleteBody", "Delete Body On Gravecross", "", "" );
		//! Doesn't work
		//CreateSlider( "GravecrossTimeThreshold", "Gravecross Threshold", "", "", 0, 2000, 60 );

		CreateToggle( "Mapping.UseCustomMappingModule", "Use CustomMapping Module", "", "" );
		CreateToggle( "Mapping.BuildingInteriors", "Building Interiors", "", "" );
		CreateToggle( "Mapping.BuildingIvys", "Building Ivys", "", "" );

		array< string > lamp_types = { "Disable", "Generators", "Always On", "Always On Everywhere" };
		CreateEnum( "EnableLamps", lamp_types, "Lamps", "", "" );
		CreateToggle( "EnableGenerators", "Generators", "", "" );
		CreateToggle( "EnableLighthouses", "Lighthouses", "", "" );
		
		CreateToggle( "EnableHUDNightvisionOverlay", "Nightvision HUD Overlay", "", "" );
		CreateToggle( "DisableMagicCrosshair", "DayZ SA Alpha Aiming", "", "" );

		CreateToggle( "EnablePlayerTags", "Player Tags", "", "" );

		//! Doesn't work
		//CreateSlider( "PlayerTagViewRange", "Player Tag View Range", "", "", 1, 25, 1 );

		//CreateToggle( "EnablePlayerList", "Player List", "", "" );
		CreateToggle( "EnableAutoRun", "Auto Run", "", "" );
		CreateToggle( "UnlimitedStamina", "Unlimited Stamina", "", "" );
		
		CreateToggle( "UseDeathScreen", "Use Death Screen", "", "" );
		CreateToggle( "UseDeathScreenStatistics", "Use Death Screen Statistics", "", "" );
		CreateToggle( "UseNewsFeedInGameMenu", "Use News Feed In GameMenu", "", "" );
		
		//CreateInt( "SystemChatColor", "System chat color", "", "" );
		//CreateInt( "AdminChatColor", "Admin chat color", "", "" );
		//CreateInt( "GlobalChatColor", "Global chat color", "", "" );
		//CreateInt( "DirectChatColor", "Global chat color", "", "" );
		//CreateInt( "TransportChatColor", "Global chat color", "", "" );
		//CreateInt( "PartyChatColor", "Global chat color", "", "" );
		//CreateInt( "TransmitterChatColor", "Global chat color", "", "" );
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