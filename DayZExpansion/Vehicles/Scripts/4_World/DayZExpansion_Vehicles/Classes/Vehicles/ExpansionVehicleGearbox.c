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
	private ExpansionVehicleBase m_Vehicle;

	private float m_TimeToUncoupleClutch;
	private float m_TimeToCoupleClutch;
	private float m_MaxClutchTorque;

	private float m_Reverse;
	private ref array< float > m_Ratios;

	private float m_ClutchAmt;

	private int m_Gear;
	private bool m_Clutch;
	
	void ExpansionVehicleGearbox( ExpansionVehicleBase vehicle )
	{
		m_Vehicle = vehicle;

		string path;
		
		m_Ratios = new array< float >();
		path = "CfgVehicles " + vehicle.GetType() + " SimulationModule Gearbox ratios";
		GetGame().ConfigGetFloatArray( path, m_Ratios );
		
		path = "CfgVehicles " + vehicle.GetType() + " SimulationModule Gearbox reverse";
		m_Reverse = GetGame().ConfigGetFloat( path );

		path = "CfgVehicles " + vehicle.GetType() + " SimulationModule Gearbox timeToUncoupleClutch";
		m_TimeToUncoupleClutch = GetGame().ConfigGetFloat( path );

		path = "CfgVehicles " + vehicle.GetType() + " SimulationModule Gearbox timeToCoupleClutch";
		m_TimeToCoupleClutch = GetGame().ConfigGetFloat( path );

		path = "CfgVehicles " + vehicle.GetType() + " SimulationModule Gearbox maxClutchTorque";
		m_MaxClutchTorque = GetGame().ConfigGetFloat( path );
	}
	
	void ~ExpansionVehicleGearbox()
	{
		delete m_Ratios;
	}

	int Count()
	{
		return m_Ratios.Count() + 1;
	}
	
	float Get( int gear )
	{
		if ( gear == CarGear.REVERSE )
			return -m_Reverse;
		if ( gear == CarGear.NEUTRAL )
			return 0;
		
		return m_Ratios[gear - 2];
	}

	float OnUpdate( bool clutch, int newGear, float pDt )
	{
		m_ClutchAmt = Math.Clamp( Math.Clamp( clutch - m_ClutchAmt, -pDt / m_TimeToUncoupleClutch, pDt / m_TimeToCoupleClutch ), 0.0, 1.0 );

		if (m_Gear != newGear)
			m_Vehicle.OnGearChanged(newGear, m_Gear);

		m_Gear = newGear;

		//if ( m_ClutchAmt >= 0.75 )
		//{
		//	m_Gear = newGear;
		//	return 1.0;
		//}

		if ( m_ClutchAmt > 0.0 )
			return 0.0;

		return Get( m_Gear );
	}

	float GetClutch()
	{
		return m_ClutchAmt;
	}

	int GetCurrentGear()
	{
		return m_Gear;
	}
};