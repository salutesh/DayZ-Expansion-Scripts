/**
 * Vehicle_ExpansionGyrocopter.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		Vehicle_ExpansionGyrocopter
 * @brief		
 **/
class Vehicle_ExpansionGyrocopter: ExpansionHelicopterScript
{
	// ------------------------------------------------------------
	void Vehicle_ExpansionGyrocopter()
	{
		m_dmgContactCoef				 			= 0.005;
		m_ExplosionSize								= 2;
	}

	// ------------------------------------------------------------
	override void LoadConstantVariables()
	{
		super.LoadConstantVariables();
	}
	
	// ------------------------------------------------------------
	override CarLightBase CreateFrontLight()
	{
		return CarLightBase.Cast( ScriptedLightBase.CreateLight(ExpansionHelicopterFrontLight) );
	}
	
	// ------------------------------------------------------------
	override int GetAnimInstance()
	{
		return ExpansionVehicleAnimInstances.EXPANSION_GYROCOPTER;
	}

	// ------------------------------------------------------------
	override int GetSeatAnimationType( int posIdx )
	{
		return DayZPlayerConstants.VEHICLESEAT_DRIVER;
	}
	
	override int Get3rdPersonCameraType()
	{
		return DayZPlayerCameras.DAYZCAMERA_3RD_VEHICLE;
	}
	
	//! Batteries
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

	//! Plugs
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
	
	// ------------------------------------------------------------
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
};