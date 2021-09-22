/*
 * Vehicle_ExpansionZodiacBoat.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Vehicle_ExpansionZodiacBoat extends ExpansionVehicleBoatBase
{
	// ------------------------------------------------------------
	void Vehicle_ExpansionZodiacBoat()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Vehicle_ExpansionZodiacBoat::Constructor - Start");
		#endif
		
		//! Vanilla
		m_dmgContactCoef			= 0.018;

		#ifdef EXPANSIONEXPRINT
		EXPrint("Vehicle_ExpansionZodiacBoat::Constructor - End");
		#endif
	}
	
	// ------------------------------------------------------------
	override int GetAnimInstance()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Vehicle_ExpansionZodiacBoat::GetAnimInstance");
		#endif
		
		return ExpansionVehicleAnimInstances.EXPANSION_ZODIAC;
	}

	// ------------------------------------------------------------
	override int GetSeatAnimationType( int posIdx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Vehicle_ExpansionZodiacBoat::GetSeatAnimationType");
		#endif
		switch( posIdx )
		{
		case 0:
			return DayZPlayerConstants.VEHICLESEAT_DRIVER;
		case 1:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_R;
		case 2:
			return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
		}

		return 0;
	}
	
	// ------------------------------------------------------------
	override bool CrewCanGetThrough( int posIdx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Vehicle_ExpansionZodiacBoat::CrewCanGetThrough");
		#endif

		return true;
	}

	// ------------------------------------------------------------
	override bool CanReachDoorsFromSeat( string pDoorsSelection, int pCurrentSeat )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Vehicle_ExpansionZodiacBoat::CanReachDoorsFromSeat");
		#endif

		return true;		
	}

	// ------------------------------------------------------------
	override bool IsVitalCarBattery()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Vehicle_ExpansionZodiacBoat::IsVitalCarBattery");
		#endif

		return false;
	}

	// ------------------------------------------------------------
	override bool IsVitalTruckBattery()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Vehicle_ExpansionZodiacBoat::IsVitalTruckBattery");
		#endif

		return true;
	}

	// ------------------------------------------------------------
	override bool IsVitalSparkPlug()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Vehicle_ExpansionZodiacBoat::IsVitalSparkPlug");
		#endif

		return false;
	}
	
	// ------------------------------------------------------------
	override bool IsVitalRadiator()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Vehicle_ExpansionZodiacBoat::IsVitalRadiator");
		#endif

		return false;
	}
	
	// ------------------------------------------------------------
	override bool IsVitalGlowPlug()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Vehicle_ExpansionZodiacBoat::IsVitalGlowPlug");
		#endif

		return true;
	}

	// ------------------------------------------------------------
	override bool IsVitalEngineBelt()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Vehicle_ExpansionZodiacBoat::IsVitalEngineBelt");
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
	
	// ------------------------------------------------------------
	override string GetActionCompNameFuel()
	{
		return "refill";
	}
}