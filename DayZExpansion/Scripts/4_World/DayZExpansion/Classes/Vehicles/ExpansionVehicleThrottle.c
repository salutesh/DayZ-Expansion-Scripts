/**
 * ExpansionVehicleThrottle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleThrottle
{
	private ExpansionVehicleScript m_Vehicle;

	private float m_ReactionTime;
	private float m_DefaultThrust;
	private float m_GentleThrust;
	private float m_TurboCoef;
	private float m_GentleCoef;

	private float m_Throttle;
	private float m_ThrottleTarget;

	void ExpansionVehicleThrottle( ExpansionVehicleScript vehicle )
	{
		m_Vehicle = vehicle;

		string path;
		
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Throttle reactionTime";
		m_ReactionTime = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Throttle defaultThrust";
		m_DefaultThrust = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Throttle gentleThrust";
		m_GentleThrust = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Throttle turboCoef";
		m_TurboCoef = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Throttle gentleCoef";
		m_GentleCoef = GetGame().ConfigGetFloat( path );
	}

	void ExpansionDebugUI( string message = "" )
	{
		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_ENGINE, message );
	}

	float Get()
	{
		return m_Throttle;
	}

	void Update( float pDt, float pIn, float pGentle, float pTurbo )
	{
		m_Throttle = m_DefaultThrust * pIn;
		m_Throttle = Math.Lerp( m_Throttle, m_GentleThrust, pGentle * pIn );
		m_Throttle = Math.Lerp( m_Throttle, 1.0, pTurbo * pIn );
		
		ExpansionDebugUI( "m_Throttle: " + m_Throttle );
	}
};