class eAIShockHandler : ShockHandler
{
	private float m_eAI_TimeSinceLastTick = VALUE_CHECK_INTERVAL + 1;
	
	override void Update(float deltaT)
	{
		m_eAI_TimeSinceLastTick += deltaT;
		
		if (m_eAI_TimeSinceLastTick > VALUE_CHECK_INTERVAL)
		{
			CheckValue(false);
			m_eAI_TimeSinceLastTick = 0;
		}
	}
};