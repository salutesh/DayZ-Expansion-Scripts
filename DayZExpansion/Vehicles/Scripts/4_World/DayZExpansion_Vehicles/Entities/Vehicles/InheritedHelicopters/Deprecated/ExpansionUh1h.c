/**
 * ExpansionUh1h.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class ExpansionUh1hDoor_1_1 extends CarDoor
{
};

class ExpansionUh1hDoor_1_2 extends CarDoor
{
};

class ExpansionUh1hDoor_2_1 extends CarDoor
{
};

class ExpansionUh1hDoor_2_2 extends CarDoor
{
};

class ExpansionUh1hWreck : ExpansionHelicopterWreck
{
};

/**
 * @class		ExpansionUh1h
 * @brief
 */
class ExpansionUh1h : ExpansionHelicopterScript
{
	void ExpansionUh1h()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "ExpansionGyrocopter");
#endif

		m_dmgContactCoef = 0.06;
		m_ExplosionSize = 20;
	}

	override CarLightBase CreateFrontLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "CreateFrontLight");
#endif

		return CarLightBase.Cast(ScriptedLightBase.CreateLight(ExpansionHelicopterFrontLight));
	}

	override int GetAnimInstance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetAnimInstance");
#endif

		return ExpansionVehicleAnimInstances.EXPANSION_UH1H;
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
		case 4:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
		case 5:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_R;
		}

		return 0;
	}

	override int Get3rdPersonCameraType()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "Get3rdPersonCameraType");
#endif

		return DayZPlayerCameras.DAYZCAMERA_3RD_VEHICLE;
	}

	override bool IsVitalIgniterPlug()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalIgniterPlug");
#endif

		return true;
	}

	override bool IsVitalHydraulicHoses()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalHydraulicHoses");
#endif

		return true;
	}

	override float GetActionDistanceFuel()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetActionDistanceFuel");
#endif

		return 2.5;
	}

	override float GetWreckAltitude()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetWreckAltitude");
#endif

		return 1.5;
	}

	override float GetCameraHeight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetCameraHeight");
#endif

		return 1;
	}

	override float GetCameraDistance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetCameraDistance");
#endif

		return 15;
	}

	override string GetActionCompNameFuel()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetActionCompNameFuel");
#endif

		return "refill";
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

			if (GetCarDoorsState("uh1hdoor_1_1") == CarDoorState.DOORS_CLOSED)
				newValue += 0.5;

			if (GetCarDoorsState("uh1hdoor_1_2") == CarDoorState.DOORS_CLOSED)
				newValue += 0.5;
			
			if (GetCarDoorsState("uh1hdoor_2_1") == CarDoorState.DOORS_CLOSED)
				newValue += 0.5;

			if (GetCarDoorsState("uh1hdoor_2_2") == CarDoorState.DOORS_CLOSED)
				newValue += 0.5;

			if (newValue > 1)
				newValue = 1;

			return newValue;
			break;
		}

		return super.OnSound(ctrl, oldValue);
	}

	override string GetDoorSelectionNameFromSeatPos(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetDoorSelectionNameFromSeatPos").Add(posIdx);
#endif

		switch (posIdx)
		{
		case 0:
			return "uh1hdoor_1_1";
			break;
		case 1:
			return "uh1hdoor_1_2";
			break;
		case 2:
			return "uh1hdoor_2_1";
			break;
		case 3:
			return "uh1hdoor_2_2";
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
			return "uh1hdoor_1_1";
			break;
		case 1:
			return "uh1hdoor_1_2";
			break;
		case 2:
			return "uh1hdoor_2_1";
			break;
		case 3:
			return "uh1hdoor_2_2";
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
		case "uh1hdoor_1_1":
		{
			if (GetAnimationPhase("uh1hdoor_1_1") > 0.5)
			{
				return CarDoorState.DOORS_OPEN;
			}
			else
			{
				return CarDoorState.DOORS_CLOSED;
			}
			break;
		}
		case "uh1hdoor_1_2":
		{
			if (GetAnimationPhase("uh1hdoor_1_2") > 0.5)
			{
				return CarDoorState.DOORS_OPEN;
			}
			else
			{
				return CarDoorState.DOORS_CLOSED;
			}
			break;
		}
		case "uh1hdoor_2_1":
		{
			if (GetAnimationPhase("uh1hdoor_2_1") > 0.5)
			{
				return CarDoorState.DOORS_OPEN;
			}
			else
			{
				return CarDoorState.DOORS_CLOSED;
			}
			break;
		}
		case "uh1hdoor_2_2":
		{
			if (GetAnimationPhase("uh1hdoor_2_2") > 0.5)
			{
				return CarDoorState.DOORS_OPEN;
			}
			else
			{
				return CarDoorState.DOORS_CLOSED;
			}
			break;
		}
		
		default:
		{
			return CarDoorState.DOORS_MISSING;
		}
		}

		return CarDoorState.DOORS_MISSING;
	}

	override bool CanReachDoorsFromSeat(string pDoorsSelection, int pCurrentSeat)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "CanReachDoorsFromSeat").Add(pDoorsSelection).Add(pCurrentSeat);
#endif

		switch (pCurrentSeat)
		{
		case 0:
			if (pDoorsSelection == "uh1hdoor_1_1")
			{
				return true;
			}
			break;
		case 1:
			if (pDoorsSelection == "uh1hdoor_1_2")
			{
				return true;
			}
			break;
		case 2:
			if (pDoorsSelection == "uh1hdoor_2_1")
			{
				return true;
			}
			break;
		case 3:
			if (pDoorsSelection == "uh1hdoor_2_2")
			{
				return true;
			}
			break;
		}
		return false;
	}
    
	override bool CrewCanGetThrough(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CrewCanGetThrough").Add(posIdx);
#endif

		switch (posIdx)
		{
		case 0:
			if (GetCarDoorsState("uh1hdoor_1_1") == CarDoorState.DOORS_CLOSED)
				return false;

			return true;
			break;

		case 1:
			if (GetCarDoorsState("uh1hdoor_1_2") == CarDoorState.DOORS_CLOSED)
				return false;

			return true;
			break;
		case 2:
			if (GetCarDoorsState("uh1hdoor_2_1") == CarDoorState.DOORS_CLOSED)
				return false;

			return true;
			break;
		case 3:
			if (GetCarDoorsState("uh1hdoor_2_2") == CarDoorState.DOORS_CLOSED)
				return false;

			return true;
			break;
		}
		return true;
	}

	override string GetAnimSourceFromSelection(string selection)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetAnimSourceFromSelection").Add(selection);
#endif

		switch (selection)
		{
		case "uh1hdoor_1_1":
			return "uh1hdoor_1_1";
		case "uh1hdoor_1_2":
			return "uh1hdoor_1_2";
		case "uh1hdoor_2_1":
			return "uh1hdoor_2_1";
		case "uh1hdoor_2_2":
			return "uh1hdoor_2_2";
		}

		return "";
	}

	override void UpdateLights(int new_gear = -1)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "UpdateLights").Add(new_gear);
#endif

		super.UpdateLights(new_gear);

		if (IsMissionClient())
		{
			ItemBase battery;

			if (IsVitalHelicopterBattery())
				battery = ItemBase.Cast(FindAttachmentBySlotName("ExpansionHelicopterBattery"));

			if (battery)
			{
				if (m_HeadlightsOn)
				{
					if (m_Lights.Count() == 0)
					{
						CreateLights(this, "interior", ExpansionPointLight, Vector(0.1, 1, 0.25), Vector(0.1, 1, 0.25), 2.3, 0.1, false, true);
					}
				}
				else
				{
					for (int b = 0; b < m_Lights.Count(); b++)
					{
						m_Lights[b].ExpansionSetEnabled(false);

						GetGame().ObjectDelete(m_Lights[b]);
					}

					m_Lights.Clear();
				}
			}
		}
	}

	override void OnDebugSpawn()
	{
		EntityAI entity;

		if (Class.CastTo(entity, this))
		{
			entity.GetInventory().CreateInInventory("ExpansionUh1hDoor_1_1");
			entity.GetInventory().CreateInInventory("ExpansionUh1hDoor_1_2");
			entity.GetInventory().CreateInInventory("ExpansionUh1hDoor_2_1");
			entity.GetInventory().CreateInInventory("ExpansionUh1hDoor_2_2");

			entity.GetInventory().CreateInInventory("ExpansionHydraulicHoses");
			entity.GetInventory().CreateInInventory("ExpansionIgniterPlug");
			entity.GetInventory().CreateInInventory("ExpansionHelicopterBattery");
			entity.GetInventory().CreateInInventory("HeadlightH7");
		}

		Fill(CarFluid.FUEL, 50);
		Fill(CarFluid.COOLANT, 6.0);
		Fill(CarFluid.OIL, 4.0);
	}
};
