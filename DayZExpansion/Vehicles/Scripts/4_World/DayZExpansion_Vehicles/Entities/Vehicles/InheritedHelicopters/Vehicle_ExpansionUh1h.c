/**
 * Vehicle_ExpansionUh1h.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		Vehicle_ExpansionUh1h
 * @brief		
 **/
class Vehicle_ExpansionUh1h: ExpansionVehicleHelicopterBase
{
	// ------------------------------------------------------------
	void Vehicle_ExpansionUh1h()
	{
		m_dmgContactCoef							= 0.6;
		m_ExplosionSize								= 20;
	}
	
	// ------------------------------------------------------------
	override CarLightBase CreateFrontLight()
	{
		return CarLightBase.Cast( ScriptedLightBase.CreateLight(ExpansionHelicopterFrontLight) );
	}
	
	// ------------------------------------------------------------
	override int GetAnimInstance()
	{
		return ExpansionVehicleAnimInstances.EXPANSION_UH1H;
	}

	// ------------------------------------------------------------
	override int GetSeatAnimationType( int posIdx )
	{
		switch( posIdx )
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
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_R;
		case 5:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
		}

		return 0;
	}
	
	// ------------------------------------------------------------
	override int Get3rdPersonCameraType()
	{
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

	//! Plugs
	// ------------------------------------------------------------
	override bool IsVitalSparkPlug()
	{
		return false;
	}

	// ------------------------------------------------------------
	override bool IsVitalGlowPlug()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	override bool IsVitalIgniterPlug()
	{
		return true;
	}
	
	//! Coolers
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
		return true;
	}

	// ------------------------------------------------------------
	override float GetActionDistanceFuel()
	{
		return 2.5;
	}

	// ------------------------------------------------------------
	override float GetWreckAltitude()
	{
		return 1.5;
	}

	// ------------------------------------------------------------
	override float GetCameraHeight()
	{
		return 0;
	}

	// ------------------------------------------------------------
	override float GetCameraDistance()
	{
		return 15;
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
	override float OnSound( CarSoundCtrl ctrl, float oldValue )
	{
		switch ( ctrl )
		{
			case CarSoundCtrl.DOORS:
				float newValue = 0;
				
				if ( GetCarDoorsState( "uh1hdoor_1_1" ) == CarDoorState.DOORS_CLOSED )
					newValue += 0.5;

				if ( GetCarDoorsState( "uh1hdoor_1_2" ) == CarDoorState.DOORS_CLOSED )
					newValue += 0.5;

				if ( newValue > 1 )
					newValue = 1;
			
				return newValue;
			break;
		}

		return super.OnSound( ctrl, oldValue );
	}

	// ------------------------------------------------------------
	override string GetDoorSelectionNameFromSeatPos(int posIdx)
	{
		switch( posIdx )
		{
		case 0:
			return "uh1hdoor_1_1";
		break;
		case 1:
			return "uh1hdoor_1_2";
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
			return "uh1hdoor_1_1";
		break;
		case 1:
			return "uh1hdoor_1_2";
		break;
		}
		
		return super.GetDoorInvSlotNameFromSeatPos(posIdx);
	}

	// ------------------------------------------------------------
	override int GetCarDoorsState( string slotType )
	{
		CarDoor carDoor;
		Class.CastTo( carDoor, FindAttachmentBySlotName( slotType ) );
		if ( !carDoor ) {
			return CarDoorState.DOORS_MISSING;
		}

		switch( slotType )
		{
			case "uh1hdoor_1_1": {
				if ( GetAnimationPhase("uh1hdoor_1_1") > 0.5 ) {
					return CarDoorState.DOORS_OPEN;
				} else {
					return CarDoorState.DOORS_CLOSED;
				}
				break;
			}
			case "uh1hdoor_1_2": {
				if ( GetAnimationPhase("uh1hdoor_1_2") > 0.5 ) {
					return CarDoorState.DOORS_OPEN;
				} else {
					return CarDoorState.DOORS_CLOSED;
				}
				break;
			}
			default: {
				return CarDoorState.DOORS_MISSING;
			}
		}

		return CarDoorState.DOORS_MISSING;
	}

	// ------------------------------------------------------------
	override bool CanReachDoorsFromSeat( string pDoorsSelection, int pCurrentSeat )
	{
		switch( pCurrentSeat )
		{
			case 0: {
				if (pDoorsSelection == "uh1hdoor_1_1")
				{
					return true;
				}
				break;
			}
			case 1: {
				if (pDoorsSelection == "uh1hdoor_1_2")
				{
					return true;
				}
				break;
			}
		}	
		return false;	
	}

	// ------------------------------------------------------------
	override bool CrewCanGetThrough( int posIdx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionUH1H::CrewCanGetThrough");
		#endif

		switch( posIdx )
		{
			case 0:
				if ( GetCarDoorsState( "uh1hdoor_1_1" ) == CarDoorState.DOORS_CLOSED )
					return false;

				return true;
			break;

			case 1:
				//Uncomment this once the door has been fixed
				/*if ( GetCarDoorsState( "uh1hdoor_1_2" ) == CarDoorState.DOORS_CLOSED )
					return false;*/

				return true;
			break;
		}
		return true;
	}

	// ------------------------------------------------------------
	override string GetAnimSourceFromSelection( string selection )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionUH1H::GetAnimSourceFromSelection");
		#endif
		
		switch( selection )
		{
			case "uh1hdoor_1_1":
				return "uh1hdoor_1_1";
			case "uh1hdoor_1_2":
				return "uh1hdoor_1_2";
		}

		return "";
	}

	// --------------------------------------------------------- ---
	override void UpdateLights(int new_gear = -1) 
	{
		super.UpdateLights( new_gear );

		if ( IsMissionClient() ) 
		{
			ItemBase battery;
			
			if ( IsVitalHelicopterBattery() ) battery = ItemBase.Cast( FindAttachmentBySlotName("ExpansionHelicopterBattery") );

			if ( battery )
			{
				if ( m_HeadlightsOn )
				{
					if ( m_Lights.Count() == 0 )
					{
						CreateLights( this, "interior", ExpansionPointLight, Vector(0.1, 1, 0.25), Vector(0.1, 1, 0.25), 2.3, 0.1, false, true );
					}
				}
				else
				{
					for ( int b = 0; b < m_Lights.Count(); b++ )
					{
						m_Lights[b].ExpansionSetEnabled( false );

						GetGame().ObjectDelete( m_Lights[b] );
					}

					m_Lights.Clear();
				}
			}
		}
	}
};