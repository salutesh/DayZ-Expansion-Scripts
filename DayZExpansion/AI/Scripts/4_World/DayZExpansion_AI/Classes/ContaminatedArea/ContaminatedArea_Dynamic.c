modded class ContaminatedArea_Dynamic
{
	#ifdef EAI_DEBUG_EVENTPATROL
	ExpansionMarkerModule m_MarkerModule;
	ExpansionMarkerData m_ServerMarker;
	#endif

	eAIDynamicPatrol m_ExpansionAIPatrol;

    static string EventType()
    {
        return "ContaminatedArea_Dynamic";
    }

	override void EEInit()
	{
		super.EEInit();
        
		if (!GetGame().IsServer())
			return;

		if ( !m_ExpansionAIPatrol )
		{
			m_ExpansionAIPatrol = PatrolManager().InitCrashPatrolSpawner(EventType(), GetPosition());

			#ifdef EAI_DEBUG_EVENTPATROL
			if ( !CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule) )
				return;
			
			m_ServerMarker = m_MarkerModule.CreateServerMarker( EventType(), "Radiation", GetPosition(), ARGB(255, 90, 50, 100), true );
			#endif
		}
	}

	void ~ContaminatedArea_Dynamic()
	{
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
