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

#ifdef EXPANSIONMODAI
	protected ref map<eAIBase, ref array<vector>> m_SpawnedAI;
	protected int m_AISpeed;
#endif
#ifdef EXPANSIONMODQUESTS
	protected ref map<eAIBase, ref array<vector>> m_QuestAI;
#endif
	protected bool m_AIBuildingPositionsSet;

	protected typename m_LastNamalskEvent;

	protected bool m_EVRStormActive;
	protected bool m_EVRStormBlowout;

#ifdef EXPANSION_NAMALSK_ADVENTURE_SURVIVAL
	protected SV_Abandoned_Sattelite_Antenna m_AbdonedSatellite;
	protected Expansion_Teleporter_Big m_SatelliteTeleporter;
	protected Expansion_Satellite_Control m_SatelliteController;
	
	protected const float SATELLITE_CRY_TIME = 300.0;
	protected float m_SatelliteCryTimer;
	protected bool m_SatelitteState;
#endif
	
	void ExpansionNamalskModule()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		s_ModuleInstance = this;

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
		EnableUpdate();
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
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			CreateDirectoryStructure(); //! Create directoy structure if not existing.
			LoadNamalskAdventureServerData(); //! Load server data.
		}
	}

	protected void LoadNamalskAdventureServerData()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		array<string> files = ExpansionStatic.FindFilesInLocation(s_dataFolderPath, ".json");
		if (files && files.Count() > 0)
		{
			for (int i = 0; i < files.Count(); i++)
			{
				string fileName = files[i];
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
		#ifdef EXPANSIONMODAI
			if (GetExpansionSettings().GetNamalskAdventure().EnableAISpawns)
				SpawnAI();
		#endif
		#ifdef EXPANSIONMODMARKET
			if (GetExpansionSettings().GetNamalskAdventure().EnableMerchant)
				CreateMerchant();
		#endif
			
		#ifdef EXPANSION_NAMALSK_ADVENTURE_SURVIVAL
			SpawnSatelliteAntennaObjects();
		#endif
		}
	}

#ifdef EXPANSIONMODAI
	//! @note: Spawns all configured AI units from the NamalskAdventureSettings class.
	protected void SpawnAI()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		for (int i = 0; i < GetExpansionSettings().GetNamalskAdventure().AISpawnPositions.Count(); i++)
		{
			ExpansionAISpawnPosition aiSpawn = GetExpansionSettings().GetNamalskAdventure().AISpawnPositions[i];
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

		if (aiSpawn.ShelterPositions)
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
			
			SetSatelitteActive(false);
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
		
		SetSatelitteActive(true);
		
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
		m_EVRStormActive = false;
		m_EVRStormBlowout = false;

		SetSatelitteActive(false);
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
	   		
			aiGroup.ClearWaypoints();
	   		
			aiGroup.AddWaypoint(shelterPositions.GetRandomElement());
			
			aiGroup.m_CurrentWaypointIndex = 0;
			ai.SetMovementSpeedLimit(eAIMovementSpeed.JOG);
			m_AISpeed = ai.GetMovementSpeedLimit();
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

#ifdef EXPANSION_NAMALSK_ADVENTURE_SURVIVAL
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

	void SpawnSatelliteAntennaObjects()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		Object obj = GetGame().CreateObject("SV_Abandoned_Sattelite_Antenna", Vector(1202.799561, 14.207986, 11784.280273));
		m_AbdonedSatellite = SV_Abandoned_Sattelite_Antenna.Cast(obj);
		if (!m_AbdonedSatellite)
		{
			Error(ToString() + "::SpawnSatelliteAntennaObjects - Could not spawn antenna object!");
			GetGame().ObjectDelete(obj);
			return;
		}
		
		m_AbdonedSatellite.SetPosition(Vector(1202.799561, 14.207986, 11784.280273));
		m_AbdonedSatellite.SetOrientation(Vector(81.209969, -0.000000, -0.000000));
		
	#ifdef EXPANSIONMODTELEPORTER
		obj = GetGame().CreateObject("Expansion_Teleporter_Big", Vector(1200.880127, 4.619668, 11780.145508));
		m_SatelliteTeleporter = Expansion_Teleporter_Big.Cast(obj);
		if (!m_SatelliteTeleporter)
		{
			Error(ToString() + "::SpawnSatelliteAntennaObjects - Could not spawn antenna teleporter object!");
			GetGame().ObjectDelete(obj);
			return;
		}
		
		m_SatelliteTeleporter.SetPosition(Vector(1200.880127, 4.619668, 11780.145508));
		m_SatelliteTeleporter.SetOrientation(Vector(-100.711388, -0.000000, -0.000000));
		m_SatelliteTeleporter.SetTeleporterID(100);
		m_SatelliteTeleporter.SetActive(false);
		
		ExpansionTeleportData teleporterData = new ExpansionTeleportData();
		teleporterData.SetID(100);
		teleporterData.SetDisplayName("???");
		teleporterData.SetObjectPosition(Vector(1200.880127, 4.619668, 11780.145508));

		ExpansionTeleportPosition teleportPos = new ExpansionTeleportPosition();
		teleportPos.SetData("???");
		teleportPos.AddPosition(Vector(5079.959961, 2085.610107, 11720.700195), Vector(39.183594, 0.000000, 0.000000));
		teleportPos.AddPosition(Vector(5075.359863, 2085.610107, 11715.299805), Vector(132.597000, 0.000000, 0.000000));

		teleporterData.AddTeleportPosition(teleportPos);
		
		ExpansionTeleporterModule.GetModuleInstance().AddTeleporterData(teleporterData);
	#endif

		obj = GetGame().CreateObject("Expansion_Satellite_Control", Vector(1204.062256, 5.146724, 11782.631836));
		m_SatelliteController = Expansion_Satellite_Control.Cast(obj);
		if (!m_SatelliteController)
		{
			Error(ToString() + "::SpawnSatelliteAntennaObjects - Could not spawn antenna teleporter object!");
			GetGame().ObjectDelete(obj);
			return;
		}
		
		m_SatelliteController.SetPosition(Vector(1204.062256, 5.146724, 11782.631836));
		m_SatelliteController.SetOrientation(Vector(171.544205, 0.000000, 0.000000));
		if (m_AbdonedSatellite)
			m_SatelliteController.SetSatelliteLink(m_AbdonedSatellite);
	#ifdef EXPANSIONMODTELEPORTER
		if (m_SatelliteTeleporter)
			m_SatelliteController.SetTeleporterLink(m_SatelliteTeleporter);
	#endif
	}
	
	void PlaySatelliteCrySFX()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		string soundSet = "Expansion_Satellite_Cry_Distance_SoundSet";
		Param3<bool, vector, int> satelliteCry = new Param3<bool, vector, int>(true, Vector(1202, 14, 11784), soundSet.Hash());
		GetGame().RPCSingleParam(null, ERPCs.RPC_SOUND_HELICRASH, satelliteCry, true);
	}
	
	void SetSatelitteActive(bool state)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (state == m_SatelitteState)
			return;
				
		if (state)
		{
			ActivateSatellite();
		}
		else
		{
			DeactivateSatellite();
		}
		
		m_SatelitteState = state;
	}
	
	void ActivateSatellite()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (!m_AbdonedSatellite)
			return;
		
		m_SatelliteController.SetActivateState(true);
		m_AbdonedSatellite.SetSatelliteActive(true);
	}
	
	void DeactivateSatellite()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (!m_AbdonedSatellite)
			return;
		
		if (!m_SatelliteController.IsActive())
		{
			m_SatelliteController.SetActivateState(false);
			m_AbdonedSatellite.SetSatelliteActive(false);
		}
	}
	
	SV_Abandoned_Sattelite_Antenna GetSatelliteAntenna()
	{
		return m_AbdonedSatellite;
	}

	Expansion_Teleporter_Big GetSatelliteTeleporter()
	{
		return m_SatelliteTeleporter;
	}
	
	Expansion_Satellite_Control GetSatelliteController()
	{
		return m_SatelliteController;
	}
#endif

	static ExpansionNamalskModule GetModuleInstance()
	{
		return s_ModuleInstance;
	}
	
	override void OnUpdate(Class sender, CF_EventArgs args)
	{
		super.OnUpdate(sender, args);

		if (!GetGame().IsServer())
			return;
		
		auto update = CF_EventUpdateArgs.Cast(args);
		
	#ifdef EXPANSION_NAMALSK_ADVENTURE_SURVIVAL
		m_SatelliteCryTimer += update.DeltaTime;
		if (m_SatelliteCryTimer >= SATELLITE_CRY_TIME)
		{
			PlaySatelliteCrySFX();
			m_SatelliteCryTimer = 0;
		}
	#endif
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