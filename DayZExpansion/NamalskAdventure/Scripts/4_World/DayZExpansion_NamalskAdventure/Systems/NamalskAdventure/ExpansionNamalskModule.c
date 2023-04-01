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
	
	protected int m_AnomalySpawnTries;
	protected ref array<Expansion_Anomaly_Base> m_Anomalies;
	protected ref array<vector> m_AnomalyPositions;

	void ExpansionNamalskModule()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		s_ModuleInstance = this;		
		
		m_Anomalies = new array<Expansion_Anomaly_Base>;
		m_AnomalyPositions = new array<vector>; 
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
			SpawnAnomalies();
			
		#ifdef EXPANSIONMODAI
			SpawnAI();
		#endif

		#ifdef EXPANSIONMODMARKET
			CreateMerchant();
		#endif
		}
	}
	
	protected void SpawnAnomalies()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		foreach (ExpansionAnomalyData anoamly: GetExpansionSettings().GetNamalskAdventure().Anomalies)
		{
			SpawnAnomaly(anoamly);
		}
	}

#ifdef EXPANSIONMODAI	
	protected void SpawnAI()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		foreach (ExpansionAISpawnPosition aiSpawn: GetExpansionSettings().GetNamalskAdventure().AISpawnPositions)
		{
			SpawnSingleAI(aiSpawn);
		}
	}

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
	}
#endif

#ifdef EXPANSIONMODMARKET
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
	
	protected vector GetRandomPosInRadius(vector center, float radius)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		vector pos = center + Vector(Math.RandomFloat(-radius, radius), 0, Math.RandomFloat(-radius, radius));
		pos[1] = GetGame().SurfaceY(pos[0], pos[2]);
		return pos;
	}

	protected void SpawnAnomaly(ExpansionAnomalyData anomaly)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);			
		
		int amountToSpawn = anomaly.Amount;
		int amountSpawned;
		ModuleDebugPrint("::SpawnAnomalies ---------============ Type: " + anomaly.AnomalyType + " | Amount: " + amountToSpawn);
		
		while (amountSpawned < amountToSpawn)
		{
			ModuleDebugPrint("----------------------------------");
			bool validPos = true;
		    vector position = GetRandomPosInRadius(anomaly.CenterPosition, anomaly.Range);
			position[1] = GetGame().SurfaceY(position[0], position[2]);
			
			foreach (vector pos: m_AnomalyPositions)
			{
				float dist = vector.Distance(position, pos);
				if (dist < anomaly.Distance)
					validPos = false;
			}
			
			if (GetGame().SurfaceIsSea(position[0], position[2]) || GetGame().SurfaceIsPond(position[0], position[2]))
				validPos = false;
			
		   	array<Object> exlcudes;
			Object entity;
		    if (GetGame().IsBoxColliding(position, Vector(4, 1, 4), Vector(5, 0, 5), exlcudes))
				validPos = false;
			
			if (!validPos)
			{
				m_AnomalySpawnTries++;
				break;
			}
			
			if (m_AnomalySpawnTries == MAX_ANOMALY_SPAWN_TRIES)
			{
				m_AnomalySpawnTries = 0;
				ModuleDebugPrint("::SpawnAnomaly - To many spawn failures: " + m_AnomalySpawnTries + ". Stop anomaly creation. Type: " + anomaly.AnomalyType + " | Center position: " + anomaly.CenterPosition + " | Randius: " + anomaly.Range);
				return;
			}

		    entity = GetGame().CreateObjectEx(anomaly.AnomalyType, position, ECE_NOLIFETIME);
			Expansion_Anomaly_Base anomalyObj;
			if (!Class.CastTo(anomalyObj, entity))
			{
				GetGame().ObjectDelete(entity);
				return;
			}
			
			ModuleDebugPrint("::SpawnAnomaly - Spawned anomaly: " + "[" + amountSpawned + "] " +  anomaly.AnomalyType + " | Position: " + position.ToString() + " | Spawn tries: " + m_AnomalySpawnTries);
			
			m_AnomalyPositions.Insert(position);
			m_Anomalies.Insert(anomalyObj);
			amountSpawned++;
		}
		
		/*for (int i = 0; i < amountToSpawn; i++) 
		{
			ModuleDebugPrint("----------------------------------");
			bool validPos = true;
		    vector position = GetRandomPosInRadius(anomaly.CenterPosition, anomaly.Range);
			position[1] = GetGame().SurfaceY(position[0], position[2]);
			
			foreach (vector pos: m_AnomalyPositions)
			{
				float dist = vector.Distance(position, pos);
				if (dist < anomaly.Distance)
					validPos = false;
			}
			
			if (GetGame().SurfaceIsSea(position[0], position[2]) || GetGame().SurfaceIsPond(position[0], position[2]))
				validPos = false;
			
		   	array<Object> exlcudes;
			Object entity;
		    if (GetGame().IsBoxColliding(position, Vector(4, 1, 4), Vector(5, 0, 5), exlcudes))
				validPos = false;
			
			if (!validPos)
			{
				amountToSpawn++;
				m_AnomalySpawnTries++;
				continue;
			}
			
			if (m_AnomalySpawnTries == MAX_ANOMALY_SPAWN_TRIES)
			{
				m_AnomalySpawnTries = 0;
				ModuleDebugPrint("::SpawnAnomaly - To many spawn failures: " + m_AnomalySpawnTries + ". Stop anomaly creation. Type: " + anomaly.AnomalyType + " | Center position: " + anomaly.CenterPosition + " | Randius: " + anomaly.Range);
				return;
			}

		    entity = GetGame().CreateObjectEx(anomaly.AnomalyType, position, ECE_NOLIFETIME);
			Expansion_Anomaly_Base anomalyObj;
			if (!Class.CastTo(anomalyObj, entity))
			{
				GetGame().ObjectDelete(entity);
				return;
			}
			
			ModuleDebugPrint("::SpawnAnomaly - Spawned anomaly: " + "[" + i + "] " +  anomaly.AnomalyType + " | Position: " + position.ToString() + " | Spawn tries: " + m_AnomalySpawnTries);
			
			m_AnomalyPositions.Insert(position);
			m_Anomalies.Insert(anomalyObj);
		}*/
	}
	
	/*void AfterQuestModuleClientInit(ExpansionQuestPersistentData playerQuestData, PlayerIdentity identity)
	{
		ModuleDebugPrint("::AfterQuestModuleClientInit - Start");

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
		string stateName = typename.EnumToString(ExpansionQuestState, questState);
		ModuleDebugPrint("::AfterQuestModuleClientInit - Main quest quest-state name: " + stateName);
		if (questState == ExpansionQuestState.NONE)
		{
			//! Show the first quest to the player
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ShowMainQuest, 3000, false, identity);
		}

		ModuleDebugPrint("::AfterQuestModuleClientInit - End");
	}

	protected void ShowMainQuest(PlayerIdentity identity)
	{
		ExpansionQuestModule.GetModuleInstance().RequestOpenQuestMenu(identity, 1);
	}*/
	
	void OnEVRStormInitPhaseServer()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		foreach (Expansion_Anomaly_Base anomaly: m_Anomalies)
		{
			if (anomaly.GetAnomalyCore())
			{
				anomaly.SetAnomalyUnstable();
			}
		}
		
		set<Expansion_AnomalyCore_Base> anomalyCores = Expansion_AnomalyCore_Base.GetAllAnomalyCores();
		foreach (Expansion_AnomalyCore_Base anomalyCore: anomalyCores)
		{
			anomalyCore.SetAnomalyCoreUnstable();
		}
	}
	
	void OnEVRStormEndPhaseServer()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		int randomTime;
		foreach (Expansion_Anomaly_Base anomaly: m_Anomalies)
		{
			if (anomaly.GetAnomalyCore())
			{
				randomTime = Math.RandomInt(1, 3);
				anomaly.SetAnomalyExplosion(randomTime);
			}
		}
	}
	
	bool IsEVRStormActive()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
	#ifdef NAMALSK_SURVIVAL
	    NamEventManager event_manager;
	    g_Script.CallFunction(GetGame().GetMission(), "GetNamEventManager", event_manager, null);
	
	    if (!event_manager)
	        return false;
	
	    return EVRStorm.Cast(event_manager.GetEvent(EVRStorm)) || EVRStorm.Cast(event_manager.GetEvent(EVRStormDeadly));
	#endif
	
		return false;
	}

	static ExpansionNamalskModule GetModuleInstance()
	{
		return s_ModuleInstance;
	}

	void ModuleDebugPrint(string text)
	{
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	}
 };

/*modded class ExpansionQuestModule
{
	override void AfterClientInit(ExpansionQuestPersistentData playerQuestData, PlayerIdentity identity)
	{
		Print(ToString() + "::AfterClientInit - Start");

		super.AfterClientInit(playerQuestData, identity);
		ExpansionNamalskModule.GetModuleInstance().AfterQuestModuleClientInit(playerQuestData, identity);

		Print(ToString() + "::AfterClientInit - Start");
	}
};*/