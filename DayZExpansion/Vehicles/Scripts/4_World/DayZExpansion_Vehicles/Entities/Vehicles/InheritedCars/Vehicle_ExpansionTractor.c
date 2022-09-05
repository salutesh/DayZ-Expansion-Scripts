/*
 * Vehicle_ExpansionTractor.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class Vehicle_ExpansionTractor extends ExpansionVehicleCarBase
{
	void Vehicle_ExpansionTractor()
	{
		m_dmgContactCoef = 0.075;

		m_EngineStartOK = "Truck_01_engine_start_SoundSet";
		m_EngineStartBattery = "Truck_01_engine_failed_start_battery_SoundSet";
		m_EngineStartPlug = "Truck_01_engine_failed_start_sparkplugs_SoundSet";
		m_EngineStartFuel = "Truck_01_engine_failed_start_fuel_SoundSet";
		m_EngineStopFuel = "Truck_01_engine_stop_fuel_SoundSet";

		m_CarDoorOpenSound = "Truck_01_door_open_SoundSet";
		m_CarDoorCloseSound = "Truck_01_door_close_SoundSet";
	}

	override vector GetTransportCameraOffset()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetTransportCameraOffset");
#endif

		return "0 2.5 0";
	}

	override float GetTransportCameraDistance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetTransportCameraDistance");
#endif

		return 3.5;
	}

	override int GetAnimInstance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetAnimInstance");
#endif

		return ExpansionVehicleAnimInstances.EXPANSION_TRACTOR;
	}

	override int GetSeatAnimationType(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetSeatAnimationType").Add(posIdx);
#endif

		return DayZPlayerConstants.VEHICLESEAT_DRIVER;
	}

	override CarLightBase CreateFrontLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "CreateFrontLight");
#endif

		return CarLightBase.Cast(ScriptedLightBase.CreateLight(ExpansionRearCarLights));
	}

	override CarRearLightBase CreateRearLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "CreateRearLight");
#endif

		return CarRearLightBase.Cast(ScriptedLightBase.CreateLight(ExpansionCarFrontLight));
	}

	override string ExpansionGetWheelType(int slot_id)
	{
		if (InventorySlots.GetSlotName(slot_id) == "ExpansionTractorFrontWheel_1_1" || InventorySlots.GetSlotName(slot_id) == "ExpansionTractorFrontWheel_2_1")
			return "ExpansionTractorFrontWheel";
		else
			return "ExpansionTractorBackWheel";
	}

	override bool CanReleaseAttachment(EntityAI attachment)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CanReleaseAttachment").Add(attachment);
#endif

		if (!super.CanReleaseAttachment(attachment))
			return false;

		string attType = attachment.GetType();

		if (EngineIsOn())
		{
			if (attType == "CarBattery")
				return false;
		}

		return true;
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
		case "ExpansionTractorDoorsDriver":
			if (GetAnimationPhase("ExpansionTractorDoorsDriver") > 0.5)
				return CarDoorState.DOORS_OPEN;
			else
				return CarDoorState.DOORS_CLOSED;
			break;

		case "ExpansionTractorDoorsCodriver":
			if (GetAnimationPhase("ExpansionTractorDoorsCodriver") > 0.5)
				return CarDoorState.DOORS_OPEN;
			else
				return CarDoorState.DOORS_CLOSED;
		}

		return CarDoorState.DOORS_MISSING;
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
			if (GetCarDoorsState("ExpansionTractorDoorsDriver") == CarDoorState.DOORS_CLOSED)
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

		if (GetAnimationPhase("ExpansionTractorDoorsCodriver") > 0.5)
			return "expansiontractordoorscodriver";

		return "expansiontractordoorsdriver";
	}

	override string GetDoorInvSlotNameFromSeatPos(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetDoorInvSlotNameFromSeatPos").Add(posIdx);
#endif

		return "ExpansionTractorDoorsDriver";

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
			if (GetCarDoorsState("ExpansionTractorDoorsDriver") == CarDoorState.DOORS_CLOSED)
			{
				newValue += 0.4;
			}

			if (GetCarDoorsState("ExpansionTractorDoorsCodriver") == CarDoorState.DOORS_CLOSED)
			{
				newValue += 0.4;
			}

			if (newValue > 1)
				newValue = 1;
			return newValue;
		default:
			break;
		}

		return super.OnSound(ctrl, oldValue);
	}

	override string GetAnimSourceFromSelection(string selection)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetAnimSourceFromSelection").Add(selection);
#endif

		switch (selection)
		{
		case "expansiontractordoorsdriver":
			return "ExpansionTractorDoorsDriver";
		case "expansiontractordoorscodriver":
			return "ExpansionTractorDoorsCodriver";
		}
		return "";
	}

	override bool IsVitalCarBattery()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalHelicopterBattery");
#endif

		return true;
	}

	override bool IsVitalTruckBattery()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalTruckBattery");
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

	override bool LeavingSeatDoesAttachment(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "LeavingSeatDoesAttachment").Add(posIdx);
#endif

		return false;
	}

	override bool CanReachDoorsFromSeat(string pDoorsSelection, int pCurrentSeat)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "CanReachDoorsFromSeat").Add(pDoorsSelection).Add(pCurrentSeat);
#endif

		if (pDoorsSelection == "ExpansionTractorDoorsDriver")
			return true;
		if (pDoorsSelection == "ExpansionTractorDoorsCodriver")
			return true;

		return false;
	}
};
