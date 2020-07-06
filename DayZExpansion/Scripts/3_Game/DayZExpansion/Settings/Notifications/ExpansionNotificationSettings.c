/**
 * ExpansionNotificationSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionNotificationSettings
 * @brief		Notification settings class
 **/
class ExpansionNotificationSettings: ExpansionSettingBase
{
	bool EnableNotification;

	bool ShowPlayerJoinServer;
	ExpansionAnnouncementType JoinMessageType;
	bool ShowPlayerLeftServer;
	ExpansionAnnouncementType LeftMessageType;
	
	bool ShowAirdropStarted;
	bool ShowAirdropClosingOn;
	bool ShowAirdropDropped;

	bool ShowPlayerAirdropStarted;
	bool ShowPlayerAirdropClosingOn;
	bool ShowPlayerAirdropDropped;
	
	bool ShowTerritoryNotifications;				//! Show the notifications when entering or leaving territory.

	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionNotificationSettings()
	{
	}
	
	// ------------------------------------------------------------
	override void HandleRPC( ref ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationSettings::HandleRPC - Start");
		#endif
		ExpansionNotificationSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionNotificationSettings::HandleRPC setting");
			return;
		}

		CopyInternal( setting );

		m_IsLoaded = true;

		ExpansionSettings.SI_Notification.Invoke();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationSettings::HandleRPC - End");
		#endif
	}
	
	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationSettings::Send - Start");
		#endif
		
		if ( !IsMissionHost() )
		{
			return 0;
		}
		
		ref ExpansionNotificationSettings thisSetting = this;
		
		ScriptRPC rpc = new ScriptRPC;
		rpc.Write( thisSetting );
		rpc.Send( null, ExpansionSettingsRPC.Notification, true, identity );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationSettings::Send - End and return");
		#endif
		return 0;
	}

	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
		ExpansionNotificationSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}

	// ------------------------------------------------------------
	private void CopyInternal( ref ExpansionNotificationSettings s )
	{
		ShowPlayerJoinServer = s.ShowPlayerJoinServer;
		JoinMessageType = s.JoinMessageType;
		ShowPlayerLeftServer = s.ShowPlayerLeftServer;
		LeftMessageType = s.LeftMessageType;

		ShowAirdropStarted = s.ShowAirdropStarted;
		ShowAirdropClosingOn = s.ShowAirdropClosingOn;
		ShowAirdropDropped = s.ShowAirdropDropped;

		ShowPlayerAirdropStarted = s.ShowPlayerAirdropStarted;
		ShowPlayerAirdropClosingOn = s.ShowPlayerAirdropClosingOn;
		ShowPlayerAirdropDropped = s.ShowPlayerAirdropDropped;
	}
	
	// ------------------------------------------------------------
	override bool IsLoaded()
	{
		return m_IsLoaded;
	}

	// ------------------------------------------------------------
	override void Unload()
	{
		m_IsLoaded = false;
	}

	// ------------------------------------------------------------
	override bool OnLoad()
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationSettings::Load - Start");
		#endif
		
		m_IsLoaded = true;
		
		if ( FileExist( EXPANSION_NOTIFICATION_SETTINGS ) )
		{
			JsonFileLoader<ExpansionNotificationSettings>.JsonLoadFile( EXPANSION_NOTIFICATION_SETTINGS, this );
	
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionNotificationSettings::Load - End - Loaded");
			#endif

			return true;
		}

		Defaults();
		Save();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationSettings::Load - End - Not Loaded");
		#endif
		return false;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationSettings::Save - Start");
		#endif
		
		if ( IsMissionHost() )
		{
			JsonFileLoader<ExpansionNotificationSettings>.JsonSaveFile( EXPANSION_NOTIFICATION_SETTINGS, this );
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionNotificationSettings::Save - Settings saved!");
			#endif

			return true;
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationSettings::Save - End");
		#endif
		return false;
	}
	
	// ------------------------------------------------------------
	override void Update( ExpansionSettingBase setting )
	{
		super.Update( setting );

		ExpansionSettings.SI_General.Invoke();
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionNotificationSettings] Loading default settings");
		#endif
		
		EnableNotification = true;

		ShowPlayerJoinServer = true;
		JoinMessageType = ExpansionAnnouncementType.NOTIFICATION;
		ShowPlayerLeftServer = true;
		LeftMessageType = ExpansionAnnouncementType.NOTIFICATION;

		ShowAirdropStarted = true;
		ShowAirdropClosingOn = true;
		ShowAirdropDropped = true;

		ShowPlayerAirdropStarted = true;
		ShowPlayerAirdropClosingOn = true;
		ShowPlayerAirdropDropped = true;
		
		ShowTerritoryNotifications = true;
	}
}