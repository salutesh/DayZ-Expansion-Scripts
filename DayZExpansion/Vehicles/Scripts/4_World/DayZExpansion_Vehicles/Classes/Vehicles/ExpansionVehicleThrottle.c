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
	private ExpansionVehicleBase m_Vehicle;

	private float m_ReactionTime;
	private float m_DefaultThrust;
	private float m_GentleThrust;
	private float m_TurboCoef;
	private float m_GentleCoef;

	private float m_Throttle;
	private float m_ThrottleTarget;

	void ExpansionVehicleThrottle( ExpansionVehicleBase vehicle )
	{
		m_Vehicle = vehicle;

		string path;
		
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Throttle reactionTime";
		m_ReactionTime = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Throttle defaultThrust";
		m_DefaultThrust = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Throttle gentleThrust";
		m_GentleThrust = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Throttle turboCoef";
		m_TurboCoef = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Throttle gentleCoef";
		m_GentleCoef = GetGame().ConfigGetFloat( path );
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
		
		#ifdef EXPANSION_DEBUG_UI_VEHICLE
		CF_Debugger_Block dbg_Vehicle = CF.Debugger.Get("Vehicle", m_Vehicle);
		
		dbg_Vehicle.Set("Throttle", m_Throttle );
		#endif
	}
};