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

class ExpansionNamalskQuestHolder
{
	int ID;
	string ClassName; 
	ref array<ref ExpansionNamalskQuestHolderPosition> Positions = new array<ref ExpansionNamalskQuestHolderPosition>;
	bool IsStatic;
	string Name;
	string DefaultText;
	int ObjectiveID;
	
	void ExpansionNamalskQuestHolder(int id, string className, int objectiveID, bool isStatic = true, string name = "Unknown", string text = "There is nothing to do here for you...")
	{
		ID = id;
		ClassName = className;
		ObjectiveID = objectiveID;
		IsStatic = isStatic;
		Name = name;
		DefaultText = text;
	}
	
	void AddPosition(ExpansionNamalskQuestHolderPosition pos)
	{
		Positions.Insert(pos);
	}
};

class ExpansionNamalskQuestHolderPosition
{
	vector Position;
	vector Orientation;
	
	void ExpansionNamalskQuestHolderPosition(vector pos, vector ori)
	{
		Position = pos;
		Orientation = ori;
	}
};

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
#ifdef EXPANSIONMODQUESTS
	protected ref map<eAIBase, ref array<vector>> m_QuestAI;
#endif
#endif

	protected typename m_LastNamalskEvent;

	protected bool m_EVRStormActive;
	protected bool m_EVRStormBlowout;

#ifdef EXPANSION_NAMALSK_ADVENTURE
	protected SV_Abandoned_Sattelite_Antenna m_AbdonedSatellite;
	protected Expansion_Teleporter_Big m_SatelliteTeleporter;
	protected Expansion_Satellite_Control m_SatelliteController;
	
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
	
	protected Expansion_Teleporter_Big m_A1BunkerTeleporter;
	protected Expansion_Bunker_Generator m_A1BungerGenerator;
	
	protected Land_Underground_Panel m_A1BunkerEntrancePanel;
	protected Land_Underground_Panel_Lever m_A1BunkerEntranceLeaver;
	protected Land_Underground_Panel_Lever m_A1BunkerFakeEntranceLeaver;
	
	protected ref array<ref ExpansionNamalskQuestHolder> m_QuestHolders;
	
	protected Land_Repair_Center m_SurvivorsRepairCenter;
#endif
	
	protected const float SUPPLY_CRATES_CHECK_TIME = 60.0; //! 1 minute tick
	protected float m_SupplyCratesCheckTimer;
	protected static ScriptCaller s_SupplyCratesCheckSC;
	static const int LOOT_DELAY_TIME = 300; //! 5 minute delay.
	
	protected int m_Expansion_ProcessedInfected;
	protected static ScriptCaller s_EVRStormBlowoutInfectedSC;
	protected const int UPDATE_INFECTED_PER_TICK = 5;  //! Refresh up to 5 infected every UPDATE_TICK_TIME interval
	
	protected ref map<int, ref ExpansionCommunityGoal> m_CommunityGoals;
	
	protected ref ScriptInvoker m_WorkbenckMenuInvoker; //! Client
	
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
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			CreateDirectoryStructure(); //! Create directoy structure if not existing.
			LoadNamalskAdventureServerData(); //! Load server data.
		}
	}
	
	override void OnMissionLoaded(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
			ServerModuleInit();

		if (GetGame().IsClient())
			ClientModuleInit();
	}
	
	protected void ServerModuleInit()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

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

		#ifdef EXPANSION_NAMALSK_ADVENTURE
			SpawnSatelliteAntennaObjects(); //! @note - Not finished yet!
			SpawnA1Bunker();
		#endif
			
			if (GetExpansionSettings().GetNamalskAdventure().EnableSupplyCrates)
				SpawnSupplyCrates();
		}
	}

	protected void ClientModuleInit()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (GetGame().IsClient())
		{
			m_WorkbenckMenuInvoker = new ScriptInvoker();
		}
	}

	protected void LoadNamalskAdventureServerData()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		bool fileExist = FileExist(s_dataFolderPath + "ServerData.json");
		if (fileExist)
		{
			GetServerData(s_dataFolderPath + "ServerData.json");
		}
		else
		{			
			m_ServerData = new ExpansionNamalskAdventureData();
			m_ServerData.Save();
			
			LoadCommunityGoals();
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
		
		LoadCommunityGoals();
	}
	
	protected void LoadCommunityGoals()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		m_CommunityGoals = new map<int, ref ExpansionCommunityGoal>;
		
		if (m_ServerData.GetCommunityGoalsData() && m_ServerData.GetCommunityGoalsData().Count() > 0)
		{
			array<ref ExpansionCommunityGoal> goals = m_ServerData.GetCommunityGoalsData();
			foreach (ExpansionCommunityGoal goalData: goals)
			{
			    m_CommunityGoals.Insert(goalData.GetID(), goalData);

				//! Fire event logic on load if flagged as finished
				if (goalData.IsFinished() || goalData.GetProgress() >= goalData.GetGoal())
				{
					OnCommunityGoalFinished(goalData.GetID(), goalData);
				}
			}
		}
		else
		{
			DefaultCommunityGoals();
		}
	}
	
	protected void DefaultCommunityGoals()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (!m_CommunityGoals)
			return;
		
		ExpansionCommunityGoal communityGoal = new ExpansionCommunityGoal(NamalskAdventureCommunityGoals.SURVIVORS_CAMP_SUPPLIES_1, 1);
		m_CommunityGoals.Insert(NamalskAdventureCommunityGoals.SURVIVORS_CAMP_SUPPLIES_1, communityGoal);
		m_ServerData.SetCommunityGoal(communityGoal);
		
		communityGoal = new ExpansionCommunityGoal(NamalskAdventureCommunityGoals.SURVIVORS_CAMP_REPAIR, 1);
		m_CommunityGoals.Insert(NamalskAdventureCommunityGoals.SURVIVORS_CAMP_REPAIR, communityGoal);
		m_ServerData.SetCommunityGoal(communityGoal);
	}
	
	void UpdateCommunityGoal(int id, int progress)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		ExpansionCommunityGoal communityGoal;
		if (!m_CommunityGoals.Find(id, communityGoal))
			return;
		
		communityGoal.AddProgress(progress);
				
		//! Update persistent community goal data.
		m_ServerData.SetCommunityGoal(communityGoal);

		//! Update module community goal data.
		m_CommunityGoals.Set(id, communityGoal);
		
		//! Events when community goal is reached.
		if (communityGoal.GetProgress() >= communityGoal.GetGoal())
		{
			OnCommunityGoalFinished(id, communityGoal);
		}
	}
	
	bool GetCommunityGoalState(int id)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		ExpansionCommunityGoal communityGoal;
		if (!m_CommunityGoals.Find(id, communityGoal))
			return false;
		
		return communityGoal.IsFinished();
	}
	
	protected void OnCommunityGoalFinished(int id, inout ExpansionCommunityGoal communityGoal)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		switch (id)
		{
		#ifdef EXPANSIONMODMARKET
			case NamalskAdventureCommunityGoals.SURVIVORS_CAMP_SUPPLIES_1:
			{
				auto traderObjects = ExpansionTraderObjectBase.GetAll();
				for (int i = 0; i < traderObjects.Count(); i++)
				{
					ExpansionTraderObjectBase traderObj = traderObjects[i];
					if (!traderObj)
					{
						Error(ToString() + "::OnCommunityGoalFinished - Could not get trader obj!");
						continue;
					}
										
					ExpansionMarketTraderZone traderZone = traderObj.GetTraderZone();
					if (!traderZone)
					{
						Error(ToString() + "::OnCommunityGoalFinished - Could not get trader zone!");
						continue;
					}
					
					//! Update survivor camp supplies (market zone).
					if (traderZone.m_FileName == "SurvivorsCamp")
					{
						ExpansionMarketTrader trader = traderObj.GetTraderMarket();
						if (!trader)
						{
							Error(ToString() + "::OnCommunityGoalFinished - Could not get trader!");
							continue;
						}
						
						traderZone.AddStock("bandagedressing", 100);
						traderZone.Save();
						
						ExpansionNotification(new StringLocaliser("Community Goal Reached"), new StringLocaliser("The Survivors faction has reached a community goal! The survivor camp trader has now new supplies!"), ExpansionIcons.GetPath("Info"), COLOR_EXPANSION_NOTIFICATION_MISSION, 7.0).Create();
					}
				}
			}
			break;
		#endif
			
			case NamalskAdventureCommunityGoals.SURVIVORS_CAMP_REPAIR:
			{
				if (!m_SurvivorsRepairCenter)
					m_SurvivorsRepairCenter = Land_Repair_Center.Cast(ExpansionWorldObjectsModule.SpawnObject("Land_Repair_Center", Vector(8615.047852, 17.235180, 10488.387695), Vector(117.000038, 0.000000, -0.000000), false, false));
				
				if (!m_SurvivorsRepairCenter)
				{
					Error(ToString() + "::OnCommunityGoalFinished - Could not create survivor camp repair center!");
					return;
				}
				
				m_SurvivorsRepairCenter.InitRepairBuilding();
			}
			break;
		}
		
		//! Set goal to finished so the system knows on server restarts if it should fire the goal event on load.
		communityGoal.SetFinished();
		
		//! Update persistent community goal data.
		m_ServerData.SetCommunityGoal(communityGoal);

		//! Update module community goal data.
		m_CommunityGoals.Set(id, communityGoal);
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

	/*#ifdef EXPANSIONMODNAVIGATION
		m_MerchantServerMarker = ExpansionMarkerModule.GetModuleInstance().CreateServerMarker("Merchant", "Coins 2", positionToUse.Position, ARGB(255, 15, 185, 177), false);
	#endif*/
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

	void SpawnSatelliteAntennaObjects()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		m_AbdonedSatellite = SV_Abandoned_Sattelite_Antenna.Cast(ExpansionWorldObjectsModule.SpawnObject("SV_Abandoned_Sattelite_Antenna", Vector(1202.799561, 14.207986, 11784.280273), Vector(81.209969, -0.000000, -0.000000), false, false));
		
	#ifdef EXPANSIONMODTELEPORTER
		m_SatelliteTeleporter = Expansion_Teleporter_Big.Cast(ExpansionWorldObjectsModule.SpawnObject("Expansion_Teleporter_Big", Vector(1200.880127, 4.619668, 11780.145508), Vector(-100.711388, -0.000000, -0.000000), false, false));
		if (m_SatelliteTeleporter)
		{
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
		}
	#endif
		
		//! @note: Don't spawn controller yet!
		m_SatelliteController = Expansion_Satellite_Control.Cast(ExpansionWorldObjectsModule.SpawnObject("Expansion_Satellite_Control", Vector(1204.062256, 5.146724, 11782.631836), Vector(171.544205, 0.000000, 0.000000), false, false));
		if (m_SatelliteController)
		{
			if (m_AbdonedSatellite)
				m_SatelliteController.SetLinkedSatellite(m_AbdonedSatellite);
		#ifdef EXPANSIONMODTELEPORTER
			if (m_SatelliteTeleporter)
				m_SatelliteController.SetLinkedTeleporter(m_SatelliteTeleporter);
		#endif
		}
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
		
		//! Teleporter
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
		
		//! Generator
		m_A1BungerGenerator = Expansion_Bunker_Generator.Cast(ExpansionWorldObjectsModule.SpawnObject("Expansion_Bunker_Generator", m_A1_Bunker_GeneratorPos, m_A1_Bunker_GeneratorOri, false, false));
		if (m_A1BungerGenerator)
		{
			if (m_A1BunkerTeleporter)
				m_A1BungerGenerator.SetLinkedTeleporter(m_A1BunkerTeleporter);
	
			Print(ToString() + "::SpawnA1Bunker - Spanwed A1 Bunker generator at position: " + m_A1BungerGenerator.GetPosition());
		}
		
	#ifdef EXPANSIONMODQUESTS
		//! Locker quest object - @note: Make this a config param class in the namalsk settings class you lazy ass!
		ExpansionNamalskQuestHolder questHolder = new ExpansionNamalskQuestHolder(1000, "ExpansionQuestObjectLocker", 1000, true, "Closed Locker");
		if (questHolder)
		{
			ExpansionNamalskQuestHolderPosition pos = new ExpansionNamalskQuestHolderPosition("1908.662354 201.666977 1244.743164", "173.734970 0.000000 -0.000000");
			questHolder.AddPosition(pos);
			pos = new ExpansionNamalskQuestHolderPosition("1915.156006 201.659302 1231.404419", "81.000038 -0.000000 -0.000000");
		questHolder.AddPosition(pos);
			pos = new ExpansionNamalskQuestHolderPosition("1899.852783 195.486664 1306.718994", "174.815613 0.000000 0.000000");
			questHolder.AddPosition(pos);
			pos = new ExpansionNamalskQuestHolderPosition("1906.268677 196.687729 1291.300415", "80.999977 -0.000000 -0.000000");
			questHolder.AddPosition(pos);
		
			if (!m_QuestHolders)
				m_QuestHolders = new array<ref ExpansionNamalskQuestHolder>;
	
			m_QuestHolders.Insert(questHolder);
			ExpansionNamalskQuestHolderPosition randomPos = questHolder.Positions.GetRandomElement();
			if (!randomPos)
				return;
			
			SpawnQuestHolder(questHolder, randomPos);
			
			//! Modify quest objective position
			ExpansionQuestObjectiveTravelConfig travelObjective = ExpansionQuestModule.GetModuleInstance().GetTravelObjectiveConfigByID(questHolder.ObjectiveID);
			if (!travelObjective)
				return;

			travelObjective.SetPosition(randomPos.Position);
		}
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

#ifdef EXPANSIONMODQUESTS
	protected void SpawnQuestHolder(ExpansionNamalskQuestHolder questHolder, ExpansionNamalskQuestHolderPosition randomPos)
	{
		TStringArray questNPCs = {"ExpansionQuestNPCBase"};
		TStringArray questAINPCs = {"ExpansionQuestNPCAIBase"};
		TStringArray questObject = {"ExpansionQuestStaticObject"};
		
		ExpansionQuestNPCData questNPCData = new ExpansionQuestNPCData;
		questNPCData.SetID(questHolder.ID); //! Unique NPC ID
		questNPCData.SetClassName(questHolder.ClassName); //! Class name of the NPC entity
		questNPCData.SetIsStatic(questHolder.IsStatic);
		questNPCData.SetNPCName(questHolder.Name);
		questNPCData.SetDefaultNPCText(questHolder.DefaultText);

		questNPCData.SetPosition(randomPos.Position); //! Quest NPC position
		questNPCData.SetOrientation(randomPos.Orientation); //! Quest NPC orientation
		
		ExpansionQuestModule.GetModuleInstance().AddQuestNPCData(questHolder.ID, questNPCData);
		
		if (ExpansionStatic.IsAnyOf(questHolder.ClassName, questObject))
		{
			ExpansionQuestStaticObject object = SpawnQuestObject(questNPCData);
			if (object)
			{
				object.SetQuestNPCID(questHolder.ID);
				object.SetQuestNPCData(questNPCData);
				ExpansionQuestModule.GetModuleInstance().AddStaticQuestObject(questHolder.ID, object);
				
				Print(ToString() + "::SpawnQuestHolder - A1 Bunker quest holder spawned at possition: " + object.GetPosition());
			}
		}
		else if (ExpansionStatic.IsAnyOf(questHolder.ClassName, questNPCs))
		{
			ExpansionQuestNPCBase questNPC = SpawnQuestNPC(questNPCData);
			if (questNPC)
			{
				questNPC.SetQuestNPCID(questHolder.ID);
				questNPC.SetQuestNPCData(questNPCData);
				ExpansionQuestModule.GetModuleInstance().AddQuestNPC(questHolder.ID, questNPC);
				
				Print(ToString() + "::SpawnQuestHolder - A1 Bunker quest holder spawned at possition: " + questNPC.GetPosition());
			}
		}
	#ifdef EXPANSIONMODAI
		else if (ExpansionStatic.IsAnyOf(questHolder.ClassName, questAINPCs))
		{
			ExpansionQuestNPCAIBase questNPCAI = SpawnQuestNPCAI(questNPCData);
			if (questNPCAI)
			{
				questNPCAI.SetQuestNPCID(questHolder.ID);
				questNPCAI.SetQuestNPCData(questNPCData);
				ExpansionQuestModule.GetModuleInstance().AddQuestNPCAI(questHolder.ID, questNPCAI);
							
				Print(ToString() + "::SpawnQuestHolder - A1 Bunker quest holder spawned at possition: " + questNPCAI.GetPosition());
			}
		}
	#endif
	}
	
	ExpansionQuestStaticObject SpawnQuestObject(ExpansionQuestNPCData questNPCData)
	{
	    Object obj = GetGame().CreateObjectEx(questNPCData.GetClassName(), questNPCData.GetPosition(), ECE_KEEPHEIGHT | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS | ECE_NOLIFETIME);
	    ExpansionQuestStaticObject questObject;
	    if (!ExpansionQuestStaticObject.CastTo(questObject, obj))
	    {
			GetGame().ObjectDelete(obj);
	        return null;
	    }

	    questObject.SetPosition(questNPCData.GetPosition());
	    questObject.SetOrientation(questNPCData.GetOrientation());
		
		if (questNPCData.GetNPCName() != string.Empty)
	    	questObject.m_Expansion_NetsyncData.Set(0, questNPCData.GetNPCName());

	    return questObject;
	}
	
	ExpansionQuestNPCBase SpawnQuestNPC(ExpansionQuestNPCData questNPCData)
	{
		Object obj = GetGame().CreateObject(questNPCData.GetClassName(), questNPCData.GetPosition());
 		ExpansionQuestNPCBase questNPC;
		if (!ExpansionQuestNPCBase.CastTo(questNPC, obj))
	    {
			GetGame().ObjectDelete(obj);
	        return null;
	    }

	    questNPC.SetPosition(questNPCData.GetPosition());
	    questNPC.SetOrientation(questNPCData.GetOrientation());
		
		if (questNPCData.GetNPCName() != string.Empty)
			questNPC.m_Expansion_NetsyncData.Set(0, questNPCData.GetNPCName());
		
		if (questNPCData.GetLoadoutFile() != string.Empty)
			ExpansionHumanLoadout.Apply(questNPC, questNPCData.GetLoadoutFile(), false);

		return questNPC;
	}

#ifdef EXPANSIONMODAI
	ExpansionQuestNPCAIBase SpawnQuestNPCAI(ExpansionQuestNPCData questNPCData)
	{
		vector position = ExpansionAIPatrol.GetPlacementPosition(questNPCData.GetPosition());
		
		Object obj = GetGame().CreateObject(questNPCData.GetClassName(), position);
		if (!obj)
			return null;

		ExpansionQuestNPCAIBase questNPC = ExpansionQuestNPCAIBase.Cast(obj);
		if (!questNPC)
		{
			GetGame().ObjectDelete(obj);
			return null;
		}

		questNPC.SetPosition(position);
		questNPC.SetOrientation(questNPCData.GetOrientation());
		questNPC.m_Expansion_NetsyncData.Set(0, questNPCData.GetNPCName());
		ExpansionHumanLoadout.Apply(questNPC, questNPCData.GetLoadoutFile(), false);
		questNPC.Expansion_SetCanBeLooted(false);
		questNPC.eAI_SetUnlimitedReload(true);
		questNPC.eAI_SetAccuracy(1.0, 1.0);
		questNPC.eAI_SetThreatDistanceLimit(800);

		eAIGroup aiGroup = questNPC.GetGroup();

		if (questNPCData.GetFaction() != string.Empty)
		{
			eAIFaction faction = eAIFaction.Create(questNPCData.GetFaction());
			if (faction && aiGroup.GetFaction().Type() != faction.Type())
				aiGroup.SetFaction(faction);
		}
		
		aiGroup.SetFormation(new eAIFormationColumn(aiGroup));
		aiGroup.SetWaypointBehaviour(eAIWaypointBehavior.ALTERNATE);
		
		array<vector> waypoints = questNPCData.GetWaypoints();
		for (int idx = 0; idx < waypoints.Count(); idx++)
		{
			aiGroup.AddWaypoint(waypoints[idx]);
			if (waypoints[idx] == position)
				aiGroup.m_CurrentWaypointIndex = idx;
		}
		
		return questNPC;
	}
#endif
#endif
	
	protected void SpawnSupplyCrates()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		//! Supply crates
		array<ref ExpansionSupplyCrateSetup> supplyCrateSpawns = GetExpansionSettings().GetNamalskAdventure().GetSupplyCrateSpawns();
		foreach (ExpansionSupplyCrateSetup supplyCrate: supplyCrateSpawns)
		{
			Object obj = GetGame().CreateObjectEx(supplyCrate.ClassName, supplyCrate.Position, ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS | ECE_NOLIFETIME);
			Expansion_SupplyCrate_Base supplyCareObj = Expansion_SupplyCrate_Base.Cast(obj);
			if (!supplyCareObj)
			{
				Error(ToString() + "::OnMissionLoaded - Could not spawn supply crate object!");
				GetGame().ObjectDelete(obj);
				continue;
			}
			
			supplyCareObj.SetPosition(supplyCrate.Position);
			supplyCareObj.SetOrientation(supplyCrate.Orientation);
			supplyCareObj.SetCrateLoot(supplyCrate.CrateLoot);
			supplyCareObj.Update();
		}
	}

	static ExpansionNamalskModule GetModuleInstance()
	{
		return s_ModuleInstance;
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
	
	protected void OnUpdateServer(float deltaTime)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE		
		m_SatelliteCryTimer += deltaTime;
		if (m_SatelliteCryTimer >= SATELLITE_CRY_TIME)
		{
			PlaySatelliteCrySFX();
			m_SatelliteCryTimer = 0;
		}
		
		m_BunkerGeneratorsCheckTimer += deltaTime;
		if (m_BunkerGeneratorsCheckTimer >= BUNKER_GENERATORS_CHECK_TIME)
		{
			if (!s_BunkerGeneratorCheckSC)
		    	s_BunkerGeneratorCheckSC = ScriptCaller.Create(OnBunkerGeneratorCheck);

			m_CurrentCheckTime = CF_Date.Now(true).GetTimestamp();

			Expansion_Bunker_Generator.s_Expansion_AllBunkerGenerators.Each(s_BunkerGeneratorCheckSC);
			m_BunkerGeneratorsCheckTimer = 0;
		}
	#endif
		
		m_SupplyCratesCheckTimer += deltaTime;
		if (m_SupplyCratesCheckTimer >= SUPPLY_CRATES_CHECK_TIME)
		{
			if (!s_SupplyCratesCheckSC)
		    	s_SupplyCratesCheckSC = ScriptCaller.Create(OnSupplyCrateCheck);

			m_CurrentCheckTime = CF_Date.Now(true).GetTimestamp();
			
			Expansion_SupplyCrate_Base.s_Expansion_AllSupplyCrates.Each(s_SupplyCratesCheckSC);
			m_SupplyCratesCheckTimer = 0;
		}
		
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
	
#ifdef EXPANSION_NAMALSK_ADVENTURE
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
	
	void OnEVRStormFinalBlowoutInfected(ZombieBase infected)
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
		}
	}

	void SendWorkbenchData(Object target, PlayerIdentity identity)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::SendWorkbenchData - Tried to call SendWorkbenchData on Client!");
			return;
		}

		Land_Repair_Center repair_center;
		if (!Class.CastTo(repair_center, target))
		{
			Error(ToString() + "::SendWorkbenchData - Invalid target for repair center object!");
			return;
		}
		
		RepairBenchGenerator repair_generator;
		if (!Class.CastTo(repair_generator, repair_center.GetGenerator()))
		{
			Error(ToString() + "::SendWorkbenchData - Could not get repair center generator!");
			return;
		}
		
		int lowBitsGen, highBitsGen;
		repair_generator.GetNetworkID(lowBitsGen, highBitsGen);
		
		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(lowBitsGen);
		rpc.Write(highBitsGen);
		rpc.Send(repair_center, ExpansionNamalskModuleRPC.SendWorkbenchData, true, identity);
	}

	protected void RPC_SendWorkbenchData(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
			return;

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RPC_SendWorkbenchData - Tried to call RPC_SendWorkbenchData on Server!");
			return;
		}
		
		int lowBitsGen;
		if (!ctx.Read(lowBitsGen))
		{
			Error(ToString() + "::RPC_SendWorkbenchData - couldn't read lowBitsGen");
			return;
		}
		
		int highBitsGen;
		if (!ctx.Read(highBitsGen))
		{
			Error(ToString() + "::RPC_SendWorkbenchData - couldn't read highBitsGen");
			return;
		}
					
		RepairBenchGenerator repair_generator;
		if (!Class.CastTo(repair_generator, GetGame().GetObjectByNetworkId(lowBitsGen, highBitsGen)))
		{
			Error(ToString() + "::RPC_SendWorkbenchData - Could not get repair center generator!");
			return;
		}
			
		Land_Repair_Center repair_center;
		if (!Class.CastTo(repair_center, target))
		{
			Error(ToString() + "::RPC_SendWorkbenchData - Invalid target for repair center object!");
			return;
		}
		
		m_WorkbenckMenuInvoker.Invoke(repair_center, repair_generator);
	}
	
	ScriptInvoker GetWorkbenchMenuSI()
	{
		return m_WorkbenckMenuInvoker;
	}
	
	protected void OnUpdateClient(float deltaTime)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE
		m_ClientUpdateTimer += deltaTime;
		if (m_ClientUpdateTimer >= CLIENT_UPDATE_TIME && GetGame().GetPlayer())
		{
			float dist = vector.Distance(m_A1_Bunker_TeleporterPos, GetGame().GetPlayer().GetPosition());
			if (dist < 200) 
			{
				Print(ToString() + "::OnUpdateClient - Preload objects for player: UID: " + GetGame().GetPlayer().GetIdentity().GetId() + " | Name: " + GetGame().GetPlayer().GetIdentity().GetName());
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
				GetGame().PreloadObject("Expansion_Teleporter_Big", 3000);
				GetGame().PreloadObject("Expansion_Bunker_Generator", 3000);
			}
			
			m_ClientUpdateTimer = 0;
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