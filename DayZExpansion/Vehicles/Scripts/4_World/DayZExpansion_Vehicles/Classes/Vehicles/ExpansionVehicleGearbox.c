/*
 * ExpansionVehicleGearbox.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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

	private int m_Gear;
	private int m_ReportingGear;
	
	private bool m_Clutch;
	private float m_ClutchAmt;
	private int m_ClutchState; // 0 - nothing, 1 - pressing, 2 - reached, 3 - releasing
	
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
		if (clutch && m_ClutchState == 0)
			m_ClutchState = 1;

		float clutchDt;
		
		switch(m_ClutchState)
		{
		case 1:
			clutchDt = pDt / m_TimeToCoupleClutch;
			if (m_ClutchAmt + clutchDt >= 1.0)
				m_ClutchState+=1;

			if (m_ClutchAmt < 0.5)
				m_Gear = newGear;
			break;
		case 2:
				m_ClutchState+=1;
			break;
		case 3:
			clutchDt = -pDt / m_TimeToUncoupleClutch;
			if (m_ClutchAmt + clutchDt <= 0)
				m_ClutchState = 0;
			break;
		}
		
		m_ClutchAmt = Math.Clamp( m_ClutchAmt + clutchDt, 0.0, 1.0 );

		#ifdef EXPANSION_DEBUG_UI_VEHICLE
		CF_Debugger_Block dbg_Vehicle = CF.Debugger.Get("Vehicle", m_Vehicle);
		
		dbg_Vehicle.Set("Clutch", clutch);
		dbg_Vehicle.Set("Time To Couple Clutch", m_TimeToCoupleClutch);
		dbg_Vehicle.Set("Time To Uncouple Clutch", m_TimeToUncoupleClutch);
		dbg_Vehicle.Set("Clutch Amt", m_ClutchAmt);
		dbg_Vehicle.Set("Reporting Gear", m_ReportingGear);
		dbg_Vehicle.Set("Gear", m_Gear);
		#endif
		
		if ( m_ClutchAmt > 0.0 )
			return Get( m_ReportingGear );
		
		if (m_ReportingGear != m_Gear)
			m_Vehicle.OnGearChanged(m_Gear, m_ReportingGear);
		
		m_ReportingGear = m_Gear;

		return Get(m_Gear);
	}

	float GetClutch()
	{
		return m_ClutchAmt;
	}

	int GetCurrentGear()
	{
		return m_ReportingGear;
	}
};