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
	int SpawnSelectionScreenMenuID;
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

		SpawnLocations = new array< ref ExpansionSpawnLocation >;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSpawnSettings::ExpansionSpawnSettings - End");
		#endif
	}

	void ~ExpansionSpawnSettings()
	{
		delete StartingClothing;
		delete StartingGear;
		
		SpawnLocations.Clear();
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
		
		SpawnLocations.Clear();
		for ( int i = 0; i < s.SpawnLocations.Count(); i++ )
		{
			SpawnLocations.Insert( s.SpawnLocations[i] );
		}
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
		
		EnableSpawnSelection = true;
		SpawnSelectionScreenMenuID = 1004;
		
		ref array<vector> positions = new array<vector>;
		ref ExpansionSpawnLocation location;
		
		//! Svetloyarsk
		positions.Insert( Vector( 14273.2, 2.4, 13053.3 ) );
		positions.Insert( Vector( 14407.3, 2.0, 13253.0 ) );
		positions.Insert( Vector( 14142.4, 3.3, 13290.2 ) );
		positions.Insert( Vector( 13910.9, 4.3, 13624.9 ) );
		location = new ExpansionSpawnLocation( "Svetloyarsk", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Berezino
		positions.Insert( Vector( 12915.7, 3.4, 9278.2 ) );
		positions.Insert( Vector( 13057.2, 2.3, 9584.48 ) );
		positions.Insert( Vector( 13052.9, 6.1, 9894.7 ) );
		positions.Insert( Vector( 13207.2, 2.3, 10193.7 ) );
		location = new ExpansionSpawnLocation( "Berezino", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Solnich (factory)
		positions.Insert( Vector( 13169.5, 3.07561, 7504.03 ) );
		positions.Insert( Vector( 13274, 1.7835, 7258.81 ) );
		positions.Insert( Vector( 13345.6, 1.87793, 6987.36 ) );
		positions.Insert( Vector( 13383, 2.75516, 6815.89 ) );
		location = new ExpansionSpawnLocation( "Solnich", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Solnichniy
		positions.Insert( Vector( 13529.530273, 2.251228, 6455.612793 ) );
		positions.Insert( Vector( 13484.724609, 1.746646, 5911.094727 ) );
		positions.Insert( Vector( 13515.912109, 2.679648, 6117.384277 ) );
		positions.Insert( Vector( 13534.671875, 1.644669, 6234.750000 ) );
		location = new ExpansionSpawnLocation( "Solnichniy", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Kamyshovo
		positions.Insert( Vector( 12321.939453, 1.926140, 3446.666748 ) );
		positions.Insert( Vector( 12188.570313, 1.727290, 3422.332275 ) );
		positions.Insert( Vector( 11992.250000, 1.982081, 3404.554443 ) );
		positions.Insert( Vector( 11859.343750, 1.901515, 3367.714844 ) );
		location = new ExpansionSpawnLocation( "Kamyshovo", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Elektrozavodsk
		positions.Insert( Vector( 11099.068359, 2.297676, 2735.562500 ) );
		positions.Insert( Vector( 10858.413086, 2.911721, 2328.290283 ) );
		positions.Insert( Vector( 10490.914063, 1.846902, 1950.148438 ) );
		positions.Insert( Vector( 9826.885742, 1.711821, 1757.374634 ) );
		positions.Insert( Vector( 9428.458008, 2.254453, 1826.218506 ) );
		positions.Insert( Vector( 9153.536133, 3.421117, 1914.300659 ) );
		location = new ExpansionSpawnLocation( "Elektrozavodsk", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Chernogorsk
		positions.Insert( Vector( 6044.103027, 6.429995, 1871.500610 ) );
		positions.Insert( Vector( 6220.440430, 1.917814, 2101.123291 ) );
		positions.Insert( Vector( 7118.122070, 1.824183, 2533.971924 ) );
		positions.Insert( Vector( 7419.497070, 1.768386, 2576.503906 ) );
		positions.Insert( Vector( 8139.250000, 1.151711, 2802.356445 ) );
		location = new ExpansionSpawnLocation( "Chernogorsk", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Balota
		positions.Insert( Vector( 4654.594238, 1.459718, 2132.866699 ) );
		positions.Insert( Vector( 4543.990723, 1.901639, 2198.166260 ) );
		positions.Insert( Vector( 4269.422852, 1.289235, 2245.660889 ) );
		positions.Insert( Vector( 4111.905762, 1.566264, 2193.932617 ) );
		location = new ExpansionSpawnLocation( "Balota", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Komarovo
		positions.Insert( Vector( 3887.559082, 1.595509, 2207.158936 ) );
		positions.Insert( Vector( 3746.655762, 2.445386, 2199.878174 ) );
		positions.Insert( Vector( 3507.422852, 2.008609, 2101.454590 ) );
		positions.Insert( Vector( 3366.985352, 1.902521, 2002.414063 ) );
		location = new ExpansionSpawnLocation( "Komarovo", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Kamenka
		positions.Insert( Vector( 2164.700195, 1.728014, 2049.443848 ) );
		positions.Insert( Vector( 2031.425415, 1.290743, 2150.743408 ) );
		positions.Insert( Vector( 1708.523071, 1.958309, 2031.263672 ) );
		positions.Insert( Vector( 1563.325684, 2.174132, 2063.254883 ) );
		location = new ExpansionSpawnLocation( "Kamenka", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
	}
};