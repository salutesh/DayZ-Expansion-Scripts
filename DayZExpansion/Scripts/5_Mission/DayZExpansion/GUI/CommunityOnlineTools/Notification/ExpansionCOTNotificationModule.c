/**
 * ExpansionCOTNotificationModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

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

	override ExpansionSettingBase GetSettingsInstance()
	{
		return GetExpansionSettings().GetNotification();
	}

	override void OnRegisterSettings()
	{
		CreateToggle( "EnableNotification", "Enable Notification", "", "" );
		
		CreateToggle( "ShowPlayerJoinServer", "Show Player Join Server", "", "" );
		CreateEnum( "JoinMessageType", ExpansionAnnouncementType, "Join Server Type", "", "" );
		CreateToggle( "ShowPlayerLeftServer", "Show Player Left Server", "", "" );
		CreateEnum( "LeftMessageType", ExpansionAnnouncementType, "Leave Server Type", "", "" );
		
		CreateToggle( "ShowAirdropStarted", "Show Airdrop Started", "", "" );
		CreateToggle( "ShowAirdropClosingOn", "Show Airdrop Closing On", "", "" );
		CreateToggle( "ShowAirdropDropped", "Show Airdrop Dropped", "", "" );
		CreateToggle( "ShowAirdropEnded", "Show Airdrop Ended", "", "" );
		
		CreateToggle( "ShowPlayerAirdropStarted", "Show Player Airdrop Started", "", "" );
		CreateToggle( "ShowPlayerAirdropClosingOn", "Show Player Airdrop Closing On", "", "" );
		CreateToggle( "ShowPlayerAirdropDropped", "Show Player Airdrop Dropped", "", "" );

		CreateToggle( "ShowTerritoryNotifications", "Show Territory Notifications", "", "" );

		CreateToggle( "EnableKillFeed", "Enable Kill-Feed", "", "" );
		CreateEnum( "KillFeedMessageType", ExpansionAnnouncementType, "Kill-Feed Type", "", "" );
		CreateToggle( "KillFeedFall", "Kill-Feed: Fall", "", "" );
		CreateToggle( "KillFeedCarHitDriver", "Kill-Feed: Car Hit Driver", "", "" );
		CreateToggle( "KillFeedCarHitNoDriver", "Kill-Feed: Car Hit No Driver", "", "" );
		CreateToggle( "KillFeedCarCrash", "Kill-Feed: Car Crash", "", "" );
		CreateToggle( "KillFeedCarCrashCrew", "Kill-Feed: Car Crash Crew", "", "" );
		CreateToggle( "KillFeedHeliHitDriver", "Kill-Feed: Heli Hit Driver", "", "" );
		CreateToggle( "KillFeedHeliHitNoDriver", "Kill-Feed: Heli Hit No Driver", "", "" );
		CreateToggle( "KillFeedHeliCrash", "Kill-Feed: Heli Crash", "", "" );
		CreateToggle( "KillFeedHeliCrashCrew", "Kill-Feed: Heli Crash Crew", "", "" );
		CreateToggle( "KillFeedBarbedWire", "Kill-Feed: Barbed Wire", "", "" );
		CreateToggle( "KillFeedFire", "Kill-Feed: Fire", "", "" );
		CreateToggle( "KillFeedWeaponExplosion", "Kill-Feed: Weapon Explosion", "", "" );
		CreateToggle( "KillFeedDehydration", "Kill-Feed: Dehydration", "", "" );
		CreateToggle( "KillFeedStarvation", "Kill-Feed: Starvation", "", "" );
		CreateToggle( "KillFeedBleeding", "Kill-Feed: Bleeding", "", "" );
		CreateToggle( "KillFeedSuicide", "Kill-Feed: Suicide", "", "" );
		CreateToggle( "KillFeedWeapon", "Kill-Feed: Weapon", "", "" );
		CreateToggle( "KillFeedMeleeWeapon", "Kill-Feed: Melee Weapon", "", "" );
		CreateToggle( "KillFeedBarehands", "Kill-Feed: Barehands", "", "" );
		CreateToggle( "KillFeedInfected", "Kill-Feed: Infected", "", "" );
		CreateToggle( "KillFeedAnimal", "Kill-Feed: Animal", "", "" );
		CreateToggle( "KillFeedKilledUnknown", "Kill-Feed: Killed Unknown", "", "" );
		CreateToggle( "KillFeedDiedUnknown", "Kill-Feed: Died Unknown", "", "" );

		//CreateToggle( "EnableKillFeedDiscordMsg", "Enable Kill-Feed Discord Messages", "", "" );
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

	override void OnSend(  ExpansionSettingBase setting )
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

	override void OnSend_RPC( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( !IsMissionHost() )
			return;

		ExpansionNotificationSettings setting = new ExpansionNotificationSettings();
		if ( !setting.OnRecieve( ctx ) )
			return;

		JMPlayerInstance inst;
		if ( !GetPermissionsManager().HasPermission( "Expansion.Notification.Update", senderRPC, inst ) )
			return;

		UpdateServer( setting, inst );
	}
};