/*
 * ExpansionTractor.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */
class ExpansionTractorDoorsDriver: CarDoor
{
};

class ExpansionTractorDoorsCodriver: CarDoor
{
};

class ExpansionTractor: CarScript
{

	protected ref UniversalTemperatureSource m_UTSource;
	protected ref UniversalTemperatureSourceSettings m_UTSSettings;
	protected ref UniversalTemperatureSourceLambdaEngine m_UTSLEngine;
	
	void ExpansionTractor()
	{
		m_dmgContactCoef = 0.075;

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

		SetEnginePos("0 1.27 1.04");
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

		return 4.0;
	}

	override vector GetTransportCameraOffset()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetTransportCameraOffset");
#endif

		return "0 2.2 0";
	}

	override int GetAnimInstance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetAnimInstance");
#endif

		return ExpansionVehicleAnimInstances.EXPANSION_TRACTOR;
	}

	override int GetSeatAnimationType(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetSeatAnimationType").Add(posIdx);
#endif

		return DayZPlayerConstants.VEHICLESEAT_DRIVER;
	}

	override CarLightBase CreateFrontLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "CreateFrontLight");
#endif

		return CarLightBase.Cast(ScriptedLightBase.CreateLight(ExpansionCarFrontLight));
	}

	override CarRearLightBase CreateRearLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "CreateRearLight");
#endif
		return CarRearLightBase.Cast(ScriptedLightBase.CreateLight(ExpansionRearCarLights));
	}

	override string ExpansionGetWheelType(int slot_id)
	{
		if (InventorySlots.GetSlotName(slot_id) == "ExpansionTractorFrontWheel_1_1" || InventorySlots.GetSlotName(slot_id) == "ExpansionTractorFrontWheel_2_1")
			return "ExpansionTractorFrontWheel";
		else
			return "ExpansionTractorBackWheel";
	}

	override bool CanReleaseAttachment(EntityAI attachment)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CanReleaseAttachment").Add(attachment);
#endif

		if (!super.CanReleaseAttachment(attachment))
			return false;

		string attType = attachment.GetType();

		if (EngineIsOn())
		{
			if (attType == "CarBattery")
				return false;
			
			if (attType == "SparkPlug")
				return false;
		}

		return true;
	}

	override bool CanDisplayAttachmentCategory( string category_name )
	{
		if ( !super.CanDisplayAttachmentCategory( category_name ) )
			return false;
	
		category_name.ToLower();		
		if ( category_name.Contains("engine") )
		{
			if ( m_ExpansionVehicle.AllDoorsClosed() )
				return false;
		}
				
		return true;
	}

	override bool CanDisplayCargo()
	{
		if ( !super.CanDisplayCargo() )
			return false;
		
		if ( m_ExpansionVehicle.AllDoorsClosed() )
			return false;
		
		return true;
	}

	override int GetCarDoorsState(string slotType)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetCarDoorsState").Add(slotType);
#endif

		CarDoor carDoor;
		Class.CastTo(carDoor, FindAttachmentBySlotName(slotType));
		if (!carDoor)
			return CarDoorState.DOORS_MISSING;

		switch (slotType)
		{
		case "ExpansionTractorDoorsDriver":
			if (GetAnimationPhase("ExpansionTractorDoorsDriver") > 0.5)
				return CarDoorState.DOORS_OPEN;
			else
				return CarDoorState.DOORS_CLOSED;
			break;

		case "ExpansionTractorDoorsCodriver":
			if (GetAnimationPhase("ExpansionTractorDoorsCodriver") > 0.5)
				return CarDoorState.DOORS_OPEN;
			else
				return CarDoorState.DOORS_CLOSED;
		}

		return CarDoorState.DOORS_MISSING;
	}

	override bool CrewCanGetThrough(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CrewCanGetThrough").Add(posIdx);
#endif

		CarDoor carDoor;
		switch (posIdx)
		{
		case 0:
			if (GetCarDoorsState("ExpansionTractorDoorsDriver") == CarDoorState.DOORS_CLOSED)
				return false;

			return true;
		}

		return false;
	}

	override string GetDoorSelectionNameFromSeatPos(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetDoorSelectionNameFromSeatPos").Add(posIdx);
#endif

		if (GetAnimationPhase("ExpansionTractorDoorsCodriver") > 0.5)
			return "expansiontractordoorscodriver";

		return "expansiontractordoorsdriver";
	}

	override string GetDoorInvSlotNameFromSeatPos(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetDoorInvSlotNameFromSeatPos").Add(posIdx);
#endif

		return "ExpansionTractorDoorsDriver";
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

			//-----
			if (GetCarDoorsState("ExpansionTractorDoorsDriver") == CarDoorState.DOORS_CLOSED)
			{
				newValue += 0.4;
			}

			if (GetCarDoorsState("ExpansionTractorDoorsCodriver") == CarDoorState.DOORS_CLOSED)
			{
				newValue += 0.4;
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
		case "expansiontractordoorsdriver":
		case "expansiontractordoorscodriver":
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

	override string GetAnimSourceFromSelection(string selection)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetAnimSourceFromSelection").Add(selection);
#endif

		switch (selection)
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalHelicopterBattery");
#endif

		return true;
	}

	override bool IsVitalTruckBattery()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalTruckBattery");
#endif

		return false;
	}

	override bool IsVitalSparkPlug()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalSparkPlug");
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

	override bool IsVitalRadiator()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalRadiator");
#endif

		return false;
	}

	override bool CanReachDoorsFromSeat(string pDoorsSelection, int pCurrentSeat)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "CanReachDoorsFromSeat").Add(pDoorsSelection).Add(pCurrentSeat);
#endif

		if (pDoorsSelection == "ExpansionTractorDoorsDriver")
			return true;
		if (pDoorsSelection == "ExpansionTractorDoorsCodriver")
			return true;

		return false;
	}

	override void OnDebugSpawn()
	{
		EntityAI entity;

		if (Class.CastTo(entity, this))
		{
			entity.GetInventory().CreateInInventory("ExpansionTractorFrontWheel");
			entity.GetInventory().CreateInInventory("ExpansionTractorFrontWheel");
			entity.GetInventory().CreateInInventory("ExpansionTractorBackWheel");
			entity.GetInventory().CreateInInventory("ExpansionTractorBackWheel");

			entity.GetInventory().CreateInInventory("ExpansionTractorDoorsDriver");
			entity.GetInventory().CreateInInventory("ExpansionTractorDoorsCodriver");

			entity.GetInventory().CreateInInventory("CarBattery");
			entity.GetInventory().CreateInInventory("SparkPlug");
			entity.GetInventory().CreateInInventory("HeadlightH7");
			entity.GetInventory().CreateInInventory("HeadlightH7");		
		}

		Fill(CarFluid.FUEL, 50);
		Fill(CarFluid.COOLANT, 6.0);
		Fill(CarFluid.OIL, 4.0);
	}
};
