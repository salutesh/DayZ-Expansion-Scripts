/**
 * ExpansionLogSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionLogSettings
 * @brief		Log settings class
 **/
class ExpansionLogSettings: ExpansionSettingBase
{
	static const int VERSION = 2;

	bool Safezone;				//! If enabled, generate logs when the player leave or enter a safezone
	bool AdminTools;			//! If enabled, generate logs of the adminhammer and expansionadmincarkey when used
	
	#ifdef EXPANSIONMODVEHICLE
	bool VehicleCarKey;			//! If enabled, generate logs about pairing, unpairing, locking, unlocking vehicles with car keys
	bool VehicleTowing;			//! If enabled, generate logs about towing and untowing vehicles
	bool VehicleLockPicking;	//! If enabled, generate logs about lockpicking a vehicle
	#endif

	#ifdef EXPANSIONMODBASEBUILDING
	bool BaseBuildingRaiding;	//! If enabled, generate logs about raiding expansion basebuilding and safes
	bool CodeLockRaiding;		//! If enabled, generate logs about codelock raiding (wrong core)
	bool Territory;				//! If enabled, generate logs about creating, deleting territories and inviting players
	#endif

	#ifdef EXPANSIONMOD
	bool Killfeed;				//! If enabled, generate logs based on the killfeed module
	bool SpawnSelection;		//! If enabled, generate logs when the player spawn
	#endif

	#ifdef EXPANSIONMODGROUPS
	bool Party;					//! If enabled, generate logs when the player create a party or invite someone
	#endif

	#ifdef EXPANSIONMODMISSIONS
	bool MissionAirdrop;		//! If enabled, generate logs of spawned airdrops from missions or player called
	//bool MissionHorde;		//! Will log Horde missions
	#endif

	#ifdef EXPANSIONMODCHAT
	bool Chat;					//! If enabled, generate logs of the chat
	#endif

	#ifdef EXPANSIONMODMARKET
	bool Market;				//! If enabled, generate logs for market system actions by all players
	bool ATM;					//! If enabled, generate logs for ATM system actions by all players
	#endif

	bool LogToScripts; 			// Should the prints be logged in the scripts logs ?
	bool LogToADM; 				// Should the prints be logged in the ADM logs ?

	#ifdef EXPANSIONMODVEHICLE
	bool VehicleDestroyed; 		// Print a log when a vehicle is destroyed
	#endif
	
	[NonSerialized()]
	private string m_FileTimestamp;

	[NonSerialized()]
	private FileHandle m_FileLog;
	
	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionLogSettings()
	{
	}
	
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnRecieve").Add(ctx);
#endif

		//Not sent to client under normal operation
		m_IsLoaded = true;

		ExpansionLogSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionLogSettings::OnRecieve setting");
			return false;
		}

		CopyInternal( setting );

		m_IsLoaded = true;

		ExpansionSettings.SI_Log.Invoke();
		
		return true;
	}
	
	override void OnSend( ParamsWriteContext ctx )
	{
		ExpansionLogSettings thisSetting = this;

		ctx.Write( thisSetting );
	}
	
	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
		//Not sent to client
	}

	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
		ExpansionLogSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}

	// ------------------------------------------------------------
	private void CopyInternal( ExpansionLogSettings s )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
#endif

		Safezone = s.Safezone;

		#ifdef EXPANSIONMODVEHICLE
		VehicleCarKey = s.VehicleCarKey;
		VehicleTowing = s.VehicleTowing;
		VehicleLockPicking = s.VehicleLockPicking;
		#endif

		#ifdef EXPANSIONMODBASEBUILDING
		BaseBuildingRaiding = s.BaseBuildingRaiding;
		CodeLockRaiding = s.CodeLockRaiding;
		Territory = s.Territory;
		#endif

		#ifdef EXPANSIONMOD
		Killfeed = s.Killfeed;
		SpawnSelection = s.SpawnSelection;
		#endif

		#ifdef EXPANSIONMODMISSIONS
		MissionAirdrop = s.MissionAirdrop;
		//MissionHorde = s.MissionHorde;
		#endif
		
		#ifdef EXPANSIONMODGROUPS
		Party = s.Party;
		#endif

		#ifdef EXPANSIONMODCHAT
		Chat = s.Chat;
		#endif

		#ifdef EXPANSIONMODMARKET
		Market = s.Market;
		ATM = s.ATM;
		#endif

		AdminTools = s.AdminTools;

		LogToScripts = s.LogToScripts;
		LogToADM = s.LogToADM;

		#ifdef EXPANSIONMODVEHICLE
		VehicleDestroyed = s.VehicleDestroyed;
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

		m_FileTimestamp = EXPANSION_LOG_FOLDER + "\\" + "ExpLog_" + ExpansionStatic.GetDateTime() + ".log";

		bool save;

		bool  logSettingsExist = FileExist(EXPANSION_LOG_SETTINGS);

		if (logSettingsExist)
		{
			EXPrint("[ExpansionLogSettings] Load existing setting file:" + EXPANSION_LOG_SETTINGS);
			
			ExpansionLogSettings settingsDefault = new ExpansionLogSettings;
			settingsDefault.Defaults();

			JsonFileLoader<ExpansionLogSettings>.JsonLoadFile(EXPANSION_LOG_SETTINGS, this);

			if (m_Version < VERSION)
			{
				if (m_Version < 2)
				{
					EXPrint("[ExpansionLogSettings] Load - Converting v" + m_Version + " \"" + EXPANSION_LOG_SETTINGS + "\" to v" + VERSION);
					
					//! New with v2
					LogToScripts = settingsDefault.LogToScripts;
					LogToADM = settingsDefault.LogToADM;
					#ifdef EXPANSIONMODVEHICLE
					VehicleDestroyed = settingsDefault.VehicleDestroyed;
					#endif
				}

				m_Version = VERSION;
				save = true;
			}
		}
		else
		{
			EXPrint("[ExpansionLogSettings] No existing setting file:" + EXPANSION_LOG_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}
		
		if (save)
			Save();

		return logSettingsExist;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionLogSettings] Saving settings");

		JsonFileLoader<ExpansionLogSettings>.JsonSaveFile( EXPANSION_LOG_SETTINGS, this );

		return true;
	}
	
	// ------------------------------------------------------------
	override void Update( ExpansionSettingBase setting )
	{
		super.Update( setting );

		ExpansionSettings.SI_Log.Invoke();
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;
		
		Safezone = true;
		AdminTools = true;

		LogToScripts = false;
		LogToADM = false;

		#ifdef EXPANSIONMODVEHICLE
		VehicleDestroyed = true;
		VehicleCarKey = true;
		VehicleTowing = true;
		VehicleLockPicking = true;
		#endif

		#ifdef EXPANSIONMODBASEBUILDING
		BaseBuildingRaiding = true;
		CodeLockRaiding = true;
		Territory = true;
		#endif

		#ifdef EXPANSIONMOD
		Killfeed = true;
		SpawnSelection = true;
		#endif

		#ifdef EXPANSIONMODMISSIONS
		MissionAirdrop = true;
		//MissionHorde = true;
		#endif
		
		#ifdef EXPANSIONMODGROUPS
		Party = true;
		#endif

		#ifdef EXPANSIONMODCHAT
		Chat = true;
		#endif
		
		#ifdef EXPANSIONMODMARKET
		Market = true;
		ATM = true;
		#endif
	}
	
	override string SettingName()
	{
		return "Log Settings";
	}

	void PrintLog(string text)
	{
		if ( !FileExist( EXPANSION_LOG_FOLDER ) )
		{
			MakeDirectory( EXPANSION_LOG_FOLDER );
		}

		string output = ExpansionStatic.GetTime() + " " + text;

		if ( LogToScripts || LogToADM )
		{
			if ( LogToScripts )
			{
				Print(output);
			}
			if ( LogToADM )
			{
				GetGame().AdminLog(output);
			}
		} else {
			if ( !FileExist( m_FileTimestamp ) )
				m_FileLog = OpenFile(m_FileTimestamp, FileMode.WRITE);
			else
				m_FileLog = OpenFile(m_FileTimestamp, FileMode.APPEND);
			FPrintln(m_FileLog, output);
			CloseFile(m_FileLog);
		}
	}
};