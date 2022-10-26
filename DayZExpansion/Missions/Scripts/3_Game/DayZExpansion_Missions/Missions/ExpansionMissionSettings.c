/**
 * ExpansionMissionSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionMissionSettings
 * @brief		Mission settings class
 **/
class ExpansionMissionSettings: ExpansionSettingBase
{
	static const int VERSION = 2;

	bool Enabled;
	
	int InitialMissionStartDelay;
	int TimeBetweenMissions;

	int MinMissions;
	int MaxMissions;

	int MinPlayersToStartMissions;

	[NonSerialized()]
	ref array< ref ExpansionMissionMeta > Missions;

	[NonSerialized()]
	private bool m_GenerateDefaults;
	
	[NonSerialized()]
	static ref ScriptInvoker SI_OnSave = new ScriptInvoker();
	
	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	// ExpansionMissionSettings Constructor
	// ------------------------------------------------------------
	void ExpansionMissionSettings()
	{		
		Missions = new array< ref ExpansionMissionMeta >;
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionModule OnRecieve
	// ------------------------------------------------------------
	//TODO: COT module sending optimization
	override bool OnRecieve( ParamsReadContext ctx )
	{
		//! Don't send to client

		return true;
	}

	// ------------------------------------------------------------
	// ExpansionMissionModule Send
	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
		//! Don't send to client
	}

	// ------------------------------------------------------------
	// ExpansionMissionModule Copy
	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
		ExpansionMissionSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}

	// ------------------------------------------------------------
	// ExpansionMissionModule CopyInternal
	// ------------------------------------------------------------
	private void CopyInternal(  ExpansionMissionSettings s )
	{
		int i = 0;

		Missions.Clear();
		for (i = 0; i < s.Missions.Count(); i++)
		{
			Missions.Insert( s.Missions[i] );
		}

		Enabled = s.Enabled;
		InitialMissionStartDelay = s.InitialMissionStartDelay;
		TimeBetweenMissions = s.TimeBetweenMissions;
		MinMissions = s.MinMissions;
		MaxMissions = s.MaxMissions;
		MinPlayersToStartMissions = s.MinPlayersToStartMissions;
	}

	// ------------------------------------------------------------
	// ExpansionMissionModule IsLoaded
	// ------------------------------------------------------------
	override bool IsLoaded()
	{
		return m_IsLoaded;
	}

	// ------------------------------------------------------------
	// ExpansionMissionModule Unload
	// ------------------------------------------------------------
	override void Unload()
	{
		m_IsLoaded = false;
	}

	// ------------------------------------------------------------
	// ExpansionMissionModule OnLoad
	// ------------------------------------------------------------
	override bool OnLoad()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnLoad");
#endif

		m_IsLoaded = true;
		
		//! Move existing missions over from old location in $profile to new location in $mission
		string folderNameOld = EXPANSION_FOLDER + "Missions\\";
		if ( FileExist( folderNameOld ) ) 
			MoveSettings( folderNameOld, EXPANSION_MISSIONS_FOLDER );

		if ( !FileExist( EXPANSION_MISSIONS_FOLDER ) ) 
		{
			ExpansionStatic.MakeDirectoryRecursive( EXPANSION_MISSIONS_FOLDER );
			
			m_GenerateDefaults = true;
		}
		
		bool save;
		
		bool missionSettingsExist = FileExist( EXPANSION_MISSION_SETTINGS );
		
		if ( missionSettingsExist )
		{
			EXPrint("[ExpansionMissionSettings] Load existing setting file:" + EXPANSION_MISSION_SETTINGS);

			JsonFileLoader<ExpansionMissionSettings>.JsonLoadFile( EXPANSION_MISSION_SETTINGS, this );

			Expansion_Assert_False( MinPlayersToStartMissions >= 0, "[ExpansionMissionSettings] MinPlayersToStartMissions can't be lower than 0!" );
			Expansion_Assert_False( MinMissions >= 0, "[ExpansionMissionSettings] MinMissions can't be lower than 0!" );
			Expansion_Assert_False( MaxMissions >= 0, "[ExpansionMissionSettings] MaxMissions can't be lower than 0!" );
			Expansion_Assert_False( MaxMissions >= MinMissions, "[ExpansionMissionSettings] MaxMissions can't be lower than MinMissions!" );

			if (m_Version < VERSION)
			{
				m_Version = VERSION;
				save = true;
			}
		}
		else
		{
			EXPrint("[ExpansionMissionSettings] No existing setting file:" + EXPANSION_MISSION_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}
		
		if (save)
			Save();
		
		return missionSettingsExist;
	}

	// ------------------------------------------------------------
	// ExpansionMissionModule OnSave
	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionMissionSettings] Saving settings");

		JsonFileLoader<ExpansionMissionSettings>.JsonSaveFile( EXPANSION_MISSION_SETTINGS, this );

		SI_OnSave.Invoke( this );

		return true;
	}

	// ------------------------------------------------------------
	// ExpansionMissionModule Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;

		Enabled = true;

		InitialMissionStartDelay = 300000; // 5 minutes
		TimeBetweenMissions = 3600000; // 1 hour

		MinMissions = 0;
		MaxMissions = 1;

		MinPlayersToStartMissions = 0;

		m_GenerateDefaults = true;
	}

	// ------------------------------------------------------------
	bool DidGenerateDefaults()
	{
		return m_GenerateDefaults;
	}
	
	// ------------------------------------------------------------
	override string SettingName()
	{
		return "Mission Settings";
	}
};
