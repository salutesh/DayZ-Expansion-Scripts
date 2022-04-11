/*
 * Vehicle_ExpansionUAZ.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class Vehicle_ExpansionUAZ extends ExpansionVehicleCarBase
{
	void Vehicle_ExpansionUAZ()
	{
		m_dmgContactCoef = 0.040;

		m_EngineStartOK = "Hatchback_02_engine_start_SoundSet";
		m_EngineStartBattery = "Hatchback_02_engine_failed_start_battery_SoundSet";
		m_EngineStartPlug = "Hatchback_02_engine_failed_start_sparkplugs_SoundSet";
		m_EngineStartFuel = "Hatchback_02_engine_failed_start_fuel_SoundSet";
		m_EngineStopFuel = "offroad_engine_stop_fuel_SoundSet";

		m_CarDoorOpenSound = "offroad_door_open_SoundSet";
		m_CarDoorCloseSound = "offroad_door_close_SoundSet";
	}

	override string ExpansionGetWheelType(int slot_id)
	{
		return "ExpansionUAZWheel";
	}

	override int GetAnimInstance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetAnimInstance");
#endif

		return ExpansionVehicleAnimInstances.EXPANSION_UAZ;
	}

	override CarRearLightBase CreateRearLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "CreateRearLight");
#endif

		return CarRearLightBase.Cast(ScriptedLightBase.CreateLight(ExpansionRearCarLights));
	}

	override CarLightBase CreateFrontLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "CreateFrontLight");
#endif

		return CarLightBase.Cast(ScriptedLightBase.CreateLight(ExpansionCarFrontLight));
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
		case 2:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
		case 3:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_R;
		}

		return 0;
	}

	override string GetDoorSelectionNameFromSeatPos(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetDoorSelectionNameFromSeatPos").Add(posIdx);
#endif

		switch (posIdx)
		{
		case 0:
			return "uazdriverdoor";
			break;
		case 1:
			return "uazcodriverdoor";
			break;
		case 2:
			return "uazcargo1door";
			break;
		case 3:
			return "uazcargo2door";
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
			return "uazdriverdoor";
			break;
		case 1:
			return "uazcodriverdoor";
			break;
		case 2:
			return "uazcargo1door";
			break;
		case 3:
			return "uazcargo2door";
			break;
		}

		return super.GetDoorInvSlotNameFromSeatPos(posIdx);
	}

	override int GetCarDoorsState(string slotType)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetCarDoorsState").Add(slotType);
#endif

		CarDoor carDoor;
		Class.CastTo(carDoor, FindAttachmentBySlotName(slotType));
		if (!carDoor)
		{
			return CarDoorState.DOORS_MISSING;
		}

		switch (slotType)
		{
		case "uazdriverdoor":
		{
			if (GetAnimationPhase("uazdriverdoor") > 0)
			{
				return CarDoorState.DOORS_OPEN;
			}
			else
			{
				return CarDoorState.DOORS_CLOSED;
			}
			break;
		}
		case "uazcodriverdoor":
		{
			if (GetAnimationPhase("uazcodriverdoor") > 0)
			{
				return CarDoorState.DOORS_OPEN;
			}
			else
			{
				return CarDoorState.DOORS_CLOSED;
			}
			break;
		}
		case "uazcargo1door":
		{
			if (GetAnimationPhase("uazcargo1door") > 0)
			{
				return CarDoorState.DOORS_OPEN;
			}
			else
			{
				return CarDoorState.DOORS_CLOSED;
			}
			break;
		}
		case "uazcargo2door":
		{
			if (GetAnimationPhase("uazcargo2door") > 0)
			{
				return CarDoorState.DOORS_OPEN;
			}
			else
			{
				return CarDoorState.DOORS_CLOSED;
			}
			break;
		}
		case "uaztrunkdoor":
		{
			return CarDoorState.DOORS_CLOSED;
			// if ( GetAnimationPhase("uaztrunkdoor") > 0.5 ) {
			//	return CarDoorState.DOORS_OPEN;
			// } else {
			//	return CarDoorState.DOORS_CLOSED;
			// }
			break;
		}
		default:
		{
			return CarDoorState.DOORS_MISSING;
		}
		}

		return CarDoorState.DOORS_MISSING;
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
			if (attType == "CarRadiator" || attType == "CarBattery" || attType == "SparkPlug")
				return false;
		}

		return true;
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

			if (GetCarDoorsState("uazdriverdoor") == CarDoorState.DOORS_CLOSED)
			{
				newValue += 0.4;
			}

			if (GetCarDoorsState("uazcodriverdoor") == CarDoorState.DOORS_CLOSED)
			{
				newValue += 0.4;
			}

			if (GetCarDoorsState("uazcargo1door") == CarDoorState.DOORS_CLOSED)
			{
				newValue += 0.4;
			}

			if (GetCarDoorsState("uazcargo2door") == CarDoorState.DOORS_CLOSED)
			{
				newValue += 0.4;
			}

			if (newValue > 1)
				newValue = 1;

			return newValue;
		default:
			break;
		}
		return oldValue;
	}

	override bool CanReachDoorsFromSeat(string pDoorsSelection, int pCurrentSeat)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "CanReachDoorsFromSeat").Add(pDoorsSelection).Add(pCurrentSeat);
#endif

		switch (pCurrentSeat)
		{
		case 0:
		{
			if (pDoorsSelection == "uazdriverdoor")
			{
				return true;
			}
			break;
		}
		case 1:
		{
			if (pDoorsSelection == "uazcodriverdoor")
			{
				return true;
			}
			break;
		}
		case 2:
		{
			if (pDoorsSelection == "uazcargo1door")
			{
				return true;
			}
			break;
		}
		case 3:
		{
			if (pDoorsSelection == "uazcargo2door")
			{
				return true;
			}
			break;
		}
		}
		return false;
	}

	override string GetAnimSourceFromSelection(string selection)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetAnimSourceFromSelection").Add(selection);
#endif

		switch (selection)
		{
		case "uazdriverdoor":
			return "uazdriverdoor";
		case "uazcodriverdoor":
			return "uazcodriverdoor";
		case "uazcargo1door":
			return "uazcargo1door";
		case "uazcargo2door":
			return "uazcargo2door";
		case "uaztrunkdoor":
			return "uaztrunkdoor";
		}

		return "";
	}

	override bool CrewCanGetThrough(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CrewCanGetThrough").Add(posIdx);
#endif

		switch (posIdx)
		{
		case 0:
		{
			if (GetAnimationPhase("uazdriverdoor") > 0)
			{
				return true;
			}
			if (FindAttachmentBySlotName("uazdriverdoor") == NULL)
				return true;

			break;
		}
		case 1:
		{
			if (GetAnimationPhase("uazcodriverdoor") > 0)
			{
				return true;
			}
			if (FindAttachmentBySlotName("uazcodriverdoor") == NULL)
				return true;

			break;
		}
		case 2:
		{
			if (GetAnimationPhase("uazcargo1door") > 0)
			{
				return true;
			}
			if (FindAttachmentBySlotName("uazcargo1door") == NULL)
				return true;

			break;
		}
		case 3:
		{
			if (GetAnimationPhase("uazcargo2door") > 0)
			{
				return true;
			}
			if (FindAttachmentBySlotName("uazcargo2door") == NULL)
				return true;

			break;
		}
		}

		return false;
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

		return true;
	}

	override bool IsVitalRadiator()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalRadiator");
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

	override bool LeavingSeatDoesAttachment(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "LeavingSeatDoesAttachment").Add(posIdx);
#endif
		return false;
	}
};

class Vehicle_ExpansionUAZCargoRoofless extends Vehicle_ExpansionUAZ
{
	void Vehicle_ExpansionUAZCargoRoofless()
	{
		m_dmgContactCoef = 0.018;
	}
};
