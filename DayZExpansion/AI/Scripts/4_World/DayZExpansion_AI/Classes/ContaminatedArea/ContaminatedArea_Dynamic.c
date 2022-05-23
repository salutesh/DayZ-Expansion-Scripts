modded class ContaminatedArea_Dynamic
{
	#ifdef EXPANSIONMODNAVIGATION
	ExpansionMarkerModule m_MarkerModule;
	ExpansionMarkerData m_ServerMarker;
	#endif

	eAIDynamicPatrol m_ExpansionAIPatrol;

	void ContaminatedArea_Dynamic()
	{
		#ifdef EXPANSIONMODNAVIGATION
		CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule);
		#endif
	}

    static string EventType()
    {
        return "ContaminatedArea_Dynamic";
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
			
			m_ServerMarker = m_MarkerModule.CreateServerMarker( EventType(), "Radiation", GetPosition(), ARGB(255, 90, 50, 100), true );
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

	void ~ContaminatedArea_Dynamic()
	{
		#ifdef EXPANSIONMODNAVIGATION
		if ( !m_ServerMarker )
			return;
		
		m_MarkerModule.RemoveServerMarker( m_ServerMarker.GetUID() );
		#endif

		if ( m_ExpansionAIPatrol )
		{
			m_ExpansionAIPatrol.Despawn();
			m_ExpansionAIPatrol.Delete();
		}
	}
};
