/**
 * ExpansionWorldMappingModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class IviesPosition
{
	vector		position;
	float   	radius;	
}

class ExpansionInteriorBuildingModule: JMModuleBase
{
	protected bool m_IsUnloadingInteriors;
	protected bool m_IsLoadingInteriors;
	
	autoptr array< ref IviesPosition > m_WhereIviesObjectsSpawn;
	
	autoptr array< Object > m_InteriorObjects;
	
	//string is classname of the object, and bool, to know if it has collision or not
	autoptr map<string, bool> m_CachedCollision;
	
	//Use multimap so you can get log(n) for type and n for position
	protected autoptr multiMap<string, vector> m_AllSpawnedPositions;
	
	protected bool m_LastLoad;
 	
	// ------------------------------------------------------------
	// ExpansionInteriorBuildingModule Constructor
	// ------------------------------------------------------------
	void ExpansionInteriorBuildingModule()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionInteriorBuildingModule::ExpansionInteriorBuildingModule - Start");
		#endif
		
		m_IsUnloadingInteriors = false;
		m_IsLoadingInteriors = false;
		
		m_InteriorObjects = new array< Object >;
		
		m_CachedCollision = new map<string, bool>;
		
		m_AllSpawnedPositions = new multiMap<string, vector>;
		
		m_LastLoad = false;
		
		LoadIviesPositions();
		
		ExpansionSettings.SI_General.Insert( OnSettingsUpdated );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionInteriorBuildingModule::ExpansionInteriorBuildingModule - End");
		#endif
	}
 	
	// ------------------------------------------------------------
	// ExpansionInteriorBuildingModule Destructor
	// ------------------------------------------------------------
	void ~ExpansionInteriorBuildingModule()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionInteriorBuildingModule::~ExpansionInteriorBuildingModule - Start");
		#endif
		
		ExpansionSettings.SI_General.Remove( OnSettingsUpdated );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionInteriorBuildingModule::~ExpansionInteriorBuildingModule - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion OnInit
	// ------------------------------------------------------------
	override void OnInit()
	{
		super.OnInit();
	}
	
	// ------------------------------------------------------------
	// Expansion OnMissionFinish
	// ------------------------------------------------------------
	override void OnMissionFinish()
	{
		super.OnMissionFinish();
		
		UnloadAllInteriors();
		
		//just to be sure it's cleared correctly
		m_InteriorObjects.Clear();
		m_AllSpawnedPositions.Clear();
		
		//Reset to default value
		m_LastLoad = false;
	}

	// ------------------------------------------------------------
	// Expansion OnSettingsUpdated
	// ------------------------------------------------------------
	override void OnSettingsUpdated()
	{
		if ( !GetExpansionSettings().GetGeneral() )
			return;

		if ( g_Game.IsLoading() )
			return;

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionInteriorBuildingModule::OnSettingsUpdated - Start");
		#endif
		
		bool currentLoad = GetExpansionSettings().GetGeneral().Mapping.BuildingInteriors;
		
		if ( m_LastLoad != currentLoad )
		{
			if ( currentLoad )
			{
				LoadAllInteriors();
			} else
			{
				UnloadAllInteriors();
			}
			
			m_LastLoad = currentLoad;
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionInteriorBuildingModule::OnSettingsUpdated - End");
		#endif
	}

	void LoadAllInteriors()
	{
		if ( m_IsLoadingInteriors || m_IsUnloadingInteriors || !GetGame() )
			return;
		
		m_IsLoadingInteriors = true;

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionInteriorBuildingModule::LoadAllInteriors - Start");
		#endif

		LoadCachedCollisions();

		for ( int i = 0; i < BuildingBase.m_AllBuldingsInteriors.Count(); ++i )
		{
			BuildingBase currBuilding = BuildingBase.m_AllBuldingsInteriors[i];
			if ( !currBuilding )
				continue;
			
			currBuilding.LoadInterior();
		}

		m_IsLoadingInteriors = false;

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionInteriorBuildingModule::LoadAllInteriors - End "  + m_InteriorObjects.Count());
		#endif
	}
	
	void UnloadAllInteriors()
	{
		if ( m_IsUnloadingInteriors || !GetGame() )
			return;

		m_IsUnloadingInteriors = true;
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionInteriorBuildingModule::UnloadAllInteriors - Start " + m_InteriorObjects.Count());
		#endif
		
		for ( int i = 0; i < m_InteriorObjects.Count(); ++i )
		{
			GetGame().ObjectDelete(m_InteriorObjects[i]);
		}
		
		m_InteriorObjects.Clear();
		m_AllSpawnedPositions.Clear();
		
		m_IsUnloadingInteriors = false;
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionInteriorBuildingModule::UnloadAllInteriors - End");
		#endif
	}
	
	void AddBuildingSpawned( string type, vector pos )
	{
		m_AllSpawnedPositions.Insert( type, pos );
	}
	
	bool AlreadySpawned( string type, vector pos )
	{
		array<vector> allPositions = m_AllSpawnedPositions.Get(type);
		
		if (allPositions)
		{
			for ( int i = 0; i < allPositions.Count(); ++i )
			{
				if (vector.Distance( allPositions[i], pos ) < 0.25)
				{
					return true;
				}
			}
		}
		
		return false;
	}
	
	private void GetIviesPositions(out TVectorArray iviesPosition)
	{
		//Svetloyark
		iviesPosition.Insert("13804 27 13232");
		iviesPosition.Insert("13826 26 13348");
		iviesPosition.Insert("13781 25 13246");
		iviesPosition.Insert("13794 26 13268");
		iviesPosition.Insert("13765 23 13284");
		iviesPosition.Insert("13743 23 13267");
		iviesPosition.Insert("13705 24 13284");
		iviesPosition.Insert("13718 24 13319");
		iviesPosition.Insert("13739 25 13325");
		iviesPosition.Insert("13760 24 13352");
		iviesPosition.Insert("13782 23 13346");
		iviesPosition.Insert("13801 25 13370");
		iviesPosition.Insert("13805 26 13327");
		iviesPosition.Insert("13798 24 13212");
		iviesPosition.Insert("13822 25 13198");
		iviesPosition.Insert("13805 24 13176");
		iviesPosition.Insert("13821 24 13169");
		iviesPosition.Insert("13847 25 13141");
		iviesPosition.Insert("14153 6 13001");
		iviesPosition.Insert("14141 6 13025");
		iviesPosition.Insert("14166 5 13043");
		iviesPosition.Insert("14106 10 13059");
		iviesPosition.Insert("14075 11 13076");
		
		// Kamyshovo 
		iviesPosition.Insert("12011.5 10 3591");
		iviesPosition.Insert("12087 10.7 3589");
		iviesPosition.Insert("12081 8.4 3619");
		iviesPosition.Insert("12074 8.3 3646");
		iviesPosition.Insert("12066 7.44 3670");
		iviesPosition.Insert("12059 8.3 3694");
		iviesPosition.Insert("12050 10 3715");
		iviesPosition.Insert("12040 10 3734");
		iviesPosition.Insert("12049 8.48 3646");
		iviesPosition.Insert("12435 9 3563");
		iviesPosition.Insert("12459 12 3555");
		iviesPosition.Insert("11931 8 3486");
		iviesPosition.Insert("11949 9 3489");
		iviesPosition.Insert("12357 9 3542");
		iviesPosition.Insert("12180 8 3552");
	}
	
	private void LoadIviesPositions()
	{
		if ( !m_WhereIviesObjectsSpawn )
		{
			m_WhereIviesObjectsSpawn = new array< ref IviesPosition >;
			
			TVectorArray positions = new TVectorArray;
			GetIviesPositions(positions);
			
			for(int i = 0; i < positions.Count(); ++i)
			{
				IviesPosition iviesPosition = new IviesPosition;
				iviesPosition.position = positions[i];
				iviesPosition.radius = 5;
				
				m_WhereIviesObjectsSpawn.Insert( iviesPosition );
			}
		}
	}
	
	void SaveCachedCollisions()
	{
		FileSerializer file = new FileSerializer;
			
		if (file.Open(EXPANSION_TEMP_INTERIORS, FileMode.WRITE ))
		{
			file.Write(m_CachedCollision);
			file.Close();
		}
	}
	
	void LoadCachedCollisions()
	{
		if (FileExist(EXPANSION_TEMP_INTERIORS))
		{
			FileSerializer file = new FileSerializer;
			
			if (file.Open(EXPANSION_TEMP_INTERIORS, FileMode.READ ))
			{
				file.Read(m_CachedCollision);
				file.Close();
			}
		}
	}
}