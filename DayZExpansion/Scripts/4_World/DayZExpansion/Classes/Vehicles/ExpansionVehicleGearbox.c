/*
 * ExpansionVehicleGearbox.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleGearbox
{
	private float _reverse;
	private ref array< float > _ratios;
	
	void ExpansionVehicleGearbox(ExpansionVehicleScript vehicle)
	{
		string path;
		
		_ratios = new array< float >();
		path = "CfgVehicles " + vehicle.GetType() + " VehicleSimulation Gearbox ratios";
		GetGame().ConfigGetFloatArray( path, _ratios );
		
		path = "CfgVehicles " + vehicle.GetType() + " VehicleSimulation Gearbox reverse";
		_reverse = GetGame().ConfigGetFloat( path );
	}
	
	void ~ExpansionVehicleGearbox()
	{
		delete _ratios;
	}

	int Count()
	{
		return _ratios.Count() + 2;
	}
	
	float Get( int gear )
	{
		if ( gear == CarGear.REVERSE )
			return -_reverse;
		if ( gear == CarGear.NEUTRAL )
			return 0;
		
		return _ratios[gear - 2];
	}
};