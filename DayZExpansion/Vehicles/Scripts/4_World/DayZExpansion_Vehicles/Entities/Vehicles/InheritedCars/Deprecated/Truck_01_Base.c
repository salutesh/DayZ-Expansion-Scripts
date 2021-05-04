/**
 * Truck_01_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/
modded class Truck_01_Base
{
	override string ExpansionGetWheelType(int slot_id)
	{
		return "Truck_01_Wheel";

		//if ( mycondition )
		//	return "Truck_01_Wheel";
		//else 
		//	return "Truck_01_WheelDouble";
	}

	override bool LeavingSeatDoesAttachment( int posIdx )
	{
		// @CAMINOonPC#6971 Never implemented this after being told to for the past 3 months
		return false;
	}

	override float GetTransportCameraDistance()
	{
		return 6.0;
	}

	override float GetCameraHeight()
	{
		return 3.2;
	}
}