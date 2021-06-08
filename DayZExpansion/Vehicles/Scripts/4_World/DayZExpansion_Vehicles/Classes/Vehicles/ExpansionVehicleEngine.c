/*
 * ExpansionVehicleEngine.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionVehicleEngineType
{
	NONE,
	RWD,
	FWD,
	AWD
};

class ExpansionVehicleEngine
{
	private ExpansionVehicleBase m_Vehicle;
	
	private float m_Inertia;

	private float m_TorqueMax;
	private float m_TorqueRPM;

	private float m_Steepness;
	
	private float m_PowerMax;
	private float m_PowerRPM;
	
	private float m_RPMIdle;
	private float m_RPMMin;
	private float m_RPMClutch;
	private float m_RPMRedline;
	private float m_RPMMax;
	
	private float m_RPM;
	private float m_Torque;

	private float m_NewTorque;
	private float m_PreviousTorque;
	
	void ExpansionVehicleEngine( ExpansionVehicleBase vehicle )
	{
		m_Vehicle = vehicle;

		string path;
		
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Engine inertia";
		m_Inertia = 0.9;
		if ( GetGame().ConfigIsExisting( path ) )
			m_Inertia = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Engine torqueMax";
		m_TorqueMax = 0;
		if ( GetGame().ConfigIsExisting( path ) )
			m_TorqueMax = GetGame().ConfigGetFloat( path );

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Engine torqueRpm";
		m_TorqueRPM = 0;
		if ( GetGame().ConfigIsExisting( path ) )
			m_TorqueRPM = GetGame().ConfigGetFloat( path );

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Engine steepness";
		m_Steepness = 1;
		if ( GetGame().ConfigIsExisting( path ) )
			m_Steepness = GetGame().ConfigGetFloat( path );

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Engine powerMax";
		m_PowerMax = 0;
		if ( GetGame().ConfigIsExisting( path ) )
			m_PowerMax = GetGame().ConfigGetFloat( path );

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Engine powerRpm";
		m_PowerRPM = 0;
		if ( GetGame().ConfigIsExisting( path ) )
			m_PowerRPM = GetGame().ConfigGetFloat( path );

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Engine rpmIdle";
		m_RPMIdle = 0;
		if ( GetGame().ConfigIsExisting( path ) )
			m_RPMIdle = GetGame().ConfigGetFloat( path );

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Engine rpmMin";
		m_RPMMin = 0;
		if ( GetGame().ConfigIsExisting( path ) )
			m_RPMMin = GetGame().ConfigGetFloat( path );

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Engine rpmClutch";
		m_RPMClutch = 0;
		if ( GetGame().ConfigIsExisting( path ) )
			m_RPMClutch = GetGame().ConfigGetFloat( path );

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Engine rpmRedline";
		m_RPMRedline = 0;
		if ( GetGame().ConfigIsExisting( path ) )
			m_RPMRedline = GetGame().ConfigGetFloat( path );

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Engine rpmMax";
		m_RPMMax = 0;
		if ( GetGame().ConfigIsExisting( path ) )
			m_RPMMax = GetGame().ConfigGetFloat( path );
	}
	
	void ~ExpansionVehicleEngine()
	{
	}

	void OnUpdate( float pDt, float pThrottle, float pGR )
	{
		#ifdef EXPANSION_DEBUG_UI_VEHICLE
		CF_Debugger_Block dbg_Vehicle = CF.Debugger.Get("Vehicle", m_Vehicle);
		#endif
		
		#ifdef EXPANSION_DEBUG_UI_VEHICLE
		dbg_Vehicle.Set("Type", ClassName() );
		#endif
		
		float axleDiff = GetDifferential();
		float wheelRPM = Math.AbsFloat( GetWheelRPM() );
		
		#ifdef EXPANSION_DEBUG_UI_VEHICLE
		dbg_Vehicle.Set("Wheel RPM", wheelRPM );
		dbg_Vehicle.Set("Axle Differential", axleDiff );
		dbg_Vehicle.Set("Gear Ratio", pGR );
		#endif

		m_RPM = ToRPM(Math.AbsFloat(wheelRPM * pGR / axleDiff));
		m_RPM = Math.Clamp( m_RPM, m_RPMIdle, m_RPMMax );
		
		#ifdef EXPANSION_DEBUG_UI_VEHICLE
		dbg_Vehicle.Set("RPM", m_RPM );
		#endif
		
		m_PreviousTorque = m_NewTorque;
		m_NewTorque = LookupTorque(m_RPM) * pThrottle;
		
		#ifdef EXPANSION_DEBUG_UI_VEHICLE
		dbg_Vehicle.Set("Torque", torque );
		#endif
		
		m_Torque += (m_NewTorque - m_PreviousTorque) * pDt / m_Inertia;
		m_Torque = Math.Clamp(m_Torque, 0, m_TorqueMax);
		
		#ifdef EXPANSION_DEBUG_UI_VEHICLE
		dbg_Vehicle.Set("Torque", m_Torque );
		#endif
		
		float torque = 0.0;
		
		if (pGR != 0) torque = m_Torque / pGR;

		ApplyAxleTorque(torque);
	}

	protected void ApplyAxleTorque( float torque )
	{
		
	}

	float Lerp(float a, float b, float t)
	{
		return (a * (1.0 - t)) + (b * t);
	}

	float SmoothStep(float t)
	{
		return t * t * (3.0 - (2.0 * t));
	}

	float FromRPM(float rpm)
	{
		return rpm * Math.PI / 30.0;
	}

	float ToRPM(float rot)
	{
		return rot * 30.0 / Math.PI;
	}

	float LookupTorque( float rpm )
	{
		#ifdef EXPANSION_DEBUG_UI_VEHICLE
		CF_Debugger_Block dbg_Vehicle = CF.Debugger.Get("Vehicle", m_Vehicle);
		#endif
		
		#ifdef EXPANSION_DEBUG_UI_VEHICLE
		dbg_Vehicle.Set("Power  Max", m_PowerMax );
		dbg_Vehicle.Set("Torque Max", m_TorqueMax );
		dbg_Vehicle.Set("Max    RPM", m_RPMMax );
		dbg_Vehicle.Set("Power  RPM", m_PowerRPM );
		dbg_Vehicle.Set("Torque RPM", m_TorqueRPM );
		dbg_Vehicle.Set("RPM    Idle", m_RPMIdle );
		#endif

		if (rpm >= m_PowerRPM && rpm <= m_RPMMax)
		{
			#ifdef EXPANSION_DEBUG_UI_VEHICLE
			dbg_Vehicle.Set("Performing", "STAGE 3" );
			#endif

			return SmoothStep(1.0 - ((rpm - m_PowerRPM) / (m_RPMMax - m_PowerRPM))) * (1000.0 * m_PowerMax) / FromRPM(m_PowerRPM);
		} else if (rpm >= m_TorqueRPM && rpm < m_PowerRPM)
		{
			#ifdef EXPANSION_DEBUG_UI_VEHICLE
			dbg_Vehicle.Set("Performing", "STAGE 2" );
			#endif

			return Lerp(m_TorqueMax, 1000.0 * m_PowerMax / FromRPM(m_PowerRPM), (rpm - m_TorqueRPM) / (m_PowerRPM - m_TorqueRPM));
		} else if (rpm >= m_RPMIdle && rpm < m_TorqueRPM)
		{
			#ifdef EXPANSION_DEBUG_UI_VEHICLE
			dbg_Vehicle.Set("Performing", "STAGE 1" );
			#endif

			float a = m_Steepness * FromRPM(rpm) / FromRPM(m_TorqueRPM);
			return m_TorqueMax * a * ((1.0 + (1.0 / m_Steepness)) / (1.0 + a));
		}

		#ifdef EXPANSION_DEBUG_UI_VEHICLE
		dbg_Vehicle.Set("Performing", "STAGE 0" );
		#endif

		return 0;
	}

	float GetRPM()
	{
		return m_RPM;
	}

	float GetRPMRedline()
	{
		return m_RPMRedline;
	}

	float GetRPMMax()
	{
		return m_RPMMax;
	}
	
	float GetTorque()
	{
		return m_Torque;
	}

	protected float GetDifferential()
	{
		return 0;
	}

	protected float GetWheelAngularVelocity()
	{
		return 0;
	}

	protected float GetWheelRPM()
	{
		return 0;
	}
};

class ExpansionVehicleEngineRWD : ExpansionVehicleEngine
{
	private ExpansionVehicleAxle m_Axle;

	void ExpansionVehicleEngineRWD( ExpansionVehicleBase vehicle )
	{
		m_Axle = vehicle.GetAxle( vehicle.GetAxleCount() - 1 );
	}

	protected override void ApplyAxleTorque( float torque )
	{
		m_Axle.ApplyTorque( torque );
	}

	protected override float GetDifferential()
	{
		return m_Axle.GetFinalRatio();
	}

	protected override float GetWheelAngularVelocity()
	{
		return m_Axle.GetAngularVelocity();
	}

	protected override float GetWheelRPM()
	{
		return m_Axle.GetRPM();
	}
};

class ExpansionVehicleEngineFWD : ExpansionVehicleEngine
{
	private ExpansionVehicleAxle m_Axle;

	void ExpansionVehicleEngineFWD( ExpansionVehicleBase vehicle )
	{
		m_Axle = vehicle.GetAxle( 0 );
	}

	protected override void ApplyAxleTorque( float torque )
	{
		m_Axle.ApplyTorque( torque );
	}

	protected override float GetDifferential()
	{
		return m_Axle.GetFinalRatio();
	}

	protected override float GetWheelAngularVelocity()
	{
		return m_Axle.GetAngularVelocity();
	}

	protected override float GetWheelRPM()
	{
		return m_Axle.GetRPM();
	}
};

class ExpansionVehicleEngineAWD : ExpansionVehicleEngine
{
	private ExpansionVehicleAxle m_AxleF;
	private ExpansionVehicleAxle m_AxleB;

	void ExpansionVehicleEngineAWD( ExpansionVehicleBase vehicle )
	{
		m_AxleF = vehicle.GetAxle( 0 );
		m_AxleB = vehicle.GetAxle( vehicle.GetAxleCount() - 1 );
	}

	protected override void ApplyAxleTorque( float torque )
	{
		m_AxleF.ApplyTorque( torque );
		m_AxleB.ApplyTorque( torque );
	}

	protected override float GetDifferential()
	{
		return ( m_AxleF.GetFinalRatio() + m_AxleB.GetFinalRatio() ) * 0.5;
	}

	protected override float GetWheelAngularVelocity()
	{
		return Math.Max( m_AxleF.GetAngularVelocity(), m_AxleB.GetAngularVelocity() );
	}

	protected override float GetWheelRPM()
	{
		return Math.Max( m_AxleF.GetRPM(), m_AxleB.GetRPM() );
	}
};