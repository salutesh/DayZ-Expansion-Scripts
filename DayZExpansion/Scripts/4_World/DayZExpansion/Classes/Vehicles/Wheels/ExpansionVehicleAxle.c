/**
 * ExpansionVehicleAxle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleAxle
{
	private string m_Name;

	protected ExpansionVehicleScript m_Vehicle;
	protected ref array< ref ExpansionVehicleWheel > m_Wheels;

	private float m_MaxSteeringAngle;
	private float m_BrakeBias;
	private float m_BrakeForce;
	private float m_FinalRatio;
	private float m_WheelHubMass;
	private float m_WheelHubRadius;

	private float m_SwayBar;
	private float m_Stiffness;
	private float m_Compression;
	private float m_Damping;
	private float m_TravelMaxUp;
	private float m_TravelMaxDown;

	private float m_Steering;

	void ExpansionVehicleAxle( ExpansionVehicleScript vehicle, string name )
	{
		m_Name = name;

		m_Vehicle = vehicle;
		m_Wheels = new array< ref ExpansionVehicleWheel >;

		string path = "";

		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + name + " maxSteeringAngle";
		m_MaxSteeringAngle = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + name + " finalRatio";
		m_FinalRatio = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + name + " brakeBias";
		m_BrakeForce = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + name + " brakeForce";
		m_BrakeBias = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + name + " wheelHubMass";
		m_WheelHubMass = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + name + " wheelHubRadius";
		m_WheelHubRadius = GetGame().ConfigGetFloat( path );

		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + name + " Suspension swayBar";
		m_SwayBar = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + name + " Suspension stiffness";
		m_Stiffness = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + name + " Suspension compression";
		m_Compression = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + name + " Suspension damping";
		m_Damping = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + name + " Suspension travelMaxUp";
		m_TravelMaxUp = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Axles " + name + " Suspension travelMaxDown";
		m_TravelMaxDown = GetGame().ConfigGetFloat( path );
	}

	void ~ExpansionVehicleAxle()
	{
		m_Wheels.Clear();

		delete m_Wheels;
	}

	string GetName()
	{
		return m_Name;
	}

	ExpansionVehicleScript GetVehicle()
	{
		return m_Vehicle;
	}

	ref ExpansionVehicleWheel AddWheel( string wheelName, out array< ExpansionVehicleWheel > allWheels )
	{
		ref ExpansionVehicleWheel wheel = new ExpansionVehicleWheel( m_Vehicle, this, wheelName );

		m_Wheels.Insert( wheel );
		allWheels.Insert( wheel );

		return wheel;
	}

	void SetupSimulation( float pDt, out int numWheelsGrounded )
	{
		for ( int i = 0; i < m_Wheels.Count(); i++ )
		{
			m_Wheels[i].SetupSimulation( pDt, numWheelsGrounded );
		}
	}

	void Simulate( float pDt, int numWheelsGrounded, out vector pImpulse, out vector pImpulseTorque )
	{
		for ( int i = 0; i < m_Wheels.Count(); i++ )
		{
			m_Wheels[i].Simulate( pDt, numWheelsGrounded, pImpulse, pImpulseTorque );
		}
	}

	float GetSteering()
	{
		return m_Steering;
	}

	void SetSteering( float steering )
	{
		m_Steering = steering;
	}

	float GetMaxSteeringAngle()
	{
		return m_MaxSteeringAngle;
	}

	float GetFinalRatio()
	{
		return m_FinalRatio;
	}

	float GetBrakeBias()
	{
		return m_BrakeBias;
	}

	float GetBrakeForce()
	{
		return m_BrakeForce;
	}

	float GetWheelHubMass()
	{
		return m_WheelHubMass;
	}

	float GetWheelHubRadius()
	{
		return m_WheelHubRadius;
	}

	float GetSwayBar()
	{
		return m_SwayBar;
	}

	float GetStiffness()
	{
		return m_Stiffness;
	}

	float GetCompression()
	{
		return m_Compression;
	}

	float GetDamping()
	{
		return m_Damping;
	}

	float GetTravelMaxUp()
	{
		return m_TravelMaxUp;
	}

	float GetTravelMaxDown()
	{
		return m_TravelMaxDown;
	}

	float GetTravelMax()
	{
		return m_TravelMaxUp + m_TravelMaxDown;
	}

	void ApplyTorque( float torque )
	{
		if ( m_FinalRatio == 0 )
			return;
		
		_ApplyTorque( torque / m_FinalRatio );
	}

	protected void _ApplyTorque( float torque )
	{

	}

	void ApplyBrake( float amt )
	{
		_ApplyBrake(m_BrakeForce * m_BrakeBias * amt );
	}

	protected void _ApplyBrake( float force )
	{

	}
}

class ExpansionVehicleOneWheelAxle : ExpansionVehicleAxle
{
	void ExpansionVehicleOneWheelAxle( ExpansionVehicleScript vehicle, string name )
	{
	}

	override void SetupSimulation( float pDt, out int numWheelsGrounded )
	{
		m_Wheels[0].SetupSimulation( pDt, numWheelsGrounded );
	}

	override void Simulate( float pDt, int numWheelsGrounded, out vector pImpulse, out vector pImpulseTorque )
	{
		m_Wheels[0].Simulate( pDt, numWheelsGrounded, pImpulse, pImpulseTorque );
	}

	protected override void _ApplyTorque( float torque )
	{
		m_Wheels[0].m_EngineForce = torque;
	}

	protected override void _ApplyBrake( float force )
	{
		m_Wheels[0].m_BrakeForce = force;
	}
}

class ExpansionVehicleTwoWheelAxle : ExpansionVehicleAxle
{
	private float m_AntiRollForce;

	void ExpansionVehicleTwoWheelAxle( ExpansionVehicleScript vehicle, string name )
	{
	}

	override void SetupSimulation( float pDt, out int numWheelsGrounded )
	{
		m_Wheels[0].SetupSimulation( pDt, numWheelsGrounded );
		m_Wheels[1].SetupSimulation( pDt, numWheelsGrounded );

		float t1 = m_Wheels[0].GetSuspensionFraction();
		float t2 = m_Wheels[1].GetSuspensionFraction();
		m_AntiRollForce = ( t1 - t2 ) * GetSwayBar();
	}

	override void Simulate( float pDt, int numWheelsGrounded, out vector pImpulse, out vector pImpulseTorque )
	{
		m_Wheels[0].Simulate( pDt, numWheelsGrounded, pImpulse, pImpulseTorque );
		m_Wheels[1].Simulate( pDt, numWheelsGrounded, pImpulse, pImpulseTorque );

		vector w1 = m_Wheels[0].GetSuspensionContactNormal() * m_AntiRollForce * pDt;
		vector w2 = m_Wheels[0].GetSuspensionContactNormal() * m_AntiRollForce * pDt;
		vector p1 = m_Wheels[0].GetSuspensionContactPosition();
		vector p2 = m_Wheels[1].GetSuspensionContactPosition();

		vector impulse;
		vector impulseTorque;

		impulse += w1;
		impulseTorque += p1 * w1;

		impulse += w2;
		impulseTorque += p2 * w2;

		pImpulse += impulse.Multiply3( m_Vehicle.m_Transform.GetBasis().data );
		pImpulseTorque += impulseTorque.Multiply3( m_Vehicle.m_Transform.GetBasis().data );
	}

	protected override void _ApplyTorque( float torque )
	{
		m_Wheels[0].m_EngineForce = torque;
		m_Wheels[1].m_EngineForce = torque;
	}

	protected override void _ApplyBrake( float force )
	{
		m_Wheels[0].m_BrakeForce = force;
		m_Wheels[1].m_BrakeForce = force;
	}
}