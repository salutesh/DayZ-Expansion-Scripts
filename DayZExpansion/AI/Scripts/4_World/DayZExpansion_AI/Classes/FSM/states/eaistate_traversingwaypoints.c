class eAIState_TraversingWaypoints: eAIState
{
	int m_Index;
	float m_Threshold=1.0;
	float m_PreviousDistance;
	vector m_Waypoint;
	vector m_PreviousWaypoint;
	bool m_GotUp;
	float m_WaypointTime;
	float m_LeaveThreshold;
	float m_WaypointCountdown;

	override void OnEntry(string Event, ExpansionState From)
	{
		m_GotUp = false;
		unit.eAI_LeaveCover();
		
		eAIWaypointBehavior behaviour = unit.GetGroup().GetWaypointBehaviour();
		if (behaviour == eAIWaypointBehavior.ROAMING && m_Waypoint == vector.Zero)
		{
			m_Waypoint = unit.GetGroup().FindClosestRoamingLocationPosition();
			m_LeaveThreshold = Math.RandomFloat(5.0, 15.0);
			m_WaypointCountdown = 0;
		}

		if (unit.m_eAI_DefaultStance > eAIStance.STANDING)
		{
			unit.Expansion_GetUp(true);
			m_GotUp = true;
		}
	}

	override void OnExit(string Event, bool Aborted, ExpansionState To)
	{
		unit.SetMovementSpeedLimit(unit.m_eAI_SpeedLimitPreference);  //! Restore speed limit
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		eAIWaypointBehavior behaviour = unit.GetGroup().GetWaypointBehaviour();
		
		TVectorArray path;
		if (behaviour != eAIWaypointBehavior.ROAMING)
		{
			path = unit.GetGroup().GetWaypoints();
			
			if (path.Count() == 0)
			{
				unit.Expansion_DeleteDebugObjects();
				return EXIT;
				//path = { unit.GetPosition() + unit.GetDirection() * unit.Expansion_GetMovementSpeed() };
			}
			
			m_Index = unit.GetGroup().m_CurrentWaypointIndex;
			if (m_Index >= path.Count())
				m_Index = 0;
			
			m_Waypoint = path[m_Index];
		}
		
		if (m_Waypoint == vector.Zero)
			return EXIT;
		
		if (!m_GotUp && unit.Expansion_GetMovementSpeed() > 0 && unit.GetGroup().GetLeader().IsAI())
		{
			unit.Expansion_GetUp(true);
			m_GotUp = true;
		}
		
		unit.eAI_SetBestSpeedLimit(unit.m_eAI_SpeedLimitPreference);
		
		vector targetPosition;
		auto pathFinding = unit.GetPathFinding();
		if (pathFinding.m_TargetPosition == m_Waypoint && !pathFinding.m_Recalculate)
		{
			//! If we only have two or less points total, we can be sure that endpoint is reachable and closest to target as possible
			if (pathFinding.m_Count <= 2 && unit.GetCurrentWaterLevel() < -0.5)
				targetPosition = pathFinding.GetEnd();
			else
				targetPosition = pathFinding.GetTarget();
		}
		else
		{
			targetPosition = m_Waypoint;
		}
		
		vector position = unit.GetPosition();
		position[1] = targetPosition[1];
		float distance = vector.DistanceSq(position, targetPosition);
		bool isFinal;
		bool waypointReached;
		if (distance < m_Threshold)
			waypointReached = true;
		
		if (waypointReached || (pathFinding.m_IsUnreachable && pathFinding.m_IsTargetUnreachable))
		{
			m_Threshold = 1.0;
			
			if (behaviour == eAIWaypointBehavior.ROAMING)
			{
				if (m_WaypointCountdown > 0)
					m_WaypointCountdown -= DeltaTime;

				m_WaypointTime += DeltaTime;
				
				if ((!unit.m_eAI_Ladder && Math.IsPointInCircle(pathFinding.GetTarget(), 30.0, position) && m_WaypointCountdown <= 0) || m_WaypointTime >= m_LeaveThreshold)
				{
					unit.GetGroup().SetRoamingLocationReached(waypointReached);
					m_PreviousWaypoint = m_Waypoint;
					m_Waypoint = unit.GetGroup().FindClosestRoamingLocationPosition();
					m_WaypointTime = 0;
					m_LeaveThreshold = Math.RandomFloat(5.0, 15.0);
					unit.m_eAI_PositionTime = 0;

					if (pathFinding.m_IsUnreachable && pathFinding.m_IsTargetUnreachable)
					{
						m_WaypointCountdown = m_LeaveThreshold;

						if (unit.m_eAI_Ladder && !unit.m_eAI_IsOnLadder && unit.m_eAI_BuildingWithLadder)
						{
							unit.m_eAI_Ladder = null;
							unit.m_eAI_PreferLadder = false;
							pathFinding.ResetUnreachable();
						}
					}
				}
				else
				{
					isFinal = true;
				}
			}
			else if (behaviour == eAIWaypointBehavior.HALT || path.Count() == 1 || pathFinding.m_IsUnreachable)
			{
				isFinal = true;
			}
			else
			{
				bool backtracking = unit.GetGroup().m_BackTracking;
				m_PreviousWaypoint = m_Waypoint;
				if (backtracking) m_Index--;
				else m_Index++;
				if (unit.Expansion_GetMovementSpeed() > 0)
					pathFinding.ForceRecalculate(true);
				
				if (m_Index < 0) 
				{
					if (behaviour == eAIWaypointBehavior.ALTERNATE)
					{
						backtracking = false;
						m_Index = 1;
					}
					else if (behaviour == eAIWaypointBehavior.LOOP)
					{
						m_Index = path.Count() - 1;
					}
					else
					{
						m_Index = 0;
					}
				}
				else if (m_Index >= path.Count())
				{
					if (behaviour == eAIWaypointBehavior.ALTERNATE)
					{
						backtracking = true;
						m_Index = path.Count() - 2;
					} 
					else if (behaviour == eAIWaypointBehavior.LOOP)
					{
						m_Index = 0;
					}
					else
					{
						m_Index = path.Count() - 1;
					}
				}
				
				m_Waypoint = path[m_Index];
		
				unit.GetGroup().m_CurrentWaypointIndex = m_Index;
				unit.GetGroup().m_BackTracking = backtracking;
				
				if (behaviour != eAIWaypointBehavior.LOOP && (m_Index == 0 || m_Index == path.Count() - 1))
					isFinal = true;
			}
		}
		else if (Math.AbsFloat(distance - m_PreviousDistance) < 0.0016)
		{
			//! We seem to not be getting closer to the waypoint, possibly the path is blocked.
			//! Increase threshold until we are within distance.
			m_Threshold += 0.01;
		}
		
		unit.GetGroup().m_CurrentWaypoint = m_Waypoint;
		
		m_PreviousDistance = distance;
		
		unit.OverrideTargetPosition(m_Waypoint, isFinal);
		unit.OverrideMovementDirection(false, 0);
		unit.OverrideMovementSpeed(false, 0);
		
		vector direction;
		if (m_PreviousWaypoint != vector.Zero || (path && path.Count() > 1))
		{
			if (m_PreviousWaypoint == vector.Zero)
				m_PreviousWaypoint = path[0] - vector.Direction(path[0], path[1]);
			direction = vector.Direction(m_PreviousWaypoint, m_Waypoint).VectorToAngles();
			direction[1] = 0.0;
			direction[2] = 0.0;
			direction = direction.AnglesToVector();
		}
		else
		{
			direction = unit.GetDirection();
		}
		unit.Expansion_DebugObject_Deferred(21 + m_Index, m_Waypoint - "0 1.5 0", "ExpansionDebugNoticeMe_Purple", direction);
		if (pathFinding.m_IsTargetUnreachable)
			unit.Expansion_DebugObject_Deferred(20, "0 0 0", "ExpansionDebugNoticeMe");
		else
			unit.Expansion_DebugObject_Deferred(20, m_Waypoint, "ExpansionDebugNoticeMe", unit.GetDirection());
		
		return EXIT;
	}

	int Guard()
	{
		auto group = unit.GetGroup();
		if (!group) return eAITransition.FAIL;
		
		if (unit.GetThreatToSelf() >= 0.4) return eAITransition.FAIL;
		
		if (group.IsInCombat())
		{
			eAITarget target = unit.GetTarget();
			if (target && !target.IsItem() && unit.GetThreatToSelf(true) >= 0.4) return eAITransition.FAIL;
		}
		
		if (group.GetFormationState() != eAIGroupFormationState.IN) return eAITransition.FAIL;
		
		// we are the leader so we traverse the waypoints
		auto leader = group.GetFormationLeader();
		if (leader && leader != unit) return eAITransition.FAIL;
		if (leader.IsInTransport()) return eAITransition.FAIL;
		if (group.GetWaypoints().Count() == 0 && group.GetWaypointBehaviour() != eAIWaypointBehavior.ROAMING) return eAITransition.FAIL;
		
		return eAITransition.SUCCESS;
	}

	int GuardIdle()
	{
		auto group = unit.GetGroup();
		if (group.GetFormationState() != eAIGroupFormationState.IN) return eAITransition.SUCCESS;
		if (unit.GetThreatToSelf() >= 0.4 || !group || group.GetFormationLeader() != unit || (group.GetWaypoints().Count() == 0 && group.GetWaypointBehaviour() != eAIWaypointBehavior.ROAMING)) return eAITransition.SUCCESS;
		
		return eAITransition.FAIL;
	}
};
