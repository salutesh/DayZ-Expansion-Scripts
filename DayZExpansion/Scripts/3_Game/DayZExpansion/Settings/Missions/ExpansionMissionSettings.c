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

	autoptr array< ref ExpansionMissionMeta > Missions;

	[NonSerialized()]
	private bool m_GenerateDefaults;
	
	[NonSerialized()]
	static ref ScriptInvoker SI_OnSave = new ScriptInvoker();
	
	[NonSerialized()]
	private bool m_IsLoaded;

    // ------------------------------------------------------------
	// Expansion ExpansionMissionSettings
	// ------------------------------------------------------------
	void ExpansionMissionSettings()
	{		
		Missions = new array< ref ExpansionMissionMeta >;
	}
	
	override void HandleRPC( ref ParamsReadContext ctx )
	{
		//No send to client
    }

	override int Send( PlayerIdentity identity )
	{
		//No send to client
	}

	// ------------------------------------------------------------
	// Expansion Send
	// ------------------------------------------------------------
    override bool Copy( ExpansionSettingBase setting )
    {
		ExpansionMissionSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}

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
		EXPrint("ExpansionMissionSettings::Load - Start");
		#endif

		m_IsLoaded = true;

		if ( !FileExist( EXPANSION_MISSIONS_FOLDER ) ) 
		{
			MakeDirectory( EXPANSION_MISSIONS_FOLDER );
		}

		if ( FileExist( EXPANSION_MISSION_SETTINGS ) )
		{
			JsonFileLoader<ExpansionMissionSettings>.JsonLoadFile( EXPANSION_MISSION_SETTINGS, this );

			if ( MinMissions < 0 )
			{
				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint( "[ExpansionMissionSettings] MinMissions can't be lower than 0!" );
				#endif

			}

			if ( MaxMissions < 0 )
			{
				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint( "[ExpansionMissionSettings] MinMissions can't be lower than 0!" );
				#endif

			} else if ( MaxMissions < MinMissions )
			{
				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint( "[ExpansionMissionSettings] MaxMissions can't be lower than MinMissions!" );
				#endif
				
			}

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
	override bool OnSave()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[ExpansionMissionSettings] Saving settings");
		#endif

		JsonFileLoader<ExpansionMissionSettings>.JsonSaveFile( EXPANSION_MISSION_SETTINGS, this );

		SI_OnSave.Invoke( this );

		return true;
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
        #ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[ExpansionMissionSettings] Loading default settings");
		#endif
		
		Enabled = true;

		TimeBetweenMissions = 3600000; // 3600000; // 1 hour

		MinMissions = 0;
		MaxMissions = 1;

		m_GenerateDefaults = true;
	}

	// ------------------------------------------------------------
	bool DidGenerateDefaults()
	{
		return m_GenerateDefaults;
	}
}