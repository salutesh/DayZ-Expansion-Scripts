class ExpansionAIPatrolManager
{
	static eAIDynamicPatrol InitObjectPatrol(string type, vector position)
	{
		if (!eAIDynamicPatrol.InitSettings())
			return NULL;

		type.ToLower();

		array<ref ExpansionAIDynamicSpawnBase> patrols;
		if (!eAIDynamicPatrol.s_AIPatrolSettings.m_ObjectPatrols.Find(type, patrols))
			return null;

		//! Find object patrol with a chance to spawn
		foreach (ExpansionAIDynamicSpawnBase patrol: patrols)
		{
			auto dynPatrol = InitPatrol(patrol, position);
			if (dynPatrol)
				return dynPatrol;
		}

		return null;
	}

	static eAIDynamicPatrol InitPatrol(ExpansionAIDynamicSpawnBase patrol, vector position = vector.Zero)
	{
		if (patrol.Chance == 0.0 || patrol.Chance < Math.RandomFloat(0.0, 1.0))
			return null;

		return eAIDynamicPatrol.CreateEx(patrol, position);
	}

	static void InitPatrols()
	{
		if (!eAIDynamicPatrol.InitSettings())
			return;

		//! Init patrols w/ waypoints
		foreach (int i, ExpansionAIDynamicSpawnBase patrol: eAIDynamicPatrol.s_AIPatrolSettings.m_Patrols)
		{
			if (!patrol.Waypoints || !patrol.Waypoints.Count())
			{
				if (patrol.Name)
					EXError.Error(null, "AIPatrolSettings.json: Patrol '" + patrol.Name + "' at index " + i + " has no waypoints", {});
				else
					EXError.Error(null, "AIPatrolSettings.json: Patrol at index " + i + " has no waypoints", {});
				continue;
			}

			if (!InitPatrol(patrol))
				continue;
		}
	}
}
