class ExpansionHatchBird: ExpansionHelicopterScript
{
	protected ref UniversalTemperatureSource m_UTSource;
	protected ref UniversalTemperatureSourceSettings m_UTSSettings;
	protected ref UniversalTemperatureSourceLambdaEngine m_UTSLEngine;

	void ExpansionHatchBird()
	{
		m_dmgContactCoef 		= 0.075;
	
		m_EngineStartOK 		= "offroad_engine_start_SoundSet";
		m_EngineStartBattery 	= "offroad_engine_failed_start_battery_SoundSet";
		m_EngineStartPlug 		= "offroad_engine_failed_start_sparkplugs_SoundSet";
		m_EngineStartFuel 		= "offroad_engine_failed_start_fuel_SoundSet";
		m_EngineStopFuel 		= "offroad_engine_stop_fuel_SoundSet";
		
		m_CarDoorOpenSound 		= "offroad_door_open_SoundSet";
		m_CarDoorCloseSound 	= "offroad_door_close_SoundSet";
		
		m_CarHornShortSoundName = "Offroad_Horn_Short_SoundSet";
		m_CarHornLongSoundName	= "Offroad_Horn_SoundSet";
		
		SetEnginePos("0 0.7 1.2");

		m_ExplosionSize = 10;
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
		super.EOnPostSimulate(other, timeSlice);
		
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
		return VehicleAnimInstances.HATCHBACK;
	}
	
	override float GetTransportCameraDistance()
	{
		return 3.5;
	}

	override vector GetTransportCameraOffset()
	{
		return "0 1.3 0";
	}

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
		}

		return 0;

	}

	override bool IsDuck()
	{
		return true;
	}
	
	
	override CarRearLightBase CreateRearLight()
	{
		return CarRearLightBase.Cast( ScriptedLightBase.CreateLight(OffroadHatchbackRearLight) );
	}
	
	override CarLightBase CreateFrontLight()
	{
		return CarLightBase.Cast( ScriptedLightBase.CreateLight(OffroadHatchbackFrontLight) );
	}

	override bool CanReleaseAttachment(EntityAI attachment)
	{
		if (!super.CanReleaseAttachment(attachment))
		{
			return false;
		}
		
		if (EngineIsOn() || GetCarDoorsState("NivaHood") == CarDoorState.DOORS_CLOSED)
		{
			string attType = attachment.GetType();
			if (attType == "CarRadiator" || attType == "CarBattery" || attType == "SparkPlug")
			{
				return false;
			}
		}

		return true;
	}
	
	override protected bool CanManipulateSpareWheel(string slotSelectionName)
	{
		return GetCarDoorsState("NivaHood") != CarDoorState.DOORS_CLOSED;
	}

	override bool CanDisplayAttachmentCategory( string category_name )
	{
		if (!super.CanDisplayAttachmentCategory(category_name))
		{
			return false;
		}
	
		category_name.ToLower();		
		if (category_name.Contains("engine"))
		{
			if (GetCarDoorsState("NivaHood") == CarDoorState.DOORS_CLOSED)
			{
				return false;
			}
		}
				
		return true;
	}
	
	override bool CanDisplayCargo()
	{
		if (!super.CanDisplayCargo())
		{
			return false;
		}
		
		if (GetCarDoorsState("NivaTrunk") == CarDoorState.DOORS_CLOSED)
		{
			return false;
		}
		
		return true;
	}

	override int GetCarDoorsState(string slotType)
	{
		CarDoor carDoor;
		
		Class.CastTo(carDoor, FindAttachmentBySlotName(slotType));
		if (!carDoor)
		{
			return CarDoorState.DOORS_MISSING;
		}

		switch (slotType)
		{
		case "NivaDriverDoors":
			return TranslateAnimationPhaseToCarDoorState("DoorsDriver");
			break;
			
		case "NivaCoDriverDoors":
			return TranslateAnimationPhaseToCarDoorState("DoorsCoDriver");
			break;
			
		case "NivaHood":
			return TranslateAnimationPhaseToCarDoorState("DoorsHood");
			break;
			
		case "NivaTrunk":
			return TranslateAnimationPhaseToCarDoorState("DoorsTrunk");
			break;
		}

		return CarDoorState.DOORS_MISSING;
	}
	
	override bool CrewCanGetThrough( int posIdx )
	{
		CarDoor carDoor;
		switch (posIdx)
		{
		case 0:
			if (GetCarDoorsState("NivaDriverDoors") == CarDoorState.DOORS_CLOSED)
				return false;
			else if (GetAnimationPhase("SeatDriver") > 0.5)
				return false;

			return true;
		break;

		case 1:
			if (GetCarDoorsState("NivaCoDriverDoors") == CarDoorState.DOORS_CLOSED)
				return false;
			else if (GetAnimationPhase("SeatCoDriver") > 0.5)
				return false;

			return true;
		break;

		case 2:
			if (GetCarDoorsState("NivaDriverDoors") == CarDoorState.DOORS_CLOSED)
				return false;
			else if (GetAnimationPhase("SeatDriver") <= 0.5)
				return false;

			return true;
		break;

		case 3:
			if (GetCarDoorsState("NivaCoDriverDoors") == CarDoorState.DOORS_CLOSED)
				return false;
			else if (GetAnimationPhase("SeatCoDriver") <= 0.5)
				return false;

			return true;
		break;
		}

		return false;
	}

	override string GetDoorSelectionNameFromSeatPos(int posIdx)
	{
		switch (posIdx)
		{
		case 0:
		case 2:
			return "doors_driver";
		break;
		case 1:
		case 3:
			return "doors_codriver";
		break;
		}
		
		return super.GetDoorSelectionNameFromSeatPos(posIdx);
	}

	override string GetDoorInvSlotNameFromSeatPos(int posIdx)
	{
		switch (posIdx)
		{
		case 0:
		case 2:
			return "NivaDriverDoors";
		break;
		case 1:
		case 3:
			return "NivaCoDriverDoors";
		break;
		}
		
		return super.GetDoorInvSlotNameFromSeatPos(posIdx);
	}
	
	// 0 full ambient and engine sound
	// 1 zero ambient and engine sound
	override float OnSound(CarSoundCtrl ctrl, float oldValue)
	{
		float tempCap = 0.0;

		switch (ctrl)
		{
		case CarSoundCtrl.DOORS:
			float newValue = 0;
			if (GetCarDoorsState("NivaDriverDoors") == CarDoorState.DOORS_CLOSED)
			{
				newValue += 0.5;
			}

			if (GetCarDoorsState("NivaCoDriverDoors") == CarDoorState.DOORS_CLOSED)
			{
				newValue += 0.5;
			}

			if (GetCarDoorsState("NivaTrunk") == CarDoorState.DOORS_CLOSED)
			{
				newValue += 0.3;
			}
		
			if (GetHealthLevel("WindowFront") == GameConstants.STATE_RUINED)
			{
				newValue -= 0.6;
			}

			if (GetHealthLevel("WindowLR") == GameConstants.STATE_RUINED)
			{
				newValue -= 0.2;
			}
		
			if (GetHealthLevel("WindowRR") == GameConstants.STATE_RUINED)
			{
				newValue -= 0.2;
			}

			return Math.Clamp(newValue, 0, 1);
		break;
		}

		return super.OnSound(ctrl, oldValue);
	}

	override string GetAnimSourceFromSelection( string selection )
	{
		switch (selection)
		{
		case "doors_driver":
			return "DoorsDriver";
		case "doors_codriver":
			return "DoorsCoDriver";
		case "doors_hood":
			return "DoorsHood";
		case "doors_trunk":
			return "DoorsTrunk";
		case "seatback_driver":
			return "SeatDriver";
		case "seatback_codriver":
			return "SeatCoDriver";
		}

		return "";
	}
	
	override string GetDoorConditionPointFromSelection( string selection )
	{
		switch( selection )
		{
		case "seat_driver":
		case "seatback_driver":
		case "seat_cargo1":
			return "seat_con_1_1";
		break;
		case "seat_codriver":
		case "seatback_codriver":
		case "seat_cargo2":
			return "seat_con_2_1";
		break;
		}
		
		return "";
	}

	override int GetCrewIndex( string selection )
	{
		switch( selection )
		{
		case "seatback_driver":
			return 0;
		case "seatback_codriver":
			return 1;
		}

		return -1;
	}

	override bool IsVitalTruckBattery()
	{
		return false;
	}

	override bool IsVitalGlowPlug()
	{
		return false;
	}
	
	override bool CanReachSeatFromSeat( int currentSeat, int nextSeat )
	{
		if ( nextSeat == 0 && GetAnimationPhase("SeatDriver") > 0.5  )
			return false;
		
		if ( nextSeat == 1 && GetAnimationPhase("SeatCoDriver") > 0.5  )
			return false;
		
		switch( currentSeat )
		{
		case 0:
			if ( nextSeat == 1 )
			{
				return true;
			}
			break;
		case 1:
			if ( nextSeat == 0 )
			{
				return true;
			}
			break;
		case 2:
			if ( nextSeat == 3 )
			{
				return true;
			}
			break;
		case 3:
			if ( nextSeat == 2 )
			{
				return true;
			}
			break;
		}
		
		return false;
	}

	override bool CanReachDoorsFromSeat( string pDoorsSelection, int pCurrentSeat )
	{
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
			if (pDoorsSelection == "DoorsDriver")
			{
				return true;
			}
		break;
		case 3:
			if (pDoorsSelection == "DoorsCoDriver")
			{
				return true;
			}
		break;
		}
		
		return false;		
	}
	
	override int GetSeatIndexFromDoor( string pDoorSelection )
	{
		switch (pDoorSelection)
		{
			case "DoorsDriver":
				return 0;
				break;
			case "DoorsCoDriver":
				return 1;
				break;
		}
		return -1;
	}
	
	override void OnDebugSpawn()
	{
		GetInventory().CreateInInventory( "HatchbackWheel" );
		GetInventory().CreateInInventory( "HatchbackWheel" );
		GetInventory().CreateInInventory( "HatchbackWheel" );
		GetInventory().CreateInInventory( "HatchbackWheel" );
		GetInventory().CreateInInventory( "HatchbackWheel" );

		GetInventory().CreateInInventory( "ExpansionHelicopterBattery" );
		GetInventory().CreateInInventory( "ExpansionIgniterPlug" );
		GetInventory().CreateInInventory( "ExpansionHydraulicHoses" );

		GetInventory().CreateInInventory( "ExpansionHatchBirdDoors_Driver" );
		GetInventory().CreateInInventory( "ExpansionHatchBirdDoors_CoDriver" );
		GetInventory().CreateInInventory( "ExpansionHatchBirdHood" );
		GetInventory().CreateInInventory( "ExpansionHatchBirdTrunk" );

		GetInventory().CreateInInventory( "HeadlightH7" );
		GetInventory().CreateInInventory( "HeadlightH7" );

		Fill( CarFluid.FUEL, 50 );
		Fill( CarFluid.COOLANT, 6.0 );
		Fill( CarFluid.OIL, 4.0 );
	}
	
	override void SetActions()
	{
		super.SetActions();

		AddAction(ActionAnimateSeats);
	}
}
