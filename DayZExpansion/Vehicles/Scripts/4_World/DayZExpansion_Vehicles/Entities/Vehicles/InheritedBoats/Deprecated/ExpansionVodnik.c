/*
 * ExpansionVodnik.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVodnikDoorDriver: CarDoor
{
};

class ExpansionVodnikDoorCoDriver: CarDoor
{
};

class ExpansionVodnikDoorCargo1: CarDoor
{
};

class ExpansionVodnikDoorCargo2: CarDoor
{
};

class ExpansionVodnik: ExpansionBoatScript
{
	protected ref UniversalTemperatureSource m_UTSource;
	protected ref UniversalTemperatureSourceSettings m_UTSSettings;
	protected ref UniversalTemperatureSourceLambdaEngine m_UTSLEngine;

	// ------------------------------------------------------------
	void ExpansionVodnik()
	{
		//! Vanilla
		m_dmgContactCoef = 0.018;
		//! Custom
		m_MaxSpeed					= 20.0;
		m_TurnCoef					= 0.15;
	
		m_Offset					= -0.4;

		//! 1.19
		m_EngineStartOK			= "Offroad_02_engine_start_SoundSet";
		m_EngineStartBattery	= "Offroad_02_engine_failed_start_battery_SoundSet";
		m_EngineStartPlug		= "Offroad_02_engine_failed_start_sparkplugs_SoundSet";
		m_EngineStartFuel		= "Offroad_02_engine_failed_start_fuel_SoundSet";
		m_EngineStopFuel		= "offroad_engine_stop_fuel_SoundSet";

		m_CarDoorOpenSound		= "offroad_02_door_open_SoundSet";
		m_CarDoorCloseSound		= "offroad_02_door_close_SoundSet";

		m_CarHornShortSoundName = "Offroad_02_Horn_Short_SoundSet";
		m_CarHornLongSoundName	= "Offroad_02_Horn_SoundSet";
		
		SetEnginePos("0 1.36 -0.4");
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

	override string ExpansionGetWheelType(int slot_id)
	{
		return "ExpansionVodnikWheel";
	}

	override int GetAnimInstance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetAnimInstance");
#endif

		return ExpansionVehicleAnimInstances.EXPANSION_VODNIK;
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
		case 4:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
		case 5:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_R;
		case 6:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_R;
		case 7:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
		case 8:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_R;
		case 9:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
		}

		return 0;
	}

	override string GetDoorSelectionNameFromSeatPos(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetDoorSelectionNameFromSeatPos").Add(posIdx);
#endif

		switch (posIdx)
		{
		case 0:
			return "vodnikdriverdoor";
			break;
		case 1:
			return "vodnikcodriverdoor";
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
			return "vodnikdriverdoor";
			break;
		case 1:
			return "vodnikcodriverdoor";
			break;
		}

		return super.GetDoorInvSlotNameFromSeatPos(posIdx);
	}

	override void UpdateVisuals()
	{
		if ( AllDoorsClosed() )
		{
			ShowSelection( "antiwater" );
		} else {
			HideSelection( "antiwater" );
		}
	}

	override void EEItemAttached( EntityAI item, string slot_name ) 
	{
		super.EEItemAttached( item, slot_name );

		UpdateVisuals();
	}

	override void EEItemDetached( EntityAI item, string slot_name )
	{
		super.EEItemDetached( item, slot_name );

		UpdateVisuals();
	}

	override int GetCarDoorsState(string slotType)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetCarDoorsState").Add(slotType);
#endif

		CarDoor carDoor;
		if ( !Class.CastTo( carDoor, FindAttachmentBySlotName( slotType ) ) )
			return CarDoorState.DOORS_MISSING;

		switch (slotType)
		{
		case "vodnikdriverdoor":
		{
			if (GetAnimationPhase("vodnikdriverdoor") > 0.5)
			{
				return CarDoorState.DOORS_OPEN;
			}
			else
			{
				return CarDoorState.DOORS_CLOSED;
			}
			break;
		}
		case "vodnikcodriverdoor":
		{
			if (GetAnimationPhase("vodnikcodriverdoor") > 0.5)
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

	override float OnSound(CarSoundCtrl ctrl, float oldValue)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "OnSound").Add(ctrl).Add(oldValue);
#endif

		switch (ctrl)
		{
		case CarSoundCtrl.DOORS:
			float newValue = 0;

			if (GetCarDoorsState("vodnikdriverdoor") == CarDoorState.DOORS_CLOSED)
			{
				newValue += 0.8;
			}

			if (GetCarDoorsState("vodnikcodriverdoor") == CarDoorState.DOORS_CLOSED)
			{
				newValue += 0.8;
			}

			if (newValue > 1)
				newValue = 1;

			return newValue;
		default:
			break;
		}

		return super.OnSound(ctrl, oldValue);
	}
	
	override protected void HandleDoorsSound(string animSource, float phase)
	{
		switch (animSource)
		{
		case "vodnikdriverdoor":
		case "vodnikcodriverdoor":
			if (phase == 0)
			{
				SEffectManager.Expansion_PlaySound(m_CarDoorOpenSound, GetPosition());
			}
			else if (phase == 1)
			{
				SEffectManager.Expansion_PlaySound(m_CarDoorCloseSound, GetPosition());
			}
			break;
		default:
			super.HandleDoorsSound(animSource, phase);
		}
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
			if (pDoorsSelection == "vodnikdriverdoor")
			{
				return true;
			}
			break;
		}
		case 1:
		{
			if (pDoorsSelection == "vodnikcodriverdoor")
			{
				return true;
			}
			break;
		}
		break;
		}
		return true;
	}

	override bool CanReachSeatFromDoors(string pSeatSelection, vector pFromPos, float pDistance = 1.0)
	{

		return true;
	}

	override string GetAnimSourceFromSelection(string selection)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetAnimSourceFromSelection").Add(selection);
#endif

		switch (selection)
		{
		case "vodnikdriverdoor":
			return "vodnikdriverdoor";
		case "vodnikcodriverdoor":
			return "vodnikcodriverdoor";
		}

		return "";
	}

	override bool CrewCanGetThrough(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CrewCanGetThrough").Add(posIdx);
#endif

		switch (posIdx)
		{
		case 0:
			if (GetCarDoorsState("vodnikdriverdoor") == CarDoorState.DOORS_CLOSED)
				return false;

			return true;
			break;

		case 1:
			if (GetCarDoorsState("vodnikcodriverdoor") == CarDoorState.DOORS_CLOSED)
				return false;

			return true;
			break;
		}
		return true;
	}

	override float GetActionDistanceFuel()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetActionDistanceFuel");
#endif

		return 4.5;
	}

	override string GetActionCompNameFuel()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetActionCompNameFuel");
#endif

		return "refill";
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

		return true;
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

		return false;
	}

	override bool IsVitalEngineBelt()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalEngineBelt");
#endif

		return false;
	}

	override bool IsCar()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsCar");
#endif

		return true;
	}

	override float GetTransportCameraDistance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetTransportCameraDistance");
#endif

		return 7.0;
	}

	override vector GetTransportCameraOffset()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetTransportCameraOffset");
#endif

		return "0 1.5 0";
	}

	override bool LeavingSeatDoesAttachment(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "LeavingSeatDoesAttachment").Add(posIdx);
#endif

		switch (posIdx)
		{
		case 0:
			return false;
			break;

		case 1:
			return false;
			break;
		}

		return true;
	}

	override void UpdateLights(int new_gear = -1)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "UpdateLights").Add(new_gear);
#endif

		super.UpdateLights(new_gear);

		if (!GetGame().IsDedicatedServer())
		{
			ItemBase battery;

			if (IsVitalTruckBattery())
				battery = ItemBase.Cast(FindAttachmentBySlotName("TruckBattery"));

			if (battery)
			{
				int b;

				vector color;
				vector ambient;

				if (m_HeadlightsOn)
				{
					if (m_Lights.Count() == 0)
					{
						CreateLights(this, "intlight", ExpansionPointLight, Vector(1, 1, 1), Vector(1, 1, 1), 5, 1, false, true);
					}
				}
				else
				{
					for (b = 0; b < m_Particles.Count(); b++)
					{
						if ( m_Particles[b] )
						{
							m_Particles[b].Stop();

							GetGame().ObjectDelete(m_Particles[b]);
						}
					}

					for (b = 0; b < m_Lights.Count(); b++)
					{
						if ( m_Lights[b] )
						{
							m_Lights[b].ExpansionSetEnabled(false);

							GetGame().ObjectDelete(m_Lights[b]);
						}
					}

					m_Lights.Clear();
				}
			}
		}
	}

	override void OnDebugSpawn()
	{
		EntityAI entity;

		if (Class.CastTo(entity, this))
		{
			entity.GetInventory().CreateInInventory("ExpansionVodnikWheel");
			entity.GetInventory().CreateInInventory("ExpansionVodnikWheel");
			entity.GetInventory().CreateInInventory("ExpansionVodnikWheel");
			entity.GetInventory().CreateInInventory("ExpansionVodnikWheel");

			entity.GetInventory().CreateInInventory("TruckBattery");
			entity.GetInventory().CreateInInventory("SparkPlug");
			entity.GetInventory().CreateInInventory("CarRadiator");

			entity.GetInventory().CreateInInventory("ExpansionVodnikDoorDriver");
			entity.GetInventory().CreateInInventory("ExpansionVodnikDoorCoDriver");

			entity.GetInventory().CreateInInventory("HeadlightH7");
			entity.GetInventory().CreateInInventory("HeadlightH7");
		}

		Fill(CarFluid.FUEL, 50);
		Fill(CarFluid.COOLANT, 6.0);
		Fill(CarFluid.OIL, 4.0);
	}
};

