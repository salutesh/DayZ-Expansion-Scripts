#ifdef EXPANSIONMODAI_ENABLEPOLICEWRECK
#ifdef GAMELABS
modded class Land_Wreck_sed01_aban1_police
#else
class Land_Wreck_sed01_aban1_police extends BuildingSuper
#endif
{
	#ifdef DIAG
	#ifdef EXPANSIONMODNAVIGATION
	ExpansionMarkerModule m_MarkerModule;
	ExpansionMarkerData m_ServerMarker;
	#endif
	#endif

	eAIDynamicPatrol m_ExpansionAIPatrol;

	void Land_Wreck_sed01_aban1_police()
	{
		#ifdef DIAG
		#ifdef EXPANSIONMODNAVIGATION
		CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule);
		#endif
		#endif

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DelayedInit, 2000, false);
	}

    static string EventType()
    {
        return "Wreck_PoliceCar";
    }

	void DelayedInit()
	{
		if ( !m_ExpansionAIPatrol )
		{
			m_ExpansionAIPatrol = PatrolManager().InitCrashPatrolSpawner(EventType(), GetPosition());

			#ifdef DIAG
			#ifdef EXPANSIONMODNAVIGATION
			if ( !m_MarkerModule )
				return;
			
			m_ServerMarker = m_MarkerModule.CreateServerMarker( EventType(), "Vehicle Crash", GetPosition(), ARGB(255, 50, 235, 90), true );
			#endif
			#endif
		}
	}

	void ~Land_Wreck_sed01_aban1_police()
	{
		#ifdef DIAG
		#ifdef EXPANSIONMODNAVIGATION
		if ( m_ServerMarker && m_MarkerModule )
			m_MarkerModule.RemoveServerMarker( m_ServerMarker.GetUID() );
		#endif
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
	#ifdef DIAG
	#ifdef EXPANSIONMODNAVIGATION
	ExpansionMarkerModule m_MarkerModule;
	ExpansionMarkerData m_ServerMarker;
	#endif
	#endif

	eAIDynamicPatrol m_ExpansionAIPatrol;

	void Land_Wreck_sed01_aban2_police()
	{
		#ifdef DIAG
		#ifdef EXPANSIONMODNAVIGATION
		CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule);
		#endif
		#endif

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DelayedInit, 2000, false);
	}

    static string EventType()
    {
        return "Wreck_PoliceCar";
    }

	void DelayedInit()
	{
		if ( !m_ExpansionAIPatrol )
		{
			m_ExpansionAIPatrol = PatrolManager().InitCrashPatrolSpawner(EventType(), GetPosition());

			#ifdef DIAG
			#ifdef EXPANSIONMODNAVIGATION
			if ( !m_MarkerModule )
				return;
			
			m_ServerMarker = m_MarkerModule.CreateServerMarker( EventType(), "Vehicle Crash", GetPosition(), ARGB(255, 50, 235, 90), true );
			#endif
			#endif
		}
	}

	void ~Land_Wreck_sed01_aban2_police()
	{
		#ifdef DIAG
		#ifdef EXPANSIONMODNAVIGATION
		if ( m_ServerMarker && m_MarkerModule )
			m_MarkerModule.RemoveServerMarker( m_ServerMarker.GetUID() );
		#endif
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
	#ifdef DIAG
	#ifdef EXPANSIONMODNAVIGATION
	ExpansionMarkerModule m_MarkerModule;
	ExpansionMarkerData m_ServerMarker;
	#endif
	#endif

	eAIDynamicPatrol m_ExpansionAIPatrol;

	void Land_Wreck_hb01_aban1_police()
	{
		#ifdef DIAG
		#ifdef EXPANSIONMODNAVIGATION
		CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule);
		#endif
		#endif

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DelayedInit, 2000, false);
	}

    static string EventType()
    {
        return "Wreck_PoliceCar";
    }

	void DelayedInit()
	{
		if ( !m_ExpansionAIPatrol )
		{
			m_ExpansionAIPatrol = PatrolManager().InitCrashPatrolSpawner(EventType(), GetPosition());

			#ifdef DIAG
			#ifdef EXPANSIONMODNAVIGATION
			if ( !m_MarkerModule )
				return;
			
			m_ServerMarker = m_MarkerModule.CreateServerMarker( EventType(), "Vehicle Crash", GetPosition(), ARGB(255, 50, 235, 90), true );
			#endif
			#endif
		}
	}

	void ~Land_Wreck_hb01_aban1_police()
	{
		#ifdef DIAG
		#ifdef EXPANSIONMODNAVIGATION
		if ( m_ServerMarker && m_MarkerModule )
			m_MarkerModule.RemoveServerMarker( m_ServerMarker.GetUID() );
		#endif
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
	#ifdef DIAG
	#ifdef EXPANSIONMODNAVIGATION
	ExpansionMarkerModule m_MarkerModule;
	ExpansionMarkerData m_ServerMarker;
	#endif
	#endif

	eAIDynamicPatrol m_ExpansionAIPatrol;

	void Land_Wreck_hb01_aban2_police()
	{
		#ifdef DIAG
		#ifdef EXPANSIONMODNAVIGATION
		CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule);
		#endif
		#endif

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DelayedInit, 2000, false);
	}

    static string EventType()
    {
        return "Wreck_PoliceCar";
    }

	void DelayedInit()
	{
		if ( !m_ExpansionAIPatrol )
		{
			m_ExpansionAIPatrol = PatrolManager().InitCrashPatrolSpawner(EventType(), GetPosition());

			#ifdef DIAG
			#ifdef EXPANSIONMODNAVIGATION
			if ( !m_MarkerModule )
				return;
			
			m_ServerMarker = m_MarkerModule.CreateServerMarker( EventType(), "Vehicle Crash", GetPosition(), ARGB(255, 50, 235, 90), true );
			#endif
			#endif
		}
	}

	void ~Land_Wreck_hb01_aban2_police()
	{
		#ifdef DIAG
		#ifdef EXPANSIONMODNAVIGATION
		if ( m_ServerMarker && m_MarkerModule )
			m_MarkerModule.RemoveServerMarker( m_ServerMarker.GetUID() );
		#endif
		#endif

		if ( m_ExpansionAIPatrol )
		{
			m_ExpansionAIPatrol.Despawn();
			m_ExpansionAIPatrol.Delete();
		}
	}
};
#endif