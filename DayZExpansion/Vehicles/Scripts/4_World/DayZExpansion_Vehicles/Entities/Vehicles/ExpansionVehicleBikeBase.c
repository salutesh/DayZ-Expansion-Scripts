/*
 * ExpansionVehicleBikeBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

/**@class		ExpansionVehicleBikeBase
 * @brief		This class handle bicycle simulation
 **/
class ExpansionVehicleBikeBase extends ExpansionVehicleBase
{
	private ExpansionBikeController m_BikeController;

	private ref ExpansionVehicleGearbox m_Gearbox;
	private ref ExpansionVehicleEngine m_Engine;
	private ref ExpansionVehicleSteering m_Steering;
	private ref ExpansionVehicleThrottle m_Throttle;
	
	private float m_AirControlForwardCoef;
	private float m_AirControlRightCoef;

	private float m_RPMVal;
	private float m_SteeringVal;
	private float m_BrakeVal;

	private bool m_AirControl;
	private float m_AirControlForward;
	private float m_AirControlRight;

	private bool m_GroundControl;
	private float m_GroundStabilizer;
	private float m_FrontSuspFraction;
	private float m_BackSuspFraction;

	private float m_RPMSynch;
	private float m_SteeringSynch;

	void ExpansionVehicleBikeBase()
	{
		Class.CastTo( m_BikeController, m_Controller );

		RegisterNetSyncVariableFloat( "m_RPMSynch", 0, 0, 4 );
		RegisterNetSyncVariableFloat( "m_SteeringSynch", 0, 0, 4 );

		m_AirControlForwardCoef = 1.0;
		m_AirControlRightCoef = 1.0;
		
		m_Gearbox = new ExpansionVehicleGearbox( this );
		m_Engine = new ExpansionVehicleEngineRWD( this );
		m_Steering = new ExpansionVehicleSteering( this );
		m_Throttle = new ExpansionVehicleThrottle( this );
	}
	
	void ~ExpansionVehicleBikeBase()
	{
		delete m_Gearbox;
		delete m_Engine;
		delete m_Steering;
		delete m_Throttle;
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
		return m_RPMVal;
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
		return m_SteeringVal;
	}

	override float GetThrottle()
	{
		return m_Throttle.Get();
	}

	override float GetBrake()
	{
		return m_BrakeVal;
	}
	
	protected override void OnHumanPilot( PlayerBase driver, float pDt )
	{
		m_SteeringVal += m_Steering.CalculateChange( pDt, Math.AbsFloat( m_LinearVelocityMS[2] ), m_SteeringVal, m_BikeController.GetTurnRight() - m_BikeController.GetTurnLeft() );

		m_Throttle.Update( pDt, m_BikeController.GetForward(), m_BikeController.GetGentle(), m_BikeController.GetTurbo() );

		m_BrakeVal = m_BikeController.GetBackward();

		m_AirControlForward = m_BikeController.GetForward() - m_BikeController.GetBackward();
		m_AirControlRight = m_BikeController.GetTurnRight() - m_BikeController.GetTurnLeft();

		ExpansionDebugUI( "Steering: " + m_SteeringVal );
	}

	protected override void OnNetworkSend( ref ParamsWriteContext ctx )
	{
		ctx.Write( m_RPMVal );
		ctx.Write( m_SteeringVal );
	}

	protected override void OnNetworkRecieve( ref ParamsReadContext ctx )
	{
		ctx.Read( m_RPMVal );
		ctx.Read( m_SteeringVal );
	}

	protected override void OnPreSimulation( float pDt )
	{
		super.OnPreSimulation( pDt );

		if ( !m_IsPhysicsHost )
		{
			if ( GetGame().IsClient() )
			{
				m_RPMVal = m_RPMSynch;
				m_SteeringVal = m_SteeringSynch;
			} else
			{
				m_RPMSynch = m_RPMVal;
				m_SteeringSynch = m_SteeringVal;
			}

			return;
		}

		m_RPMVal = m_Engine.GetRPM();

		int gear = m_Gearbox.GetCurrentGear();
		float throttleVal = m_Throttle.Get();
		if ( throttleVal == 0.0 && gear != CarGear.NEUTRAL && !m_HasDriver )
		{
			m_BrakeVal = 1.0;
		}
		
		float fMax = m_Axles[0].GetTravelMax();
		float bMax = m_Axles[1].GetTravelMax();
		m_FrontSuspFraction = ( fMax - m_Wheels[0].GetSuspensionLength() ) - m_Axles[0].GetTravelMaxDown();
		m_BackSuspFraction = ( bMax - m_Wheels[1].GetSuspensionLength() ) - m_Axles[1].GetTravelMaxDown();
		
		ExpansionDebugUI( "fAmt: " + m_FrontSuspFraction );
		ExpansionDebugUI( "bAmt: " + m_BackSuspFraction );
		
		m_FrontSuspFraction = ( fMax - m_FrontSuspFraction ) * ( fMax + m_FrontSuspFraction ) / ( fMax * fMax );
		m_BackSuspFraction = ( bMax - m_BackSuspFraction ) * ( bMax + m_BackSuspFraction ) / ( bMax * bMax );
		
		float yUp = vector.Dot( m_Transform.data[1], "0 1 0" );
		
		m_GroundStabilizer = Math.Max( m_FrontSuspFraction, m_BackSuspFraction ); //! we can lean so long 1 wheel is on the ground

		ExpansionDebugUI( "fAmt: " + m_FrontSuspFraction );
		ExpansionDebugUI( "bAmt: " + m_BackSuspFraction );
		ExpansionDebugUI( "yUp: " + yUp );
		ExpansionDebugUI( "amt: " + m_GroundStabilizer );

		m_AirControl = m_GroundStabilizer < 0.1;
		m_GroundControl = yUp > 0.5 && m_GroundStabilizer > 0.1;
		
		ApplyAxleSteering( 0, m_SteeringVal );
		ApplyAxleSteering( 1, 0.0 );
			
		ApplyAxleTorque( 0, 0.0 );
		ApplyAxleTorque( 1, 0.0 );
		
		if ( EngineIsOn() )
		{
			m_Engine.OnUpdate( pDt, throttleVal, m_Gearbox.OnUpdate( m_ClutchState, m_BikeController.GetGear(), pDt ) );
			
			ApplyAxleBrake( 0, m_BrakeVal );
			ApplyAxleBrake( 1, m_BrakeVal );
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

		float absForwardSpeed = Math.AbsFloat( m_LinearVelocityMS[2] ) + 0.1;

		if ( m_GroundControl ) //! we are on the ground
		{
			vector fWheelNormal = m_Wheels[0].GetSuspensionContactNormal() * m_FrontSuspFraction;
			vector bWheelNormal = m_Wheels[1].GetSuspensionContactNormal() * m_BackSuspFraction;
			vector terrainSurface = fWheelNormal + bWheelNormal;
			terrainSurface = terrainSurface.Normalized().Multiply3( m_Transform.data );

			vector estDirUp = GetEstimatedOrientation( 0.1 )[1].Normalized();
			
			float leanAmount = Math.Clamp( m_SteeringVal, -0.16, 0.16 );
			float leanOnX = leanAmount * absForwardSpeed * 0.001;
			leanOnX = Math.Clamp( leanOnX, -0.8, 0.8 );

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
			
			stabilize = stabilize * 50.0 * m_BodyMass * m_GroundStabilizer;

			stabilize = stabilize.InvMultiply3( m_Transform.data );
			stabilize[2] = 0;
			torque += ( applyPosition * stabilize ).Multiply3( m_Transform.data );
		} else if ( m_AirControl ) //! we are in the air
		{
			float airControlForward = m_AirControlForward * m_AirControlForwardCoef;
			float airControlRight = m_AirControlRight * m_AirControlRightCoef;

			vector airControl = Vector( airControlRight, 0, -airControlForward ) * m_BodyMass * 0.25;

			torque += ( Vector( 0, 1, 0 ) * airControl ).Multiply3( m_Transform.data );
		}

		ExpansionDebugUI();
	}
}