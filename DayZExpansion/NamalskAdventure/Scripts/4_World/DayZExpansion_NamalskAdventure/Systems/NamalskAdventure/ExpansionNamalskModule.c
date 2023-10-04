/**
 * ExpansionNamalskModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
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
	protected ref ExpansionObjectSet m_MerchantObjectSet;	//! Server

	#ifdef EXPANSIONMODAI
	protected ref map<eAIBase, ref array<vector>> m_SpawnedAI;
	protected int m_AISpeed;
	#ifdef EXPANSIONMODQUESTS
	protected ref map<eAIBase, ref array<vector>> m_QuestAI;
	#endif
	#endif

	protected typename m_LastNamalskEvent;

	protected bool m_EVRStormActive;
	protected bool m_EVRStormBlowout;

	#ifdef EXPANSION_NAMALSK_ADVENTURE
	protected SV_Abandoned_Sattelite_Antenna m_AbdonedSatellite;
	#ifdef EXPANSIONMODTELEPORTER
	protected Expansion_Teleporter_Big m_SatelliteTeleporter;
	#endif
	protected Expansion_Satellite_Control m_SatelliteController;
	protected Expansion_Satellite_Generator m_SatelliteGenerator;
	
	protected bool m_SatelliteFacilityHasPower = false;

	protected const float UPDATE_TICK_TIME = 0.500;
	protected float m_UpdateQueueTimer;

	protected const float SATELLITE_CRY_TIME = 300.0; //! 5 minutes tick
	protected float m_SatelliteCryTimer;
	protected bool m_SatelliteState;

	protected const float CLIENT_UPDATE_TIME = 5.0; //! 5 seconds tick
	protected float m_ClientUpdateTimer;

	protected const float BUNKER_GENERATORS_CHECK_TIME = 10.0; //! 10 seconds tick
	protected float m_BunkerGeneratorsCheckTimer;
	protected static ScriptCaller s_BunkerGeneratorCheckSC;
	static const int BUNKER_GENERATOR_DEACTIVATION_TIME = 300.0; //! 5 minute delay.

	protected int m_CurrentCheckTime;

	protected const vector m_A1_Bunker_EntrancePos = "4229.486328 81.322319 8218.904297";

	protected const vector m_A1_Bunker_GeneratorPos = "1851.220459 208.544495 1386.371704";
	protected const vector m_A1_Bunker_GeneratorOri = "170.999863 0.000000 -0.000000";

	protected const vector m_A1_Bunker_TeleporterPos = "1806.715942 196.610580 1390.737427";
	protected const vector m_A1_Bunker_TeleporterOri = "-96.636742 0.000000 -0.000000";

	protected const vector m_A1_Bunker_EntrancePanelPos = "4226.144531 82.264626 8193.572266";
	protected const vector m_A1_Bunker_EntrancePanelOri = "-7.753430 -0.000000 -0.000000";

	protected const vector m_A1_Bunker_EntranceLeaverPos = "4231.801758 81.996811 8198.053711";
	protected const vector m_A1_Bunker_EntranceLeaverOri = "-97.819839 -0.000000 -0.000000";

	protected const vector m_A1_Bunker_FakeEntranceLeaverPos = "1925.534912 242.507050 1202.186157";
	protected const vector m_A1_Bunker_FakeEntranceLeaverOri = "-98.999931 0.000000 -0.000000";

	#ifdef EXPANSIONMODTELEPORTER
	protected Expansion_Teleporter_Big m_A1BunkerTeleporter;
	#endif
	protected Expansion_Bunker_Generator m_A1BungerGenerator;

	protected Land_Underground_Panel m_A1BunkerEntrancePanel;
	protected Land_Underground_Panel_Lever m_A1BunkerEntranceLeaver;
	protected Land_Underground_Panel_Lever m_A1BunkerFakeEntranceLeaver;

	protected Land_Repair_Center m_SurvivorsRepairCenter;
	#endif
	
	protected Expansion_Fusion_Core m_FusionCoreResistance;

	protected const float SUPPLY_CRATES_CHECK_TIME = 60.0; //! 1 minute tick
	protected float m_SupplyCratesCheckTimer;
	protected static ScriptCaller s_SupplyCratesCheckSC;
	static const int LOOT_DELAY_TIME = 300; //! 5 minute delay.

	protected int m_Expansion_ProcessedInfected;
	protected static ScriptCaller s_EVRStormBlowoutInfectedSC;
	protected const int UPDATE_INFECTED_PER_TICK = 5;  //! Refresh up to 5 infected every UPDATE_TICK_TIME interval

	protected ref ScriptInvoker m_WorkbenchMenuInvoker; //! Client

	void ExpansionNamalskModule()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		s_ModuleInstance = this;

		#ifdef EXPANSIONMODAI
		m_SpawnedAI = new map<eAIBase, ref array<vector>>;
		#ifdef EXPANSIONMODQUESTS
		m_QuestAI = new map<eAIBase, ref array<vector>>;
		#endif
		#endif
	}

	override void OnInit()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.OnInit();

		EnableMissionStart();
		EnableMissionLoaded();
		EnableRPC();
		#ifdef SERVER
		EnableUpdate();
		#endif
	}

	protected void CreateDirectoryStructure()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!FileExist(s_dataFolderPath))
			ExpansionStatic.MakeDirectoryRecursive(s_dataFolderPath);
	}

	override void OnMissionStart(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.OnMissionLoaded(sender, args);

		//! Server only
		#ifdef SERVER
		CreateDirectoryStructure(); //! Create directoy structure if not existing.
		LoadNamalskAdventureServerData(); //! Load namalsk adventure server data.
		#endif
	}

	override void OnMissionLoaded(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		#ifdef SERVER
		ServerModuleInit();
		#endif

		#ifndef SERVER
		ClientModuleInit();
		#endif
	}

	protected void ServerModuleInit()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		//! Server only
		#ifdef EXPANSIONMODAI
		if (GetExpansionSettings().GetNamalskAdventure().EnableAISpawns)
			SpawnAI();
		#endif
		#ifdef EXPANSIONMODMARKET
		if (GetExpansionSettings().GetNamalskAdventure().EnableMerchant)
			CreateMerchant();
		#endif

		if (GetExpansionSettings().GetNamalskAdventure().EnableSupplyCrates)
			SpawnSupplyCrates();

		#ifdef EXPANSION_NAMALSK_ADVENTURE
		SpawnSatelliteAntennaObjects(); //! @note: Secret antenna event objects. Not finished yet!
		SpawnA1Bunker(); //! @note: A1 bunker event objects.
		SpawnFusionCores();
		#endif
	}

	protected void ClientModuleInit()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_WorkbenchMenuInvoker = new ScriptInvoker();
	}

	protected void LoadNamalskAdventureServerData()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (FileExist(s_dataFolderPath + "ServerData.json"))
		{
			GetServerData(s_dataFolderPath + "ServerData.json");
		}
		else
		{
			m_ServerData = new ExpansionNamalskAdventureData();
			m_ServerData.Save();
		}
	}

	protected void GetServerData(string file)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_ServerData = ExpansionNamalskAdventureData.Load(file);
		if (!m_ServerData)
		{
			Error(ToString() + "::GetServerData - Could not get namalsk adventure server data!");
			return;
		}
	}

	#ifdef EXPANSIONMODAI
	//! @note: Spawns all configured AI units from the NamalskAdventureSettings class.
	protected void SpawnAI()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		array<ref ExpansionAISpawnPosition> aiPositions = GetExpansionSettings().GetNamalskAdventure().AISpawnPositions;
		for (int i = 0; i < aiPositions.Count(); i++)
		{
			ExpansionAISpawnPosition aiSpawn = aiPositions[i];
			if (!aiSpawn)
				continue;

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
		
		ai.m_Expansion_NetsyncData.Set(0, aiSpawn.NPCName);
		ai.m_Expansion_NetsyncData.Set(1, "{3364F58EF7F7FBE3}DayZExpansion/Core/GUI/icons/misc/T_Soldier_256x256.edds");

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
		aiGroup.SetWaypointBehaviour(eAIWaypointBehavior.HALT_OR_ALTERNATE);

		for (int idx = 0; idx < aiSpawn.Waypoints.Count(); idx++)
		{
			aiGroup.AddWaypoint(aiSpawn.Waypoints[idx]);
			if (aiSpawn.Waypoints[idx] == pos)
				aiGroup.m_CurrentWaypointIndex = idx;
		}

		if (aiSpawn.ShelterPositions && aiSpawn.ShelterPositions.Count() > 0)
		{
			m_SpawnedAI.Insert(ai, aiSpawn.ShelterPositions);
		}
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

			for (int i = 0; i < GetExpansionSettings().GetNamalskAdventure().MerchantPositions.Count(); i++)
			{
				ExpansionMerchantPosition pos = GetExpansionSettings().GetNamalskAdventure().MerchantPositions[i];
				if (!pos)
					continue;

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

			for (int j = 0; j < GetExpansionSettings().GetNamalskAdventure().MerchantItemSets.Count(); j++)
			{
				ExpansionMerchantItemSet itemSet = GetExpansionSettings().GetNamalskAdventure().MerchantItemSets[j];
				if (!itemSet)
					continue;

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
		
		//! Create the temporary market zone for the merchant
		ExpansionMarketTraderZone traderZone = new ExpansionMarketTraderZone();
		traderZone.m_DisplayName = "Merchant";
		traderZone.Position = positionToUse.Position;
		traderZone.Radius = 10.0;
		traderZone.BuyPricePercent = 100.0;
		traderZone.SellPricePercent = -1.0;
		
		//! Create the temporary trader data
		ExpansionMarketTrader trader = new ExpansionMarketTrader();
		trader.DisplayName = "Merchant";
		#ifdef EXPANSIONMODHARDLINE
		trader.MinRequiredReputation = 0;
		trader.MaxRequiredReputation = int.MAX;
		#endif
		trader.TraderIcon = "Deliver";
		trader.Currencies.Insert("expansionbanknotehryvnia");
		
		//! Add the items this trader will sell to the temporary market zone
		string className;
		int amount;
		ExpansionMarketTraderBuySell buySell;

		array<ref ExpansionMerchantItem> items = itemSetToUse.Items;

		for (int k = 0; k < items.Count(); k++)
		{
			ExpansionMerchantItem item = items[k];
			if (!item)
				continue;

			className = item.ClassName;
			amount = item.Amount;
			buySell = item.BuySell;

			traderZone.AddStock(className, amount);
			trader.AddItem(className, buySell);
		}
		
		marketSettings.AddMarketZone(traderZone);
		marketSettings.AddMarketTrader(trader);
		
		//! Create the temporary trader entity
		Object obj = GetGame().CreateObject("ExpansionTraderMirek", positionToUse.Position);
		ExpansionTraderNPCBase npcTrader = ExpansionTraderNPCBase.Cast(obj);
		if (!npcTrader)
		{
			Error(ToString() + "::CreateMerchant - Could not spawn merchant object!");
			GetGame().ObjectDelete(obj);
			return;
		}

		ExpansionTraderObjectBase traderBase = new ExpansionTraderObjectBase(npcTrader);
		if (!traderBase)
		{
			Error(ToString() + "::CreateMerchant - Could not spawn merchant trader base object!");
			GetGame().ObjectDelete(obj);
			return;
		}

		traderBase.SetTraderZone(traderZone);
		traderBase.SetTraderMarket(trader);

		npcTrader.SetTraderObject(traderBase);
		npcTrader.m_Expansion_NetsyncData.Set(0, "Merchant");
		npcTrader.m_Expansion_NetsyncData.Set(1, "{5F2743E5F6F4DF0D}DayZExpansion/Core/GUI/icons/misc/coinstack2_64x64.edds");
		ExpansionHumanLoadout.Apply(npcTrader, "SurvivorLoadout", false);
		npcTrader.SetPosition(positionToUse.Position);
		npcTrader.SetOrientation(positionToUse.Orientation);
		traderBase.UpdateTraderZone();

		#ifdef EXPANSIONMODNAVIGATION
		//! Create a server marker on the trader position
		m_MerchantServerMarker = ExpansionMarkerModule.GetModuleInstance().CreateServerMarker("Merchant", "Coins 2", positionToUse.Position, ARGB(255, 15, 185, 177), false);
		#endif
		
		//! Spawn the map objects set for this trader entry
		if (FileExist("$mission:expansion\\namalsk_adventure\\objects\\" + positionToUse.ObjectSetFile + ".map"))
		{
			m_MerchantObjectSet = new ExpansionObjectSet("$mission:expansion\\namalsk_adventure\\objects\\" + positionToUse.ObjectSetFile + ".map", positionToUse.ObjectSetFile);
			m_MerchantObjectSet.SpawnObjects();
		}
	}
	#endif

	void OnNamalskEventStart(typename eventType)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (eventType == EVRStorm || eventType == EVRStormDeadly)
		{
			m_EVRStormActive = true;
		}
	}

	void OnNamalskEventCancel(typename eventType)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		#ifdef DIAG
		ExpansionNotification(new StringLocaliser("NAMALSK EVENT CANCELED"), new StringLocaliser("%1 EVENT CANCELED", eventType.ToString()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_INFO, 7, ExpansionNotificationType.TOAST).Create();
		#endif

		m_LastNamalskEvent = eventType;

		if (eventType == EVRStorm || eventType == EVRStormDeadly)
		{
			m_EVRStormActive = false;
			//SetSatelitteActive(false);
		}
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

		//SetSatelitteActive(true);

		#ifdef EXPANSIONMODAI
		SetAIBuildingPositions();
		#endif
	}

	//! @note: Handles events that should start when ever the EVR storm final blowout starts.
	//! Modified version to process and loop thrue spawned anomaly entities in chuncks.
	void OnEVRStormFinalBlowout()
	{
	    auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_EVRStormBlowout = true;
	}

	//! @note: Handles events that should start when ever a EVR storm event end phase starts.
	void OnEVRStormEndPhaseServer()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		#ifdef EXPANSIONMODAI
		ResetAIPositions();
		#endif

		NamEventManager event_manager;
	    g_Script.CallFunction(GetGame().GetMission(), "GetNamEventManager", event_manager, null);

		m_LastNamalskEvent = event_manager.GetLastEventType();

		//SetSatelitteActive(false);
	}

	//! @note: Handles events that should start when ever a EVR storm event ended.
	//! Gets called on every active player on the server.
	void OnEVRStormFinished(SurvivorBase player)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		//! PLACEHOLDER
	}

	#ifdef EXPANSIONMODAI
	void SetAIBuildingPositions()
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
	}

	//! @note: Handles reset of spawned AI units and active quest AI NPCs waypoints after a EVR strom event.
	protected void ResetAIPositions()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		for (int i = 0; i < m_SpawnedAI.Count(); i++)
		{
			eAIBase npcAI = m_SpawnedAI.GetKey(i);
			ModuleDebugPrint("::ResetAIPositions - AI: " + npcAI.GetType());
			ResetSpawnedAIWaypoints(npcAI);
		}
	}

	protected void GetAIBuildingWaypoint(eAIBase ai)
	{
	    auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

	    if (!ai.IsAlive())
	        return;

	    eAIGroup aiGroup = ai.GetGroup();
	    if (!aiGroup)
	        return;

		array<vector> shelterPositions = m_SpawnedAI.Get(ai);
		ModuleDebugPrint("::GetAIBuildingWaypoint - Shelter positions: " + shelterPositions.ToString() + " | Count: " + shelterPositions.Count());
		if (shelterPositions)
		{
			array<vector> waypoints = new array<vector>;
			waypoints.Copy(aiGroup.GetWaypoints());
			m_SpawnedAI.Set(ai, waypoints);
			
			if (shelterPositions.Count() > 0 && !MiscGameplayFunctions.IsUnderRoof(ai))
			{
				aiGroup.ClearWaypoints();
				aiGroup.AddWaypoint(shelterPositions.GetRandomElement());
				aiGroup.m_CurrentWaypointIndex = 0;
				ai.SetMovementSpeedLimit(eAIMovementSpeed.JOG);
				m_AISpeed = ai.GetMovementSpeedLimit();
			}
		}
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
		for (int i = 0; i < waypoints.Count(); i++)
		{
			vector pos = waypoints[i];

			ModuleDebugPrint("::ResetSpawnedAIWaypoints - Adding back old waypoint: " + waypoints.Count());
			aiGroup.AddWaypoint(pos);
		}

		aiGroup.m_CurrentWaypointIndex = 0;
		ai.SetMovementSpeedLimit(m_AISpeed);
	}
	#endif

	/*
	#ifdef EXPANSIONMODQUESTS
	void AfterQuestModuleClientInit(ExpansionQuestPersistentData playerQuestData, PlayerIdentity identity)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

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
	}

	protected void ShowMainQuest(PlayerIdentity identity)
	{
		ExpansionQuestModule.GetModuleInstance().RequestOpenQuestMenu(identity, 1);
	}
	#endif
	*/

	#ifdef EXPANSION_NAMALSK_ADVENTURE
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

	protected void SpawnSatelliteAntennaObjects()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_AbdonedSatellite = SV_Abandoned_Sattelite_Antenna.Cast(GetGame().CreateObject("SV_Abandoned_Sattelite_Antenna", "1227.198242 14.356772 11766.359375"));
		if (m_AbdonedSatellite)
		{
			m_AbdonedSatellite.SetPosition("1227.198242 14.356772 11766.359375");
			m_AbdonedSatellite.SetOrientation("81.432739 0.000000 -0.000000");
			m_AbdonedSatellite.Update();
		}
		
		m_SatelliteGenerator = Expansion_Satellite_Generator.Cast(GetGame().CreateObject("Expansion_Satellite_Generator", "1193.147095 1.941499 11813.058594"));
		if (m_SatelliteGenerator)
		{
			m_SatelliteGenerator.SetPosition("1193.147095 1.941499 11813.058594");
			m_SatelliteGenerator.SetOrientation("-98.999878 0.000000 -0.000000");
			m_SatelliteGenerator.Update();
		}

	#ifdef EXPANSIONMODTELEPORTER
		m_SatelliteTeleporter = Expansion_Teleporter_Big.Cast(GetGame().CreateObject("Expansion_Teleporter_Big", "1211.435059 2.206216 11724.829102"));
		if (m_SatelliteTeleporter)
		{
			m_SatelliteTeleporter.SetPosition("1211.435059 2.206216 11724.829102");
			m_SatelliteTeleporter.SetOrientation("-95.745811 0.000000 0.000000");
			m_SatelliteTeleporter.SetTeleporterID(100);
			m_SatelliteTeleporter.SetActive(false);

			ExpansionTeleportData teleporterData = new ExpansionTeleportData();
			teleporterData.SetID(100);
			teleporterData.SetDisplayName("???");
			teleporterData.SetObjectPosition("1211.435059 2.206216 11724.829102");

			ExpansionTeleportPosition teleportPos = new ExpansionTeleportPosition();
			teleportPos.SetData("???");
			teleportPos.AddPosition(Vector(5079.959961, 2085.610107, 11720.700195), Vector(39.183594, 0.000000, 0.000000));
			teleportPos.AddPosition(Vector(5075.359863, 2085.610107, 11715.299805), Vector(132.597000, 0.000000, 0.000000));

			teleporterData.AddTeleportPosition(teleportPos);

			ExpansionTeleporterModule.GetModuleInstance().AddTeleporterData(teleporterData);
			m_SatelliteTeleporter.Update();
		}
	#endif

		m_SatelliteController = Expansion_Satellite_Control.Cast(GetGame().CreateObject("Expansion_Satellite_Control", "1221.751343 2.596711 11723.593750"));
		if (m_SatelliteController)
		{
			m_SatelliteController.SetPosition("1221.751343 2.596711 11723.593750");
			m_SatelliteController.SetOrientation("-10.577682 0.000000 -0.000000");
			if (m_AbdonedSatellite)
				m_SatelliteController.SetLinkedSatellite(m_AbdonedSatellite);
			#ifdef EXPANSIONMODTELEPORTER
			if (m_SatelliteTeleporter)
				m_SatelliteController.SetLinkedTeleporter(m_SatelliteTeleporter);
			#endif
			SetSatelitteActive(true);
			m_SatelliteController.Update();
		}
	}

	protected void PlaySatelliteCrySFX()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		string soundSet = "Expansion_Satellite_Cry_Distance_SoundSet";
		Param3<bool, vector, int> satelliteCry = new Param3<bool, vector, int>(true, Vector(1202, 14, 11784), soundSet.Hash());
		GetGame().RPCSingleParam(null, ERPCs.RPC_SOUND_HELICRASH, satelliteCry, true);
	}
	
	void SetSatelliteFacilityPower(bool state)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (state == m_SatelliteFacilityHasPower)
			return;
		
		if (state)
		{
			ActivateSatelliteFacility();
		}
		else
		{
			DeactivateSatelliteFacility();
		}
		
		m_SatelliteFacilityHasPower = state;
	}
	
	void ActivateSatelliteFacility()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	}
	
	void DeactivateSatelliteFacility()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	}
	
	void OverloadSatelliteGenerator()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (m_SatelliteGenerator)
		{
			if (m_SatelliteGenerator.GetCompEM().CanSwitchOff())
			{
				m_SatelliteGenerator.GetCompEM().SwitchOff();
				m_SatelliteGenerator.GetCompEM().InteractBranch(m_SatelliteGenerator);
			}
			
			ItemBase sparkPlug = ItemBase.Cast(m_SatelliteGenerator.FindAttachmentBySlotName("SparkPlug"));
			if (sparkPlug)
				sparkPlug.AddHealthLevel(4);
		}
	}
	
	bool HasSatelliteFacilityPower()
	{
		return m_SatelliteFacilityHasPower;
	}

	void SetSatelitteActive(bool state)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (state == m_SatelliteState)
			return;

		if (state)
		{
			ActivateSatellite();
		}
		else
		{
			DeactivateSatellite();
		}

		m_SatelliteState = state;
	}
	
	bool GetSatelliteState()
	{
		return m_SatelliteState;
	}

	void ActivateSatellite()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!m_AbdonedSatellite || !m_SatelliteController)
			return;
		
		if (!m_SatelliteController.IsActive())
		{
			m_SatelliteController.SetActivateState(true);
			m_AbdonedSatellite.SetSatelliteActive(true);
		}
	}

	void DeactivateSatellite()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!m_AbdonedSatellite || !m_SatelliteController)
			return;

		if (m_SatelliteController.IsActive())
		{
			m_SatelliteController.SetActivateState(false);
			m_AbdonedSatellite.SetSatelliteActive(false);
		}
	}

	SV_Abandoned_Sattelite_Antenna GetSatelliteAntenna()
	{
		return m_AbdonedSatellite;
	}

	#ifdef EXPANSIONMODTELEPORTER
	Expansion_Teleporter_Big GetSatelliteTeleporter()
	{
		return m_SatelliteTeleporter;
	}
	#endif

	Expansion_Satellite_Control GetSatelliteController()
	{
		return m_SatelliteController;
	}
	
	Expansion_Satellite_Generator GetSatelliteGenerator()
	{
		return m_SatelliteGenerator;
	}
	
	protected void SpawnFusionCores()
	{
		//! Resistance Camp
		m_FusionCoreResistance = Expansion_Fusion_Core.Cast(ExpansionWorldObjectsModule.SpawnObject("Expansion_Fusion_Core", Vector(5990.666016, 7.455487, 10012.241211), Vector(-159.684494, 0.000000, -0.000000), false, false));
		m_FusionCoreResistance.SetPosition(Vector(5990.666016, 7.455487, 10012.241211));
		m_FusionCoreResistance.Update();
	}

	protected void SpawnA1Bunker()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		//! Entrance trigger
		ExpansionBunkerEntranceTriggerBase trigger = ExpansionBunkerEntranceTriggerBase.Cast(ExpansionWorldObjectsModule.SpawnObject("ExpansionBunkerEntranceTriggerBase", m_A1_Bunker_EntrancePos, "0 0 0", false, false));
		if (trigger)
		{
			trigger.SetPosition(m_A1_Bunker_EntrancePos);
			trigger.SetTriggerRadius(1);
			trigger.Update();

			Print(ToString() + "::SpawnA1Bunker - Spanwed A1 Bunker entrance trigger at position: " + trigger.GetPosition());
		}
		
		//! Bunker event teleporter
		#ifdef EXPANSIONMODTELEPORTER
		m_A1BunkerTeleporter = Expansion_Teleporter_Big.Cast(ExpansionWorldObjectsModule.SpawnObject("Expansion_Teleporter_Big", m_A1_Bunker_TeleporterPos, m_A1_Bunker_TeleporterOri, false, false));
		if (m_A1BunkerTeleporter)
		{
			m_A1BunkerTeleporter.SetTeleporterID(101);
			m_A1BunkerTeleporter.SetActive(false);

			ExpansionTeleportData teleporterData = new ExpansionTeleportData();
			teleporterData.SetID(101);
			teleporterData.SetDisplayName("???");
			teleporterData.SetObjectPosition(m_A1_Bunker_TeleporterPos);
			teleporterData.SetIsExit(true);

			ExpansionTeleportPosition teleportPos = new ExpansionTeleportPosition();
			teleportPos.SetData("???");
			teleportPos.AddPosition(Vector(4272.379883, 98.819305, 8398.056641), Vector(0.000000, 0.000000, 0.000000));
			teleportPos.AddPosition(Vector(4580.25, 85.4889, 8432.24), Vector(0.000000, 0.000000, 0.000000));
			teleportPos.AddPosition(Vector(4753.02, 64.3489, 8264.39), Vector(0.000000, 0.000000, 0.000000));
			teleporterData.AddTeleportPosition(teleportPos);
			ExpansionTeleporterModule.GetModuleInstance().AddTeleporterData(teleporterData);

			Print(ToString() + "::SpawnA1Bunker - Spanwed A1 Bunker teleporter object at position: " + m_A1BunkerTeleporter.GetPosition());
		}
		#endif

		//! Bunker event generator
		m_A1BungerGenerator = Expansion_Bunker_Generator.Cast(ExpansionWorldObjectsModule.SpawnObject("Expansion_Bunker_Generator", m_A1_Bunker_GeneratorPos, m_A1_Bunker_GeneratorOri, false, false));
		if (m_A1BungerGenerator)
		{
			#ifdef EXPANSIONMODTELEPORTER
			if (m_A1BunkerTeleporter)
				m_A1BungerGenerator.SetLinkedTeleporter(m_A1BunkerTeleporter);
			#endif

			Print(ToString() + "::SpawnA1Bunker - Spanwed A1 Bunker generator at position: " + m_A1BungerGenerator.GetPosition());
		}

		#ifdef EXPANSIONMODQUESTS
		//! Bunker event locker quest object
		//! @note: Make this a config param class in the namalsk settings class you lazy ass!
		ExpansionTempQuestHolder questHolder = new ExpansionTempQuestHolder(1000, "ExpansionQuestObjectLocker", "Closed Locker");
		if (!questHolder)
			return;

		ExpansionTempQuestHolderPosition pos = new ExpansionTempQuestHolderPosition("1908.662354 201.666977 1244.743164", "173.734970 0.000000 -0.000000");
		questHolder.AddPosition(pos);
		pos = new ExpansionTempQuestHolderPosition("1915.156006 201.659302 1231.404419", "81.000038 -0.000000 -0.000000");
		questHolder.AddPosition(pos);
		pos = new ExpansionTempQuestHolderPosition("1899.852783 195.486664 1306.718994", "174.815613 0.000000 0.000000");
		questHolder.AddPosition(pos);
		pos = new ExpansionTempQuestHolderPosition("1906.268677 196.687729 1291.300415", "80.999977 -0.000000 -0.000000");
		questHolder.AddPosition(pos);

		ExpansionTempQuestHolderPosition randomPos = questHolder.Positions.GetRandomElement();
		if (!randomPos)
			return;

		ExpansionQuestModule.GetModuleInstance().SpawnQuestHolder(questHolder, randomPos);

		//! Modify quest travel objective position
		ExpansionQuestObjectiveTravelConfig travelObjective = ExpansionQuestModule.GetModuleInstance().GetTravelObjectiveConfigByID(1000);
		if (!travelObjective)
			return;

		travelObjective.SetPosition(randomPos.Position);
		#endif

		//! Bunker fake entrance panel
		m_A1BunkerFakeEntranceLeaver = Land_Underground_Panel_Lever.Cast(ExpansionWorldObjectsModule.SpawnObject("Land_Underground_Panel_Lever", m_A1_Bunker_FakeEntranceLeaverPos, m_A1_Bunker_FakeEntranceLeaverOri, false, false));
		if (m_A1BunkerFakeEntranceLeaver)
		{
			Print(ToString() + "::SpawnA1Bunker - Spanwed A1 Bunker fake entrance lever panel at position: " + m_A1BunkerFakeEntranceLeaver.GetPosition());
		}

		//! Bunker entrance panel
		m_A1BunkerEntrancePanel = Land_Underground_Panel.Cast(ExpansionWorldObjectsModule.SpawnObject("Land_Underground_Panel", m_A1_Bunker_EntrancePanelPos, m_A1_Bunker_EntrancePanelOri, false, false));
		if (m_A1BunkerEntrancePanel)
		{
			m_A1BunkerEntrancePanel.SetLinkedFakePanel(m_A1BunkerFakeEntranceLeaver);
			Print(ToString() + "::SpawnA1Bunker - Spanwed A1 Bunker entrance panel at position: " + m_A1BunkerEntrancePanel.GetPosition());
		}

		//! Bunker entrance panel leaver
		m_A1BunkerEntranceLeaver = Land_Underground_Panel_Lever.Cast(ExpansionWorldObjectsModule.SpawnObject("Land_Underground_Panel_Lever", m_A1_Bunker_EntranceLeaverPos, m_A1_Bunker_EntranceLeaverOri, false, false));
		if (m_A1BunkerEntranceLeaver)
		{
			m_A1BunkerEntranceLeaver.SetLinkedFakePanel(m_A1BunkerFakeEntranceLeaver);
			Print(ToString() + "::SpawnA1Bunker - Spanwed A1 Bunker fake entrance panel at position: " + m_A1BunkerEntranceLeaver.GetPosition());
		}
	}
	#endif

	//! @note: Method that handles spawning of configured supply crates in the ExpansionNamalskAdventureSettings class with there loot on mission load.
	protected void SpawnSupplyCrates()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		//! Supply crates
		array<ref ExpansionSupplyCrateSetup> supplyCrateSpawns = GetExpansionSettings().GetNamalskAdventure().GetSupplyCrateSpawns();
		foreach (ExpansionSupplyCrateSetup supplyCrate: supplyCrateSpawns)
		{
			Expansion_SupplyCrate_Base supplyCareObj = Expansion_SupplyCrate_Base.Cast(GetGame().CreateObject(supplyCrate.ClassName, supplyCrate.Position));
			if (!supplyCareObj)
			{
				Error(ToString() + "::SpawnSupplyCrates - Could not spawn supply crate object!");
				continue;
			}

			supplyCareObj.SetPosition(supplyCrate.Position);
			supplyCareObj.SetOrientation(supplyCrate.Orientation);
			supplyCareObj.SetCrateLoot(supplyCrate.CrateLoot);
			supplyCareObj.Update();
		}
	}

	override void OnUpdate(Class sender, CF_EventArgs args)
	{
		super.OnUpdate(sender, args);

		auto update = CF_EventUpdateArgs.Cast(args);

		#ifdef SERVER
		OnUpdateServer(update.DeltaTime);
		#else
		OnUpdateClient(update.DeltaTime);
		#endif
	}
	
	//! @note: Server side update loop
	protected void OnUpdateServer(float deltaTime)
	{
		#ifdef EXPANSION_NAMALSK_ADVENTURE
		//! Satellite cry sound FX
		m_SatelliteCryTimer += deltaTime;
		if (m_SatelliteCryTimer >= SATELLITE_CRY_TIME)
		{
			PlaySatelliteCrySFX();
			m_SatelliteCryTimer = 0;
		}

		//! Reset bunker generators
		m_BunkerGeneratorsCheckTimer += deltaTime;
		if (m_BunkerGeneratorsCheckTimer >= BUNKER_GENERATORS_CHECK_TIME)
		{
			if (!s_BunkerGeneratorCheckSC)
		    	s_BunkerGeneratorCheckSC = ScriptCaller.Create(OnBunkerGeneratorCheck);

			m_CurrentCheckTime = CF_Date.Now(true).GetTimestamp();

			Expansion_Bunker_Generator.s_Expansion_AllBunkerGenerators.Each(s_BunkerGeneratorCheckSC);
			m_BunkerGeneratorsCheckTimer = 0;
		}

		//! Reset supply crates
		m_SupplyCratesCheckTimer += deltaTime;
		if (m_SupplyCratesCheckTimer >= SUPPLY_CRATES_CHECK_TIME)
		{
			if (!s_SupplyCratesCheckSC)
		    	s_SupplyCratesCheckSC = ScriptCaller.Create(OnSupplyCrateCheck);

			m_CurrentCheckTime = CF_Date.Now(true).GetTimestamp();

			Expansion_SupplyCrate_Base.s_Expansion_AllSupplyCrates.Each(s_SupplyCratesCheckSC);
			m_SupplyCratesCheckTimer = 0;
		}
		#endif

		//! Events on EVR Storms - Not used yet
		/*m_UpdateQueueTimer += deltaTime;
		if (m_UpdateQueueTimer >= UPDATE_TICK_TIME)
		{
			//! @note: Update events when m_EVRStormBlowout flag is set to true.
			if (m_EVRStormActive && m_EVRStormBlowout)
			{
				OnEVRStormBlowout();
			}

			m_UpdateQueueTimer = 0;
		}*/
	}

	#ifdef EXPANSION_NAMALSK_ADVENTURE
	protected void OnSupplyCrateCheck(Expansion_SupplyCrate_Base supplyCrate)
	{
		auto trace = EXTrace.Profile(EXTrace.NAMALSKADVENTURE, this);

		if (supplyCrate.HasLootDelay() && !supplyCrate.IsCrateOpened())
		{
			int crateCloseTime = supplyCrate.GetLastCloseTime();
			int crateReactivationTime = crateCloseTime + LOOT_DELAY_TIME;
			if (m_CurrentCheckTime > crateReactivationTime)
			{
				supplyCrate.EndLootDelay();
			}
		}
		else if (supplyCrate.IsCrateOpened())
		{
			int crateOpenTime = supplyCrate.GetLastOpenTime();
			int crateDelayTime = crateOpenTime + LOOT_DELAY_TIME;
			if (m_CurrentCheckTime > crateDelayTime)
			{
				supplyCrate.CloseCrate();
			}
		}
	}

	protected void OnBunkerGeneratorCheck(Expansion_Bunker_Generator generator)
	{
		auto trace = EXTrace.Profile(EXTrace.NAMALSKADVENTURE, this);

		if (generator.IsActive())
		{
			int generatorActivationTime = generator.GetLastActivationTime();
			int generatorDisableTime = generatorActivationTime + BUNKER_GENERATOR_DEACTIVATION_TIME;
			if (m_CurrentCheckTime > generatorDisableTime)
			{
				int slot_id_key = InventorySlots.GetSlotIdFromString("Att_ExpansionGeneratorKey");
				Expansion_Bunker_Generator_Key key = Expansion_Bunker_Generator_Key.Cast(generator.GetInventory().FindAttachment(slot_id_key));
				if (!key)
					return;

				GetGame().ObjectDelete(key);
			}
		}
	}
	#endif
	
	//! @note: Client side update loop
	protected void OnUpdateClient(float deltaTime)
	{
		m_ClientUpdateTimer += deltaTime;
		if (m_ClientUpdateTimer >= CLIENT_UPDATE_TIME && GetGame().GetPlayer())
		{
			//! @note: Preload A1 Bunker objects on client when the client is near bunker entrance position. 
			#ifdef EXPANSION_NAMALSK_ADVENTURE
			float dist = vector.Distance(m_A1_Bunker_TeleporterPos, GetGame().GetPlayer().GetPosition());
			if (dist < 200)
			{
				GetGame().PreloadObject("Land_Underground_Stairs_Exit", 3000);
				GetGame().PreloadObject("Land_Underground_Stairs_Block", 3000);
				GetGame().PreloadObject("Land_Underground_Stairs_Start", 3000);
				GetGame().PreloadObject("Land_Underground_Floor_Crew", 3000);
				GetGame().PreloadObject("Land_Underground_Floor_Comms", 3000);
				GetGame().PreloadObject("Land_Underground_Stairs_Collapsed", 3000);
				GetGame().PreloadObject("Land_Mil_Barracks_Round", 3000);
				GetGame().PreloadObject("Land_Underground_Stairs_Block_Terminator", 3000);
				GetGame().PreloadObject("bldr_expansion_Sign_roadbarrier", 3000);
				GetGame().PreloadObject("ExpansionQuestObjectLocker", 3000);
				GetGame().PreloadObject("Expansion_Bunker_Generator", 3000);
				#ifdef EXPANSIONMODTELEPORTER
				GetGame().PreloadObject("Expansion_Teleporter_Big", 3000);
				#endif
			}
			#endif

			m_ClientUpdateTimer = 0;
		}
	}

	//! @note: Not used yet
	protected void OnEVRStormBlowout()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (m_Expansion_ProcessedInfected < ZombieBase.s_Expansion_AllInfected.m_Count)
		{
			if (!s_EVRStormBlowoutInfectedSC)
  				s_EVRStormBlowoutInfectedSC = ScriptCaller.Create(OnEVRStormFinalBlowoutInfected);

			m_Expansion_ProcessedInfected += ZombieBase.s_Expansion_AllInfected.Each(s_EVRStormBlowoutInfectedSC, UPDATE_INFECTED_PER_TICK);
		}

		if (m_Expansion_ProcessedInfected >= ZombieBase.s_Expansion_AllInfected.m_Count)
		{
			m_EVRStormActive = false;
			m_EVRStormBlowout = false;
			m_Expansion_ProcessedInfected = 0;
		}
	}

	//! @note: Not used yet
	protected void OnEVRStormFinalBlowoutInfected(ZombieBase infected)
    {
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		//! Gamble if infected gets killed by EVR Storm blowout or not
		int gamble = Math.RandomIntInclusive(0, 2);
		if (gamble == 2)
			infected.SetHealth("", "", 0);
    }
	
	override int GetRPCMin()
	{
		return ExpansionNamalskModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionNamalskModuleRPC.COUNT;
	}

	override void OnRPC(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);

		super.OnRPC(sender, args);
		auto rpc = CF_EventRPCArgs.Cast(args);

		switch (rpc.ID)
		{
			case ExpansionNamalskModuleRPC.SendWorkbenchData:
			{
				RPC_SendWorkbenchData(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
			case ExpansionNamalskModuleRPC.SendWorkbenchResult:
			{
				RPC_SendWorkbenchResult(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
			case ExpansionNamalskModuleRPC.SetWorkbenchState:
			{
				RPC_SetWorkbenchState(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
		}
	}

	void SendWorkbenchData(Object target, PlayerIdentity identity)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		Expansion_3DPrinter printer;
		if (!Class.CastTo(printer, target))
		{
			Error(ToString() + "::SendWorkbenchData - Invalid target for repair center object!");
			return;
		}

		auto rpc = ExpansionScriptRPC.Create();
		rpc.Send(printer, ExpansionNamalskModuleRPC.SendWorkbenchData, true, identity);
	}

	protected void RPC_SendWorkbenchData(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
			return;

		Expansion_3DPrinter printer;
		if (!Class.CastTo(printer, target))
		{
			Error(ToString() + "::RPC_SendWorkbenchData - Invalid target for 3D printer object!");
			return;
		}

		m_WorkbenchMenuInvoker.Invoke(printer);
	}
	
	void SendWorkbenchResult(Expansion_3DPrinter printer, NA_WorkbenchRecipe recipe)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		int lowBitsGen, highBitsGen;
		printer.GetNetworkID(lowBitsGen, highBitsGen);
		
		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(lowBitsGen);
		rpc.Write(highBitsGen);
		recipe.OnSend(rpc);
		rpc.Send(printer, ExpansionNamalskModuleRPC.SendWorkbenchResult, true);
		
		EffectSound effect_confirm;
		effect_confirm = SEffectManager.PlaySound("printer_confirm_soundset", printer.GetPosition(), 0, 0, false);
		effect_confirm.SetParent(printer);
		effect_confirm.GetSoundObject().SetVolume(1.0);
		effect_confirm.SetSoundAutodestroy(true);
	}
	
	protected void RPC_SendWorkbenchResult(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
			return;
		
		int lowBitsGen;
		if (!ctx.Read(lowBitsGen))
		{
			Error(ToString() + "::RPC_SendWorkbenchResult - Couldn't read lowBitsGen");
			return;
		}

		int highBitsGen;
		if (!ctx.Read(highBitsGen))
		{
			Error(ToString() + "::RPC_SendWorkbenchResult - Couldn't read highBitsGen");
			return;
		}
		
		Expansion_3DPrinter printer;
		if (!Class.CastTo(printer, GetGame().GetObjectByNetworkId(lowBitsGen, highBitsGen)))
		{
			Error(ToString() + "::RPC_SendWorkbenchResult - Couldn't get 3D printer object!");
			return;
		}
				
		NA_WorkbenchRecipe recipe = new NA_WorkbenchRecipe();
		if (!recipe.OnRecieve(ctx))
		{
			Error(ToString() + "::RPC_SendWorkbenchResult - Couldn't get recipe data!");
			return;
		}
		
		//! Set recipe on printer object
		printer.SetRecipe(recipe);
		printer.ActivatePrinter();
		
		//! Finalize printing
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(printer.CompletePrinting, (printer.PRINTING_LENGTH * 1000));
	}

	void SetWorkbenchState(Expansion_3DPrinter printer, bool state)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		int lowBitsGen, highBitsGen;
		printer.GetNetworkID(lowBitsGen, highBitsGen);
		
		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(lowBitsGen);
		rpc.Write(highBitsGen);
		rpc.Write(state);
		rpc.Send(null, ExpansionNamalskModuleRPC.SetWorkbenchState, true);
	}
	
	protected void RPC_SetWorkbenchState(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
			return;
		
		int lowBitsGen;
		if (!ctx.Read(lowBitsGen))
		{
			Error(ToString() + "::RPC_SetWorkbenchState - Couldn't read lowBitsGen");
			return;
		}

		int highBitsGen;
		if (!ctx.Read(highBitsGen))
		{
			Error(ToString() + "::RPC_SetWorkbenchState - Couldn't read highBitsGen");
			return;
		}
		
		Expansion_3DPrinter printer;
		if (!Class.CastTo(printer, GetGame().GetObjectByNetworkId(lowBitsGen, highBitsGen)))
		{
			Error(ToString() + "::RPC_SetWorkbenchState - Couldn't get 3D printer object!");
			return;
		}
		
		bool state;
		if (!ctx.Read(state))
		{
			Error(ToString() + "::RPC_SetWorkbenchState - Couldn't read state");
			return;
		}
		
		printer.SetCanViewCargo(state);
	}
	
	ScriptInvoker GetWorkbenchMenuSI()
	{
		return m_WorkbenchMenuInvoker;
	}

	static ExpansionNamalskModule GetModuleInstance()
	{
		return s_ModuleInstance;
	}

	protected void ModuleDebugPrint(string text)
	{
		#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
		#endif
	}
};

/*
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
*/