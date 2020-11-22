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

	protected ExpansionVehicleBase m_Vehicle;
	protected ref array< ref ExpansionVehicleWheel > m_Wheels;
	
	private float m_WheelRadius;

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

	void ExpansionVehicleAxle( ExpansionVehicleBase pVehicle, string pName )
	{
		m_Name = pName;

		m_Vehicle = pVehicle;
		m_Wheels = new array< ref ExpansionVehicleWheel >;

		string path = "";

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " maxSteeringAngle";
		m_MaxSteeringAngle = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " finalRatio";
		m_FinalRatio = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " brakeBias";
		m_BrakeBias = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " brakeForce";
		m_BrakeForce = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " wheelHubMass";
		m_WheelHubMass = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " wheelHubRadius";
		m_WheelHubRadius = GetGame().ConfigGetFloat( path );

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " Suspension swayBar";
		m_SwayBar = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " Suspension stiffness";
		m_Stiffness = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " Suspension compression";
		m_Compression = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " Suspension damping";
		m_Damping = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " Suspension travelMaxUp";
		m_TravelMaxUp = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " Suspension travelMaxDown";
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

	ExpansionVehicleBase GetVehicle()
	{
		return m_Vehicle;
	}

	ref ExpansionVehicleWheel AddWheel( string pName, out array< ExpansionVehicleWheel > pWheels )
	{
		ref ExpansionVehicleWheel wheel = new ExpansionVehicleWheel( m_Vehicle, this, pName );

		m_Wheels.Insert( wheel );
		pWheels.Insert( wheel );

		return wheel;
	}

	void SetupSimulation( float pDt, out int pNumWheelsGrounded )
	{
		for ( int i = 0; i < m_Wheels.Count(); i++ )
			m_Wheels[i].SetupSimulation( pDt, pNumWheelsGrounded );
	}

	void Simulate( float pDt, int pNumWheelsGrounded, out vector pImpulse, out vector pImpulseTorque )
	{
		for ( int i = 0; i < m_Wheels.Count(); i++ )
			m_Wheels[i].Simulate( pDt, pNumWheelsGrounded, pImpulse, pImpulseTorque );
	}

	void Animate( float pDt, bool pPhysics )
	{
		for ( int i = 0; i < m_Wheels.Count(); i++ )
			m_Wheels[i].Animate( pDt, pPhysics );
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

	float GetAngularVelocity()
	{
		float highest = 0;

		for ( int i = 0; i < m_Wheels.Count(); ++i )
			if (m_Wheels[i].GetAngularVelocity() > highest)
				highest = m_Wheels[i].GetAngularVelocity();
		
		return highest;
	}
	
	float GetWheelRadius()
	{
		return m_WheelRadius;
	}
	
	void UpdateWheelRadius()
	{
		for ( int i = 0; i < m_Wheels.Count(); ++i )
			if ( m_Wheels[i].GetItem() )
				m_WheelRadius += m_Wheels[i].GetItem().m_Radius;
		
		m_WheelRadius = m_WheelRadius / m_Wheels.Count();
	}
	
	float GetRPM()
	{
		float highest = 0;

		for ( int i = 0; i < m_Wheels.Count(); ++i )
			if (m_Wheels[i].GetRPM() > highest)
				highest = m_Wheels[i].GetRPM();
		
		return highest;
	}

	void SetSteering( float pSteering )
	{
		_SetSteering( pSteering * GetMaxSteeringAngle() );
	}

	protected void _SetSteering( float pSteering )
	{

	}

	void ApplyTorque( float pTorque )
	{
		_ApplyTorque( pTorque * m_FinalRatio );
	}

	protected void _ApplyTorque( float pTorque )
	{

	}

	void ApplyBrake( float pBrake )
	{
		_ApplyBrake( m_BrakeForce * m_BrakeBias * pBrake );
	}

	protected void _ApplyBrake( float pTorque )
	{

	}
}

class ExpansionVehicleOneWheelAxle : ExpansionVehicleAxle
{
	private ExpansionVehicleWheel m_Center;

	void ExpansionVehicleOneWheelAxle( ExpansionVehicleBase pVehicle, string pName )
	{
	}

	override ref ExpansionVehicleWheel AddWheel( string pName, out array< ExpansionVehicleWheel > pWheels )
	{
		m_Center = super.AddWheel( pName, pWheels );

		return m_Center;
	}

	override float GetAngularVelocity()
	{
		return m_Center.GetAngularVelocity();
	}

	override float GetRPM()
	{
		return m_Center.GetRPM();
	}

	override void SetupSimulation( float pDt, out int pNumWheelsGrounded )
	{
		m_Center.SetupSimulation( pDt, pNumWheelsGrounded );
	}

	override void Simulate( float pDt, int pNumWheelsGrounded, out vector pImpulse, out vector pImpulseTorque )
	{
		m_Center.Simulate( pDt, pNumWheelsGrounded, pImpulse, pImpulseTorque );
	}

	override void Animate( float pDt, bool pPhysics )
	{
		m_Center.Animate( pDt, pPhysics );
	}

	protected override void _SetSteering( float pSteering )
	{
		m_Center.SetSteering( pSteering );
	}

	protected override void _ApplyTorque( float pTorque )
	{
		m_Center.ApplyTorque( pTorque );
	}

	protected override void _ApplyBrake( float pTorque )
	{
		m_Center.ApplyBrake( pTorque );
	}
}

class ExpansionVehicleTwoWheelAxle : ExpansionVehicleAxle
{
	private float m_AntiRollForce;

	private ExpansionVehicleWheel m_Left;
	private ExpansionVehicleWheel m_Right;

	void ExpansionVehicleTwoWheelAxle( ExpansionVehicleBase pVehicle, string pName )
	{
	}

	override ref ExpansionVehicleWheel AddWheel( string pName, out array< ExpansionVehicleWheel > pWheels )
	{
		if ( pName == "Left" )
		{
			m_Left = super.AddWheel( pName, pWheels );
			return m_Left;
		} else if ( pName == "Right" )
		{
			m_Right = super.AddWheel( pName, pWheels );
			return m_Right;
		}

		return super.AddWheel( pName, pWheels );
	}

	override float GetAngularVelocity()
	{
		return Math.Max( m_Left.GetAngularVelocity(), m_Right.GetAngularVelocity() );
	}

	override float GetRPM()
	{
		return Math.Max( m_Left.GetRPM(), m_Right.GetRPM() );
	}

	override void SetupSimulation( float pDt, out int pNumWheelsGrounded )
	{
		m_Left.SetupSimulation( pDt, pNumWheelsGrounded );
		m_Right.SetupSimulation( pDt, pNumWheelsGrounded );

		float t1 = m_Left.GetSuspensionFraction();
		float t2 = m_Right.GetSuspensionFraction();
		m_AntiRollForce = ( t1 - t2 ) * GetSwayBar();
	}

	override void Simulate( float pDt, int pNumWheelsGrounded, out vector pImpulse, out vector pImpulseTorque )
	{
		m_Left.Simulate( pDt, pNumWheelsGrounded, pImpulse, pImpulseTorque );
		m_Right.Simulate( pDt, pNumWheelsGrounded, pImpulse, pImpulseTorque );
		
		float coef = 0; //m_AntiRollForce * dBodyGetMass( m_Vehicle ) * pDt;

		vector w1 = m_Left.GetSuspensionContactNormal() * coef;
		vector w2 = m_Right.GetSuspensionContactNormal() * coef;
		vector p1 = m_Left.GetSuspensionContactPosition();
		vector p2 = m_Right.GetSuspensionContactPosition();

		vector impulse;
		vector impulseTorque;

		impulse += w1;
		impulseTorque += p1 * w1;

		impulse += w2;
		impulseTorque += p2 * w2;

		pImpulse += impulse.Multiply3( m_Vehicle.m_Transform.GetBasis().data );
		pImpulseTorque += impulseTorque.Multiply3( m_Vehicle.m_Transform.GetBasis().data );
	}

	override void Animate( float pDt, bool pPhysics )
	{
		m_Left.Animate( pDt, pPhysics );
		m_Right.Animate( pDt, pPhysics );
	}

	protected override void _SetSteering( float pSteering )
	{
		m_Left.SetSteering( pSteering );
		m_Right.SetSteering( pSteering );
	}

	protected override void _ApplyTorque( float pTorque )
	{
		m_Left.ApplyTorque( pTorque * 0.5 );
		m_Right.ApplyTorque( pTorque * 0.5 );
	}

	protected override void _ApplyBrake( float pTorque )
	{
		m_Left.ApplyBrake( pTorque * 0.5 );
		m_Right.ApplyBrake( pTorque * 0.5 );
	}
}