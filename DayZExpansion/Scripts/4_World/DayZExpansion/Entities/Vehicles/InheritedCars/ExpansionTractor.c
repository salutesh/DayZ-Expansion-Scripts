/*
 * ExpansionTractor.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionTractor extends CarScript
{
	void ExpansionTractor()
	{
		m_dmgContactCoef = 0.075;
	}
	
	override int GetAnimInstance()
	{
		return ExpansionVehicleAnimInstances.EXPANSION_TRACTOR;
	}

	override int GetSeatAnimationType( int posIdx )
	{
		return DayZPlayerConstants.VEHICLESEAT_DRIVER;
	}

	/* 
	override CarLightBase CreateFrontLight()
	{
		return CarLightBase.Cast( ScriptedLightBase.CreateLight(TractorFrontLight) );
	}

	override CarRearLightBase CreateRearLight()
	{
		return CarRearLightBase.Cast( ScriptedLightBase.CreateLight(TractorRearLight) );
	}
	*/
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
		
		if ( EngineIsOn() == CarDoorState.DOORS_CLOSED )
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
				Class.CastTo( carDoor, FindAttachmentBySlotName( slotType ) );
				if ( carDoor )
				{
					if ( GetAnimationPhase("ExpansionTractorDoorsDriver") > 0.5 )
					{
						return CarDoorState.DOORS_OPEN;
					}
					else
					{
						return CarDoorState.DOORS_CLOSED;
					}
				}

				return CarDoorState.DOORS_MISSING;
			break;
			
			case "ExpansionTractorDoorsCodriver":
				Class.CastTo( carDoor, FindAttachmentBySlotName( slotType ) );
				if ( carDoor )
				{
					if ( GetAnimationPhase("ExpansionTractorDoorsCodriver") > 0.5 )
					{
						return CarDoorState.DOORS_OPEN;
					}
					else
					{
						return CarDoorState.DOORS_CLOSED;
					}
				}

				return CarDoorState.DOORS_MISSING;
			break;
		}
		
		return CarDoorState.DOORS_MISSING;
	}

	override bool CrewCanGetThrough( int posIdx )
	{
		if ( GetCarDoorsState( "expansiontractordoorsdriver" ) == CarDoorState.DOORS_OPEN || GetCarDoorsState( "expansiontractordoorsdriver" ) == CarDoorState.DOORS_MISSING )
		{
			return true;
		}

		if ( GetCarDoorsState( "expansiontractordoorscodriver" ) == CarDoorState.DOORS_OPEN || GetCarDoorsState( "expansiontractordoorscodriver" ) == CarDoorState.DOORS_MISSING )
		{
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	override string GetDoorSelectionNameFromSeatPos(int posIdx)
	{
		switch( posIdx )
		{
		case 0:
			return "expansiontractordoorsdriver";
		break;
		case 1:
			return "expansiontractordoorscodriver";
		break;
		}
		
		return super.GetDoorSelectionNameFromSeatPos(posIdx);
	}

	// ------------------------------------------------------------
	override string GetDoorInvSlotNameFromSeatPos(int posIdx)
	{
		switch( posIdx )
		{
		case 0:
			return "expansiontractordoorsdriver";
		break;
		case 1:
			return "expansiontractordoorscodriver";
		break;
		}
		
		return super.GetDoorInvSlotNameFromSeatPos(posIdx);
	}

	override float OnSound( CarSoundCtrl ctrl, float oldValue )
	{
		switch ( ctrl )
		{
			case CarSoundCtrl.DOORS:
				float newValue = 0;

				//-----
				if ( GetCarDoorsState( "expansiontractordoorsdriver" ) == CarDoorState.DOORS_CLOSED )
				{
					newValue += 0.25;
				}

				if ( GetCarDoorsState( "expansiontractordoorscodriver" ) == CarDoorState.DOORS_CLOSED )
				{
					newValue += 0.25;
				}

				if ( newValue > 1 )
					newValue = 1;

			return newValue;
			break;
		}

		return oldValue;
	}
	
	override string GetAnimSourceFromSelection( string selection )
	{
		switch( selection )
		{
		case "expansiontractordoorsdriver":
			return "expansiontractordoorsdriver";
		case "expansiontractordoorscodriver":
			return "expansiontractordoorscodriver";
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

	override bool CanReachSeatFromSeat( int currentSeat, int nextSeat )
	{
		switch( currentSeat )
		{
		case 0:
			if ( nextSeat == 1 )
				return true;
		break;		
		case 1:
			if ( nextSeat == 0 )
				return true;
		break;
		}

		return false;
	}

	override string GetDoorConditionPointFromSelection( string selection )
	{
		switch( selection )
		{
		case "seat_driver":
			return "seat_con_1_1";
		break;
		case "seatback_driver":
			return "seat_con_2_1";
		break;
		}
		
		return "";
	}

	override bool CanReachDoorsFromSeat( string pDoorsSelection, int pCurrentSeat )
	{
		switch( pCurrentSeat )
		{
		case 0:
			if (pDoorsSelection == "expansiontractordoorsdriver")
			{
				return true;
			}
		break;
		case 1:
			if (pDoorsSelection == "expansiontractordoorscodriver")
			{
				return true;
			}
		break;
		}
		
		return false;		
	}

	// ------------------------------------------------------------
	override float GetCameraDistance()
	{
		return 10;
	}
}