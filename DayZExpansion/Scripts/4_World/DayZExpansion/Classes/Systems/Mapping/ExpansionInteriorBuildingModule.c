/**
 * ExpansionInteriorBuildingModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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

[CF_RegisterModule(ExpansionInteriorBuildingModule)]
class ExpansionInteriorBuildingModule: CF_ModuleWorld
{
	autoptr array< ref IviesPosition > m_WhereIviesObjectsSpawn;
	
	//string is classname of the object, and bool, to know if it has collision or not
	autoptr map<string, bool> m_CachedCollision;
	protected int m_LastSavedCount;
 	
	// ------------------------------------------------------------
	// ExpansionInteriorBuildingModule Constructor
	// ------------------------------------------------------------
	void ExpansionInteriorBuildingModule()
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, this);
#endif

		m_CachedCollision = new map<string, bool>;

		ExpansionSettings.SI_General.Insert( OnSettingsUpdated );
	}
 	
	// ------------------------------------------------------------
	// ExpansionInteriorBuildingModule Destructor
	// ------------------------------------------------------------
	void ~ExpansionInteriorBuildingModule()
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, this);
#endif
		
		ExpansionSettings.SI_General.Remove( OnSettingsUpdated );
	}
	
	// ------------------------------------------------------------
	// Expansion OnInit
	// ------------------------------------------------------------
	override void OnInit()
	{
		super.OnInit();

		EnableMissionStart();
	}
 	
 	override void OnMissionStart(Class sender, CF_EventArgs args)
 	{
		super.OnMissionStart(sender, args);

		BuildingBase.s_Expansion_LoadCustomObjectsDelay = 1000;

		LoadCachedCollisions();
	}

	void OnSettingsUpdated()
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, this);
#endif

		if (!GetGame().IsDedicatedServer())
		{
			LoadIviesPositions();
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SaveCachedCollisions, 20000, false);
		}
	}
	
	private void GetIviesPositions(out TVectorArray iviesPosition)
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, this);
#endif

		//! Set default markers depending on map name
		string world_name = "empty";
		GetGame().GetWorldName(world_name);
		world_name.ToLower();
		
		if ( world_name.IndexOf("gloom") == world_name.Length() - 5 )
		{
			world_name = world_name.Substring(0, world_name.Length() - 5);
		}
		
		switch (world_name)
		{
			case "chernarusplus":
				GetChernarusPlusIviesPositions( iviesPosition );
			break;
			case "enoch":
				GetEnochIviesPositions( iviesPosition ); //! TODO
			break;
			case "deerisle":
				GetDeerIsleIviesPositions( iviesPosition ); //! TODO
			break;
			case "namalsk":
				GetNamalskIviesPositions( iviesPosition ); //! TODO
			break;
			case "chiemsee":
				GetChiemseeIviesPositions( iviesPosition ); //! TODO
			break;
			case "rostow":
				GetRostowIviesPositions( iviesPosition ); //! TODO
			break;
			case "esseker":
				GetEssekerIviesPositions( iviesPosition ); //! TODO
			break;
			case "valning":
				GetValningIviesPositions( iviesPosition ); //! TODO
			break;
			case "banov":
				GetBanovIviesPositions( iviesPosition ); //! TODO
			break;
			case "sandbox":
				GetSandboxIviesPositions( iviesPosition );
			break;
			case "expansiontest":
				GetExpansionTestIviesPositions( iviesPosition ); //! TODO
			break;
		}
	}

	void GetChernarusPlusIviesPositions(out TVectorArray iviesPosition)
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

	void GetEnochIviesPositions(out TVectorArray iviesPosition)
	{

	}

	void GetDeerIsleIviesPositions(out TVectorArray iviesPosition)
	{

	}

	void GetNamalskIviesPositions(out TVectorArray iviesPosition)
	{

	}

	void GetChiemseeIviesPositions(out TVectorArray iviesPosition)
	{

	}

	void GetRostowIviesPositions(out TVectorArray iviesPosition)
	{

	}

	void GetEssekerIviesPositions(out TVectorArray iviesPosition)
	{

	}

	void GetValningIviesPositions(out TVectorArray iviesPosition)
	{

	}

	void GetBanovIviesPositions(out TVectorArray iviesPosition)
	{

	}

	void GetSandboxIviesPositions(out TVectorArray iviesPosition)
	{

	}

	void GetExpansionTestIviesPositions(out TVectorArray iviesPosition)
	{

	}

	private void LoadIviesPositions()
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, this);
#endif

		if ( !GetExpansionSettings().GetGeneral() )
			return;

		if ( !GetExpansionSettings().GetGeneral().Mapping.BuildingIvys )
			return;

		if ( !m_WhereIviesObjectsSpawn ) {
			m_WhereIviesObjectsSpawn = new array< ref IviesPosition >;
			
			TVectorArray positions = new TVectorArray;
			GetIviesPositions(positions);
			
			for (int i = 0; i < positions.Count(); ++i) {
				IviesPosition iviesPosition = new IviesPosition;
				iviesPosition.position = positions[i];
				iviesPosition.radius = 5;
				
				m_WhereIviesObjectsSpawn.Insert( iviesPosition );
			}
		}
	}
	
	void SaveCachedCollisions()
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, this);
#endif

		if (m_LastSavedCount == m_CachedCollision.Count())
			return;

		FileSerializer file = new FileSerializer;
			
		if (file.Open(EXPANSION_TEMP_INTERIORS, FileMode.WRITE ))
		{
			file.Write(m_CachedCollision);
			file.Close();
		}

		m_LastSavedCount = m_CachedCollision.Count();
	}
	
	void LoadCachedCollisions()
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, this);
#endif

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
	
	bool ShouldIvySpawn(vector position)
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, this);
#endif

		// If Ivys are disabled in settings, this will be NULL
		if ( m_WhereIviesObjectsSpawn )
		{
			for (int i = 0; i < m_WhereIviesObjectsSpawn.Count(); i++)
				if (vector.Distance(m_WhereIviesObjectsSpawn[i].position, position) <= m_WhereIviesObjectsSpawn[i].radius)
					return true;
		}
		
		return false;
	}
}