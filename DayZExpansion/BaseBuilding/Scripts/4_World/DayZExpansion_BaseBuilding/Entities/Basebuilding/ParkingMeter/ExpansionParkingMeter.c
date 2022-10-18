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
class ExpansionParkingMeter extends ItemBase
{
	private static ref set<ExpansionParkingMeter> s_Expansion_ParkingMeters = new set<ExpansionParkingMeter>;

	static const string CAR_BATTERY_ATTACH_SOUND = "carbattery_attach_SoundSet";
	static const string CAR_BATTERY_DETACH_SOUND = "carbattery_detach_SoundSet";
	static const string RED_LIGHT_GLOW = "dz\\gear\\camping\\data\\battery_charger_light_r.rvmat";
	static const string GREEN_LIGHT_GLOW = "dz\\gear\\camping\\data\\battery_charger_light_g.rvmat";

	protected ExpansionTerritoryModule m_TerritoryModule;
	protected ExpansionGarageModule m_GarageModule;
	protected int m_TerritoryID;	 //! Unique terriotry id. Used to get and identify the parking meters territory in the territory and garage module.

	void ExpansionParkingMeter()
	{
		CF_Modules<ExpansionTerritoryModule>.Get(m_TerritoryModule);
		CF_Modules<ExpansionGarageModule>.Get(m_GarageModule);
		
		m_TerritoryID = -1;

		RegisterNetSyncVariableInt("m_TerritoryID");
		s_Expansion_ParkingMeters.Insert(this);
	}

	void ~ExpansionParkingMeter()
	{
		if (!GetGame())
			return;

		if (GetGame().IsServer() && GetGame().IsMultiplayer() && ExpansionGarageModule.s_Instance && m_TerritoryID > -1)
		{
			ExpansionGarageModule.s_Instance.RemoveParkingMeter(m_TerritoryID);
		}

		int i = s_Expansion_ParkingMeters.Find(this);
		if (i >= 0)
		{
			s_Expansion_ParkingMeters.Remove(i);
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
		m_GarageModule.AddParkingMeter(this, m_TerritoryID);
	}

	bool IsInTerritory()
	{
		if (!m_TerritoryModule)
			return false;

		return m_TerritoryModule.IsInTerritory(GetPosition());
	}

	override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI player)
	{
		return false;
	}

	/*override bool CanDisplayAttachmentSlot(int slot_id)
	{
		if (!super.CanDisplayAttachmentSlot(slot_id))
			return false;
		
		int slot_id_board = InventorySlots.GetSlotIdFromString("Att_ExpansionCircuitBoard");
		if (slot_id == slot_id_board && !HasAttachedBoard())
			return false;

		return true;
	}*/

	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);

		if (item.IsInherited(CarBattery))
		{
			if (GetCompEM().CanWork())
			{
				GetCompEM().SwitchOn();
				UpdateVisuals();
			}
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
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		if (item.IsInherited(CarBattery))
		{
			GetCompEM().SwitchOff();
			UpdateVisuals();
		}
		
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


	void UpdateVisuals()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(UpdateVisuals_Deferred, 0, false);
	}

	protected void UpdateVisuals_Deferred()
	{
		HideSelection("light_green");

		if (GetCompEM().CanWork())
		{
			EXTrace.Print(EXTrace.BASEBUILDING, this, "::UpdateVisuals - Working");
			HideSelection("light_red");
			ShowSelection("light_green");
			SetObjectMaterial(1, GREEN_LIGHT_GLOW);
			SetObjectMaterial(2, "");
		}
		else
		{
			EXTrace.Print(EXTrace.BASEBUILDING, this, "::UpdateVisuals - Not working");
			HideSelection("light_green");
			ShowSelection("light_red");
			SetObjectMaterial(1, "");
			SetObjectMaterial(2, RED_LIGHT_GLOW);
		}
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		UpdateVisuals();
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
			m_GarageModule.AddParkingMeter(this, m_TerritoryID);

		UpdateVisuals();
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
};
#endif

class ExpansionCircuitBoardBase extends ItemBase
{
	int GetTier()
	{
		return -1;
	}
};

class ExpansionCircuitBoard_MK1 extends ExpansionCircuitBoardBase
{
	override int GetTier()
	{
		return 1;
	}
};
class ExpansionCircuitBoard_MK2 extends ExpansionCircuitBoardBase
{
	override int GetTier()
	{
		return 2;
	}
};
class ExpansionCircuitBoard_MK3 extends ExpansionCircuitBoardBase
{
	override int GetTier()
	{
		return 3;
	}
};
