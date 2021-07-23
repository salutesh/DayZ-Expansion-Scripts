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

/**@class		ExpansionNotificationSettingsBase
 * @brief		Map settings base class
 **/
class ExpansionNotificationSettingsBase: ExpansionSettingBase
{
	bool EnableNotification;
	bool ShowPlayerJoinServer;
	ExpansionAnnouncementType JoinMessageType;
	bool ShowPlayerLeftServer;
	ExpansionAnnouncementType LeftMessageType;
	bool ShowAirdropStarted;
	bool ShowAirdropClosingOn;
	bool ShowAirdropDropped;
	bool ShowAirdropEnded;
	bool ShowPlayerAirdropStarted;
	bool ShowPlayerAirdropClosingOn;
	bool ShowPlayerAirdropDropped;
	bool ShowTerritoryNotifications;				//! Show the notifications when entering or leaving territory.
	bool EnableKillFeed;
	ExpansionAnnouncementType KillFeedMessageType;
	bool KillFeedFall;
	bool KillFeedCarHitDriver;
	bool KillFeedCarHitNoDriver;
	bool KillFeedCarCrash;
	bool KillFeedCarCrashCrew;
	bool KillFeedHeliHitDriver;
	bool KillFeedHeliHitNoDriver;
	bool KillFeedHeliCrash;
	bool KillFeedHeliCrashCrew;
	bool KillFeedBoatCrash;
	bool KillFeedBoatCrashCrew;
	bool KillFeedBarbedWire;
	bool KillFeedFire;
	bool KillFeedWeaponExplosion;
	bool KillFeedDehydration;
	bool KillFeedStarvation;
	bool KillFeedBleeding;
	bool KillFeedSuicide;
	bool KillFeedWeapon;
	bool KillFeedMeleeWeapon;
	bool KillFeedBarehands;
	bool KillFeedInfected;
	bool KillFeedAnimal;
	bool KillFeedKilledUnknown;
	bool KillFeedDiedUnknown;
	bool EnableKillFeedDiscordMsg;
}

/**@class		ExpansionNotificationSettings
 * @brief		Notification settings class
 **/
class ExpansionNotificationSettings: ExpansionNotificationSettingsBase
{	
	static const int VERSION = 2;
	
	bool ShowVictimOnKillFeed;
	bool ShowDistanceOnKillFeed;
	bool ShowKillerOnKillFeed;
	bool ShowWeaponOnKillFeed;
	
	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionNotificationSettings()
	{
	}
	
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationSettings::OnRecieve - Start");
		#endif
		ExpansionNotificationSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionNotificationSettings::OnRecieve setting");
			return false;
		}

		CopyInternal( setting );

		m_IsLoaded = true;

		ExpansionSettings.SI_Notification.Invoke();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationSettings::OnRecieve - End");
		#endif

		return true;
	}
	
	override void OnSend( ParamsWriteContext ctx )
	{
		ref ExpansionNotificationSettings thisSetting = this;

		ctx.Write( thisSetting );
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
		
		ScriptRPC rpc = new ScriptRPC;
		OnSend( rpc );
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
		ShowDistanceOnKillFeed = s.ShowDistanceOnKillFeed;
		ShowVictimOnKillFeed = s.ShowVictimOnKillFeed;
		ShowKillerOnKillFeed = s.ShowKillerOnKillFeed;
		ShowWeaponOnKillFeed = s.ShowWeaponOnKillFeed;
		
		ExpansionNotificationSettingsBase sb = s;
		CopyInternal( sb );
	}
	
	// ------------------------------------------------------------
	private void CopyInternal( ref ExpansionNotificationSettingsBase s )
	{
		EnableNotification = s.EnableNotification;
		ShowPlayerJoinServer = s.ShowPlayerJoinServer;
		JoinMessageType = s.JoinMessageType;
		ShowPlayerLeftServer = s.ShowPlayerLeftServer;
		LeftMessageType = s.LeftMessageType;

		ShowAirdropStarted = s.ShowAirdropStarted;
		ShowAirdropClosingOn = s.ShowAirdropClosingOn;
		ShowAirdropDropped = s.ShowAirdropDropped;
		ShowAirdropEnded = s.ShowAirdropEnded;
		
		ShowPlayerAirdropStarted = s.ShowPlayerAirdropStarted;
		ShowPlayerAirdropClosingOn = s.ShowPlayerAirdropClosingOn;
		ShowPlayerAirdropDropped = s.ShowPlayerAirdropDropped;
		
		ShowTerritoryNotifications = s.ShowTerritoryNotifications;
		
		EnableKillFeed = s.EnableKillFeed;
		KillFeedMessageType = s.KillFeedMessageType;
		KillFeedFall = s.KillFeedFall;
		KillFeedCarHitDriver = s.KillFeedCarHitDriver;
		KillFeedCarHitNoDriver = s.KillFeedCarHitNoDriver;
		KillFeedCarCrash = s.KillFeedCarCrash;
		KillFeedCarCrashCrew = s.KillFeedCarCrashCrew;
		KillFeedHeliHitDriver = s.KillFeedHeliHitDriver;
		KillFeedHeliHitNoDriver = s.KillFeedHeliHitNoDriver;
		KillFeedHeliCrash = s.KillFeedHeliCrash;
		KillFeedHeliCrashCrew = s.KillFeedHeliCrashCrew;
		KillFeedBoatCrash = s.KillFeedBoatCrash;
		KillFeedBoatCrashCrew = s.KillFeedBoatCrashCrew;
		KillFeedBarbedWire = s.KillFeedBarbedWire;
		KillFeedFire = s.KillFeedFire;
		KillFeedWeaponExplosion = s.KillFeedWeaponExplosion;
		KillFeedDehydration = s.KillFeedDehydration;
		KillFeedStarvation = s.KillFeedStarvation;
		KillFeedBleeding = s.KillFeedBleeding;
		KillFeedSuicide = s.KillFeedSuicide;
		KillFeedWeapon = s.KillFeedWeapon;
		KillFeedMeleeWeapon = s.KillFeedMeleeWeapon;
		KillFeedBarehands = s.KillFeedBarehands;
		KillFeedInfected = s.KillFeedInfected;
		KillFeedAnimal = s.KillFeedAnimal;
		KillFeedKilledUnknown = s.KillFeedKilledUnknown;
		KillFeedDiedUnknown = s.KillFeedDiedUnknown;
		EnableKillFeedDiscordMsg = s.EnableKillFeedDiscordMsg;
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

		bool save;

		bool notificationSettingsExist = FileExist(EXPANSION_NOTIFICATION_SETTINGS);

		if (notificationSettingsExist)
		{
			ExpansionNotificationSettings settingsDefault = new ExpansionNotificationSettings;
			settingsDefault.Defaults();

			ExpansionNotificationSettingsBase settingsBase;

			JsonFileLoader<ExpansionNotificationSettingsBase>.JsonLoadFile(EXPANSION_NOTIFICATION_SETTINGS, settingsBase);

			if (settingsBase.m_Version < VERSION)
			{
				if (settingsBase.m_Version < 2)
				{
					EXPrint("[ExpansionNotificationSettings] Load - Converting v1 \"" + EXPANSION_NOTIFICATION_SETTINGS + "\" to v" + VERSION);
					
					//! New with v2
					CopyInternal(settingsDefault);
				}
				//! Copy over old settings that haven't changed
				CopyInternal(settingsBase);

				m_Version = VERSION;
				save = true;
			}
			else
			{
				JsonFileLoader<ExpansionNotificationSettings>.JsonLoadFile(EXPANSION_NOTIFICATION_SETTINGS, this);
			}
		}
		else
		{
			Defaults();
			save = true;
		}
		
		if (save)
			Save();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationSettings::Load - End - Loaded: " + notificationSettingsExist);
		#endif
		
		return notificationSettingsExist;
	}
	
	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionNotificationSettings] Saving settings");

		JsonFileLoader<ExpansionNotificationSettings>.JsonSaveFile( EXPANSION_NOTIFICATION_SETTINGS, this );

		return true;
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
		Print("[ExpansionNotificationSettings] Loading default settings");
		
		m_Version = VERSION;
		
		EnableNotification = true;

		ShowPlayerJoinServer = true;
		JoinMessageType = ExpansionAnnouncementType.NOTIFICATION;
		ShowPlayerLeftServer = true;
		LeftMessageType = ExpansionAnnouncementType.NOTIFICATION;

		ShowAirdropStarted = true;
		ShowAirdropClosingOn = true;
		ShowAirdropDropped = true;
		ShowAirdropEnded = true;
		
		ShowPlayerAirdropStarted = true;
		ShowPlayerAirdropClosingOn = true;
		ShowPlayerAirdropDropped = true;
		
		ShowTerritoryNotifications = true;
		
		EnableKillFeed = true;
		KillFeedMessageType = ExpansionAnnouncementType.NOTIFICATION;

		ShowDistanceOnKillFeed = true;
		ShowVictimOnKillFeed = true;
		ShowKillerOnKillFeed = true;
		ShowWeaponOnKillFeed = true;
		
		KillFeedFall = true;
		KillFeedCarHitDriver = true;
		KillFeedCarHitNoDriver = true;
		KillFeedCarCrash = true;
		KillFeedCarCrashCrew = true;
		KillFeedHeliHitDriver = true;
		KillFeedHeliHitNoDriver = true;
		KillFeedHeliCrash = true;
		KillFeedHeliCrashCrew = true;
		KillFeedBarbedWire = true;
		KillFeedFire = true;
		KillFeedWeaponExplosion = true;
		KillFeedDehydration = true;
		KillFeedStarvation = true;
		KillFeedBleeding = true;
		KillFeedSuicide = true;
		KillFeedWeapon = true;
		KillFeedMeleeWeapon = true;
		KillFeedBarehands = true;
		KillFeedInfected = true;
		KillFeedAnimal = true;
		KillFeedKilledUnknown = true;
		KillFeedDiedUnknown = true;

		EnableKillFeedDiscordMsg = false;
	}
	
	// ------------------------------------------------------------	
	override string SettingName()
	{
		return "Notification Settings";
	}
};