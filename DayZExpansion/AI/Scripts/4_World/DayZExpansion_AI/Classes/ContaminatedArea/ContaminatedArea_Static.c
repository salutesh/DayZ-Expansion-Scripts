modded class ContaminatedArea_Static
{
	#ifdef EXPANSIONMODNAVIGATION
	ExpansionMarkerModule m_MarkerModule;
	ExpansionMarkerData m_ServerMarker;
	#endif

	eAIDynamicPatrol m_ExpansionAIPatrol;

	void ContaminatedArea_Static()
	{
		#ifdef EXPANSIONMODNAVIGATION
		CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule);
		#endif
	}

    static string EventType()
    {
        return "ContaminatedArea_Static";
    }

	override void EEInit()
	{
		super.EEInit();
        
		if ( !m_ExpansionAIPatrol )
		{
			m_ExpansionAIPatrol = PatrolManager().InitCrashPatrolSpawner(EventType(), GetPosition());

			#ifdef EXPANSIONMODNAVIGATION
			if ( !m_MarkerModule )
				return;
			
			m_ServerMarker = m_MarkerModule.CreateServerMarker( EventType(), "Radiation", GetPosition(), ARGB(255, 120, 30, 180), true );
			#endif
		}
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete( parent );

		#ifdef EXPANSIONMODNAVIGATION
		if ( !m_ServerMarker )
			return;
		
		m_MarkerModule.RemoveServerMarker( m_ServerMarker.GetUID() );
		#endif

		if ( m_ExpansionAIPatrol )
			m_ExpansionAIPatrol.Delete();
	}

	void ~ContaminatedArea_Static()
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
