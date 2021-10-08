/**
 * ExpansionTraderBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionTraderNPCBase
 * @brief		This class handles trader NPCs
 **/
class ExpansionTraderNPCBase extends DayZPlayer
{
	private static ref set<ExpansionTraderNPCBase> m_allTraders = new set<ExpansionTraderNPCBase>;

	protected ref TInputActionMap m_InputActionMap;

	bool m_ActionsInitialize;

	private ref ExpansionHumanCommandTrader_ST m_CommandTraderTable;

	private ref ExpansionTraderObjectBase m_TraderObject;
	
	// ------------------------------------------------------------
	// ExpansionTraderNPCBase Constructor
	// ------------------------------------------------------------
	void ExpansionTraderNPCBase()
	{
		if (IsMissionHost())
		{
			SetAllowDamage(false);
		}

		m_allTraders.Insert(this);

		if (GetGame() && GetGame().IsClient())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(LoadTrader, 250);
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderNPCBase Destructor
	// ------------------------------------------------------------
	void ~ExpansionTraderNPCBase()
	{
		if (!GetGame())
			return;

		delete m_CommandTraderTable;
		
		int idx = m_allTraders.Find(this);
		if (idx >= 0)
		{
			m_allTraders.Remove(idx);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderNPCBase GetAll
	// ------------------------------------------------------------
	static set<ExpansionTraderNPCBase> GetAll()
	{
		return m_allTraders;
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderNPCBase SetActions
	// ------------------------------------------------------------
	void SetActions()
	{
		//AddAction(ExpansionActionOpenTraderMenu);
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderNPCBase InitializeActions
	// ------------------------------------------------------------
	void InitializeActions()
	{
		m_InputActionMap = new TInputActionMap;
		SetActions();
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderNPCBase GetActions
	// ------------------------------------------------------------	
	override void GetActions(typename action_input_type, out array<ActionBase_Basic> actions)
	{
		if(!m_ActionsInitialize)
		{
			m_ActionsInitialize = true;
			InitializeActions();
		}
		
		actions = m_InputActionMap.Get(action_input_type);
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderNPCBase AddAction
	// ------------------------------------------------------------	
	void AddAction(typename actionName)
	{
		ActionBase action = ActionManagerBase.GetAction(actionName);

		typename ai = action.GetInputType();
		if(!ai)
		{
			m_ActionsInitialize = false;
			return;
		}
		
		array<ActionBase_Basic> action_array = m_InputActionMap.Get( ai );
		
		if(!action_array)
		{
			action_array = new array<ActionBase_Basic>;
			m_InputActionMap.Insert(ai, action_array);
		}
		action_array.Insert(action); 
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderNPCBase RemoveAction
	// ------------------------------------------------------------	
	void RemoveAction(typename actionName)
	{
		ActionBase action = ActionManagerBase.GetAction(actionName);
		typename ai = action.GetInputType();
		array<ActionBase_Basic> action_array = m_InputActionMap.Get( ai );
		
		if(action_array)
		{
			for(int i = 0; i < action_array.Count(); i++)
			{
				if(action == action_array.Get(i))
				{
					action_array.Remove(i);
				}
			}
			action_array = new array<ActionBase_Basic>;
			m_InputActionMap.Insert(ai, action_array);
		}
		action_array.Insert(action); 
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderNPCBase OnRPC
	// ------------------------------------------------------------
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		m_TraderObject.OnRPC(sender, rpc_type, ctx);
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderNPCBase IsInventoryVisible
	// ------------------------------------------------------------
	override bool IsInventoryVisible()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderNPCBase CanDisplayCargo
	// ------------------------------------------------------------
	override bool CanDisplayCargo()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderNPCBase CanPutIntoHands
	// ------------------------------------------------------------
	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderNPCBase CommandHandler
	// ------------------------------------------------------------
	override void CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished) 
	{
		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_SCRIPT)
		{
			HumanCommandScript hcs = GetCommand_Script();	
		
			ExpansionHumanCommandTrader traderCommand;
			if (Class.CastTo(traderCommand, hcs))
			{
				return;
			}
		}

		if (m_CommandTraderTable == NULL)
			m_CommandTraderTable = new ExpansionHumanCommandTrader_ST(this);
		
		StartCommand_Script(new ExpansionHumanCommandTrader(this, m_CommandTraderTable));
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
};

/**@class		ExpansionTraderObjectBase
 * @brief		This class handles all trader functionality
 **/
class ExpansionTraderObjectBase
{
	private static ref set<ExpansionTraderObjectBase> m_allTraderObjects = new set<ExpansionTraderObjectBase>;

	protected ref ExpansionMarketTraderZone m_TraderZone;
	protected ref ExpansionMarketTrader m_Trader;

	private EntityAI m_TraderEntity;
	
	// ------------------------------------------------------------
	// ExpansionTraderObjectBase Constructor
	// ------------------------------------------------------------
	void ExpansionTraderObjectBase(EntityAI traderEntity, string fileName = "")
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint(ToString() + "::ExpansionTraderObjectBase");
		#endif
		
		m_allTraderObjects.Insert(this);

		SetTraderEntity(traderEntity);
		LoadTrader(fileName);
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderObjectBase Destructor
	// ------------------------------------------------------------
	void ~ExpansionTraderObjectBase()
	{
		EXPrint(ToString() + "::~ExpansionTraderObjectBase");

		if (!GetGame())
			return;

		int idx = m_allTraderObjects.Find(this);
		if (idx >= 0)
		{
			m_allTraderObjects.Remove(idx);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderObjectBase GetAll
	// ------------------------------------------------------------
	static set<ExpansionTraderObjectBase> GetAll()
	{
		return m_allTraderObjects;
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderObjectBase LoadTrader
	// ------------------------------------------------------------
	void LoadTrader(string fileName)
	{
		if (IsMissionHost())
		{
			LoadTraderHost(fileName);
		} 
		else
		{
			RequestTraderObject();
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderObjectBase LoadTraderHost
	// ------------------------------------------------------------
	void LoadTraderHost(string fileName)
	{
		if (!m_TraderEntity)
		{
			Error(ToString() + "::LoadTraderHost - ERROR: Trader does not have entity!");
			return;
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint(ToString() + "::LoadTraderHost - m_TraderEntity: " + m_TraderEntity + " " + m_TraderEntity.GetType());
		#endif

		m_Trader = GetExpansionSettings().GetMarket().GetMarketTrader(fileName);
		if (!m_Trader)
		{
			if (fileName)
				Error(ToString() + "::LoadTraderHost - ERROR: Trader does not exist: " + fileName);
			return;
		}

		m_TraderZone = GetExpansionSettings().GetMarket().GetTraderZoneByPosition(m_TraderEntity.GetPosition());
		#ifdef EXPANSIONEXPRINT
		EXPrint(ToString() + "::LoadTraderHost - m_Trader: " + m_Trader);
		#endif
		if (!m_TraderZone)
		{
			Error(ToString() + "::LoadTraderHost - ERROR: Trader is not within a trader zone: " + m_TraderEntity + " " + m_TraderEntity.GetPosition());
			return;
		}

		EXPrint(ToString() + "::LoadTraderHost - trader zone: " + m_TraderZone.m_FileName);
		UpdateTraderZone();
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderObjectBase RequestTraderObject
	// ------------------------------------------------------------
	void RequestTraderObject()
	{
		if ( IsMissionOffline() )
			return;

		ScriptRPC rpc = new ScriptRPC();
		rpc.Send(this.GetTraderEntity(), ExpansionMarketRPC.TraderObject, true, NULL);
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderObjectBase OnRPC
	// ------------------------------------------------------------
	void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		if (rpc_type <= ExpansionMarketRPC.INVALID)
			return;
		if (rpc_type >= ExpansionMarketRPC.COUNT)
			return;
		
		switch (rpc_type)
		{
		case ExpansionMarketRPC.TraderObject:
			RPC_TraderObject(ctx, sender);
			break;
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderObjectBase RPC_TraderObject
	// ------------------------------------------------------------
	private void RPC_TraderObject(ParamsReadContext ctx, PlayerIdentity sender)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint(ToString() + "::RPC_TraderObject");
		#endif
		
		if (IsMissionHost())
		{
			if (!m_Trader)
				return;

			ScriptRPC rpc = new ScriptRPC();

			//! Items are sent via market module
			map<string, ExpansionMarketTraderBuySell> items = m_Trader.Items;
			m_Trader.Items = NULL;

			rpc.Write(m_Trader);

			m_Trader.Items = items;

			rpc.Write(m_Trader.m_FileName);

			rpc.Send(this.GetTraderEntity(), ExpansionMarketRPC.TraderObject, true, sender);
			return;
		}

		if (!ctx.Read(m_Trader))
		{
			Error("ExpansionTraderObjectBase::RPC_TraderObject - Error reading m_Trader param");
		}

		if (!ctx.Read(m_Trader.m_FileName))
		{
			Error("ExpansionTraderObjectBase::RPC_TraderObject - Error reading m_Trader.m_FileName param");
		}

		ExpansionMarketTrader trader = GetExpansionSettings().GetMarket().GetMarketTrader(m_Trader.m_FileName);
		if (trader)
		{
			//! Use cached trader
			EXPrint("RPC_TraderObject - using cached trader " + trader.m_FileName);
			m_Trader = trader;
			EXPrint("RPC_TraderObject - trader items " + m_Trader.Items.Count());
		}
		else
		{
			//! Cache trader
			EXPrint("RPC_TraderObject - caching trader " + m_Trader.m_FileName);
			m_Trader.m_Items = new array<ref ExpansionMarketTraderItem>;
			GetExpansionSettings().GetMarket().AddMarketTrader(m_Trader);
		}

		EXPrint("RPC_TraderObject - trader currencies " + m_Trader.Currencies.Count());
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderObjectBase UpdateTraderZone
	// ------------------------------------------------------------
	void UpdateTraderZone()
	{
		if (!m_TraderZone)
			return;

		#ifdef EXPANSIONMODMARKET_DEBUG
		EXPrint(ToString() + "::UpdateTraderZone - trader " + m_Trader.TraderName + " - trader zone " + m_TraderZone.m_DisplayName + " - stock before update");
		m_TraderZone.DebugPrint();
		#endif

		bool updated;

		foreach (ExpansionMarketTraderItem item : m_Trader.m_Items)
		{
			if (!m_TraderZone.Stock.Contains(item.MarketItem.ClassName))
			{
				int newStock;
				if (item.MarketItem.IsStaticStock())
					newStock = 1;
				else
					newStock = item.MarketItem.MaxStockThreshold;
				m_TraderZone.Stock.Insert(item.MarketItem.ClassName, newStock);
				updated = true;
			}
		}

		if (updated)
			m_TraderZone.Save();

		#ifdef EXPANSIONMODMARKET_DEBUG
		EXPrint(ToString() + "::UpdateTraderZone - trader " + m_Trader.TraderName + " - trader zone " + m_TraderZone.m_DisplayName + " - stock after update");
		m_TraderZone.DebugPrint();
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderObjectBase GetNetworkSerialization
	// ------------------------------------------------------------
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
	
	// ------------------------------------------------------------
	// ExpansionTraderObjectBase GetTraderZone
	// ------------------------------------------------------------
	ExpansionMarketTraderZone GetTraderZone()
	{
		return m_TraderZone;
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderObjectBase GetTraderMarket
	// ------------------------------------------------------------
	ExpansionMarketTrader GetTraderMarket()
	{
		return m_Trader;
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderObjectBase HasVehicleSpawnPosition
	// ------------------------------------------------------------
	bool HasVehicleSpawnPosition(string className, out vector spawnPosition, out vector spawnOrientation, out ExpansionMarketVehicleSpawnType spawnType = 0, out ExpansionMarketResult result = 0, out Object blockingObject = NULL, int amountNeeded = 1)
	{
		//array<vector> positions;
		array<ref ExpansionMarketSpawnPosition> positions;

		if (GetGame().IsKindOf(className, "ExpansionBoatScript") || GetGame().IsKindOf(className, "ExpansionVehicleBoatBase"))
		{
			positions = GetExpansionSettings().GetMarket().WaterSpawnPositions;
			spawnType = ExpansionMarketVehicleSpawnType.WATER;
		} 
		else if (GetGame().IsKindOf(className, "ExpansionHelicopterScript") || GetGame().IsKindOf(className, "ExpansionVehicleHelicopterBase"))
		{
			positions = GetExpansionSettings().GetMarket().AirSpawnPositions;
			spawnType = ExpansionMarketVehicleSpawnType.AIR;
		} 
		else if (GetGame().IsKindOf(className, "ExpansionVehiclePlaneBase") || GetGame().IsKindOf(className, "ExpansionVehiclePlaneBase"))
		{
			positions = GetExpansionSettings().GetMarket().AirSpawnPositions;
			spawnType = ExpansionMarketVehicleSpawnType.AIR;
		} 
		else
		{
			positions = GetExpansionSettings().GetMarket().LandSpawnPositions;
			spawnType = ExpansionMarketVehicleSpawnType.LAND;
		}

		if (!positions || !positions.Count())
		{
			result = ExpansionMarketResult.FailedNoVehicleSpawnPositions;
			return false;
		}

		result = ExpansionMarketResult.Success;

		map<ref ExpansionMarketSpawnPosition, float> foundPositions = new map<ref ExpansionMarketSpawnPosition, float>;

		float minDistance = GetExpansionSettings().GetMarket().GetMinVehicleDistanceToTrader(className);
		float maxDistance = GetExpansionSettings().GetMarket().GetMaxVehicleDistanceToTrader(className);

		float radius = ExpansionStatic.GetBoundingRadius(className);

		EXPrint(ToString() + "::HasVehicleSpawnPosition - " + className + " bounding radius: " + radius);

		ExpansionMarketSpawnPosition lastCheckedPos;

		foreach (ExpansionMarketSpawnPosition position : positions)
		{
			float distance = vector.Distance( position.Position, m_TraderEntity.GetPosition() );

			if (distance < minDistance || distance > maxDistance)
				continue;
			
			if (!VehicleSpawnPositionFree(position.Position, radius))
			{
				result = ExpansionMarketResult.FailedVehicleSpawnOccupied;
				lastCheckedPos = position;
				continue;
			}
			
			#ifdef EXPANSIONEXPRINT
			EXPrint(ToString() + "::HasVehicleSpawnPosition - found candidate " + position.Position + " (distance " + distance + ") for " + className);
			#endif
			foundPositions.Insert(position, distance);
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint(ToString() + "::HasVehicleSpawnPosition - found " + foundPositions.Count() + " candidate spawn positions for " + className + ", needed " + amountNeeded);
		#endif
		
		if (foundPositions.Count() >= amountNeeded)
		{
			float closestDistance = 1000;

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
			
			#ifdef EXPANSIONEXPRINT
			EXPrint(ToString() + "::HasVehicleSpawnPosition - selected " + spawnPosition + " (distance " + closestDistance + ") for " + className);
			#endif
			
			return true;
		}

		if (lastCheckedPos)
			blockingObject = ExpansionGetObjectBlockingPosition(lastCheckedPos.Position, radius);
		else
			result = ExpansionMarketResult.FailedNotEnoughVehicleSpawnPositionsNear;

		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderObjectBase VehicleSpawnPositionFree
	// ------------------------------------------------------------	
	private bool VehicleSpawnPositionFree(vector pos, float radius = 5)
	{
		vector start = pos + Vector(0, 3, 0);

		PhxInteractionLayers layerMask = PhxInteractionLayers.CHARACTER | PhxInteractionLayers.ITEM_LARGE | PhxInteractionLayers.VEHICLE;

		return !DayZPhysics.SphereCastBullet(start, pos, radius, layerMask, NULL, NULL, NULL, NULL, NULL);
	}
	
	Object ExpansionGetObjectBlockingPosition(vector position, float radius)
	{
		array< Object > objects = new array< Object >;
		array< CargoBase > proxyCargos = new array< CargoBase >;

		GetGame().GetObjectsAtPosition3D(position, radius, objects, proxyCargos);

		foreach (Object obj: objects)
		{
			bool match = obj.IsInherited(Man) || (obj.IsInherited(ItemBase) && obj.ConfigGetString("physLayer") == "item_large") || obj.IsInherited(CarScript);

			#ifdef EXPANSIONMODVEHICLE
			match |= obj.IsInherited(ExpansionVehicleBase);
			#endif

			if (match)
				return obj;
		}

		return NULL;
	}

	string GetDisplayName()
	{
		if (!GetTraderMarket())
			return string.Empty;

		string traderName = GetTraderMarket().DisplayName;
		if (traderName.Contains("#"))
		{
			StringLocaliser name = new StringLocaliser(traderName);
			traderName = name.Format();
		}

		if (!m_TraderEntity)
			return traderName;

		return m_TraderEntity.GetDisplayName() + " [" + traderName + "]";
	}

	void SetTraderEntity(EntityAI entity)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint(ToString() + "::SetTraderEntity " + entity + " " + entity.GetType());
		#endif
		m_TraderEntity = entity;
	}

	EntityAI GetTraderEntity()
	{
		return m_TraderEntity;
	}
}

/**@class		ExpansionTraderStaticBase
 * @brief		This class handles static trader objects
 **/
class ExpansionTraderStaticBase extends BuildingSuper
{
	private static ref set<ExpansionTraderStaticBase> m_allTraders = new set<ExpansionTraderStaticBase>;

	private ref ExpansionTraderObjectBase m_TraderObject;

	// ------------------------------------------------------------
	// ExpansionTraderStaticBase Constructor
	// ------------------------------------------------------------
	void ExpansionTraderStaticBase()
	{
		if (IsMissionHost())
		{
			SetAllowDamage(false);
		}

		m_allTraders.Insert(this);

		if (GetGame() && GetGame().IsClient())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(LoadTrader, 250);
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderStaticBase Destructor
	// ------------------------------------------------------------
	void ~ExpansionTraderStaticBase()
	{
		if (!GetGame())
			return;

		int idx = m_allTraders.Find(this);
		if (idx >= 0)
		{
			m_allTraders.Remove(idx);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderStaticBase GetAll
	// ------------------------------------------------------------
	static set<ExpansionTraderStaticBase> GetAll()
	{
		return m_allTraders;
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderStaticBase OnRPC
	// ------------------------------------------------------------
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		m_TraderObject.OnRPC(sender, rpc_type, ctx);
	}

	void SetTraderObject(ExpansionTraderObjectBase traderObject)
	{
		m_TraderObject = traderObject;
		#ifdef EXPANSIONEXPRINT
		EXPrint(ToString() + "::SetTraderObject " + m_TraderObject + " " + GetPosition());
		#endif
	}

	ExpansionTraderObjectBase GetTraderObject()
	{
		return m_TraderObject;
	}

	void LoadTrader(string fileName = "")
	{
		SetTraderObject(new ExpansionTraderObjectBase(this, fileName));
	}
}

/**@class		ExpansionTraderZombieBase
 * @brief		This class handles Infected traders
 **/
class ExpansionTraderZombieBase extends ZombieBase
{
	private static ref set<ExpansionTraderZombieBase> m_allTraders = new set<ExpansionTraderZombieBase>;

	private ref ExpansionTraderObjectBase m_TraderObject;

	// ------------------------------------------------------------
	// ExpansionTraderZombieBase Constructor
	// ------------------------------------------------------------
	void ExpansionTraderZombieBase()
	{
		if (IsMissionHost())
		{
			SetAllowDamage(false);
		}

		m_allTraders.Insert(this);

		if (GetGame() && GetGame().IsClient())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(LoadTrader, 250);
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderZombieBase Destructor
	// ------------------------------------------------------------
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
	
	// ------------------------------------------------------------
	// ExpansionTraderZombieBase GetAll
	// ------------------------------------------------------------
	//static set<ExpansionTraderZombieBase> GetAll()
	//{
		//return m_allTraders;
	//}
	
	// ------------------------------------------------------------
	// ExpansionTraderZombieBase OnRPC
	// ------------------------------------------------------------
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		m_TraderObject.OnRPC(sender, rpc_type, ctx);
	}

	void SetTraderObject(ExpansionTraderObjectBase traderObject)
	{
		m_TraderObject = traderObject;
		#ifdef EXPANSIONEXPRINT
		EXPrint(ToString() + "::SetTraderObject " + m_TraderObject + " " + GetPosition());
		#endif
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

		m_SafeZone = true;
	}

	override void OnExitZone(ExpansionZoneType type)
	{
		if (type != ExpansionZoneType.SAFE) return;

		m_SafeZone = false;
	}
}

#ifdef ENFUSION_AI_PROJECT
/**@class		ExpansionTraderAIBase
 * @brief		This class handles AI traders
 **/
class ExpansionTraderAIBase extends eAIBase
{
	private ref ExpansionTraderObjectBase m_TraderObject;

	// ------------------------------------------------------------
	// ExpansionTraderAIBase Constructor
	// ------------------------------------------------------------
	void ExpansionTraderAIBase()
	{
		if (IsMissionHost())
		{
			SetAllowDamage(false);
		}

		if (GetGame() && GetGame().IsClient())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(LoadTrader, 250);
	}
	
	// ------------------------------------------------------------
	// ExpansionTraderAIBase OnRPC
	// ------------------------------------------------------------
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		m_TraderObject.OnRPC(sender, rpc_type, ctx);
	}

	void SetTraderObject(ExpansionTraderObjectBase traderObject)
	{
		m_TraderObject = traderObject;
		#ifdef EXPANSIONEXPRINT
		EXPrint(ToString() + "::SetTraderObject " + m_TraderObject + " " + GetPosition());
		#endif
	}

	ExpansionTraderObjectBase GetTraderObject()
	{
		return m_TraderObject;
	}

	void LoadTrader(string fileName = "")
	{
		SetTraderObject(new ExpansionTraderObjectBase(this, fileName));
	}

	override bool PlayerIsEnemy(PlayerBase other)
	{
		if ( !IsInSafeZone() )
			return false;

		return super.PlayerIsEnemy(other);
	}
}

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

class ExpansionTraderZmbM_JournalistSkinny: ExpansionTraderZombieBase {};
