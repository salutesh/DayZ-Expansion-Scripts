/**
 * ExpansionPlaneScript.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionPlane
 * @brief		This class handle plane movement and physics
 **/
class ExpansionPlaneScript extends CarScript
{
	/*
	ARMA 3 - Plane_CAS_02_base_F
	maxSpeed = 900;
	angleOfIndicence = -0.00872665;
	envelope[] = {0.0,0.07,0.28,0.63,1.11,1.74,2.51,3.41,4.46,5.64,6.96,8.42,8.8,9.11,9.38,9.45,9.43,9.0,8.0,7.0,6.0}
	altNoForce = 10000;
	altFullForce = 2000;
	thrustCoef[] = {1.28,1.33,1.37,1.4,1.41,1.39,1.36,1.33,1.29,1.25,1.2,1.15,1.05,0.5,0.0,0.0,0.0,0.0,0.0}
	aileronSensitivity = 0.75;
	aileronCoef[] = {0.0,0.1,0.3,0.6,0.8,0.95,1.0,1.02,1.03,1.04,1.04,1.0,0.9,0.7,0.3,0.2,0.15,0.1,0.1}
	elevatorSensitivity = 0.75;
	elevatorCoef[] = {0.0,0.2,0.95,0.6,0.55,0.5,0.45,0.4,0.35,0.3,0.26,0.22,0.19,0.16,0.14,0.12,0.1,0.09,0.08}
	rudderInfluence = 0.866;
	rudderCoef[] = {0.0,0.6,1.2,1.5,1.8,2.0,2.2,2.4,2.45,2.5,2.55,2.6,2.2,1.3,0.3,0.2,0.15,0.1,0.1}
	aileronControlsSensitivityCoef = 3.6;
	elevatorControlsSensitivityCoef = 3.4;
	rudderControlsSensitivityCoef = 3.4;
	draconicForceXCoef = 13;
	draconicForceYCoef = 1.3;
	draconicForceZCoef = 1.0;
	draconicTorqueXCoef[] = {4.8,6.0,7.5,8.2,9.0,10.4,11.0,11.5,12.0,12.4,12.6,12.7,12.8,12.9,13.0,13.1}
	draconicTorqueYCoef[] = {12.0,7.5,4.0,1.5,0.1,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}
	*/

	/*
	ARMA 3 CUP - CUP_AN2_Base
	
	maxSpeed = 258;
	aileronSensitivity = 1;
	elevatorSensitivity = 0.8;
	envelope[] = {0.0,0.1,1.0,1.8,2.8,3.3,3.5,3.2,2.6,2.0,1.5,1.0,0.5,0.0}
	*/

	// ------------------------------------------------------------
	//! Constant Values - Set in Constructor, Errors occur if not.
	// ------------------------------------------------------------

	//! Coefficient for body friction
	protected float m_BodyFrictionCoef;

	protected float m_AngleOfIndicence = -0.00872665;

	protected float m_ElevatorForceCoef;
	protected float m_AileronForceCoef;

	protected float m_RudderForceCoef;
	protected float m_RudderInfluence;

	//! Lift factor 
	private int m_MaxFlightEnvelope = 12;
	private float m_FlightEnvelope[12] = { 0.0, 0.2, 0.9, 2.1, 2.5, 3.3, 3.5, 3.2, 2.5, 2.0, 1.5, 1.0 };

	// ------------------------------------------------------------
	//! Member values
	// ------------------------------------------------------------
	private float m_RotorSpeed;
	private float m_RotorSpeedTarget;

	private float m_Thrust;
	private float m_ThrustTarget;

	private float m_AngleOfAttack;

	private float m_Elevator;
	private float m_ElevatorTarget;

	private float m_Aileron;
	private float m_AileronTarget;

	private float m_Bank;

	private float m_Rudder;
	private float m_RudderTarget;

	private float m_Flaps;
	private float m_FlapsTarget;

	private bool m_Hit;
	private bool m_HitDetermined;
	private Object m_HitObject;
	private vector m_HitPosition;
	private vector m_HitNormal;
	private float m_HitFraction;

	// ------------------------------------------------------------
	//! Animations
	// ------------------------------------------------------------
	private float m_RotorAnimationPosition;

	private float m_ThrustSync;
	private float m_ElevatorSync;
	private float m_AileronSync;
	private float m_RudderSync;
	private float m_FlapsSync;

	// ------------------------------------------------------------
	//! Controller, casted
	// ------------------------------------------------------------
	protected ExpansionPlaneController m_PlaneController;

	// ------------------------------------------------------------	
	void ExpansionPlaneScript()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPlaneScript::Constructor start");
		#endif
		
		SetEventMask( EntityEvent.CONTACT | EntityEvent.SIMULATE | EntityEvent.INIT );

		m_Controller = new ExpansionPlaneController( this );

		Class.CastTo( m_PlaneController, m_Controller );

		RegisterNetSyncVariableFloat( "m_RotorSpeed" );

		RegisterNetSyncVariableFloat( "m_ElevatorSync" );
		RegisterNetSyncVariableFloat( "m_AileronSync" );
		RegisterNetSyncVariableFloat( "m_RudderSync" );	
		RegisterNetSyncVariableFloat( "m_FlapsSync" );	
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPlaneScript::Constructor end");
		#endif
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if ( !m_IsPhysicsHost )
		{
			m_Elevator = m_ElevatorSync;
			m_Aileron = m_AileronSync;
			m_Rudder = m_RudderSync;
			m_Flaps = m_FlapsSync;
		}
	}

	// ------------------------------------------------------------
	protected override void OnHumanPilot( PlayerBase driver, float pDt )
	{
		if ( EngineIsOn() )
		{
			m_ThrustTarget = m_PlaneController.GetEngine();

			m_FlapsTarget = m_PlaneController.GetFlaps();

			m_ElevatorTarget = m_PlaneController.GetElevatorDown() - m_PlaneController.GetElevatorUp();

			m_AileronTarget = m_PlaneController.GetAileronLeft() - m_PlaneController.GetAileronRight();

			m_RudderTarget = m_PlaneController.GetRudderRight() - m_PlaneController.GetRudderLeft();
		} else
		{
			m_ThrustTarget = 0;
		}
	}

	// ------------------------------------------------------------
	protected override void OnPreSimulation( float pDt )
	{
		m_HitDetermined = false;
		
		PerformGroundRaycast();

		super.OnPreSimulation( pDt );
	}

	// ------------------------------------------------------------
	override void OnPostSimulation( float pDt )
	{
		super.OnPostSimulation( pDt );
		
		ExpansionDebugger.Push( EXPANSION_DEBUG_VEHICLE_PLANE );
	}

	// ------------------------------------------------------------
	protected override void OnParticleUpdate( float pDt )
	{
	}

	// ------------------------------------------------------------
	protected override void OnAnimationUpdate( float pDt )
	{
		if ( m_Exploded )
		{
			HideSelection( "hiderotor" );
			HideSelection( "hiderotorblur" );
			return;
		}

		m_RotorAnimationPosition += m_RotorSpeed * pDt * 0.1;

		if ( m_RotorSpeed > 0.7 )
		{
			ShowSelection( "hiderotorblur" );
			HideSelection( "hiderotor" );
		} else
		{
			ShowSelection( "hiderotor" );
			HideSelection( "hiderotorblur" );
		}

		if ( m_RotorAnimationPosition >= 1 )
			m_RotorAnimationPosition -= 1;

		if ( m_RotorSpeed > 0.01 )
		{
			SetAnimationPhase( "rotor", m_RotorAnimationPosition );
		}

		SetAnimationPhase( "aileron", m_Aileron );
		SetAnimationPhase( "elevator", m_Elevator );
		SetAnimationPhase( "rudder", m_Rudder );
		SetAnimationPhase( "flap", m_Flaps );

		SetAnimationPhase( "compasspointer", GetOrientation()[0] * Math.DEG2RAD );
		SetAnimationPhase( "attitudeDiveRTD", GetDirection()[1] );
		SetAnimationPhase( "attitudeBankRTD", GetOrientation()[2] / 360 );

		super.OnAnimationUpdate( pDt );
	}

	// ------------------------------------------------------------
	override void EOnContact( IEntity other, Contact extra ) 
	{
		if ( IsMissionHost() )
		{
			if ( extra.Impulse > m_BodyMass * 25 )
			{
				Explode( DT_EXPLOSION, "RGD5Grenade_Ammo" );
			}
		}
	}

	// ------------------------------------------------------------
	override void ExpansionOnExplodeServer( int damageType, string ammoType )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPlaneScript::ExpansionOnExplodeServer - Start");
		#endif

		super.ExpansionOnExplodeServer( damageType, ammoType );

		EntityAI item = null;
		item = FindAttachmentBySlotName( "GlowPlug" );
		if ( item )
			item.SetHealth( "", "", 0 );
					
		item = FindAttachmentBySlotName( "ExpansionAircraftBattery" );
		if ( item )
			item.SetHealth( "", "", 0 );
			
		item = FindAttachmentBySlotName( "Reflector_1_1" );
		if ( item )
			item.SetHealth( "", "", 0 );
		
		item = FindAttachmentBySlotName( "Reflector_2_1" );
		if ( item )
			item.SetHealth( "", "", 0 );

		m_Exploded = true;
		m_ExplodedSynchRemote = true;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPlaneScript::ExpansionOnExplodeServer - End");
		#endif	
	}

	// ------------------------------------------------------------
	protected override void OnSimulation( float pDt, out vector force, out vector torque )
	{
		if ( !dBodyIsActive( this ) )
			return;

		bool isAboveWater;
		float buoyancyForce;

		if ( m_Exploded )
		{
			if ( dBodyIsActive( this ) )
			{
				// floaty mc float
			}

			return;
		}

		if ( m_PlaneController.GetEngine() > 0.1 )
		{
			GetUApi().GetInputByName( "UACarBack" ).ForceDisable( true );
		} else 
		{
			GetUApi().GetInputByName( "UACarBack" ).ForceDisable( false );
		}

		float fullDirControlSpeed = 25;
		float noDirControlSpeed = 13.5;
		if ( m_LinearVelocityMS[2] < fullDirControlSpeed )
		{
			float max = Math.Clamp( ( m_LinearVelocityMS[2] - noDirControlSpeed - 1 ) / ( fullDirControlSpeed - noDirControlSpeed ), 0, 1 );

			m_ElevatorTarget = Math.Clamp( m_ElevatorTarget, -max, max );
			m_AileronTarget = Math.Clamp( m_AileronTarget, -max, max );
		}

		m_Thrust += Math.Clamp( m_ThrustTarget - m_Thrust, -0.7 * pDt, 0.4 * pDt );
		m_Thrust = Math.Clamp( m_Thrust, 0, 1 );

		m_RotorSpeed += Math.Clamp( m_RotorSpeedTarget - m_RotorSpeed, -0.1 * pDt, 0.1 * pDt );
		m_RotorSpeed = Math.Clamp( m_RotorSpeed, 0, 1 );

		m_Elevator += Math.Clamp( m_ElevatorTarget - m_Elevator, -4 * pDt, 4 * pDt );
		m_Elevator = Math.Clamp( m_Elevator, -1, 1 );

		m_Aileron += Math.Clamp( m_AileronTarget - m_Aileron, -4 * pDt, 4 * pDt );
		m_Aileron = Math.Clamp( m_Aileron, -1, 1 );

		m_Rudder += Math.Clamp( m_RudderTarget - m_Rudder, -4 * pDt, 4 * pDt );
		m_Rudder = Math.Clamp( m_Rudder, -1, 1 );

		m_Flaps += Math.Clamp( m_FlapsTarget - m_Flaps, -1 * pDt, 1 * pDt );
		m_Flaps = Math.Clamp( m_Flaps, 0, 1 );

		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_PLANE, "Thrust: " + m_Thrust );
		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_PLANE, "Thrust Target: " + m_ThrustTarget );

		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_PLANE, "Elevator: " + m_Elevator );
		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_PLANE, "Elevator Target: " + m_ElevatorTarget );

		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_PLANE, "Aileron: " + m_Aileron );
		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_PLANE, "Aileron Target: " + m_AileronTarget );

		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_PLANE, "Rudder: " + m_Rudder );
		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_PLANE, "Rudder Target: " + m_RudderTarget );

		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_PLANE, "Flaps: " + m_Flaps );
		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_PLANE, "Flaps Target: " + m_FlapsTarget );

		vector tForce			= vector.Zero;
		vector tCenter			= vector.Zero;

		CalculateAngleOfAttack();

		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_PLANE, "Angle Of Attack: " + m_AngleOfAttack );

		// lift force
		{
			tForce[0] = 0;
			tForce[1] = GetUpForce() * m_BodyMass * 10.0;
			tForce[2] = 0;

			float bank = GetOrientation()[2] * Math.DEG2RAD;
			float bankCoef = 2.0;

			tCenter[0] = bankCoef * -Math.Sin( bank );
			tCenter[1] = 0;
			tCenter[2] = bankCoef * ( 1.0 - Math.Cos( bank ) );

			ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_PLANE, "Lift Force: " + tForce[1] );

			force += tForce;
			torque += tCenter * tForce;
		}

		// engine force
		{
			float speedCoef = MathHelper.Interpolate( m_LinearVelocityMS[2], m_MaxSpeedMS * 0.66, m_MaxSpeedMS * 1.15, 1, 0 ) * 0.25;

			tForce[0] = 0;
			tForce[1] = 0;
			tForce[2] = m_Thrust * m_RotorSpeed * m_BodyMass * ( 0.4 * 10.0 / 13.0 ) * m_MaxSpeedMS * speedCoef;

			force += tForce;
		}

		// elevator
		{
			tForce[0] = m_Elevator * 5.0 * m_ElevatorForceCoef * m_BodyMass * m_BoundingRadius;
			tForce[1] = 0;
			tForce[2] = 0;

			torque += tForce;
		}

		// aileron
		{
			tForce[0] = 0;
			tForce[1] = 0;
			tForce[2] = m_Aileron * 5.0 * m_AileronForceCoef * m_BodyMass * m_BoundingRadius;

			torque += tForce;
		}

		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_PLANE, "m_Transform: " + m_Transform.GetBasis() );
		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_PLANE, "Side: " + m_Transform.data[0] );
		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_PLANE, "Up : " + m_Transform.data[1] );
		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_PLANE, "Forward : " + m_Transform.data[2] );
		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_PLANE, "Position : " + m_Transform.data[3] );

		// rudder
		{
			float rudderControl = 1;

			if ( m_Rudder * m_LinearVelocityMS[2] != 0 )
			{
				float horSpeed = Math.Sqrt( ( m_LinearVelocityMS[2] * m_LinearVelocityMS[2] ) + ( m_LinearVelocityMS[0] * m_LinearVelocityMS[0] ) );
				rudderControl = m_LinearVelocityMS[2] / horSpeed;
				rudderControl = MathHelper.Interpolate( Math.AbsFloat( rudderControl ), m_RudderInfluence, 1, 0, 1 );
			}

			tForce[0] = m_Rudder * m_RudderForceCoef * rudderControl * m_BodyMass;
			tForce[1] = 0;
			tForce[2] = 0;

			tCenter[0] = 0;
			tCenter[1] = -0.25;
			tCenter[2] = m_BoundingRadius * 0.75;

			torque += tCenter * tForce;
		}

		// glide force
		{
			float velLengthSq = m_LinearVelocityMS.LengthSq();
			float velLength = m_LinearVelocityMS.Length();
			vector velNormal = m_LinearVelocityMS.Normalized();

			float glideForceX = Math.AbsFloat( velNormal[0] ) * ( velLengthSq * 0.005 + velLength * 0.02 );
			float glideForceY = Math.AbsFloat( velNormal[1] ) * ( velLengthSq * 0.02 + velLength * 0.2 );
			
			tForce[0] = -velNormal[0] * glideForceX * 7.5;
			tForce[1] = -velNormal[1] * glideForceY;
			tForce[2] = velNormal[2] * ( glideForceX * 0.15 + glideForceY * 0.25 );

			force += tForce * m_BodyMass;
		}

		// friction
		{
			force += GetLinearFrictionForce() * m_BodyFrictionCoef;
		}
		
		// convert forces to worldspace
		{
			force = force.Multiply3( m_Transform.GetBasis().data );
			torque = torque.Multiply3( m_Transform.GetBasis().data );
		}

		dBodySetDamping( this, 0.0, 0.5 );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPlaneScript::UpdatePhysics end");
		#endif
	}

	// ------------------------------------------------------------
	private float GetUpForce()
	{
		float modelSize = m_BoundingRadius * 1.5;
		float nearGround = 0;

		if ( m_Hit && modelSize > 0 )
		{
			float relHeight = ( GetPosition()[1] - m_HitPosition[1] ) / modelSize;
			relHeight = Math.Clamp( 1.5 - relHeight, 0, 1 );
			nearGround = relHeight * relHeight * 0.1;
		}

		int maxI = m_MaxFlightEnvelope - 1;
		float maxAoa = 18 * Math.DEG2RAD;
		float minAoa = -10 * Math.DEG2RAD;
		float zeroAoA = 2 * Math.DEG2RAD;
		float aoa = m_AngleOfAttack + zeroAoA;
		aoa += m_Flaps * Math.DEG2RAD;

		float speedRel = Math.AbsFloat( m_LinearVelocityMS[2] ) * 0.8 / m_MaxSpeedMS;

		aoa += m_Flaps * 4.0 * Math.DEG2RAD * MathHelper.Interpolate( speedRel, 0.22, 0.4, 1.0, 0.0 );

		if ( aoa > maxAoa )
		{
			aoa = 2 * maxAoa - aoa; 
			if ( aoa < 0 )
				aoa = 0;
		}

		if ( aoa < minAoa )
			aoa = minAoa;

		aoa *= 1.0 / maxAoa;

		float fSpeed = maxI * speedRel;

		int iLow = Math.Floor( fSpeed );
		float frac = fSpeed - iLow;

		float liftFactor = 0;
		if ( iLow >= maxI )
		{
			liftFactor = m_FlightEnvelope[ maxI - 1 ];
		} else
		{
			float fLow = m_FlightEnvelope[ iLow ];
			float fLowInc = m_FlightEnvelope[ iLow + 1 ];
			liftFactor = fLow + ( fLowInc - fLow ) * frac;
		}

		liftFactor *= aoa;
		if ( liftFactor >= 0 )
			liftFactor *= 1 + nearGround;

		return liftFactor;
	}
	
	// ------------------------------------------------------------
	private void CalculateAngleOfAttack()
	{
		float spd = m_LinearVelocityMS.Length();
		if ( spd > 0.00001 )
		{
			m_AngleOfAttack = -m_LinearVelocityMS[2] * ( 1.0 / spd ) * m_AngleOfIndicence;
		} else
		{
			m_AngleOfAttack = 0;
		}
	}

	// ------------------------------------------------------------
	private vector GetLinearFrictionForce()
	{
		vector friction = vector.Zero;

		friction[0] = ( MathHelper.SquareSign( m_LinearVelocityMS[0] ) * 0.001 ) + ( m_LinearVelocityMS[0] * 0.1 );
		friction[1] = ( MathHelper.SquareSign( m_LinearVelocityMS[1] ) * 0.0005 ) + ( m_LinearVelocityMS[1] * 0.05 );
		friction[2] = ( MathHelper.SquareSign( m_LinearVelocityMS[2] ) * 0.00006 ) + ( m_LinearVelocityMS[2] * 0.006 );

		return -friction * m_BodyMass;
	}

	// ------------------------------------------------------------
	private void PerformGroundRaycast()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionPlaneScript::PerformGroundRaycast - Start" );
		#endif
		
		if ( m_HitDetermined )
			return;
		
		float modelSize = m_BoundingRadius * 1.5;
		modelSize += 10.0;

		vector start = GetPosition();
		vector end = GetPosition() - Vector( 0, modelSize, 0 );

		PhxInteractionLayers collisionLayerMask = PhxInteractionLayers.BUILDING|PhxInteractionLayers.DOOR|PhxInteractionLayers.VEHICLE|PhxInteractionLayers.ROADWAY|PhxInteractionLayers.TERRAIN|PhxInteractionLayers.ITEM_SMALL|PhxInteractionLayers.ITEM_LARGE|PhxInteractionLayers.FENCE;
		
		m_Hit = DayZPhysics.SphereCastBullet( start, end, 5.0, collisionLayerMask, this, m_HitObject, m_HitPosition, m_HitNormal, m_HitFraction );
		m_HitDetermined = true;

		#ifdef EXPANSIONEXPRINT
		EXPrint( "m_Hit: " + m_Hit );
		#endif
		#ifdef EXPANSIONEXPRINT
		EXPrint( "m_HitDetermined: " + m_HitDetermined );
		#endif
		#ifdef EXPANSIONEXPRINT
		EXPrint( "m_HitObject: " + m_HitObject );
		#endif
		#ifdef EXPANSIONEXPRINT
		EXPrint( "m_HitPosition: " + m_HitPosition );
		#endif
		#ifdef EXPANSIONEXPRINT
		EXPrint( "m_HitNormal: " + m_HitNormal );
		#endif
		#ifdef EXPANSIONEXPRINT
		EXPrint( "m_HitFraction: " + m_HitFraction );
		#endif

		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionPlaneScript::PerformGroundRaycast - End" );
		#endif
	}

	// ------------------------------------------------------------
	override void SetActions()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPlaneScript::SetActions - Start");
		#endif

		super.SetActions();

		AddAction( ExpansionActionFlare );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPlaneScript::SetActions - End");
		#endif
	}

	// ------------------------------------------------------------
	override void OnEngineStop()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPlaneScript::OnEngineStop - Start");
		#endif

		super.OnEngineStop();

		m_RotorSpeedTarget = 0;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPlaneScript::OnEngineStop - End");
		#endif
	}

	// ------------------------------------------------------------
	override void OnEngineStart()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPlaneScript::OnEngineStart - Start");
		#endif

		super.OnEngineStart();

		m_RotorSpeedTarget = 1;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPlaneScript::OnEngineStart - End");
		#endif
	}

	// ------------------------------------------------------------
	override void ExpansionOnSpawnExploded()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPlaneScript::ExpansionOnSpawnExploded - Start");
		#endif

		super.ExpansionOnSpawnExploded();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPlaneScript::ExpansionOnSpawnExploded - End");
		#endif
	}

	// ------------------------------------------------------------
	override int Get3rdPersonCameraType()
	{
		return DayZPlayerCameras.DAYZCAMERA_3RD_VEHICLE;
	}

	// ------------------------------------------------------------
	override int GetAnimInstance()
	{
		return ExpansionVehicleAnimInstances.EX_HATCHBACK;
	}

	// ------------------------------------------------------------
	override int GetSeatAnimationType( int posIdx )
	{
		switch( posIdx )
		{
		case 0:
			return DayZPlayerConstants.VEHICLESEAT_DRIVER;
		case 1:
			return DayZPlayerConstants.VEHICLESEAT_CODRIVER;
		default:
			return 0;
		}

		return 0;
	}

	// ------------------------------------------------------------
	override bool CrewCanGetThrough( int posIdx )
	{
		return true;
	}

	// ------------------------------------------------------------
	override float OnSound( CarSoundCtrl ctrl, float oldValue )
	{
		if ( m_Exploded )
			return 0;

		switch ( ctrl )
		{
			case CarSoundCtrl.SPEED:
			{
				return m_RotorSpeed;
			}
			case CarSoundCtrl.RPM:
			{
				return m_RotorSpeed /* ( 0.1 + m_Thrust )*/;
			}
			case CarSoundCtrl.ENGINE:
			{
				if ( m_RotorSpeed > 0 )
					return 1;

				return 0;
			}
		}

		return oldValue;
	}

	// ------------------------------------------------------------
	override bool CanReachDoorsFromSeat( string pDoorsSelection, int pCurrentSeat )
	{
		return true;		
	}

	// ------------------------------------------------------------
	override bool CanReachSeatFromDoors( string pSeatSelection, vector pFromPos, float pDistance = 1.0 )
	{
		return true;		
	}

	// ------------------------------------------------------------
	protected override bool CanSimulate()
	{
		return true;
	}

	// ------------------------------------------------------------
	override bool IsCar()
	{
		return true;
	}

	// ------------------------------------------------------------
	override bool IsPlane()
	{
		return true;
	}

	// ------------------------------------------------------------
	override CarLightBase CreateFrontLight()
	{
		return CarLightBase.Cast( ScriptedLightBase.CreateLight( ExpansionHelicopterFrontLight ) );
	}

	// ------------------------------------------------------------
	override CarLightBase CreateRearLight()
	{
		return CarRearLightBase.Cast( ScriptedLightBase.CreateLight( ExpansionRearHelicopterLights ) );
	}
}