modded class CrashBase
{
	#ifdef EAI_DEBUG_EVENTPATROL
	ExpansionMarkerModule m_MarkerModule;
	ExpansionMarkerData m_ServerMarker;
	#endif

	eAIDynamicPatrol m_ExpansionAIPatrol;

    string EventType()
    {
        return "";
    }

	override void EEOnCECreate()
	{
		auto trace = EXTrace.Start(EXTrace.AI, this);

		super.EEOnCECreate();

		eAI_CrashPatrol_Init();
	}

	override void AfterStoreLoad()
	{
		auto trace = EXTrace.Start(EXTrace.AI, this);

		super.AfterStoreLoad();

		eAI_CrashPatrol_Init();
	}

	void eAI_CrashPatrol_Init()
	{
		if ( !m_ExpansionAIPatrol )
		{
			m_ExpansionAIPatrol = PatrolManager().InitCrashPatrolSpawner(EventType(), GetPosition());

			#ifdef EAI_DEBUG_EVENTPATROL
			if ( !CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule) )
				return;
			
			m_ServerMarker = m_MarkerModule.CreateServerMarker( EventType(), "Helicopter", GetPosition(), ARGB(255, 235, 50, 90), true );
			#endif
		}
	}

	void ~CrashBase()
	{
		auto trace = EXTrace.Start(EXTrace.AI, this);

		if (!GetGame() || !GetGame().IsServer())
			return;

		#ifdef EAI_DEBUG_EVENTPATROL
		if ( m_ServerMarker && m_MarkerModule )
			m_MarkerModule.RemoveServerMarker( m_ServerMarker.GetUID() );
		#endif

		if ( m_ExpansionAIPatrol )
		{
			m_ExpansionAIPatrol.Despawn();
			m_ExpansionAIPatrol.Delete();
		}
	}
};

modded class Wreck_UH1Y
{
    override string EventType()
    {
        return "Wreck_UH1Y";
    }
};

modded class Wreck_Mi8_Crashed
{
    override string EventType()
    {
        return "Wreck_Mi8";
    }
};


modded class Wreck_Mi8
{
    override string EventType()
    {
        return "Wreck_Mi8";
    }
};

