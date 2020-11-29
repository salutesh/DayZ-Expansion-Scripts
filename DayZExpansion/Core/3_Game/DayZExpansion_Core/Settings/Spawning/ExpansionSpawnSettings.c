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
		
		EnableSpawnSelection = false; 		//! Will be enabled if the map have a configured spawn location on generation
		SpawnSelectionScreenMenuID = 1004;

		//! Set default markers depending on map name
		string world_name = "empty";
		GetGame().GetWorldName(world_name);
		world_name.ToLower();

		//! Vanilla Maps
		if ( world_name.Contains( "chernarusplus" ) || world_name.Contains( "chernarusplusgloom" ) )
		{
			ExpansionSpawnsChernarus();
		}
		else if ( world_name.Contains( "enoch" ) || world_name.Contains( "enochgloom" ) )
		{
			ExpansionSpawnsLivonia();
		} else if ( world_name.Contains( "deerisle" ) || world_name.Contains( "deerislegloom" ) )
		{
			ExpansionSpawnsDeerisle();
		} else if ( world_name.Contains( "sandbox" ) || world_name.Contains( "sandboxgloom" ) )
		{
			ExpansionSpawnsSandbox();
		} else if ( world_name.Contains( "namalsk" ) || world_name.Contains( "namalskgloom" ) )
		{
			ExpansionSpawnsNamalsk();
		}
	}

	
	// ------------------------------------------------------------
	// Expansion ExpansionSpawnsChernarus
	// ------------------------------------------------------------
	void ExpansionSpawnsChernarus()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionSpawnsChernarus::Defaults - Start");
		#endif

		ref array<vector> positions = new array<vector>;
		ref ExpansionSpawnLocation location;

		EnableSpawnSelection = true;
		
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
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionSpawnsChernarus::Defaults - End");
		#endif

	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionSpawnsLivonia
	// ------------------------------------------------------------
	void ExpansionSpawnsLivonia()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionSpawnsLivonia::Defaults - Start");
		#endif
		
		ref array<vector> positions = new array<vector>;
		ref ExpansionSpawnLocation location;

		EnableSpawnSelection = true;
		
		//! Bielawa
		positions.Insert( Vector( 1291.614258, 286.404175, 9849.049805 ) );
		positions.Insert( Vector( 1488.869751, 284.999756, 9866.576172 ) );
		positions.Insert( Vector( 1692.644165, 286.825073, 9950.559570 ) );
		positions.Insert( Vector( 1772.424316, 275.384155, 9837.327148 ) );
		positions.Insert( Vector( 1107.232178, 282.667786, 9262.442383 ) );
		location = new ExpansionSpawnLocation( "Bielawa", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Lukow
		positions.Insert( Vector( 3028.617432, 271.855133, 12067.580078 ) );
		positions.Insert( Vector( 3243.989258, 250.724686, 11933.792969 ) );
		positions.Insert( Vector( 3640.191162, 244.446060, 12446.875977 ) );
		positions.Insert( Vector( 4053.288818, 242.455200, 11554.082031 ) );
		positions.Insert( Vector( 3397.184326, 263.205750, 11606.515625 ) );
		location = new ExpansionSpawnLocation( "Lukow", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Brena
		positions.Insert( Vector( 6116.879395, 185.599640, 11316.484375 ) );
		positions.Insert( Vector( 6237.983398, 170.833481, 10880.137695 ) );
		positions.Insert( Vector( 6600.863281, 188.712906, 11543.671875 ) );
		positions.Insert( Vector( 7015.449219, 172.516708, 11113.072266 ) );
		positions.Insert( Vector( 6426.084473, 201.526901, 11602.169922 ) );
		location = new ExpansionSpawnLocation( "Brena", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Kolembrody
		positions.Insert( Vector( 8350.314453, 202.867752, 12259.904297 ) );
		positions.Insert( Vector( 8590.466797, 172.585556, 11946.450195 ) );
		positions.Insert( Vector( 8294.539063, 174.662094, 11968.569336 ) );
		positions.Insert( Vector( 8553.800781, 187.723114, 12174.582031 ) );
		location = new ExpansionSpawnLocation( "Kolembrody", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Grabin
		positions.Insert( Vector( 11212.156250, 197.501846, 11669.453125 ) );
		positions.Insert( Vector( 11171.645508, 174.064804, 11324.010742 ) );
		positions.Insert( Vector( 10999.512695, 173.887360, 11395.913086 ) );
		positions.Insert( Vector( 11345.790039, 181.382370, 11485.919922 ) );
		positions.Insert( Vector( 11187.933594, 170.951965, 11216.284180 ) );
		positions.Insert( Vector( 10829.276367, 173.663818, 11294.203125 ) );
		location = new ExpansionSpawnLocation( "Grabin", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionSpawnsLivonia::Defaults - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionSpawnsSandbox
	// ------------------------------------------------------------
	void ExpansionSpawnsSandbox()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionSpawnsSandbox::Defaults - Start");
		#endif
		
		ref array<vector> positions = new array<vector>;
		ref ExpansionSpawnLocation location;

		EnableSpawnSelection = true;
		
		//! Top Left
		positions.Insert( Vector( 112.827866, 10.012445, 101.494431 ) );
		positions.Insert( Vector( 106.173439, 10.012496, 283.065094 ) );
		positions.Insert( Vector( 292.435364, 10.012502, 94.893372 ) );
		positions.Insert( Vector( 329.115234, 10.012492 ,220.430328 ) );
		location = new ExpansionSpawnLocation( "Top Left", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Top Right
		positions.Insert( Vector( 40.214359, 10.012502, 1245.800049 ) );
		positions.Insert( Vector( 53.061890, 10.012502, 1052.014648 ) );
		positions.Insert( Vector( 159.996994, 10.012480, 1088.825195 ) );
		positions.Insert( Vector( 215.231918, 10.012502, 1236.653809 ) );
		location = new ExpansionSpawnLocation( "Top Right", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Bottom Right
		positions.Insert( Vector( 1237.554077, 10.012468, 1253.929321 ) );
		positions.Insert( Vector( 1130.051758, 10.012502, 1252.959351 ) );
		positions.Insert( Vector( 1168.290771, 10.012502, 1169.665527 ) );
		positions.Insert( Vector( 1229.704712, 10.012502, 1133.009155 ) );
		location = new ExpansionSpawnLocation( "Bottom Right", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Bottom Left
		positions.Insert( Vector( 1268.623169, 10.012502, 21.224548 ) );
		positions.Insert( Vector( 1157.893677, 10.012421, 33.013878 ) );
		positions.Insert( Vector( 1157.629028, 10.012501, 146.891006 ) );
		positions.Insert( Vector( 1254.572998, 10.012502, 184.080612 ) );
		location = new ExpansionSpawnLocation( "Bottom Left", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Middle
		positions.Insert( Vector( 761.465820, 10.012502, 673.523376 ) );
		positions.Insert( Vector( 697.438843, 10.012501, 737.098755 ) );
		positions.Insert( Vector( 692.220642, 10.012502, 600.312195 ) );
		positions.Insert( Vector( 828.482178, 10.012494, 580.280029 ) );
		positions.Insert( Vector( 838.387878, 10.012502, 743.722046 ) );
		location = new ExpansionSpawnLocation( "Middle", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionSpawnsSandbox::Defaults - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionSpawnsDeerIsle
	// ------------------------------------------------------------
	void ExpansionSpawnsDeerisle()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionSpawnsDeerIsle::Defaults - Start");
		#endif
		
		ref array<vector> positions = new array<vector>;
		ref ExpansionSpawnLocation location;

		EnableSpawnSelection = true;
		
		//! Smallville
		positions.Insert( Vector( 10307.980469, 4.610003, 4283.039551 ) );
		positions.Insert( Vector( 10481.480469, 9.250264, 4247.019043 ) );
		positions.Insert( Vector( 10516.448242, 12.487309, 4415.490723 ) );
		positions.Insert( Vector( 10813.910156, 1.885007, 4589.654297 ) );
		positions.Insert( Vector( 10230.116211, 3.189571, 4238.677246 ) );
		positions.Insert( Vector( 10098.096680, 16.852987, 4507.153809 ) );
		location = new ExpansionSpawnLocation( "Smallville", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Oceanville
		positions.Insert( Vector( 10501.626953, 38.448135, 4838.285645 ) );
		positions.Insert( Vector( 10264.243164, 29.939362, 4646.568359 ) );
		positions.Insert( Vector( 10171.602539, 33.866161, 4771.109863 ) );
		positions.Insert( Vector( 10027.609375, 46.279781, 5010.659668 ) );
		positions.Insert( Vector( 10299.725586, 43.082348, 5171.118652 ) );
		location = new ExpansionSpawnLocation( "Oceanville", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Fisher Camp
		positions.Insert( Vector( 10451.924805, 2.585993, 5605.494629 ) );
		positions.Insert( Vector( 10493.400391, 16.896009, 5411.615723 ) );
		positions.Insert( Vector( 10397.962891, 17.839876, 5373.638672 ) );
		positions.Insert( Vector( 10266.131836, 8.428172, 5572.642578 ) );
		positions.Insert( Vector( 10339.783203, 7.476208, 5502.157227 ) );
		location = new ExpansionSpawnLocation( "Fisher Camp", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Unnnamed Town
		positions.Insert( Vector( 8955.467773, 30.112320, 5266.041992 ) );
		positions.Insert( Vector( 9130.750977, 27.931656, 5132.872070 ) );
		positions.Insert( Vector( 9228.518555, 29.032925, 5206.634766 ) );
		positions.Insert( Vector( 9179.117188, 24.799879, 5319.198242 ) );
		positions.Insert( Vector( 9304.708984, 28.948595, 5198.045410 ) );
		positions.Insert( Vector( 9289.031250, 39.871628, 5421.801270 ) );
		positions.Insert( Vector( 9479.124023, 27.024431, 5234.839844 ) );
		location = new ExpansionSpawnLocation( "Unnnamed Town", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionSpawnsDeerIsle::Defaults - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionSpawnsNamaslk
	// ------------------------------------------------------------
	void ExpansionSpawnsNamalsk()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionSpawnsNamalsk::Defaults - Start");
		#endif

		EnableSpawnSelection = false;
		
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionSpawnsNamalsk::Defaults - End");
		#endif
	}
};