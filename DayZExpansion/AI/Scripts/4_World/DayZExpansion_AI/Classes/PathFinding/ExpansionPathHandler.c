#include "0_DayZExpansion_AI_Preload/Common/DayZExpansion_AI_Defines.c"

class ExpansionPathHandler
{
	static bool ATTACHMENT_PATH_FINDING = true;
	
	eAIBase m_Unit;
	float m_Time;

	autoptr PGFilter m_PathFilter;
	autoptr PGFilter m_CheckFilter;

	ref ExpansionPathPoint m_Current;
	ref ExpansionPathPoint m_Target;
	ref ExpansionPathPoint m_TargetReference;

	vector m_OverridePosition;
	bool m_OverridingPosition;

	int m_Count;
	ref ExpansionPathPoint m_Next0;
	ref ExpansionPathPoint m_Next1;

	AIWorld m_AIWorld;

	ref array<ref ExpansionPathPoint> m_Path = new array<ref ExpansionPathPoint>();
	ref array<vector> m_Points = new array<vector>();

	bool m_Recalculate;

	void ExpansionPathHandler(eAIBase unit)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "ExpansionPathHandler").Add(unit);
#endif

		m_Unit = unit;

		m_PathFilter = new PGFilter();
		m_CheckFilter = new PGFilter();

		m_AIWorld = GetGame().GetWorld().GetAIWorld();

		m_Current = new ExpansionPathPoint();
		m_Target = new ExpansionPathPoint();
		m_TargetReference = new ExpansionPathPoint();
		m_Next0 = new ExpansionPathPoint();
		m_Next1 = new ExpansionPathPoint();

		SetPathFilter();
	}

	private void SetPathFilter()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "SetPathFilter");
#endif

		m_PathFilter = new PGFilter();

		int in = PGPolyFlags.UNREACHABLE | PGPolyFlags.DISABLED | PGPolyFlags.WALK | PGPolyFlags.DOOR | PGPolyFlags.INSIDE | PGPolyFlags.LADDER;
		int ex = PGPolyFlags.CRAWL | PGPolyFlags.CROUCH | PGPolyFlags.SWIM_SEA | PGPolyFlags.SWIM;
		int ec = PGPolyFlags.NONE;

		if (eAIBase.AI_HANDLEVAULTING)
		{
			in |= PGPolyFlags.SPECIAL;
		}
		else
		{
			ex |= PGPolyFlags.SPECIAL;
		}

		m_PathFilter.SetCost(PGAreaType.LADDER, 1.0);
		m_PathFilter.SetCost(PGAreaType.CRAWL, 1.0);
		m_PathFilter.SetCost(PGAreaType.CROUCH, 1.0);
		m_PathFilter.SetCost(PGAreaType.FENCE_WALL, 1.0);
		m_PathFilter.SetCost(PGAreaType.JUMP, 1.0);
		m_PathFilter.SetCost(PGAreaType.WATER, 1.0);

		m_PathFilter.SetCost(PGAreaType.DOOR_CLOSED, 1.0);
		m_PathFilter.SetCost(PGAreaType.DOOR_OPENED, 1.0);

		m_PathFilter.SetCost(PGAreaType.ROADWAY, 1.0);
		m_PathFilter.SetCost(PGAreaType.TREE, 1.0);

		m_PathFilter.SetCost(PGAreaType.OBJECTS_NOFFCON, 1.0);
		m_PathFilter.SetCost(PGAreaType.OBJECTS, 1.0);
		m_PathFilter.SetCost(PGAreaType.TERRAIN, 1.0);
		m_PathFilter.SetCost(PGAreaType.BUILDING, 1.0);
		m_PathFilter.SetCost(PGAreaType.ROADWAY_BUILDING, 1.0);

		m_PathFilter.SetFlags(in, ex, ec);
	}

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
		if (m_Count <= 1)
			return false;

		return true;

		//vector hitPos;
		//return Raycast(PGPolyFlags.DOOR, 1.5, hitPos);
	}

	bool IsVault()
	{
		if (m_Count <= 1)
			return false;
			
		if (m_Current && m_Current.NavMesh)
		{
			vector hitPos;
			return Raycast(PGPolyFlags.CLIMB, 0.5, hitPos);
		}

		return true;

		//vector hitPos;
		//return Raycast(PGPolyFlags.CLIMB, 0.5, hitPos);
	}
	
	bool IsBlocked(vector start, vector end)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_2(this, "IsBlocked").Add(start).Add(end);
		#endif

		vector hitPos;
		vector hitNormal;
		
		return m_AIWorld.RaycastNavMesh(start, end, m_PathFilter, hitPos, hitNormal);
	}
	
	bool IsBlocked(vector start, vector end, out vector hitPos, out vector hitNormal)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_2(this, "IsBlocked").Add(start).Add(end);
		#endif

		return m_AIWorld.RaycastNavMesh(start, end, m_PathFilter, hitPos, hitNormal);
	}

	vector CalculateOffset()
	{
		if (m_Count <= 2)
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
		
#ifdef EAI_DEBUG_PATH
#ifndef SERVER		
		//m_Unit.AddShape(Shape.CreateSphere(0xFFFF0000, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, m_Current.GetPosition(), 0.3));
		//m_Unit.AddShape(Shape.CreateSphere(0xFF00FF00, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, m_Target.GetPosition(), 0.3));
		
		//m_Unit.AddShape(Shape.CreateSphere(0xFF0000FF, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, m_TargetReference.GetPosition(), 0.3));
		
		//m_Unit.AddShape(Shape.CreateSphere(0xFFFF00AA, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, m_Next0.GetPosition(), 0.3));
		//m_Unit.AddShape(Shape.CreateSphere(0xFFAA00FF, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, m_Next1.GetPosition(), 0.3));

		int i;		
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

		for (i = 0; i < m_Count; i++)
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
#endif
#endif
	}

	void OnUpdate(float pDt, int pSimulationPrecision)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_2(this, "OnUpdate").Add(pDt).Add(pSimulationPrecision);
#endif

		m_Time += pDt;
		
		//SetPathFilter();
		
		vector unitPosition = m_Unit.GetPosition();
		vector unitDirection = m_Unit.GetDirection();
		vector unitVelocity = GetVelocity(m_Unit);

		vector targetPosition = m_TargetReference.GetPosition();

		bool recalculate = m_Recalculate;
		bool recalculateFromNext = false;
		
#ifdef DIAG
		recalculate = true;
#endif

		int i;
		
		if (pSimulationPrecision < 0)
		{
			recalculate = true;
			
			// HACK FIX for recalculating while climbing
			
			unitPosition = unitPosition + (unitDirection * 0.5);
			
			
			m_Current.Position = unitPosition;
			m_Current.Parent = Object.Cast(m_Unit.GetParent());
			m_Current.OnParentUpdate();
		}
		else
		{
			UpdateCurrent();
		}
		
		if (!recalculate && m_Time >= ((pSimulationPrecision + 1.0) * 2.0))
		{
			recalculate = vector.DistanceSq(unitPosition, targetPosition) > 0.5;
		}

		if (!recalculate && m_Count >= 1)
		{
			vector next0Position = m_Next0.GetPosition();

			float d0 = vector.DistanceSq(unitPosition, next0Position);
			float d1 = vector.DistanceSq(unitPosition + unitVelocity, next0Position);
			if (d0 < d1)
			{
				recalculate = true;

				//recalculateFromNext
			}
		}

		if (recalculate)
		{
			m_Time = 0;

			array<vector> tempPath();

			m_Points.Clear();
			m_Count = 0;
			
			int F_STATE = -1;
			int P_STATE = -1;

			if (ATTACHMENT_PATH_FINDING)
			{
				if (m_Current.Parent && !m_TargetReference.Parent) // moving to world
				{
					F_STATE = 1;
					
					m_Target.Copy(m_Current);
	
					m_Target.Position = m_TargetReference.Position;
					m_Target.OnParentUpdate();
	
					m_Target.FindPath(this, m_Points);
					
					m_Count = m_Points.Count();
	
					if (m_Count == 2)
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
	
					m_Target.Position = unitPosition;
					m_Target.OnParentUpdate();
					
					if (!m_Target.NavMesh)
					{
						Print("Navmesh removed??");
					}
					
					vector checkingIfAlreadyOnPathsPosition = m_Target.Position;
					
					m_Target.NavMesh.SamplePosition(checkingIfAlreadyOnPathsPosition, checkingIfAlreadyOnPathsPosition);
					
					float heightDiff = Math.AbsFloat(m_Target.Position[1] - checkingIfAlreadyOnPathsPosition[1]) * 2.0;
					
					bool isAlreadyOnPaths = vector.Distance(checkingIfAlreadyOnPathsPosition, m_Target.Position) < heightDiff;
			
					if (isAlreadyOnPaths)
					{
						m_Target.Copy(m_TargetReference);
		
						m_Target.FindPath(this, m_Points);
		
						m_Count = m_Points.Count();
					}
					else
					{
						m_Target.FindPathFrom(m_TargetReference.GetPosition(), this, tempPath);
						m_Count = tempPath.Count();
						
						// Find the path to the entry
						
						m_Target.Position = m_Current.Position;
						m_Target.Parent = null;
						m_Target.OnParentUpdate();
						
						vector closestPositionOnAttachment = tempPath[m_Count - 1];
											
						m_Target.FindPathFrom(closestPositionOnAttachment, this, m_Points);
						
						m_Path.RemoveOrdered(m_Count);
						
						m_Path.Invert();					
						m_Points.Invert();
						
						m_Points.Remove(m_Points.Count() - 1);
					
						for (i = 0; i < tempPath.Count(); i++)
						{
							int ii = tempPath.Count() - (i + 1);
							m_Points.Insert(tempPath[ii]);
						}
						
						m_Count = m_Points.Count();
					}
				}
				else if (m_TargetReference.Parent) // moving in attachment
				{
					F_STATE = 3;
					
					m_Target.Copy(m_TargetReference);
	
					m_Target.FindPath(this, m_Points);
	
					m_Count = m_Points.Count();
				}
				else if (!m_TargetReference.Parent) // moving in world
				{
					F_STATE = 4;
					
					m_Target.Copy(m_TargetReference);
	
					m_Target.FindPath(this, m_Points);
	
					m_Count = m_Points.Count();
				}
				else
				{
					F_STATE = 5;
				}
			}
			else
			{
				F_STATE = 6;
					
				m_Target.Copy(m_TargetReference);

				m_Target.FindPath(this, m_Points);

				m_Count = m_Points.Count();
			}
			
			if (m_Count > 2)
			{
				P_STATE = 1;
				
				UpdatePoint(m_Next0, m_Points[1]);
				UpdatePoint(m_Next1, m_Points[2]);
			}
			else if (m_Count != 0)
			{
				P_STATE = 2;
				
				UpdatePoint(m_Next0, m_Points[1]);
			}
			else 
			{
				P_STATE = 3;
				
				m_Next0.Copy(m_Current);
			}
			
			//Print(F_STATE);
			//Print(P_STATE);
		}
		
		if (m_Count == 2)
		{
			UpdatePoint(m_Next0, m_TargetReference.GetPosition());
		}

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

#ifdef DIAG
		DrawDebug();
#endif
	}

	int GetNext(out vector position)
	{
		position = m_Next0.GetPosition();// + CalculateOffset();

		//! Hack fix so AI doesn't fall from a large height (e.g. building top) and die
		//! FIXME: Marked improvement, but doesn't always prevent AI just running over the edge :-(
		vector unitPosition = m_Unit.GetPosition();
		if (unitPosition[1] - position[1] > 2.5)
		{
			vector checkPosition = Vector(position[0], unitPosition[1], position[2]);
			vector checkDirection = vector.Direction(unitPosition, checkPosition).Normalized() * 2;
			if (!m_Unit.eAI_IsFallSafe(checkDirection))
			{
				position = unitPosition;
				return 1;
			}
		}

		return m_Count;
	}

	void UpdatePoint(inout ExpansionPathPoint point, vector position)
	{
		if (!point)
		{
			point = new ExpansionPathPoint();
		}

		point.Position = position;
		point.Parent = ExpansionAttachmentHelper.IsAttachment(m_Unit, position, 0.5);
		point.OnParentUpdate();
	}

	void UpdateCurrent()
	{
		m_Current.Position = m_Unit.GetPosition();
		m_Current.Parent = Object.Cast(m_Unit.GetParent());
		m_Current.OnParentUpdate();
	}

	void SetTarget(vector pPosition)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "pPosition").Add(pPosition);
#endif

		if (m_OverridingPosition)
			pPosition = m_OverridePosition;

		vector oldPos = m_TargetReference.Position;

		UpdatePoint(m_TargetReference, pPosition);
		
		if (m_TargetReference.Parent)
		{
			return;
		}

		vector inPos = pPosition;

		// TODO: investigate why the same variable source must be used for 0th and 3rd parameter, and that it can't be a member variable for either
		if (!m_AIWorld.SampleNavmeshPosition(inPos, 1.0, m_PathFilter, inPos))
		{
			//inPos = oldPos;
		}

		m_TargetReference.Position = inPos;
	}

	void OverridePosition(vector pPosition)
	{
		m_OverridePosition = pPosition;
		m_OverridingPosition = true;
	}

	bool GetOverride()
	{
		return m_OverridingPosition;
	}

	void StopOverride()
	{
		m_OverridingPosition = false;
	}
};
