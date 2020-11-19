/**
 * ExpansionMissionSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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
	bool Enabled;
	int TimeBetweenMissions;

	int MinMissions;
	int MaxMissions;

	int MinPlayersToStartMissions;

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
	// ExpansionMissionModule Destructor
	// ------------------------------------------------------------
	void ~ExpansionMissionSettings()
	{		
		delete Missions;
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
	private void CopyInternal( ref ExpansionMissionSettings s )
	{
		int i = 0;

		Missions.Clear();
		for (i = 0; i < s.Missions.Count(); i++)
		{
			Missions.Insert( s.Missions[i] );
		}

		Enabled = s.Enabled;
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
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMissionSettings::Load - Start");
		#endif

		m_IsLoaded = true;

		if ( !FileExist( EXPANSION_MISSIONS_FOLDER ) ) 
		{
			MakeDirectory( EXPANSION_MISSIONS_FOLDER );
			
			m_GenerateDefaults = true;
		}

		if ( FileExist( EXPANSION_MISSION_SETTINGS ) )
		{
			Print("[ExpansionMissionSettings] Loading settings");

			JsonFileLoader<ExpansionMissionSettings>.JsonLoadFile( EXPANSION_MISSION_SETTINGS, this );

			Expansion_Assert_False( MinPlayersToStartMissions >= 0, "[ExpansionMissionSettings] MinPlayersToStartMissions can't be lower than 0!" );
			Expansion_Assert_False( MinMissions >= 0, "[ExpansionMissionSettings] MinMissions can't be lower than 0!" );
			Expansion_Assert_False( MaxMissions >= 0, "[ExpansionMissionSettings] MaxMissions can't be lower than 0!" );
			Expansion_Assert_False( MaxMissions >= MinMissions, "[ExpansionMissionSettings] MaxMissions can't be lower than MinMissions!" );

			array< int > toRemove = new array< int >();
			for ( int i = 0; i < Missions.Count(); ++i )
			{
				if ( !FileExist( Missions[i].MissionPath ) )
				{
					Print( "[ExpansionMissionSettings] Mission \"" + Missions[i].MissionPath + "\" doesn't exist, removing entry." );
					toRemove.Insert( i );
				}
			}

			for ( int j = 0; j < toRemove.Count(); ++j )
			{
				Missions.Remove( toRemove[j] );
			}

			Save();

			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionMissionSettings::Load - End");
			#endif

			return true;
		}

		Defaults();
		Save();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMissionSettings::Load - End");
		#endif

		return false;
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
		Print("[ExpansionMissionSettings] Loading default settings");
		
		string world_name = "empty";
		GetGame().GetWorldName(world_name);
		world_name.ToLower();

		Enabled = true;
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
};