/**
 * ExpansionMissionEventBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

static ref ScriptInvoker SI_OnMissionEnd = new ScriptInvoker();

class ExpansionMissionEventBase
{
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
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase Destructor
	// ------------------------------------------------------------
	void ~ExpansionMissionEventBase()
	{
		if ( m_IsRunning )
		{
			End();
		}
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
		m_FileName = file;

		OnLoadMission();
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase SaveMission
	// ------------------------------------------------------------
	void SaveMission()
	{
		OnSaveMission();
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase CreateNotification
	// ------------------------------------------------------------
	void CreateNotification( ref StringLocaliser text, string icon, float time = 3 )
	{
		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MISSION_NOTIF_TITLE", m_EventName ), text, icon, COLOR_EXPANSION_NOTIFICATION_MISSION, time );
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase CreateNotification
	// ------------------------------------------------------------
	void CreateNotification( ref StringLocaliser text, string icon, float time, PlayerIdentity identity )
	{
		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MISSION_NOTIF_TITLE", m_EventName ), text, icon, COLOR_EXPANSION_NOTIFICATION_MISSION, time, identity );
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
		m_IsRunning = true;
		m_CurrentMissionTime = 0;

		Event_OnStart();

		GetGame().GetUpdateQueue( CALL_CATEGORY_SYSTEM ).Insert( OnUpdate );
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase End
	// ------------------------------------------------------------
	void End()
	{
		GetGame().GetUpdateQueue( CALL_CATEGORY_SYSTEM ).Remove( OnUpdate );

		SI_OnMissionEnd.Invoke( this );

		m_IsRunning = false;
		
		Event_OnEnd();
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionEventBase OnUpdate
	// ------------------------------------------------------------
	void OnUpdate( float delta )
	{
		m_CurrentMissionTime += delta;

		Event_OnUpdate( delta );

		if ( m_CurrentMissionTime >= MissionMaxTime )
		{
			End();
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
}