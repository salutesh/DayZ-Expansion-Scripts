class ExpansionMissionEventSoldier extends ExpansionMissionEventBase
{    
	autoptr array< ref ExpansionMissionEventSoldierObject > Objects;
	autoptr array< ref ExpansionMissionEventSoldierLoot > Loot;

	[NonSerialized()]
	ExpansionMissionEventSoldierContainer Container;

	[NonSerialized()]
	private autoptr array< Object > m_MissionObjects;

	[NonSerialized()]
	private autoptr array< ExpansionSoldierBase > m_MissionAI;

	[NonSerialized()]
	private ItemBase m_Container;

	// ------------------------------------------------------------
	// Expansion ExpansionMissionEventSoldier
	// ------------------------------------------------------------
	void ExpansionMissionEventSoldier()
	{
		m_EventName = "Soldiers";
	}
	
	// handle mission start
	override void Event_OnStart()
	{
		m_Container = ItemBase.Cast( SpawnObjectEx( Container.ContainerName, Container.Position, Container.Orientation, true ) );
		
		if ( !m_Container )
			End();

		m_Container.Open();
    }

	// handle mission cleanup
	override void Event_OnEnd()
	{
	}

	// update tick for the mission
	override void Event_OnUpdate( float delta )
	{
	}

	protected override void OnLoadMission()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMissionEventSoldier::OnLoadMission");
		#endif

		JsonFileLoader<ExpansionMissionEventSoldier>.JsonLoadFile( m_FileName, this );
	}

	protected override void OnSaveMission()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMissionEventSoldier::OnSaveMission");
		#endif

		JsonFileLoader<ExpansionMissionEventSoldier>.JsonSaveFile( m_FileName, this );
	}

	override int MaxDefaultMissions()
	{
		return 1;
	}

	protected override string OnDefaultMission( int index )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMissionEventSoldier::OnDefaultMission");
		#endif
		
		Enabled = false;
		
		switch ( index )
        {
        default:
        case 0:
			Weight = 5;
			MissionName = "Vybor";
			break;
		}

		string fname = MissionName;
		fname.Replace( " ", "-" );
        return fname;
	}
}