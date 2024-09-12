/*
 * ExpansionLandRover.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_Landrover_Driverdoor: CarDoor{};
class Expansion_Landrover_Codriverdoor: CarDoor{};
class Expansion_Landrover_Left: CarDoor{};
class Expansion_Landrover_Right: CarDoor{};
class Expansion_Landrover_Trunk: CarDoor{};
class Expansion_Landrover_Hood: CarDoor{};

class Expansion_Landrover: Expansion_Landrover_Base{};

class Expansion_Landrover_Base: CarScript
{
	protected ref UniversalTemperatureSource m_UTSource;
	protected ref UniversalTemperatureSourceSettings m_UTSSettings;
	protected ref UniversalTemperatureSourceLambdaEngine m_UTSLEngine;
	
	void Expansion_Landrover_Base()
	{
		m_dmgContactCoef = 0.030;

		m_EngineStartOK = "offroad_engine_start_SoundSet";
		m_EngineStartBattery = "offroad_engine_failed_start_battery_SoundSet";
		m_EngineStartPlug = "offroad_engine_failed_start_sparkplugs_SoundSet";
		m_EngineStartFuel = "offroad_engine_failed_start_fuel_SoundSet";
		m_EngineStopFuel = "offroad_engine_stop_fuel_SoundSet";
		
		m_CarDoorOpenSound = "offroad_door_open_SoundSet";
		m_CarDoorCloseSound = "offroad_door_close_SoundSet";
		
		//! 1.19
		m_CarHornShortSoundName = "Offroad_Horn_Short_SoundSet";
		m_CarHornLongSoundName	= "Offroad_Horn_SoundSet";
		
		SetEnginePos("0 0.95 1.7");
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
    
	override int GetAnimInstance()
	{
		return VehicleAnimInstances.SEDAN;
	}
	
	override CarRearLightBase CreateRearLight()
	{
		return CarRearLightBase.Cast( ScriptedLightBase.CreateLight(ExpansionRearCarLights) );
	}
	
	override CarLightBase CreateFrontLight()
	{
		return CarLightBase.Cast( ScriptedLightBase.CreateLight(ExpansionCarFrontLight) );
	}
	
	override float GetActionDistanceFuel()
    {
        return 3.2;
    }
	
	override float GetActionDistanceCoolant()
    {
        return 4.2;
    }

	override bool IsVitalTruckBattery()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalTruckBattery");
#endif
		return false;
	}

	override bool IsVitalGlowPlug()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalGlowPlug");
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
	
	override bool IsVitalCarBattery()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalHelicopterBattery");
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

	override bool CanDisplayAttachmentCategory( string category_name )
	{
		if ( !super.CanDisplayAttachmentCategory( category_name ) )
			return false;
	
		category_name.ToLower();		
		if ( category_name.Contains("engine") )
		{
			if ( GetCarDoorsState("expansion_landrover_hood") == CarDoorState.DOORS_CLOSED )
				return false;
		}
				
		return true;
	}
	
	override bool CanDisplayCargo()
	{
		if ( !super.CanDisplayCargo() )
			return false;
		
		if ( GetCarDoorsState("expansion_landrover_trunk") == CarDoorState.DOORS_CLOSED )
			return false;
		
		return true;
	}
	
	override int GetCarDoorsState( string slotType )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetCarDoorsState").Add(slotType);
#endif
		CarDoor carDoor;
		Class.CastTo( carDoor, FindAttachmentBySlotName( slotType ) );
		if ( !carDoor )
			return CarDoorState.DOORS_MISSING;
	
		switch( slotType )
		{
			case "expansion_landrover_driverdoor":
				if ( GetAnimationPhase("DoorsDriver") > 0.5 )
					return CarDoorState.DOORS_OPEN;
				else
					return CarDoorState.DOORS_CLOSED;
				
            break;
			
			case "expansion_landrover_codriverdoor":
				if ( GetAnimationPhase("DoorsCoDriver") > 0.5 )
					return CarDoorState.DOORS_OPEN;
				else
					return CarDoorState.DOORS_CLOSED;
				
			break;
			
			case "expansion_landrover_left":
				if ( GetAnimationPhase("DoorsCargo1") > 0.5 )
					return CarDoorState.DOORS_OPEN;
				else
					return CarDoorState.DOORS_CLOSED;
				
			break;
			
			case "expansion_landrover_right":
				if ( GetAnimationPhase("DoorsCargo2") > 0.5 )
					return CarDoorState.DOORS_OPEN;
				else
					return CarDoorState.DOORS_CLOSED;
				
			break;
			
			case "expansion_landrover_trunk":
				if ( GetAnimationPhase("DoorsTrunk") > 0.5 )
					return CarDoorState.DOORS_OPEN;
				else
					return CarDoorState.DOORS_CLOSED;
				
			break;
			
			case "expansion_landrover_hood":
				if ( GetAnimationPhase("DoorsHood") > 0.5 )
					return CarDoorState.DOORS_OPEN;
				else
					return CarDoorState.DOORS_CLOSED;
				
			break;
		}
        return CarDoorState.DOORS_MISSING;
	}
	
	override string GetDoorConditionPointFromSelection( string selection )
	{
		switch (selection)
		{
			case "seat_driver":
				return "seat_con_1_1";
			break;
			case "seat_codriver":
				return "seat_con_2_1";
			break;
			case "seat_cargo1":
				return "seat_con_1_2";
			break;
			case "seat_cargo2":
				return "seat_con_2_2";
			break;
        }
		return "";
	}

	override bool CrewCanGetThrough( int posIdx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CrewCanGetThrough").Add(posIdx);
#endif	
		switch( posIdx )
		{
			case 0: {
				if ( GetAnimationPhase("DoorsDriver") > 0 ) {
					return true;
				}
				if ( FindAttachmentBySlotName("expansion_landrover_driverdoor") == NULL ) 
					return true;
				
				break;
			}
			case 1: {
				if ( GetAnimationPhase("DoorsCoDriver") > 0 ) {
					return true;
				}
				if ( FindAttachmentBySlotName("expansion_landrover_codriverdoor") == NULL ) 
					return true;
				
				break;
			}
			case 2: {
				if ( GetAnimationPhase("DoorsCargo1") > 0 ) {
					return true;
				}
				if ( FindAttachmentBySlotName("expansion_landrover_left") == NULL ) 
					return true;
				
				break;
			}
			case 3: {
				if ( GetAnimationPhase("DoorsCargo2") > 0 ) {
					return true;
				}
				if ( FindAttachmentBySlotName("expansion_landrover_right") == NULL ) 
					return true;
				
				break;
			}
			case 4: {
				if ( GetAnimationPhase("DoorsTrunk") > 0 ) {
					return true;
				}
				if ( FindAttachmentBySlotName("expansion_landrover_trunk") == NULL ) 
					return true;
				
				break;
			}
			case 5: {
				if ( GetAnimationPhase("DoorsHood") > 0 ) {
					return true;
				}
				if ( FindAttachmentBySlotName("expansion_landrover_hood") == NULL ) 
					return true;
				
				break;
			}
		}

		return false;
	}
	
	override string GetDoorSelectionNameFromSeatPos(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetDoorSelectionNameFromSeatPos").Add(posIdx);
#endif	
		switch( posIdx )
		{
		case 0:
			return "doors_driver";
		break;
		case 1:
			return "doors_codriver";
		break;
		case 2:
			return "doors_cargo_left";
		break;
		case 3:
			return "doors_cargo_right";
		break;
        }
		
		return super.GetDoorSelectionNameFromSeatPos(posIdx);
	}
	
	override string GetDoorInvSlotNameFromSeatPos(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetDoorInvSlotNameFromSeatPos").Add(posIdx);
#endif
		switch( posIdx )
		{
		case 0:
			return "expansion_landrover_driverdoor";
		break;
		case 1:
			return "expansion_landrover_codriverdoor";
		break;
		case 2:
			return "expansion_landrover_left";
		break;
		case 3:
			return "expansion_landrover_right";
		break;

		}
		
		return super.GetDoorInvSlotNameFromSeatPos(posIdx);
	}
	
	override bool CanReleaseAttachment( EntityAI attachment )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CanReleaseAttachment").Add(attachment);
#endif
		if( !super.CanReleaseAttachment( attachment ) )
			return false;
		
		string attType = attachment.GetType();
		
		switch( attType )
		{
			case "CarRadiator":
				if ( GetCarDoorsState("expansion_landrover_hood") == CarDoorState.DOORS_CLOSED || EngineIsOn() )
					return false;
			break;
			case "HeadlightH7":
				if ( GetCarDoorsState("expansion_landrover_hood") == CarDoorState.DOORS_CLOSED || EngineIsOn() )
					return false;
			break;
			case "CarBattery": 
				if ( GetCarDoorsState("expansion_landrover_hood") == CarDoorState.DOORS_CLOSED || EngineIsOn() )
					return false;
			break;
			case "SparkPlug":
				if ( GetCarDoorsState("expansion_landrover_hood") == CarDoorState.DOORS_CLOSED || EngineIsOn() )
					return false;
			break;			
            case "Expansion_Landrover_Trunk":
                ItemBase sparewheel;
                sparewheel = ItemBase.Cast(FindAttachmentBySlotName("expansion_landrover_sparewheel"));
                if ( sparewheel )
                    return false;
            break;
		}

		return true;
	}
	
	override bool CanReceiveAttachment (EntityAI attachment, int slotId)
    {
		if (!super.CanReceiveAttachment(attachment, slotId))
			return false;

        string slot_name = InventorySlots.GetSlotName(slotId);
        
        if (slot_name == "expansion_landrover_sparewheel" && GetCarDoorsState( "expansion_landrover_trunk" ) == CarDoorState.DOORS_MISSING)
        {
            return false;
        }
        return true;
    }

	override string GetAnimSourceFromSelection( string selection )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetAnimSourceFromSelection").Add(selection);
#endif
		switch( selection )
		{
		case "doors_driver":
			return "DoorsDriver";
		case "doors_codriver":
			return "DoorsCoDriver";
		case "doors_cargo_left":
			return "DoorsCargo1";
		case "doors_cargo_right":
			return "DoorsCargo2";
		case "doors_trunk":
			return "DoorsTrunk";
		case "doors_hood":
			return "DoorsHood";
		}
        return "";
	}

	override int GetSeatAnimationType( int posIdx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetSeatAnimationType").Add(posIdx);
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
				return false;
			break;
		case 3:
			if ( nextSeat == 2 )
				return false;
			break;
		case 4:
			if ( nextSeat == 3 )
				return false;
			break;
        }
		return false;
	}

	override bool CanReachDoorsFromSeat( string pDoorsSelection, int pCurrentSeat )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "CanReachDoorsFromSeat").Add(pDoorsSelection).Add(pCurrentSeat);
#endif
		switch( pCurrentSeat )
		{
		case 0:
			if (pDoorsSelection == "DoorsDriver")
			{
				return true;
			}
		break;
		case 1:
			if (pDoorsSelection == "DoorsCoDriver")
			{
				return true;
			}
		break;
		case 2:
			if (pDoorsSelection == "DoorsCargo1")
			{
				return true;
			}
		break;
		case 3:
			if (pDoorsSelection == "DoorsCargo2")
			{
				return true;
			}
		break;
		case 4:
			if (pDoorsSelection == "DoorsTrunk")
			{
				return true;
			}
		break;
        }
		return false;		
	}
	
	override float OnSound( CarSoundCtrl ctrl, float oldValue )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "OnSound").Add(ctrl).Add(oldValue);
#endif
		switch ( ctrl )
		{
			case CarSoundCtrl.DOORS:
				float newValue = 0;
				
				if ( GetCarDoorsState( "expansion_landrover_driverdoor" ) == CarDoorState.DOORS_CLOSED )
					newValue += 0.25;
				
				if ( GetCarDoorsState( "expansion_landrover_codriverdoor" ) == CarDoorState.DOORS_CLOSED )
					newValue += 0.25;
				
	         	if ( GetCarDoorsState( "expansion_landrover_left" ) == CarDoorState.DOORS_CLOSED )
					newValue += 0.25;
					
				if ( GetCarDoorsState( "expansion_landrover_right" ) == CarDoorState.DOORS_CLOSED )
					newValue += 0.25;
					
				if ( GetCarDoorsState( "expansion_landrover_trunk" ) == CarDoorState.DOORS_CLOSED )
					newValue += 0.25;
				
				if ( GetHealthLevel( "WindowFront") == GameConstants.STATE_RUINED )
					newValue -= 0.6;
					
				if ( newValue > 1 )
					newValue = 1;
			
				return newValue;
			break;
		}

		return super.OnSound(ctrl, oldValue);
	}

	override float GetTransportCameraDistance()
	{
		return 6.0;
	}

	override vector GetTransportCameraOffset()
	{
		return "0 1.6 0";
	}
	
	override void OnDebugSpawn()
	{
		EntityAI entity;

		if (Class.CastTo(entity, this))
		{
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");

			entity.GetInventory().CreateInInventory("CarBattery");
			entity.GetInventory().CreateInInventory("SparkPlug");
			entity.GetInventory().CreateInInventory("CarRadiator");
			entity.GetInventory().CreateInInventory("HeadlightH7");
			entity.GetInventory().CreateInInventory("HeadlightH7");
			entity.GetInventory().CreateInInventory("SeaChest");
			entity.GetInventory().CreateInInventory("WoodenCrate");
			entity.GetInventory().CreateInInventory("WoodenCrate");
			entity.GetInventory().CreateInInventory("WoodenCrate");
			entity.GetInventory().CreateInInventory("CanisterGasoline");
			

			entity.GetInventory().CreateInInventory("Expansion_Landrover_Hood");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Driverdoor");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Codriverdoor");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Left");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Right");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Trunk");

			
		}

		Fill(CarFluid.FUEL, 50);
		Fill(CarFluid.COOLANT, 6.0);
		Fill(CarFluid.OIL, 4.0);
	}
};

modded class SeaChest
{
	override void SetActions()
	{
		super.SetActions();

		AddAction(ActionAttachOnSelection);
		AddAction(ActionDetach);
	}
};

modded class CanisterGasoline
{
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionAttachOnProxy);
		AddAction(ActionAttachOnSelection);
		AddAction(ActionDetach);
	}
};
modded class SeaChest
{
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionAttachOnSelection);
		AddAction(ActionDetach);
	}
};
modded class WoodenCrate
{
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionDetach);
	}
};

class Expansion_Landrover_Yellow: Expansion_Landrover_Base
{
	override void OnDebugSpawn()
	{
		EntityAI entity;

		if (Class.CastTo(entity, this))
		{
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");

			entity.GetInventory().CreateInInventory("CarBattery");
			entity.GetInventory().CreateInInventory("SparkPlug");
			entity.GetInventory().CreateInInventory("CarRadiator");
			entity.GetInventory().CreateInInventory("HeadlightH7");
			entity.GetInventory().CreateInInventory("HeadlightH7");
			entity.GetInventory().CreateInInventory("SeaChest");
			entity.GetInventory().CreateInInventory("WoodenCrate");
			entity.GetInventory().CreateInInventory("WoodenCrate");
			entity.GetInventory().CreateInInventory("WoodenCrate");
			entity.GetInventory().CreateInInventory("CanisterGasoline");
			

			entity.GetInventory().CreateInInventory("Expansion_Landrover_Hood_Yellow");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Driverdoor_Yellow");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Codriverdoor_Yellow");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Left_Yellow");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Right_Yellow");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Trunk_Yellow");
			
		}

		Fill(CarFluid.FUEL, 50);
		Fill(CarFluid.COOLANT, 6.0);
		Fill(CarFluid.OIL, 4.0);
	}
};

class Expansion_Landrover_Green: Expansion_Landrover_Base
{
	override void OnDebugSpawn()
	{
		EntityAI entity;

		if (Class.CastTo(entity, this))
		{
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");

			entity.GetInventory().CreateInInventory("CarBattery");
			entity.GetInventory().CreateInInventory("SparkPlug");
			entity.GetInventory().CreateInInventory("CarRadiator");
			entity.GetInventory().CreateInInventory("HeadlightH7");
			entity.GetInventory().CreateInInventory("HeadlightH7");
			entity.GetInventory().CreateInInventory("SeaChest");
			entity.GetInventory().CreateInInventory("WoodenCrate");
			entity.GetInventory().CreateInInventory("WoodenCrate");
			entity.GetInventory().CreateInInventory("WoodenCrate");
			entity.GetInventory().CreateInInventory("CanisterGasoline");
			

			entity.GetInventory().CreateInInventory("Expansion_Landrover_Hood_Green");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Driverdoor_Green");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Codriverdoor_Green");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Left_Green");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Right_Green");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Trunk_Green");	
		}

		Fill(CarFluid.FUEL, 50);
		Fill(CarFluid.COOLANT, 6.0);
		Fill(CarFluid.OIL, 4.0);
	}
};

class Expansion_Landrover_Red: Expansion_Landrover_Base
{
	override void OnDebugSpawn()
	{
		EntityAI entity;

		if (Class.CastTo(entity, this))
		{
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");

			entity.GetInventory().CreateInInventory("CarBattery");
			entity.GetInventory().CreateInInventory("SparkPlug");
			entity.GetInventory().CreateInInventory("CarRadiator");
			entity.GetInventory().CreateInInventory("HeadlightH7");
			entity.GetInventory().CreateInInventory("HeadlightH7");
			entity.GetInventory().CreateInInventory("SeaChest");
			entity.GetInventory().CreateInInventory("WoodenCrate");
			entity.GetInventory().CreateInInventory("WoodenCrate");
			entity.GetInventory().CreateInInventory("WoodenCrate");
			entity.GetInventory().CreateInInventory("CanisterGasoline");
			

			entity.GetInventory().CreateInInventory("Expansion_Landrover_Hood_Red");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Driverdoor_Red");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Codriverdoor_Red");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Left_Red");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Right_Red");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Trunk_Red");		
		}

		Fill(CarFluid.FUEL, 50);
		Fill(CarFluid.COOLANT, 6.0);
		Fill(CarFluid.OIL, 4.0);
	}
};

class Expansion_Landrover_Grey: Expansion_Landrover_Base
{
	override void OnDebugSpawn()
	{
		EntityAI entity;

		if (Class.CastTo(entity, this))
		{
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");

			entity.GetInventory().CreateInInventory("CarBattery");
			entity.GetInventory().CreateInInventory("SparkPlug");
			entity.GetInventory().CreateInInventory("CarRadiator");
			entity.GetInventory().CreateInInventory("HeadlightH7");
			entity.GetInventory().CreateInInventory("HeadlightH7");
			entity.GetInventory().CreateInInventory("SeaChest");
			entity.GetInventory().CreateInInventory("WoodenCrate");
			entity.GetInventory().CreateInInventory("WoodenCrate");
			entity.GetInventory().CreateInInventory("WoodenCrate");
			entity.GetInventory().CreateInInventory("CanisterGasoline");
			

			entity.GetInventory().CreateInInventory("Expansion_Landrover_Hood_Grey");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Driverdoor_Grey");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Codriverdoor_Grey");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Left_Grey");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Right_Grey");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Trunk_Grey");	
		}

		Fill(CarFluid.FUEL, 50);
		Fill(CarFluid.COOLANT, 6.0);
		Fill(CarFluid.OIL, 4.0);
	}
};

class Expansion_Landrover_Blue: Expansion_Landrover_Base
{
	override void OnDebugSpawn()
	{
		EntityAI entity;

		if (Class.CastTo(entity, this))
		{
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");

			entity.GetInventory().CreateInInventory("CarBattery");
			entity.GetInventory().CreateInInventory("SparkPlug");
			entity.GetInventory().CreateInInventory("CarRadiator");
			entity.GetInventory().CreateInInventory("HeadlightH7");
			entity.GetInventory().CreateInInventory("HeadlightH7");
			entity.GetInventory().CreateInInventory("SeaChest");
			entity.GetInventory().CreateInInventory("WoodenCrate");
			entity.GetInventory().CreateInInventory("WoodenCrate");
			entity.GetInventory().CreateInInventory("WoodenCrate");
			entity.GetInventory().CreateInInventory("CanisterGasoline");
			

			entity.GetInventory().CreateInInventory("Expansion_Landrover_Hood_Blue");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Driverdoor_Blue");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Codriverdoor_Blue");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Left_Blue");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Right_Blue");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Trunk_Blue");
		}

		Fill(CarFluid.FUEL, 50);
		Fill(CarFluid.COOLANT, 6.0);
		Fill(CarFluid.OIL, 4.0);
	}
};

class Expansion_Landrover_Orange: Expansion_Landrover_Base
{
	override void OnDebugSpawn()
	{
		EntityAI entity;

		if (Class.CastTo(entity, this))
		{
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Wheel");

			entity.GetInventory().CreateInInventory("CarBattery");
			entity.GetInventory().CreateInInventory("SparkPlug");
			entity.GetInventory().CreateInInventory("CarRadiator");
			entity.GetInventory().CreateInInventory("HeadlightH7");
			entity.GetInventory().CreateInInventory("HeadlightH7");
			entity.GetInventory().CreateInInventory("SeaChest");
			entity.GetInventory().CreateInInventory("WoodenCrate");
			entity.GetInventory().CreateInInventory("WoodenCrate");
			entity.GetInventory().CreateInInventory("WoodenCrate");
			entity.GetInventory().CreateInInventory("CanisterGasoline");
			

			entity.GetInventory().CreateInInventory("Expansion_Landrover_Hood_Orange");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Driverdoor_Orange");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Codriverdoor_Orange");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Left_Orange");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Right_Orange");
			entity.GetInventory().CreateInInventory("Expansion_Landrover_Trunk_Orange");
		}

		Fill(CarFluid.FUEL, 50);
		Fill(CarFluid.COOLANT, 6.0);
		Fill(CarFluid.OIL, 4.0);
	}
};