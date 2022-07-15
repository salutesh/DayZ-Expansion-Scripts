/**
 * ExpansionAIPatrolBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAISpawnBase
{
	string Faction;                     // Raiders, Mercenaries, West, East, Guards, Civilian, Passive
	string Formation;                   // Column, File, Vee, Wall or RANDOM
	string LoadoutFile;                 // a json file containing the loadout of this team - if empty, will use the default loadout of the faction
	int NumberOfAI;                     // How many bots, -x will make it random between 0 and x
	string Behaviour;                   // See eAIWaypointBehavior
	string Speed;                       // See eAIMovementSpeed
	string UnderThreatSpeed;            // 
	bool CanBeLooted;                   // if enabled, the bots can be looted by the players
	bool UnlimitedReload;               // should bots be able to reload indefinitely (still needs spare mag in inventory!)

	void ExpansionAISpawnBase(int bod = 1, string spd = "JOG", string threatspd = "SPRINT", string beh = "ALTERNATE", string fac = "WEST", string loa = "", bool canbelooted = true, bool unlimitedreload = false)
	{
		NumberOfAI = bod;
		Speed = spd;
		UnderThreatSpeed = threatspd;
		Behaviour = beh;
		Faction = fac;
		LoadoutFile = loa;
		CanBeLooted = canbelooted;
		UnlimitedReload = unlimitedreload;
	}

	float GetSpeed()
	{
		return typename.StringToEnum(eAIMovementSpeed, Speed);
	}

	float GetThreatSpeed()
	{
		return typename.StringToEnum(eAIMovementSpeed, UnderThreatSpeed);
	}

	int GetBehaviour()
	{
		return typename.StringToEnum(eAIWaypointBehavior, Behaviour);
	}

	void UpdateSettings()
	{
		//! v1/v2 to v3
		switch (Faction)
		{
			case "INSURGENT":
				Faction = "Raiders";
				break;
			case "WEST":
			case "EAST":
			case "RAIDERS":
			case "CIVILIAN":
			case "GUARDS":
			case "PASSIVE":
				string lowerpart = Faction.Substring(1, Faction.Length() - 1);
				lowerpart.ToLower();
				Faction = Faction[0] + lowerpart;
				break;
		}

		//! v1/v2/v3 to v4
		switch (Behaviour)
		{
			case "HOLD":
				Behaviour = "HALT";
				break;
			case "PATROL":
			case "REVERSE":
				Behaviour = "ALTERNATE";
				break;
			case "HOLD OR PATROL":
				Behaviour = "HALT_OR_ALTERNATE";
				break;
		}

		if (Speed == "RANDOM")
			Speed = "RANDOM_NONSTATIC";

		if (UnderThreatSpeed == "RANDOM")
			UnderThreatSpeed = "RANDOM_NONSTATIC";
	}

	TVectorArray PathInterpolated(TVectorArray path, ECurveType curveType = ECurveType.CatmullRom)
	{
		TVectorArray points();
		TVectorArray interpolatedPath = {path[0]};

		foreach (vector pathPoint: path)
		{
			points.Insert(Vector(pathPoint[0], pathPoint[2], 0));
		}

		int index;
		float t;
		float step = 0.01;
		float tEnd = path.Count() - 1;
		vector previousPoint3D = path[0];
		float previousAngle;
		while (t < tEnd)
		{
			t += step;

			float f = t - Math.Floor(t);
			//if (f > 0.25 && f < 0.75)
				//continue;

			vector curvePoint = Math3D.Curve(curveType, t / tEnd, points);
			float y = GetGame().SurfaceY(curvePoint[0], curvePoint[1]);
			vector point3D = Vector(curvePoint[0], y, curvePoint[1]);

			previousPoint3D[1] = point3D[1];
			vector curveDir2D = vector.Direction(previousPoint3D, point3D);

			float angle = curveDir2D.VectorToAngles()[0];

			if (/*t != Math.Round(t) && */interpolatedPath.Count() > 1 && t < tEnd)
			{
				float angleDiff = Math.AbsFloat(ExpansionMath.AngleDiff2(previousAngle, angle));
				if (angleDiff < 5.0)
					continue;
			}

			if (/*t == Math.Round(t) || */curveDir2D.Length() > 4.472136 || t == tEnd)
			{
				previousPoint3D = point3D;
				previousAngle = angle;

				interpolatedPath.Insert(point3D);
			}
		}

		return interpolatedPath;
	}
};

class ExpansionAIDynamicSpawnBase: ExpansionAISpawnBase
{
	float MinDistRadius;	            // If the player is closer than MinDistRadius from the spawn point, the patrol won't spawn, if set to -2, will use the general setting instead
	float MaxDistRadius;	            // Same but if the player is further away than MaxDistRadius, the bots won't spawn, if set to -2, will use the general setting instead
	float DespawnRadius;
	float MinSpreadRadius;
	float MaxSpreadRadius;
	float Chance;                       // chance for this patrol to spawn
	string WaypointInterpolation;

	void ExpansionAIDynamicSpawnBase(int bod = 1, string spd = "JOG", string threatspd = "SPRINT", string beh = "ALTERNATE", string fac = "WEST", string loa = "", bool canbelooted = true, bool unlimitedreload = false, float chance = 1.0, float mindistradius = -2, float maxdistradius = -2)
	{
		Chance = chance;
		MinDistRadius = mindistradius;
		MaxDistRadius = maxdistradius;
		DespawnRadius = maxdistradius * 1.1;
	}
};
