/*
 * ExpansionBikeScript.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

/**@class		ExpansionBikeScript
 * @brief		This class handle bicycle simulation
 **/
class ExpansionBikeScript extends ExpansionVehicleScript
{
	private ExpansionBikeController m_BikeController;

	private ref ExpansionVehicleGearbox m_Gearbox;
	private ref ExpansionVehicleEngine m_Engine;
	private ref ExpansionVehicleSteering m_Steering;
	
	private float m_Turn;
	private float m_TargetTurn;
	
	private float m_HandleSteer;

	private float m_Brake;
	private float m_Throttle;

	void ExpansionBikeScript()
	{
		Class.CastTo( m_BikeController, m_Controller );
		
		m_Gearbox = new ExpansionVehicleGearbox( this );
		m_Engine = new ExpansionVehicleEngineRWD( this );
		m_Steering = new ExpansionVehicleSteering( this );
	}
	
	void ~ExpansionBikeScript()
	{
		delete m_Gearbox;
		delete m_Engine;
		delete m_Steering;
	}

	override ExpansionVehicleController GetControllerInstance()
	{
		return new ExpansionBikeController( this );
	}

	ExpansionBikeController GetController()
	{
		return m_BikeController;
	}

	override int GetAnimInstance()
	{
		return ExpansionVehicleAnimInstances.EXPANSION_MH6;
	}

	override int GetSeatAnimationType( int posIdx )
	{
		switch( posIdx )
		{
		case 0:
			return DayZPlayerConstants.VEHICLESEAT_DRIVER;
		}

		return 0;
	}
	
	override bool CrewCanGetThrough( int posIdx )
	{
		return true;
	}

	override bool CanReachDoorsFromSeat( string pDoorsSelection, int pCurrentSeat )
	{
		return true;		
	}
	
	override bool IsVitalCarBattery()
	{
		return false;
	}

	override bool IsVitalSparkPlug()
	{
		return false;
	}
	
	override bool IsVitalRadiator()
	{
		return false;
	}
	
	override bool IsVitalGlowPlug()
	{
		return false;
	}

	override bool IsVitalEngineBelt()
	{
		return false;
	}

	override bool IsVitalTruckBattery()
	{
		return false;
	}

	override float GetCameraDistance()
	{
		return 3;
	}
	
	override float EngineGetRPMMax()
	{
		return m_Engine.GetRPMMax();
	}

	override float EngineGetRPMRedline()
	{
		return m_Engine.GetRPMRedline();
	}

	override float EngineGetRPM()
	{
		return m_Engine.GetRPM();
	}

	override int GetGearsCount()
	{
		return m_Gearbox.Count();
	}

	override float GetClutch()
	{
		return m_Gearbox.GetClutch();
	}

	override float GetSteering()
	{
		return m_HandleSteer;
	}

	override float GetThrottle()
	{
		return m_Throttle;
	}

	override float GetBraking()
	{
		return m_Brake;
	}
	
	protected override void OnHumanPilot( PlayerBase driver, float pDt )
	{
		m_TargetTurn = ( m_BikeController.GetTurnRight() - m_BikeController.GetTurnLeft() );
		
		float turnChange = m_Steering.CalculateChange( pDt, Math.AbsFloat( m_LinearVelocityMS[2] ), m_Turn, m_TargetTurn );
		
		m_Turn += turnChange;
		
		float absForwardSpeed = Math.AbsFloat( m_LinearVelocityMS[2] ) + 0.1;
		
		float maxSteer = 1.0 - Math.Clamp( ( absForwardSpeed ) / 30.0, 0.0, 0.95 );
		
		m_HandleSteer = Math.Clamp( m_Turn, -maxSteer, maxSteer );

		m_Throttle = Math.Clamp( m_Throttle + Math.Clamp( m_BikeController.GetForward() - m_Throttle, -1.0 * pDt, 1.0 * pDt ), 0.0, 1.0 );
		
		m_Brake = m_BikeController.GetBackward();
		
		ExpansionDebugUI( "Turn: " + m_Turn );
		ExpansionDebugUI( "Handle Steer: " + m_HandleSteer );
		ExpansionDebugUI( "Target Turn: " + m_TargetTurn );
	}

	protected override void OnPreSimulation( float pDt )
	{
		super.OnPreSimulation( pDt );

		if ( !m_IsPhysicsHost )
			return;
		
		int gear = m_Gearbox.GetCurrentGear();
		if ( m_Throttle == 0.0 && gear != CarGear.NEUTRAL && !m_HasDriver )
		{
			m_Brake = 1.0;
		}
		
		ApplyAxleSteering( 0, m_HandleSteer );
		ApplyAxleSteering( 1, 0.0 );
			
		ApplyAxleTorque( 0, 0.0 );
		ApplyAxleTorque( 1, 0.0 );
		
		if ( EngineIsOn() )
		{
			m_Engine.OnUpdate( pDt, m_Throttle, m_Gearbox.OnUpdate( m_ClutchState, m_BikeController.GetGear(), pDt ) );
			
			ApplyAxleBrake( 0, m_Brake );
			ApplyAxleBrake( 1, m_Brake );
		} else
		{
			ApplyAxleBrake( 0, 1.0 );
			ApplyAxleBrake( 1, 1.0 );
		}
	}

	protected override void OnAnimationUpdate( float pDt )
	{
		super.OnAnimationUpdate( pDt );

		SetAnimationPhase( "steeringwheel", GetSteering() );
	}

	protected override void OnSimulation( float pDt, out vector force, out vector torque )
	{
		super.OnSimulation( pDt, force, torque );

		float clampedForwardVel = Math.Clamp( m_LinearVelocityMS[2], -1, 1 );
		if ( clampedForwardVel > 0.9 )
		{
			m_AdjustCenterOfMass[2] = clampedForwardVel * 1.5;
		} else if ( clampedForwardVel < -0.9 )
		{
			m_AdjustCenterOfMass[2] = clampedForwardVel * 1.5;
		} else
		{
			m_AdjustCenterOfMass[2] = 0;
		}

		float absForwardSpeed = Math.AbsFloat( m_LinearVelocityMS[2] ) + 0.1;

		vector upDir = m_Transform.data[1];

		if ( upDir[1] > 0.5 )
		{
			vector terrainSurface = m_Wheels[0].GetSuspensionContactNormal() + m_Wheels[1].GetSuspensionContactNormal();
			terrainSurface = terrainSurface.Normalized().Multiply3( m_Transform.data );

			vector estDirUp = GetEstimatedOrientation( 0.1 )[1].Normalized();
			
			float leanAmount = Math.Clamp( m_Turn, -0.26, 0.26 );
			float leanOnX = leanAmount * absForwardSpeed * 0.001;
			leanOnX = Math.Clamp( leanOnX, -0.76, 0.76 );

			vector upDirWanted = Vector( -terrainSurface[0], 1.0, -terrainSurface[2] ).Normalized();
			
			vector upFromForward[4];
			Math3D.DirectionAndUpMatrix( GetDirection(), "0 1 0", upFromForward );
			upDirWanted = upDirWanted.InvMultiply3( upFromForward );
			upDirWanted[0] = leanOnX;
			upDirWanted.Normalize();
			upDirWanted = upDirWanted.Multiply3( upFromForward );
		
			vector applyPosition = Vector( 0.0, 1.0, 0.0 );

			vector stabilize = (upDirWanted - estDirUp);
			stabilize = stabilize.Normalized() * stabilize.LengthSq();
			const float maxStabCoef = 0.8;
			if ( stabilize.LengthSq() > maxStabCoef * maxStabCoef )
				stabilize = stabilize.Normalized() * maxStabCoef;
			
			stabilize = stabilize * 50.0 * m_BodyMass;

			stabilize = stabilize.InvMultiply3( m_Transform.data );
			stabilize[2] = 0;
			torque += ( applyPosition * stabilize ).Multiply3( m_Transform.data );
		}

		ExpansionDebugUI();
	}
}