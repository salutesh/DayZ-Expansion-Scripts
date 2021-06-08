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
		} else if ( world_name.Contains( "chiemsee" ) || world_name.Contains( "chiemseegloom" ) )
		{
			ExpansionSpawnsChiemsee();
		} else if ( world_name.Contains( "esseker" ) || world_name.Contains( "essekergloom" ) )
		{
			ExpansionSpawnsEsseker();
		} else if ( world_name.Contains( "expansiontest" ) || world_name.Contains( "expansiontestgloom" ) )
		{
			ExpansionSpawnsExpansionTest();
		} else {
			ExpansionSpawnsChernarus();
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

		ref array<vector> positions = new array<vector>;
		ref ExpansionSpawnLocation location;

		EnableSpawnSelection = true;
		
		//! Tara Harbor
		positions.Insert( Vector( 8085.469238, 1.565431, 8224.343750 ) );
		positions.Insert( Vector( 7972.799805, 3.732371, 8055.347656 ) );
		positions.Insert( Vector( 7820.460449, 5.780485, 7983.991211 ) );
		positions.Insert( Vector( 7823.861328, 5.436656, 7876.534180 ) );
		positions.Insert( Vector( 7981.229004, 3.026991, 7395.474121 ) );
		positions.Insert( Vector( 7991.038574, 5.828287, 7320.502930 ) );
		positions.Insert( Vector( 7999.814941, 4.266075, 7226.579590 ) );
		positions.Insert( Vector( 7999.422852, 3.917540, 7135.880859 ) );
		location = new ExpansionSpawnLocation( "Tara Harbor", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Tara SawMill
		positions.Insert( Vector( 7451.234863, 2.629746, 5767.628418 ) );
		positions.Insert( Vector( 7437.044434, 1.961083, 5686.347656 ) );
		positions.Insert( Vector( 7270.833496, 2.368652, 5548.581055 ) );
		positions.Insert( Vector( 7173.758789, 2.490517, 5535.238770 ) );
		positions.Insert( Vector( 7029.282227, 4.738458, 5508.801758 ) );
		positions.Insert( Vector( 6907.666504, 2.793373, 5516.155273 ) );
		location = new ExpansionSpawnLocation( "Tara SawMill", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Brensk Marsh
        positions.Insert( Vector( 6151.092285, 1.264987, 5573.928223 ) );
        positions.Insert( Vector( 6283.334473, 1.935929, 5781.833008 ) );
        positions.Insert( Vector( 6277.715820, 3.072493, 5881.100586 ) );
        positions.Insert( Vector( 6242.260254, 2.079003, 5646.101563 ) );
        positions.Insert( Vector( 6075.745605, 1.194679, 5531.776855 ) );
        positions.Insert( Vector( 6077.612793, 3.505146, 5477.541016 ) );
        positions.Insert( Vector( 5994.918457, 1.527500, 5409.187988 ) );
		location = new ExpansionSpawnLocation( "Brensk Marsh", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Brensk
		positions.Insert( Vector( 4599.437500, 3.117430, 4800.037598 ) );
		positions.Insert( Vector( 4531.106445, 3.245819, 4734.894531 ) );
		positions.Insert( Vector( 4441.681641, 1.156230, 4705.120117 ) );
		positions.Insert( Vector( 4171.402832, 2.025218, 4714.413574 ) );
		positions.Insert( Vector( 4073.524658, 3.821608, 4738.167480 ) );
		positions.Insert( Vector( 4016.597656, 3.288769, 4808.031250 ) );
		location = new ExpansionSpawnLocation( "Brensk", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Lubjansk
		positions.Insert( Vector( 6001.995605, 2.685731, 11810.514648 )	);
		positions.Insert( Vector( 6070.703125, 2.419812, 11857.980469 )	);
		positions.Insert( Vector( 6120.427246, 2.542685, 11909.213867 )	);
		positions.Insert( Vector( 6182.059082, 1.988964, 11946.935547 )	);
		positions.Insert( Vector( 6240.597168, 1.854375, 11944.241211 )	);
		positions.Insert( Vector( 6334.204590, 2.814608, 11950.695313 )	);
		positions.Insert( Vector( 6397.264648, 2.427259, 11974.208008 )	);
		positions.Insert( Vector( 6453.961426, 2.999459, 11997.404297 )	);
		positions.Insert( Vector( 6532.939941, 1.888822, 12048.618164 )	);
		positions.Insert( Vector( 6614.894043, 2.246978, 12016.815430 )	);
		positions.Insert( Vector( 6711.061035, 1.821189, 11977.592773 )	);
		positions.Insert( Vector( 6799.871582, 7.757498, 11967.160156 )	);
		positions.Insert( Vector( 6993.547363, 2.643507, 11925.248047 )	);
		positions.Insert( Vector( 7086.631836, 2.728105, 11897.601563 )	);
		positions.Insert( Vector( 7220.497559, 1.426603, 11889.622070 )	);
		positions.Insert( Vector( 7303.187500, 2.878823, 11881.951172 )	);
		positions.Insert( Vector( 7418.365723, 2.204709, 11854.547852 )	);
		positions.Insert( Vector( 7516.029297, 1.701337, 11800.656250 )	);
		positions.Insert( Vector( 7665.194824, 2.558721, 11813.786133 )	);
		positions.Insert( Vector( 7766.950195, 1.250474, 11752.501953 )	);
		location = new ExpansionSpawnLocation( "Lubjansk", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Jalovisko
		positions.Insert( Vector( 8625.460938, 3.831624, 11224.921875 )	);
		positions.Insert( Vector( 8694.847656, 3.723127, 11138.439453 )	);
		positions.Insert( Vector( 8622.545898, 2.531196, 11054.539063 )	);
		positions.Insert( Vector( 8673.834961, 1.655312, 10926.909180 )	);
		positions.Insert( Vector( 8763.146484, 2.962097, 10880.047852 )	);
		positions.Insert( Vector( 8831.335938, 3.817108, 10787.751953 )	);
		location = new ExpansionSpawnLocation( "Jalovisko", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		//! Nemsk
		positions.Insert( Vector( 9106.095703, 1.958728, 10311.020508 ) );
		positions.Insert( Vector( 9140.910156, 2.197809, 10249.210938 ) );
		positions.Insert( Vector( 9155.135742, 1.494921, 10173.037109 ) );
		positions.Insert( Vector( 9141.457031, 0.993115, 10123.311523 ) );
		positions.Insert( Vector( 9071.829102, 1.655611, 10005.583008 ) );
		positions.Insert( Vector( 9029.701172, 2.618637, 9954.769531 ) );
		positions.Insert( Vector( 8995.268555, 2.661735, 9906.744141 ) );
		positions.Insert( Vector( 8928.280273, 3.379394, 9872.879883 ) );
		location = new ExpansionSpawnLocation( "Nemsk", positions );
		SpawnLocations.Insert( location );
		positions.Clear();

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionSpawnsNamalsk::Defaults - End");
		#endif
	}

	
	// ------------------------------------------------------------
	// Expansion ExpansionSpawnsChiemsee
	// ------------------------------------------------------------
	void ExpansionSpawnsChiemsee()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionSpawnsChiemsee::Defaults - Start");
		#endif

		ref array<vector> positions = new array<vector>;
		ref ExpansionSpawnLocation location;

		EnableSpawnSelection = true;
		
		//! In the middle of the woods fucks know where
		positions.Insert( Vector( 7695.176270, 109.585709, 8274.574219 ) );
		positions.Insert( Vector( 7806.858398, 108.956848, 7918.468750 ) );
		positions.Insert( Vector( 7519.054199, 101.605705, 7859.429199 ) );
		positions.Insert( Vector( 7394.625977, 95.575340, 8126.528809 ) );
		positions.Insert( Vector( 8013.906738, 99.150436, 7958.108398 ) );
		positions.Insert( Vector( 7971.340332, 95.097450, 7634.947754 ) );
		positions.Insert( Vector( 7747.353516, 104.652306, 7446.146973 ) );
		positions.Insert( Vector( 7740.395020, 105.165588, 7781.511230 ) );
		positions.Insert( Vector( 7586.498047, 114.430550, 7909.261230 ) );
		positions.Insert( Vector( 7489.011230, 104.665100, 8481.092773 ) );
		positions.Insert( Vector( 7583.570801, 122.583450, 8777.276367 ) );
		positions.Insert( Vector( 7770.703613, 104.025017, 8580.901367 ) );
		positions.Insert( Vector( 7681.148438, 116.996262, 8910.384766 ) );
		positions.Insert( Vector( 7812.642090, 122.677345, 8827.981445 ) );
		positions.Insert( Vector( 7947.310059, 120.652092, 8844.392578 ) );
		positions.Insert( Vector( 8217.591797, 85.408607, 8743.672852 ) );
		positions.Insert( Vector( 8079.215820, 71.149635, 8437.955078 ) );
		positions.Insert( Vector( 8141.916504, 60.722095, 8174.657227 ) );
		location = new ExpansionSpawnLocation( "Woods", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionSpawnsChiemsee::Defaults - End");
		#endif

	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionSpawnsEsseker
	// ------------------------------------------------------------
	void ExpansionSpawnsEsseker()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionSpawnsEsseker::Defaults - Start");
		#endif
		
		ref array<vector> positions = new array<vector>;
		ref ExpansionSpawnLocation location;

		EnableSpawnSelection = true;
		
		positions.Insert( Vector( 9565.315430, 73.700882, 5462.385254 ) );
        positions.Insert( Vector( 11480.708008, 90.736420, 5315.443848 ) );
        positions.Insert( Vector( 11148.616211, 82.239159, 5435.018066 ) );
        positions.Insert( Vector( 10840.094727, 87.835564, 5695.586914 ) );
        positions.Insert( Vector( 10449.486328, 79.059921, 5874.159668 ) );
        positions.Insert( Vector( 10738.149414, 109.114601, 5864.669922 ) );
        positions.Insert( Vector( 8517.313477, 98.113503, 6003.735352 ) );
        positions.Insert( Vector( 7903.954590, 104.279541, 5846.573242 ) );
        positions.Insert( Vector( 7811.805176, 86.072418, 5737.385254 ) );
        positions.Insert( Vector( 9117.009766, 108.684692, 5944.108398 ) );
		location = new ExpansionSpawnLocation( "Esseker", positions );
		SpawnLocations.Insert( location );
		positions.Clear();

        positions.Insert( Vector( 6213.568359, 173.489670, 5262.031250 ) );
        positions.Insert( Vector( 6631.679199, 168.740982, 5266.032715 ) );
        positions.Insert( Vector( 6614.347656, 108.247978, 5546.375488 ) );
        positions.Insert( Vector( 6251.624512, 163.699326, 5202.340820 ) );
        positions.Insert( Vector( 6702.913574, 144.765900, 5345.629395 ) );
        positions.Insert( Vector( 6637.131836, 122.660553, 5451.673340 ) );
		location = new ExpansionSpawnLocation( "Neptune Resort", positions );
		SpawnLocations.Insert( location );
		positions.Clear();

        positions.Insert( Vector( 5232.718750, 141.714386, 5293.656738 ) );
        positions.Insert( Vector( 4807.120117, 177.191376, 5325.092773 ) );
        positions.Insert( Vector( 4675.387207, 175.012375, 5285.001953 ) );
        positions.Insert( Vector( 4686.989746, 175.012390, 5276.281738 ) );
        positions.Insert( Vector( 4536.588379, 182.754379, 5104.264160 ) );
        positions.Insert( Vector( 4660.011230, 174.727921, 4926.456055 ) );
        positions.Insert( Vector( 4866.981445, 194.962906, 4884.998535 ) );
        positions.Insert( Vector( 5011.791504, 179.612839, 4967.582520 ) );
        positions.Insert( Vector( 5250.353516, 159.675873, 5055.750000 ) );
		location = new ExpansionSpawnLocation( "Kula", positions );
		SpawnLocations.Insert( location );
		positions.Clear();

        positions.Insert( Vector( 6486.550781, 103.465195, 5980.384766 ) );
        positions.Insert( Vector( 6601.896973, 95.890488, 5908.692383 ) );
        positions.Insert( Vector( 6801.812988, 83.851158, 5898.941895 ) );
        positions.Insert( Vector( 6897.495117, 106.019684, 6030.813965 ) );
        positions.Insert( Vector( 6792.085938, 110.905701, 6158.352539 ) );
        positions.Insert( Vector( 6366.182129, 142.959198, 6382.857910 ) );
        positions.Insert( Vector( 6232.808594, 139.982742, 5965.803711 ) );
        positions.Insert( Vector( 6349.494629, 124.625175, 5962.504883 ) );
        positions.Insert( Vector( 6164.781250, 143.901749, 6103.173340 ) );
        positions.Insert( Vector( 6079.798340, 113.399956, 6272.316406 ) );
        positions.Insert( Vector( 6236.822266, 143.239914, 6384.316895 ) );
		location = new ExpansionSpawnLocation( "Gulash", positions );
		SpawnLocations.Insert( location );
		positions.Clear();

        positions.Insert( Vector( 6214.985840, 196.867386, 6753.580566 ) );
        positions.Insert( Vector( 6448.906250, 226.074799, 6706.618164 ) );
        positions.Insert( Vector( 6469.884766, 208.480209, 6953.398438 ) );
        positions.Insert( Vector( 6415.810059, 200.270554, 7123.985840 ) );
        positions.Insert( Vector( 6324.077148, 209.368042, 7236.364746 ) );
        positions.Insert( Vector( 6245.438965, 212.855011, 7221.653809 ) );
        positions.Insert( Vector( 6098.721191, 220.719238, 7149.075684 ) );
        positions.Insert( Vector( 5908.775879, 218.342422, 6970.965332 ) );
        positions.Insert( Vector( 5969.068359, 220.182510, 6833.822754 ) );
        positions.Insert( Vector( 6095.966309, 203.902664, 6624.937012 ) );
		location = new ExpansionSpawnLocation( "Lug", positions );
		SpawnLocations.Insert( location );
		positions.Clear();

        positions.Insert( Vector( 7569.151367, 153.567520, 7041.453125 ) );
        positions.Insert( Vector( 7519.393066, 160.912994, 6673.455566 ) );
        positions.Insert( Vector( 7882.391113, 144.499191, 6657.431152 ) );
        positions.Insert( Vector( 8067.430176, 158.169617, 6990.247070 ) );
        positions.Insert( Vector( 7961.088379, 149.771851, 7038.036133 ) );
        positions.Insert( Vector( 7734.949707, 150.316696, 7076.026367 ) );
        positions.Insert( Vector( 7566.331055, 155.758224, 7064.740234 ) );
        positions.Insert( Vector( 7432.634277, 151.480347, 7056.217773 ) );
		location = new ExpansionSpawnLocation( "Posetra", positions );
		SpawnLocations.Insert( location );
		positions.Clear();

        positions.Insert( Vector( 7665.729980, 244.999557, 8097.007813 ) );
        positions.Insert( Vector( 9571.978516, 263.761627, 7225.784180 ) );
        positions.Insert( Vector( 9254.591797, 241.796234, 7378.860352 ) );
        positions.Insert( Vector( 9555.787109, 247.374786, 7374.302734 ) );
        positions.Insert( Vector( 7863.500977, 223.043015, 8513.747070 ) );
        positions.Insert( Vector( 10406.526367, 185.784714, 8016.970703 ) );
        positions.Insert( Vector( 10367.518555, 101.088333, 7714.915039 ) );
        positions.Insert( Vector( 9956.348633, 158.499130, 7899.201660 ) );
		location = new ExpansionSpawnLocation( "Woods", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionSpawnsEsseker::Defaults - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionSpawnsExpansionTest
	// ------------------------------------------------------------
	void ExpansionSpawnsExpansionTest()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionSpawnsExpansionTest::Defaults - Start");
		#endif
		
		ref array<vector> positions = new array<vector>;
		ref ExpansionSpawnLocation location;

		EnableSpawnSelection = true;

		//! Airstrip
		positions.Insert( Vector( 1072.486816, 43.562496 ,1101.662109 ) );
		location = new ExpansionSpawnLocation( "Airstrip", positions );
		SpawnLocations.Insert( location );
		positions.Clear();

		//! Watchtower
		positions.Insert( Vector( 932.087341, 80.662506, 1399.989014 ) );
		location = new ExpansionSpawnLocation( "Watchtower", positions );
		SpawnLocations.Insert( location );
		positions.Clear();

		//! Village
		positions.Insert( Vector( 794.725647, 43.832951, 1165.473389 ) );
		location = new ExpansionSpawnLocation( "Village", positions );
		SpawnLocations.Insert( location );
		positions.Clear();

		//! Port
		positions.Insert( Vector( 928.760437, 6.862098, 792.848206 ) );
		location = new ExpansionSpawnLocation( "Port", positions );
		SpawnLocations.Insert( location );
		positions.Clear();

		//! Lake
		positions.Insert( Vector( 885.015503, 44.101490, 1263.654053 ) );
		location = new ExpansionSpawnLocation( "Lake", positions );
		SpawnLocations.Insert( location );
		positions.Clear();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionSpawnsExpansionTest::Defaults - End");
		#endif
	}
	
	override string SettingName()
	{
		return "Spawn Settings";
	}
};