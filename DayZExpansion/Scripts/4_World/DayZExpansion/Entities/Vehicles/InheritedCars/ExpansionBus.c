/*
 * ExpansionBus.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBus extends OffroadHatchback
{
	private CarLightBase m_BusLight1;
	private CarLightBase m_BusLight2;
	private CarLightBase m_BusLight3;

	// ------------------------------------------------------------
	void ExpansionBus()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBus::Constructor - Start");
		#endif

		m_dmgContactCoef = 0.018;

		// SetEventMask(EntityEvent.SIMULATE);

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBus::Constructor - End");
		#endif
	}

	// ------------------------------------------------------------
	void ~ExpansionBus()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBus::Deconstructor - Start");
		#endif

		if ( IsMissionClient() )
		{
			if ( m_BusLight1 )
				m_BusLight1.Destroy();

			if ( m_BusLight2 )
				m_BusLight2.Destroy();

			if ( m_BusLight2 )
				m_BusLight2.Destroy();
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBus::Deconstructor - End");
		#endif
	}

	// ------------------------------------------------------------
	void PartyBus()
	{
		Megaphone megaphone = Megaphone.Cast(GetInventory().CreateInInventory( "Megaphone" ));
		megaphone.GetInventory().CreateAttachment("Battery9V");

		megaphone = Megaphone.Cast(GetInventory().CreateInInventory( "Megaphone" ));
		megaphone.GetInventory().CreateAttachment("Battery9V");

		megaphone = Megaphone.Cast(GetInventory().CreateInInventory( "Megaphone" ));
		megaphone.GetInventory().CreateAttachment("Battery9V");
	}

	// ------------------------------------------------------------
	override int GetAnimInstance()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBus::GetAnimInstance");
		#endif

		return ExpansionVehicleAnimInstances.EXPANSION_BUS;
	}

	// ------------------------------------------------------------
	override CarRearLightBase CreateRearLight()
	{
		return CarRearLightBase.Cast( ScriptedLightBase.CreateLight(ExpansionRearCarLights) );
	}
	
	// ------------------------------------------------------------
	override CarLightBase CreateFrontLight()
	{
		return CarLightBase.Cast( ScriptedLightBase.CreateLight(ExpansionCarFrontLight) );
	}

	// ------------------------------------------------------------
	CarLightBase CreateBusLight()
	{
		return CarLightBase.Cast( ScriptedLightBase.CreateLight( ExpansionBusLights ) ); 
	}

	// ------------------------------------------------------------
	override void UpdateLights(int new_gear = -1) // -1 is invalid gear.
	{
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() ) // client side
		{
			ItemBase battery;
			
			if ( IsVitalTruckBattery() ) 
				battery = ItemBase.Cast( FindAttachmentBySlotName("TruckBattery") );
			
			if (battery)
			{
				if (m_HeadlightsOn)
				{
					if (!m_BusLight1 && m_HeadlightsState != CarHeadlightBulbsState.NONE)
					{
						m_BusLight1 = CreateBusLight();
					}

					if (!m_BusLight2 && m_HeadlightsState != CarHeadlightBulbsState.NONE)
					{
						m_BusLight2 = CreateBusLight();
					}

					if (!m_BusLight3 && m_HeadlightsState != CarHeadlightBulbsState.NONE)
					{
						m_BusLight3 = CreateBusLight();
					}

					if (m_HeadlightsState == CarHeadlightBulbsState.BOTH)
					{
						if ( IsInherited( ExpansionBus ) ) 
						{
							m_BusLight1.AttachOnObject(this, GetMemoryPointPos( "intlight1" ));
							m_BusLight1.AggregateLight();

							m_BusLight2.AttachOnObject(this, GetMemoryPointPos( "intlight2" ));
							m_BusLight2.AggregateLight();

							m_BusLight3.AttachOnObject(this, GetMemoryPointPos( "intlight3" ));
							m_BusLight3.AggregateLight();
						}
					}

					DashboardShineOn();
					
					if (!m_Headlight  &&  m_HeadlightsState != CarHeadlightBulbsState.NONE)
					{
						m_Headlight = CreateFrontLight();
						TailLightsShineOn();
					}
					
					if (m_HeadlightsState == CarHeadlightBulbsState.LEFT)
					{
						m_Headlight.AttachOnMemoryPoint(this, m_LeftHeadlightPoint, m_LeftHeadlightTargetPoint);
						m_Headlight.SegregateLight();
						LeftFrontLightShineOn();
						RightFrontLightShineOff();
					}
					else if (m_HeadlightsState == CarHeadlightBulbsState.RIGHT)
					{
						m_Headlight.AttachOnMemoryPoint(this, m_RightHeadlightPoint, m_RightHeadlightTargetPoint);
						m_Headlight.SegregateLight();
						RightFrontLightShineOn();
						LeftFrontLightShineOff();
					}
					else if (m_HeadlightsState == CarHeadlightBulbsState.BOTH)
					{
						vector local_pos_left = GetMemoryPointPos(m_LeftHeadlightPoint);
						vector local_pos_right = GetMemoryPointPos(m_RightHeadlightPoint);
						
						vector local_pos_middle = (local_pos_left + local_pos_right) *0.5;
						m_Headlight.AttachOnObject(this, local_pos_middle);
						m_Headlight.AggregateLight();
						LeftFrontLightShineOn();
						RightFrontLightShineOn();
					}
					
					if (m_Headlight  &&  m_HeadlightsState == CarHeadlightBulbsState.NONE)
					{
						m_Headlight.FadeOut();
						m_Headlight = null;
						LeftFrontLightShineOff();
						RightFrontLightShineOff();
						TailLightsShineOff();
					}
				}
				else
				{
					TailLightsShineOff();
					DashboardShineOff();
					LeftFrontLightShineOff();
					RightFrontLightShineOff();
					
					if (m_Headlight)
					{
						m_Headlight.FadeOut();
						m_Headlight = null;
					}

					if (m_BusLight1)
					{
						m_BusLight1.FadeOut();
						m_BusLight1 = null;
					}

					if (m_BusLight2)
					{
						m_BusLight2.FadeOut();
						m_BusLight2 = null;
					}

					if (m_BusLight3)
					{
						m_BusLight3.FadeOut();
						m_BusLight3 = null;
					}
				}
			
				if ( EngineIsOn() )
				{
					int reverse_light_state = CarRearLightType.NONE;
								
					int gear;
					
					if (new_gear == -1)
						gear = GetController().GetGear();
					else
						gear = new_gear;
					
					
					if (gear == CarGear.REVERSE)
					{
						reverse_light_state = CarRearLightType.REVERSE_ONLY;
					}
					else
					{
						reverse_light_state = CarRearLightType.NONE;
					}
					
					if (m_BrakesArePressed)
					{
						if (reverse_light_state == CarRearLightType.REVERSE_ONLY)
						{
							reverse_light_state = CarRearLightType.BRAKES_AND_REVERSE;
						}
						else
						{
							reverse_light_state = CarRearLightType.BRAKES_ONLY;
						}
					}
					
					if (reverse_light_state != CarRearLightType.NONE    &&  m_HeadlightsState != CarHeadlightBulbsState.NONE  &&  !m_RearLight)
					{
						m_RearLight = CreateRearLight();
						vector local_pos = GetMemoryPointPos(m_ReverseLightPoint);
						m_RearLight.AttachOnObject(this, local_pos, "180 0 0");
					}
					
					if (m_RearLight)
					{
						if (reverse_light_state == CarRearLightType.REVERSE_ONLY)
						{
							m_RearLight.SetAsSegregatedReverseLight();

							if (m_HeadlightsState != CarHeadlightBulbsState.NONE)
							{
								ReverseLightsShineOn();
								BrakeLightsShineOff();
							}
						}
						else if (reverse_light_state == CarRearLightType.BRAKES_ONLY)
						{
							m_RearLight.SetAsSegregatedBrakeLight();
							
							if (m_HeadlightsState != CarHeadlightBulbsState.NONE)
							{
								ReverseLightsShineOff();
								BrakeLightsShineOn();
							}
						}
						else if (reverse_light_state == CarRearLightType.BRAKES_AND_REVERSE)
						{
							m_RearLight.AggregateLight();
							m_RearLight.SetFadeOutTime(1);
							
							if (m_HeadlightsState != CarHeadlightBulbsState.NONE)
							{
								BrakeLightsShineOn();
								ReverseLightsShineOn();
							}
						}
						else if (reverse_light_state == CarRearLightType.NONE)
						{
							m_RearLight.FadeOut();
							m_RearLight = null;
							ReverseLightsShineOff();
						}
					}
					else
					{
						ReverseLightsShineOff();
						BrakeLightsShineOff();
					}
				}
				else
				{
					BrakeLightsShineOff();
					ReverseLightsShineOff();
					
					if (m_RearLight)
					{
						m_RearLight.FadeOut();
						m_RearLight = null;
					}
				}
			}
			else
			{
				LeftFrontLightShineOff();
				RightFrontLightShineOff();
				DashboardShineOff();
				BrakeLightsShineOff();
				ReverseLightsShineOff();
				
				if (m_RearLight)
				{
					m_RearLight.FadeOut();
					m_RearLight = null;
				}
				
				if (m_Headlight)
				{
					m_Headlight.FadeOut();
					m_Headlight = null;
				}

				if (m_BusLight1)
				{
					m_BusLight1.FadeOut();
					m_BusLight1 = null;
				}

				if (m_BusLight2)
				{
					m_BusLight2.FadeOut();
					m_BusLight2 = null;
				}

				if (m_BusLight3)
				{
					m_BusLight3.FadeOut();
					m_BusLight3 = null;
				}
			}
		}
	}

	// ------------------------------------------------------------
	override string GetDoorConditionPointFromSelection( string selection )
	{
		return selection;

		/*
		switch( selection )
		{
			case "seat_driver":
				return "seat_con_1";
			case "seat_codriver":
				return "seat_con_2";
			case "seat_cargo1":
				return "seat_con_3";
			case "seat_cargo2":
				return "seat_con_4";
			case "seat_cargo3":
				return "seat_con_5";
			case "seat_cargo4":
				return "seat_con_6";
			case "seat_cargo5":
				return "seat_con_7";
			case "seat_cargo6":
				return "seat_con_8";
			case "seat_cargo7":
				return "seat_con_9";
			case "seat_cargo8":
				return "seat_con_10";
			case "seat_cargo9":
				return "seat_con_11";
			case "seat_cargo10":
				return "seat_con_12";
			case "seat_cargo11":
				return "seat_con_13";
			case "seat_cargo12":
				return "seat_con_14";
			case "seat_cargo13":
				return "seat_con_15";
			case "seat_cargo14":
				return "seat_con_16";
			case "seat_cargo15":
				return "seat_con_17";
			case "seat_cargo16":
				return "seat_con_18";
			case "seat_cargo17":
				return "seat_con_19";
			case "seat_cargo18":
				return "seat_con_20";
			case "seat_cargo19":
				return "seat_con_21";
			case "seat_cargo20":
				return "seat_con_22";
		}
		return "";
		*/
	}

	// ------------------------------------------------------------
	override int GetSeatAnimationType( int posIdx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBus::GetSeatAnimationType");
		#endif

		switch( posIdx )
		{
		case 0:
			return DayZPlayerConstants.VEHICLESEAT_DRIVER;
		case 1:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_R;
		case 2:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
		case 3:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_R;
		case 4:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_R;
		case 5:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_R;
		case 6:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_R;
		case 7:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_R;
		case 8:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_R;
		case 9:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
		case 10:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_R;
		case 11:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
		case 12:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
		case 13:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
		case 14:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
		case 15:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
		case 16:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_R;
		case 17:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
		case 18:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
		case 19:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
		case 20:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
		case 21:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
		}

		return 0;
	}
	
	// ------------------------------------------------------------
	override int GetCarDoorsState( string slotType )
	{
		/*
		CarDoor carDoor;
		Class.CastTo( carDoor, FindAttachmentBySlotName( slotType ) );
		if ( !carDoor ) {
			return CarDoorState.DOORS_MISSING;
		}

		*/

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBus::GetCarDoorsState");
		#endif

		return CarDoorState.DOORS_MISSING;
	}
	
	/* 
	// ------------------------------------------------------------
	override float OnSound( CarSoundCtrl ctrl, float oldValue )
	{
		switch ( ctrl )
		{
			case CarSoundCtrl.DOORS:
				float newValue = 0;

				if ( GetCarDoorsState( "uazdriverdoor" ) == CarDoorState.DOORS_CLOSED )
				{
					newValue += 0.4;
				}

				if ( GetCarDoorsState( "uazcodriverdoor" ) == CarDoorState.DOORS_CLOSED )
				{
					newValue += 0.4;
				}

				if ( GetCarDoorsState( "uazcargo1door" ) == CarDoorState.DOORS_CLOSED )
				{
					newValue += 0.4;
				}

				if ( GetCarDoorsState( "uazcargo2door" ) == CarDoorState.DOORS_CLOSED )
				{
					newValue += 0.4;
				}

				if ( newValue > 1 )
					newValue = 1;

				return newValue;
			default:
				break;
		}

		return oldValue;
	}
	*/

	// ------------------------------------------------------------
	override bool CanReachDoorsFromSeat( string pDoorsSelection, int pCurrentSeat )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBus::CanReachDoorsFromSeat");
		#endif

		return true;		
	}
	
	// ------------------------------------------------------------
	override bool CanReachSeatFromSeat( int currentSeat, int nextSeat )
	{
		return false;
/*
		switch( currentSeat )
		{
		case 11:
			if ( nextSeat == 17 )
				return true;
			if ( nextSeat == 10 )
				return true;
			return false;
		case 12:
			if ( nextSeat == 18 )
				return true;
			if ( nextSeat == 9 )
				return true;
			return false;
		case 13:
			if ( nextSeat == 19 )
				return true;
			if ( nextSeat == 8 )
				return true;
			return false;
		case 14:
			if ( nextSeat == 20 )
				return true;
			if ( nextSeat == 3 )
				return true;
			if ( nextSeat == 2 )
				return true;
			return false;
		case 15:
			if ( nextSeat == 21 )
				return true;
			if ( nextSeat == 2 )
				return true;
			if ( nextSeat == 1 )
				return true;
			return false;
		case 16:
			if ( nextSeat == 22 )
				return true;
			if ( nextSeat == 1 )
				return true;
			return false;
			
		case 17:
			if ( nextSeat == 11 )
				return true;
			return false;
		case 18:
			if ( nextSeat == 12 )
				return true;
			return false;
		case 19:
			if ( nextSeat == 13 )
				return true;
			return false;
		case 20:
			if ( nextSeat == 14 )
				return true;
			return false;
		case 21:
			if ( nextSeat == 15 )
				return true;
			return false;
		case 22:
			if ( nextSeat == 16 )
				return true;
			return false;

		}
		
		return false;
*/
	}

	// ------------------------------------------------------------
	override string GetAnimSourceFromSelection( string selection )
	{
		return "";
	}

	// ------------------------------------------------------------
	override bool CrewCanGetThrough( int posIdx )
	{
		return true;
	}

	// ------------------------------------------------------------
	override bool IsVitalCarBattery()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBus::IsVitalCarBattery");
		#endif

		return false;
	}

	// ------------------------------------------------------------
	override bool IsVitalTruckBattery()
    {
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBus::IsVitalTruckBattery");
		#endif

        return true;
    }

	// ------------------------------------------------------------
	override bool IsVitalSparkPlug()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBus::IsVitalSparkPlug");
		#endif

		return false;
	}
	
	// ------------------------------------------------------------
	override bool IsVitalRadiator()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBus::IsVitalRadiator");
		#endif
		return false;
	}
	
	// ------------------------------------------------------------
	override bool IsVitalGlowPlug()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBus::IsVitalGlowPlug");
		#endif

		return true;
	}

	// ------------------------------------------------------------
	override bool IsVitalEngineBelt()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBus::IsVitalEngineBelt");
		#endif

		return false;
	}

	// ------------------------------------------------------------
	override bool CanObjectAttach( Object obj )
	{
		if ( vector.Distance( GetPosition(), obj.GetPosition() ) > m_BoundingRadius * 1.5 )
			return false;

		return true;
	}
}