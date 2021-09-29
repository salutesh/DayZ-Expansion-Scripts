class ExpansionZoneItemCleanup : ExpansionZoneActor
{
	ItemBase m_Item;
	int m_Stage;
	int m_LifeTime;
	int m_Time;

	void ExpansionZoneItemCleanup(ItemBase item)
	{
		m_Item = item;

		m_Item.SetLifetime(GetExpansionSettings().GetSafeZone().ItemLifetimeInSafeZone);
	}

	override void OnUpdate()
	{
		if (m_Stage == 0)
		{
			if (m_Item.GetLifetime() < 0)
			{
				m_Stage = 1;
				m_LifeTime = 0;
			}

			return;
		}

		if (m_Item.GetLifetime() > 0)
		{
			m_Stage = 0;
			return;
		}

		int prev = m_Time;
		m_Time = GetGame().GetTime();
		m_LifeTime += prev - m_Time;

		if (m_LifeTime > 10000)
		{
			GetGame().ObjectDelete(m_Item);
		}
	}
};
