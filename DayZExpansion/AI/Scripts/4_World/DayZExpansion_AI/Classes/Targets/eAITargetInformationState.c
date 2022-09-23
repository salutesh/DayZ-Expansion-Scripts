class eAITargetInformationState
{
	eAIBase m_AI;
	eAITargetInformation m_Info;

	int m_ThreatLevelUpdateTimestamp;
	float m_ThreatLevel;
	float m_ThreatLevelActive;
	vector m_LastKnownPosition;
	bool m_PositionReached;
	int m_SearchPositionUpdateTimestamp;
	vector m_SearchPosition;
	vector m_SearchDirection;
	bool m_LOS;

	void eAITargetInformationState(eAIBase ai, eAITargetInformation info)
	{
		m_AI = ai;
		m_Info = info;

		UpdateThreat(true);
		UpdatePosition(true);
	}

	void UpdateThreat(bool force = false)
	{
		int time = GetGame().GetTime();
		int diff = time - m_ThreatLevelUpdateTimestamp;
		if (force || diff > Math.RandomIntInclusive(250, 300))
		{
			m_ThreatLevelUpdateTimestamp = time;
			m_ThreatLevel = m_Info.CalculateThreat(m_AI);

			//! Make active threat level rise/fall instantly if LOS, rise/fall slowly if no LOS
			if (m_LOS)
				m_ThreatLevelActive = m_ThreatLevel;
			else if (!force)
				m_ThreatLevelActive = Math.Lerp(m_ThreatLevelActive, Math.Min(0.1999, m_ThreatLevel), diff / 33.333333 * 0.001111);
		}
	}

	void UpdatePosition(bool force = false)
	{
		int time = GetGame().GetTime();
		if (force || m_LOS || time - m_SearchPositionUpdateTimestamp > Math.RandomIntInclusive(250, 300))
		{
			if (force || m_LOS)
			{
				//! Update last known target position
				m_LastKnownPosition = m_Info.GetPosition(m_AI, true);
				m_PositionReached = false;
				m_SearchPositionUpdateTimestamp = time;
				m_SearchPosition = m_LastKnownPosition;
				m_SearchDirection = vector.Direction(m_AI.GetPosition(), m_SearchPosition);
				if (!force)
					m_AI.GetGroup().m_UpdateSearchPosition = false;
			}
			else if (m_ThreatLevelActive > 0.2)
			{
				//! Make AI search the area by randomly moving around last known target position
				//! Start search after first AI in group reaches initial search position
				bool updateSearchPosition = m_AI.GetGroup().m_UpdateSearchPosition;
				if ((updateSearchPosition && !m_PositionReached) || (updateSearchPosition && time - m_SearchPositionUpdateTimestamp > 10000) || vector.DistanceSq(m_AI.GetPosition(), m_SearchPosition) < 1.0)
				{
					m_PositionReached = true;
					m_SearchPositionUpdateTimestamp = time;
					vector targetPosition = m_Info.GetPosition(m_AI, true);
					if (vector.DistanceSq(m_AI.GetPosition(), targetPosition) < 100.0)  //! Spidey sense within 10 m
						m_SearchPosition = targetPosition;
					else
						m_SearchPosition = ExpansionStatic.GetSurfacePosition(ExpansionMath.GetRandomPointInCircle(m_SearchPosition + m_SearchDirection.Normalized(), 30));
					m_AI.GetGroup().m_UpdateSearchPosition = true;
				}
			}
		}
	}
};
