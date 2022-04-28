#include "0_DayZExpansion_AI_Preload/Common/DayZExpansion_AI_Defines.c"

class ExpansionDebugPathPoint
{
	vector Position;
	Object Parent;

	void ExpansionDebugPathPoint(vector position, Object parent)
	{
		Position = position;
		Parent = parent;
		
		if (Parent)
		{
			vector transform[4];
			Parent.GetTransform(transform);

			Position = Position.InvMultiply4(transform);
		}
	}

	vector GetPosition()
	{
		if (Parent)
		{
			vector transform[4];
			Parent.GetTransform(transform);

			return Position.Multiply4(transform);
		}

		return Position;
	}
};

class ExpansionPathPoint
{
	vector Position;
	Object Parent;

	ExpansionNavMesh NavMesh;

	vector GetPosition()
	{
		if (Parent)
		{
			vector transform[4];
			Parent.GetTransform(transform);

			return Position.Multiply4(transform);
		}

		return Position;
	}

	void OnParentUpdate()
	{
		if (Parent)
		{
			NavMesh = ExpansionNavMeshes.Get(Parent);

			if (!NavMesh)
			{
				Parent = null;
			}
		}

		if (Parent)
		{
			vector transform[4];
			Parent.GetTransform(transform);

			Position = Position.InvMultiply4(transform);
		}
		else
		{
			NavMesh = null;
		}
	}

	void Copy(ExpansionPathPoint other)
	{
		Position = other.Position;
		Parent = other.Parent;
		NavMesh = other.NavMesh;
	}

	void FindPath(ExpansionPathHandler pathFinding, inout array<vector> path)
	{
		vector transform[4];
		pathFinding.m_Unit.GetTransform(transform);
		
		FindPathFrom(transform[3], pathFinding, path);
	}

	void FindPathFrom(vector startPos, ExpansionPathHandler pathFinding, inout array<vector> path, bool insertDebugReverse = false)
	{
		int i;
		
		if (Parent)
		{
			vector transform[4];
			Parent.GetTransform(transform);

			NavMesh.FindPath(startPos.InvMultiply4(transform), Position, path);
			
			for (i = 0; i < path.Count(); i++)
			{
				path[i] = path[i].Multiply4(transform);
			}
		}
		else
		{
			pathFinding.m_AIWorld.FindPath(startPos, Position, pathFinding.m_PathFilter, path);
		}

		if (insertDebugReverse)
		{
			for (i = path.Count() - 1; i >= 0; i--)
			{
				pathFinding.m_DebugPath.Insert(new ExpansionDebugPathPoint(path[i], Parent));
			}
		}
		else
		{
			for (i = 0; i < path.Count(); i++)
			{
				pathFinding.m_DebugPath.Insert(new ExpansionDebugPathPoint(path[i], Parent));
			}
		}
	}
};
