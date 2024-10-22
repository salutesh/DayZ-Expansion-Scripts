/**
 * ExpansionMissionEventBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMissionEventBase
{
	static ref ScriptInvoker SI_OnMissionEnd = new ScriptInvoker();

	int m_Version;

	bool Enabled;

	float Weight;
	int MissionMaxTime;

	string MissionName;

	int Difficulty; // GUI ONLY. 0 - Easy, 1 - Medium, 2 - Hard
	int Objective; // GUI ONLY. 0 - Loot, 1 - Capture, 2 - ?
	string Reward; // GUI ONLY

	[NonSerialized()]
	ExpansionMissionMeta m_MissionMeta;
	
	[NonSerialized()]
	protected string m_EventName;

	[NonSerialized()]
	protected string m_FileName;

	[NonSerialized()]
	protected float m_CurrentMissionTime;

	[NonSerialized()]
	private bool m_IsRunning;

	[NonSerialized()]
	static ref map < typename, int > s_LocationsCount = new map < typename, int >;

	[NonSerialized()]
	static ref map < typename, ref array < ref ExpansionLocatorArray > > s_AvailableLocations = new map < typename, ref array < ref ExpansionLocatorArray > >;

	[NonSerialized()]
	static ref map < typename, ref array < ref ExpansionLocatorArray > > s_SelectedLocations = new map < typename, ref array < ref ExpansionLocatorArray > >;
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase Constructor
	// ------------------------------------------------------------
	void ExpansionMissionEventBase()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase Destructor
	// ------------------------------------------------------------
	void ~ExpansionMissionEventBase()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
		
		if ( GetGame() && GetGame().IsServer() )
		{
			if ( m_IsRunning )
			{
				End();
			}
		}
	}

	void Copy(ExpansionMissionEventBase mission)
	{
		Enabled = mission.Enabled;

		Weight = mission.Weight;
		MissionMaxTime = mission.MissionMaxTime;

		MissionName = mission.MissionName;

		Difficulty = mission.Difficulty;
		Objective = mission.Objective;
		Reward = mission.Reward;
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase OnLoadMission
	// ------------------------------------------------------------
	protected void OnLoadMission()
	{
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase OnSaveMission
	// ------------------------------------------------------------
	protected void OnSaveMission()
	{
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase OnDefaultMission
	// ------------------------------------------------------------
	protected string OnDefaultMission( int index )
	{
	}

	protected string DefaultMission( int index )
	{
	}

	protected string RandomMission(int idx, out ExpansionLocatorArray loc)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif

		typename type = Type();

		array<ref ExpansionLocatorArray> availableLocs = s_AvailableLocations[type];

		if ( availableLocs.Count() == 0 )
		{
			//! Fall back to defaults
			return DefaultMission( idx );
		}

		Weight = 1;  //! As locations are chosen randomly, set all weights equal

		int locIdx;

		array<ref ExpansionLocatorArray> selectedLocs = s_SelectedLocations[type];

		if ( selectedLocs.Count() == 0 )
		{
			//! Pick a random first location
			locIdx = availableLocs.GetRandomIndex();
		} else
		{
			//! Search for a location that is at least some distance from already picked locations
			array< int > candidates = new array< int >;

			int minDistance = 1500;

			while ( selectedLocs.Count() + candidates.Count() < MaxDefaultMissions() )
			{
				foreach ( int i, auto availableLoc: availableLocs )
				{
					foreach ( int j, auto selectedLoc: selectedLocs )
					{
						float distance = vector.Distance( availableLoc.position, selectedLoc.position );

						if ( distance > minDistance )
						{
							candidates.Insert( i );
						}
					}
				}

				if ( minDistance == 0 )
					break;

				//! In case we didn't find enough candidates, reduce min distance and try again
				minDistance -= 250;
			}

			locIdx = candidates.GetRandomElement();
		}

		loc = availableLocs[locIdx];

		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXPrint(this, "OnDefaultMission - " + loc.name);
		#endif

		selectedLocs.Insert( loc );

		availableLocs.Remove( locIdx );

		if (MissionName)
			MissionName += "_" + loc.classname;
		else
			MissionName = loc.classname;

		return MissionName;
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase MaxDefaultMissions
	// ------------------------------------------------------------
	int MaxDefaultMissions()
	{
		typename type = Type();

		if ( !s_LocationsCount.Contains(type) )
		{
			//! Get possible locations from world config
			array< ref ExpansionLocatorArray > locs = ExpansionLocatorStatic.GetWorldLocations();

			array< ref ExpansionLocatorArray > availableLocs = {};

			foreach (auto loc: locs)
			{
				if ( loc.type.Contains( "Capital" ) || loc.type.Contains( "City" ) || loc.type.Contains( "Village" ) )
				{
					availableLocs.Insert( loc );
				}
			}

			s_LocationsCount[type] = availableLocs.Count();
			s_AvailableLocations[type] = availableLocs;
			s_SelectedLocations[type] = {};
		}

		return Math.Min( s_LocationsCount[type], 13 );
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase LoadDefault
	// ------------------------------------------------------------
	string LoadDefault( int index )
	{
		Enabled = true;

		Weight = 5;
		MissionMaxTime = 600;

		Difficulty = 0;
		Objective = 0;
		Reward = "";

		string name = m_EventName + "_" + OnDefaultMission( index );

		m_FileName = EXPANSION_MISSIONS_FOLDER + name + ".json";
		
		SaveMission();

		return name;
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase GetPath
	// ------------------------------------------------------------
	string GetPath()
	{
		return m_FileName;
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase LoadMission
	// ------------------------------------------------------------
	void LoadMission( string file )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
		
		if ( GetGame().IsServer() )
		{
			m_FileName = file;
	
			OnLoadMission();
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase SaveMission
	// ------------------------------------------------------------
	void SaveMission()
	{	
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
		
		if ( GetGame().IsServer() )
		{
			OnSaveMission();
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase CreateNotification
	// ------------------------------------------------------------
	void CreateNotification(  StringLocaliser text, string icon, float time = 3, PlayerIdentity identity = NULL )
	{
		ExpansionNotification(new StringLocaliser("STR_EXPANSION_MISSION_NOTIF_TITLE", m_EventName), text, icon, COLOR_EXPANSION_NOTIFICATION_MISSION, time).Create(identity);
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase IsRunning
	// ------------------------------------------------------------
	bool IsRunning()
	{
		return m_IsRunning;
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase GetElapsedTime
	// ------------------------------------------------------------
	float GetElapsedTime()
	{
		return m_CurrentMissionTime;
	}

	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase GetElapsedTime
	// ------------------------------------------------------------
	// Missions can end before the max time runs out so this may not be the right option for you
	float GetMaxRemainingTime()
	{
		return MissionMaxTime - m_CurrentMissionTime;
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase Start
	// ------------------------------------------------------------
	void Start()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
		
		if ( GetGame().IsServer() )
		{
			m_IsRunning = true;
			m_CurrentMissionTime = 0;
	
			Event_OnStart();
	
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( OnUpdate, 1000, true, 1.0 );
		}
	}
	
	bool CanEnd()
	{
		return true;
	}

	// ------------------------------------------------------------
	// ExpansionMissionEventBase End
	// ------------------------------------------------------------
	void End()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
		
		if ( GetGame().IsServer() )
		{
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Remove( OnUpdate );
	
			SI_OnMissionEnd.Invoke( this );
	
			m_IsRunning = false;
			
			Event_OnEnd();
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase OnUpdate
	// ------------------------------------------------------------
	void OnUpdate( float delta )
	{
		if ( GetGame().IsServer() )
		{
			m_CurrentMissionTime += delta;
	
			Event_OnUpdate( delta );
	
			if ( m_CurrentMissionTime >= MissionMaxTime && CanEnd() )
			{
				End();
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase Event_OnStart
	// ------------------------------------------------------------
	// handle mission start
	void Event_OnStart()
	{
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase Event_OnEnd
	// ------------------------------------------------------------
	// handle mission cleanup
	void Event_OnEnd()
	{
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase Event_OnUpdate
	// ------------------------------------------------------------
	// update tick for the mission
	void Event_OnUpdate( float delta )
	{
	}
};
