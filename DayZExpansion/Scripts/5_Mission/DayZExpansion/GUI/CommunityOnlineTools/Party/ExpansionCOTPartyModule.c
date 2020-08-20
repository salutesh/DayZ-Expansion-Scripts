class ExpansionCOTPartyModule: ExpansionCOTModuleBase
{
	void ExpansionCOTPartyModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.Party.View" );
		GetPermissionsManager().RegisterPermission( "Expansion.Party.Update" );
	}

	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.Party.View" );
	}

	override string GetLayoutRoot()
	{
		return "DayZExpansion/GUI/layouts/COT/party/form.layout";
	}

	override string GetTitle()
	{
		return "[EX] Party Settings";
	}

	override string GetIconName()
	{
		return "P";
	}

	override bool ImageIsIcon()
	{
		return false;
	}

	override string GetWebhookTitle()
	{
		return "Expansion Party Settings";
	}

	override ref ExpansionSettingBase GetSettingsInstance()
	{
		return GetExpansionSettings().GetParty();
	}

	override void OnRegisterSettings()
	{
		CreateToggle( "EnableParties", "Enable Parties", "", "" );
		CreateToggle( "ShowPartyMembers3DMarkers", "Show Party Members 3D Markers", "", "" );
		CreateSlider( "DistanceForPartyMarkers", "Distance PM", "", "Distance For Party Markers", 5.0, 5000.0, 1.0);
		CreateToggle( "UseWholeMapForInviteList", "Use Whole Map For Invite List", "", "" );
		CreateToggle( "EnableQuickMarker", "Enable Quick Marker", "", "" );
		CreateToggle( "ShowDistanceUnderQuickMarkers", "Show Distance Under Quick Markers", "", "" );
		CreateToggle( "ShowNameOnQuickMarkers", "Show Name On Quick Markers", "", "" );
	}

	override int GetRPCMin()
	{
		return ExpansionCOTPartyModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionCOTPartyModuleRPC.COUNT;
	}

	override int GetRPCUpdate()
	{
		return ExpansionCOTPartyModuleRPC.Update;
	}

	override void OnSend( ref ExpansionSettingBase setting )
	{
		if ( GetGame().IsClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			setting.OnSend( rpc );
			rpc.Send( NULL, ExpansionCOTPartyModuleRPC.Update, true, NULL );
		} else if ( IsMissionOffline() )
		{
			GetSettingsInstance().Update( setting );
		}
	}

	override void OnSend_RPC( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( !IsMissionHost() )
			return;

		ref ExpansionPartySettings setting = new ExpansionPartySettings();
		if ( !setting.OnRecieve( ctx ) )
			return;

		JMPlayerInstance inst;
		if ( !GetPermissionsManager().HasPermission( "Expansion.Party.Update", senderRPC, inst ) )
			return;

		UpdateServer( setting, inst );
	}
};