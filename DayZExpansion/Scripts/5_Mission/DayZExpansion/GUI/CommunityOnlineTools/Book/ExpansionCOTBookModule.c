class ExpansionCOTBookModule: ExpansionCOTModuleBase
{
	void ExpansionCOTBookModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.Book.View" );
		GetPermissionsManager().RegisterPermission( "Expansion.Book.Update" );
	}

	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.Book.View" );
	}

	override string GetLayoutRoot()
	{
		return "DayZExpansion/GUI/layouts/COT/book/form.layout";
	}

	override string GetTitle()
	{
		return "[EX] Book Settings";
	}

	override string GetIconName()
	{
		return "B";
	}

	override bool ImageIsIcon()
	{
		return false;
	}

	override string GetWebhookTitle()
	{
		return "Expansion Book Settings";
	}

	override ref ExpansionSettingBase GetSettingsInstance()
	{
		return GetExpansionSettings().GetBook();
	}

	override void OnRegisterSettings()
	{		
		CreateToggle( "EnableBook", "EnableBook", "", "" );
		CreateToggle( "EnableStatusTab", "Enable Status Tab", "", "" );
		CreateToggle( "EnablePartyTab", "Enable Party Tab", "", "" );
		CreateToggle( "EnableServerInfoTab", "Enable Server Info Tab", "", "" );
		CreateToggle( "EnableServerRulesTab", "Enable Server Rules Tab", "", "" );
		CreateToggle( "EnableTerritoryTab", "Enable Territory Tab", "", "" );
		CreateToggle( "ShowServerSettings", "Show Server Settings", "", "" );
	}

	override int GetRPCMin()
	{
		return ExpansionCOTBookModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionCOTBookModuleRPC.COUNT;
	}

	override int GetRPCUpdate()
	{
		return ExpansionCOTBookModuleRPC.Update;
	}

	override void OnSend( ref ExpansionSettingBase setting )
	{
		if ( GetGame().IsClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			setting.OnSend( rpc );
			rpc.Send( NULL, ExpansionCOTBookModuleRPC.Update, true, NULL );
		} else if ( IsMissionOffline() )
		{
			GetSettingsInstance().Update( setting );
		}
	}

	override void OnSend_RPC( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( !IsMissionHost() )
			return;

		ref ExpansionBookSettings setting = new ExpansionBookSettings();
		if ( !setting.OnRecieve( ctx ) )
			return;

		JMPlayerInstance inst;
		if ( !GetPermissionsManager().HasPermission( "Expansion.Book.Update", senderRPC, inst ) )
			return;

		UpdateServer( setting, inst );
	}
};