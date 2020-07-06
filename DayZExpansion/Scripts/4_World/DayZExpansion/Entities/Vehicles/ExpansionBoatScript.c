/**
 * ExpansionBoatScript.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionBoat
 * @brief		This class handle boat movement and physics
 **/
class ExpansionBoatScript extends OffroadHatchback
{
	// ------------------------------------------------------------
	//! Static Values
	// ------------------------------------------------------------	
	// To whomever reads this, this will be re-written
	protected float m_Offset;

	protected float m_TurnCoef;

	// ------------------------------------------------------------
	//! Local values
	// ------------------------------------------------------------
	private float m_Thrust;
	private float m_ThrustTarget;

	private float m_Turn;
	private float m_TurnTarget;

	private bool m_UseBoatController;

	private bool m_MotorOn;
	
	// ------------------------------------------------------------
	//! Effects
	// ------------------------------------------------------------	
	private Particle m_ParticleFirst;
	private Particle m_ParticleSecond;

	private Particle m_ParticleSideFirst;
	private Particle m_ParticleSideSecond;

	private Particle m_ParticleEngine;

	private ExpansionBoatScriptSoundProxyBase m_BoatSoundProxy;

	// ------------------------------------------------------------
	//! Animations
	// ------------------------------------------------------------
	private float m_RotorAnimationPosition;
	
	// ------------------------------------------------------------
	//! Controller, casted
	// ------------------------------------------------------------
	private ExpansionBoatController m_BoatController;

	// ------------------------------------------------------------
	//! Constructor
	// ------------------------------------------------------------	
	void ExpansionBoatScript()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBoatScript::Constructor start");
		#endif
		
		//!Values
		m_Offset 					= 0.75;
		m_UseBoatController			= true;

		SetEventMask( EntityEvent.CONTACT | EntityEvent.SIMULATE | EntityEvent.INIT );

		RegisterNetSyncVariableBool( "m_MotorOn" );
		
		m_Controller = new ExpansionBoatController( this );

		if ( IsMissionClient() )
		{
			/*		
			string sound_controller_path = "CfgVehicles " + GetType() + " Sounds boatSoundProxy";
			string sound_controller = GetGame().ConfigGetTextOut( sound_controller_path );
		
			if ( Class.CastTo( m_BoatSoundProxy, GetGame().CreateObject( sound_controller, GetPosition(), true ) ) )
			{
				m_BoatSoundProxy.SetPosition( "0 0 0" );
				AddChild( m_BoatSoundProxy, -1 );

				m_BoatSoundProxy.SetBoat( this );
			}
			*/
		}
		
		Class.CastTo( m_BoatController, m_Controller );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBoatScript::Constructor end");
		#endif
	}

	void ~ExpansionBoatScript()
	{
		if ( m_ParticleEngine )
		{
			m_ParticleEngine.Stop();
		}

		if ( m_ParticleFirst ) 
		{
			m_ParticleFirst.Stop();
		}
			
		if ( m_ParticleSecond ) 
		{
			m_ParticleSecond.Stop();
		}

		if ( m_ParticleSideFirst ) 
		{
			m_ParticleSideFirst.Stop();
		}
			
		if ( m_ParticleSideSecond ) 
		{
			m_ParticleSideSecond.Stop();
		}
	}
	
	override void DeferredInit()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBoatScript::DeferredInit start");
		#endif

		DisablePhysics( this );

		super.DeferredInit();

		EnablePhysics( this );

		HideSelection( "hiderotorblur" );
		ShowSelection( "hiderotor" );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBoatScript::DeferredInit end");
		#endif
	}

	bool IsUsingBoatController()
	{
		return m_UseBoatController;
	}

	void SetUsingBoatController( bool use )
	{
		m_UseBoatController = use;
	}

	void MotorStart()
	{
		m_MotorOn = true;

		dBodyActive( this, ActiveState.ACTIVE );
		dBodyDynamic( this, true );

		SetSynchDirty();
	}

	void MotorStop()
	{
		m_MotorOn = false;

		SetSynchDirty();
	}

	bool MotorIsOn()
	{
		return m_MotorOn;
	}

	// ------------------------------------------------------------
	override void SetActions()
	{
		super.SetActions();
	}

	// ------------------------------------------------------------
	protected override void OnHumanPilot( PlayerBase driver, float pDt )
	{
		float steering = m_BoatController.GetTurnLeft() - m_BoatController.GetTurnRight();
		float brake = m_BoatController.GetBackward();
		float thrust = m_BoatController.GetForward();

		int gear = GetController().GetGear();

		if ( brake > 0 )
		{
			m_ThrustTarget = -m_LinearVelocityMS[2] * ( 1.0 / m_MaxSpeedMS ) * 2.0;
		} else
		{
			float ratio = 0;

			if ( gear == CarGear.REVERSE )
			{
				thrust *= -1.0;

				steering = -steering;
			} else if ( gear == CarGear.NEUTRAL )
			{
				thrust = 0;

				steering = 0;
			} else
			{
				thrust *= 1.0;

				steering = steering;
			}

			m_TurnTarget += Math.Clamp( steering - m_TurnTarget, -40.0 * pDt, 40.0 * pDt );
			m_ThrustTarget += Math.Clamp( thrust - m_ThrustTarget, -40.0 * pDt, 40.0 * pDt );
		}
	}

	// ------------------------------------------------------------
	protected override void OnParticleUpdate( float pDt )
	{
		vector enginePosition = ModelToWorld(GetMemoryPointPos("engine")); 
		//! TODO: Make particles for reverse gear too
		if ( enginePosition[1] <= GetGame().SurfaceGetSeaLevel() && GetSpeedometer() > 5 ) 
		{
			if (!m_ParticleEngine && MemoryPointExists("engine"))
			{
				m_ParticleEngine = Particle.PlayOnObject(ParticleList.EXPANSION_BOAT_ENGINE, this, GetMemoryPointPos("engine"), Vector(0, 90, 0));	
			}

			if (!m_ParticleFirst && MemoryPointExists("waterdecal"))
			{
				m_ParticleFirst = Particle.PlayOnObject(ParticleList.EXPANSION_BOAT_DECAL, this, GetMemoryPointPos("waterdecal") + Vector(0, 0, 2.5), Vector(-180, 0, 0));	
			}

			if (!m_ParticleSideFirst && MemoryPointExists("waterstream1_side"))
			{
				m_ParticleSideFirst = Particle.PlayOnObject(ParticleList.EXPANSION_BOAT_WATER, this, GetMemoryPointPos("waterstream1_side"), Vector(-25, 90, 0));	
			}

			if (!m_ParticleSideSecond && MemoryPointExists("waterstream2_side"))
			{
				m_ParticleSideSecond = Particle.PlayOnObject(ParticleList.EXPANSION_BOAT_WATER, this, GetMemoryPointPos("waterstream2_side"), Vector(25, 90, 0));	
			}
		} else 
		{
			if ( m_ParticleEngine )
			{
				m_ParticleEngine.Stop();
			}

			if ( m_ParticleFirst ) 
			{
				m_ParticleFirst.Stop();
			}
				
			if ( m_ParticleSecond ) 
			{
				m_ParticleSecond.Stop();
			}	

			if ( m_ParticleSideFirst ) 
			{
				m_ParticleSideFirst.Stop();
			}
				
			if ( m_ParticleSideSecond ) 
			{
				m_ParticleSideSecond.Stop();
			}
		}
	}

	// ------------------------------------------------------------
	protected override void OnAnimationUpdate( float pDt )
	{
		m_RotorAnimationPosition += m_Thrust * 10 * 0.0001;

		if ( m_RotorAnimationPosition >= 1 )
			m_RotorAnimationPosition -= 1;

		SetAnimationPhase("rotor", m_RotorAnimationPosition );

		SetAnimationPhase( "compasspointer", GetOrientation()[0] * Math.DEG2RAD );

		float steering = m_BoatController.GetTurnLeft() - m_BoatController.GetTurnRight();

		SetAnimationPhase( "drivingWheel", steering );

		super.OnAnimationUpdate( pDt );
	}

	protected override void OnSimulation( float pDt, out vector force, out vector torque )
	{
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

		// To whomever reads this, this will be re-written

		if ( !MotorIsOn() )
		{
			m_TurnTarget = 0;
			m_ThrustTarget = 0;
		}

		vector tForce			= vector.Zero;
		vector tCenter			= vector.Zero;

		float change;

		change = m_ThrustTarget - m_Thrust;
		m_Thrust += Math.Clamp( change, -1.0 * pDt, 1.0 * pDt );
		m_Thrust = Math.Clamp( m_Thrust, -1.0, 1.0 );

		change = ( m_TurnTarget * m_Thrust ) - m_Turn;
		m_Turn += Math.Clamp( change, -1.0 * pDt, 1.0 * pDt );
		m_Turn = Math.Clamp( m_Turn, -1.0, 1.0 );

		float linVel = 0;
		if ( m_MaxSpeedMS > 0 )
			linVel = m_LinearVelocityMS[2] * ( 1.0 / m_MaxSpeedMS );

		buoyancyForce = g_Game.CalculateBuoyancyAtPosition( GetPosition(), m_Offset, m_BodyMass, 0.5, m_LinearVelocity, isAboveWater );

		float waterContactCoef = Math.Clamp( MathHelper.Sign( buoyancyForce ), 0, 1 );

		// thrust
		if ( buoyancyForce > 0 )
		{
			float thrust = ( 3.0 * m_Thrust ) - ( 2.0 * linVel );

			tForce[0] = 0;
			tForce[1] = 0;
			tForce[2] = thrust * m_BodyMass;

			float linVelAbs = Math.AbsFloat( linVel );
			float thrustAbs = Math.AbsFloat( m_Thrust );

			float thrustCoef = 4.0 * linVelAbs;
			thrustCoef = Math.Clamp( thrustCoef, 0.0, 4.0 );

			float turnCoef = thrustCoef * thrustCoef * m_TurnCoef;
			tCenter[0] = m_Turn * m_BoundingRadius * turnCoef;
			tCenter[1] = -4.0;
			tCenter[2] = -m_BoundingRadius;

			force += tForce;
			torque += tCenter * tForce;
		}

		// friction
		if ( buoyancyForce > 0 )
		{
			force += GetLinearFrictionForce();
		}
		
		// convert forces to worldspace
		{
			force = force.Multiply3( m_Transform.GetBasis().data );
			torque = torque.Multiply3( m_Transform.GetBasis().data );
		}

		// bouyancy forces
		if ( buoyancyForce > 0 )
		{
			tForce[0] = 0;
			tForce[1] = buoyancyForce;
			tForce[2] = 0;

			force += tForce;
		}

		// stabilize
		if ( buoyancyForce > 0 )
		{
			// https://www.youtube.com/watch?v=weUDuqA6dF4?t=9
			vector upWanted = Vector( 0, 1, 0 );
			vector estOrient = GetEstimatedOrientation( 0.025 ).data[1];

			vector stabilize = vector.Zero;
			stabilize[0] = upWanted[0] - estOrient[0];
			stabilize[1] = 0;
			stabilize[2] = upWanted[2] - estOrient[2];

			// convert to local space
			stabilize = stabilize.InvMultiply3( m_Transform.GetBasis().data );

			// in local space, limit the axis of movement
			stabilize[0] = Math.Clamp( stabilize[0], -0.06, 0.06 );
			stabilize[2] = Math.Clamp( stabilize[2], -0.3, 0.3 );

			// convert to world space
			stabilize = stabilize.Multiply3( m_Transform.GetBasis().data );

			// apply 800N*mass of torque to keep the ship upright, and then copy this to bike scripts
			torque += Vector( 0, 400.0 * m_BodyMass, 0 ) * stabilize * waterContactCoef;
		}

		dBodySetDamping( this, 0.0, 0.5 );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBoatScript::UpdatePhysics end");
		#endif
	}

	private vector GetLinearFrictionForce()
	{
		vector friction = vector.Zero;

		friction[0] = -MathHelper.SquareSign( m_LinearVelocityMS[0] ) * m_BodyMass * 0.8;
		friction[1] = -MathHelper.SquareSign( m_LinearVelocityMS[1] ) * m_BodyMass * 0.8;

		friction[2] = -MathHelper.SquareSign( m_LinearVelocityMS[2] ) * m_BodyMass * 0.0001;

		return friction;
	}

	override void ExpansionOnExplodeClient( int damageType, string ammoType )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBoatScript::ExpansionOnExplodeClient start");
		#endif
		super.ExpansionOnExplodeClient( damageType, ammoType );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBoatScript::ExpansionOnExplodeClient end");
		#endif
	}

	override void ExpansionOnSpawnExploded()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBoatScript::ExpansionOnSpawnExploded - Start");
		#endif

		super.ExpansionOnSpawnExploded();

		if (m_ParticleEngine)
			m_ParticleEngine.Stop();
		
		if (m_ParticleFirst) 
			m_ParticleFirst.Stop();

		if (m_ParticleSecond) 
			m_ParticleSecond.Stop();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBoatScript::ExpansionOnSpawnExploded - End");
		#endif
	}

	// ------------------------------------------------------------
	override void EOnInit( IEntity other, int extra )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBoatScript::EOnInit start");
		#endif

		DisablePhysics( this );

		vector position = GetPosition();
	
		if ( g_Game.SurfaceIsSea( position[0], position[2] ) )
		{
			position[1] = g_Game.SurfaceGetSeaLevel() + m_Offset;
			SetPosition( position );
		} else if ( g_Game.SurfaceIsPond( position[0], position[2] ) )
		{
			float depth = g_Game.GetWaterDepth( position ) + m_Offset;
			position[1] = position[1] + depth;
			SetPosition( position );
		}

		SetOrientation( "0 0 0" );

		EnablePhysics( this );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBoatScript::EOnInit end");
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
	protected override bool CanSimulate()
	{
		return true;
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

	/**
	 * @brief This updates the sound for the boat.
	 * 
	 * @param ctrl, sound control (in config) which will be updated
	 * @param oldValue, engine defined value for the sound control
	 */
	float OnSoundBoat( CarSoundCtrl ctrl, float oldValue )
	{
		switch ( ctrl )
		{
			case CarSoundCtrl.RPM:
			{
				float speed = GetSpeedometer();
				if ( speed > 100 )
				{
					return 50;
				}
				else
				{
					return speed / 2;
				}
				
				break;
			}
			
			case CarSoundCtrl.ENGINE:
			{
				if ( MotorIsOn() )
				{
					return 1;
				}
				else 
				{
					return 0;
				}
			}
		}

		return oldValue;
	}
	
	/**
	 * @brief This updates the sound for the car.
	 * 
	 * @param ctrl, sound control (in config) which will be updated
	 * @param oldValue, engine defined value for the sound control
	 */
	override float OnSound( CarSoundCtrl ctrl, float oldValue )
	{
		if ( EngineIsOn() )
		{
			return super.OnSound( ctrl, oldValue );
		} 
		else if ( MotorIsOn() )
		{
			switch ( ctrl )
			{
				case CarSoundCtrl.RPM:
				{
					float speed = GetSpeedometer();
					if ( speed > 100 )
					{
						return 50;
					}
					else
					{
						return speed / 2;
					}
				
					break;
				}
			
				case CarSoundCtrl.ENGINE:
				{
					if ( MotorIsOn() || EngineIsOn() )
					{
						return 1;
					}
					else 
					{
						return 0;
					}
				}
			}

			return oldValue;
		}

		return super.OnSound( ctrl, oldValue );
	}

	// ------------------------------------------------------------
	override CarLightBase CreateRearLight()
	{
		return CarRearLightBase.Cast( ScriptedLightBase.CreateLight( ExpansionRearBoatLights ) );
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
	override bool IsCar()
	{
		return false;
	}

	// ------------------------------------------------------------
	override bool IsBoat()
	{
		return true;
	}

	// ------------------------------------------------------------
	override float GetCameraHeight()
	{
		return 1.5;
	}

	// ------------------------------------------------------------
	override float GetCameraDistance()
	{
		return 12;
	}
}