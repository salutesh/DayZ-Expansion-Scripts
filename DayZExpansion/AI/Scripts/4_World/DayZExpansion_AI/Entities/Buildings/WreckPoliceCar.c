class Land_Wreck_sed01_aban1_police extends BuildingSuper
{
	#ifdef EXPANSIONMODNAVIGATION
	ExpansionMarkerModule m_MarkerModule;
	ExpansionMarkerData m_ServerMarker;
	#endif

	eAIDynamicPatrol m_ExpansionAIPatrol;

	void Land_Wreck_sed01_aban1_police()
	{
		#ifdef EXPANSIONMODNAVIGATION
		CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule);
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

			#ifdef EXPANSIONMODNAVIGATION
			if ( !m_MarkerModule )
				return;
			
			m_ServerMarker = m_MarkerModule.CreateServerMarker( EventType(), "Vehicle Crash", GetPosition(), ARGB(255, 50, 235, 90), true );
			#endif
		}
	}

	void ~Land_Wreck_sed01_aban1_police()
	{
		#ifdef EXPANSIONMODNAVIGATION
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

class Land_Wreck_sed01_aban2_police extends BuildingSuper
{
	#ifdef EXPANSIONMODNAVIGATION
	ExpansionMarkerModule m_MarkerModule;
	ExpansionMarkerData m_ServerMarker;
	#endif

	eAIDynamicPatrol m_ExpansionAIPatrol;

	void Land_Wreck_sed01_aban2_police()
	{
		#ifdef EXPANSIONMODNAVIGATION
		CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule);
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

			#ifdef EXPANSIONMODNAVIGATION
			if ( !m_MarkerModule )
				return;
			
			m_ServerMarker = m_MarkerModule.CreateServerMarker( EventType(), "Vehicle Crash", GetPosition(), ARGB(255, 50, 235, 90), true );
			#endif
		}
	}

	void ~Land_Wreck_sed01_aban2_police()
	{
		#ifdef EXPANSIONMODNAVIGATION
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
class Land_Wreck_hb01_aban1_police extends BuildingSuper
{
	#ifdef EXPANSIONMODNAVIGATION
	ExpansionMarkerModule m_MarkerModule;
	ExpansionMarkerData m_ServerMarker;
	#endif

	eAIDynamicPatrol m_ExpansionAIPatrol;

	void Land_Wreck_hb01_aban1_police()
	{
		#ifdef EXPANSIONMODNAVIGATION
		CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule);
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

			#ifdef EXPANSIONMODNAVIGATION
			if ( !m_MarkerModule )
				return;
			
			m_ServerMarker = m_MarkerModule.CreateServerMarker( EventType(), "Vehicle Crash", GetPosition(), ARGB(255, 50, 235, 90), true );
			#endif
		}
	}

	void ~Land_Wreck_hb01_aban1_police()
	{
		#ifdef EXPANSIONMODNAVIGATION
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
class Land_Wreck_hb01_aban2_police extends BuildingSuper
{
	#ifdef EXPANSIONMODNAVIGATION
	ExpansionMarkerModule m_MarkerModule;
	ExpansionMarkerData m_ServerMarker;
	#endif

	eAIDynamicPatrol m_ExpansionAIPatrol;

	void Land_Wreck_hb01_aban2_police()
	{
		#ifdef EXPANSIONMODNAVIGATION
		CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule);
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

			#ifdef EXPANSIONMODNAVIGATION
			if ( !m_MarkerModule )
				return;
			
			m_ServerMarker = m_MarkerModule.CreateServerMarker( EventType(), "Vehicle Crash", GetPosition(), ARGB(255, 50, 235, 90), true );
			#endif
		}
	}

	void ~Land_Wreck_hb01_aban2_police()
	{
		#ifdef EXPANSIONMODNAVIGATION
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