/**
 * ExpansionMh6.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class Expansion_Mh6_Door_1_1 extends CarDoor
{
};

class Expansion_Mh6_Door_1_2 extends CarDoor
{
};

class Expansion_Mh6_Door_2_1 extends CarDoor
{
};

class Expansion_Mh6_Door_2_2 extends CarDoor
{
};

class ExpansionMh6Wreck : ExpansionHelicopterWreck
{
};

/**
 * @class		ExpansionMh6
 * @brief
 */
class ExpansionMh6 : ExpansionHelicopterScript
{
	void ExpansionMh6()
	{
		m_dmgContactCoef = 0.05;
		m_ExplosionSize = 10;
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

		return ExpansionVehicleAnimInstances.EXPANSION_MH6;
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

		return 10;
	}

	override bool CanReleaseAttachment(EntityAI attachment)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CanReleaseAttachment").Add(attachment);
#endif

		string attType = attachment.GetType();

		if (Expansion_EngineIsOn(1))
		{
			if (attType == "ExpansionHelicopterBattery" || attType == "CarRadiator" || attType == "ExpansionIgniterPlug" || attType == "ExpansionHydraulicHoses")
				return false;
		}

		return super.CanReleaseAttachment(attachment);
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

			if (GetCarDoorsState("mh6door_1_1") == CarDoorState.DOORS_CLOSED)
				newValue += 0.25;

			if (GetCarDoorsState("mh6door_1_2") == CarDoorState.DOORS_CLOSED)
				newValue += 0.25;

			if (GetCarDoorsState("mh6door_2_1") == CarDoorState.DOORS_CLOSED)
				newValue += 0.25;

			if (GetCarDoorsState("mh6door_2_2") == CarDoorState.DOORS_CLOSED)
				newValue += 0.25;

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
			return "mh6door_1_1";
			break;
		case 1:
			return "mh6door_1_2";
			break;
		case 2:
			return "mh6door_2_1";
			break;
		case 3:
			return "mh6door_2_2";
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
			return "mh6door_1_1";
			break;
		case 1:
			return "mh6door_1_2";
			break;
		case 2:
			return "mh6door_2_1";
			break;
		case 3:
			return "mh6door_2_2";
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
		if (!Class.CastTo(carDoor, FindAttachmentBySlotName(slotType)))
		{
			return CarDoorState.DOORS_MISSING;
		}

		switch (slotType)
		{
		case "mh6door_1_1":
		{
			if (GetAnimationPhase("mh6door_1_1") > 0.5)
			{
				return CarDoorState.DOORS_OPEN;
			}
			else
			{
				return CarDoorState.DOORS_CLOSED;
			}
			break;
		}
		case "mh6door_1_2":
		{
			if (GetAnimationPhase("mh6door_1_2") > 0.5)
			{
				return CarDoorState.DOORS_OPEN;
			}
			else
			{
				return CarDoorState.DOORS_CLOSED;
			}
			break;
		}
		case "mh6door_2_1":
		{
			if (GetAnimationPhase("mh6door_2_1") > 0.5)
			{
				return CarDoorState.DOORS_OPEN;
			}
			else
			{
				return CarDoorState.DOORS_CLOSED;
			}
			break;
		}
		case "mh6door_2_2":
		{
			if (GetAnimationPhase("mh6door_2_2") > 0.5)
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
		case 2:
			if (nextSeat == 3)
				return true;
			break;
		case 3:
			if (nextSeat == 2)
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
		{
			if (pDoorsSelection == "mh6door_1_1")
			{
				return true;
			}
			break;
		}
		case 1:
		{
			if (pDoorsSelection == "mh6door_1_2")
			{
				return true;
			}
			break;
		}
		case 2:
		{
			if (pDoorsSelection == "mh6door_2_1")
			{
				return true;
			}
			break;
		}
		case 3:
		{
			if (pDoorsSelection == "mh6door_2_2")
			{
				return true;
			}
			break;
		}
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
			if (GetAnimationPhase("mh6door_1_1") > 0.5)
				return true;

			if (FindAttachmentBySlotName("mh6door_1_1") == NULL)
				return true;
			break;

		case 1:
			if (GetAnimationPhase("mh6door_1_2") > 0.5)
				return true;

			if (FindAttachmentBySlotName("mh6door_1_2") == NULL)
				return true;
			break;

		case 2:
			if (GetAnimationPhase("mh6door_2_1") > 0.5)
				return true;

			if (FindAttachmentBySlotName("mh6door_2_1") == NULL)
				return true;
			break;

		case 3:
			if (GetAnimationPhase("mh6door_2_2") > 0.5)
				return true;

			if (FindAttachmentBySlotName("mh6door_2_2") == NULL)
				return true;
			break;
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
		case "mh6door_1_1":
			return "mh6door_1_1";
		case "mh6door_1_2":
			return "mh6door_1_2";
		case "mh6door_2_1":
			return "mh6door_2_1";
		case "mh6door_2_2":
			return "mh6door_2_2";
		}

		return "";
	}

	override void UpdateLights(int new_gear = -1)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "UpdateLights").Add(new_gear);
#endif

		if (IsMissionClient())
		{
			ItemBase battery;

			if (IsVitalHelicopterBattery())
				battery = ItemBase.Cast(FindAttachmentBySlotName("ExpansionHelicopterBattery"));

			if (battery)
			{
				if (m_HeadlightsOn)
				{
					if (!m_Headlight && m_HeadlightsState != CarHeadlightBulbsState.NONE)
					{
						m_Headlight = CreateFrontLight();

						m_Headlight.AttachOnMemoryPoint(this, CarScript.m_LeftHeadlightPoint, CarScript.m_LeftHeadlightTargetPoint);
						m_Headlight.SegregateLight();
						TailLightsShineOn();
					}

					if (m_HeadlightsState != CarHeadlightBulbsState.NONE)
					{
						LeftFrontLightShineOn();
						RightFrontLightShineOn();
					}

					if (m_Lights.Count() == 0)
					{
						CreateLights(this, "interior", ExpansionPointLight, Vector(1, 0.01, 0.01), Vector(1, 0.01, 0.01), 3, 0.35, false, true);
					}
				}
				else
				{
					LeftFrontLightShineOff();
					RightFrontLightShineOff();

					if (m_Headlight)
					{
						m_Headlight.FadeOut();
						m_Headlight = null;
					}

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
};
