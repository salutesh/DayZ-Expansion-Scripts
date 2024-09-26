/**
 * ExpansionP2PMarketTraderConfig.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionP2PMarketTraderConfigBase
{
	int m_Version;
	int m_TraderID;
	string m_ClassName;
	vector m_Position;
	vector m_Orientation;
	string m_LoadoutFile;
	vector m_VehicleSpawnPosition;
	vector m_WatercraftSpawnPosition;
	vector m_AircraftSpawnPosition;
};

class ExpansionP2PMarketTraderConfigV1: ExpansionP2PMarketTraderConfigBase
{
	autoptr array<ref ExpansionP2PMarketListing> m_Listings = new array<ref ExpansionP2PMarketListing>;
};

class ExpansionP2PMarketTraderConfig: ExpansionP2PMarketTraderConfigBase
{
	static const int VERSION = 8;

	string m_DisplayName = "Unknown";
	string m_DisplayIcon = "Deliver";

	// AI
	string m_Faction = "InvincibleObservers";
	ref array<vector> m_Waypoints;
	int m_EmoteID = EmoteConstants.ID_EMOTE_WATCHING;
	bool m_EmoteIsStatic;
	string m_RequiredFaction;

	// Hardline
	bool m_UseReputation;
	int m_MinRequiredReputation;
	int m_MaxRequiredReputation;

	// Quests
	int m_RequiredCompletedQuestID;

	bool m_IsGlobalTrader;
	
	autoptr TStringArray m_Currencies;
	int m_DisplayCurrencyValue = 1;
	string m_DisplayCurrencyName;

	[NonSerialized()]
	int m_DisplayCurrencyPrecision;

	void ExpansionP2PMarketTraderConfig()
	{
		m_Version = VERSION;
	#ifdef EXPANSIONMODAI
		m_Waypoints = new array<vector>;
	#endif
		m_Currencies = new TStringArray; 
	}

	void CopyFromBaseClass(ExpansionP2PMarketTraderConfigBase base)
	{
		m_TraderID = base.m_TraderID;
		m_ClassName = base.m_ClassName;
		m_Position = base.m_Position;
		m_Orientation = base.m_Orientation;
		m_LoadoutFile = base.m_LoadoutFile;
		m_VehicleSpawnPosition = base.m_VehicleSpawnPosition;
		m_WatercraftSpawnPosition = base.m_WatercraftSpawnPosition;
		m_AircraftSpawnPosition = base.m_AircraftSpawnPosition;
	}

	void SetID(int id)
	{
		m_TraderID = id;
	}

	int GetID()
	{
		return m_TraderID;
	}

	void SetClassName(string className)
	{
		m_ClassName = className;
	}

	string GetClassName()
	{
		return m_ClassName;
	}

	void SetPosition(vector pos)
	{
		m_Position = pos;
	}

	void SetOrientation(vector ori)
	{
		m_Orientation = ori;
	}

	void SetLoadoutFile(string loadoutFile)
	{
		m_LoadoutFile = loadoutFile;
	}

	vector GetVehicleSpawnPosition()
	{
		return m_VehicleSpawnPosition;
	}

	void SetVehicleSpawnPosition(vector pos)
	{
		m_VehicleSpawnPosition = pos;
	}

	vector GetAircraftSpawnPosition()
	{
		return m_AircraftSpawnPosition;
	}

	void SetAircraftSpawnPosition(vector pos)
	{
		m_AircraftSpawnPosition = pos;
	}

	vector GetWatercraftSpawnPosition()
	{
		return m_WatercraftSpawnPosition;
	}

	void SetWatercraftSpawnPosition(vector pos)
	{
		m_WatercraftSpawnPosition = pos;
	}

	vector GetOrientation()
	{
		return m_Orientation;
	}

	string GetDisplayName()
	{
		return m_DisplayName;
	}

	string GetDisplayIcon()
	{
		return m_DisplayIcon;
	}

#ifdef EXPANSIONMODAI
	string GetFaction()
	{
		return m_Faction;
	}
	
	array<vector> GetWaypoints()
	{
		return m_Waypoints;
	}
	
	int GetEmoteID()
	{
		return m_EmoteID;
	}
	
	bool IsEmoteStatic()
	{
		return m_EmoteIsStatic;
	}
#endif

	bool IsGlobalTrader()
	{
		return m_IsGlobalTrader;
	}
	
	void AddCurrency(string currency)
	{
		currency.ToLower();
		if (m_Currencies.Find(currency) == -1)
			m_Currencies.Insert(currency);
	}
	
	TStringArray GetCurrencies()
	{
		return m_Currencies;
	}
	
	static ExpansionP2PMarketTraderConfig Load(string fileName)
	{
		CF_Log.Info("[ExpansionP2PMarketTraderConfig] Attempting to load P2P market listing file:" + fileName);

		ExpansionP2PMarketTraderConfig traderConfig;
		if (!ExpansionJsonFileParser<ExpansionP2PMarketTraderConfig>.Load(fileName, traderConfig))
			return NULL;

		if (traderConfig.m_Version < VERSION)
		{
			if (traderConfig.m_Version < 2)
			{
				ExpansionP2PMarketTraderConfigV1 traderConfigV1;
				if (!ExpansionJsonFileParser<ExpansionP2PMarketTraderConfigV1>.Load(fileName, traderConfigV1))
					return NULL;

				array<ref ExpansionP2PMarketListing> listings = traderConfigV1.m_Listings;
				foreach (ExpansionP2PMarketListing listing: listings)
				{
					listing.m_Version = ExpansionP2PMarketListing.VERSION;
					listing.SetTraderID(1);
					ExpansionP2PMarketModule.GetModuleInstance().AddListing(1, listing);
					listing.Save();
				}
			}
			
			if (traderConfig.m_Version < 6 && !traderConfig.m_Currencies.Count())
				traderConfig.AddCurrency("expansionbanknotehryvnia");

			if (traderConfig.m_Version < 7 && !traderConfig.m_DisplayCurrencyValue)
				traderConfig.m_DisplayCurrencyValue = 1;


			if (traderConfig.m_Version < 8)
			{
				traderConfig.m_RequiredFaction = "";
				traderConfig.m_UseReputation = false;
				traderConfig.m_MinRequiredReputation = 0;
				traderConfig.m_MaxRequiredReputation = int.MAX;
				traderConfig.m_RequiredCompletedQuestID = -1;
			}
			
			traderConfig.m_Version = VERSION;
		
			Save(traderConfig);
		}

		traderConfig.m_Currencies = ExpansionMarketSettings.StringArrayToLower(traderConfig.m_Currencies);
		traderConfig.m_DisplayCurrencyPrecision = ExpansionStatic.GetPrecision(traderConfig.m_DisplayCurrencyValue);

		return traderConfig;
	}

	void Save()
	{
		Save(this);
	}

	static void Save(ExpansionP2PMarketTraderConfig traderConfig)
	{
		ExpansionJsonFileParser<ExpansionP2PMarketTraderConfig>.Save(ExpansionP2PMarketModule.s_P2PMarketConfigFolderPath + "P2PTrader_" + traderConfig.m_TraderID + ".json", traderConfig);
	}

	void Spawn()
	{
		Object obj = ExpansionGame.CreateObjectExSafe(m_ClassName, m_Position, ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS | ECE_NOLIFETIME);

	#ifdef ENFUSION_AI_PROJECT
		if (!obj || (!obj.IsInherited(ExpansionP2PMarketTraderStatic) && !obj.IsInherited(ExpansionP2PMarketTraderNPC) && !obj.IsInherited(ExpansionP2PMarketTraderNPCAI)))
	#else
		if (!obj || (!obj.IsInherited(ExpansionP2PMarketTraderStatic) && !obj.IsInherited(ExpansionP2PMarketTraderNPC)))
	#endif
		{
			if (obj)
				GetGame().ObjectDelete(obj);
			Error(ToString() + "::Spawn - Tried to spawn P2P market trader with unsuported type name: " + m_ClassName);
			return;
		}

		auto object = ExpansionP2PMarketTraderStatic.Cast(obj);
		auto npc = ExpansionP2PMarketTraderNPC.Cast(obj);
	#ifdef ENFUSION_AI_PROJECT
		auto npcAI = ExpansionP2PMarketTraderNPCAI.Cast(obj);
	#endif

	#ifdef ENFUSION_AI_PROJECT
		if (!object && !npc && !npcAI)
	#else
		if (!object && !npc)
	#endif
		{
			GetGame().ObjectDelete(obj);
			Error(ToString() + "::Spawn - Could not spawn NPC: " + m_ClassName);
			return;
		}

		obj.SetOrientation(m_Orientation);
		obj.Update();

		if (object)
		{
			object.SetP2PTraderID(m_TraderID);
			object.m_Expansion_NetsyncData.Set(0, m_DisplayName);
			object.m_Expansion_NetsyncData.Set(1, m_DisplayIcon);
		}
		else if (npc)
		{
			npc.SetP2PTraderID(m_TraderID);
			ExpansionHumanLoadout.Apply(npc, m_LoadoutFile, false);
			npc.m_Expansion_NetsyncData.Set(0, m_DisplayName);
			npc.m_Expansion_NetsyncData.Set(1, m_DisplayIcon);
		}
	#ifdef EXPANSIONMODAI
		else if (npcAI)
		{
			npcAI.SetP2PTraderID(m_TraderID);
			ExpansionHumanLoadout.Apply(npcAI, m_LoadoutFile, false);
			npcAI.m_Expansion_NetsyncData.Set(0, m_DisplayName);
			npcAI.m_Expansion_NetsyncData.Set(1, m_DisplayIcon);

			npcAI.Expansion_SetCanBeLooted(false);
			npcAI.eAI_SetUnlimitedReload(true);
			npcAI.eAI_SetAccuracy(1.0, 1.0);
			npcAI.eAI_SetThreatDistanceLimit(800);
			npcAI.Expansion_SetEmote(m_EmoteID, !m_EmoteIsStatic);
			
			eAIGroup aiGroup = npcAI.GetGroup();
			if (m_Faction != string.Empty)
			{
				eAIFaction faction = eAIFaction.Create(m_Faction);
				if (faction && aiGroup.GetFaction().Type() != faction.Type())
					aiGroup.SetFaction(faction);
			}
			
			aiGroup.SetFormation(new eAIFormationColumn(aiGroup));
			aiGroup.SetWaypointBehaviour(eAIWaypointBehavior.ALTERNATE);
	
			if (!m_Waypoints.Count())
				m_Waypoints.Insert(m_Position);

			for (int idx = 0; idx < m_Waypoints.Count(); idx++)
			{
				aiGroup.AddWaypoint(m_Waypoints[idx]);
				if (m_Waypoints[idx] == m_Position)
					aiGroup.m_CurrentWaypointIndex = idx;
			}
		}
	#endif
	}

	protected bool LoadItem(ExpansionP2PMarketListing listing, PlayerBase player = null, out EntityAI loadedEntity = null)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
#endif 
		
		if (!ExpansionEntityStorageModule.RestoreFromFile(listing.GetEntityStorageFileName(), loadedEntity, null, player))
		{
			Error(ToString() + "::LoadItem - Could not restore item " + listing.GetClassName() + " from file " + listing.GetEntityStorageFileName());
			return false;
		}

		return true;
	}

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(m_TraderID);
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(m_TraderID))
			return false;

		return true;
	}
};