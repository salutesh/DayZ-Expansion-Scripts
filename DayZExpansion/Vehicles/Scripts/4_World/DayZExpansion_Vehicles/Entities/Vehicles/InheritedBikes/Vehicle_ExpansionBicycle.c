/*
 * Vehicle_ExpansionBicycle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Vehicle_ExpansionBicycle extends ExpansionVehicleBikeBase
{
	// ------------------------------------------------------------
	//! Constructor
	// ------------------------------------------------------------

	void Vehicle_ExpansionBicycle()
	{
		//! Events layer mask
		SetEventMask(EntityEvent.CONTACT | EntityEvent.SIMULATE);
	}
	
	// ------------------------------------------------------------
	//! Override functions
	// ------------------------------------------------------------

	override void OnEngineStart(int index)
	{
		if ( IsMissionHost() )
		{
			GetInventory().CreateAttachmentEx( "ExpansionBicycleWheel", InventorySlots.GetSlotIdFromString( "bicyclewheel_2_1" ) );
			GetInventory().CreateAttachmentEx( "ExpansionBicycleWheel", InventorySlots.GetSlotIdFromString( "bicyclewheel_2_2" ) ); 
		}
	}
		
	// ------------------------------------------------------------
	override int GetAnimInstance()
	{
		return ExpansionVehicleAnimInstances.EXPANSION_MH6;
	}

	// ------------------------------------------------------------
	override int GetSeatAnimationType( int posIdx )
	{
		switch( posIdx )
		{
		case 0:
			return DayZPlayerConstants.VEHICLESEAT_DRIVER;
		}

		return 0;
	}
	
	// ------------------------------------------------------------
	override bool CrewCanGetThrough( int posIdx )
	{
		return true;
	}

	// ------------------------------------------------------------
	override bool CanReachDoorsFromSeat( string pDoorsSelection, int pCurrentSeat )
	{
		return true;		
	}
	
	// ------------------------------------------------------------
	override bool IsVitalCarBattery()
	{
		return false;
	}

	// ------------------------------------------------------------
	override bool IsVitalSparkPlug()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	override bool IsVitalRadiator()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	override bool IsVitalGlowPlug()
	{
		return false;
	}

	// ------------------------------------------------------------
	override bool IsVitalEngineBelt()
	{
		return false;
	}

	// ------------------------------------------------------------
	override bool IsVitalTruckBattery()
	{
		return false;
	}
}