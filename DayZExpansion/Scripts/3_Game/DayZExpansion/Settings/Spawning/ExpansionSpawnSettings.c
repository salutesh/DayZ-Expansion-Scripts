/**
 * ExpansionSpawnSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionSpawnSettings
 * @brief		Spawn settings class
 **/
class ExpansionSpawnSettings: ExpansionSettingBase
{
	ref ExpansionStartingClothing StartingClothing;
	ref ExpansionStartingGear StartingGear;

	bool EnableSpawnSelection;
	int SpawnSelectionScreenMenuID; //
	bool SpawnOnTerritory;

	ref array< ref ExpansionSpawnLocation > SpawnLocations;
	
	[NonSerialized()]
	private bool m_IsLoaded;
	
	// ------------------------------------------------------------
	void ExpansionSpawnSettings()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSpawnSettings::ExpansionSpawnSettings - Start");
		#endif
		
		StartingClothing = new ExpansionStartingClothing();
		StartingGear = new ExpansionStartingGear();

		SpawnLocations = new array< ref ExpansionSpawnLocation >();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSpawnSettings::ExpansionSpawnSettings - End");
		#endif
	}

	void ~ExpansionSpawnSettings()
	{
		delete StartingClothing;
		delete StartingGear;
		
		delete SpawnLocations;
	}
	
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSpawnSettings::OnRecieve - Start");
		#endif
		
		ExpansionSpawnSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionSpawnSettings::OnRecieve setting");
			return false;
		}

		CopyInternal( setting );

		m_IsLoaded = true;

		ExpansionSettings.SI_Spawn.Invoke();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSpawnSettings::OnRecieve - End");
		#endif

		return true;
	}
	
	override void OnSend( ParamsWriteContext ctx )
	{
		ref ExpansionSpawnSettings thisSetting = this;

		ctx.Write( thisSetting );
	}

	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSpawnSettings::Send - Start");
		#endif
		
		if ( !IsMissionHost() )
		{
			return 0;
		}
		
		ScriptRPC rpc = new ScriptRPC;
		OnSend( rpc );
		rpc.Send( null, ExpansionSettingsRPC.Spawn, true, identity );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSpawnSettings::Send - End and return");
		#endif
		return 0;
	}

	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
		ExpansionSpawnSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}

	// ------------------------------------------------------------
	private void CopyInternal( ref ExpansionSpawnSettings s )
	{
		StartingClothing = s.StartingClothing;
		StartingGear = s.StartingGear;

		EnableSpawnSelection = s.EnableSpawnSelection;
		SpawnSelectionScreenMenuID = s.SpawnSelectionScreenMenuID;
		SpawnOnTerritory = s.SpawnOnTerritory;

		for ( int i = 0; i < s.SpawnLocations.Count(); i++ )
			SpawnLocations.Insert( ExpansionSpawnLocation.Copy( s.SpawnLocations[i] ) );
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
		EXPrint("ExpansionSpawnSettings::Load - Start");
		#endif

		m_IsLoaded = true;

		if ( FileExist( EXPANSION_SPAWN_SETTINGS ) )
		{
			Print("[ExpansionSpawnSettings] Loading settings");

			JsonFileLoader<ExpansionSpawnSettings>.JsonLoadFile( EXPANSION_SPAWN_SETTINGS, this );

			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionSpawnSettings::Load - End");
			#endif

			return true;
		}
		
		Defaults();
		Save();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSpawnSettings::Load - End");
		#endif
		return false;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionSpawnSettings] Saving settings");

		JsonFileLoader<ExpansionSpawnSettings>.JsonSaveFile( EXPANSION_SPAWN_SETTINGS, this );

		return true;
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[ExpansionSpawnSettings] Loading default settings");
		#endif
		
		StartingGear.Defaults();
		StartingClothing.Defaults();
	}
};