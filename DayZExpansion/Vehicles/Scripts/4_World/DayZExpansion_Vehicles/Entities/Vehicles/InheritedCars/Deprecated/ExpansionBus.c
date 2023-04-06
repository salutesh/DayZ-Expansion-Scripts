/*
 * ExpansionBus.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class ExpansionBus: CarScript
{
	CarLightBase m_BusLight1;
	CarLightBase m_BusLight2;
	CarLightBase m_BusLight3;

	protected ref UniversalTemperatureSource m_UTSource;
	protected ref UniversalTemperatureSourceSettings m_UTSSettings;
	protected ref UniversalTemperatureSourceLambdaEngine m_UTSLEngine;

	void ExpansionBus()
	{
		m_dmgContactCoef = 0.035;

		m_EngineStartOK = "Truck_01_engine_start_SoundSet";
		m_EngineStartBattery = "Truck_01_engine_failed_start_battery_SoundSet";
		m_EngineStartPlug = "Truck_01_engine_failed_start_sparkplugs_SoundSet";
		m_EngineStartFuel = "Truck_01_engine_failed_start_fuel_SoundSet";
		m_EngineStopFuel = "Truck_01_engine_stop_fuel_SoundSet";

		m_CarDoorOpenSound = "Truck_01_door_open_SoundSet";
		m_CarDoorCloseSound = "Truck_01_door_close_SoundSet";
		
		//! 1.19
		m_CarHornShortSoundName = "Truck_01_Horn_Short_SoundSet";
		m_CarHornLongSoundName	= "Truck_01_Horn_SoundSet";

		SetEnginePos("0.42 0.58 4.0");
	}
	
	override void EEInit()
	{		
		super.EEInit();
		
		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
 			m_UTSSettings 					= new UniversalTemperatureSourceSettings();
			m_UTSSettings.m_ManualUpdate 	= true;
			m_UTSSettings.m_TemperatureMin	= 0;
			m_UTSSettings.m_TemperatureMax	= 30;
			m_UTSSettings.m_RangeFull		= 0.5;
			m_UTSSettings.m_RangeMax		= 2;
			m_UTSSettings.m_TemperatureCap	= 25;
			
			m_UTSLEngine					= new UniversalTemperatureSourceLambdaEngine();
			m_UTSource						= new UniversalTemperatureSource(this, m_UTSSettings, m_UTSLEngine);
		}		
	}
	
	override void OnEngineStart()
	{
		super.OnEngineStart();

		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			m_UTSource.SetDefferedActive(true, 20.0);
		}
	}
	
	override void OnEngineStop()
	{
		super.OnEngineStop();

		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			m_UTSource.SetDefferedActive(false, 10.0);
		}
	}
	
	override void EOnPostSimulate(IEntity other, float timeSlice)
	{
		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			if (m_UTSource.IsActive())
			{
				m_UTSource.Update(m_UTSSettings, m_UTSLEngine);
			}
		}
	}

	override float GetTransportCameraDistance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetTransportCameraDistance");
#endif

		return 10.0;
	}

	override vector GetTransportCameraOffset()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetTransportCameraOffset");
#endif

		return "-0.25 3.2 0";
	}

	void ~ExpansionBus()
	{
		if (IsMissionClient())
		{
			if (m_BusLight1)
				m_BusLight1.Destroy();

			if (m_BusLight2)
				m_BusLight2.Destroy();

			if (m_BusLight2)
				m_BusLight2.Destroy();
		}
	}

	override string ExpansionGetWheelType(int slot_id)
	{
		if (InventorySlots.GetSlotName(slot_id) == "BusWheel_1_1" || InventorySlots.GetSlotName(slot_id) == "BusWheel_2_1")
			return "ExpansionBusWheel";
		else
			return "ExpansionBusWheelDouble";
	}

	void PartyBus()
	{
		Megaphone megaphone = Megaphone.Cast(GetInventory().CreateInInventory("Megaphone"));
		megaphone.GetInventory().CreateAttachment("Battery9V");

		megaphone = Megaphone.Cast(GetInventory().CreateInInventory("Megaphone"));
		megaphone.GetInventory().CreateAttachment("Battery9V");

		megaphone = Megaphone.Cast(GetInventory().CreateInInventory("Megaphone"));
		megaphone.GetInventory().CreateAttachment("Battery9V");
	}

	override int GetAnimInstance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetAnimInstance");
#endif

		return ExpansionVehicleAnimInstances.EXPANSION_BUS;
	}

	override CarRearLightBase CreateRearLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "CreateRearLight");
#endif
		return CarRearLightBase.Cast(ScriptedLightBase.CreateLight(ExpansionRearCarLights));
	}

	override CarLightBase CreateFrontLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "CreateFrontLight");
#endif

		return CarLightBase.Cast(ScriptedLightBase.CreateLight(ExpansionCarFrontLight));
	}

	CarLightBase CreateBusLight()
	{
		return CarLightBase.Cast(ScriptedLightBase.CreateLight(ExpansionBusLights));
	}

	override void UpdateLights(int new_gear = -1) // -1 is invalid gear.
	{
		if (!GetGame().IsServer() || !GetGame().IsMultiplayer()) // client side
		{
			ItemBase battery;

			if (IsVitalTruckBattery())
				battery = ItemBase.Cast(FindAttachmentBySlotName("TruckBattery"));

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
						if (IsInherited(ExpansionBus))
						{
							m_BusLight1.AttachOnObject(this, GetMemoryPointPos("intlight1"));
							m_BusLight1.AggregateLight();

							m_BusLight2.AttachOnObject(this, GetMemoryPointPos("intlight2"));
							m_BusLight2.AggregateLight();

							m_BusLight3.AttachOnObject(this, GetMemoryPointPos("intlight3"));
							m_BusLight3.AggregateLight();
						}
					}

					DashboardShineOn();

					if (!m_Headlight && m_HeadlightsState != CarHeadlightBulbsState.NONE)
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

						vector local_pos_middle = (local_pos_left + local_pos_right) * 0.5;
						m_Headlight.AttachOnObject(this, local_pos_middle);
						m_Headlight.AggregateLight();
						LeftFrontLightShineOn();
						RightFrontLightShineOn();
					}

					if (m_Headlight && m_HeadlightsState == CarHeadlightBulbsState.NONE)
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

				if (EngineIsOn())
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

					if (reverse_light_state != CarRearLightType.NONE && m_HeadlightsState != CarHeadlightBulbsState.NONE && !m_RearLight)
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

	override string GetDoorConditionPointFromSelection(string selection)
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

	override int GetCarDoorsState(string slotType)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetCarDoorsState").Add(slotType);
#endif

		return CarDoorState.DOORS_MISSING;
	}

	override bool CanReachDoorsFromSeat(string pDoorsSelection, int pCurrentSeat)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "CanReachDoorsFromSeat").Add(pDoorsSelection).Add(pCurrentSeat);
#endif

		return true;
	}

	// ------------------------------------------------------------
	override bool CanReachSeatFromSeat(int currentSeat, int nextSeat)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "CanReachSeatFromSeat").Add(currentSeat).Add(nextSeat);
#endif

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

	override string GetAnimSourceFromSelection(string selection)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetAnimSourceFromSelection").Add(selection);
#endif

		return "";
	}

	override bool CrewCanGetThrough(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CrewCanGetThrough").Add(posIdx);
#endif

		return true;
	}

	override bool IsVitalCarBattery()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalHelicopterBattery");
#endif

		return false;
	}

	override bool IsVitalTruckBattery()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalTruckBattery");
#endif

		return true;
	}

	override bool IsVitalSparkPlug()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalSparkPlug");
#endif

		return false;
	}

	override bool IsVitalRadiator()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalRadiator");
#endif

		return true;
	}

	override bool IsVitalGlowPlug()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalGlowPlug");
#endif

		return true;
	}

	override bool IsVitalEngineBelt()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalEngineBelt");
#endif

		return false;
	}

	override bool LeavingSeatDoesAttachment(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "LeavingSeatDoesAttachment").Add(posIdx);
#endif

		return true;
	}

	override void OnDebugSpawn()
	{
		EntityAI entity;

		if (Class.CastTo(entity, this))
		{
			entity.GetInventory().CreateInInventory("ExpansionBusWheel");
			entity.GetInventory().CreateInInventory("ExpansionBusWheel");
			entity.GetInventory().CreateInInventory("ExpansionBusWheelDouble");
			entity.GetInventory().CreateInInventory("ExpansionBusWheelDouble");

			entity.GetInventory().CreateInInventory("TruckBattery");
			entity.GetInventory().CreateInInventory("GlowPlug");
			entity.GetInventory().CreateInInventory("CarRadiator");
			entity.GetInventory().CreateInInventory("HeadlightH7");
			entity.GetInventory().CreateInInventory("HeadlightH7");		
		}

		Fill(CarFluid.FUEL, 50);
		Fill(CarFluid.COOLANT, 6.0);
		Fill(CarFluid.OIL, 4.0);
	}
};
