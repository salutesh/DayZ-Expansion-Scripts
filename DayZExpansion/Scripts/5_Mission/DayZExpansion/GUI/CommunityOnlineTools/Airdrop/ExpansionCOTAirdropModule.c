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

	override ref ExpansionSettingBase GetSettingsInstance()
	{
		return GetExpansionSettings().GetAirdrop();
	}

	override void OnRegisterSettings()
	{
		CreateToggle( "ServerMarkerOnDropLocation", "Server Marker On Drop Location", "", "" );
		CreateToggle( "ShowAirdropTypeOnMarker", "Show Airdrop Type On Marker", "", "" );
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

		if ( !GetPermissionsManager().HasPermission( "Expansion.Airdrop.Update", senderRPC ) )
			return;

		UpdateServer( setting );
	}
};