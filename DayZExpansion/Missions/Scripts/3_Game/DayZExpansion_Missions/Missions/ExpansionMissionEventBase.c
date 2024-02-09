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

static ref ScriptInvoker SI_OnMissionEnd = new ScriptInvoker();

class ExpansionMissionEventBase
{
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
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase MaxDefaultMissions
	// ------------------------------------------------------------
	int MaxDefaultMissions()
	{
		return 1;
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
