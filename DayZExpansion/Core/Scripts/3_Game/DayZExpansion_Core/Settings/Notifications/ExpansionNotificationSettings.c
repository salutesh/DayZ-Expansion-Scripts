/**
 * ExpansionNotificationSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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

#ifdef EXPANSIONMODMISSIONS
	bool ShowAirdropStarted;
	bool ShowAirdropClosingOn;
	bool ShowAirdropDropped;
	bool ShowAirdropEnded;

	bool ShowPlayerAirdropStarted;
	bool ShowPlayerAirdropClosingOn;
	bool ShowPlayerAirdropDropped;
	
#ifdef EXPANSION_MISSION_AI_ENABLE
	bool ShowAIMissionStarted;
	bool ShowAIMissionAction;
	bool ShowAIMissionEnded;
#endif
#endif

#ifdef EXPANSIONMODBASEBUILDING
	bool ShowTerritoryNotifications;				//! Show the notifications when entering or leaving territory.
#endif

#ifdef EXPANSIONMODKILLFEED
	bool EnableKillFeed;
	ExpansionAnnouncementType KillFeedMessageType;
	bool KillFeedFall;
	bool KillFeedCarHitDriver;
	bool KillFeedCarHitNoDriver;
	bool KillFeedCarCrash;
	bool KillFeedCarCrashCrew;
#ifdef EXPANSIONMODVEHICLE
	bool KillFeedHeliHitDriver;
	bool KillFeedHeliHitNoDriver;
	bool KillFeedHeliCrash;
	bool KillFeedHeliCrashCrew;
	bool KillFeedBoatHitDriver;
	bool KillFeedBoatHitNoDriver;
	bool KillFeedBoatCrash;
	bool KillFeedBoatCrashCrew;
	/*bool KillFeedPlaneHitDriver;
	bool KillFeedPlaneHitNoDriver;
	bool KillFeedBikeHitDriver;
	bool KillFeedBikeHitNoDriver;*/
#endif
	bool KillFeedBarbedWire;
	bool KillFeedFire;
	bool KillFeedWeaponExplosion;
	bool KillFeedDehydration;
	bool KillFeedStarvation;
	bool KillFeedBleeding;
	bool KillFeedStatusEffects;
	bool KillFeedSuicide;
	bool KillFeedWeapon;
	bool KillFeedMeleeWeapon;
	bool KillFeedBarehands;
	bool KillFeedInfected;
	bool KillFeedAnimal;
	bool KillFeedKilledUnknown;
	bool KillFeedDiedUnknown;
#ifdef JM_COT
	bool EnableKillFeedDiscordMsg;
#endif
#endif
}

/**@class		ExpansionNotificationSettings
 * @brief		Notification settings class
 **/
class ExpansionNotificationSettings: ExpansionNotificationSettingsBase
{	
	static const int VERSION = 4;
	
#ifdef EXPANSIONMODKILLFEED
	//! These are not implemented, uncomment once done
	//bool ShowVictimOnKillFeed;
	//bool ShowDistanceOnKillFeed;
	//bool ShowKillerOnKillFeed;
	//bool ShowWeaponOnKillFeed;
#endif
	
	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionNotificationSettings()
	{
	}
	
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnRecieve").Add(ctx);
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

		return true;
	}
	
	override void OnSend( ParamsWriteContext ctx )
	{
		ExpansionNotificationSettings thisSetting = this;

		ctx.Write( thisSetting );
	}
	
	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Send").Add(identity);
#endif

		if ( !IsMissionHost() )
		{
			return 0;
		}
		
		ScriptRPC rpc = new ScriptRPC;
		OnSend( rpc );
		rpc.Send( null, ExpansionSettingsRPC.Notification, true, identity );
		
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
	private void CopyInternal(  ExpansionNotificationSettings s )
	{
#ifdef EXPANSIONMODKILLFEED
		//! These are not implemented, uncomment once done
		//ShowDistanceOnKillFeed = s.ShowDistanceOnKillFeed;
		//ShowVictimOnKillFeed = s.ShowVictimOnKillFeed;
		//ShowKillerOnKillFeed = s.ShowKillerOnKillFeed;
		//ShowWeaponOnKillFeed = s.ShowWeaponOnKillFeed;
#endif
		
		ExpansionNotificationSettingsBase sb = s;
		CopyInternal( sb );
	}
	
	// ------------------------------------------------------------
	private void CopyInternal(  ExpansionNotificationSettingsBase s )
	{
		EnableNotification = s.EnableNotification;
		ShowPlayerJoinServer = s.ShowPlayerJoinServer;
		JoinMessageType = s.JoinMessageType;
		ShowPlayerLeftServer = s.ShowPlayerLeftServer;
		LeftMessageType = s.LeftMessageType;

#ifdef EXPANSIONMODMISSIONS
		ShowAirdropStarted = s.ShowAirdropStarted;
		ShowAirdropClosingOn = s.ShowAirdropClosingOn;
		ShowAirdropDropped = s.ShowAirdropDropped;
		ShowAirdropEnded = s.ShowAirdropEnded;
		ShowPlayerAirdropStarted = s.ShowPlayerAirdropStarted;
		ShowPlayerAirdropClosingOn = s.ShowPlayerAirdropClosingOn;
		ShowPlayerAirdropDropped = s.ShowPlayerAirdropDropped;
#endif

#ifdef EXPANSIONMODBASEBUILDING
		ShowTerritoryNotifications = s.ShowTerritoryNotifications;
#endif

#ifdef EXPANSIONMODKILLFEED
		EnableKillFeed = s.EnableKillFeed;
		KillFeedMessageType = s.KillFeedMessageType;
		KillFeedFall = s.KillFeedFall;
		KillFeedCarHitDriver = s.KillFeedCarHitDriver;
		KillFeedCarHitNoDriver = s.KillFeedCarHitNoDriver;
		KillFeedCarCrash = s.KillFeedCarCrash;
		KillFeedCarCrashCrew = s.KillFeedCarCrashCrew;
#ifdef EXPANSIONMODVEHICLE
		KillFeedHeliHitDriver = s.KillFeedHeliHitDriver;
		KillFeedHeliHitNoDriver = s.KillFeedHeliHitNoDriver;
		KillFeedHeliCrash = s.KillFeedHeliCrash;
		KillFeedHeliCrashCrew = s.KillFeedHeliCrashCrew;
		KillFeedBoatHitDriver = s.KillFeedBoatHitDriver;
		KillFeedBoatHitNoDriver = s.KillFeedBoatHitNoDriver;
		KillFeedBoatCrash = s.KillFeedBoatCrash;
		KillFeedBoatCrashCrew = s.KillFeedBoatCrashCrew;
		/*KillFeedPlaneHitDriver = s.KillFeedPlaneHitDriver;
		KillFeedPlaneHitNoDriver = s.KillFeedPlaneHitNoDriver;
		KillFeedBikeHitDriver = s.KillFeedBikeHitDriver;
		KillFeedBikeHitNoDriver = s.KillFeedBikeHitNoDriver;*/
#endif
		KillFeedBarbedWire = s.KillFeedBarbedWire;
		KillFeedFire = s.KillFeedFire;
		KillFeedWeaponExplosion = s.KillFeedWeaponExplosion;
		KillFeedDehydration = s.KillFeedDehydration;
		KillFeedStarvation = s.KillFeedStarvation;
		KillFeedBleeding = s.KillFeedBleeding;
		KillFeedStatusEffects = s.KillFeedStatusEffects;
		KillFeedSuicide = s.KillFeedSuicide;
		KillFeedWeapon = s.KillFeedWeapon;
		KillFeedMeleeWeapon = s.KillFeedMeleeWeapon;
		KillFeedBarehands = s.KillFeedBarehands;
		KillFeedInfected = s.KillFeedInfected;
		KillFeedAnimal = s.KillFeedAnimal;
		KillFeedKilledUnknown = s.KillFeedKilledUnknown;
		KillFeedDiedUnknown = s.KillFeedDiedUnknown;
#ifdef JM_COT
		EnableKillFeedDiscordMsg = s.EnableKillFeedDiscordMsg;
#endif
#endif
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnLoad");
#endif

		m_IsLoaded = true;

		bool save;

		bool notificationSettingsExist = FileExist(EXPANSION_NOTIFICATION_SETTINGS);

		if (notificationSettingsExist)
		{
			EXPrint("[ExpansionNotificationSettings] Load existing setting file:" + EXPANSION_NOTIFICATION_SETTINGS);
			
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

#ifdef EXPANSIONMODKILLFEED
#ifdef EXPANSIONMODVEHICLE
				if (settingsBase.m_Version < 3)
				{
					KillFeedBoatHitDriver = settingsDefault.KillFeedBoatHitDriver;
					KillFeedBoatHitNoDriver = settingsDefault.KillFeedBoatHitNoDriver;
				}
#endif

				if (settingsBase.m_Version < 4)
				{
					KillFeedStatusEffects = settingsDefault.KillFeedStatusEffects;
				}
#endif

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
			EXPrint("[ExpansionNotificationSettings] No existing setting file:" + EXPANSION_NOTIFICATION_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}
		
		if (save)
			Save();
				
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

		ExpansionSettings.SI_Notification.Invoke();
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;
		
		EnableNotification = true;

		ShowPlayerJoinServer = true;
		JoinMessageType = ExpansionAnnouncementType.NOTIFICATION;
		ShowPlayerLeftServer = true;
		LeftMessageType = ExpansionAnnouncementType.NOTIFICATION;

#ifdef EXPANSIONMODMISSIONS
		ShowAirdropStarted = true;
		ShowAirdropClosingOn = true;
		ShowAirdropDropped = true;
		ShowAirdropEnded = true;
		ShowPlayerAirdropStarted = true;
		ShowPlayerAirdropClosingOn = true;
		ShowPlayerAirdropDropped = true;
#endif

#ifdef EXPANSIONMODBASEBUILDING
		ShowTerritoryNotifications = true;
#endif

#ifdef EXPANSIONMODKILLFEED
		EnableKillFeed = true;
		KillFeedMessageType = ExpansionAnnouncementType.NOTIFICATION;

		//! These are not implemented, uncomment once done
		//ShowDistanceOnKillFeed = true;
		//ShowVictimOnKillFeed = true;
		//ShowKillerOnKillFeed = true;
		//ShowWeaponOnKillFeed = true;
		
		KillFeedFall = true;
		KillFeedCarHitDriver = true;
		KillFeedCarHitNoDriver = true;
		KillFeedCarCrash = true;
		KillFeedCarCrashCrew = true;
#ifdef EXPANSIONMODVEHICLE
		KillFeedHeliHitDriver = true;
		KillFeedHeliHitNoDriver = true;
		KillFeedHeliCrash = true;
		KillFeedHeliCrashCrew = true;
		KillFeedBoatHitDriver = true;
		KillFeedBoatHitNoDriver = true;
		KillFeedBoatCrash = true;
		KillFeedBoatCrashCrew = true;
		/*KillFeedPlaneHitDriver = true;
		KillFeedPlaneHitNoDriver = true;
		KillFeedBikeHitDriver = true;
		KillFeedBikeHitNoDriver = true;*/
#endif
		KillFeedBarbedWire = true;
		KillFeedFire = true;
		KillFeedWeaponExplosion = true;
		KillFeedDehydration = true;
		KillFeedStarvation = true;
		KillFeedBleeding = true;
		KillFeedStatusEffects = true;
		KillFeedSuicide = true;
		KillFeedWeapon = true;
		KillFeedMeleeWeapon = true;
		KillFeedBarehands = true;
		KillFeedInfected = true;
		KillFeedAnimal = true;
		KillFeedKilledUnknown = true;
		KillFeedDiedUnknown = true;

#ifdef JM_COT
		EnableKillFeedDiscordMsg = false;
#endif
#endif
	}
	
	// ------------------------------------------------------------	
	override string SettingName()
	{
		return "Notification Settings";
	}
};