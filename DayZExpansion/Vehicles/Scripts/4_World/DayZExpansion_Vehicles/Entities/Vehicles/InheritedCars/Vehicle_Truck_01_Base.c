/**
 * Vehicle_Truck_01_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class Vehicle_Truck_01_Base: ExpansionVehicleCarBase
{
	override bool LeavingSeatDoesAttachment(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "LeavingSeatDoesAttachment").Add(posIdx);
#endif

		return false;
	}

	void Vehicle_Truck_01_Base()
	{
		m_dmgContactCoef = 0.018;
		m_enginePtcPos = "0 1.346 2.205";

		m_EngineStartOK = "Truck_01_engine_start_SoundSet";
		m_EngineStartBattery = "Truck_01_engine_failed_start_battery_SoundSet";
		m_EngineStartPlug = "Truck_01_engine_failed_start_sparkplugs_SoundSet";
		m_EngineStartFuel = "Truck_01_engine_failed_start_fuel_SoundSet";
		m_EngineStopFuel = "Truck_01_engine_stop_fuel_SoundSet";

		m_CarDoorOpenSound = "Truck_01_door_open_SoundSet";
		m_CarDoorCloseSound = "Truck_01_door_close_SoundSet";
	}

	override float GetTransportCameraDistance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetTransportCameraDistance");
#endif

		return 6.0;
	}

	override vector GetTransportCameraOffset()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetTransportCameraOffset");
#endif

		return "0 3.2 0";
	}

	override int GetAnimInstance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetAnimInstance");
#endif

		return VehicleAnimInstances.V3S;
	}

	override int GetSeatAnimationType(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetSeatAnimationType").Add(posIdx);
#endif

		switch (posIdx)
		{
		case 0:
			return DayZPlayerConstants.VEHICLESEAT_DRIVER;
		case 1:
			return DayZPlayerConstants.VEHICLESEAT_CODRIVER;
		}

		return 0;
	}

	override CarLightBase CreateFrontLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "CreateFrontLight");
#endif

		return CarLightBase.Cast(ScriptedLightBase.CreateLight(Truck_01FrontLight));
	}

	override CarRearLightBase CreateRearLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "CreateRearLight");
#endif
		return CarRearLightBase.Cast(ScriptedLightBase.CreateLight(Truck_01RearLight));
	}

	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_3(ExpansionTracing.VEHICLES, this, "EEHealthLevelChanged").Add(oldLevel).Add(newLevel).Add(zone);
#endif

		super.EEHealthLevelChanged(oldLevel, newLevel, zone);
	}

	override bool CrewCanGetThrough(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CrewCanGetThrough").Add(posIdx);
#endif

		CarDoor carDoor;
		switch (posIdx)
		{
		case 0:
			if (GetCarDoorsState("Truck_01_Door_1_1") == CarDoorState.DOORS_CLOSED)
				return false;

			return true;

		case 1:
			if (GetCarDoorsState("Truck_01_Door_2_1") == CarDoorState.DOORS_CLOSED)
				return false;

			return true;
		}

		return false;
	}

	override string GetDoorSelectionNameFromSeatPos(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetDoorSelectionNameFromSeatPos").Add(posIdx);
#endif

		switch (posIdx)
		{
		case 0:
			return "doors_driver";
			break;
		case 1:
			return "doors_codriver";
			break;
		}

		return super.GetDoorSelectionNameFromSeatPos(posIdx);
	}

	override string GetDoorInvSlotNameFromSeatPos(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetDoorInvSlotNameFromSeatPos").Add(posIdx);
#endif

		switch (posIdx)
		{
		case 0:
			return "Truck_01_Door_1_1";
			break;
		case 1:
			return "Truck_01_Door_2_1";
			break;
		}

		return super.GetDoorInvSlotNameFromSeatPos(posIdx);
	}

	override float OnSound(CarSoundCtrl ctrl, float oldValue)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "OnSound").Add(ctrl).Add(oldValue);
#endif

		switch (ctrl)
		{
		case CarSoundCtrl.DOORS:
			float newValue = 0;

			//-----
			if (GetCarDoorsState("Truck_01_Door_1_1") == CarDoorState.DOORS_CLOSED)
			{
				newValue += 0.4;
			}

			if (GetCarDoorsState("Truck_01_Door_2_1") == CarDoorState.DOORS_CLOSED)
			{
				newValue += 0.4;
			}

			if (newValue > 1)
				newValue = 1;

			return newValue;
			break;
		}

		return super.OnSound(ctrl, oldValue);
	}

	override int GetCarDoorsState(string slotType)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetCarDoorsState").Add(slotType);
#endif

		CarDoor carDoor;
		Class.CastTo(carDoor, FindAttachmentBySlotName(slotType));
		if (!carDoor)
			return CarDoorState.DOORS_MISSING;

		switch (slotType)
		{
		case "Truck_01_Door_1_1":
			if (GetAnimationPhase("DoorsDriver") > 0.5)
				return CarDoorState.DOORS_OPEN;
			else
				return CarDoorState.DOORS_CLOSED;
			break;

		case "Truck_01_Door_2_1":
			if (GetAnimationPhase("DoorsCoDriver") > 0.5)
				return CarDoorState.DOORS_OPEN;
			else
				return CarDoorState.DOORS_CLOSED;

			break;

		case "Truck_01_Hood":
			if (GetAnimationPhase("DoorsHood") > 0.5)
				return CarDoorState.DOORS_OPEN;
			else
				return CarDoorState.DOORS_CLOSED;
		}

		return CarDoorState.DOORS_MISSING;
	}

	override string GetAnimSourceFromSelection(string selection)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetAnimSourceFromSelection").Add(selection);
#endif

		switch (selection)
		{
		case "doors_driver":
			return "DoorsDriver";
		case "doors_codriver":
			return "DoorsCoDriver";
		case "doors_hood":
			return "DoorsHood";
		case "doors_trunk":
			return "DoorsTrunk";
		case "wheelsideplate1":
			return "WheelSidePlate1";
		case "wheelsideplate2":
			return "WheelSidePlate2";
		}

		return "";
	}

	override bool CanReachSeatFromSeat(int currentSeat, int nextSeat)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "CanReachSeatFromSeat").Add(currentSeat).Add(nextSeat);
#endif

		switch (currentSeat)
		{
		case 0:
			if (nextSeat == 1)
				return true;
			break;
		case 1:
			if (nextSeat == 0)
				return true;
			break;
		}

		return false;
	}

	override bool CanReachDoorsFromSeat(string pDoorsSelection, int pCurrentSeat)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "CanReachDoorsFromSeat").Add(pDoorsSelection).Add(pCurrentSeat);
#endif

		switch (pCurrentSeat)
		{
		case 0:
			if (pDoorsSelection == "DoorsDriver")
			{
				return true;
			}
			break;
		case 1:
			if (pDoorsSelection == "DoorsCoDriver")
			{
				return true;
			}
			break;
		}

		return false;
	}

	override int GetSeatIndexFromDoor(string pDoorSelection)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetSeatIndexFromDoor").Add(pDoorSelection);
#endif

		switch (pDoorSelection)
		{
		case "DoorsDriver":
			return 0;
			break;
		case "DoorsCoDriver":
			return 1;
			break;
		}
		return -1;
	}

	override bool IsVitalTruckBattery()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalTruckBattery");
#endif

		return true;
	}

	override bool IsVitalCarBattery()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalHelicopterBattery");
#endif

		return false;
	}

	override bool IsVitalSparkPlug()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalSparkPlug");
#endif

		return false;
	}

	override bool IsVitalGlowPlug()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalGlowPlug");
#endif

		return false;
	}

	override bool IsVitalEngineBelt()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalEngineBelt");
#endif

		return false;
	}

	override bool IsVitalRadiator()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalRadiator");
#endif

		return false;
	}

	override void SetActions()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "SetActions");
#endif

		super.SetActions();

		AddAction(ActionSidePlateInteract);
		AddAction(ActionDetachFromTarget_SpecificSlot_WoodenLogs);
		AddAction(ActionDetachFromTarget_SpecificSlot_WoodenPlanks);
		AddAction(ActionDetachFromTarget_SpecificSlot_MetalSheets);
		AddAction(ActionDetachFromTarget_SpecificSlotsCategory_Barrel);
		AddAction(ActionDetachFromTarget_SpecificSlotsCategory_WoodenCrate);
	}

	override void OnDebugSpawn()
	{
		EntityAI entity;

		if (Class.CastTo(entity, this))
		{
			entity.GetInventory().CreateInInventory("Truck_01_Wheel");
			entity.GetInventory().CreateInInventory("Truck_01_Wheel");
			entity.GetInventory().CreateInInventory("Truck_01_Wheel");
			entity.GetInventory().CreateInInventory("Truck_01_Wheel");

			entity.GetInventory().CreateInInventory("Truck_01_WheelDouble");
			entity.GetInventory().CreateInInventory("Truck_01_WheelDouble");
			entity.GetInventory().CreateInInventory("Truck_01_WheelDouble");
			entity.GetInventory().CreateInInventory("Truck_01_WheelDouble");

			entity.GetInventory().CreateInInventory("TruckBattery");

			entity.GetInventory().CreateInInventory("Truck_01_Door_1_1");
			entity.GetInventory().CreateInInventory("Truck_01_Door_2_1");
			entity.GetInventory().CreateInInventory("Truck_01_Hood");

			entity.GetInventory().CreateInInventory("HeadlightH7");
			entity.GetInventory().CreateInInventory("HeadlightH7");
		};

		Fill(CarFluid.FUEL, 120);
		Fill(CarFluid.OIL, 4.0);
	};
};
