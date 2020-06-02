class ExpansionBusTest extends ExpansionCarScript
{
	override int GetAnimInstance()
	{
		return ExpansionVehicleAnimInstances.EXPANSION_BUS;
	}

	override int GetSeatAnimationType( int posIdx )
	{
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
	
	// Override for car-specific light type
	override CarLightBase CreateFrontLight()
	{
		return CarLightBase.Cast( ScriptedLightBase.CreateLight(ExpansionRearCarLights) );
	}
	
	// Override for car-specific light type
	override CarRearLightBase CreateRearLight()
	{
		return CarRearLightBase.Cast( ScriptedLightBase.CreateLight(ExpansionCarFrontLight) );
	}
	
	override bool CanReleaseAttachment( EntityAI attachment )
	{
		if( !super.CanReleaseAttachment( attachment ) )
			return false;
		
		return true;
	}
	
	override int GetCarDoorsState( string slotType )
	{
		return CarDoorState.DOORS_MISSING;
	}
	
	override bool CrewCanGetThrough( int posIdx )
	{
		return true;
	}

	override string GetAnimSourceFromSelection( string selection )
	{
		return "";
	}

	override string GetDoorConditionPointFromSelection( string selection )
	{
		return selection;
	}
	
	override bool IsVitalCarBattery()
	{
		return false;
	}

	override bool IsVitalSparkPlug()
	{
		return false;
	}
	
	override bool IsVitalRadiator()
	{
		return false;
	}

	override bool IsVitalTruckBattery()
	{
		return true;
	}

	override bool IsVitalGlowPlug()
	{
		return true;
	}
	
	override bool CanReachSeatFromSeat( int currentSeat, int nextSeat )
	{
		return false;
	}

	override bool CanReachDoorsFromSeat( string pDoorsSelection, int pCurrentSeat )
	{
		return true;		
	}
};