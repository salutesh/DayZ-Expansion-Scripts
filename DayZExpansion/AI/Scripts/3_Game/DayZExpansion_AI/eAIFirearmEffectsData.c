class eAIFirearmEffectsData: ImpactEffectsData
{
	EntityAI m_Source;
	int m_TimeOut;

	void eAIFirearmEffectsData(EntityAI source, float timeOut)
	{
		m_Source = source;
		m_TimeOut = timeOut;
	}
}
