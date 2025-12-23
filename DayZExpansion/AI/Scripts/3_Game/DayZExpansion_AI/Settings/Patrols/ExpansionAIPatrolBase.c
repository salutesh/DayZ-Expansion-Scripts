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
	string Name;  // user-definable name for easier recognition
	bool Persist;   // Patrol will be saved & restored between server restarts unless all members of it are killed
	string Faction;                     // Raiders, Mercenaries, West, East, Guards, Civilian, Passive
	string Formation;                   // Column, File, Vee, Wall or RANDOM
	float FormationScale;
	float FormationLooseness;
	string Loadout;                 // a json file containing the loadout of this team - if empty, will use the default loadout of the faction
	ref TStringArray Units = {};        // If non-empty, pick from these AI classnames when spawning
	int NumberOfAI;                     // How many bots, -x will make it random between 0 and x
	string Behaviour;                   // See eAIWaypointBehavior
	string LootingBehaviour;
	string Speed;                       // See eAIMovementSpeed
	string UnderThreatSpeed;            //
	string DefaultStance = "STANDING";
	float DefaultLookAngle = -1;
	bool CanBeLooted;                   // if enabled, the bots can be looted by the players
	string LootDropOnDeath;            // Loot to drop when AI dies (file name)
	int UnlimitedReload;               // should bots be able to reload indefinitely (still needs spare mag in inventory!)
	float SniperProneDistanceThreshold;
	float AccuracyMin;
	float AccuracyMax;
	float ThreatDistanceLimit;
	float NoiseInvestigationDistanceLimit;
	float MaxFlankingDistance = -1;
	int EnableFlankingOutsideCombat = -1;
	float DamageMultiplier;
	float DamageReceivedMultiplier;
	float HeadshotResistance;

	[NonSerialized()]
	string m_BaseName;

	void ExpansionAISpawnBase(int bod = 1, string spd = "JOG", string threatspd = "SPRINT", string beh = "ALTERNATE", string fac = "West", string loa = "", bool canbelooted = true, int unlimitedreload = 0)
	{
		NumberOfAI = bod;
		Speed = spd;
		UnderThreatSpeed = threatspd;
		Behaviour = beh;
		Faction = fac;
		Loadout = loa;
		CanBeLooted = canbelooted;
		UnlimitedReload = unlimitedreload;
		AccuracyMin = -1;
		AccuracyMax = -1;
		ThreatDistanceLimit = -1;
		NoiseInvestigationDistanceLimit = -1;
		DamageMultiplier = -1;
		DamageReceivedMultiplier = -1;
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

	eAIStance GetDefaultStance()
	{
		return typename.StringToEnum(eAIStance, DefaultStance);
	}

	void SetDefaultLootingBehaviour()
	{
		if (Behaviour == "ROAMING")
			LootingBehaviour = "ALL";
		else
			LootingBehaviour = "DEFAULT";
	}

	int GetLootingBehaviour()
	{
		if (!LootingBehaviour)
			SetDefaultLootingBehaviour();

		int bitmask;

		TStringArray lootingBehaviors = {};
		LootingBehaviour.Split("|", lootingBehaviors);
		foreach (string lootingBehavior: lootingBehaviors)
		{
			lootingBehavior.TrimInPlace();
			int value = typename.StringToEnum(eAILootingBehavior, lootingBehavior);
			if (value == -1)
				EXError.Error(this, "Invalid looting behaviour " + lootingBehavior, {});
			else
				bitmask |= value;
		}

		return bitmask;
	}

	static vector GetPlacementPosition(vector pos)
	{
		vector hitPosition;
		PhxInteractionLayers layerMask;
		layerMask |= PhxInteractionLayers.BUILDING;
		layerMask |= PhxInteractionLayers.VEHICLE;
		layerMask |= PhxInteractionLayers.ROADWAY;
		layerMask |= PhxInteractionLayers.TERRAIN;
		layerMask |= PhxInteractionLayers.ITEM_LARGE;
		layerMask |= PhxInteractionLayers.FENCE;
		if (DayZPhysics.RayCastBullet(pos + "0 1.8 0", pos - "0 10 0", layerMask, null, null, hitPosition, null, null))
		{
			pos = hitPosition;
		}
		else
		{
			//! Make sure position is not under terrain
			float surfaceY = ExpansionStatic.GetSurfaceRoadY3D(pos[0], pos[1], pos[2], RoadSurfaceDetection.CLOSEST, UseObjectsMode.Wait);
			if (pos[1] < surfaceY)
				pos[1] = surfaceY;
		}

		float waterDepth = g_Game.GetWaterDepth(pos);
		if (waterDepth > 0)
			pos[1] = pos[1] + waterDepth;

		return pos;
	}

	TVectorArray GetWaypoints(vector position = vector.Zero, int beh = eAIWaypointBehavior.HALT)
	{
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

	void GenerateBaseName()
	{
		ExpansionSHA256.Update(Name);
		m_BaseName = ExpansionSHA256.HexDigest();
		ExpansionSHA256.Reset();
	}

	void Log(string msg)
	{
	}
};

class ExpansionAIDynamicSpawnBase: ExpansionAISpawnBase
{
	bool CanBeTriggeredByAI;
	float MinDistRadius;	            // If the player is closer than MinDistRadius from the spawn point, the patrol won't spawn, if set to -2, will use the general setting instead
	float MaxDistRadius;	            // Same but if the player is further away than MaxDistRadius, the bots won't spawn, if set to -2, will use the general setting instead
	float DespawnRadius;
	float MinSpreadRadius;
	float MaxSpreadRadius;
	float Chance;                       // chance for this patrol to spawn
	float DespawnTime;					// if all players outside despawn radius, ticks up time. When despawn time reached, patrol is deleted. If set to -1, will use general setting instead
	float RespawnTime;	                // Time in seconds before the dead group will respawn. If set to -1, they won't respawn, if set to -2, will use the general setting instead
	string LoadBalancingCategory;       // Category name to use for load balancing. Needs matching entry in AIPatrolSetttings::LoadBalancingCategories.
	                                    // If left empty, will use "ObjectPatrols" for object patrols, "Patrols" for normal patrols, or "Global".
	string ObjectClassName;  // Object for patrol to spawn on, needs to be registered via eAIRegisterDynamicPatrolSpawner (by default, only BuildingBase is registered). If set, Waypoints array (below) will be ignored and waypoints will be generated dynamically
	string WaypointInterpolation;
	bool UseRandomWaypointAsStartPoint;
	autoptr TVectorArray Waypoints;     // a list of positions to create a path to follow

	void ExpansionAIDynamicSpawnBase(int bod = 1, string spd = "JOG", string threatspd = "SPRINT", string beh = "ALTERNATE", string fac = "West", string loa = "", bool canbelooted = true, int unlimitedreload = 0, float chance = 1.0, float mindistradius = -1, float maxdistradius = -1)
	{
		Chance = chance;
		MinDistRadius = mindistradius;
		MaxDistRadius = maxdistradius;
		if (maxdistradius > 0)
			DespawnRadius = maxdistradius * 1.1;
		else
			DespawnRadius = -1;
		DespawnTime = -1;
		RespawnTime = -2;
	}

	//! TODO: Improve the spread code, this is very unoptimised
	void DefaultSpread()
	{
		if (Behaviour == "HALT")
		{
			if (ObjectClassName == "ContaminatedArea_Static" || ObjectClassName == "ContaminatedArea_Dynamic")
			{
				MinSpreadRadius = 0;
				MaxSpreadRadius = 50;
			}
			else
			{
				MinSpreadRadius = 5;
				MaxSpreadRadius = 10;
			}
		}
		else
		{
			if (ObjectClassName == "ContaminatedArea_Static" || ObjectClassName == "ContaminatedArea_Dynamic")
			{
				MinSpreadRadius = 0;
				MaxSpreadRadius = 150;
			}
			else
			{
				MinSpreadRadius = 5;
				MaxSpreadRadius = 20;
			}
		}
	}

	//! @note behavior param is deprecated and should not be used
	TVectorArray GenerateWaypoints(vector position = vector.Zero, int beh = eAIWaypointBehavior.HALT)
	{
		TVectorArray waypoints = {};
		vector waypoint;
		int amountofwaypoints;

		if (!MinSpreadRadius)
			MinSpreadRadius = 5;
		if (MaxSpreadRadius < MinSpreadRadius)
			MaxSpreadRadius = MinSpreadRadius + 5;

		if (ObjectClassName == "ContaminatedArea_Static" || ObjectClassName == "ContaminatedArea_Dynamic")
		{
			amountofwaypoints = Math.RandomIntInclusive(4, 8);

			for (int i = 0; i < amountofwaypoints; i++)
			{
				waypoint = ExpansionMath.GetRandomPointInRing(position, MinSpreadRadius, MaxSpreadRadius);
				waypoint = ExpansionStatic.GetSurfaceRoadPosition(waypoint, RoadSurfaceDetection.CLOSEST, UseObjectsMode.Wait);

				waypoints.Insert(waypoint);
			}
		}
		else
		{
			//! For wrecks, we let AI follow a roughly circular pattern around the wreck so they don't run into it

			int ccw = Math.RandomIntInclusive(0, 1);
			float angle = Math.RandomFloat(0, 360);
			float angleIncCumulative;

			//! This will generate anywhere from 8 to 10 waypoints
			while (true)
			{
				waypoint = ExpansionMath.GetRandomPointAtDegrees(position, angle, MinSpreadRadius, MaxSpreadRadius);
				waypoint = ExpansionStatic.GetSurfaceRoadPosition(waypoint, RoadSurfaceDetection.CLOSEST, UseObjectsMode.Wait);

				waypoints.Insert(waypoint);

				float angleInc = Math.RandomFloat(36, 45);

				angleIncCumulative += angleInc;
				if (angleIncCumulative > 360)
					break;

				if (ccw)
				{
					angle -= angleInc;
					if (angle < 0)
						angle += 360;
				}
				else
				{
					angle += angleInc;
					if (angle > 360)
						angle -= 360;
				}
			}
		}

		return waypoints;
	}

	override TVectorArray GetWaypoints(vector position = vector.Zero, int beh = eAIWaypointBehavior.HALT)
	{
		TVectorArray waypoints;

		if (ObjectClassName)
		{
			waypoints = GenerateWaypoints(position, beh);
		}
		else
		{
			waypoints = {};

			foreach (vector waypoint: Waypoints)
			{
				if (MaxSpreadRadius > 0)
					waypoint = ExpansionMath.GetRandomPointInRing(waypoint, MinSpreadRadius, MaxSpreadRadius);

				waypoints.Insert(GetPlacementPosition(waypoint));
			}
		}

		if (WaypointInterpolation)
		{
			int curveType;

			if (!ExpansionStatic.StringToEnumEx(ECurveType, WaypointInterpolation, curveType))
			{
				EXError.Error(null, string.Format("WaypointInterpolation \"%1\" is not one of the valid values, \"CatmullRom\", \"NaturalCubic\" or \"UniformCubic\"", WaypointInterpolation), {});
			}
			else
			{
				bool smooth;

				if (MaxSpreadRadius > 0)
					smooth = true;

				waypoints = ExpansionMath.PathInterpolated(waypoints, curveType, smooth);
			}
		}

		TVectorArray filteredPoints;

		if (ObjectClassName)
		{
			//! If generated waypoints, filter
			filteredPoints = {};

			foreach (vector candidate: waypoints)
			{
				//! Ignore waypoint if in high water (threshold of 1 m matches SHumanCommandMoveSettings.m_fWaterLevelSpeedRectrictionHigh)
				if (g_Game.GetWaterDepth(candidate) < 1.0)
					filteredPoints.Insert(candidate);
			}

			waypoints = filteredPoints;
		}

		return waypoints;
	}

	override void Log(string msg)
	{
		auto settings = GetExpansionSettings().GetLog();
		if (ObjectClassName)
		{
			if (settings.AIObjectPatrol)
				settings.PrintLog("[AI Object Patrol] %1", msg);
		}
		else if (settings.AIPatrol)
		{
			settings.PrintLog("[AI Patrol] %1", msg);
		}
	}
};
