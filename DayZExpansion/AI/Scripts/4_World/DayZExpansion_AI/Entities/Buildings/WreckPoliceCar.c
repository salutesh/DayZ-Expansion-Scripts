#ifdef EXPANSIONMODAI_ENABLEPOLICEWRECK
#ifdef GAMELABS
modded class Land_Wreck_sed01_aban1_police
#else
class Land_Wreck_sed01_aban1_police extends BuildingSuper
#endif
{
	#ifdef EAI_DEBUG_EVENTPATROL
	ExpansionMarkerModule m_MarkerModule;
	ExpansionMarkerData m_ServerMarker;
	#endif

	eAIDynamicPatrol m_ExpansionAIPatrol;

    static string EventType()
    {
        return "Wreck_PoliceCar";
    }

	override void EEOnCECreate()
	{
		auto trace = EXTrace.Start(EXTrace.AI, this);

		super.EEOnCECreate();

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(eAI_CrashPatrol_Init, 2000, false);
	}

	//! Police car wrecks, unlike heli wrecks, are not saved by default, but in can be enabled via types.xml
	override void AfterStoreLoad()
	{
		auto trace = EXTrace.Start(EXTrace.AI, this);

		super.AfterStoreLoad();

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(eAI_CrashPatrol_Init, 2000, false);
	}

	void eAI_CrashPatrol_Init()
	{
		if ( !m_ExpansionAIPatrol )
		{
			m_ExpansionAIPatrol = PatrolManager().InitCrashPatrolSpawner(EventType(), GetPosition());

			#ifdef EAI_DEBUG_EVENTPATROL
			if ( !CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule) )
				return;
			
			m_ServerMarker = m_MarkerModule.CreateServerMarker( EventType(), "Vehicle Crash", GetPosition(), ARGB(255, 50, 235, 90), true );
			#endif
		}
	}

	void ~Land_Wreck_sed01_aban1_police()
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

#ifdef GAMELABS
modded class Land_Wreck_sed01_aban2_police
#else
class Land_Wreck_sed01_aban2_police extends BuildingSuper
#endif
{
	#ifdef EAI_DEBUG_EVENTPATROL
	ExpansionMarkerModule m_MarkerModule;
	ExpansionMarkerData m_ServerMarker;
	#endif

	eAIDynamicPatrol m_ExpansionAIPatrol;

    static string EventType()
    {
        return "Wreck_PoliceCar";
    }

	override void EEOnCECreate()
	{
		auto trace = EXTrace.Start(EXTrace.AI, this);

		super.EEOnCECreate();

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(eAI_CrashPatrol_Init, 2000, false);
	}

	//! Police car wrecks, unlike heli wrecks, are not saved by default, but in can be enabled via types.xml
	override void AfterStoreLoad()
	{
		auto trace = EXTrace.Start(EXTrace.AI, this);

		super.AfterStoreLoad();

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(eAI_CrashPatrol_Init, 2000, false);
	}

	void eAI_CrashPatrol_Init()
	{
		if ( !m_ExpansionAIPatrol )
		{
			m_ExpansionAIPatrol = PatrolManager().InitCrashPatrolSpawner(EventType(), GetPosition());

			#ifdef EAI_DEBUG_EVENTPATROL
			if ( !CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule) )
				return;
			
			m_ServerMarker = m_MarkerModule.CreateServerMarker( EventType(), "Vehicle Crash", GetPosition(), ARGB(255, 50, 235, 90), true );
			#endif
		}
	}

	void ~Land_Wreck_sed01_aban2_police()
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
#ifdef GAMELABS
modded class Land_Wreck_hb01_aban1_police
#else
class Land_Wreck_hb01_aban1_police extends BuildingSuper
#endif
{
	#ifdef EAI_DEBUG_EVENTPATROL
	ExpansionMarkerModule m_MarkerModule;
	ExpansionMarkerData m_ServerMarker;
	#endif

	eAIDynamicPatrol m_ExpansionAIPatrol;

	void Land_Wreck_hb01_aban1_police()
	{
		if (!GetGame().IsServer())
			return;

		#ifdef EAI_DEBUG_EVENTPATROL
		CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule);
		#endif

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(eAI_CrashPatrol_Init, 2000, false);
	}

    static string EventType()
    {
        return "Wreck_PoliceCar";
    }

	override void EEOnCECreate()
	{
		auto trace = EXTrace.Start(EXTrace.AI, this);

		super.EEOnCECreate();

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(eAI_CrashPatrol_Init, 2000, false);
	}

	//! Police car wrecks, unlike heli wrecks, are not saved by default, but in can be enabled via types.xml
	override void AfterStoreLoad()
	{
		auto trace = EXTrace.Start(EXTrace.AI, this);

		super.AfterStoreLoad();

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(eAI_CrashPatrol_Init, 2000, false);
	}

	void eAI_CrashPatrol_Init()
	{
		if ( !m_ExpansionAIPatrol )
		{
			m_ExpansionAIPatrol = PatrolManager().InitCrashPatrolSpawner(EventType(), GetPosition());

			#ifdef EAI_DEBUG_EVENTPATROL
			if ( !CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule) )
				return;
			
			m_ServerMarker = m_MarkerModule.CreateServerMarker( EventType(), "Vehicle Crash", GetPosition(), ARGB(255, 50, 235, 90), true );
			#endif
		}
	}

	void ~Land_Wreck_hb01_aban1_police()
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
#ifdef GAMELABS
modded class Land_Wreck_hb01_aban2_police
#else
class Land_Wreck_hb01_aban2_police extends BuildingSuper
#endif
{
	#ifdef EAI_DEBUG_EVENTPATROL
	ExpansionMarkerModule m_MarkerModule;
	ExpansionMarkerData m_ServerMarker;
	#endif

	eAIDynamicPatrol m_ExpansionAIPatrol;

    static string EventType()
    {
        return "Wreck_PoliceCar";
    }

	override void EEOnCECreate()
	{
		auto trace = EXTrace.Start(EXTrace.AI, this);

		super.EEOnCECreate();

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(eAI_CrashPatrol_Init, 2000, false);
	}

	//! Police car wrecks, unlike heli wrecks, are not saved by default, but in can be enabled via types.xml
	override void AfterStoreLoad()
	{
		auto trace = EXTrace.Start(EXTrace.AI, this);

		super.AfterStoreLoad();

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(eAI_CrashPatrol_Init, 2000, false);
	}

	void eAI_CrashPatrol_Init()
	{
		if ( !m_ExpansionAIPatrol )
		{
			m_ExpansionAIPatrol = PatrolManager().InitCrashPatrolSpawner(EventType(), GetPosition());

			#ifdef EAI_DEBUG_EVENTPATROL
			if ( !CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule) )
				return;
			
			m_ServerMarker = m_MarkerModule.CreateServerMarker( EventType(), "Vehicle Crash", GetPosition(), ARGB(255, 50, 235, 90), true );
			#endif
		}
	}

	void ~Land_Wreck_hb01_aban2_police()
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
#endif