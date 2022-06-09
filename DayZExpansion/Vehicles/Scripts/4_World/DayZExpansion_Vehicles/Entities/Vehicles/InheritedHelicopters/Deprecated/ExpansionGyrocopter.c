/**
 * ExpansionGyrocopter.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class ExpansionGyrocopterWreck : ExpansionHelicopterWreck
{
};

/**
 * @class		ExpansionGyrocopter
 * @brief
 */
class ExpansionGyrocopter : ExpansionHelicopterScript
{
	void ExpansionGyrocopter()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "ExpansionGyrocopter");
#endif

		//! Vanilla
		m_dmgContactCoef = 0.05;

		//! explosions
		m_ExplosionSize = 2;

		m_MaxSpeed = 100.0;

		m_AltitudeFullForce = 1000.0;
		m_AltitudeNoForce = 2000.0;

		m_EngineStartDuration = 5.0;

		m_MinAutoRotateSpeed = 2.0;
		m_MaxAutoRotateSpeed = 10.0;

		m_BodyFrictionCoef = 4;
		m_LiftForceCoef = 1.6;

		m_CyclicForceCoef = 1.5; // All rotations

		m_CyclicSideSpeed = 4.0;
		m_CyclicSideMax = 1.0;	// Roll, A/D, Speed
		m_CyclicSideCoef = 0.4; // Roll, A/D, Speed

		m_CyclicForwardSpeed = 10.0;
		m_CyclicForwardMax = 1.0;  // Pitch, W/S, Speed
		m_CyclicForwardCoef = 0.3; // Pitch, W/S, Speed

		m_AntiTorqueSpeed = 1.5;
		m_AntiTorqueMax = 0.04; // Yaw, Q/E, same as m_TailForceCoef

		m_BankForceCoef = 0.1; // Speed at which heli will rotate when rotor is banking left or right
		m_TailForceCoef = 8.0; // Yaw, Q/E, same as m_AntiTorqueMax
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

		return ExpansionVehicleAnimInstances.EXPANSION_GYROCOPTER;
	}

	override int GetSeatAnimationType(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetSeatAnimationType").Add(posIdx);
#endif

		return DayZPlayerConstants.VEHICLESEAT_DRIVER;
	}

	override int Get3rdPersonCameraType()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "Get3rdPersonCameraType");
#endif

		return DayZPlayerCameras.DAYZCAMERA_3RD_VEHICLE;
	}

	override bool IsVitalSparkPlug()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalSparkPlug");
#endif

		return true;
	}

	override float GetActionDistanceFuel()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetActionDistanceFuel");
#endif

		return 4.5;
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

		return 6;
	}

	override string GetActionCompNameFuel()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetActionCompNameFuel");
#endif

		return "refill";
	}

	override string GetDoorSelectionNameFromSeatPos(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetDoorSelectionNameFromSeatPos").Add(posIdx);
#endif
		
		return "";
	}

	override string GetDoorInvSlotNameFromSeatPos(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetDoorInvSlotNameFromSeatPos").Add(posIdx);
#endif

		return "";
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

		return CarDoorState.DOORS_MISSING;
	}

	override bool CanReachSeatFromSeat(int currentSeat, int nextSeat)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "CanReachSeatFromSeat").Add(currentSeat).Add(nextSeat);
#endif

		return false;
	}

	override bool CanReachDoorsFromSeat(string pDoorsSelection, int pCurrentSeat)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "CanReachDoorsFromSeat").Add(pDoorsSelection).Add(pCurrentSeat);
#endif

		return false;
	}

	override bool CrewCanGetThrough(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CrewCanGetThrough").Add(posIdx);
#endif

		return true;
	}

	override string GetAnimSourceFromSelection(string selection)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetAnimSourceFromSelection").Add(selection);
#endif

		return "";
	}

	override void OnDebugSpawn()
	{
		EntityAI entity;

		if (Class.CastTo(entity, this))
		{
			entity.GetInventory().CreateInInventory("ExpansionHelicopterBattery");
			entity.GetInventory().CreateInInventory("SparkPlug");
			entity.GetInventory().CreateInInventory("HeadlightH7");
		}

		Fill(CarFluid.FUEL, 50);
		Fill(CarFluid.COOLANT, 6.0);
		Fill(CarFluid.OIL, 4.0);
	}
};
