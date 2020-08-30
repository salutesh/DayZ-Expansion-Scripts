/**
 * ExpansionMh6.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_Mh6_Door_1_1 extends CarDoor {}
class Expansion_Mh6_Door_1_2 extends CarDoor {}
class Expansion_Mh6_Door_2_1 extends CarDoor {}
class Expansion_Mh6_Door_2_2 extends CarDoor {}
class ExpansionMh6Wreck: ExpansionHelicopterWreck {}

/**@class		ExpansionMh6
 * @brief		
 **/
class ExpansionMh6: ExpansionHelicopterScript
{
	// ------------------------------------------------------------
	void ExpansionMh6()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMh6::Constructor - Start");
		#endif

		//! Vanilla
		m_dmgContactCoef				 			= 0.005;

		//! explosions
		m_ExplosionSize					 			= 10;

		m_MaxSpeed									= 220.0;

		m_AltitudeFullForce							= 1000.0;
		m_AltitudeNoForce							= 2000.0;

		m_EngineStartDuration						= 15.0;

		m_MinAutoRotateSpeed						= 2.0;
		m_MaxAutoRotateSpeed						= 10.0;

		m_BodyFrictionCoef							= 1.1;
		m_LiftForceCoef								= 1.0;

		m_CyclicForceCoef							= 1.5; // All rotations

		m_CyclicSideSpeed							= 4.0;
		m_CyclicSideMax								= 0.6; // Roll, A/D, Speed
		m_CyclicSideCoef							= 0.6; // Roll, A/D, Speed

		m_CyclicForwardSpeed						= 10.0;
		m_CyclicForwardMax							= 0.7;  // Pitch, W/S, Speed
		m_CyclicForwardCoef							= 1.0; // Pitch, W/S, Speed

		m_AntiTorqueSpeed							= 1.5;
		m_AntiTorqueMax								= 0.16; // Yaw, Q/E, same as m_TailForceCoef

		m_BankForceCoef								= 0.05; // Speed at which heli will rotate when rotor is banking left or right 
		m_TailForceCoef								= 2.0; // Yaw, Q/E, same as m_AntiTorqueMax

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMh6::Constructor - End");
		#endif
	}

	// ------------------------------------------------------------
	override CarLightBase CreateFrontLight()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMh6::CreateFrontLight");
		#endif
		
		return CarLightBase.Cast( ScriptedLightBase.CreateLight(ExpansionHelicopterFrontLight) );
	}
	
	// ------------------------------------------------------------
	override int GetAnimInstance()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMh6::GetAnimInstance");
		#endif

		return ExpansionVehicleAnimInstances.EXPANSION_MH6;
	}

	// ------------------------------------------------------------
	override int GetSeatAnimationType( int posIdx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMh6::GetSeatAnimationType");
		#endif

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
		}

		return 0;
	}
	
	override int Get3rdPersonCameraType()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMh6::Get3rdPersonCameraType");
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
		return 1;
	}

	// ------------------------------------------------------------
	override float GetCameraDistance()
	{
		return 10;
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
	override bool CanReleaseAttachment( EntityAI attachment )
	{
		string attType = attachment.GetType();
		
		if ( EngineIsOn() )
		{
			if ( attType == "ExpansionHelicopterBattery" || attType == "CarRadiator" || attType == "ExpansionIgniterPlug" || attType == "ExpansionHydraulicHoses")
				return false;
		}

		return super.CanReleaseAttachment( attachment );
	}
	
	// ------------------------------------------------------------
	override float OnSound( CarSoundCtrl ctrl, float oldValue )
	{
		switch ( ctrl )
		{
			case CarSoundCtrl.DOORS:
				float newValue = 0;
				
				if ( GetCarDoorsState( "mh6door_1_1" ) == CarDoorState.DOORS_CLOSED )
					newValue += 0.25;

				if ( GetCarDoorsState( "mh6door_1_2" ) == CarDoorState.DOORS_CLOSED )
					newValue += 0.25;
			
				if ( GetCarDoorsState( "mh6door_2_1" ) == CarDoorState.DOORS_CLOSED )
					newValue += 0.25;

				if ( GetCarDoorsState( "mh6door_2_2" ) == CarDoorState.DOORS_CLOSED )
					newValue += 0.25;

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

	// ------------------------------------------------------------
	override string GetDoorInvSlotNameFromSeatPos(int posIdx)
	{
		switch( posIdx )
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
	
	// ------------------------------------------------------------
	override int GetCarDoorsState( string slotType )
	{
		CarDoor carDoor;
		if ( !Class.CastTo( carDoor, FindAttachmentBySlotName( slotType ) ) )
		{
			return CarDoorState.DOORS_MISSING;
		}

		switch( slotType )
		{
			case "mh6door_1_1": {
				if ( GetAnimationPhase("mh6door_1_1") > 0.5 ) {
					return CarDoorState.DOORS_OPEN;
				} else {
					return CarDoorState.DOORS_CLOSED;
				}
				break;
			}
			case "mh6door_1_2": {
				if ( GetAnimationPhase("mh6door_1_2") > 0.5 ) {
					return CarDoorState.DOORS_OPEN;
				} else {
					return CarDoorState.DOORS_CLOSED;
				}
				break;
			}
			case "mh6door_2_1": {
				if ( GetAnimationPhase("mh6door_2_1") > 0.5 ) {
					return CarDoorState.DOORS_OPEN;
				} else {
					return CarDoorState.DOORS_CLOSED;
				}
				break;
			}
			case "mh6door_2_2": {
				if ( GetAnimationPhase("mh6door_2_2") > 0.5 ) {
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
		case 2:
			if ( nextSeat == 3 )
				return true;
			break;
		case 3:
			if ( nextSeat == 2 )
				return true;
			break;
		}
		
		return false;
	}

	// ------------------------------------------------------------
	override bool CanReachDoorsFromSeat( string pDoorsSelection, int pCurrentSeat )
	{
		switch( pCurrentSeat )
		{
			case 0: {
				if (pDoorsSelection == "mh6door_1_1")
				{
					return true;
				}
				break;
			}
			case 1: {
				if (pDoorsSelection == "mh6door_1_2")
				{
					return true;
				}
				break;
			}
			case 2: {
				if (pDoorsSelection == "mh6door_2_1")
				{
					return true;
				}
				break;
			}
			case 3: {
				if (pDoorsSelection == "mh6door_2_2")
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
		switch( posIdx )
		{
			case 0:
				if ( GetAnimationPhase( "mh6door_1_1" ) > 0.5 )
					return true;		
				
				if ( FindAttachmentBySlotName("mh6door_1_1") == NULL ) 
					return true;
			break;

			case 1:
				if ( GetAnimationPhase( "mh6door_1_2" ) > 0.5 )
					return true;
				
				if ( FindAttachmentBySlotName("mh6door_1_2") == NULL ) 
					return true;
			break;

			case 2:
				if ( GetAnimationPhase( "mh6door_2_1" ) > 0.5 )
					return true;
				
				if ( FindAttachmentBySlotName("mh6door_2_1") == NULL ) 
					return true;
			break;

			case 3:
				if ( GetAnimationPhase( "mh6door_2_2" ) > 0.5 )
					return true;
				
				if ( FindAttachmentBySlotName("mh6door_2_2") == NULL ) 
					return true;								
			break;
		}

		return false;
	}

	// ------------------------------------------------------------
	override string GetAnimSourceFromSelection( string selection )
	{
		switch( selection )
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

	// --------------------------------------------------------- ---
	override void UpdateLights(int new_gear = -1) 
	{
		if ( IsMissionClient() ) 
		{
			ItemBase battery;
			
			if ( IsVitalHelicopterBattery() )
				battery = ItemBase.Cast( FindAttachmentBySlotName("ExpansionHelicopterBattery") );
			
			if ( battery )
			{
				if ( m_HeadlightsOn )
				{
					if ( !m_Headlight && m_HeadlightsState != CarHeadlightBulbsState.NONE )
					{
						m_Headlight = CreateFrontLight();

						m_Headlight.AttachOnMemoryPoint(this, CarScript.m_LeftHeadlightPoint, CarScript.m_LeftHeadlightTargetPoint);
						m_Headlight.SegregateLight();
						TailLightsShineOn();
					}

					if ( m_HeadlightsState != CarHeadlightBulbsState.NONE )
					{
						LeftFrontLightShineOn();
						RightFrontLightShineOn();
					}

					if ( m_Lights.Count() == 0 )
					{
						CreateLights( this, "interior", ExpansionPointLight, Vector(1, 0.01, 0.01), Vector(1, 0.01, 0.01), 3, 0.35, false, true );
					}
				}
				else
				{
					LeftFrontLightShineOff();
					RightFrontLightShineOff();

					if ( m_Headlight )
					{
						m_Headlight.FadeOut();
						m_Headlight = null;
					}

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
}
