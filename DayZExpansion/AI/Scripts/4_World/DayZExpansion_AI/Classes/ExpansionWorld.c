enum eAIProcessingState
{
	UNPROCESSED = 0,
	PROCESSED,
	REMOVE
};

modded class ExpansionWorld
{
	private ref eAIRoadNetwork m_Network;
	private bool m_NetworkGenerate;
	private vector m_NetworkPosition;
	private float m_NetworkRadius;

	void ExpansionWorld()
	{
		m_CommandManager = new eAICommandManagerClient();

		m_Network = new eAIRoadNetwork();
		//m_Network.Init();

		if (g_Game.IsDedicatedServer())
		{
			auto settings = GetExpansionSettings().GetAILocation();

			if (settings.RoamingLocations.Count() == 0)
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GenerateRoamingLocations, 10000);

			CheckCreateExampleAILootDrop();
		}
	}

	void GenerateRoamingLocations()
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
	#endif

		auto settings = GetExpansionSettings().GetAILocation();
		auto excludedAreas = settings.NoGoAreas;

		int worldSize = g_Game.GetWorld().GetWorldSize();
		vector min = Vector(0, -1000, 0);
		vector max = Vector(worldSize, 1000, worldSize);

		int amount = Math.Pow(worldSize / 1500.0, 2.0);

		array<EntityAI> candidates = {};
		DayZPlayerUtils.SceneGetEntitiesInBox(min, max, candidates, QueryFlags.STATIC | QueryFlags.ORIGIN_DISTANCE);

		EXPrint(this, "Found " + candidates.Count() + " static entities (buildings) on the map");

		int locIdx = settings.RoamingLocations.Count();
		EntityAI candidate;
		while (candidates.Count() > 0 && settings.RoamingLocations.Count() < amount)
		{
			int index = candidates.GetRandomIndex();
			candidate = candidates[index];
			candidates.Remove(index);

			if (!candidate.IsBuilding())
				continue;

			vector position = candidate.GetPosition();

			if (excludedAreas.IsPointInside(position))
				continue;

			string type = candidate.GetType();
			type.ToLower();
			if (ExpansionString.StartsWithAny(type, settings.ExcludedRoamingBuildings))
				continue;

			float radius = 100.0;
			string name = "Random_" + (locIdx + 1);
			string className = "Local_" + name;
			settings.RoamingLocations.Insert(new ExpansionAIRoamingLocation(position, radius, name, "Local", className, null, locIdx++));
		}

		if (settings.RoamingLocations.Count() > 0)
			settings.Save();

		EXPrint(this, "Selected " + settings.RoamingLocations.Count() + " random buildings as locations for AI roaming");
	}

/*
	override void OnUpdate(bool doSim, float timeslice)
	{
		super.OnUpdate(doSim, timeslice);

		if (m_NetworkGenerate)
		{
			m_NetworkGenerate = false;

			m_Network.NotifyGenerate(m_NetworkPosition, m_NetworkRadius);
		}
		
		if (g_Game.GetInput().LocalPress("UAFire"))
		{			
			vector pos = g_Game.GetCurrentCameraPosition();
			float radius = 500;
			
			m_Network.DS_Destroy();
			
			GenerateRoadNetwork(pos, radius);
		}
	}
*/

	void GenerateRoadNetwork(vector position, float radius)
	{
		m_NetworkGenerate = true;
		m_NetworkPosition = position;
		m_NetworkRadius = radius;
	}

	eAIRoadNetwork GetRoadNetwork()
	{
		return m_Network;
	}

	/*
	// List of all eAI entities
	private autoptr array<eAIProcessingState> m_AIStates = {};
	private autoptr array<ref eAIPlayerHandler> m_AI = {};

	private int m_ProcessingIndex = 0;
	private int m_MaxProcessingAI = 0;
	private float m_MinimumTime = 0.5; // 0.050; // 20hz
	private float m_ProcessingTime = 0;

	override Class AddAI(DayZPlayer entity)
	{
		eAIPlayerHandler handler = new eAIPlayerHandler(PlayerBase.Cast(entity));
		m_AI.Insert(handler); // insert the new handler to the back of the array
		return handler;
	}
	
	override void OnUpdate(bool doSim, float timeslice)
	{
		// don't process if we aren't the server
		if (!g_Game.IsServer()) return;

		m_ProcessingTime += timeslice;
		
		// we are back at the start
		if (m_ProcessingIndex == 0)
		{
			// we have processed all the AI within the minimum time, we can wait.
			if (m_ProcessingTime < m_MinimumTime)
			{
				return;
			}
			
			m_ProcessingTime = 0;

			if (m_AIStates.Count() == m_MaxProcessingAI)
			{
				// remove all AI that was marked for removal
				for (int i = m_AIStates.Count() - 1; i >= 0; i--)
				{
					switch (m_AIStates[i])
					{
						case eAIProcessingState.REMOVE:
						{
							m_AI.RemoveOrdered(i);
							break;
						}
					}
				}
			} else
			{
				Error("An unexpected desync with AI processing happened.");
			}

			// how many AI we can process for this next simulation step
			m_MaxProcessingAI = m_AI.Count();
			m_AIStates.Clear();

			// if we have 0 ai to process, wait 2 "ai" frames
			if (m_MaxProcessingAI == 0)
			{
				m_ProcessingTime = -m_MinimumTime;
				return;
			}
		}

		if (m_ProcessingIndex < m_MaxProcessingAI)
		{
			// this is the AI we will be processing this frame
			eAIPlayerHandler ai_Handler = m_AI[m_ProcessingIndex];
			m_ProcessingIndex++;

			// ai is null
			if (ai_Handler == null)
			{
				m_AIStates.Insert(eAIProcessingState.REMOVE);
				return;
			}

			// is dead
			if (ai_Handler.isDead())
			{
				m_AIStates.Insert(eAIProcessingState.REMOVE);
				return;
			}

			// update the AI
			ai_Handler.OnTick();

			// mark the AI as processed
			m_AIStates.Insert(eAIProcessingState.PROCESSED);
		} 
		else 
		{
			// reset the AI index if all are processed
			m_ProcessingIndex = 0; 
		}
	}
	*/

	override void FirearmEffects(Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal,
				  vector exitPos, vector inSpeed, vector outSpeed, bool isWater, bool deflected, string ammoType) 
	{
		super.FirearmEffects(source, directHit, componentIndex, surface, pos, surfNormal, exitPos, inSpeed, outSpeed, isWater, deflected, ammoType);

		if (g_Game.IsServer())
		{
			Weapon_Base weapon;
			if (Class.CastTo(weapon, source))
				weapon.eAI_FirearmEffects(directHit, componentIndex, surface, pos, surfNormal, exitPos, inSpeed, outSpeed, isWater, deflected, ammoType);
		}
	}
	/**
	 * @brief fix firearm FX for redirected AI dmg
	 */
	override void FixAIFirearmFX(inout Object source, inout Object directHit, inout int componentIndex, inout string surface, inout vector pos, inout vector surfNormal, inout vector exitPos, inout vector inSpeed, inout vector outSpeed, inout bool isWater, inout bool deflected, inout string ammoType)
	{
		if (!directHit)
			return;

		bool isClient = g_Game.IsClient();

		//! source is NULL on client
		if (isClient)
			source = m_FirearmFXSource;

		EntityAI entity;
		eAIBase ai;

		if (source && Class.CastTo(entity, source) && Class.CastTo(ai, entity.GetHierarchyRoot()))
		{
			Object hitObject;

			if (isClient)
				hitObject = m_FirearmFXHitObject;
			else
				hitObject = ai.m_eAI_HitObject;

			if (hitObject && directHit != hitObject)
			{
				if (directHit.IsDayZCreature() || directHit.IsItemBase() || directHit.IsMan() || directHit.IsTransport())
				{
					pos = directHit.WorldToModel(pos);
					exitPos = directHit.WorldToModel(exitPos);

					directHit = hitObject;

					pos = directHit.ModelToWorld(pos);
					exitPos = directHit.ModelToWorld(exitPos);
				}
			}
		}

		if (isClient)
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ResetFirearmFXSource, 3, false, source);
	}

	void ResetFirearmFXSource(Object source)
	{
		if (m_FirearmFXSource == source)
			m_FirearmFXSource = null;
	}

	void CheckCreateExampleAILootDrop()
	{
		if (FileExist(EXPANSION_AI_LOOTDROPS_FOLDER))
			return;

		ExpansionStatic.MakeDirectoryRecursive(EXPANSION_AI_LOOTDROPS_FOLDER);

		string path = EXPANSION_AI_LOOTDROPS_FOLDER + "Example.json";

		if (FileExist(path))
			return;

		auto defaultLootDrop = new array<ref ExpansionPrefab>;
		
		ExpansionPrefab prefab;

		prefab = new ExpansionPrefab();
		prefab.ClassName = "Ammo_545x39";
		prefab.Chance = 0.5;
		prefab.SetHealth(0.7, 1.0);
		prefab.SetQuantity(0.4, 0.8);
		defaultLootDrop.Insert(prefab);

		prefab = new ExpansionPrefab();
		prefab.ClassName = "Ammo_545x39Tracer";
		prefab.Chance = 0.5;
		prefab.SetHealth(0.7, 1.0);
		prefab.SetQuantity(0.4, 0.8);
		defaultLootDrop.Insert(prefab);

		prefab = new ExpansionPrefab();
		prefab.ClassName = "Ammo_762x39";
		prefab.Chance = 0.25;
		prefab.SetHealth(0.7, 1.0);
		prefab.SetQuantity(0.2, 0.6);
		defaultLootDrop.Insert(prefab);

		prefab = new ExpansionPrefab();
		prefab.ClassName = "Ammo_762x39Tracer";
		prefab.Chance = 0.25;
		prefab.SetHealth(0.7, 1.0);
		prefab.SetQuantity(0.2, 0.6);
		defaultLootDrop.Insert(prefab);

		prefab = new ExpansionPrefab();
		prefab.ClassName = "Ammo_556x45";
		prefab.Chance = 0.5;
		prefab.SetHealth(0.7, 1.0);
		prefab.SetQuantity(0.4, 0.8);
		defaultLootDrop.Insert(prefab);

		prefab = new ExpansionPrefab();
		prefab.ClassName = "Ammo_556x45Tracer";
		prefab.Chance = 0.5;
		prefab.SetHealth(0.7, 1.0);
		prefab.SetQuantity(0.4, 0.8);
		defaultLootDrop.Insert(prefab);

		prefab = new ExpansionPrefab();
		prefab.ClassName = "Ammo_762x54";
		prefab.Chance = 0.15;
		prefab.SetHealth(0.7, 1.0);
		prefab.SetQuantity(0.2, 0.6);
		defaultLootDrop.Insert(prefab);

		prefab = new ExpansionPrefab();
		prefab.ClassName = "Ammo_762x54Tracer";
		prefab.Chance = 0.15;
		prefab.SetHealth(0.7, 1.0);
		prefab.SetQuantity(0.2, 0.6);
		defaultLootDrop.Insert(prefab);

		prefab = new ExpansionPrefab();
		prefab.ClassName = "Ammo_308Win";
		prefab.Chance = 0.15;
		prefab.SetHealth(0.7, 1.0);
		prefab.SetQuantity(0.2, 0.6);
		defaultLootDrop.Insert(prefab);

		prefab = new ExpansionPrefab();
		prefab.ClassName = "Ammo_308WinTracer";
		prefab.Chance = 0.15;
		prefab.SetHealth(0.7, 1.0);
		prefab.SetQuantity(0.2, 0.6);
		defaultLootDrop.Insert(prefab);

		prefab = new ExpansionPrefab();
		prefab.ClassName = "FirstAidKit";
		prefab.Chance = 0.33;
		prefab.SetHealth(0.7, 1.0);
		ExpansionPrefabObject obj;
		obj = prefab.BeginCargo("BandageDressing");
			obj.Chance = 1.0;
			obj.SetHealth(0.7, 1.0);
			obj.End();
		obj = prefab.BeginCargo("BandageDressing");
			obj.Chance = 0.4;
			obj.SetHealth(0.7, 1.0);
			obj.End();
		obj = prefab.BeginCargo("IodineTincture");
			obj.Chance = 0.25;
			obj.SetHealth(0.7, 1.0);
			obj.SetQuantity(0.2, 1.0);
			obj.End();
		obj = prefab.BeginCargo("Morphine");
			obj.Chance = 0.25;
			obj.SetHealth(0.7, 1.0);
			obj.End();
		obj = prefab.BeginCargo("Epinephrine");
			obj.Chance = 0.25;
			obj.SetHealth(0.7, 1.0);
			obj.End();
		defaultLootDrop.Insert(prefab);

		string errorMsg;
		if (!JsonFileLoader<array<ref ExpansionPrefab>>.SaveFile(path, defaultLootDrop, errorMsg))
			EXError.Error(this, string.Format("Couldn't save '%1': %2", path, errorMsg), {});
	}
};
