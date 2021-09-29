/*
 * Vehicle_ExpansionTractor.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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

	// ------------------------------------------------------------
	override vector GetTransportCameraOffset()
	{
		return "0 2.5 0";
	}

	// ------------------------------------------------------------
	override float GetTransportCameraDistance()
	{
		return 3.5;
	}
	
	override int GetAnimInstance()
	{
		return ExpansionVehicleAnimInstances.EXPANSION_TRACTOR;
	}

	override int GetSeatAnimationType( int posIdx )
	{
		return DayZPlayerConstants.VEHICLESEAT_DRIVER;
	}

	override CarLightBase CreateFrontLight()
	{
		//TractorFrontLight
		return CarLightBase.Cast( ScriptedLightBase.CreateLight(ExpansionRearCarLights) );
	}

	override CarRearLightBase CreateRearLight()
	{
		//TractorRearLight
		return CarRearLightBase.Cast( ScriptedLightBase.CreateLight(ExpansionCarFrontLight) );
	}

	override string ExpansionGetWheelType(int slot_id)
	{
		if (InventorySlots.GetSlotName(slot_id) == "ExpansionTractorFrontWheel_1_1" || InventorySlots.GetSlotName(slot_id) == "ExpansionTractorFrontWheel_2_1"  )
			return "ExpansionTractorFrontWheel";
		else
			return "ExpansionTractorBackWheel";
	}

	override bool CanReleaseAttachment( EntityAI attachment )
	{
		if( !super.CanReleaseAttachment( attachment ) )
			return false;
		
		string attType = attachment.GetType();
		
		if ( EngineIsOn() )
		{
			if ( attType == "CarBattery")
				return false;
		}

		return true;
	}

	override int GetCarDoorsState( string slotType )
	{
		CarDoor carDoor;
		Class.CastTo( carDoor, FindAttachmentBySlotName( slotType ) );
		if ( !carDoor )
			return CarDoorState.DOORS_MISSING;

		switch( slotType )
		{
			case "ExpansionTractorDoorsDriver":
				if ( GetAnimationPhase("ExpansionTractorDoorsDriver") > 0.5 )
					return CarDoorState.DOORS_OPEN;
				else
					return CarDoorState.DOORS_CLOSED;
			break;
			
			case "ExpansionTractorDoorsCodriver":
				if ( GetAnimationPhase("ExpansionTractorDoorsCodriver") > 0.5 )
					return CarDoorState.DOORS_OPEN;
				else
					return CarDoorState.DOORS_CLOSED;
		}
		
		return CarDoorState.DOORS_MISSING;
	}

	override bool CrewCanGetThrough( int posIdx )
	{
		CarDoor carDoor;
		switch( posIdx )
		{
			case 0:
				if ( GetCarDoorsState( "ExpansionTractorDoorsDriver" ) == CarDoorState.DOORS_CLOSED )
					return false;

				return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	override string GetDoorSelectionNameFromSeatPos(int posIdx)
	{
		if ( GetAnimationPhase("ExpansionTractorDoorsCodriver") > 0.5 )
			return "expansiontractordoorscodriver";

		return "expansiontractordoorsdriver";
		
		//return super.GetDoorSelectionNameFromSeatPos(posIdx);
	}

	// ------------------------------------------------------------
	override string GetDoorInvSlotNameFromSeatPos(int posIdx)
	{
		return "ExpansionTractorDoorsDriver";
		
		return super.GetDoorInvSlotNameFromSeatPos(posIdx);
	}

	override float OnSound( CarSoundCtrl ctrl, float oldValue )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Vehicle_ExpansionTractor::OnSound - Start");
		#endif

		switch ( ctrl )
		{
			case CarSoundCtrl.DOORS:
				float newValue = 0;

				//-----
				if ( GetCarDoorsState( "ExpansionTractorDoorsDriver" ) == CarDoorState.DOORS_CLOSED )
				{
					newValue += 0.4;
				}

				if ( GetCarDoorsState( "ExpansionTractorDoorsCodriver" ) == CarDoorState.DOORS_CLOSED )
				{
					newValue += 0.4;
				}

				if ( newValue > 1 )
					newValue = 1;

				#ifdef EXPANSIONEXPRINT
				EXPrint("Vehicle_ExpansionTractor::OnSound - End");
				#endif

				return newValue;
			default:
				break;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionUAZ::OnSound - End");
		#endif

		return oldValue;
	}
	
	override string GetAnimSourceFromSelection( string selection )
	{
		switch( selection )
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
		return true;
	}

	override bool IsVitalTruckBattery()
	{
		return false;
	}

	override bool IsVitalSparkPlug()
	{
		return false;
	}

	override bool IsVitalGlowPlug()
	{
		return false;
	}

	override bool IsVitalEngineBelt()
	{
		return false;
	}
	
	override bool IsVitalRadiator()
	{
		return false;
	}

	override bool LeavingSeatDoesAttachment( int posIdx )
	{
		// @CAMINOonPC#6971 Never implemented this after being told to for the past 3 months
		return false;
	}

	override bool CanReachDoorsFromSeat( string pDoorsSelection, int pCurrentSeat )
	{
		if (pDoorsSelection == "ExpansionTractorDoorsDriver")
			return true;
		if (pDoorsSelection == "ExpansionTractorDoorsCodriver")
			return true;

		return false;
	}
};