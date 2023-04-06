/**
 * ExpansionNamalskModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

[CF_RegisterModule(ExpansionNamalskModule)]
class ExpansionNamalskModule: CF_ModuleWorld
{
	protected static ExpansionNamalskModule s_ModuleInstance;
	static string s_dataFolderPath = "$mission:expansion\\namalsk_adventure\\";

	protected ref ExpansionNamalskAdventureData m_ServerData; //! Server
#ifdef EXPANSIONMODNAVIGATION
	protected ExpansionMarkerData m_MerchantServerMarker;
#endif

	protected const int MAX_ANOMALY_SPAWN_TRIES = 100;
	protected const int ANOMALY_SPAWN_DISTANCE = 100;

	protected ref array<vector> m_AnomalyPositions;
	protected bool m_DynamicSpawned;
	protected bool m_StaticSpawned;

#ifdef EXPANSIONMODAI
	protected ref map<eAIBase, ref array<vector>> m_SpawnedAI;
#endif
#ifdef EXPANSIONMODQUESTS
	protected ref map<eAIBase, ref array<vector>> m_QuestAI;
#endif
	protected bool m_AIBuildingPositionsSet;

	protected typename m_LastNamalskEvent;
	protected ref array<vector> m_BuildingPositions;

	void ExpansionNamalskModule()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		s_ModuleInstance = this;
		m_AnomalyPositions = new array<vector>;
		m_BuildingPositions = new array<vector>;

	#ifdef EXPANSIONMODAI
		m_SpawnedAI = new map<eAIBase, ref array<vector>>;
	#endif
	#ifdef EXPANSIONMODQUESTS
		m_QuestAI = new map<eAIBase, ref array<vector>>;
	#endif
	}

	override void OnInit()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.OnInit();

		EnableMissionStart();
		EnableMissionLoaded();
	}

	protected void CreateDirectoryStructure()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!FileExist(s_dataFolderPath))
			ExpansionStatic.MakeDirectoryRecursive(s_dataFolderPath);
	}

#ifdef EXPANSIONMODMARKET
	override void OnMissionStart(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.OnMissionLoaded(sender, args);

		//! Server only
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			CreateDirectoryStructure(); //! Create directoy structure if not existing.
			LoadNamalskAdventureServerData(); //! Load server data.
		}
	}
#endif

	protected void LoadNamalskAdventureServerData()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		array<string> files = ExpansionStatic.FindFilesInLocation(s_dataFolderPath, ".json");
		if (files && files.Count() > 0)
		{
			foreach (string fileName: files)
			{
				GetServerData(fileName, s_dataFolderPath);
			}
		}
		else
		{
			m_ServerData = new ExpansionNamalskAdventureData();
			m_ServerData.Save();
		}
	}

	protected void GetServerData(string fileName, string path)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_ServerData = ExpansionNamalskAdventureData.Load(path + fileName);
		if (!m_ServerData)
			Error(ToString() + "::GetServerData - Could not get namalsk adventure server data!");
	}

	override void OnMissionLoaded(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.OnMissionLoaded(sender, args);

		//! Server only
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			if (GetExpansionSettings().GetNamalskAdventure().EnableAnomalies)
				SpawnAnomalies();

		#ifdef EXPANSIONMODAI
			if (GetExpansionSettings().GetNamalskAdventure().EnableAISpawns)
				SpawnAI();
		#endif
		#ifdef EXPANSIONMODMARKET
			if (GetExpansionSettings().GetNamalskAdventure().EnableMerchant)
				CreateMerchant();
		#endif
		}
	}

	//! @note: Spawns all configured anomalies from the NamalskAdventureSettings class.
	protected void SpawnAnomalies(bool isDynamicEvent = false)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (GetExpansionSettings().GetNamalskAdventure().EnableDynamic)
		{
			if (!m_DynamicSpawned && (!isDynamicEvent && !GetExpansionSettings().GetNamalskAdventure().SpawnDynamicWithEVRStorms || isDynamicEvent && GetExpansionSettings().GetNamalskAdventure().SpawnDynamicWithEVRStorms))
			{
				array<ref ExpansionAnomalyDynamic> dynamicSpawns = GetExpansionSettings().GetNamalskAdventure().DynamicAnomalies;
				for (int i = 0; i < dynamicSpawns.Count(); i++)
				{
					ExpansionAnomalyDynamic dynamicAnoamlyData = dynamicSpawns[i];
					if (!dynamicAnoamlyData)
						continue;

					SpawnAnomaliesDynamic(dynamicAnoamlyData, isDynamicEvent);
				}

				m_DynamicSpawned = true;
			}
		}

		if (GetExpansionSettings().GetNamalskAdventure().EnableStatic)
		{
			if (!m_StaticSpawned && (!isDynamicEvent && !GetExpansionSettings().GetNamalskAdventure().SpawnStaticWithEVRStorms || isDynamicEvent && GetExpansionSettings().GetNamalskAdventure().SpawnStaticWithEVRStorms))
			{
				array<ref ExpansionAnomalyStatic> staticSpawns = GetExpansionSettings().GetNamalskAdventure().StaticAnomalies;
				for (int j = 0; j < staticSpawns.Count(); j++)
				{
					ExpansionAnomalyStatic staticAnoamlyData = staticSpawns[j];
					if (!staticAnoamlyData)
						continue;

					SpawnAnomalieStatic(staticAnoamlyData, isDynamicEvent);
				}

				m_StaticSpawned = true;
			}
		}
	}

#ifdef EXPANSIONMODAI
	//! @note: Spawns all configured AI units from the NamalskAdventureSettings class.
	protected void SpawnAI()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		foreach (ExpansionAISpawnPosition aiSpawn: GetExpansionSettings().GetNamalskAdventure().AISpawnPositions)
		{
			SpawnSingleAI(aiSpawn);
		}
	}

	//! @note: Handles spawns of AI units.
	protected void SpawnSingleAI(ExpansionAISpawnPosition aiSpawn)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		vector pos = ExpansionAIPatrol.GetPlacementPosition(aiSpawn.Position);

		eAIBase ai;
		if (!Class.CastTo(ai, GetGame().CreateObject(GetRandomAI(), pos)))
			return;

		if (!ai.m_Expansion_NetsyncData)
			ai.m_Expansion_NetsyncData = new ExpansionNetsyncData(ai);

		if (ai.m_Expansion_NetsyncData)
		{
			ai.m_Expansion_NetsyncData.Set(0, aiSpawn.NPCName);
			ai.m_Expansion_NetsyncData.Set(1, "{3364F58EF7F7FBE3}DayZExpansion/Core/GUI/icons/misc/T_Soldier_256x256.edds");
		}

		ai.SetPosition(pos);
		ai.SetOrientation(aiSpawn.Orientation);
		ExpansionHumanLoadout.Apply(ai, aiSpawn.Loadout, false);
		ai.SetMovementSpeedLimits(aiSpawn.MinSpeed, aiSpawn.MaxSpeed);
		ai.Expansion_SetCanBeLooted(aiSpawn.CanBeLooted);
		ai.eAI_SetUnlimitedReload(aiSpawn.UnlimtedReload);
		ai.eAI_SetAccuracy(aiSpawn.MinAccuracy, aiSpawn.MaxAccuracy);
		ai.eAI_SetThreatDistanceLimit(aiSpawn.ThreatDistanceLimit);
		ai.eAI_SetDamageMultiplier(aiSpawn.DamageMultiplier);

		eAIGroup aiGroup;
		eAIFaction faction = eAIFaction.Create(aiSpawn.Faction);
		if (!Class.CastTo(aiGroup, ai.GetGroup()))
		{
			if (!Class.CastTo(aiGroup, eAIGroup.CreateGroup(faction)))
				return;

			ai.SetGroup(aiGroup);
		}

		aiGroup.SetFaction(faction);
		aiGroup.SetFormation(eAIFormation.Create("Column"));
		aiGroup.SetWaypointBehaviour(eAIWaypointBehavior.ALTERNATE);

		for (int idx = 0; idx < aiSpawn.Waypoints.Count(); idx++)
		{
			aiGroup.AddWaypoint(aiSpawn.Waypoints[idx]);
			if (aiSpawn.Waypoints[idx] == pos)
				aiGroup.m_CurrentWaypointIndex = idx;
		}

		array<vector> positions = new array<vector>;
		positions = aiGroup.GetWaypoints();
		m_SpawnedAI.Insert(ai, positions);
	}
#endif

#ifdef EXPANSIONMODMARKET
	//! @note: Handles spawn of the dynamic merchant
	protected void CreateMerchant()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		ExpansionMarketSettings marketSettings = GetExpansionSettings().GetMarket();
		int findIndex = -1;

		array<int> usedMerchantPositions = m_ServerData.GetUsedMerchantPositions();
		array<int> usedMerchantSets = m_ServerData.GetUsedMerchantItemSets();

		ExpansionMerchantPosition positionToUse = GetExpansionSettings().GetNamalskAdventure().GetMerchantPosition(); //! Get a random position for the merchant to spawn.
		int posIDToUse = positionToUse.ID;

		//! Check if the random seleted position already got used in the previous sessions.
		findIndex = -1;
		findIndex = usedMerchantPositions.Find(posIDToUse);

		//! If the position got used we want to check for a valid position to use and if we already used all positions
		//! we reset the server data that holds that information and get a random position again.
		if (findIndex > -1)
		{
			ModuleDebugPrint("::CreateMerchant - Position with ID " + posIDToUse + " got already used in previous sessions. Check if we have still unused positions..");
			positionToUse = null;
			foreach (ExpansionMerchantPosition pos: GetExpansionSettings().GetNamalskAdventure().MerchantPositions)
			{
				int posID = pos.ID;
				ModuleDebugPrint("::CreateMerchant - Check if position with ID " + posID + " got used yet..");
				findIndex = -1;
				findIndex = usedMerchantPositions.Find(posID);
				if (findIndex == -1)
				{
					ModuleDebugPrint("::CreateMerchant - Position with ID " + posID + " got not used yet! Using position..");
					positionToUse = pos;
					break;
				}
				else
				{
					ModuleDebugPrint("::CreateMerchant - Position with ID " + posID + " got already used! Check next..");
				}
			}

			//! If we found no position to use we need to reset the server data vector array of the last used merchant positions.
			if (!positionToUse)
			{
				ModuleDebugPrint("::CreateMerchant - No unused positions left! Reset used positons..");
				m_ServerData.ResetUsedPositions();
				positionToUse = GetExpansionSettings().GetNamalskAdventure().GetMerchantPosition(); //! Get a random position for the merchant to spawn.
			}
		}

		if (!positionToUse)
		{
			Error(ToString() + "::CreateMerchant - Could not get a valid position to spawn merchant!");
			return;
		}

		m_ServerData.GetUsedMerchantPositions().Insert(positionToUse.ID);

		ExpansionMerchantItemSet itemSetToUse = GetExpansionSettings().GetNamalskAdventure().GetMerchantItemSet(); //! Get a random set of items that the merchant will display.
		int itemSetIDToUse = itemSetToUse.ID;

		//! Check if the random seleted set already got used in the previous sessions.
		findIndex = -1;
		findIndex = usedMerchantSets.Find(itemSetIDToUse);

		//! If the set got used we want to check for a valid set to use and if we already used all sets
		//! we reset the server data that holds that information and get a random set again.
		if (findIndex > -1)
		{
			ModuleDebugPrint("::CreateMerchant - Item set with ID " + itemSetIDToUse + " got already used in previous sessions. Check if we have still unused item sets..");
			itemSetToUse = null;
			foreach (ExpansionMerchantItemSet itemSet: GetExpansionSettings().GetNamalskAdventure().MerchantItemSets)
			{
				int setID = itemSet.ID;
				ModuleDebugPrint("::CreateMerchant - Check if item set with ID " + setID + " got used yet..");
				findIndex = -1;
				findIndex = usedMerchantSets.Find(setID);
				if (findIndex == -1)
				{
					ModuleDebugPrint("::CreateMerchant - Item set with ID " + setID + " got not used yet! Using Item set..");
					itemSetToUse = itemSet;
					break;
				}
				else
				{
					Print(ToString() + "::CreateMerchant - Item set with ID " + setID + " got already used! Check next..");
				}
			}

			//! If we found no set to use we need to reset the server data int array of the last used merchant item sets.
			if (!itemSetToUse)
			{
				ModuleDebugPrint("::CreateMerchant - No unused item sets left! Reset used item sets..");
				m_ServerData.ResetUsedItemSets();
				itemSetToUse = GetExpansionSettings().GetNamalskAdventure().GetMerchantItemSet(); //! Get a random set of items that the merchant will display.
			}
		}

		if (!itemSetToUse)
		{
			Error(ToString() + "::CreateMerchant - Could not get a valid item set for merchant!");
			return;
		}

		m_ServerData.GetUsedMerchantItemSets().Insert(itemSetToUse.ID);
		m_ServerData.Save();

		ExpansionMarketTraderZone traderZone = new ExpansionMarketTraderZone();
		traderZone.m_DisplayName = "Merchant";
		traderZone.Position = positionToUse.Position;
		traderZone.Radius = 10.0;
		traderZone.BuyPricePercent = 100.0;
		traderZone.SellPricePercent = -1.0;

		ExpansionMarketTrader trader = new ExpansionMarketTrader();
		trader.DisplayName = "Merchant";
	#ifdef EXPANSIONMODHARDLINE
		trader.MinRequiredReputation = 0;
		trader.MaxRequiredReputation = 2147483647;
	#endif
		trader.TraderIcon = "Deliver";
		trader.Currencies.Insert("expansionbanknotehryvnia");

		string className;
		int amount;
		ExpansionMarketTraderBuySell buySell;

		array<ref ExpansionMerchantItem> items = itemSetToUse.Items;
		foreach (ExpansionMerchantItem item: items)
		{
			className = item.ClassName;
			amount = item.Amount;
			buySell = item.BuySell;

			traderZone.AddStock(className, amount);
			trader.AddItem(className, buySell);
		}

		marketSettings.AddMarketZone(traderZone);
		marketSettings.AddMarketTrader(trader);

		Object obj = GetGame().CreateObject("ExpansionTraderAIMirek", positionToUse.Position);
		ExpansionTraderAIBase aiTrader = ExpansionTraderAIBase.Cast(obj);
		if (!aiTrader)
		{
			Error(ToString() + "::CreateMerchant - Could not spawn merchant object!");
			GetGame().ObjectDelete(obj);
			return;
		}

		ExpansionTraderObjectBase traderBase = new ExpansionTraderObjectBase(aiTrader);
		if (!traderBase)
		{
			Error(ToString() + "::CreateMerchant - Could not spawn merchant trader base object!");
			GetGame().ObjectDelete(obj);
			return;
		}

		traderBase.SetTraderZone(traderZone);
		traderBase.SetTraderMarket(trader);
		aiTrader.SetTraderObject(traderBase);

		aiTrader.m_Expansion_NetsyncData.Set(0, "Merchant");
		aiTrader.m_Expansion_NetsyncData.Set(1, "{5F2743E5F6F4DF0D}DayZExpansion/Core/GUI/icons/misc/coinstack2_64x64.edds");

		ExpansionHumanLoadout.Apply(aiTrader, "SurvivorLoadout", false);
		aiTrader.SetPosition(positionToUse.Position);
		aiTrader.SetOrientation(positionToUse.Orientation);

		traderBase.UpdateTraderZone();

	#ifdef EXPANSIONMODNAVIGATION
		m_MerchantServerMarker = ExpansionMarkerModule.GetModuleInstance().CreateServerMarker("Merchant", "Coins 2", positionToUse.Position, ARGB(255, 15, 185, 177), false);
	#endif
	}
#endif

	//! @note: Handles spawns of static anomaly spawns
	protected void SpawnAnomalieStatic(ExpansionAnomalyStatic anomaly, bool isDynamicEvent = false)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		string typeToSpawn = anomaly.AnomalyTypes.GetRandomElement();
	    Object entity = GetGame().CreateObjectEx(typeToSpawn, anomaly.CenterPosition, ECE_PLACE_ON_SURFACE, ECE_NOLIFETIME);
	    Expansion_Anomaly_Base anomalyObj = Expansion_Anomaly_Base.Cast(entity);
		if (!anomalyObj)
        {
           GetGame().ObjectDelete(entity);
           return;
        }

		//! @note: We increase the survived EVR storms count for the static spawned anomalies on spawn so the OnEVRStormFinalBlowout method
		//! takes these anomalies into account for the anomaly implosion event gamble on the next EVR storm event that hits the server.
		anomalyObj.IncreaseEVRStormsCount();

		if (anomaly.Loot && anomaly.Loot.Count() > 0)
		{
			ModuleDebugPrint("::SpawnAnomalieStatic - Add loot configuration from settings to anomaly. Loot amount: " + anomaly.Loot.Count());
			anomalyObj.SetLoot(anomaly.Loot, anomaly.LootItemsMin, anomaly.LootItemsMax, anomaly.LootSpawnType);
			if (!isDynamicEvent && anomaly.LootSpawnType == ExpansionAnomalyLootSpawnType.STATIC && !anomalyObj.IsLootSpawned())
				anomalyObj.SpawnLoot();
		}

        m_AnomalyPositions.Insert(anomaly.CenterPosition);

		ModuleDebugPrint("::SpawnAnomalieStatic - Spawned anomaly: " + typeToSpawn + " | Position: " + anomaly.CenterPosition);
	}

	//! @note: Handles spawns of dynamic anomaly spawns
	protected void SpawnAnomaliesDynamic(ExpansionAnomalyDynamic anomaly, bool isDynamicEvent = false)
	{
	    auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

	    int amountToSpawn = anomaly.Amount;
		float squareSize = anomaly.SquareSize;
	    int amountSpawned = 0;

		 ModuleDebugPrint("::SpawnAnomaliesDynamic - Generate random positions for " + amountToSpawn + " anomalies.");

		//! Generate positions within a square area
		array<vector> positions = GeneratePositions(anomaly.CenterPosition, squareSize, anomaly.Amount, ANOMALY_SPAWN_DISTANCE);
		ModuleDebugPrint("::SpawnAnomaliesDynamic - Generated " + positions.Count() + " random positions.");

		for (int i = 0; i < positions.Count(); i++)
		{
			if (amountSpawned >= amountToSpawn)
				return;

			ModuleDebugPrint("----------------------------------");
	        string typeToSpawn = anomaly.AnomalyTypes.GetRandomElement();
			int flags = ECE_PLACE_ON_SURFACE;
			if (anomaly.Persistance < ExpansionAnomalyPersistance.LIFETIME)
				flags = ECE_NOLIFETIME;

	        Object entity = GetGame().CreateObjectEx(typeToSpawn, positions[i], flags);
	        Expansion_Anomaly_Base anomalyObj = Expansion_Anomaly_Base.Cast(entity);
	        if (!anomalyObj)
	        {
	            GetGame().ObjectDelete(entity);
	            break;
	        }

			if (anomaly.Loot && anomaly.Loot.Count() > 0)
			{
				ModuleDebugPrint("::SpawnAnomaliesDynamic - Add loot configuration from settings to anomaly. Loot amount: " + anomaly.Loot.Count());
				anomalyObj.SetLoot(anomaly.Loot, anomaly.LootItemsMin, anomaly.LootItemsMax, anomaly.LootSpawnType);
				if (!isDynamicEvent && anomaly.LootSpawnType == ExpansionAnomalyLootSpawnType.STATIC && !anomalyObj.IsLootSpawned())
					anomalyObj.SpawnLoot();
			}

			//! @note: We increase the survived EVR storms count for the dynamic spawned anomalies on spawn so the OnEVRStormFinalBlowout method
			//! takes these anomalies into account for the anomaly implosion event gamble on the next EVR storm event that hits the server.
			anomalyObj.IncreaseEVRStormsCount();

	        m_AnomalyPositions.Insert(positions[i]);

			amountSpawned++;

	        ModuleDebugPrint("::SpawnAnomaliesDynamic - Spawned anomaly: " + "[" + amountSpawned + "] " +  typeToSpawn + " | Position: " + positions[i].ToString());
		}
	}

	void OnNamalskEventStart(typename eventType)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

	#ifdef DIAG
		ExpansionNotification(new StringLocaliser("NAMALSK EVENT STARTED"), new StringLocaliser("%1 EVENT STARTED", eventType.ToString()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_INFO, 7, ExpansionNotificationType.TOAST).Create();
	#endif

		if (eventType == EVRStorm || eventType == EVRStormDeadly)
		{
			Expansion_Anomaly_Base.EVRStormStart();
			
			Expansion_AnomalyCore_Base.EVRStormStart();
		}
	}

	void OnNamalskEventCancel(typename eventType)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

	#ifdef DIAG
		ExpansionNotification(new StringLocaliser("NAMALSK EVENT CANCELED"), new StringLocaliser("%1 EVENT CANCELED", eventType.ToString()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_INFO, 7, ExpansionNotificationType.TOAST).Create();
	#endif

		if (eventType == EVRStorm || eventType == EVRStormDeadly || m_LastNamalskEvent == EVRStorm || m_LastNamalskEvent == EVRStormDeadly)
			StabilizeAnomalies();

		m_LastNamalskEvent = eventType;
	}

	//! @note: Handles events that should start when ever a EVR storm event starts.
	void OnEVRStormInitPhaseServer()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	}

	//! @note: Handles events that should start when ever a EVR storm mid phase starts.
	void OnEVRStormMidPhaseServer()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		SetAIBuildingPositions();
	}
	
	//! @note: Handles events that should start when ever the EVR storm final blowout starts.
	//! Modified version to process and loop thrue spawned anomaly entities in chuncks.
	void OnEVRStormFinalBlowout()
	{
	    auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

	    if (GetGame().IsServer())
	    {
	        Expansion_Anomaly_Base.EVRStormFinalBlowout();

	        Expansion_AnomalyCore_Base.EVRStormFinalBlowout();
		}
	}

	//! @note: Handles events that should start when ever a EVR storm event end phase starts.
	void OnEVRStormEndPhaseServer()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!m_DynamicSpawned)
		{
			SpawnAnomalies(true);
			Sleep(5000);
		}

		ResetAIPositions();

		StabilizeAnomalies();

		NamEventManager event_manager;
	    g_Script.CallFunction(GetGame().GetMission(), "GetNamEventManager", event_manager, null);
		m_LastNamalskEvent = event_manager.GetLastEventType();
	}

	//! @note: Handles events that should start when ever a EVR storm event ended.
	//! Gets called on every active player on the server.
	void OnEVRStormFinished(SurvivorBase player)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		//! PLACEHOLDER
	}

	protected void SetAIBuildingPositions()
	{
	    auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

	    const int CHUNK_SIZE = 10; //! @note: Modifiy if needed
		int endIndex;

	    int numAI = m_SpawnedAI.Count();
	    for (int i = 0; i < numAI; i += CHUNK_SIZE)
	    {
	        endIndex = Math.Min(i + CHUNK_SIZE, numAI);
	        for (int j = i; j < endIndex; j++)
	        {
	            eAIBase ai = m_SpawnedAI.GetKey(j);
			 	GetAIBuildingWaypoint(ai);
	        }
	    }

	    m_QuestAI.Clear();

		auto node = ExpansionQuestNPCAIBase.s_Expansion_AllNPCs.m_Head;
	    while (node)
	    {
	       	ExpansionQuestNPCAIBase questNPC = node.m_Value;
			ModuleDebugPrint("::SetAIBuildingPositions - Quest NPC: " + questNPC.ToString());
			
			if (m_QuestAI.Contains(questNPC))
			{
				node = node.m_Next;
				continue;
			}
			
			eAIGroup questNPCGroup = questNPC.GetGroup();
			if (!questNPCGroup)
			{
				node = node.m_Next;
				continue;
			}

			array<vector> npcPositions = new array<vector>;
			npcPositions = questNPCGroup.GetWaypoints();
			m_QuestAI.Insert(questNPC, npcPositions);
			
			GetAIBuildingWaypoint(questNPC);
			
			node = node.m_Next;
	    }
	}
	
	bool IsEntityOnInteriorSurface(notnull EntityAI entity_ai)
	{
		string surface_type;
		vector pos = entity_ai.GetPosition();
		GetGame().SurfaceGetType3D(pos[0], pos[1] + 1.0, pos[2], surface_type);
		return ((GetGame().ConfigGetInt("CfgSurfaces " + surface_type + " interior")) == 1);
	}

	//! @note: Handles reset of spawned AI units and active quest AI NPCs waypoints after a EVR strom event.
	protected void ResetAIPositions()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		for (int i = 0; i < m_SpawnedAI.Count(); i++)
		{
			eAIBase npcAI = m_SpawnedAI.GetKey(i);
			ModuleDebugPrint("::ResetAIPositions - AI: " + npcAI.GetType());
			//ResetSpawnedAIWaypoints(npcAI);
			
			eAIGroup aiGroup = npcAI.GetGroup();
			if (!aiGroup)
				continue;
			
			array<vector> aiPositions = new array<vector>;
			aiPositions = aiGroup.GetWaypoints();
			if (aiGroup.m_CurrentWaypointIndex > 0)
				aiGroup.m_CurrentWaypointIndex = 0;
		}
		
		auto node = ExpansionQuestNPCAIBase.s_Expansion_AllNPCs.m_Head;
	    while (node)
	    {
			ExpansionQuestNPCAIBase questNPC = node.m_Value;
			//ResetQuestAIWaypoints(questAI);
			
			eAIGroup questNPCGroup = questNPC.GetGroup();
			if (!questNPCGroup)
			{
				node = node.m_Next;
				continue;
			}
			
			array<vector> npcPositions = new array<vector>;
			npcPositions = questNPCGroup.GetWaypoints();
			if (questNPCGroup.m_CurrentWaypointIndex > 0)
				questNPCGroup.m_CurrentWaypointIndex = 0;
			
			node = node.m_Next;
		}
	}

	protected void StabilizeAnomalies()
	{
	    auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

	    Expansion_Anomaly_Base.StabilizeAnomalies();
	}

	protected void GetAIBuildingWaypoint(eAIBase ai)
	{
	    auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	
	    if (!ai.IsAlive())
	        return;
	
	    eAIGroup aiGroup = ai.GetGroup();
	    if (!aiGroup)
	        return;
	
	    vector pos = ai.GetPosition();
	    float radius = 50; //! Replace with your desired radius
	    array<Object> objects = new array<Object>;
	    array<string> buildingTypes = {"House", "BuildingWithFireplace"}; //! Add more types as desired
		
		//! Get nearby objects
	    GetGame().GetObjectsAtPosition(pos, radius, objects, null);

	    //! Get positions within buildings
	    foreach (Object obj : objects)
	    {
	        if (ExpansionStatic.IsAnyOf(obj.GetType(), buildingTypes))
	        {
	            vector objPos = obj.GetPosition();
	            float surfacePos = GetGame().SurfaceY(objPos[0], objPos[2]);
	            objPos[1] = surfacePos;
	            m_BuildingPositions.Insert(objPos);
	        }
	    }
	
	    ModuleDebugPrint("::GetAIBuildingWaypoint - Found " + m_BuildingPositions.Count() + " buildings within " + radius + " meters of " + pos);

		//! Select a random building position
	    vector buildingPos = m_BuildingPositions.GetRandomElement();
		
		//! Generate a random position from the selected building position.
		array<vector> randomPositions = GeneratePositions(buildingPos, 2.0, 1);
		ModuleDebugPrint("::GetAIBuildingWaypoint - Selected building position " + buildingPos.ToString() + ".");
		if (randomPositions.Count() == 0)
		{
			Error(ToString() + "::GetAIBuildingWaypoint - Could not generate any random building position for AI " + ai.ToString() + " for building position " + buildingPos.ToString());
			return;
		}
		
	    aiGroup.ClearWaypoints();
	    aiGroup.AddWaypoint(randomPositions[0]);
		aiGroup.m_CurrentWaypointIndex = 0;
	}

	protected void ResetSpawnedAIWaypoints(eAIBase ai)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ModuleDebugPrint("::ResetSpawnedAIWaypoints - AI: " + ai.ToString());

		eAIGroup aiGroup = ai.GetGroup();
		if (!aiGroup)
		{
			Error("::ResetSpawnedAIWaypoints - AI has not Group!");
			return;
		}

		ModuleDebugPrint("::ResetSpawnedAIWaypoints - AI group: " + aiGroup.ToString());
		aiGroup.ClearWaypoints();

		array<vector> waypoints = m_SpawnedAI.Get(ai);
		ModuleDebugPrint("::ResetSpawnedAIWaypoints - Waypoints: " + waypoints.ToString());
		foreach (vector pos: waypoints)
		{
			ModuleDebugPrint("::ResetSpawnedAIWaypoints - Adding back old waypoint: " + waypoints.Count());
			aiGroup.AddWaypoint(pos);
		}
		
		aiGroup.m_CurrentWaypointIndex = 0;
	}

	protected void ResetQuestAIWaypoints(eAIBase ai)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ModuleDebugPrint("::ResetQuestAIWaypoints - AI: " + ai.ToString());
		
		eAIGroup aiGroup = ai.GetGroup();
		if (!aiGroup)
		{
			Error("::ResetQuestAIWaypoints - AI has not Group!");
			return;
		}

		ModuleDebugPrint("::ResetQuestAIWaypoints - AI group: " + aiGroup.ToString());
		
		aiGroup.ClearWaypoints();

		array<vector> waypoints = m_QuestAI.Get(ai);
		ModuleDebugPrint("::ResetQuestAIWaypoints - Waypoints: " + waypoints.ToString());
		foreach (vector pos: waypoints)
		{
			ModuleDebugPrint("::ResetQuestAIWaypoints - Adding back old waypoint: " + waypoints.Count());
			aiGroup.AddWaypoint(pos);
		}
		
		aiGroup.m_CurrentWaypointIndex = 0;
	}

#ifdef EXPANSIONMODQUESTS
	void AfterQuestModuleClientInit(ExpansionQuestPersistentData playerQuestData, PlayerIdentity identity)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		/*
		//! Server only
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::AfterQuestModuleClientInit - Tryed to call AfterQuestModuleClientInit on Client!");
			return;
		}

		if (!playerQuestData)
		{
			Error(ToString() + "::AfterQuestModuleClientInit - Could not get player quest data for player with UID: " + identity.GetId());
			return;
		}

		ExpansionQuestState questState = playerQuestData.GetQuestStateByQuestID(1);
		ModuleDebugPrint("::AfterQuestModuleClientInit - Main quest quest-state: " + questState);
		ModuleDebugPrint("::AfterQuestModuleClientInit - Main quest quest-state name: " + typename.EnumToString(ExpansionQuestState, questState));

		if (questState == ExpansionQuestState.NONE)
		{
			//! Show the first quest to the player
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ShowMainQuest, 3000, false, identity);
		}
		*/
	}

	protected void ShowMainQuest(PlayerIdentity identity)
	{
		ExpansionQuestModule.GetModuleInstance().RequestOpenQuestMenu(identity, 1);
	}
#endif

	//! @note: Condition check if a EVR storm is currently active.
	bool IsEVRStormActive()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

	#ifdef NAMALSK_SURVIVAL
	    NamEventManager event_manager;
	    g_Script.CallFunction(GetGame().GetMission(), "GetNamEventManager", event_manager, null);

	    if (!event_manager)
	        return false;

		EVRStorm storm = EVRStorm.Cast(event_manager.GetEvent(EVRStorm));
		EVRStormDeadly stormDeadly = EVRStormDeadly.Cast(event_manager.GetEvent(EVRStormDeadly));
		if (storm && storm.GetEVRStormPhase() > ExpansionEVRStormPhase.NONE || stormDeadly && stormDeadly.GetEVRStormPhase() > ExpansionEVRStormPhase.NONE)
	   		return true;
	#endif

		return false;
	}
	
	bool HasActiveLEHSSuit(PlayerBase player)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		int slot_id_lehsSuit = InventorySlots.GetSlotIdFromString("LEHS");
		dzn_lehs lehsSuit = dzn_lehs.Cast(player.GetInventory().FindAttachment(slot_id_lehsSuit));
		if (!lehsSuit || lehsSuit && lehsSuit.IsRuined())
		{
			ModuleDebugPrint("::OnEnterAnomalyServer - Return FALSE");
			return false;
		}

		ModuleDebugPrint("::OnEnterAnomalyServer - Player has LEHS suit: " + lehsSuit.ToString());

		int slot_id_Headgear = InventorySlots.GetSlotIdFromString("Headgear");
		dzn_lehs_helmet lehsHelmet = dzn_lehs_helmet.Cast(player.GetInventory().FindAttachment(slot_id_Headgear));
		if (!lehsHelmet || lehsHelmet && lehsHelmet.IsRuined())
		{
			ModuleDebugPrint("::OnEnterAnomalyServer - Return FALSE");
			return false;
		}

		ModuleDebugPrint("::OnEnterAnomalyServer - Player has LEHS helmet: " + lehsHelmet.ToString());
		ModuleDebugPrint("::OnEnterAnomalyServer - LEHS status: Visor up: " + lehsHelmet.IsVisorUp() + " | Has power: " + lehsHelmet.HasPower() + " | Pressurized: " + lehsHelmet.IsPressurized());

		if (lehsHelmet.IsVisorUp() || !lehsHelmet.HasPower() || !lehsHelmet.IsPressurized() || !lehsHelmet.HasCircuitBoard())
		{
			ModuleDebugPrint("::OnEnterAnomalyServer - Return FALSE");
			return false;
		}


		ModuleDebugPrint("::OnEnterAnomalyServer - Return TRUE");
		return true;
	}
	
	void ProcessCargoDamage(EntityAI parent, int minDmg, int maxDmg)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ModuleDebugPrint("::ProcessCargoDamage - Entity: " + parent.GetType() + " | Entity parent: " + parent.GetParent().ToString());

		array<EntityAI> items = new array<EntityAI>;
		parent.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

		foreach (EntityAI item: items)
		{
			ModuleDebugPrint("::ProcessCargoDamage - Item: " + item.GetType() + " | Item parent: " + item.GetParent().ToString());
			if (item.IsInherited(SurvivorBase) || item.IsInherited(ExpansionAnomalyCoreProtectiveCase))
				continue;

			//! @note: Don't damage items in anomaly/evr protector cases.
			if (item.GetParent() && ExpansionAnomalyCoreProtectiveCase.Cast(item.GetParent()))
				continue;

			item.AddHealth("", "", Math.RandomFloatInclusive(minDmg, maxDmg));
		}
	}

	//! @note: In this modified version of the GeneratePositions method, a larger square area is generated using a for loop with a fixed number of iterations equal to amount * 2.
	//! Valid positions within the larger area are then selected randomly until the desired number of positions is reached or there are no more valid positions left.
	//! This reduces the number of collision checks and distance calculations required and should improve server performance.
	array<vector> GeneratePositions(vector center, float squareSize, int amount, int distanceToPos = 0)
	{
	    auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

	    array<vector> positions = new array<vector>;
		vector position;

	    //! Generate positions within a larger square area
	    float largeSquareSize = squareSize * 2;
	    array<vector> largePositions = new array<vector>;
	    for (int i = 0; i < amount * 2; i++)
	    {
	        position = center + Vector(Math.RandomFloat(-largeSquareSize / 2, largeSquareSize / 2), 0, Math.RandomFloat(-largeSquareSize / 2, largeSquareSize / 2));
	        position[1] = GetGame().SurfaceY(position[0], position[2]);

	        if (!GetGame().SurfaceIsSea(position[0], position[2]) && !GetGame().SurfaceIsPond(position[0], position[2]))
	            largePositions.Insert(position);
	    }

	    //! Select valid positions from the larger area
	    while (positions.Count() < amount && largePositions.Count() > 0)
	    {
	        int index = Math.RandomInt(0, largePositions.Count() - 1);
	        position = largePositions[index];
	        largePositions.Remove(index);

	        array<Object> excludes;
	        if (!GetGame().IsBoxColliding(position, Vector(4, 1, 4), Vector(5, 0, 5), excludes))
	        {
	            bool validPos = true;
	            foreach (vector prevPos: positions)
	            {
					if (position == vector.Zero)
					{
	                    validPos = false;
	                    break;
	                }
					
					if (distanceToPos > 0)
					{
		                if (vector.Distance(position, prevPos) < distanceToPos)
		                {
		                    validPos = false;
		                    break;
		                }
					}
	            }

	            if (validPos)
	                positions.Insert(position);
	        }
	    }

	    return positions;
	}

	static ExpansionNamalskModule GetModuleInstance()
	{
		return s_ModuleInstance;
	}

	void ModuleDebugPrint(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};

#ifdef EXPANSIONMODQUESTS
modded class ExpansionQuestModule
{
	override void AfterClientInit(ExpansionQuestPersistentData playerQuestData, PlayerIdentity identity)
	{
		Print(ToString() + "::AfterClientInit - Start");

		super.AfterClientInit(playerQuestData, identity);
		ExpansionNamalskModule.GetModuleInstance().AfterQuestModuleClientInit(playerQuestData, identity);

		Print(ToString() + "::AfterClientInit - Start");
	}
};
#endif