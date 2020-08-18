/*
 * ExpansionCarScript.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * Copyright (c) 2005 Erwin Coumans http://continuousphysics.com/Bullet/
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionCarScript
 * @brief		This class handle bicycle simulation
 **/
class ExpansionCarScript extends ExpansionVehicleScript
{
	private ExpansionCarController m_CarController;

	private float m_RPM;

	private float m_Thrust;
	private float m_Turn;

	private float m_TargetThrust;
	private float m_TargetTurn;

	private float m_EngineForce;
	private float m_BrakeForce;

	// ------------------------------------------------------------
	void ExpansionCarScript()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "[" + this + "] ExpansionCarScript" );
		#endif

		m_Controller = new ExpansionCarController( this );
		Class.CastTo( m_CarController, m_Controller );
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

	// ------------------------------------------------------------
	override float GetCameraDistance()
	{
		return 4;
	}

	// ------------------------------------------------------------
	float Sign( float number )
	{
		if ( number < 0 )
			return -1;
		return 1;
	}

	// ------------------------------------------------------------
	protected override void OnHumanPilot( PlayerBase driver, float pDt )
	{
		m_TargetTurn = ( m_CarController.GetTurnRight() - m_CarController.GetTurnLeft() );
		m_EngineForce = ( m_CarController.GetForward() );
		m_BrakeForce = ( m_CarController.GetBackward() );

		if ( Math.AbsFloat( m_TargetTurn ) > Math.AbsFloat( m_Turn ) )
		{
			m_Turn += ( m_TargetTurn - m_Turn ) * pDt * 10.0;
		} else if ( Math.AbsFloat( m_TargetTurn ) < Math.AbsFloat( m_Turn ) )
		{
			m_Turn = 0;
		}

		ExpansionDebugUI();

		ExpansionDebugUI( "Turn: " + m_Turn );
		ExpansionDebugUI( "Engine Force: " + m_EngineForce );
		ExpansionDebugUI( "Brake Force: " + m_BrakeForce );
	}

	// ------------------------------------------------------------
	protected override void OnAIPilot( ExpansionAIBase driver, float pDt )
	{
	}

	// ------------------------------------------------------------
	protected override void OnPreSimulation( float pDt )
	{
		super.OnPreSimulation( pDt );

		if ( !m_IsPhysicsHost )
			return;

		float wheelBase = Math.AbsFloat( m_Wheels[2].GetInitialWheelPosition()[2] - m_Wheels[0].GetInitialWheelPosition()[2] );
		float turnRadius = 5;

		float leftBackWheel = Math.AbsFloat( m_Wheels[2].GetInitialWheelPosition()[0] ) * Sign( -m_Turn );
		float rightBackWheel = Math.AbsFloat( m_Wheels[3].GetInitialWheelPosition()[0] ) * Sign( m_Turn );

		float leftFrontWheelSteer = Math.Atan2( wheelBase / ( turnRadius + leftBackWheel ), 1 );
		float rightFrontWheelSteer = Math.Atan2( wheelBase / ( turnRadius + rightBackWheel ), 1 );

		SetWheelSteering( 0, Math.RAD2DEG * leftFrontWheelSteer * m_Turn );
		SetWheelSteering( 1, Math.RAD2DEG * rightFrontWheelSteer * m_Turn );

		SetWheelBrakeForce( 2, m_BrakeForce * 5 );
		SetWheelBrakeForce( 3, m_BrakeForce * 5 );

		SetWheelEngineForce( 2, m_EngineForce * 10 );
		SetWheelEngineForce( 3, m_EngineForce * 10 );
	}

	// ------------------------------------------------------------
	override float EngineGetRPM()
	{
		return m_RPM;
	}

	// ------------------------------------------------------------
	override float EngineGetRPMMax()
	{
		return 2;
	}

	// ------------------------------------------------------------
	override float EngineGetRPMRedline()
	{
		return 1;
	}
}