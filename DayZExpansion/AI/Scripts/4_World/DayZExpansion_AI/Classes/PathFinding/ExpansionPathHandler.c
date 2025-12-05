class ExpansionPathHandler
{
	static const float PATH_RECALCULATE_THRESHOLD = 5.0;

	ref EXTimeIt m_TimeIt;

	eAIBase m_Unit;
	vector m_UnitVelocity;
	float m_Time;
	float m_MinTimeUntilNextUpdate;

	PGFilter m_PathFilter;
	PGFilter m_PathFilter_NoJumpClimb;
	//autoptr PGFilter m_CheckFilter;
	PGFilter m_BlockFilter;
	PGFilter m_AllFilter;

	ref ExpansionPathPoint m_Current;
	ref ExpansionPathPoint m_Target;
	ref ExpansionPathPoint m_TargetReference;
	vector m_TargetPosition;
	vector m_CurrentTargetPosition;
	bool m_IsTargetUnreachable;
	bool m_IsSwimmingEnabled;
#ifdef DIAG_DEVELOPER
	string m_LastDbgMsg;
	vector m_LastTargetPosition;
#endif

	bool m_OverridingPosition;

	int m_Count;
	int m_PrevCount;
	int m_TempCount;
	int m_PrevTempCount;
	int m_PointIdx;
	int m_PathGlueIdx;
	vector m_PrevPoint;
	ref ExpansionPathPoint m_Next0;
	ref ExpansionPathPoint m_Next1;
	vector m_PathSegmentDirection;

	AIWorld m_AIWorld;

	ref array<ref ExpansionPathPoint> m_Path = new array<ref ExpansionPathPoint>();
	ref array<vector> m_Points = new array<vector>();
	ref array<vector> m_TempPoints = new array<vector>();
	int m_DrawDebug_PointIdx;

	bool m_Recalculate;
	bool m_SuppressRecalculate;
	bool m_IsBlocked;
	bool m_IsBlockedPhysically;
	bool m_IsJumpClimb;
	bool m_DoClimbTestEx;
	bool m_IsUnreachable;
	bool m_AllowJumpClimb = true;
	float m_AllowJumpClimb_Timeout;
	bool m_AllowClosedDoors = true;
	float m_AllowClosedDoors_Timeout;

	void ExpansionPathHandler(eAIBase unit)
	{
		m_Unit = unit;

#ifdef EXTRACE_DIAG
		m_TimeIt = new EXHitch(m_Unit.ToString() + " ExpansionPathHandler::OnUpdate ", 20000);
#else
		m_TimeIt = new EXTimeIt();
#endif

		m_PathFilter = ExpansionPathFilters.GetInstance().m_PathFilter;
		m_PathFilter_NoJumpClimb = ExpansionPathFilters.GetInstance().m_PathFilter_NoJumpClimb;
		//m_CheckFilter = new PGFilter();
		m_BlockFilter = ExpansionPathFilters.GetInstance().m_BlockFilter;
		m_AllFilter = ExpansionPathFilters.GetInstance().m_AllFilter;

		m_AIWorld = g_Game.GetWorld().GetAIWorld();

		m_Current = new ExpansionPathPoint();
		m_Target = new ExpansionPathPoint();
		m_TargetReference = new ExpansionPathPoint();
		m_Next0 = new ExpansionPathPoint();
		m_Next1 = new ExpansionPathPoint();

		EnableSwimming();  //! Start with swimming enabled (helps on Sakhal if AI is standing on ice floe)
	}

	void SetAllowJumpClimb(bool allow, float timeout = 0)
	{
		if (!allow && allow != m_AllowJumpClimb && m_TargetPosition != vector.Zero)
			ForceRecalculate(true);

		m_AllowJumpClimb = allow;

		if (timeout > 0)
			m_AllowJumpClimb_Timeout = timeout;
	}

	PGFilter GetFilter()
	{
		if (m_AllowJumpClimb && m_Unit.GetHealth01() >= 0.2)
			return m_PathFilter;

		return m_PathFilter_NoJumpClimb;
	}

	void EnableSwimming(bool enable = true, bool forceUpdate = true)
	{
		if (enable)
		{
			if (m_IsSwimmingEnabled)
				return;

			if (m_AllowClosedDoors)
			{
				m_PathFilter = ExpansionPathFilters.GetInstance().m_PathFilter_Swimming;
				m_PathFilter_NoJumpClimb = ExpansionPathFilters.GetInstance().m_PathFilter_NoJumpClimb_Swimming;
			}
			else
			{
				m_PathFilter = ExpansionPathFilters.GetInstance().m_PathFilter_NoClosedDoors_Swimming;
				m_PathFilter_NoJumpClimb = ExpansionPathFilters.GetInstance().m_PathFilter_NoClosedDoors_NoJumpClimb_Swimming;
			}
		}
		else
		{
			if (!m_IsSwimmingEnabled)
				return;

			if (m_AllowClosedDoors)
			{
				m_PathFilter = ExpansionPathFilters.GetInstance().m_PathFilter;
				m_PathFilter_NoJumpClimb = ExpansionPathFilters.GetInstance().m_PathFilter_NoJumpClimb;
			}
			else
			{
				m_PathFilter = ExpansionPathFilters.GetInstance().m_PathFilter_NoClosedDoors;
				m_PathFilter_NoJumpClimb = ExpansionPathFilters.GetInstance().m_PathFilter_NoClosedDoors_NoJumpClimb;
			}
		}

	#ifdef DIAG_DEVELOPER
		ExpansionStatic.MessageNearPlayers(m_Unit.GetPosition(), 100.0, m_Unit.ToString() + " EnableSwimming " + enable + " forceUpdate " + forceUpdate);
	#endif

		if (forceUpdate)
			ForceRecalculate(true);

		m_IsSwimmingEnabled = enable;
	}

	void AllowClosedDoors(bool allow = true, float timeout = 0, bool forceUpdate = true)
	{
		if (allow)
		{
			if (m_AllowClosedDoors)
				return;

			if (m_IsSwimmingEnabled)
			{
				m_PathFilter = ExpansionPathFilters.GetInstance().m_PathFilter_Swimming;
				m_PathFilter_NoJumpClimb = ExpansionPathFilters.GetInstance().m_PathFilter_NoJumpClimb_Swimming;
			}
			else
			{
				m_PathFilter = ExpansionPathFilters.GetInstance().m_PathFilter;
				m_PathFilter_NoJumpClimb = ExpansionPathFilters.GetInstance().m_PathFilter_NoJumpClimb;
			}
		}
		else
		{
			if (!m_AllowClosedDoors)
				return;

			if (m_IsSwimmingEnabled)
			{
				m_PathFilter = ExpansionPathFilters.GetInstance().m_PathFilter_NoClosedDoors_Swimming;
				m_PathFilter_NoJumpClimb = ExpansionPathFilters.GetInstance().m_PathFilter_NoClosedDoors_NoJumpClimb_Swimming;
			}
			else
			{
				m_PathFilter = ExpansionPathFilters.GetInstance().m_PathFilter_NoClosedDoors;
				m_PathFilter_NoJumpClimb = ExpansionPathFilters.GetInstance().m_PathFilter_NoClosedDoors_NoJumpClimb;
			}

			m_AllowClosedDoors_Timeout = timeout;
		}

	#ifdef DIAG_DEVELOPER
		ExpansionStatic.MessageNearPlayers(m_Unit.GetPosition(), 100.0, m_Unit.ToString() + " AllowClosedDoors " + allow + " forceUpdate " + forceUpdate);
	#endif

		if (forceUpdate)
			ForceRecalculate(true);

		m_AllowClosedDoors = allow;
	}

/*
	bool Raycast(PGPolyFlags filter, float distance, out vector hitPos)
	{
		m_CheckFilter.SetFlags(filter, ~filter, PGPolyFlags.NONE);
		m_CheckFilter.SetCost(PGAreaType.BUILDING, 1.0);

		vector p = m_Unit.GetPosition();
		vector d = m_Unit.GetDirection();

		vector p0 = p - (d * distance * 0.5);
		vector p1 = p + (d * distance);
		vector hitNor;

		if (m_Current.NavMesh)
		{
			vector transform[4];
			m_Current.Parent.GetTransform(transform);
			return m_Current.NavMesh.Raycast(p0.Multiply4(transform), p1.Multiply4(transform), m_CheckFilter, hitPos, hitNor);
		}

		return m_AIWorld.RaycastNavMesh(p0, p1, m_CheckFilter, hitPos, hitNor);
	}

	bool IsDoor()
	{
		if (m_Count <= 1 + m_PointIdx)
			return false;

		return true;

		//vector hitPos;
		//return this.Raycast(PGPolyFlags.DOOR, 1.5, hitPos);
	}

	bool IsVault()
	{
		if (m_Count <= 1 + m_PointIdx)
			return false;

		if (m_Current && m_Current.NavMesh)
		{
			vector hitPos;
			return this.Raycast(PGPolyFlags.CLIMB, 0.5, hitPos);
		}

		return true;

		//vector hitPos;
		//return this.Raycast(PGPolyFlags.CLIMB, 0.5, hitPos);
	}
*/

	/**
	 * @brief Check if path is blocked in navmesh
	 * 
	 * @param start
	 * @param end
	 * @param [out] hitPos
	 * @param [out] hitNormal
	 */
	bool IsBlocked(vector start, vector end, PGFilter filter, out vector hitPos = vector.Zero, out vector hitNormal = vector.Zero)
	{
		return m_AIWorld.RaycastNavMesh(start, end, filter, hitPos, hitNormal);
	}

	bool IsBlocked(vector start, vector end, out vector hitPos = vector.Zero, out vector hitNormal = vector.Zero)
	{
		return m_AIWorld.RaycastNavMesh(start, end, GetFilter(), hitPos, hitNormal);
	}

	/**
	 * @brief Check if path is blocked physically
	 */
	bool IsBlockedPhysically(vector start, vector end, out vector hitPos = vector.Zero, out vector hitNormal = vector.Zero, out int contactComponent = -1, out Object hitObj = null)
	{
		set<Object> results = new set<Object>;
		bool hit;

		hit = DayZPhysics.RaycastRV(start, end, hitPos, hitNormal, contactComponent, results, null, m_Unit, false, false, ObjIntersectGeom);

		//if (!hit)
			//hit = DayZPhysics.RaycastRV(start, end, hitPos, hitNormal, contactComponent, results, null, m_Unit, false, false, ObjIntersectView);

		if (hit)
		{
			if (results.Count())
			{
				hitObj = results[0];

				if (ExpansionStatic.CanObstruct(hitObj))
					return true;
			}
		}

		return false;
	}

	vector CalculateOffset()
	{
		if (m_Count <= 2 + m_PointIdx)
		{
			return vector.Zero;
		}

		vector transform[4];
		m_Unit.GetTransform(transform);

		vector p0 = transform[3]; // could use m_Current.GetPosition()
		vector p1 = m_Next0.GetPosition();
		vector p2 = m_Next1.GetPosition();

		vector d0 = transform[2];
		vector d1 = vector.Direction(p0, p1).Normalized();
		vector d2 = vector.Direction(p1, p2).Normalized();

		float do0 = vector.Dot(d0, d1);
		float do1 = vector.Dot(d1, d2);

		return d0 * (do0 * (1.0 - do1)) * 2.0;
	}

	vector GetDebugPosition(int index)
	{
		if (index < 0)
		{
			index = 0;
		}

		if (index >= m_Path.Count())
		{
			index = m_Path.Count() - 1;
		}

		return m_Path[index].GetPosition();
	}

	vector CalculateDebugOffset(int index)
	{
		if (m_Path.Count() <= 2)
		{
			return vector.Zero;
		}

		vector p0 = GetDebugPosition(index - 1);
		vector p1 = GetDebugPosition(index);
		vector p2 = GetDebugPosition(index + 1);


		vector d1 = vector.Direction(p0, p1).Normalized();
		vector d2 = vector.Direction(p1, p2).Normalized();

		float do0 = 1.0;
		if (index <= 1)
		{
			vector transform[4];
			m_Unit.GetTransform(transform);

			vector d0 = transform[2];
			do0 = vector.Dot(d0, d1);
		}

		float do1 = vector.Dot(d1, d2);

		return d0 * (do0 * (1.0 - do1)) * 2.0;
	}

	void DrawDebug()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "DrawDebug");
#endif

#ifdef DIAG_DEVELOPER
#ifndef SERVER
		ExpansionNavMesh navMesh = m_Current.NavMesh;
		Object parent = m_Current.Parent;
		if (!navMesh)
		{
			navMesh = m_TargetReference.NavMesh;
			parent = m_TargetReference.Parent;
		}

		if (navMesh)
		{
			navMesh.DrawDebug(parent);
		}
#endif
#endif

#ifdef EAI_DEBUG_PATH
		int i;

#ifndef SERVER
		//m_Unit.AddShape(Shape.CreateSphere(0xFFFF0000, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, m_Current.GetPosition(), 0.3));
		//m_Unit.AddShape(Shape.CreateSphere(0xFF00FF00, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, m_Target.GetPosition(), 0.3));

		//m_Unit.AddShape(Shape.CreateSphere(0xFF0000FF, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, m_TargetReference.GetPosition(), 0.3));

		//m_Unit.AddShape(Shape.CreateSphere(0xFFFF00AA, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, m_Next0.GetPosition(), 0.3));
		//m_Unit.AddShape(Shape.CreateSphere(0xFFAA00FF, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, m_Next1.GetPosition(), 0.3));

		vector points[2];

		vector offset = vector.Zero;

		map<int, int> possibleFlags();
		//possibleFlags.Insert(PGPolyFlags.WALK, 			0xAAAA0000);
		//possibleFlags.Insert(PGPolyFlags.DISABLED,		0xAAFF0000);
		//possibleFlags.Insert(PGPolyFlags.DOOR,			0xAA0000FF);
		//possibleFlags.Insert(PGPolyFlags.INSIDE,		0xAA00FF00);
		//possibleFlags.Insert(PGPolyFlags.SWIM,			0xAA0000AA);
		//possibleFlags.Insert(PGPolyFlags.SWIM_SEA,		0xAA00AA00);
		possibleFlags.Insert(PGPolyFlags.LADDER,		0xAAAA00FF);
		possibleFlags.Insert(PGPolyFlags.JUMP_OVER,		0xAAFF00AA);
		possibleFlags.Insert(PGPolyFlags.JUMP_DOWN,		0xAAFFAA00);
		possibleFlags.Insert(PGPolyFlags.CLIMB,			0xAAAAFF00);
		//possibleFlags.Insert(PGPolyFlags.CRAWL,			0xAA00FFAA);
		//possibleFlags.Insert(PGPolyFlags.CROUCH,		0xAA00AAFF);
		//possibleFlags.Insert(PGPolyFlags.UNREACHABLE,	0xAA000000);

		float radius = 0.2;

		for (i = m_PointIdx; i < m_Count; i++)
		{
			bool isFinal = i == (m_Count - 1);

			int color = 0xFF00FF00;
			if (m_Path[i].Parent)
				color = 0xFF0000FF;

			points[0] = GetDebugPosition(i);
			points[1] = GetDebugPosition(i + 1);

			m_Unit.AddShape(Shape.CreateLines(color, ShapeFlags.VISIBLE | ShapeFlags.NOZBUFFER, points, 2));

			vector flagOffset = Vector(0, radius, 0);
			int currentFlag = m_Path[i].Flags;

			foreach (auto flag, auto flagColor : possibleFlags)
			{
				if ((currentFlag & flag) == 0)
				{
					continue;
				}

				m_Unit.AddShape(Shape.CreateSphere(flagColor, ShapeFlags.NOOUTLINE | ShapeFlags.VISIBLE | ShapeFlags.TRANSP, points[0] + flagOffset, radius));
				flagOffset = flagOffset + Vector(0, radius * 2, 0);
			}

			/*
			if (i != 0)
				offset = CalculateDebugOffset(i);
			points[0] = points[0] + offset;

			offset = vector.Zero;

			if (!isFinal)
				offset = CalculateDebugOffset(i + 1);
			points[1] = points[1] + offset;

			m_Unit.AddShape(Shape.CreateLines(0xAAFF0000, ShapeFlags.VISIBLE | ShapeFlags.NOZBUFFER | ShapeFlags.TRANSP, points, 2));
			*/
		}
#else
		if (m_DrawDebug_PointIdx >= m_Count)
			m_DrawDebug_PointIdx = 0;
			//m_DrawDebug_PointIdx = m_PointIdx;

		vector origin;
		if (m_DrawDebug_PointIdx > 0 && m_DrawDebug_PointIdx != m_PathGlueIdx)
			origin = m_Points[m_DrawDebug_PointIdx - 1];

		if (m_Count > m_PointIdx)
		{
			string debugObj;

			if (m_DrawDebug_PointIdx < 1 + m_PointIdx)
			{
				debugObj = "ExpansionDebugConeSmall_Blue";
			}
			else if (m_IsUnreachable)
			{
				debugObj = "ExpansionDebugConeSmall_Red";
			}
			else if (m_DrawDebug_PointIdx > 1 + m_PointIdx)
			{
				if (m_PathGlueIdx > -1 && m_DrawDebug_PointIdx >= m_PathGlueIdx)
					debugObj = "ExpansionDebugConeSmall_Cyan";
				else
					debugObj = "ExpansionDebugConeSmall_White";
			}
			else if (m_IsBlockedPhysically)
			{
				if (m_AllowJumpClimb)
					debugObj = "ExpansionDebugConeSmall_Orange";
				else
					debugObj = "ExpansionDebugConeSmall_Red";
			}
			else if (m_IsJumpClimb)
			{
				if (m_AllowJumpClimb)
					debugObj = "ExpansionDebugConeSmall_Yellow";
				else
					debugObj = "ExpansionDebugConeSmall_Red";
			}
			else if (m_IsBlocked)
			{
				debugObj = "ExpansionDebugConeSmall_Purple";
			}
			else
			{
				debugObj = "ExpansionDebugConeSmall";
			}

			m_Unit.Expansion_DebugObject(11111 + m_DrawDebug_PointIdx, m_Points[m_DrawDebug_PointIdx++], debugObj, vector.Zero, origin, 3, ShapeFlags.NOZBUFFER);
		}

		for (i = m_PrevCount - 1; i > m_Count - 1; i--)
		{
			m_Unit.Expansion_DeleteDebugObject(11111 + i);
		}

		m_PrevCount = m_Count;
#endif
#endif
	}

	void OnUpdate(float pDt, int pSimulationPrecision)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Profile(EXTrace.AI_PROFILE, m_Unit, "CommandHandler(07) -> ExpansionPathHandler::OnUpdate");
	#endif

		m_TimeIt.Start();

		m_Time += pDt;

		if (m_AllowJumpClimb_Timeout > 0)
			m_AllowJumpClimb_Timeout -= pDt;

		if (m_AllowClosedDoors_Timeout > 0)
		{
			m_AllowClosedDoors_Timeout -= pDt;
			if (m_AllowClosedDoors_Timeout <= 0)
				AllowClosedDoors(true, 0, false);
		}

		bool recalculate;

		int i;

		UpdateCurrent();

		if (m_Recalculate && (!m_SuppressRecalculate || m_Time > PATH_RECALCULATE_THRESHOLD))
		{
			bool isSwimming;
			float timeUntilNextUpdate;

			if (m_Unit.IsSwimming())
			{
				isSwimming = true;
				//! We need a higher time for swimming since path seems to be more likely to go through same points
				//! and AI could end up moving in circles
				timeUntilNextUpdate = PATH_RECALCULATE_THRESHOLD;
			}
			else
			{
				timeUntilNextUpdate = (pSimulationPrecision + 1.0) * 2.0;
			}

			if (m_Time >= timeUntilNextUpdate)
			{
				recalculate = m_Recalculate;
			}
			else if (!isSwimming && m_Count >= 1 && m_Time >= m_MinTimeUntilNextUpdate)
			{
				vector unitPosition = m_Current.Position;
				vector unitVelocity = m_Unit.Expansion_GetActualVelocity();
				vector next0Position = m_Next0.GetPosition();

				float d0 = vector.DistanceSq(unitPosition, next0Position);
				float d1 = vector.DistanceSq(unitPosition + unitVelocity, next0Position);
				if (d0 < d1)
				{
					recalculate = m_Recalculate;
					//EXPrint(m_Unit.ToString() + " recalculating because d0 < d1 (overshoot), diff " + ExpansionStatic.FloatToString(d1 - d0) + ", velocity " + unitVelocity);
				}
			}
		}

		if (recalculate)
		{
			UpdatePoint(m_TargetReference, m_TargetPosition);

			m_CurrentTargetPosition = m_TargetPosition;

		#ifdef DIAG_DEVELOPER
			string dbgMsg;
		#endif

			if (!m_TargetReference.Parent)
			{
				//vector oldPos = m_TargetReference.Position;
				vector inPos = m_TargetPosition;
				float maxDistance = 1.0;
				bool isFormationLeaderOnGround;

			/*
				if (!m_Unit.GetGroup().IsFormationLeaderSwimming())
				{
					isFormationLeaderOnGround = true;
				}
				else
				{
					vector leaderPos = m_Unit.GetGroup().GetFormationLeader().GetPosition();
					leaderPos[1] = g_Game.SurfaceY(leaderPos[0], leaderPos[2]);
					if (g_Game.GetWaterDepth(leaderPos) < 2.0)
						isFormationLeaderOnGround = true;
				}
			*/

				// TODO: investigate why the same variable source must be used for 0th and 3rd parameter, and that it can't be a member variable for either
				if (!m_AIWorld.SampleNavmeshPosition(inPos, maxDistance, m_AllFilter, inPos) /*&& isFormationLeaderOnGround*/)
				{
				#ifdef DIAG_DEVELOPER
					if (!m_IsTargetUnreachable)
						dbgMsg = "target unreachable";
				#endif

					//inPos = oldPos;
					m_IsTargetUnreachable = true;
				}
				else if (g_Game.GetWaterDepth(inPos) > 1.5 && !m_IsSwimmingEnabled && m_Unit.GetCurrentWaterLevel() < -0.5 && !m_Unit.m_eAI_EffectArea)
				{
				#ifdef DIAG_DEVELOPER
					if (!m_IsTargetUnreachable)
						dbgMsg = "target in deep water";
				#endif

					m_IsTargetUnreachable = true;
				}
				else
				{
					m_IsTargetUnreachable = false;
				}

			#ifdef DIAG_DEVELOPER
				Object dbgConeRed;
				Object dbgConeWhite;

				if (vector.DistanceSq(inPos, m_TargetPosition) >= 0.01)
				{
					m_Unit.Expansion_DebugObject(33333, m_TargetPosition, "ExpansionDebugSphereSmall_Red");
					dbgConeRed = m_Unit.Expansion_DebugObject(44444, m_TargetPosition + "0 0.06 0", "ExpansionDebugConeSmall_Red");
				}
				else
				{
					m_Unit.Expansion_DebugObject(33333, vector.Zero, "ExpansionDebugSphereSmall_Red");
					m_Unit.Expansion_DebugObject(44444, vector.Zero, "ExpansionDebugConeSmall_Red");
				}

				m_Unit.Expansion_DebugObject(55555, inPos, "ExpansionDebugSphereSmall_White");
				dbgConeWhite = m_Unit.Expansion_DebugObject(66666, inPos + "0 0.06 0", "ExpansionDebugConeSmall_White");

				if (dbgConeRed)
					dbgConeRed.SetOrientation("0 180 0");

				if (dbgConeWhite)
					dbgConeWhite.SetOrientation("0 180 0");
			#endif

				m_TargetReference.Position = inPos;
			}

			m_Recalculate = false;

			m_IsUnreachable = false;

			m_Time = 0;

			m_Points.Clear();
			m_PathGlueIdx = -1;

			int F_STATE = -1;

			bool found;

			#ifdef EXPANSION_AI_ATTACHMENT_PATH_FINDING
				if (m_Current.Parent && !m_TargetReference.Parent) // moving to world
				{
					F_STATE = 1;

					m_Target.Copy(m_Current);

					m_Target.Position = m_TargetReference.Position;
					m_Target.OnParentUpdate();

					found = m_Target.FindPath(this, m_Points);

					if (m_Points.Count() == 2)
					{
						vector pathDir = vector.Direction(m_Points[0], m_Points[1]).Normalized();
						m_Points[1] = m_Points[1] + (pathDir * 2.0);
					}
				}
				else if (!m_Current.Parent && m_TargetReference.Parent) // moving to attachment
				{
					F_STATE = 2;

					// Find the path from the target position to the entry of the attachment

					m_Target.Copy(m_TargetReference);

					m_Target.Position = m_Unit.GetPosition();
					m_Target.OnParentUpdate();

					if (!m_Target.NavMesh)
					{
						Print("Navmesh removed??");
					}

					vector checkingIfAlreadyOnPathsPosition = m_Target.Position;

					m_Target.NavMesh.SamplePosition(checkingIfAlreadyOnPathsPosition, checkingIfAlreadyOnPathsPosition);

					float heightDiff = Math.AbsFloat(m_Target.Position[1] - checkingIfAlreadyOnPathsPosition[1]) * 2.0;

					if (vector.Distance(checkingIfAlreadyOnPathsPosition, m_Target.Position) < heightDiff)  //! Already on path
					{
						m_Target.Copy(m_TargetReference);

						found = m_Target.FindPath(this, m_Points);
					}
					else
					{
						array<vector> tempPath = {};
						found = m_Target.FindPathFrom(m_TargetReference.GetPosition(), this, tempPath);
						int count = tempPath.Count();

						// Find the path to the entry

						m_Target.Position = m_Current.Position;
						m_Target.Parent = null;
						m_Target.OnParentUpdate();

						vector closestPositionOnAttachment = tempPath[count - 1];

						found = m_Target.FindPathFrom(closestPositionOnAttachment, this, m_Points);

#ifdef EAI_DEBUG_PATH
#ifndef SERVER
						m_Path.RemoveOrdered(count);

						m_Path.Invert();
#endif
#endif
						m_Points.Invert();

						m_Points.Remove(m_Points.Count() - 1);

						for (i = 0; i < count; i++)
						{
							int ii = count - (i + 1);
							m_Points.Insert(tempPath[ii]);
						}
					}
				}
				else if (m_TargetReference.Parent) // moving in attachment
				{
					F_STATE = 3;

					m_Target.Copy(m_TargetReference);

					found = m_Target.FindPath(this, m_Points);
				}
				else if (!m_TargetReference.Parent) // moving in world
				{
					F_STATE = 4;

					m_Target.Copy(m_TargetReference);

					found = m_Target.FindPath(this, m_Points, m_PathGlueIdx);
				}
				else
				{
					F_STATE = 5;
				}
			#else
				F_STATE = 6;

				m_Target.Copy(m_TargetReference);

				found = m_Target.FindPath(this, m_Points, m_PathGlueIdx);
			#endif

			m_Count = m_Points.Count();
			m_PointIdx = 0;

			//Print(F_STATE);

			UpdateNext();

			//! @note normally, we would check m_Count <= 2, but there are certain structures where this won't work if we are on the roof
			//! and need to go down a ladder e.g. Land_City_Stand_*
			if (!m_IsTargetUnreachable && m_Count <= 3 && m_Unit.m_eAI_CommandTime > 1.0 && m_Unit.m_eAI_BuildingWithLadder)
			{
				vector end = GetEnd();

				if (Math.IsPointInCircle(end, 0.55, m_Current.Position) && vector.DistanceSq(end, GetTarget()) >= 4.0)
				{
					if (!m_Unit.m_eAI_Ladder || (!m_Unit.m_eAI_IsOnLadder && !m_Unit.eAI_IsCloseToLadderEntryPoint()))
					{
					#ifdef DIAG_DEVELOPER
						dbgMsg = "target unreachable from path endpoint";
					#endif

						m_IsTargetUnreachable = true;
					}
				}
			}

		#ifdef DIAG_DEVELOPER
			if (dbgMsg != string.Empty && (dbgMsg != m_LastDbgMsg || vector.DistanceSq(m_LastTargetPosition, m_TargetPosition) > 1.0))
			{
				m_LastDbgMsg = dbgMsg;
				m_LastTargetPosition = m_TargetPosition;

				dbgMsg += " " + ExpansionStatic.VectorToString(m_TargetPosition, ExpansionVectorToString.Labels);
				dbgMsg += " (" + vector.Distance(m_Current.Position, m_TargetPosition) + " m)";

				ExpansionStatic.MessageNearPlayers(m_Unit.GetPosition(), 100.0, m_Unit.ToString() + " " + dbgMsg);
			}
		#endif

		#ifdef EAI_DEBUG_PATH
			//! UNUSED temp points
			if (m_TempCount > 0)
			{
				vector origin;
				for (i = 0; i < m_TempCount; i++)
				{
					if (i > 0)
						origin = m_TempPoints[i - 1];
					else
						origin = vector.Zero;

					Object dbgObj = m_Unit.Expansion_DebugObject(77777 + i, m_TempPoints[i], "ExpansionDebugConeSmall_Orange", vector.Zero, origin, 3, ShapeFlags.NOZBUFFER);
					if (dbgObj)
						dbgObj.SetOrientation("0 180 0");
				}

				for (i = m_PrevTempCount - 1; i > m_TempCount - 1; i--)
				{
					m_Unit.Expansion_DeleteDebugObject(77777 + i);
				}

				m_PrevTempCount = m_TempCount;

				m_TempCount = 0;
			}
		#endif
		}

		m_MinTimeUntilNextUpdate = pDt + m_TimeIt.GetElapsed() * 0.0002;

	#ifdef EXTRACE_DIAG
		m_TimeIt.Log();
	#endif

#ifdef EAI_DEBUG_PATH
#ifndef SERVER
		if (m_Path.Count() < 3)
		{
			m_Path.Resize(3);
		}

		m_Path[0] = m_Current;
		m_Path[1] = m_Next0;
		m_Path[2] = m_Next1;

		m_Path.Resize(m_Count + 1);

		for (i = 2; i < m_Count; i++)
		{
			int actualIndex = i + 1;
			ExpansionPathPoint pathPoint = m_Path[actualIndex];
			UpdatePoint(pathPoint, m_Points[i]);
			m_Path[actualIndex] = pathPoint;
		}

		for (i = 1; i < m_Count + 1; i++)
		{
			m_Path[i - 1].Next = m_Path[i];
		}

		for (i = 0; i < m_Count + 1; i++)
		{
			m_Path[i].UpdateFlags(this);
		}
#endif
#endif

#ifdef DIAG_DEVELOPER
		DrawDebug();
#endif
	}

	void UpdateNext(bool incNextIdx = false)
	{
	//#ifdef EXTRACE_DIAG
		//auto trace = EXTrace.StartStack(EXTrace.AI, m_Unit, "incNextIdx " + incNextIdx + " m_PointIdx " + m_PointIdx + " m_Count " + m_Count);
	//#endif

		if (m_IsUnreachable)
			return;

		if (incNextIdx && m_PointIdx < m_Count - 2)
		{
		//#ifdef DIAG_DEVELOPER
			//m_Unit.Expansion_DeleteDebugObject(11111 + m_PointIdx);
		//#endif
			m_PointIdx++;
		}

		m_SuppressRecalculate = false;
		m_IsBlocked = false;
		m_IsBlockedPhysically = false;
		m_IsJumpClimb = false;
		m_DoClimbTestEx = false;

		//! Only check fall height if path endpoint (i.e. near potential edge) so we don't get stuck
		//! on certain structures upper level (e.g. Land_Barn_Metal_Big or Land_Mil_GuardTower) 
		if (m_Count == 1 + m_PointIdx && !CheckFallHeight())
			return;

		int P_STATE = -1;

		if (m_Count > 1 + m_PointIdx)
		{
			//! We have two or more path points remaining

			if (m_PointIdx > 0)
				m_PrevPoint = m_Points[m_PointIdx];

		#ifdef EXPANSION_AI_GENERATE_INTERMEDIATE_POINTS
			vector unitVelocity = m_Unit.Expansion_GetActualVelocity();
			float velocitySq = unitVelocity.LengthSq();

			if (velocitySq > 0.0001)
			{
				vector pathDir = vector.Direction(m_Points[m_PointIdx], m_Points[1 + m_PointIdx]);

				float speedMult = 1.0;

				if (velocitySq > 16.67)  //! Jog -> Sprint threshold
					speedMult = Math.Min(velocitySq * 0.12, 4.0);  //! ~2.0 when jogging, 4.0 when sprinting
				else if (velocitySq > 5.64)  //! Walk -> Jog threshold
					speedMult = Math.Min(velocitySq * 0.1773, 2.0);  //! ~1.0 when walking, 2.0 when jogging

				//! If distance from current to next point is 10 m or more, set next point to at least 5 m from current
				//! (makes AI get back on path quicker after turning/strafing to avoid obstacles)
				if (pathDir.LengthSq() >= 100.0 * speedMult)
				{
					m_Points.InsertAt(m_Points[m_PointIdx] + pathDir.Normalized() * (5.0 * speedMult), 1 + m_PointIdx);
					m_Count++;
				}
			}
		#endif

			UpdatePoint(m_Next0, m_Points[1 + m_PointIdx]);

		#ifdef EAI_DEBUG_PATH
		#ifndef SERVER
			if (m_Count > 2 + m_PointIdx)
			{
				//! We have three or more path points remaining

				P_STATE = 1;

				UpdatePoint(m_Next1, m_Points[2 + m_PointIdx]);
			}
			else
			{
				P_STATE = 2;
			}
		#endif
		#endif

			UpdatePathSegmentState();
		}
		else
		{
			P_STATE = 3;

			if (m_Current.Position != m_Unit.GetPosition())
				UpdateCurrent();

			m_Next0.Copy(m_Current);
		}

		//Print(P_STATE);
	}

	bool CheckFallHeight()
	{
		if (m_Unit.eAI_IsDangerousAltitude())
		{
			//! Prevent fall from a large height (e.g. building top) - path direction check
			vector checkDirection = vector.Direction(m_Unit.GetPosition(), m_Points[1 + m_PointIdx]);
			checkDirection[1] = 0;
			float len = checkDirection.Length();
			if ((!m_Unit.m_eAI_Ladder || (!m_Unit.m_eAI_IsOnLadder && !m_Unit.eAI_IsCloseToLadderEntryPoint())) && !m_Unit.eAI_IsFallSafe(checkDirection.Normalized() * (len + 2.0), true, DayZPlayerImplementFallDamage.HEALTH_HEIGHT_LOW, true, 1339))
			{
				//! Move next point away from edge if possible
				float offset = 0.55;
				if (m_PathSegmentDirection.LengthSq() > offset * offset)
				{
					vector nextPoint = m_Points[1 + m_PointIdx] - m_PathSegmentDirection.Normalized() * offset;
					m_Points[1 + m_PointIdx] = nextPoint;
					UpdatePoint(m_Next0, nextPoint);
					return true;
				}

			#ifdef DIAG_DEVELOPER
				if (!m_IsUnreachable)
				{
					EXTrace.Print(EXTrace.AI, this, m_Unit.ToString() + " unreachable (path endpoint not fall safe)");
					ExpansionStatic.MessageNearPlayers(m_Unit.GetPosition(), 100.0, m_Unit.ToString() + " unreachable (path endpoint not fall safe)");
				}
			#endif
				m_IsUnreachable = true;
				m_IsTargetUnreachable = true;
				return false;
			}
		}

		return true;
	}

	void UpdatePathSegmentState()
	{
		vector start = m_Points[m_PointIdx];  //! Usually AI position
		vector end = m_Next0.GetPosition();  //! p1

		m_PathSegmentDirection = vector.Direction(start, end);

		//! Blockfilter raycast will return true on doors & vault/climb
		if ((m_Unit.AI_HANDLEVAULTING || m_Unit.AI_HANDLEDOORS) && IsBlocked(start, end, m_BlockFilter))
		{
			m_IsBlocked = true;

			//! Normal pathfilter raycast will return true on vault/climb but not on opened doors
			if (m_Unit.AI_HANDLEVAULTING && IsBlocked(start, end, m_PathFilter))
			{
				if (IsVaultClimb(start, end))
				{
					if (!m_Next0.Parent)
					{
						m_IsJumpClimb = true;
						//m_DoClimbTestEx = true;
					}

					m_SuppressRecalculate = true;
				}
			}
		}
		else if (m_Unit.AI_HANDLEVAULTING)
		{
			if (IsBlockedPhysically(start + "0 0.49 0", end + "0 0.49 0") || IsElevated(start))
			{
				m_IsBlockedPhysically = true;
				if (!m_Next0.Parent)
				{
					m_IsJumpClimb = true;
					//m_DoClimbTestEx = true;
				}
			}
		}

		if (m_Unit.AI_HANDLEVAULTING && !m_SuppressRecalculate && m_Count > 2 + m_PointIdx)
		{
			/**
			 * Vanilla FindPath sometimes places fixed points around some vaultable objects even if AI is already closer to it than p1,
			 * we need to deal with this so GetNext works correctly for AI following formation where there would otherwise be a lot of
			 * path recalculation with possibility of following AI never making the jump due to same p1 and p2 being calculated over
			 * and over while leader is moving.
			 * 
			 *      x         <-- fixed point p1
			 *      v         <-- AI moving towards p2
			 * ------------   <-- vaultable object, e.g. a fence
			 * 
			 *      x         <-- fixed point p2
			 */
			vector p2 = m_Points[2 + m_PointIdx];
			//if (vector.DistanceSq(start, p2) < vector.DistanceSq(end, p2))
			//! Normal pathfilter raycast will return true on vault/climb but not on opened doors
			if (IsBlocked(end, p2, m_PathFilter) && IsVaultClimb(end, p2))
			{
				m_SuppressRecalculate = true;
			}
		}
	}

	bool IsVaultClimb(vector start, vector end)
	{
		//! Rough avg distance of points when vaulting/climbing is 1 m up to 8 m
		//! We use this information to limit vaulting/climbing to any distance between 0.5 and 10 m (squared)
		float distSq = vector.DistanceSq(start, end);
		EXTrace.Print(EXTrace.AI, m_Unit, "Vault dist " + Math.Sqrt(distSq));
		if (distSq > 0.25 && distSq < 100.0)
			return true;

		return false;
	}

	bool IsElevated(vector start)
	{
		if (start[1] - m_Unit.GetPosition()[1] > 0.5)
			return true;

		return false;
	}

	int GetNext(out vector position)
	{
		if (m_IsUnreachable)
		{
			m_Unit.m_eAI_TargetPositionIsFinal = true;
			return 0;
		}

		if (m_Count > m_PointIdx)
			position = m_Next0.GetPosition();// + CalculateOffset();

		return m_Count - m_PointIdx;
	}

	vector GetCurrentPoint()
	{
		if (m_Count > m_PointIdx)
			return m_Points[m_PointIdx];

		return m_Points[m_Count - 1];
	}

	vector GetNextPoint()
	{
		if (m_Count > 1 + m_PointIdx)
			return m_Points[1 + m_PointIdx];

		return m_Points[m_Count - 1];
	}

	vector GetEnd()
	{
		if (m_Count > m_PointIdx)
			return m_Points[m_Count - 1];

		return m_Unit.GetPosition();
	}

	int GetRemainingCount()
	{
		return m_Count - m_PointIdx;
	}

	void UpdatePoint(inout ExpansionPathPoint point, vector position)
	{
		if (!point)
		{
			point = new ExpansionPathPoint();
		}

		point.Position = position;
	#ifdef EXPANSION_AI_ATTACHMENT_PATH_FINDING
		point.Parent = ExpansionAttachmentHelper.IsAttachment(m_Unit, position, 0.5);
		point.OnParentUpdate();
	#endif
	}

	void UpdateCurrent()
	{
		m_Current.Position = m_Unit.GetPosition();
	#ifdef EXPANSION_AI_ATTACHMENT_PATH_FINDING
		m_Current.Parent = Object.Cast(m_Unit.Expansion_GetParent());
		m_Current.OnParentUpdate();
	#endif
	}

	void SetTarget(vector pPosition, float maxDistance = 1.0, bool allowJumpClimb = true)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "pPosition").Add(pPosition);
#endif

		if (m_AllowJumpClimb_Timeout <= 0)
			SetAllowJumpClimb(allowJumpClimb);

		if (m_OverridingPosition)
			return;

		if (pPosition == m_TargetPosition)
		{
			m_SuppressRecalculate = false;
			return;
		}

		m_TargetPosition = pPosition;

		m_IsTargetUnreachable = false;

		m_Recalculate = true;
	}

	vector GetTarget()
	{
		return m_TargetReference.GetPosition();
	}

	void OverridePosition(vector pPosition, bool forceUpdate = false)
	{
		StopOverride();
		SetTarget(pPosition, 1.0);
		m_OverridingPosition = true;
		m_SuppressRecalculate = false;
		if (forceUpdate)
			m_Time = PATH_RECALCULATE_THRESHOLD;
	}

	bool GetOverride()
	{
		return m_OverridingPosition;
	}

	void StopOverride()
	{
		m_OverridingPosition = false;
	}

	void ForceRecalculate(bool forceUpdate = false)
	{
	//#ifdef EXTRACE_DIAG
		//auto trace = EXTrace.StartStack(EXTrace.AI, m_Unit, "forceUpdate " + forceUpdate);
	//#endif

		if (m_TargetPosition == vector.Zero)
			return;

		m_Recalculate = true;
		m_SuppressRecalculate = false;
		if (forceUpdate)
			m_Time = PATH_RECALCULATE_THRESHOLD;
	}

	void ResetUnreachable()
	{
		m_IsTargetUnreachable = false;
		m_IsUnreachable = false;
		m_SuppressRecalculate = false;
		m_Unit.m_eAI_PositionTime = 0;
	}
};
