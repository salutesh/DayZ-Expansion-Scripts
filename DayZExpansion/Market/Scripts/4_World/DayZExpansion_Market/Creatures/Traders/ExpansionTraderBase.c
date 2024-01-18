/**
 * ExpansionTraderBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionTraderNPCBase
 * @brief		This class handles trader NPCs
 **/
class ExpansionTraderNPCBase: ExpansionNPCBase
{
	private static ref set<ExpansionTraderNPCBase> m_allTraders = new set<ExpansionTraderNPCBase>;

	private ref ExpansionTraderObjectBase m_TraderObject;

	void ExpansionTraderNPCBase()
	{
		m_allTraders.Insert(this);

		if (GetGame() && GetGame().IsClient())
			LoadTrader("");
	}

	void ~ExpansionTraderNPCBase()
	{
		if (!GetGame())
			return;
		
		int idx = m_allTraders.Find(this);
		if (idx >= 0)
		{
			m_allTraders.Remove(idx);
		}
	}

	static set<ExpansionTraderNPCBase> GetAll()
	{
		return m_allTraders;
	}

	void SetTraderObject(ExpansionTraderObjectBase traderObject)
	{
		m_TraderObject = traderObject;
		EXPrint(ToString() + "::SetTraderObject " + m_TraderObject + " " + GetPosition());
	}

	ExpansionTraderObjectBase GetTraderObject()
	{
		return m_TraderObject;
	}

	void LoadTrader(string fileName = "")
	{
		SetTraderObject(new ExpansionTraderObjectBase(this, fileName));
	}

	override bool Expansion_IsTrader()
	{
		return true;
	}
};

/**@class		ExpansionTraderObjectBase
 * @brief		This class handles all trader functionality
 **/
class ExpansionTraderObjectBase
{
	private static ref set<ExpansionTraderObjectBase> m_allTraderObjects = new set<ExpansionTraderObjectBase>;

	protected ref ExpansionMarketTraderZone m_TraderZone;
	protected ref ExpansionMarketTrader m_Trader;
	protected int m_DisplayCurrencyPrecision;

	private EntityAI m_TraderEntity;

	ref ExpansionRPCManager m_Expansion_RPCManager;

	void ExpansionTraderObjectBase(EntityAI traderEntity, string fileName = "")
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.MARKET, this, "ExpansionTraderObjectBase");
#endif
		
		m_allTraderObjects.Insert(this);

		SetTraderEntity(traderEntity);
		LoadTrader(fileName);
	}

	void ~ExpansionTraderObjectBase()
	{
		if (!GetGame())
			return;

		auto trace = EXTrace.Start(ExpansionTracing.MARKET, this, "" + m_TraderEntity, "" + m_Expansion_RPCManager);

		int idx = m_allTraderObjects.Find(this);
		if (idx >= 0)
		{
			m_allTraderObjects.Remove(idx);
		}
	}

	static set<ExpansionTraderObjectBase> GetAll()
	{
		return m_allTraderObjects;
	}

	void LoadTrader(string fileName)
	{
		if (IsMissionHost())
		{
			LoadTraderHost(fileName);
		} 
		else
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(RequestTraderObject, 250, false);
		}
	}

	void LoadTraderHost(string fileName)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.MARKET, this, "LoadTraderHost");
#endif

		if (!m_TraderEntity)
		{
			Error(ToString() + "::LoadTraderHost - ERROR: Trader does not have entity!");
			return;
		}
		
		CF_Log.Debug(ToString() + "::LoadTraderHost - m_TraderEntity: " + m_TraderEntity + " " + m_TraderEntity.GetType());

		m_Trader = GetExpansionSettings().GetMarket().GetMarketTrader(fileName);
		if (!m_Trader)
		{
			if (fileName)
			{
				CF_Log.Error(ToString() + "::LoadTraderHost - ERROR: Trader does not exist: " + fileName);
			}
			return;
		}

		m_TraderZone = GetExpansionSettings().GetMarket().GetTraderZoneByPosition(m_TraderEntity.GetPosition());
		
		CF_Log.Debug(ToString() + "::LoadTraderHost - m_Trader: " + m_Trader);

		if (!m_TraderZone)
		{
			CF_Log.Error(ToString() + "::LoadTraderHost - ERROR: Trader is not within a trader zone: " + m_TraderEntity + " " + m_TraderEntity.GetPosition());
			return;
		}

		CF_Log.Debug(ToString() + "::LoadTraderHost - trader zone: " + m_TraderZone.m_FileName);

		UpdateTraderZone();
	}

	void RequestTraderObject()
	{
		if ( IsMissionOffline() )
			return;

		auto rpc = m_Expansion_RPCManager.CreateRPC("RPC_TraderObject");
		rpc.Expansion_Send(GetTraderEntity(), true);
	}

	private void RPC_TraderObject(PlayerIdentity sender, ParamsReadContext ctx)
	{
		if (GetGame().IsServer())
		{
			if (!m_Trader)
				return;

			auto rpc = m_Expansion_RPCManager.CreateRPC("RPC_TraderObject");

			rpc.Write(m_Trader.m_FileName);
			rpc.Write(m_Trader.DisplayName);
			rpc.Write(m_Trader.TraderIcon);
			rpc.Write(m_Trader.Currencies);
			rpc.Write(m_Trader.DisplayCurrencyValue);
			rpc.Write(m_Trader.DisplayCurrencyName);
			rpc.Write(m_Trader.m_Categories);

			rpc.Expansion_Send(GetTraderEntity(), true, sender);
		}
		else
		{
			string fileName;
			if (!ctx.Read(fileName))
				return;
				
			auto marketSettings = GetExpansionSettings().GetMarket(false);
			m_Trader = marketSettings.GetMarketTrader(fileName);
			if (!m_Trader)
			{
				m_Trader = new ExpansionMarketTrader;
				m_Trader.m_FileName = fileName;
				marketSettings.AddMarketTrader(m_Trader);
			}

			if (!ctx.Read(m_Trader.DisplayName))
				return;

			if (!ctx.Read(m_Trader.TraderIcon))
				return;

			if (!ctx.Read(m_Trader.Currencies))
				return;

			if (!ctx.Read(m_Trader.DisplayCurrencyValue))
				return;

			m_Trader.m_DisplayCurrencyPrecision = ExpansionStatic.GetPrecision(m_Trader.DisplayCurrencyValue);

			if (!ctx.Read(m_Trader.DisplayCurrencyName))
				return;

			if (!ctx.Read(m_Trader.m_Categories))
				return;
		}
	}

	void UpdateTraderZone()
	{
		if (!m_TraderZone)
			return;

		#ifdef EXPANSIONMODMARKET_DEBUG
		EXPrint(ToString() + "::UpdateTraderZone - trader " + m_Trader.m_FileName + " - trader zone " + m_TraderZone.m_DisplayName + " - stock before update");
		m_TraderZone.DebugPrint();
		#endif

		bool updated;

		foreach (ExpansionMarketTraderItem item : m_Trader.m_Items)
		{
			if (!m_TraderZone.Stock.Contains(item.MarketItem.ClassName))
			{
				ExpansionMarketCategory cat = GetExpansionSettings().GetMarket().GetCategory(item.MarketItem.CategoryID);
				int newStock;
				if (item.MarketItem.IsStaticStock())
					newStock = 1;
				else
					newStock = item.MarketItem.MaxStockThreshold;
				if (cat)
					newStock = newStock * cat.InitStockPercent * 0.01;
				m_TraderZone.Stock.Insert(item.MarketItem.ClassName, newStock);
				updated = true;
			}
		}

		if (updated)
			m_TraderZone.Save();

		#ifdef EXPANSIONMODMARKET_DEBUG
		EXPrint(ToString() + "::UpdateTraderZone - trader " + m_Trader.m_FileName + " - trader zone " + m_TraderZone.m_DisplayName + " - stock after update");
		m_TraderZone.DebugPrint();
		#endif
	}

	int GetNetworkSerialization(out array<ref ExpansionMarketNetworkItem> list, int start, bool stockOnly, TIntArray itemIDs = NULL)
	{
		if (!m_Trader)
		{
			Error("ExpansionTraderObjectBase::GetNetworkSerialization - Error reading m_Trader");
			return -1;
		}
		
		if (!m_TraderZone)
		{
			Error("ExpansionTraderObjectBase::GetNetworkSerialization - Error reading m_TraderZone");
			return -1;
		}
		
		return m_TraderZone.GetNetworkSerialization(m_Trader, list, start, stockOnly, itemIDs);
	}

	ExpansionMarketTraderZone GetTraderZone()
	{
		return m_TraderZone;
	}
	
	void SetTraderZone(ExpansionMarketTraderZone zone)
	{
		m_TraderZone = zone;
	}

	ExpansionMarketTrader GetTraderMarket()
	{
		return m_Trader;
	}
	
	void SetTraderMarket(ExpansionMarketTrader trader)
	{
		m_Trader = trader;
	}

	string GetDisplayPrice(int price, bool shorten = false, bool format = true, bool includeDisplayCurrencyName = false)
	{
		return ExpansionMarketModule.GetDisplayPrice(m_Trader, price, shorten, format, includeDisplayCurrencyName);
	}

	bool HasVehicleSpawnPosition(string className, out vector spawnPosition, out vector spawnOrientation, out ExpansionMarketVehicleSpawnType spawnType = 0, out ExpansionMarketResult result = ExpansionMarketResult.Success, out Object blockingObject = NULL, int amountNeeded = 1)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.MARKET, this, "HasVehicleSpawnPosition");
#endif

		//array<vector> positions;
		array<ref ExpansionMarketSpawnPosition> positions;		
		string vehicleClass = GetGame().ConfigGetTextOut("CfgVehicles " + className + " vehicleClass");
		switch (vehicleClass)
		{
			case "Expansion_Boat":
			case "Expansion_Ship":
			case "Vehicles_Boat":
			case "Vehicles_Ship":
				positions = GetExpansionSettings().GetMarket().WaterSpawnPositions;
				spawnType = ExpansionMarketVehicleSpawnType.WATER;
				break;
			case "Expansion_Helicopter":
			case "Expansion_Plane":
			case "Vehicles_Helicopter":
			case "Vehicles_Plane":
				positions = GetExpansionSettings().GetMarket().AirSpawnPositions;
				spawnType = ExpansionMarketVehicleSpawnType.AIR;
				break;
		#ifdef HypeTrain
			case "HypeTrain_Locomotive":
			case "HypeTrain_Wagon":
				positions = GetExpansionSettings().GetMarket().TrainSpawnPositions;
				spawnType = ExpansionMarketVehicleSpawnType.RAILTRACK;
				break;
		#endif
			default:
				positions = GetExpansionSettings().GetMarket().LandSpawnPositions;
				spawnType = ExpansionMarketVehicleSpawnType.LAND;
		}

		if (!positions || !positions.Count())
		{
			result = ExpansionMarketResult.FailedNoVehicleSpawnPositions;
			return false;
		}

		map<ref ExpansionMarketSpawnPosition, float> foundPositions = new map<ref ExpansionMarketSpawnPosition, float>;

		float minDistance = GetExpansionSettings().GetMarket().GetMinVehicleDistanceToTrader(className);
		float maxDistance = GetExpansionSettings().GetMarket().GetMaxVehicleDistanceToTrader(className);

		ExpansionMarketSpawnPosition lastCheckedPos;

		Object tempBlockingObject;
		foreach (ExpansionMarketSpawnPosition position : positions)
		{
			float distance = vector.Distance( position.Position, m_TraderEntity.GetPosition() );

			if (distance < minDistance || distance > maxDistance)
				continue;
			
			lastCheckedPos = position;

			if (!ExpansionItemSpawnHelper.IsSpawnPositionFree(position.Position, position.Orientation, className, tempBlockingObject))
			{
				blockingObject = tempBlockingObject;
				continue;
			}
			
			CF_Log.Debug(ToString() + "::HasVehicleSpawnPosition - found candidate " + position.Position + " (distance " + distance + ") for " + className);

			foundPositions.Insert(position, distance);
		}
		
		CF_Log.Debug(ToString() + "::HasVehicleSpawnPosition - found " + foundPositions.Count() + " candidate spawn positions for " + className + ", needed " + amountNeeded);
		
		if (foundPositions.Count() >= amountNeeded)
		{
			float closestDistance = int.MAX;

			//! Select closest one
			foreach (ExpansionMarketSpawnPosition candidatePosition, float candidateDistance : foundPositions)
			{
				if (candidateDistance < closestDistance)
				{
					closestDistance = candidateDistance;
					spawnPosition = candidatePosition.Position;
					spawnOrientation = candidatePosition.Orientation;
				}
			}
			
			CF_Log.Debug(ToString() + "::HasVehicleSpawnPosition - selected " + spawnPosition + " (distance " + closestDistance + ") for " + className);
			
			result = ExpansionMarketResult.Success;

			return true;
		}

		if (!lastCheckedPos)
			result = ExpansionMarketResult.FailedNotEnoughVehicleSpawnPositionsNear;
		else if (blockingObject)
			result = ExpansionMarketResult.FailedVehicleSpawnOccupied;

		return false;
	}

	string GetDisplayName()
	{
		if (!GetTraderMarket())
			return string.Empty;

		string traderName = GetTraderMarket().DisplayName;

		if (!m_TraderEntity)
			return traderName;

		string entityName = m_TraderEntity.GetDisplayName();

		if (entityName != traderName)
			return entityName + " [" + traderName + "]";
		
		return entityName;
	}

	void SetTraderEntity(EntityAI entity)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.MARKET, this, "SetTraderEntity").Add(entity);
#endif

		m_TraderEntity = entity;

		EnScript.GetClassVar(entity, "m_Expansion_RPCManager", 0, m_Expansion_RPCManager);
		if (!m_Expansion_RPCManager)
		{
			typename type = ExpansionWorld.GetModdableRootType(entity);
			m_Expansion_RPCManager = new ExpansionRPCManager(entity, type);
			EnScript.SetClassVar(entity, "m_Expansion_RPCManager", 0, m_Expansion_RPCManager);
		}
		m_Expansion_RPCManager.RegisterBoth("RPC_TraderObject", this);
	}

	EntityAI GetTraderEntity()
	{
		return m_TraderEntity;
	}

	void AddInteractingPlayer(Man player)
	{
	#ifdef EXPANSIONMODAI
		eAIBase ai;
		if (Class.CastTo(ai, m_TraderEntity))
			ai.eAI_AddInteractingPlayer(player);
	#endif
	}

	void RemoveInteractingPlayer(Man player)
	{
	#ifdef EXPANSIONMODAI
		eAIBase ai;
		if (Class.CastTo(ai, m_TraderEntity))
			ai.eAI_RemoveInteractingPlayer(player);
	#endif
	}
}

/**@class		ExpansionTraderStaticBase
 * @brief		This class handles static trader objects
 **/
class ExpansionTraderStaticBase: ExpansionStaticObjectBase
{
	private static ref set<ExpansionTraderStaticBase> m_allTraders = new set<ExpansionTraderStaticBase>;
	private ref ExpansionTraderObjectBase m_TraderObject;

	void ExpansionTraderStaticBase()
	{
		m_allTraders.Insert(this);
		m_Expansion_NetsyncData = new ExpansionNetsyncData(this);
		if (GetGame() && GetGame().IsClient())
			LoadTrader("");
	}

	void ~ExpansionTraderStaticBase()
	{
		if (!GetGame())
			return;

		int idx = m_allTraders.Find(this);
		if (idx >= 0)
			m_allTraders.Remove(idx);
	}

	static set<ExpansionTraderStaticBase> GetAll()
	{
		return m_allTraders;
	}

	void SetTraderObject(ExpansionTraderObjectBase traderObject)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.MARKET, this, "SetTraderObject");
#endif

		m_TraderObject = traderObject;
	}

	ExpansionTraderObjectBase GetTraderObject()
	{
		return m_TraderObject;
	}

	void LoadTrader(string fileName = "")
	{
		SetTraderObject(new ExpansionTraderObjectBase(this, fileName));
	}

	bool Expansion_IsTrader()
	{
		return true;
	}
};

/**@class		ExpansionTraderZombieBase
 * @brief		This class handles Infected traders
 **/
class ExpansionTraderZombieBase: ZombieBase
{
	private static ref set<ExpansionTraderZombieBase> m_allTraders = new set<ExpansionTraderZombieBase>;
	private ref ExpansionTraderObjectBase m_TraderObject;

	void ExpansionTraderZombieBase()
	{
		if (IsMissionHost())
		{
			SetAllowDamage(false);
		}

		m_allTraders.Insert(this);

		m_Expansion_NetsyncData = new ExpansionNetsyncData(this);

		if (GetGame() && GetGame().IsClient())
			LoadTrader("");
	}

	void ~ExpansionTraderZombieBase()
	{
		if (!GetGame())
			return;

		int idx = m_allTraders.Find(this);
		if (idx >= 0)
		{
			m_allTraders.Remove(idx);
		}
	}

	void SetTraderObject(ExpansionTraderObjectBase traderObject)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.MARKET, this, "SetTraderObject");
#endif

		m_TraderObject = traderObject;
	}

	ExpansionTraderObjectBase GetTraderObject()
	{
		return m_TraderObject;
	}

	void LoadTrader(string fileName = "")
	{
		SetTraderObject(new ExpansionTraderObjectBase(this, fileName));
	}

	override void OnEnterZone(ExpansionZoneType type)
	{
		if (type != ExpansionZoneType.SAFE) return;

		m_Expansion_IsInSafeZone = true;
	}

	override void OnExitZone(ExpansionZoneType type)
	{
		if (type != ExpansionZoneType.SAFE) return;

		m_Expansion_IsInSafeZone = false;
	}

	bool Expansion_IsTrader()
	{
		return true;
	}
};

#ifdef ENFUSION_AI_PROJECT
/**@class		ExpansionTraderAIBase
 * @brief		This class handles AI traders
 **/
#ifdef EXPANSIONMODAI
class ExpansionTraderAIBase: eAINPCBase
#else
class ExpansionTraderAIBase: eAIBase
#endif
{
	private ref ExpansionTraderObjectBase m_TraderObject;

	void ExpansionTraderAIBase()
	{
#ifdef EXPANSIONMODAI
		SetMovementSpeedLimits(1.0);  //! Always walk
#endif

		if (GetGame() && GetGame().IsClient())
			LoadTrader("");
	}
	
	override void Expansion_Init()
	{
	#ifdef EXPANSIONMODAI
		if (GetGame().IsServer())
			m_eAI_FactionType = eAIFactionInvincibleObservers;
	#else
		Expansion_SetAllowDamage(false);
	#endif

		super.Expansion_Init();
	}

#ifndef EXPANSIONMODAI
	//! Vanilla, prevent being targeted by Zs/animals
	override bool CanBeTargetedByAI(EntityAI ai)
	{
		return false;
	}
#endif

	void SetTraderObject(ExpansionTraderObjectBase traderObject)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.MARKET, this, "SetTraderObject");
#endif

		m_TraderObject = traderObject;
	}

	ExpansionTraderObjectBase GetTraderObject()
	{
		return m_TraderObject;
	}

	void LoadTrader(string fileName = "")
	{
		SetTraderObject(new ExpansionTraderObjectBase(this, fileName));
	}

#ifndef EXPANSIONMODAI
	override bool PlayerIsEnemy(PlayerBase other)
	{
		return false;
	}
#endif

	override bool Expansion_IsTrader()
	{
		return true;
	}
};

class ExpansionTraderAIMirek: ExpansionTraderAIBase {};
class ExpansionTraderAIDenis: ExpansionTraderAIBase {};
class ExpansionTraderAIBoris: ExpansionTraderAIBase {};
class ExpansionTraderAICyril: ExpansionTraderAIBase {};
class ExpansionTraderAIElias: ExpansionTraderAIBase {};
class ExpansionTraderAIFrancis: ExpansionTraderAIBase {};
class ExpansionTraderAIGuo: ExpansionTraderAIBase {};
class ExpansionTraderAIHassan: ExpansionTraderAIBase {};
class ExpansionTraderAIIndar: ExpansionTraderAIBase {};
class ExpansionTraderAIJose: ExpansionTraderAIBase {};
class ExpansionTraderAIKaito: ExpansionTraderAIBase {};
class ExpansionTraderAILewis: ExpansionTraderAIBase {};
class ExpansionTraderAIManua: ExpansionTraderAIBase {};
class ExpansionTraderAINiki: ExpansionTraderAIBase {};
class ExpansionTraderAIOliver: ExpansionTraderAIBase {};
class ExpansionTraderAIPeter: ExpansionTraderAIBase {};
class ExpansionTraderAIQuinn: ExpansionTraderAIBase {};
class ExpansionTraderAIRolf: ExpansionTraderAIBase {};
class ExpansionTraderAISeth: ExpansionTraderAIBase {};
class ExpansionTraderAITaiki: ExpansionTraderAIBase {};
class ExpansionTraderAILinda: ExpansionTraderAIBase {};
class ExpansionTraderAIMaria: ExpansionTraderAIBase {};
class ExpansionTraderAIFrida: ExpansionTraderAIBase {};
class ExpansionTraderAIGabi: ExpansionTraderAIBase {};
class ExpansionTraderAIHelga: ExpansionTraderAIBase {};
class ExpansionTraderAIIrena: ExpansionTraderAIBase {};
class ExpansionTraderAIJudy: ExpansionTraderAIBase {};
class ExpansionTraderAIKeiko: ExpansionTraderAIBase {};
class ExpansionTraderAIEva: ExpansionTraderAIBase {};
class ExpansionTraderAINaomi: ExpansionTraderAIBase {};
class ExpansionTraderAIBaty: ExpansionTraderAIBase {};
#endif

class ExpansionTraderMirek: ExpansionTraderNPCBase {};
class ExpansionTraderDenis: ExpansionTraderNPCBase {};
class ExpansionTraderBoris: ExpansionTraderNPCBase {};
class ExpansionTraderCyril: ExpansionTraderNPCBase {};
class ExpansionTraderElias: ExpansionTraderNPCBase {};
class ExpansionTraderFrancis: ExpansionTraderNPCBase {};
class ExpansionTraderGuo: ExpansionTraderNPCBase {};
class ExpansionTraderHassan: ExpansionTraderNPCBase {};
class ExpansionTraderIndar: ExpansionTraderNPCBase {};
class ExpansionTraderJose: ExpansionTraderNPCBase {};
class ExpansionTraderKaito: ExpansionTraderNPCBase {};
class ExpansionTraderLewis: ExpansionTraderNPCBase {};
class ExpansionTraderManua: ExpansionTraderNPCBase {};
class ExpansionTraderNiki: ExpansionTraderNPCBase {};
class ExpansionTraderOliver: ExpansionTraderNPCBase {};
class ExpansionTraderPeter: ExpansionTraderNPCBase {};
class ExpansionTraderQuinn: ExpansionTraderNPCBase {};
class ExpansionTraderRolf: ExpansionTraderNPCBase {};
class ExpansionTraderSeth: ExpansionTraderNPCBase {};
class ExpansionTraderTaiki: ExpansionTraderNPCBase {};
class ExpansionTraderLinda: ExpansionTraderNPCBase {};
class ExpansionTraderMaria: ExpansionTraderNPCBase {};
class ExpansionTraderFrida: ExpansionTraderNPCBase {};
class ExpansionTraderGabi: ExpansionTraderNPCBase {};
class ExpansionTraderHelga: ExpansionTraderNPCBase {};
class ExpansionTraderIrena: ExpansionTraderNPCBase {};
class ExpansionTraderJudy: ExpansionTraderNPCBase {};
class ExpansionTraderKeiko: ExpansionTraderNPCBase {};
class ExpansionTraderEva: ExpansionTraderNPCBase {};
class ExpansionTraderNaomi: ExpansionTraderNPCBase {};
class ExpansionTraderBaty: ExpansionTraderNPCBase {};

class ExpansionTraderZmbM_JournalistSkinny: ExpansionTraderZombieBase {};
