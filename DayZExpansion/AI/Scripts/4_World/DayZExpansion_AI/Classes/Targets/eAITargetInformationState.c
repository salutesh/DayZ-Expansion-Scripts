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
	bool m_LOS;  //! LOS state (may be stale if not current target for AI)
#ifdef DIAG
	Object m_LOSRaycastHitObject;
	vector m_LOSRaycastHitPosition;
#endif

	void eAITargetInformationState(eAIBase ai, eAITargetInformation info, bool initialUpdate = true)
	{
		m_AI = ai;
		m_Info = info;

		UpdatePosition(initialUpdate);
		UpdateThreat(initialUpdate);
	}

	void SetInitial(float threat, vector position)
	{
		int time = GetGame().GetTime();

		m_ThreatLevelUpdateTimestamp = time;
		m_ThreatLevel = threat;
		m_ThreatLevelActive = threat;

		m_SearchPositionUpdateTimestamp = time;
		m_LastKnownPosition = position;
		m_SearchPosition = position;
		m_SearchDirection = vector.Direction(m_AI.GetPosition(), m_SearchPosition);
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
		#ifdef EXPANSION_AI_ITEM_TARGET_REQUIRE_LOS
			if (m_LOS || !m_Info.IsEntity())
		#else
			if (m_LOS || !m_Info.IsEntity() || m_Info.IsInherited(eAIItemTargetInformation))
		#endif
			{
				//! Threat level rises slowly if below fighting threshold and AI has not been attacked by player,
				//! unless player is in vehicle (parent non-null)
				float distanceFactor;
				DayZPlayerImplement player;
				if (m_ThreatLevelActive < 0.4 && !m_AI.GetGroup().GetFaction().IsObserver() && Class.CastTo(player, m_Info.GetEntity()) && !player.GetParent() && ExpansionStatic.GetTime(true) - player.m_eAI_LastAggressionTime > 1.0)
					distanceFactor = m_SearchDirection.Length() * 3;
				if (distanceFactor > 0)
				{
					m_ThreatLevelActive = Math.Min(m_ThreatLevelActive + m_ThreatLevel / distanceFactor, m_ThreatLevel);
				}
				else
				{
					m_ThreatLevelActive = m_ThreatLevel;
				}
			}
			else if (!force)
			{
			#ifdef EXPANSION_AI_ITEM_TARGET_REQUIRE_LOS
				if (m_Info.IsInherited(eAIItemTargetInformation))
					m_ThreatLevelActive = Math.Min(0.3999, m_ThreatLevel);  //! No interpolation for item targets
				else
			#endif
				m_ThreatLevelActive = Math.Lerp(m_ThreatLevelActive, Math.Min(0.0999, m_ThreatLevel), diff / 33.333333 * 0.001111);
			}
		}
	}

	void UpdatePosition(bool force = false)
	{
		int time = GetGame().GetTime();
		if (force || m_LOS || time - m_SearchPositionUpdateTimestamp > Math.RandomIntInclusive(250, 300))
		{
#ifdef DIAG
			auto hitch = new EXHitch(m_AI.ToString() + " eAITargetInformationState::UpdatePosition ", 20000);
#endif

			if (force || (m_LOS && m_AI.GetTarget().info == m_Info))
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
			else if (m_ThreatLevelActive > 0.2 && m_AI.m_eAI_FlankTime == 0.0)
			{
				//! Make AI search the area by moving in direction of last known target position
				//! Start search after first AI in group reaches initial search position
				bool updateSearchPosition = m_AI.GetGroup().m_UpdateSearchPosition;

				if (m_Info.IsInherited(eAINoiseTargetInformation))
				{
					//! The distance check may not work for noise targets since position is randomly generated
					//! in a radius around actual noise source and may not be reachable, so we rely on time-based updates.
					//! Also, this helps further spread search position in case we are far from noise target.
					updateSearchPosition = true;
					m_SearchPositionUpdateCount++;
				}

				if ((updateSearchPosition && !m_SearchPositionUpdateCount) || (updateSearchPosition && time - m_SearchPositionUpdateTimestamp > 10000) || vector.DistanceSq(m_AI.GetPosition(), m_SearchPosition) < 1.0)
				{
					m_SearchPositionUpdateCount++;
					m_SearchPositionUpdateTimestamp = time;
					if (m_SearchPositionUpdateCount <= 3)
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
