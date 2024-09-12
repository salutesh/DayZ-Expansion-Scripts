class ExpansionAIPatrolManager
{
	static eAIDynamicPatrol InitObjectPatrol(string type, vector position)
	{
		if (!eAIDynamicPatrol.InitSettings())
			return NULL;

		//! Init matching object patrol
		foreach(ExpansionAIObjectPatrol patrol: eAIDynamicPatrol.s_AIPatrolSettings.ObjectPatrols)
		{
			if (!patrol.ClassName || !CF_String.EqualsIgnoreCase(patrol.ClassName, type))
				continue;

			auto dynPatrol = InitPatrol(patrol, position);
			if (!dynPatrol)
				continue;

			//! Return dynamic patrol
			return dynPatrol;
		}

		return NULL;
	}

	static eAIDynamicPatrol InitPatrol(ExpansionAIDynamicSpawnBase patrol, vector position = vector.Zero)
	{
		if (patrol.Chance == 0.0 || patrol.Chance < Math.RandomFloat(0.0, 1.0))
			return NULL;

		return eAIDynamicPatrol.CreateEx(patrol, position);
	}

	static void InitPatrols()
	{
		if (!eAIDynamicPatrol.InitSettings())
			return;

		//! Init all roaming patrols
		foreach(ExpansionAIPatrol patrol: eAIDynamicPatrol.s_AIPatrolSettings.Patrols)
		{
			if (!patrol.Waypoints || !patrol.Waypoints.Count())
			{
				eAIDynamicPatrol.Log(patrol, "!!! ERROR !!!");
				eAIDynamicPatrol.Log(patrol, "No waypoints (validate your file with a json validator)");
				eAIDynamicPatrol.Log(patrol, "!!! ERROR !!!");
				continue;
			}

			if (!InitPatrol(patrol))
				continue;
		}
	}
}
