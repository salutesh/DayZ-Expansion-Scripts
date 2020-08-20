class ExpansionCOTNotificationModule: ExpansionCOTModuleBase
{
	void ExpansionCOTNotificationModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.Notification.View" );
		GetPermissionsManager().RegisterPermission( "Expansion.Notification.Update" );
	}

	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.Notification.View" );
	}

	override string GetLayoutRoot()
	{
		return "DayZExpansion/GUI/layouts/COT/notification/form.layout";
	}

	override string GetTitle()
	{
		return "[EX] Notification Settings";
	}

	override string GetIconName()
	{
		return "N";
	}

	override bool ImageIsIcon()
	{
		return false;
	}

	override string GetWebhookTitle()
	{
		return "Expansion Notification Settings";
	}

	override ref ExpansionSettingBase GetSettingsInstance()
	{
		return GetExpansionSettings().GetNotification();
	}

	override void OnRegisterSettings()
	{
		CreateToggle( "EnableNotification", "Enable Notification", "", "" );
		CreateToggle( "ShowPlayerJoinServer", "Show Player Join Server", "", "" );
		CreateToggle( "ShowPlayerLeftServer", "Show Player Left Server", "", "" );
		CreateToggle( "ShowAirdropStarted", "Show Airdrop Started", "", "" );
		CreateToggle( "ShowAirdropClosingOn", "Show Airdrop Closing On", "", "" );
		CreateToggle( "ShowAirdropDropped", "Show Airdrop Dropped", "", "" );
		CreateToggle( "ShowPlayerAirdropStarted", "Show Player Airdrop Started", "", "" );
		CreateToggle( "ShowPlayerAirdropClosingOn", "Show Player Airdrop Closing On", "", "" );
		CreateToggle( "ShowPlayerAirdropDropped", "Show Player Airdrop Dropped", "", "" );
		CreateToggle( "ShowTerritoryNotifications", "Show Territory Notifications", "", "" );
		CreateToggle( "EnableKillFeed", "Enable Kill-Feed", "", "" );
		CreateToggle( "EnableKillFeedDiscordMsg", "Enable Kill-Feed Discord Messages", "", "" );
	}

	override int GetRPCMin()
	{
		return ExpansionCOTNotificationModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionCOTNotificationModuleRPC.COUNT;
	}

	override int GetRPCUpdate()
	{
		return ExpansionCOTNotificationModuleRPC.Update;
	}

	override void OnSend( ref ExpansionSettingBase setting )
	{
		if ( GetGame().IsClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			setting.OnSend( rpc );
			rpc.Send( NULL, ExpansionCOTNotificationModuleRPC.Update, true, NULL );
		} else if ( IsMissionOffline() )
		{
			GetSettingsInstance().Update( setting );
		}
	}

	override void OnSend_RPC( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( !IsMissionHost() )
			return;

		ref ExpansionNotificationSettings setting = new ExpansionNotificationSettings();
		if ( !setting.OnRecieve( ctx ) )
			return;

		JMPlayerInstance inst;
		if ( !GetPermissionsManager().HasPermission( "Expansion.Notification.Update", senderRPC, inst ) )
			return;

		UpdateServer( setting, inst );
	}
};