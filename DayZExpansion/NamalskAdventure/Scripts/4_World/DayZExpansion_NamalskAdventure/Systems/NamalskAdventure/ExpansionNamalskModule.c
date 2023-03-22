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

#ifdef EXPANSION_NAMALSK_ADVENTURE
#ifdef NAMALSK_TERRAIN
[CF_RegisterModule(ExpansionNamalskModule)]
class ExpansionNamalskModule: CF_ModuleWorld
{
	protected static ExpansionNamalskModule s_ModuleInstance;
	static string s_dataFolderPath = "$mission:expansion\\namalsk_adventure\\";

	protected ref ExpansionNamalskAdventureData m_ServerData; //! Server

#ifdef EXPANSIONMODMARKET
	protected ref array<ref ExpansionMerchantPosition> m_MerchantPositions; //! Server
	protected ref array<ref ExpansionMerchantItemSet> m_MerchantItemSets; //! Server
#endif
#ifdef EXPANSIONMODNAVIGATION
	protected ExpansionMarkerData m_MerchantServerMarker;
#endif

	void ExpansionNamalskModule()
	{
		s_ModuleInstance = this;
	#ifdef EXPANSIONMODMARKET
		m_MerchantPositions = new array<ref ExpansionMerchantPosition>;
		m_MerchantItemSets = new array<ref ExpansionMerchantItemSet>;
	#endif
	}

	override void OnInit()
	{
		super.OnInit();

		EnableMissionStart();
		EnableMissionLoaded();
	}

	protected void CreateDirectoryStructure()
	{
		if (!FileExist(s_dataFolderPath))
			ExpansionStatic.MakeDirectoryRecursive(s_dataFolderPath);
	}

#ifdef EXPANSIONMODMARKET
	override void OnMissionStart(Class sender, CF_EventArgs args)
	{
		ModuleDebugPrint("::OnMissionStart - Start");

		super.OnMissionLoaded(sender, args);

		//! Server only
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			CreateDirectoryStructure();
			LoadNamalskAdventureServerData();
			SetMerchantData();
		}

		ModuleDebugPrint("::OnMissionStart - End");
	}
#endif

	protected void LoadNamalskAdventureServerData()
	{
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
		m_ServerData = ExpansionNamalskAdventureData.Load(path + fileName);
		if (!m_ServerData)
			Error(ToString() + "::GetServerData - Could not get namalsk adventure server data!");
	}

	protected void SetMerchantData()
	{
		//! Merchant positions
		m_MerchantPositions.Insert(new ExpansionMerchantPosition_1());
		m_MerchantPositions.Insert(new ExpansionMerchantPosition_2());
		m_MerchantPositions.Insert(new ExpansionMerchantPosition_3());
		m_MerchantPositions.Insert(new ExpansionMerchantPosition_4());
		m_MerchantPositions.Insert(new ExpansionMerchantPosition_5());

		//! Merchant item sets
		m_MerchantItemSets.Insert(new ExpansionMerchantItemSet_1());
	}

	protected ExpansionMerchantPosition GetMerchantPosition()
	{
		return m_MerchantPositions.GetRandomElement();
	}

	protected ExpansionMerchantItemSet GetMerchantItemSet()
	{
		return m_MerchantItemSets.GetRandomElement();
	}

	override void OnMissionLoaded(Class sender, CF_EventArgs args)
	{
		ModuleDebugPrint("::OnMissionLoaded - Start");

		super.OnMissionLoaded(sender, args);

		//! Server only
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
		#ifdef EXPANSIONMODAI
			Jalovisco_AI();
			A3_AI();
		#endif

		#ifdef EXPANSIONMODMARKET
			CreateMerchant();
		#endif
		}

		ModuleDebugPrint("::OnMissionLoaded - End");
	}

#ifdef EXPANSIONMODAI
	protected void Jalovisco_AI()
	{
		array<vector> waypoints = new array<vector>;
		string npcName = "Survivor Guard";
		//! Spawn AI guards
		waypoints = new array<vector>;
		waypoints.Insert(Vector(8597.78, 14.7325, 10529.7));
		SpawnSingleGuardAI(npcName, Vector(8597.78, 14.7325, 10529.7), Vector(215.0, 0, 0), waypoints, "GorkaLoadout", "Guards", 1.0, 1.0, 1.0, 1.0, false, true);

		waypoints = new array<vector>;
		waypoints.Insert(Vector(8619.11, 34.9968, 10479.6));
		SpawnSingleGuardAI(npcName, Vector(8619.11, 34.9968, 10479.6), Vector(335.0, 0, 0), waypoints, "GorkaLoadout", "Guards", 1.0, 1.0, 1.0, 1.0, false, true);

		waypoints = new array<vector>;
		waypoints.Insert(Vector(8634.89, 35.0096, 10512.5));
		SpawnSingleGuardAI(npcName, Vector(8634.89, 35.0096, 10512.5), Vector(280.0, 0, 0), waypoints, "GorkaLoadout", "Guards", 1.0, 1.0, 1.0, 1.0, false, true);

		waypoints = new array<vector>;
		waypoints.Insert(Vector(8566.56, 35.1376, 10554.2));
		SpawnSingleGuardAI(npcName, Vector(8566.56, 35.1376, 10554.2), Vector(150.0, 0, 0), waypoints, "GorkaLoadout", "Guards", 1.0, 1.0, 1.0, 1.0, false, true);

		waypoints = new array<vector>;
		waypoints.Insert(Vector(8539.49, 35.1852, 10512.8));
		SpawnSingleGuardAI(npcName, Vector(8539.49, 35.1852, 10512.8), Vector(100.0, 0, 0), waypoints, "GorkaLoadout", "Guards", 1.0, 1.0, 1.0, 1.0, false, true);

		waypoints = new array<vector>;
		waypoints.Insert(Vector(8583.78, 14.7504, 10496.2));
		SpawnSingleGuardAI(npcName, Vector(8583.78, 14.7504, 10496.2), Vector(35.0, 0, 0), waypoints, "GorkaLoadout", "Guards", 1.0, 1.0, 1.0, 1.0, false, true);

		waypoints = new array<vector>;
		waypoints.Insert(Vector(8618.76, 14.7886, 10516.2));
		SpawnSingleGuardAI(npcName, Vector(8618.76, 14.7886, 10516.2), Vector(330.0, 0, 0), waypoints, "GorkaLoadout", "Guards", 1.0, 1.0, 1.0, 1.0, false, true);

		waypoints = new array<vector>;
		waypoints.Insert(Vector(8559.18, 15.682, 10528.2));
		SpawnSingleGuardAI(npcName, Vector(8559.18, 15.682, 10528.2), Vector(345.0, 0, 0), waypoints, "GorkaLoadout", "Guards", 1.0, 1.0, 1.0, 1.0, false, true);

		waypoints = new array<vector>;
		waypoints.Insert(Vector(8578.85, 14.7807, 10489.5));
		SpawnSingleGuardAI(npcName, Vector(8578.85, 14.7807, 10489.5), Vector(230.0, 0, 0), waypoints, "GorkaLoadout", "Guards", 1.0, 1.0, 1.0, 1.0, false, true);
	}

	protected void A3_AI()
	{
		array<vector> waypoints = new array<vector>;
		//! Spawn NAC AI Soldier Units at A3
		waypoints.Insert(Vector(3601.81, 145.102, 6661.04));
		SpawnSingleAI(Vector(3601.81, 145.102, 6661.04), Vector(15.0, 0, 0), waypoints, "Namalsk_NAC", "NAC");

		waypoints = new array<vector>;
		waypoints.Insert(Vector(3596.59, 170.731, 6659.7));
		SpawnSingleAI(Vector(3596.59, 170.731, 6659.7), Vector(15.0, 0, 0), waypoints, "Namalsk_NAC", "NAC");

		waypoints = new array<vector>;
		waypoints.Insert(Vector(3605.11, 155.144, 6659.77));
		SpawnSingleAI(Vector(3605.11, 155.144, 6659.77), Vector(15.0, 0, 0), waypoints, "Namalsk_NAC", "NAC");

		waypoints = new array<vector>;
		waypoints.Insert(Vector(3591.66, 143.823, 6718.72));
		SpawnSingleAI(Vector(3591.66, 143.823, 6718.72), Vector(15.0, 0, 0), waypoints, "Namalsk_NAC", "NAC");
	}

	protected void SpawnSingleAI(vector pos, vector ori, array<vector> waypoints, string loadout, string factionName, float minSpeed = 1.0, float maxSpeed = 1.0, float minAccuracy = 0.5, float maxAccuracy = 1.0, bool canBeLooted = true, bool unlimtedReload = true)
	{
		pos = ExpansionAIPatrol.GetPlacementPosition(pos);

		eAIBase ai;
		if (!Class.CastTo(ai, GetGame().CreateObject(GetRandomAI(), pos)))
			return;

		ai.SetPosition(pos);
		ai.SetOrientation(ori);
		ExpansionHumanLoadout.Apply(ai, loadout, false);
		ai.SetMovementSpeedLimits(minSpeed, maxSpeed);
		ai.Expansion_SetCanBeLooted(canBeLooted);
		ai.eAI_SetUnlimitedReload(unlimtedReload);
		ai.eAI_SetAccuracy(minAccuracy, maxAccuracy);
		ai.eAI_SetThreatDistanceLimit(800);
		ai.eAI_SetDamageMultiplier(1.0);

		eAIGroup aiGroup;
		eAIFaction faction = eAIFaction.Create(factionName);
		if (!Class.CastTo(aiGroup, ai.GetGroup()))
		{
			if (!Class.CastTo(aiGroup, eAIGroup.CreateGroup(faction)))
				return;

			ai.SetGroup(aiGroup);
		}

		aiGroup.SetFaction(faction);
		aiGroup.SetFormation(eAIFormation.Create("Column"));
		aiGroup.SetWaypointBehaviour(eAIWaypointBehavior.ALTERNATE);

		for (int idx = 0; idx < waypoints.Count(); idx++)
		{
			aiGroup.AddWaypoint(waypoints[idx]);
			if (waypoints[idx] == pos)
				aiGroup.m_CurrentWaypointIndex = idx;
		}
	}

	protected void SpawnSingleGuardAI(string npcName, vector pos, vector ori, array<vector> waypoints, string loadout, string factionName, float minSpeed = 1.0, float maxSpeed = 1.0, float minAccuracy = 0.5, float maxAccuracy = 1.0, bool canBeLooted = true, bool unlimtedReload = true)
	{
		pos = ExpansionAIPatrol.GetPlacementPosition(pos);

		ExpansionGuardAI ai;
		if (!Class.CastTo(ai, GetGame().CreateObject(GetRandomGuardAI(), pos)))
			return;

		ai.m_Expansion_NetsyncData.Set(0, npcName);
		ai.m_Expansion_NetsyncData.Set(1, "{3364F58EF7F7FBE3}DayZExpansion/Core/GUI/icons/misc/T_Soldier_256x256.edds");
		ai.SetPosition(pos);
		ai.SetOrientation(ori);
		ExpansionHumanLoadout.Apply(ai, loadout, false);
		ai.SetMovementSpeedLimits(minSpeed, maxSpeed);
		ai.Expansion_SetCanBeLooted(canBeLooted);
		ai.eAI_SetUnlimitedReload(unlimtedReload);
		ai.eAI_SetAccuracy(minAccuracy, maxAccuracy);
		ai.eAI_SetThreatDistanceLimit(800);
		ai.eAI_SetDamageMultiplier(1.0);

		eAIGroup aiGroup;
		eAIFaction faction = eAIFaction.Create(factionName);
		if (!Class.CastTo(aiGroup, ai.GetGroup()))
		{
			if (!Class.CastTo(aiGroup, eAIGroup.CreateGroup(faction)))
				return;

			ai.SetGroup(aiGroup);
		}

		aiGroup.SetFaction(faction);
		aiGroup.SetFormation(eAIFormation.Create("Column"));
		aiGroup.SetWaypointBehaviour(eAIWaypointBehavior.ALTERNATE);

		for (int idx = 0; idx < waypoints.Count(); idx++)
		{
			aiGroup.AddWaypoint(waypoints[idx]);
			if (waypoints[idx] == pos)
				aiGroup.m_CurrentWaypointIndex = idx;
		}
	}
#endif

#ifdef EXPANSIONMODMARKET
	protected void CreateMerchant()
	{
		Print(ToString() + "::CreateMerchant - Start");
		ExpansionMarketSettings marketSettings = GetExpansionSettings().GetMarket();
		int findIndex = -1;

		array<int> usedMerchantPositions = m_ServerData.GetUsedMerchantPositions();
		array<int> usedMerchantSets = m_ServerData.GetUsedMerchantItemSets();

		ExpansionMerchantPosition positionToUse = GetMerchantPosition(); //! Get a random position for the merchant to spawn.
		int posIDToUse = positionToUse.GetID();

		//! Check if the random seleted position already got used in the previous sessions.
		findIndex = -1;
		findIndex = usedMerchantPositions.Find(posIDToUse);

		//! If the position got used we want to check for a valid position to use and if we already used all positions
		//! we reset the server data that holds that information and get a random position again.
		if (findIndex > -1)
		{
			Print(ToString() + "::CreateMerchant - Position with ID " + posIDToUse + " got already used in previous sessions. Check if we have still unused positions..");
			positionToUse = null;
			foreach (ExpansionMerchantPosition pos: m_MerchantPositions)
			{
				int posID = pos.GetID();
				Print(ToString() + "::CreateMerchant - Check if position with ID " + posID + " got used yet..");
				findIndex = -1;
				findIndex = usedMerchantPositions.Find(posID);
				if (findIndex == -1)
				{
					Print(ToString() + "::CreateMerchant - Position with ID " + posID + " got not used yet! Using position..");
					positionToUse = pos;
					break;
				}
				else
				{
					Print(ToString() + "::CreateMerchant - Position with ID " + posID + " got already used! Check next..");
				}
			}

			//! If we found no position to use we need to reset the server data vector array of the last used merchant positions.
			if (!positionToUse)
			{
				Print(ToString() + "::CreateMerchant - No unused positions left! Reset used positons..");
				m_ServerData.ResetUsedPositions();
				positionToUse = GetMerchantPosition(); //! Get a random position for the merchant to spawn.
			}
		}

		if (!positionToUse)
		{
			Error(ToString() + "::CreateMerchant - Could not get a valid position to spawn merchant!");
			return;
		}

		m_ServerData.GetUsedMerchantPositions().Insert(positionToUse.GetID());

		ExpansionMerchantItemSet itemSetToUse = GetMerchantItemSet(); //! Get a random set of items that the merchant will display.
		int itemSetIDToUse = itemSetToUse.GetID();

		//! Check if the random seleted set already got used in the previous sessions.
		findIndex = -1;
		findIndex = usedMerchantSets.Find(itemSetIDToUse);

		//! If the set got used we want to check for a valid set to use and if we already used all sets
		//! we reset the server data that holds that information and get a random set again.
		if (findIndex > -1)
		{
			Print(ToString() + "::CreateMerchant - Item set with ID " + itemSetIDToUse + " got already used in previous sessions. Check if we have still unused item sets..");
			itemSetToUse = null;
			foreach (ExpansionMerchantItemSet itemSet: m_MerchantItemSets)
			{
				int setID = itemSet.GetID();
				Print(ToString() + "::CreateMerchant - Check if item set with ID " + setID + " got used yet..");
				findIndex = -1;
				findIndex = usedMerchantSets.Find(setID);
				if (findIndex == -1)
				{
					Print(ToString() + "::CreateMerchant - Item set with ID " + setID + " got not used yet! Using Item set..");
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
				Print(ToString() + "::CreateMerchant - No unused item sets left! Reset used item sets..");
				m_ServerData.ResetUsedItemSets();
				itemSetToUse = GetMerchantItemSet(); //! Get a random set of items that the merchant will display.
			}
		}

		if (!itemSetToUse)
		{
			Error(ToString() + "::CreateMerchant - Could not get a valid item set for merchant!");
			return;
		}

		m_ServerData.GetUsedMerchantItemSets().Insert(itemSetToUse.GetID());
		m_ServerData.Save();

		ExpansionMarketTraderZone traderZone = new ExpansionMarketTraderZone();
		traderZone.m_DisplayName = "Merchant";
		traderZone.Position = positionToUse.GetPosition();
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

		map<string, int> items = itemSetToUse.GetItems();
		foreach (string className, int stock: items)
		{
			Print(ToString() + "::CreateMerchant - Add item to market zone and trader: " + className + " | Stock: " + stock);
			traderZone.AddStock(className, stock);
			trader.AddItem(className, ExpansionMarketTraderBuySell.CanOnlyBuy);
		}

		marketSettings.AddMarketZone(traderZone);
		marketSettings.AddMarketTrader(trader);

		Object obj = GetGame().CreateObject("ExpansionTraderAIMirek", positionToUse.GetPosition());
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
		aiTrader.SetPosition(positionToUse.GetPosition());
		aiTrader.SetOrientation(positionToUse.GetOrientation());

		traderBase.UpdateTraderZone();

	#ifdef EXPANSIONMODNAVIGATION
		m_MerchantServerMarker = ExpansionMarkerModule.GetModuleInstance().CreateServerMarker("Merchant", "Coins 2", positionToUse.GetPosition(), ARGB(255, 235, 59, 90), false);
	#endif

		Print(ToString() + "::CreateMerchant - End");
	}
#endif

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

	static ExpansionNamalskModule GetModuleInstance()
	{
		return s_ModuleInstance;
	}

	void ModuleDebugPrint(string text)
	{
		Print(ToString() + text);
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
#endif
#endif