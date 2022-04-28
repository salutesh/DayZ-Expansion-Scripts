#include "0_DayZExpansion_AI_Preload/Common/DayZExpansion_AI_Defines.c"

class ExpansionPathHandler
{
	static bool ATTACHMENT_PATH_FINDING = false;
	
	eAIBase m_Unit;
	float m_Time;

	autoptr PGFilter m_PathFilter;
	autoptr PGFilter m_CheckFilter;

	ref ExpansionPathPoint m_Current;
	ref ExpansionPathPoint m_Target;
	ref ExpansionPathPoint m_TargetReference;

	int m_Count;
	ref ExpansionPathPoint m_Next0;
	ref ExpansionPathPoint m_Next1;

	AIWorld m_AIWorld;

	ref array<ref ExpansionDebugPathPoint> m_DebugPath = new array<ref ExpansionDebugPathPoint>();

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

		int in = PGPolyFlags.WALK | PGPolyFlags.DOOR | PGPolyFlags.INSIDE /* | PGPolyFlags.JUMP | PGPolyFlags.CLIMB*/;
		int ex = PGPolyFlags.DISABLED | PGPolyFlags.CRAWL | PGPolyFlags.CROUCH | PGPolyFlags.SWIM_SEA | PGPolyFlags.SWIM;
		int ec = PGPolyFlags.NONE;

		m_PathFilter.SetCost(PGAreaType.LADDER, 3.0);
		m_PathFilter.SetCost(PGAreaType.CRAWL, 0.0);
		m_PathFilter.SetCost(PGAreaType.CROUCH, 0.0);
		m_PathFilter.SetCost(PGAreaType.FENCE_WALL, 0.0);
		m_PathFilter.SetCost(PGAreaType.JUMP, 0.0);
		m_PathFilter.SetCost(PGAreaType.WATER, 5.0);

		m_PathFilter.SetCost(PGAreaType.DOOR_CLOSED, 1.0);
		// m_PathFilter.SetCost(PGAreaType.DOOR_OPENED, 0.0);

		// m_PathFilter.SetCost(PGAreaType.ROADWAY_BUILDING, 1.0);
		// m_PathFilter.SetCost(PGAreaType.TREE, 1.0);

		// m_PathFilter.SetCost(PGAreaType.OBJECTS_NOFFCON, 1.0);
		m_PathFilter.SetCost(PGAreaType.OBJECTS, 1.0);
		m_PathFilter.SetCost(PGAreaType.TERRAIN, 1.0);
		m_PathFilter.SetCost(PGAreaType.BUILDING, 1.0);
		m_PathFilter.SetCost(PGAreaType.ROADWAY_BUILDING, 1.0);

		m_PathFilter.SetFlags(in, ex, ec);
	}

	bool IsDoor(out vector hitPos)
	{
		if (m_Count <= 1)
			return false;

		int in = PGPolyFlags.DOOR;
		int ex = PGPolyFlags.WALK | PGPolyFlags.DISABLED | PGPolyFlags.CRAWL | PGPolyFlags.CROUCH | PGPolyFlags.SWIM_SEA | PGPolyFlags.SWIM | PGPolyFlags.INSIDE | PGPolyFlags.JUMP | PGPolyFlags.CLIMB;
		int ec = PGPolyFlags.NONE;

		m_CheckFilter.SetFlags(in, ex, ec);
		m_CheckFilter.SetCost(PGAreaType.BUILDING, 1.0);

		vector p0 = m_Unit.GetPosition() - (m_Unit.GetDirection() * 0.5);
		vector p1 = m_Next0.GetPosition() + (m_Unit.GetDirection() * 1.5);

#ifdef EAI_DEBUG_PATH
#ifndef SERVER
		//m_Unit.AddShape(Shape.CreateSphere(0xFF0000FF, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, p0, 0.2));
		//m_Unit.AddShape(Shape.CreateSphere(0xFF0000FF, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, p1, 0.2));
#endif
#endif

		vector hitNor;
		return m_AIWorld.RaycastNavMesh(p0, p1, m_PathFilter, hitPos, hitNor);
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

		if (index >= m_DebugPath.Count())
		{
			index = m_DebugPath.Count() - 1;
		}

		return m_DebugPath[index].GetPosition();
	}

	vector CalculateDebugOffset(int index)
	{
		if (m_DebugPath.Count() <= 2)
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
		vector pos;
		if (IsDoor(pos))
		{
			//m_Unit.AddShape(Shape.CreateSphere(0xFF00FF00, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, pos, 0.3));
		}
		
		//m_Unit.AddShape(Shape.CreateSphere(0xFFFF0000, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, m_Current.GetPosition(), 0.3));
		//m_Unit.AddShape(Shape.CreateSphere(0xFF00FF00, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, m_Target.GetPosition(), 0.3));
		
		//m_Unit.AddShape(Shape.CreateSphere(0xFF0000FF, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, m_TargetReference.GetPosition(), 0.3));
		
		//m_Unit.AddShape(Shape.CreateSphere(0xFFFF00AA, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, m_Next0.GetPosition(), 0.3));
		//m_Unit.AddShape(Shape.CreateSphere(0xFFAA00FF, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, m_Next1.GetPosition(), 0.3));

		int i;
		vector points[2];
		vector offset = vector.Zero;

		vector offsetOffset = "0 0 0";

		for (i = 0; i < m_DebugPath.Count() - 1; i++)
		{
			bool isFinal = (i + 1) == (m_DebugPath.Count() - 1);

			int color = 0xFF00FF00;
			if (m_DebugPath[i + 1].Parent)
				color = 0xFF0000FF;

			points[0] = GetDebugPosition(i);
			points[1] = GetDebugPosition(i + 1);

			m_Unit.AddShape(Shape.CreateLines(color, ShapeFlags.VISIBLE | ShapeFlags.NOZBUFFER, points, 2));

			if (i != 0)
				offset = CalculateDebugOffset(i);
			points[0] = points[0] + offset + offsetOffset;

			offset = vector.Zero;

			if (!isFinal)
				offset = CalculateDebugOffset(i + 1);
			points[1] = points[1] + offset + offsetOffset;

			//m_Unit.AddShape(Shape.CreateLines(0xAAFF0000, ShapeFlags.VISIBLE | ShapeFlags.NOZBUFFER | ShapeFlags.TRANSP, points, 2));
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

		UpdateCurrent();

		vector unitPosition = m_Unit.GetPosition();
		vector unitVelocity = GetVelocity(m_Unit);

		vector targetPosition = m_TargetReference.GetPosition();

		bool recalculate = false;
		if (m_Time >= ((pSimulationPrecision + 1.0) * 2.0))
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
			}
		}

		if (recalculate)
		{
			m_Time = 0;

			array<vector> path();
			array<vector> tempPath();

			m_DebugPath.Clear();
			
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
	
					m_Target.FindPath(this, path);
					
					m_Count = path.Count();
	
					if (m_Count == 2)
					{
						vector pathDir = vector.Direction(path[0], path[1]).Normalized();
						path[1] = path[1] + (pathDir * 2.0);
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
	
					m_Target.FindPathFrom(m_TargetReference.GetPosition(), this, tempPath);
					m_Count = tempPath.Count();
					
					//m_Unit.AddShape(Shape.CreateSphere(0xFFFF0000, ShapeFlags.NOZBUFFER, tempPath[m_Count - 1], 0.5));
					
					// Find the path to the entry
						
					m_Target.Position = m_Current.Position;
					m_Target.Parent = null;
					m_Target.OnParentUpdate();
	
					m_Target.FindPathFrom(tempPath[m_Count - 1], this, path);
					m_Count = tempPath.Count() + path.Count();
					
					m_DebugPath.Invert();
					path.Invert();
					
					for (int i = 0; i < tempPath.Count(); i++)
					{
						int ii = tempPath.Count() - (i + 1);
						path.Insert(tempPath[ii]);
					}
				}
				else if (m_TargetReference.Parent) // moving in attachment
				{
					F_STATE = 3;
					
					m_Target.Copy(m_TargetReference);
	
					m_Target.FindPath(this, path);
	
					m_Count = path.Count();
				}
				else if (!m_TargetReference.Parent) // moving in world
				{
					F_STATE = 4;
					
					m_Target.Copy(m_TargetReference);
	
					m_Target.FindPath(this, path);
	
					m_Count = path.Count();
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

				m_Target.FindPath(this, path);

				m_Count = path.Count();
			}

			if (m_Count > 2)
			{
				P_STATE = 1;
				UpdatePoint(m_Next0, path[0]);
				UpdatePoint(m_Next1, path[1]);
			}
			else if (m_Count != 0)
			{
				P_STATE = 2;
				UpdatePoint(m_Next0, path[1]);
			}
			else 
			{
				P_STATE = 3;
				m_Next0.Copy(m_Current);
			}
			
			//Print(F_STATE);
			//Print(P_STATE);
		}

		if (m_DebugPath.Count() > 0)
		{
			m_DebugPath[0].Position = unitPosition;
			
			if (m_DebugPath[0].Parent)
			{
				vector transform[4];
				m_DebugPath[0].Parent.GetTransform(transform);
	
				m_DebugPath[0].Position = unitPosition.InvMultiply4(transform);
			}
		}

#ifdef DIAG
		DrawDebug();
#endif
	}

	int GetNext(out vector position)
	{
		position = m_Next0.GetPosition() + CalculateOffset();
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
			inPos = oldPos;
		}

		m_TargetReference.Position = inPos;
	}
};
