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

	override ref ExpansionSettingBase GetSettingsInstance()
	{
		return GetExpansionSettings().GetGeneral();
	}

	override void OnRegisterSettings()
	{
		CreateToggle( "PlayerLocationNotifier", "Player Location", "", "" );
		CreateToggle( "EnableGlobalChat", "Globalchat", "", "" );
		CreateToggle( "EnableGravecross", "Gravecross", "", "" );
		CreateToggle( "GravecrossDeleteBody", "Delete Body On Gravecross", "", "" );
		CreateSlider( "GravecrossTimeThreshold", "Gravecross Threshold", "", "", 0.0, 2000.0, 60.0 );
		array< string > lamp_types = { "Disable", "Generators", "Always On", "Always On Everywhere" };
		CreateEnum( "EnableLamps", lamp_types, "Lamps", "", "" );
		CreateToggle( "EnableGenerators", "Generators", "", "" );
		CreateToggle( "EnableLighthouses", "Lighthouses", "", "" );
		CreateToggle( "EnableHUDGPS", "GPS on HUD", "", "" );
		CreateToggle( "NeedGPSItemForKeyBinding", "GPS Item Keybinding", "", "" );
		CreateToggle( "NeedMapItemForKeyBinding", "Map Item Keybinding", "", "" );
		CreateToggle( "EnableHUDNightvisionOverlay", "Nightvision HUD Overlay", "", "" );
		CreateToggle( "DisableMagicCrosshair", "DayZ SA Alpha Aiming", "", "" );
		CreateToggle( "EnablePlayerTags", "Player Tags", "", "" );
		CreateSlider( "PlayerTagViewRange", "Player Tag View Range", "", "", 0.0, 100.0, 1.0 );
		CreateToggle( "EnablePlayerList", "Player List", "", "" );
		CreateToggle( "EnableAutoRun", "Auto Run", "", "" );
		CreateToggle( "UnlimitedStamina", "Unlimited Stamina", "", "" );
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

		if ( !GetPermissionsManager().HasPermission( "Expansion.General.Update", senderRPC ) )
			return;

		UpdateServer( setting );
	}
};