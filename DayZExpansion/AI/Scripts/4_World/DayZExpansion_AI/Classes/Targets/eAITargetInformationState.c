class eAITargetInformationState
{
	eAIBase m_AI;
	eAITargetInformation m_Info;

	int m_ThreatLevelUpdateTimestamp;
	float m_ThreatLevel;
	float m_ThreatLevelActive;
	vector m_LastKnownPosition;
	int m_SearchPositionUpdateCount;
	int m_SearchPositionUpdateTimestamp;
	vector m_SearchPosition;
	vector m_SearchDirection;
	bool m_LOS;

	void eAITargetInformationState(eAIBase ai, eAITargetInformation info)
	{
		m_AI = ai;
		m_Info = info;

		UpdatePosition(true);
		UpdateThreat(true);
	}

	void UpdateThreat(bool force = false)
	{
		int time = GetGame().GetTime();
		int diff = time - m_ThreatLevelUpdateTimestamp;
		if (force || diff > Math.RandomIntInclusive(250, 300))
		{
			m_ThreatLevelUpdateTimestamp = time;
			m_ThreatLevel = m_Info.CalculateThreat(m_AI);

			//! Make active threat level rise depending on distance if LOS, fall slowly if no LOS
			if (m_LOS)
			{
				float distanceFactor;
				if (m_ThreatLevelActive < 0.4)
					distanceFactor = m_SearchDirection.Length();
				if (distanceFactor > 0)
				{
					vector dirNorm = m_SearchDirection.Normalized();
					float toTargetDot = vector.Dot(m_AI.GetAimDirection(), dirNorm);
					if (toTargetDot < 0.75)
						distanceFactor *= 7;  //! AI is facing away
					else
						distanceFactor *= 3;  //! AI is facing target
					m_ThreatLevelActive = Math.Min(m_ThreatLevelActive + m_ThreatLevel / distanceFactor, m_ThreatLevel);
				}
				else
				{
					m_ThreatLevelActive = m_ThreatLevel;
				}
			}
			else if (!force)
			{
				m_ThreatLevelActive = Math.Lerp(m_ThreatLevelActive, Math.Min(0.1999, m_ThreatLevel), diff / 33.333333 * 0.001111);
			}
		}
	}

	void UpdatePosition(bool force = false)
	{
		int time = GetGame().GetTime();
		if (force || m_LOS || time - m_SearchPositionUpdateTimestamp > Math.RandomIntInclusive(250, 300))
		{
#ifdef DIAG
			auto hitch = EXHitch(m_AI.ToString() + " eAITargetInformationState::UpdatePosition ", 20000);
#endif

			if (force || m_LOS)
			{
				//! Update last known target position
				m_LastKnownPosition = m_Info.GetPosition(m_AI, true);
				m_SearchPositionUpdateCount = 0;
				m_SearchPositionUpdateTimestamp = time;
				m_SearchPosition = m_LastKnownPosition;
				m_SearchDirection = vector.Direction(m_AI.GetPosition(), m_SearchPosition);
				if (!force)
					m_AI.GetGroup().m_UpdateSearchPosition = false;
			}
			else if (m_ThreatLevelActive > 0.2)
			{
				//! Make AI search the area by moving in direction of last known target position
				//! Start search after first AI in group reaches initial search position
				bool updateSearchPosition = m_AI.GetGroup().m_UpdateSearchPosition;
				if ((updateSearchPosition && !m_SearchPositionUpdateCount) || (updateSearchPosition && time - m_SearchPositionUpdateTimestamp > 10000) || vector.DistanceSq(m_AI.GetPosition(), m_SearchPosition) < 1.0)
				{
					m_SearchPositionUpdateCount++;
					m_SearchPositionUpdateTimestamp = time;
					vector targetPosition = m_Info.GetPosition(m_AI, true);
					if (vector.DistanceSq(m_AI.GetPosition(), targetPosition) < 100.0)  //! Spidey sense within 10 m
					{
						m_SearchPosition = targetPosition;
					}
					else if (m_SearchPositionUpdateCount <= 3)
					{
						//! Directional search
						vector angles = m_SearchDirection.Normalized().VectorToAngles();
						angles[0] = ExpansionMath.AbsAngle(ExpansionMath.RelAngle(angles[0] + Math.RandomFloat(-33.75, 33.75)));
						vector dir = angles.AnglesToVector();
						m_SearchPosition = ExpansionStatic.GetSurfacePosition(m_SearchPosition + dir * Math.RandomFloat(5.0, 10.0));
					}
					else
					{
						//! Radial search after three updates
						m_SearchPosition = ExpansionStatic.GetSurfacePosition(ExpansionMath.GetRandomPointInCircle(m_SearchPosition + m_SearchDirection.Normalized(), 30));
					}
					m_AI.GetGroup().m_UpdateSearchPosition = true;
				}
			}
		}
	}
};
