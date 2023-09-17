/**
 * ExpansionParkingMeter.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODGARAGE
enum ExpansionParkingMeterState
{
	OFF = 0,
	ACTIVE = 1,
	LOW_ENERGY = 2,
	NO_ENERGY = 3,
	EVR = 4
};

class ExpansionParkingMeter: ExpansionDeployableConstruction
{
	static const string CAR_BATTERY_ATTACH_SOUND = "carbattery_attach_SoundSet";
	static const string CAR_BATTERY_DETACH_SOUND = "carbattery_detach_SoundSet";
	static const string RED_LIGHT_GLOW = "dz\\gear\\camping\\data\\battery_charger_light_r.rvmat";
	static const string YELLOW_LIGHT_GLOW = "dz\\gear\\camping\\data\\battery_charger_light_y.rvmat";
	static const string GREEN_LIGHT_GLOW = "dz\\gear\\camping\\data\\battery_charger_light_g.rvmat";
	static const string BLUE_LIGHT_GLOW = "dz\\gear\\consumable\\data\\chemlight_blue_on.rvmat";
	
	protected ExpansionTerritoryModule m_TerritoryModule;
	protected ExpansionGarageModule m_GarageModule;
	protected int m_TerritoryID;	 //! Unique terriotry id. Used to get and identify the parking meters territory in the territory and garage module.
	protected float m_ChargeEnergyPerSecond;

	void ExpansionParkingMeter()
	{
		m_ChargeEnergyPerSecond = GetGame().ConfigGetFloat ("CfgVehicles " + GetType() + " ChargeEnergyPerSecond");

		CF_Modules<ExpansionTerritoryModule>.Get(m_TerritoryModule);
		CF_Modules<ExpansionGarageModule>.Get(m_GarageModule);
		
		m_TerritoryID = -1;

		RegisterNetSyncVariableInt("m_TerritoryID");
	}

	void ~ExpansionParkingMeter()
	{
		if (!GetGame())
			return;

		if (GetGame().IsServer() && GetGame().IsMultiplayer() && ExpansionGarageModule.s_Instance)
		{
			if (m_TerritoryID > -1)
				ExpansionGarageModule.s_Instance.RemoveTerritoryParkingMeter(m_TerritoryID);
			else
				ExpansionGarageModule.s_Instance.RemoveParkingMeter(this);
		}
	}

	override void EOnInit(IEntity other, int extra)
	{
		m_TerritoryModule = ExpansionTerritoryModule.Cast(CF_ModuleCoreManager.Get(ExpansionTerritoryModule));
		m_GarageModule = ExpansionGarageModule.Cast(CF_ModuleCoreManager.Get(ExpansionGarageModule));
	}

	void SetTerritoryID(int territoryID)
	{
		m_TerritoryID = territoryID;
		m_GarageModule.AddTerritoryParkingMeter(this, m_TerritoryID);
	}

	bool IsInTerritory()
	{
		if (!m_TerritoryModule)
			return false;

		return m_TerritoryModule.IsInTerritory(GetPosition());
	}
	
	void OnDeployOutsideTerritory()
	{		
		m_GarageModule.AddParkingMeter(this);
	}
	
	override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI player)
	{
		return false;
	}
	
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);

		if (item.IsInherited(CarBattery))
		{
			if (GetCompEM().CanWork())
				GetCompEM().SwitchOn();
		}
		
		if (GetGame().IsServer())
		{
			ExpansionCircuitBoardBase board;
			if (Class.CastTo(board, item))
			{
				int slot_id_board = InventorySlots.GetSlotIdFromString("Att_ExpansionCircuitBoard");
				GetInventory().SetSlotLock(slot_id_board, true);
			}
		}

		#ifndef SERVER
		EffectSound sound = SEffectManager.PlaySound(CAR_BATTERY_ATTACH_SOUND, GetPosition());
		sound.SetAutodestroy( true );
		#endif

		if (GetGame().IsServer() && GetCompEM().CanWork() && GetAttachmentByType(ExpansionCircuitBoardBase))
		{
			if (GetExpansionSettings().GetGarage().ParkingMeterEnableFlavor)
				ExpansionItemBaseModule.s_Instance.PlaySound(GetPosition(), "Expansion_ParkingMeter_Activated_SoundSet");
		}
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		
		if (item.IsInherited(CarBattery))
			GetCompEM().SwitchOff();
		
		if (GetGame().IsServer())
		{
			ExpansionCircuitBoardBase board;
			if (Class.CastTo(board, item))
			{
				int slot_id_board = InventorySlots.GetSlotIdFromString("Att_ExpansionCircuitBoard");
				GetInventory().SetSlotLock(slot_id_board, false);
			}
		}

		#ifndef SERVER
		EffectSound sound = SEffectManager.PlaySound(CAR_BATTERY_DETACH_SOUND, GetPosition());
		sound.SetAutodestroy(true);
		#endif
	}
	
	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
		super.EEHealthLevelChanged(oldLevel, newLevel, zone);
		
		//! If object is destroyed drop all stored vehicles and destroy them.
		if (GetGame().IsServer() && GetGame().IsMultiplayer() && m_GarageModule && zone == "" && newLevel == GameConstants.STATE_RUINED)
		{
			DropStoredVehicles(true);
		}
	}
	
	override void DestroyConstruction()
	{		
		if (GetGame().IsServer())
		{
			//! Drop objects in inventory
			MiscGameplayFunctions.DropAllItemsInInventoryInBounds(this, Vector(0.8, 0.15, 1.3));

			DropStoredVehicles();
		}
		
		super.DestroyConstruction();
	}

	int GetCircuitBoardTier(string typeName)
	{
		int tier = -1;
		typeName.ToLower();
		typeName.Replace("expansioncircuitboard_mk", "");
		tier = typeName.ToInt();
		return tier;
	}

	bool HasCarBattery()
	{
		int slot = InventorySlots.GetSlotIdFromString("CarBattery");

		CarBattery carBattery;
		if (Class.CastTo(carBattery, GetInventory().FindAttachment(slot)))
			return true;

		return false;
	}


	protected void UpdateVisuals(ExpansionParkingMeterState state)
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(UpdateVisuals_Deferred, 0, false, state);
	}

	protected void UpdateVisuals_Deferred(ExpansionParkingMeterState state)
	{
		//ShowSelection("light");
		switch (state)
		{
			case ExpansionParkingMeterState.OFF:
			{
				SetObjectMaterial(1, "");
			}
			break;
			case ExpansionParkingMeterState.ACTIVE:
			{
				SetObjectMaterial(1, GREEN_LIGHT_GLOW);
			}
			break;
			case ExpansionParkingMeterState.LOW_ENERGY:
			{
				SetObjectMaterial(1, YELLOW_LIGHT_GLOW);
			}
			break;
			case ExpansionParkingMeterState.NO_ENERGY:
			{
				SetObjectMaterial(1, RED_LIGHT_GLOW);
			}
			break;
			case ExpansionParkingMeterState.EVR:
			{
				SetObjectMaterial(1, BLUE_LIGHT_GLOW);
			}
			break;
		}
	}

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return;

		ctx.Write(m_TerritoryID);
	}

	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return true;

		if (!ctx.Read(m_TerritoryID))
			return false;

		return true;
	}
	#endif

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();

		if (m_TerritoryID > -1)
		{
			m_GarageModule.AddTerritoryParkingMeter(this, m_TerritoryID);
		}
		else
		{
			m_GarageModule.AddParkingMeter(this);
		}
	}

	float GetRadiusByCircuitBoardType()
	{
		auto settings = GetExpansionSettings().GetGarage();
		int slot_id_board = InventorySlots.GetSlotIdFromString("Att_ExpansionCircuitBoard");
		auto attachedBoard = ExpansionCircuitBoardBase.Cast(GetInventory().FindAttachment(slot_id_board));

		if (attachedBoard)
		{
			string typeName = attachedBoard.GetType();
			typeName.ToLower();

			switch (typeName)
			{
				case "expansioncircuitboard_mk1":
					return settings.MaxRangeTier1;
				case "expansioncircuitboard_mk2":
					return settings.MaxRangeTier2;
				case "expansioncircuitboard_mk3":
					return settings.MaxRangeTier3;
			}
		}

		return settings.VehicleSearchRadius;
	}

	int GetStorageSizeByCircuitBoardType()
	{
		auto settings = GetExpansionSettings().GetGarage();
		int slot_id_board = InventorySlots.GetSlotIdFromString("Att_ExpansionCircuitBoard");
		auto attachedBoard = ExpansionCircuitBoardBase.Cast(GetInventory().FindAttachment(slot_id_board));

		if (attachedBoard)
		{
			string typeName = attachedBoard.GetType();
			typeName.ToLower();

			switch (typeName)
			{
				case "expansioncircuitboard_mk1":
					return settings.MaxStorableTier1;
				case "expansioncircuitboard_mk2":
					return settings.MaxStorableTier2;
				case "expansioncircuitboard_mk3":
					return settings.MaxStorableTier3;
			}
		}

		return settings.MaxStorableVehicles;
	}
	
	bool HasAttachedBoard()
	{
		auto settings = GetExpansionSettings().GetGarage();
		int slot_id_board = InventorySlots.GetSlotIdFromString("Att_ExpansionCircuitBoard");
		auto attachedBoard = ExpansionCircuitBoardBase.Cast(GetInventory().FindAttachment(slot_id_board));
		if (attachedBoard)
			return true;
		
		return false;
	}
	
	override string GetConstructionKitType()
	{
		return "ExpansionParkingMeterKit";
	}
	
	bool IsEVRStormActive()
	{
	#ifdef NAMALSK_SURVIVAL
	    NamEventManager event_manager;
	    g_Script.CallFunction(GetGame().GetMission(), "GetNamEventManager", event_manager, null);
	
	    if (!event_manager)
	        return false;
	
	    return EVRStorm.Cast(event_manager.GetEvent(EVRStorm)) || EVRStorm.Cast(event_manager.GetEvent(EVRStormDeadly));
	#endif
	
		return false;
	}
	
	void DropStoredVehicles(bool destroy = false)
	{
		//! Drop all stored vehicles.
		if (m_TerritoryID > -1)
			m_GarageModule.DropTerritoryVehicles(m_TerritoryID, destroy);
		else
			m_GarageModule.DropParkingMeterVehicles(GetPosition(), GetRadiusByCircuitBoardType(), destroy);
	}
	
	override void OnWork(float consumed_energy)
	{
		if (!GetGame().IsServer())
			return;

		int slot = InventorySlots.GetSlotIdFromString("CarBattery");
		CarBattery carBattery;
		if (Class.CastTo(carBattery, GetInventory().FindAttachment(slot)))
		{
			bool isEVRStormActive = IsEVRStormActive();
			bool isUnderRoof = MiscGameplayFunctions.IsUnderRoof(this);
			
			World world = GetGame().GetWorld();
			
			Weather weather = GetGame().GetWeather();
			float overcast = weather.GetOvercast().GetActual();
			float rain = weather.GetRain().GetActual();
			float fog = weather.GetFog().GetActual();
			float sunshine = ExpansionMath.LinearConversion(0.3, 0.7, overcast, 1.0, 0.0) * Math.Min(1.3 - rain, 1.0) * Math.Min(1.3 - fog, 1.0);
			
			float energy_delta = consumed_energy;
			float max = carBattery.GetCompEM().GetEnergyMax();
			
			//! If no EVR storm, consume energy during cloudy/rainy/foggy weather, at night, or if object is in building
			if (!isEVRStormActive && (isUnderRoof || world.IsNight() || sunshine < 0.3))
			{
				if (!isUnderRoof)
					energy_delta *= 1.0 - sunshine;
			#ifdef DIAG
				EXTrace.Print(EXTrace.BASEBUILDING, this, "::OnWork - consuming energy " + energy_delta);
			#endif
				carBattery.GetCompEM().ConsumeEnergy(energy_delta);
			}
			else
			{
				//! Add energy to the battery during daytime if not under roof or during EVR storms
				energy_delta = m_ChargeEnergyPerSecond * (consumed_energy / GetCompEM().GetEnergyUsage());  //! Add charge each second (base value, influenced by EVR and sunshine)
				float health01 = GetHealth01("", "");
				energy_delta *= (0.5 + health01 * 0.5); //! 50% damage causes 75% efficiency
				if (isEVRStormActive)
					energy_delta * 2.0;  //! EVR gives a boost to recharging
				else
					energy_delta *= sunshine;
			#ifdef DIAG
				EXTrace.Print(EXTrace.BASEBUILDING, this, "::OnWork - recharging energy " + energy_delta);
			#endif
				carBattery.GetCompEM().AddEnergy(energy_delta);
			}
			
			float energy = carBattery.GetCompEM().GetEnergy();
			float frac = energy / max;
			ExpansionParkingMeterState state;
			if (frac <= 0)
				state = ExpansionParkingMeterState.NO_ENERGY;
			else if (isEVRStormActive)
				state = ExpansionParkingMeterState.EVR;
			else if (frac >= 0.3)
				state = ExpansionParkingMeterState.ACTIVE;
			else
				state = ExpansionParkingMeterState.LOW_ENERGY;
			
			UpdateVisuals(state);
		}
	}
	
	override void OnWorkStop()
	{
		super.OnWorkStop();
		
		ExpansionParkingMeterState state = ExpansionParkingMeterState.OFF;
		UpdateVisuals(state);
	}
};
#endif
