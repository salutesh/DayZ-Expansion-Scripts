/**
 * ExpansionGyrocopter.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionGyrocopterWreck: ExpansionHelicopterWreck {}

/**@class		ExpansionGyrocopter
 * @brief		
 **/
class ExpansionGyrocopter: ExpansionHelicopterScript
{
	// ------------------------------------------------------------
	void ExpansionGyrocopter()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGyrocopter::Constructor - Start");
		#endif

		//! Vanilla
		m_dmgContactCoef				 			= 0.005;

		//! explosions
		m_ExplosionSize								= 2;

		m_MaxSpeed									= 100.0;

		m_AltitudeFullForce							= 1000.0;
		m_AltitudeNoForce							= 2000.0;

		m_EngineStartDuration						= 5.0;

		m_MinAutoRotateSpeed						= 2.0;
		m_MaxAutoRotateSpeed						= 10.0;

		m_BodyFrictionCoef							= 4;
		m_LiftForceCoef								= 1.2;

		m_CyclicForceCoef							= 1.5;

		m_CyclicSideSpeed							= 4.0;
		m_CyclicSideMax								= 1.0;
		m_CyclicSideCoef							= 0.4;

		m_CyclicForwardSpeed						= 10.0;
		m_CyclicForwardMax							= 1.0;
		m_CyclicForwardCoef							= 0.3;

		m_AntiTorqueSpeed							= 1.5;
		m_AntiTorqueMax								= 0.04;

		m_BankForceCoef								= 0.1;
		m_TailForceCoef								= 8.0;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGyrocopter::Constructor - End");
		#endif
	}

	// ------------------------------------------------------------
	override void LoadConstantVariables()
	{
		super.LoadConstantVariables();
	}
	
	// ------------------------------------------------------------
	override CarLightBase CreateFrontLight()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGyrocopter::CreateFrontLight");
		#endif

		return CarLightBase.Cast( ScriptedLightBase.CreateLight(ExpansionHelicopterFrontLight) );
	}
	
	// ------------------------------------------------------------
	override int GetAnimInstance()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGyrocopter::GetAnimInstance");
		#endif

		return ExpansionVehicleAnimInstances.EXPANSION_MH6;
	}

	// ------------------------------------------------------------
	override int GetSeatAnimationType( int posIdx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGyrocopter::GetSeatAnimationType");
		#endif

		return DayZPlayerConstants.VEHICLESEAT_DRIVER;
	}
	
	override int Get3rdPersonCameraType()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGyrocopter::Get3rdPersonCameraType");
		#endif

		return DayZPlayerCameras.DAYZCAMERA_3RD_VEHICLE;
	}
	
	// Batteries
	// ------------------------------------------------------------
	override bool IsVitalHelicopterBattery()
	{
		return true;
	}

	// ------------------------------------------------------------
	override bool IsVitalTruckBattery()
	{
		return false;
	}

	// ------------------------------------------------------------
	override bool IsVitalCarBattery()
	{
		return false;
	}

	// Plugs
	// ------------------------------------------------------------
	override bool IsVitalSparkPlug()
	{
		return true;
	}

	// ------------------------------------------------------------
	override bool IsVitalGlowPlug()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	override bool IsVitalIgniterPlug()
	{
		return false;
	}
	
	// Coolers
	// ------------------------------------------------------------
	override bool IsVitalRadiator()
	{
		return false;
	}

	// ------------------------------------------------------------
	override bool IsVitalEngineBelt()
	{
		return false;
	}

	// ------------------------------------------------------------	
	override bool IsVitalHydraulicHoses()
	{
		return false;
	}

	// ------------------------------------------------------------
	override float GetActionDistanceFuel()
	{
		return 4.5;
	}

	// ------------------------------------------------------------
	override float GetCameraHeight()
	{
		return 1;
	}

	// ------------------------------------------------------------
	override float GetCameraDistance()
	{
		return 6;
	}

	// ------------------------------------------------------------
	override bool CanObjectAttach( Object obj )
	{
		return false;
	}

	// ------------------------------------------------------------
	override bool LeavingSeatDoesAttachment( int posIdx )
	{
		// @CAMINOonPC#6971 Never implemented this after being told to for the past 3 months
		return false;
	}

	// ------------------------------------------------------------
	override string GetActionCompNameFuel()
	{
		return "refill";
	}

	// ------------------------------------------------------------
	override string GetDoorSelectionNameFromSeatPos(int posIdx)
	{
	}

	// ------------------------------------------------------------
	override string GetDoorInvSlotNameFromSeatPos(int posIdx)
	{
	}

	// ------------------------------------------------------------
	override int GetCarDoorsState( string slotType )
	{
		CarDoor carDoor;
		if ( !Class.CastTo( carDoor, FindAttachmentBySlotName( slotType ) ) )
		{
			return CarDoorState.DOORS_MISSING;
		}

		return CarDoorState.DOORS_MISSING;
	}
	
	override bool CanReachSeatFromSeat( int currentSeat, int nextSeat )
	{
		return false;
	}

	// ------------------------------------------------------------
	override bool CanReachDoorsFromSeat( string pDoorsSelection, int pCurrentSeat )
	{
		return false;	
	}

	// ------------------------------------------------------------
	override bool CrewCanGetThrough( int posIdx )
	{
		return true;
	}

	// ------------------------------------------------------------
	override string GetAnimSourceFromSelection( string selection )
	{
		return "";
	}

	// ------------------------------------------------------------
	// SetActions
	// ------------------------------------------------------------
	override void SetActions()
	{
		super.SetActions();
	}
}