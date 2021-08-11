/*
 * ExpansionVehicleCarBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * Copyright (c) 2005 Erwin Coumans http://continuousphysics.com/Bullet/
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionVehicleCarBase
 * @brief		This class handle bicycle simulation
 **/
class ExpansionVehicleCarBase extends ExpansionVehicleBase
{
	private ExpansionCarController m_CarController;

	private ref ExpansionVehicleGearbox m_Gearbox;
	private ref ExpansionVehicleEngine m_Engine;
	private ref ExpansionVehicleSteering m_Steering;
	private ref ExpansionVehicleThrottle m_Throttle;
	
	private float m_RPMVal;
	private float m_SteeringVal;
	private float m_BrakeVal;

	private float m_RPMSynch;
	private float m_SteeringSynch;

	void ExpansionVehicleCarBase()
	{
		Class.CastTo( m_CarController, m_Controller );

		RegisterNetSyncVariableFloat( "m_RPMSynch", 0, 0, 4 );
		RegisterNetSyncVariableFloat( "m_SteeringSynch", 0, 0, 4 );
		
		string path;
		
		path = "CfgVehicles " + GetType() + " SimulationModule drive";
		
		string drive;
		GetGame().ConfigGetText( path, drive );
		
		if ( drive == "DRIVE_RWD" )
		{
			m_Engine = new ExpansionVehicleEngineRWD( this );
		} else if ( drive == "DRIVE_FWD" )
		{
			m_Engine = new ExpansionVehicleEngineFWD( this );
		} else if ( drive == "DRIVE_AWD" )
		{
			m_Engine = new ExpansionVehicleEngineAWD( this );
		} else if ( drive == "DRIVE_662" )
		{
			m_Engine = new ExpansionVehicleEngineAWD( this );
		} else if ( drive == "DRIVE_642" )
		{
			m_Engine = new ExpansionVehicleEngineAWD( this );
		}
		
		m_Gearbox = new ExpansionVehicleGearbox( this );	
		m_Steering = new ExpansionVehicleSteering( this );
		m_Throttle = new ExpansionVehicleThrottle( this );
	}

	void ~ExpansionVehicleCarBase()
	{
		delete m_Gearbox;
		delete m_Engine;
		delete m_Steering;
		delete m_Throttle;
	}

	override ExpansionVehicleController GetControllerInstance()
	{
		return new ExpansionCarController( this );
	}

	ExpansionCarController GetController()
	{
		return m_CarController;
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
		return 4;
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

	override int GetGear()
	{
		return m_Gearbox.GetCurrentGear();
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

	float Sign( float number )
	{
		if ( number < 0 )
			return -1;
		return 1;
	}

	protected override void OnHumanPilot( PlayerBase driver, float pDt )
	{
		#ifdef EXPANSION_DEBUG_UI_VEHICLE
		CF_Debugger_Block dbg_Vehicle = CF.Debugger.Get("Vehicle", this);
		#endif

		m_SteeringVal += m_Steering.CalculateChange( pDt, Math.AbsFloat( m_LinearVelocityMS[2] ), m_SteeringVal, m_CarController.GetTurnRight() - m_CarController.GetTurnLeft() );
		
		m_Throttle.Update( pDt, m_CarController.GetForward(), m_CarController.GetGentle(), m_CarController.GetTurbo() );

		m_BrakeVal = m_CarController.GetBackward();
		
		#ifdef EXPANSION_DEBUG_UI_VEHICLE
		dbg_Vehicle.Set("Steering", m_SteeringVal );
		#endif
	}

	protected override void OnAIPilot( ExpansionAIBase driver, float pDt )
	{
	}

	protected override void OnNetworkSend(  ParamsWriteContext ctx )
	{
		ctx.Write( m_RPMVal );
		ctx.Write( m_SteeringVal );
	}

	protected override void OnNetworkRecieve( ParamsReadContext ctx )
	{
		ctx.Read( m_RPMVal );
		ctx.Read( m_SteeringVal );
	}

	protected override void OnPreSimulation( float pDt )
	{
		#ifdef EXPANSION_DEBUG_UI_VEHICLE
		CF_Debugger_Block dbg_Vehicle = CF.Debugger.Get("Vehicle", this);
		#endif

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
		
		int gear = m_Gearbox.GetCurrentGear();
		float throttleVal = m_Throttle.Get();
		if ( throttleVal == 0.0 && gear != CarGear.NEUTRAL && !m_HasDriver )
		{
			m_BrakeVal = 1.0;
		}

		float wheelBase = Math.AbsFloat( m_Wheels[2].GetInitialWheelPosition()[2] - m_Wheels[0].GetInitialWheelPosition()[2] );
		float turnRadius = ( Math.DEG2RAD * m_Axles[0].GetMaxSteeringAngle() ) / ( Math.PI2 );

		float leftBackWheel = Math.AbsFloat( m_Wheels[2].GetInitialWheelPosition()[0] ) * -Sign( m_SteeringVal );
		float rightBackWheel = Math.AbsFloat( m_Wheels[3].GetInitialWheelPosition()[0] ) * Sign( m_SteeringVal );

		float leftFrontWheelSteer = Math.Atan2( wheelBase / ( turnRadius + leftBackWheel ), 1 );
		float rightFrontWheelSteer = Math.Atan2( wheelBase / ( turnRadius + rightBackWheel ), 1 );
		leftFrontWheelSteer = Math.AbsFloat( Math.RAD2DEG * leftFrontWheelSteer * 2.0 / Math.PI ) * m_SteeringVal;
		rightFrontWheelSteer = Math.AbsFloat( Math.RAD2DEG * rightFrontWheelSteer * 2.0 / Math.PI ) * m_SteeringVal;
		
		#ifdef EXPANSION_DEBUG_UI_VEHICLE
		dbg_Vehicle.Set("LF Steer", leftFrontWheelSteer );
		dbg_Vehicle.Set("RF Steer", rightFrontWheelSteer );
		#endif

		m_Wheels[0].SetSteering( leftFrontWheelSteer );
		m_Wheels[1].SetSteering( rightFrontWheelSteer );

		if ( EngineIsOn() )
		{
			m_Engine.OnUpdate( pDt, throttleVal, m_Gearbox.OnUpdate( m_ClutchState, m_CarController.GetGear(), pDt ) );
			
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
		#ifdef EXPANSION_DEBUG_UI_VEHICLE
		CF_Debugger_Block dbg_Vehicle = CF.Debugger.Get("Vehicle", this);
		#endif

		super.OnSimulation( pDt, force, torque );
		
		vector minmax[2];
		ClippingInfo(minmax);
		
		vector fBL = Vector(minmax[0][0], minmax[0][1], 0);
		vector fBR = Vector(minmax[1][0], minmax[0][1], 0);
		vector fTL = Vector(minmax[0][0], minmax[1][1], 0);
		vector fTR = Vector(minmax[1][0], minmax[1][1], 0);
		
		DBGDrawLineMS(fBL, fBR);
		DBGDrawLineMS(fBR, fTR);
		DBGDrawLineMS(fTR, fTL);
		DBGDrawLineMS(fTL, fBL);
		
		float w = Math.AbsFloat(minmax[0][0]) + Math.AbsFloat(minmax[1][0]);
		float h = Math.AbsFloat(minmax[0][1]) + Math.AbsFloat(minmax[1][1]);
		
		#ifdef EXPANSION_DEBUG_UI_VEHICLE
		dbg_Vehicle.Set("W", w );
		dbg_Vehicle.Set("H", h );
		#endif
		
		float drag_c = 0.5 * (1.0 - m_AirDragFrontTotal) * (w * h) * 1.29;

		#ifdef EXPANSION_DEBUG_UI_VEHICLE
		dbg_Vehicle.Set("Drag", drag_c );
		#endif
		
		float speed = m_LinearVelocityMS.Length();
		
		vector drag = m_LinearVelocityMS * -drag_c * m_BodyMass * speed * pDt;
		
		force += drag.Multiply3(m_Transform.data);
	}
}