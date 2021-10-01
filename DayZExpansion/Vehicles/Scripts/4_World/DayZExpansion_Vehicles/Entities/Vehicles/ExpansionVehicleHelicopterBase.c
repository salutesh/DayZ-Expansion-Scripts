/**
 * ExpansionVehicleHelicopterBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/
	
/**@class		ExpansionVehicleHelicopterBase
 * @brief		This class handle helicopter movement and physics
 **/
class ExpansionVehicleHelicopterBase extends ExpansionVehicleBase
{
	// ------------------------------------------------------------
	//! Constant Values - Set in Constructor, Errors occur if not.
	// ------------------------------------------------------------

	// the rotor speeds for the drag of the helicopter when going down while the engine is turned off
	protected float m_MinAutoRotateSpeed; // (m/s)
	protected float m_MaxAutoRotateSpeed; // (m/s)

	// Coefficient for cyclic control
	protected float m_BodyFrictionCoef;
	protected float m_LiftForceCoef;

	// Coefficient for cyclic control
	protected float m_CyclicForceCoef;

	// the rotation at which the forward cyclic moves (pitch)
	protected float m_CyclicForwardSpeed; // (m/s) change per tick
	protected float m_CyclicForwardMax; // (m) per tick
	protected float m_CyclicForwardCoef; // Coefficient for forward cyclic

	// the rotation at which the side cyclic moves (roll)
	protected float m_CyclicSideSpeed; // (m/s)
	protected float m_CyclicSideMax; // (m)
	protected float m_CyclicSideCoef; // Coefficient for side cyclic

	// the rotation at which the anti torque moves (yaw)
	protected float m_AntiTorqueSpeed; // (m/s)
	protected float m_AntiTorqueMax; // (m)

	protected float m_BankForceCoef = 0.7;
	protected float m_TailForceCoef = 8.0;

	// the time it takes for the engine to startup
	protected float m_EngineStartDuration; // (s)

	protected vector m_LinearFrictionCoef;
	protected float m_AngularFrictionCoef;

	// Lift factor (taken from Arma 2 config)
	private int m_MaxFlightEnvelope = 15;
	private float m_FlightEnvelope[15] = { 0.0, 0.2, 0.9, 2.1, 2.5, 3.3, 3.5, 3.6, 3.7, 3.8, 3.8, 3.0, 0.9, 0.7, 0.5 };

	// ------------------------------------------------------------
	//! Member values
	// ------------------------------------------------------------
	private float m_RotorSpeed;
	private float m_RotorSpeedTarget;

	private float m_MainRotorSpeed;
	private float m_MainRotorSpeedTarget;

	private float m_BackRotorSpeed;
	private float m_BackRotorSpeedTarget;

	private float m_TailRotateFactor;

	private float m_Bank;

	private float m_CyclicForward;
	private float m_CyclicForwardTarget;

	private float m_CyclicSide;
	private float m_CyclicSideTarget;

	private float m_AutoHoverAltitude;
	private bool m_AutoHover;
	private bool m_IsFreeLook = true;
	private bool m_WasFreeLookPressed;
	private vector m_AutoHoverSpeed;
	private vector m_AutoHoverSpeedTarget;

	private bool m_Hit;
	private bool m_HitDetermined;
	private Object m_HitObject;
	private vector m_HitPosition;
	private vector m_HitNormal;
	private float m_HitFraction;

	private vector m_WindSpeedSync;
	private bool m_EnableWind;
	private Object m_WindDebugObject;

	private bool m_EnableTailRotorDamage;
	private bool m_EnableHelicopterExplosions;

	// ------------------------------------------------------------
	//! Particles
	// ------------------------------------------------------------
	private Particle m_DustParticle;
	private Particle m_WaterParticle;

	// ------------------------------------------------------------
	//! Animations
	// ------------------------------------------------------------
	private float m_RotorAnimationPosition;
	
	private autoptr NoiseParams m_NoiseParams;
		
	// ------------------------------------------------------------
	void ExpansionVehicleHelicopterBase()
	{
		RegisterNetSyncVariableFloat( "m_RotorSpeed" );
		
		m_NoiseParams = new NoiseParams();
		m_NoiseParams.Load("HeliExpansionNoise");

		string path;
		
		path = "CfgVehicles " + GetType() + " SimulationModule maxSpeed";
		if ( GetGame().ConfigIsExisting( path ) )
			m_MaxSpeed = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + GetType() + " SimulationModule altitudeFullForce";
		if ( GetGame().ConfigIsExisting( path ) )
			m_AltitudeFullForce = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + GetType() + " SimulationModule altitudeNoForce";
		if ( GetGame().ConfigIsExisting( path ) )
			m_AltitudeNoForce = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + GetType() + " SimulationModule bodyFrictionCoef";
		if ( GetGame().ConfigIsExisting( path ) )
			m_BodyFrictionCoef = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + GetType() + " SimulationModule liftForceCoef";
		if ( GetGame().ConfigIsExisting( path ) )
			m_LiftForceCoef = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + GetType() + " SimulationModule bankForceCoef";
		if ( GetGame().ConfigIsExisting( path ) )
			m_BankForceCoef = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + GetType() + " SimulationModule tailForceCoef";
		if ( GetGame().ConfigIsExisting( path ) )
			m_TailForceCoef = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + GetType() + " SimulationModule linearFrictionCoef";
		if ( GetGame().ConfigIsExisting( path ) )
			m_LinearFrictionCoef = GetGame().ConfigGetVector( path );
		
		path = "CfgVehicles " + GetType() + " SimulationModule angularFrictionCoef";
		if ( GetGame().ConfigIsExisting( path ) )
			m_AngularFrictionCoef = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + GetType() + " SimulationModule Rotor minAutoRotateSpeed";
		if ( GetGame().ConfigIsExisting( path ) )
			m_MinAutoRotateSpeed = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + GetType() + " SimulationModule Rotor maxAutoRotateSpeed";
		if ( GetGame().ConfigIsExisting( path ) )
			m_MaxAutoRotateSpeed = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + GetType() + " SimulationModule Rotor startUpTime";
		if ( GetGame().ConfigIsExisting( path ) )
			m_EngineStartDuration = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + GetType() + " SimulationModule AntiTorque speed";
		if ( GetGame().ConfigIsExisting( path ) )
			m_AntiTorqueSpeed = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + GetType() + " SimulationModule AntiTorque max";
		if ( GetGame().ConfigIsExisting( path ) )
			m_AntiTorqueMax = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + GetType() + " SimulationModule Cyclic forceCoefficient";
		if ( GetGame().ConfigIsExisting( path ) )
			m_CyclicForceCoef = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + GetType() + " SimulationModule Cyclic Forward speed";
		if ( GetGame().ConfigIsExisting( path ) )
			m_CyclicForwardSpeed = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + GetType() + " SimulationModule Cyclic Forward max";
		if ( GetGame().ConfigIsExisting( path ) )
			m_CyclicForwardMax = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + GetType() + " SimulationModule Cyclic Forward coefficient";
		if ( GetGame().ConfigIsExisting( path ) )
			m_CyclicForwardCoef = GetGame().ConfigGetFloat( path );
		
		//path = "CfgVehicles " + GetType() + " SimulationModule Cyclic Forward animation";
		//if ( GetGame().ConfigIsExisting( path ) )
		//	m_Inertia = GetGame().ConfigGetTextOut( path );
		
		path = "CfgVehicles " + GetType() + " SimulationModule Cyclic Side speed";
		if ( GetGame().ConfigIsExisting( path ) )
			m_CyclicSideSpeed = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + GetType() + " SimulationModule Cyclic Side max";
		if ( GetGame().ConfigIsExisting( path ) )
			m_CyclicSideMax = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + GetType() + " SimulationModule Cyclic Side coefficient";
		if ( GetGame().ConfigIsExisting( path ) )
			m_CyclicSideCoef = GetGame().ConfigGetFloat( path );
		
		//path = "CfgVehicles " + GetType() + " SimulationModule Cyclic Side animation";
		//if ( GetGame().ConfigIsExisting( path ) )
		//	m_Inertia = GetGame().ConfigGetTextOut( path );
	}

	// ------------------------------------------------------------
	void ~ExpansionVehicleHelicopterBase()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleHelicopterBase::Destructor - Start");
		#endif
		
		if ( IsMissionClient() )
		{
			if ( m_DustParticle )
			{
				m_DustParticle.Stop();
			}	

			if ( m_WaterParticle )
			{
				m_WaterParticle.Stop();
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleHelicopterBase::Destructor - End");
		#endif
	}

	// ------------------------------------------------------------
	override void OnAfterLoadConstantVariables()
	{
		super.OnAfterLoadConstantVariables();

		m_CyclicSideCoef = 0.6 * m_CyclicSideCoef;
		m_CyclicForwardCoef = -1.6 * m_CyclicForwardCoef;

		if ( m_EngineStartDuration <= 0 )
		{
			m_EngineStartDuration = 0.0;
		} else
		{
			m_EngineStartDuration = 1.0 / m_EngineStartDuration;
		}
	}

	// ------------------------------------------------------------
	override void OnSettingsUpdated()
	{
		super.OnSettingsUpdated();

		m_EnableWind = GetExpansionSettings().GetVehicle().EnableWindAerodynamics;
		m_EnableTailRotorDamage = GetExpansionSettings().GetVehicle().EnableTailRotorDamage;
		m_EnableHelicopterExplosions = GetExpansionSettings().GetVehicle().EnableHelicopterExplosions;
	}

	bool IsAutoHover()
	{
		return m_AutoHover;
	}

	void SwitchAutoHover()
	{
		m_AutoHover = !m_AutoHover;

		if ( IsAutoHover() )
		{
			m_AutoHoverAltitude = m_State.EstimatePosition( 2.0 )[1];
		}
	}

	override bool IsFreeLook()
	{
		return m_IsFreeLook;
	}

	override void SwitchGear()
	{
		Error( "Not implemented!" );
	}

	private float GetLiftFactor()
	{
		int iMaxFlightEnvelope = m_MaxFlightEnvelope - 1;
		float fHoriSpeedRel = Vector( m_State.m_LinearVelocityMS[0], 0, m_State.m_LinearVelocityMS[2] ).Length() / ( m_MaxSpeedMS * ( iMaxFlightEnvelope / 10.0 ) );
		float fCurrentEnvelope = iMaxFlightEnvelope * fHoriSpeedRel;
		int iCurrentEnvelopeFloor = Math.Floor( fCurrentEnvelope );

		if ( iCurrentEnvelopeFloor >= iMaxFlightEnvelope )
			return m_FlightEnvelope[ iMaxFlightEnvelope ];
		else if ( iCurrentEnvelopeFloor < 0 )
			return 0;

		float coef = 1.0;
		//if ( iCurrentEnvelopeFloor == 0 )
		//	coef = 0.01;

		float fCurrentEnvelopeFloor = m_FlightEnvelope[ iCurrentEnvelopeFloor ];
		float fCurrentEnvelopeFloorNext = m_FlightEnvelope[ iCurrentEnvelopeFloor + 1 ];
		return Math.Lerp( fCurrentEnvelopeFloor, fCurrentEnvelopeFloorNext, fCurrentEnvelope - iCurrentEnvelopeFloor ) * coef;
	}

	override void OnHandleController(DayZPlayerImplement driver, float dt)
	{
		UAInterface input = driver.GetInputInterface();

		float m_c_forward;
		float m_c_backward;
		float m_c_left;
		float m_c_right;

		#ifdef COMPONENT_SYSTEM
		if (IsMissionClient())
		{
			if (GetExpansionClientSettings().UseHelicopterMouseControl)
			{
				bool isFreelook = input.SyncedValue("UAExpansionHeliFreeLook");

				if (isFreelook && !m_WasFreeLookPressed)
				{
					m_WasFreeLookPressed = true;
					m_IsFreeLook = !m_IsFreeLook;
				}
				else if (!isFreelook)
				{
					m_WasFreeLookPressed = false;
				}
			}
			else
			{
				m_IsFreeLook = true;
			}

			if (!IsFreeLook())
			{
				//! Mouse control

				if (GetExpansionClientSettings().UseInvertedMouseControl)
				{
					m_c_forward = input.SyncedValue("UAAimUp");
					m_c_backward = input.SyncedValue("UAAimDown");
				}
				else
				{
					m_c_forward = input.SyncedValue("UAAimDown");
					m_c_backward = input.SyncedValue("UAAimUp");
				}

				m_c_left = input.SyncedValue("UAAimLeft");
				m_c_right = input.SyncedValue("UAAimRight");
			}
		}

		float c_up = input.SyncedValue("UAExpansionHeliCollectiveUp");
		float c_down = input.SyncedValue("UAExpansionHeliCollectiveDown");

		float at_left = input.SyncedValue("UAExpansionHeliAntiTorqueLeft");
		float at_right = input.SyncedValue("UAExpansionHeliAntiTorqueRight");

		float c_forward = input.SyncedValue("UAExpansionHeliCyclicForward");
		float c_backward = input.SyncedValue("UAExpansionHeliCyclicBackward");

		float c_left = input.SyncedValue("UAExpansionHeliCyclicLeft");
		float c_right = input.SyncedValue("UAExpansionHeliCyclicRight");
		#else
		if (IsMissionClient())
		{
			if (GetExpansionClientSettings().UseHelicopterMouseControl)
			{
				bool isFreelook = input.SyncedValue_ID(UAExpansionHeliFreeLook);

				if (isFreelook && !m_WasFreeLookPressed)
				{
					m_WasFreeLookPressed = true;
					m_IsFreeLook = !m_IsFreeLook;
				}
				else if (!isFreelook)
				{
					m_WasFreeLookPressed = false;
				}
			}
			else
			{
				m_IsFreeLook = true;
			}

			if (!IsFreeLook())
			{
				//! Mouse control

				if (GetExpansionClientSettings().UseInvertedMouseControl)
				{
					m_c_forward = input.SyncedValue_ID(UAAimUp);
					m_c_backward = input.SyncedValue_ID(UAAimDown);
				}
				else
				{
					m_c_forward = input.SyncedValue_ID(UAAimDown);
					m_c_backward = input.SyncedValue_ID(UAAimUp);
				}

				m_c_left = input.SyncedValue_ID(UAAimLeft);
				m_c_right = input.SyncedValue_ID(UAAimRight);
			}
		}

		float c_up = input.SyncedValue_ID(UAExpansionHeliCollectiveUp);
		float c_down = input.SyncedValue_ID(UAExpansionHeliCollectiveDown);

		float at_left = input.SyncedValue_ID(UAExpansionHeliAntiTorqueLeft);
		float at_right = input.SyncedValue_ID(UAExpansionHeliAntiTorqueRight);

		float c_forward = input.SyncedValue_ID(UAExpansionHeliCyclicForward);
		float c_backward = input.SyncedValue_ID(UAExpansionHeliCyclicBackward);

		float c_left = input.SyncedValue_ID(UAExpansionHeliCyclicLeft);
		float c_right = input.SyncedValue_ID(UAExpansionHeliCyclicRight);
		#endif

		if (IsMissionClient() && !IsFreeLook())
		{
			c_forward += m_c_forward * GetExpansionClientSettings().HelicopterMouseVerticalSensitivity;
			c_backward += m_c_backward * GetExpansionClientSettings().HelicopterMouseVerticalSensitivity;

			c_left += m_c_left * GetExpansionClientSettings().HelicopterMouseHorizontalSensitivity;
			c_right += m_c_right * GetExpansionClientSettings().HelicopterMouseHorizontalSensitivity;
		}

		m_BackRotorSpeedTarget = at_left - at_right;

		if ( IsAutoHover() )
		{
			if ( IsMissionClient() )
			{
				float autoHoverHeight = m_AutoHoverAltitude;
				float autoHoverChange = c_up - c_down;
				
				autoHoverHeight += autoHoverChange * 0.3;

				float surfaceY = GetGame().SurfaceY( GetPosition()[0], GetPosition()[2] );
				if ( m_Hit )
					surfaceY = m_HitPosition[1];

				autoHoverHeight -= surfaceY;

				if ( m_State.m_LinearVelocity[2] > 11 )
					if ( autoHoverHeight < 11 )
						autoHoverHeight = 11;

				if ( autoHoverHeight < -0.2 )
					autoHoverHeight = 0.2;

				autoHoverHeight += surfaceY;

				m_AutoHoverAltitude = autoHoverHeight;

				float change;
				float fSpd = c_forward - c_backward;
				float sSpd = c_right - c_left;

				float sSpdMult = m_MaxSpeedMS * 0.7;
				float fSpdMult = m_MaxSpeedMS;

				m_AutoHoverSpeedTarget[0] = sSpd * sSpdMult;
				m_AutoHoverSpeedTarget[2] = fSpd * fSpdMult;

				change = Math.Clamp( m_AutoHoverSpeedTarget[0] - m_AutoHoverSpeed[0], -sSpdMult * 0.5 * dt, sSpdMult * 0.5 * dt );
				m_AutoHoverSpeed[0] = Math.Clamp( m_AutoHoverSpeed[0] + change, -sSpdMult, sSpdMult );

				change = Math.Clamp( m_AutoHoverSpeedTarget[2] - m_AutoHoverSpeed[2], -fSpdMult * 0.5 * dt, fSpdMult * 0.5 * dt );
				m_AutoHoverSpeed[2] = Math.Clamp( m_AutoHoverSpeed[2] + change, -fSpdMult, fSpdMult );
			}

			m_CyclicForwardTarget = 0.0;
			m_CyclicSideTarget = 0.0;
		} else
		{
			if (IsMissionClient())
			{
				m_CyclicForwardTarget = c_forward - c_backward;
				m_CyclicSideTarget = c_right - c_left;
			}

			m_AutoHoverAltitude		= GetPosition()[1];
			m_AutoHoverSpeed		= "0 0 0";

			float mainRotorInput	= c_up	- c_down;
			m_MainRotorSpeedTarget	= Math.Clamp( mainRotorInput, -0.25 * dt, 0.25 * dt ) + m_MainRotorSpeed;
		}
	}

	// ------------------------------------------------------------
	protected override void OnPreSimulation( float pDt )
	{
		m_HitDetermined = false;

		PerformGroundRaycast();

		super.OnPreSimulation( pDt );

		if ( m_IsPhysicsHost )
		{
			if ( IsMissionClient() && m_EnableWind )
			{
				m_WindSpeedSync = GetGame().GetWeather().GetWind();

				if ( m_Hit )
				{

					float distance = 0;

					if ( ExpansionStatic.SurfaceIsWater( m_HitPosition ) )
					{
						distance = GetPosition()[1] - GetGame().SurfaceGetSeaLevel();
					} else
					{
						distance = GetPosition()[1] - m_HitPosition[1];
					}

					distance /= ( ( m_BoundingRadius * 1.5 ) + 10.0 );
					m_WindSpeedSync = m_WindSpeedSync * distance;
				} else
				{
					//float rnd = fad.m_Player.GetRandomGeneratorSyncManager().GetRandom01(RandomGeneratorSyncUsage.RGSGeneric);

					//m_HasDriver

					// turbulence?
				}

#ifdef EXPANSION_HELI_WIND_DEBUG
				if ( !m_WindDebugObject )
				{
					m_WindDebugObject = GetGame().CreateObject( "ExpansionDebugBox_Red", "0 0 0", true );
					AddChild( GetGame().CreateObject( "ExpansionDebugBox", "0 0 0", true ), -1 );
					AddChild( m_WindDebugObject, -1 );
				}
#endif
			} else
			{
				m_WindSpeedSync = "0 0 0";
			}
		}
	}

	// ------------------------------------------------------------
	protected override void OnPostSimulation( float pDt )
	{
		super.OnPostSimulation( pDt );
	}

	// ------------------------------------------------------------
	private void PerformGroundRaycast()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionVehicleHelicopterBase::PerformGroundRaycast - Start" );
		#endif
		
		if ( m_HitDetermined )
			return;
		
		float modelSize = m_BoundingRadius * 1.5;
		modelSize += 10.0;

		vector start = GetPosition();
		vector end = GetPosition() - Vector( 0, modelSize, 0 );

		PhxInteractionLayers collisionLayerMask = PhxInteractionLayers.BUILDING|PhxInteractionLayers.DOOR|PhxInteractionLayers.ROADWAY|PhxInteractionLayers.TERRAIN|PhxInteractionLayers.ITEM_SMALL|PhxInteractionLayers.ITEM_LARGE|PhxInteractionLayers.FENCE;
		
		m_Hit = false;
		if ( dBodyIsDynamic( this ) )
			m_Hit = DayZPhysics.SphereCastBullet( start, end, 5.0, collisionLayerMask, this, m_HitObject, m_HitPosition, m_HitNormal, m_HitFraction );
		
		m_HitDetermined = true;

		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionVehicleHelicopterBase::PerformGroundRaycast - End" );
		#endif
	}

	// ------------------------------------------------------------
	protected override void OnParticleUpdate( float pDt )
	{
		if ( m_Exploded )
		{
			if ( m_DustParticle )
			{
				m_DustParticle.Stop();
			}

			if ( m_WaterParticle )
			{
				m_WaterParticle.Stop();
			}

			return;
		}

		if ( m_Hit && m_RotorSpeed > 0 )
		{
			float size = ( 1 - m_HitFraction ) * m_RotorSpeed * 4.0 * m_BoundingRadius;

			if ( ExpansionStatic.SurfaceIsWater( m_HitPosition ) )
			{
				m_HitPosition[1] = GetGame().SurfaceGetSeaLevel();

				if ( !m_WaterParticle )
				{
					if ( size > 10 || !m_Exploded )
					{
						m_WaterParticle = Particle.CreateInWorld( ParticleList.EXPANSION_HELICOPTER_WATER, m_HitPosition );
					}
					
					if ( m_WaterParticle )
					{
						m_WaterParticle.PlayParticle();
					}
				} else
				{
					if ( m_WaterParticle )
					{
						m_WaterParticle.SetPosition( m_HitPosition );
					}
				}

				if ( m_DustParticle )
				{
					m_DustParticle.Stop();
				}
			} else
			{
				if ( !m_DustParticle )
				{
					if ( size > 10 || !m_Exploded )
					{
						m_DustParticle = Particle.CreateInWorld( ParticleList.EXPANSION_HELICOPTER_GROUND, m_HitPosition );
					}
					
					if ( m_DustParticle )
					{
						m_DustParticle.PlayParticle();
					}
				} else
				{
					if ( m_DustParticle )
					{
						m_DustParticle.SetPosition( m_HitPosition );
					}
				}

				if ( m_WaterParticle )
				{
					m_WaterParticle.Stop();
				}
			}
		} else
		{
			if ( m_DustParticle )
			{
				m_DustParticle.Stop();
			}

			if ( m_WaterParticle )
			{
				m_WaterParticle.Stop();
			}
		}

		if ( size < 10 || m_Exploded )
		{
			if ( m_DustParticle )
			{
				m_DustParticle.Stop();
			}

			if ( m_WaterParticle )
			{
				m_WaterParticle.Stop();
			}
		}
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

		float n;

		m_RotorAnimationPosition += m_RotorSpeed * pDt * 20.0 / ( Math.PI * 2.0 );

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
			SetAnimationPhase( "rearrotor", m_RotorAnimationPosition );
		}

		SetAnimationPhase( "compasspointer", GetOrientation()[0] * Math.DEG2RAD );
		SetAnimationPhase( "attitudeDiveRTD", GetDirection()[1] );
		SetAnimationPhase( "attitudeBankRTD", GetOrientation()[2] / 360 );

		SetAnimationPhase( "cyclicForward", m_CyclicForwardTarget );
		SetAnimationPhase( "cyclicAside", -m_CyclicSideTarget );

		super.OnAnimationUpdate( pDt );
	}

	// ------------------------------------------------------------
	override void EOnContact( IEntity other, Contact extra ) 
	{
		if ( m_IsBeingTowed )
			return;

		if ( IsMissionHost() && m_EnableHelicopterExplosions && CanBeDamaged() )
		{
			vector transform[4];
			GetTransform( transform );

			//! Unfortunately GetTransform[1] sometimes returns "0.7 -0.7 0" sometimes on flat terrain "0 1 0"
			//! In game rendering does not show this behaviour and the helicopter appears to only translate, not rotate
			//! This is possibly a DayZ SA/Enfusion bug but it will need more testing. May also be the cause for some
			//! helicoper simulation weirdness when on the ground?
			
			float dot = vector.Dot( transform[1], vector.Up );
			float dotMO = dot - 1.0;
			
			const float maxVelocityMagnitude = 11.0; // ~40km/h
			float impulseRequired = m_State.m_Mass * maxVelocityMagnitude * ( ( dotMO * dotMO * dotMO ) + 1.0 ) * 40.0; 

			if ( other ) //! check done just incase
				impulseRequired += Math.Max( dBodyGetMass( other ), 0.0 ) * maxVelocityMagnitude * 2.0;


			if ( extra.Impulse > impulseRequired )
			{
				#ifdef EXPANSIONVEHICLELOG
				Print( dot );				
				Print( impulseRequired );				
				Print( other );	
				Print( extra.Impulse );				
				Print(GetVelocity(this));
				Print(dBodyGetAngularVelocity(this));
				#endif
				//Print( "Boom!" );
				//! Maybe instead just tick damage down instead?
				//! Should have a way to repair the helicopter then though
				Explode( DT_EXPLOSION, "RGD5Grenade_Ammo" );
			}
		}
	}

	// ------------------------------------------------------------
	override void ExpansionOnExplodeServer( int damageType, string ammoType )
	{
		#ifdef EXPANSIONVEHICLELOG
		Print( "+ExpansionVehicleHelicopterBase::ExpansionOnExplodeServer" );
		#endif

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleHelicopterBase::ExpansionOnExplodeServer - Start");
		#endif

		vector position = GetPosition();
		vector orientation = GetOrientation();

		//SetInvisible( true );

		EntityAI attachment;
		for ( int j = 0; j < GetInventory().AttachmentCount(); j++ )
		{	
			attachment = GetInventory().GetAttachmentFromIndex( j );

			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().ObjectDelete, 0, false, attachment );
		}

		#ifdef EXPANSIONVEHICLELOG
		Print( "+ExpansionVehicleHelicopterBase::ExpansionOnExplodeServer - Attachment Delete" );
		#endif

		PlayerBase player;
		DayZPlayerCommandDeathCallback callback;
		
		for ( int i = 0; i < CrewSize(); i++ )
		{
			if ( Class.CastTo( player, CrewMember( i ) ) )
			{
				player.StartCommand_Fall(0);
				
				CrewDeath( i );
				CrewGetOut( i );
				
				player.UnlinkFromLocalSpace();
				
				player.SetAllowDamage( true );
				player.SetHealth( 0.0 );
				
				dBodySetInteractionLayer(player, PhxInteractionLayers.RAGDOLL);
				
				RemoveChild(player);
			}
		}

		#ifdef EXPANSIONVEHICLELOG
		Print( "+ExpansionHelicopterScript::ExpansionOnExplodeServer - Crew Unlink" );
		#endif

		IEntity child = GetChildren();
		while ( child )
		{
			if ( Class.CastTo( player, child ) )
			{				
				child = child.GetSibling();
				
				player.UnlinkFromLocalSpace();
				RemoveChild(player);
				
				player.SetAllowDamage( true );
				player.SetHealth( 0.0 );
				
				dBodySetInteractionLayer(player, PhxInteractionLayers.RAGDOLL);
				
				player.StartCommand_Fall(0);
			} else
			{
				child = child.GetSibling();
			}
		}

		#ifdef EXPANSIONVEHICLELOG
		Print( "+ExpansionVehicleHelicopterBase::ExpansionOnExplodeServer - Player Unlink" );
		#endif

		ExpansionWreck wreck;
		if ( Class.CastTo( wreck, GetGame().CreateObjectEx( GetWreck(), position + "0 2.5 0", ECE_CREATEPHYSICS|ECE_UPDATEPATHGRAPH ) ) )
		{
			#ifdef EXPANSIONVEHICLELOG
			Print( "ExpansionVehicleHelicopterBase::ExpansionOnExplodeServer - Wreck Create" );
			#endif

			wreck.SetPosition( position + "0 2.5 0" );
			wreck.SetOrientation( orientation );

			wreck.CreateDynamicPhysics( PhxInteractionLayers.DYNAMICITEM );
			wreck.SetDynamicPhysicsLifeTime( 60 );
			wreck.EnableDynamicCCD( true );

			wreck.SetOffset( GetWreckOffset() );
			wreck.SetAltitude( GetWreckAltitude() );

			wreck.SetHealth( 0.0 );
			dBodySetMass( wreck, m_State.m_Mass );

			#ifdef EXPANSIONVEHICLELOG
			Print( "+ExpansionVehicleHelicopterBase::ExpansionOnExplodeServer - Mass Apply" );
			#endif

			vector inertiaM[3];
			dBodyGetInvInertiaTensorWorld( this, inertiaM );
			dBodySetInertiaTensorM( wreck, inertiaM );
			dBodySetInertiaTensorV( wreck, dBodyGetLocalInertia( this ) );

			#ifdef EXPANSIONVEHICLELOG
			Print( "+ExpansionVehicleHelicopterBase::ExpansionOnExplodeServer - Tensor Apply" );
			#endif

			SetVelocity( wreck, m_State.m_LinearVelocity );
			dBodySetAngularVelocity( wreck, m_State.m_AngularVelocity );

			dBodyApplyForce( wreck, m_State.m_LinearAcceleration * m_State.m_Mass );

			#ifdef EXPANSIONVEHICLELOG
			Print( "+ExpansionVehicleHelicopterBase::ExpansionOnExplodeServer - Self Delete" );
			#endif

			// GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( MiscGameplayFunctions.TransferInventory, 1, false, this, wreck, playerForTransfer );
		
			array<Object> objects = new array<Object>;
			array<CargoBase> proxy = new array<CargoBase>;
			GetGame().GetObjectsAtPosition( GetPosition(), 10, objects, proxy );

			for ( int n = 0; n < objects.Count(); ++n ) 
			{
				Object obj = objects[n];

				TreeHard treeHard;
				TreeSoft treeSoft;
				BushHard bushHard;
				BushSoft bushSoft;
				if ( Class.CastTo( treeHard, obj ) )
				{
					GetGame().RPCSingleParam( wreck, PlantType.TREE_HARD, new Param1< vector >( obj.GetPosition() ), true );
				} else if ( Class.CastTo( treeSoft, obj ) )
				{
					GetGame().RPCSingleParam( wreck, PlantType.TREE_SOFT, new Param1< vector >( obj.GetPosition() ), true );
				} else if ( Class.CastTo( bushHard, obj ) )
				{
					GetGame().RPCSingleParam( wreck, PlantType.BUSH_HARD, new Param1< vector >( obj.GetPosition() ), true );
				} else if ( Class.CastTo( bushSoft, obj ) )
				{
					GetGame().RPCSingleParam( wreck, PlantType.BUSH_SOFT, new Param1< vector >( obj.GetPosition() ), true );
				} else 
				{
					continue;
				}

				if ( obj.GetHealth( "", "" ) > 0 )
				{
					obj.SetHealth( "", "", 0 );

					if ( dBodyIsSet( obj ) )
						dBodyDestroy( obj );
						
					GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Call( GetGame().ObjectDelete, obj );
				}
			}

			#ifdef EXPANSIONVEHICLELOG
			Print( "+ExpansionVehicleHelicopterBase::ExpansionOnExplodeServer - Foliage Chop" );
			#endif

			super.ExpansionOnExplodeServer( damageType, ammoType );

			#ifdef EXPANSIONVEHICLELOG
			Print( "+ExpansionVehicleHelicopterBase::ExpansionOnExplodeServer - Super Call" );
			#endif

			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Call( GetGame().ObjectDelete, this );

			#ifdef EXPANSIONVEHICLELOG
			Print( "+ExpansionVehicleHelicopterBase::ExpansionOnExplodeServer - Self Delete" );
			#endif
		} else
		{
			super.ExpansionOnExplodeServer( damageType, ammoType );

			#ifdef EXPANSIONVEHICLELOG
			Print( "+ExpansionVehicleHelicopterBase::ExpansionOnExplodeServer - Super Call" );
			#endif
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleHelicopterBase::ExpansionOnExplodeServer - End");
		#endif	

		#ifdef EXPANSIONVEHICLELOG
		Print( "-ExpansionVehicleHelicopterBase::ExpansionOnExplodeServer" );
		#endif
	}

	// ------------------------------------------------------------
	override void ExpansionOnExplodeClient( int damageType, string ammoType )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleHelicopterBase::ExpansionOnExplodeClient - Start");
		#endif

		super.ExpansionOnExplodeClient( damageType, ammoType );

		if ( !IsMissionOffline() && GetGame().GetPlayer().GetParent() == this )
		{
			GetGame().GetPlayer().UnlinkFromLocalSpace();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleHelicopterBase::ExpansionOnExplodeClient - End");
		#endif
	}

	// ------------------------------------------------------------
	override void ExpansionOnSpawnExploded()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleHelicopterBase::ExpansionOnSpawnExploded - Start");
		#endif

		super.ExpansionOnSpawnExploded();
		
		if ( m_DustParticle )
		{
			m_DustParticle.Stop();
		}

		if ( m_WaterParticle )
		{
			m_WaterParticle.Stop();
		}	

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleHelicopterBase::ExpansionOnSpawnExploded - End");
		#endif
	}

	// ------------------------------------------------------------
	protected override void OnNetworkSend(  ParamsWriteContext ctx )
	{
		super.OnNetworkSend( ctx );

		ctx.Write(m_AutoHover);

		if ( IsAutoHover() )
		{
			ctx.Write( m_AutoHoverAltitude );
			ctx.Write( m_AutoHoverSpeed );
		} 
		else
		{
			ctx.Write(m_CyclicForwardTarget);
			ctx.Write(m_CyclicSideTarget);
		}

		ctx.Write( m_WindSpeedSync );
	}

	// ------------------------------------------------------------
	protected override void OnNetworkRecieve( ParamsReadContext ctx )
	{
		super.OnNetworkRecieve( ctx );

		ctx.Read(m_AutoHover);

		if ( IsAutoHover() )
		{
			ctx.Read( m_AutoHoverAltitude );
			ctx.Read( m_AutoHoverSpeed );
		}
		else
		{
			ctx.Read(m_CyclicForwardTarget);
			ctx.Read(m_CyclicSideTarget);
		}

		ctx.Read( m_WindSpeedSync );

		if ( !m_EnableWind )
			m_WindSpeedSync = "0 0 0";
	}

	private vector m_ExHeliForce;
	private vector m_ExHeliTorque;

	#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		super.CF_OnDebugUpdate(instance, type);

		instance.Add("Auto-Hover Height", m_AutoHoverAltitude );
		instance.Add("Auto-Hover Target Speed", m_AutoHoverSpeedTarget );
		instance.Add("Auto-Hover Speed", m_AutoHoverSpeed );

		instance.Add("Rotor Speed", m_RotorSpeed );
		instance.Add("Lift Force Coef", m_LiftForceCoef );
		
		instance.Add("Applying Force", m_ExHeliForce );
		instance.Add("Applying Torque", m_ExHeliTorque );

		return true;
	}
	#endif
	
	// ------------------------------------------------------------
	protected override void OnSimulation( float pDt, out vector force, out vector torque )
	{
		bool isAboveWater;
		float buoyancyForce;

		if ( m_Exploded )
		{
			// if the heli isn't over water no force will be applied and the game will clean up physics for us
			if ( m_WaterVolume < m_TotalVolume )
			{
				buoyancyForce = ExpansionPhysics.CalculateBuoyancyAtPosition( GetPosition(), 2.0, m_State.m_Mass, 2.0, m_State.m_LinearVelocity, isAboveWater );

				if ( !isAboveWater )
				{
					// slowly sink helicopter
					m_WaterVolume += 0.05 * pDt * m_WaterVolume;

					force += Vector( 0, buoyancyForce * ( m_TotalVolume - m_WaterVolume ) / m_TotalVolume, 0 );
				}
			}

			dBodySetDamping( this, 0.0, 0.0 );

			return;
		} else if ( EngineIsOn() )
		{
			if ( IsMissionHost() && m_NoiseParams )
			{
				GetGame().GetNoiseSystem().AddNoise( this, m_NoiseParams );
			}

			if ( !m_Hit && !m_HasDriver && !IsAutoHover() )
			{
				m_MainRotorSpeedTarget = Math.RandomFloatInclusive( -1, 1 );
				m_BackRotorSpeedTarget = Math.RandomFloatInclusive( -1, 1 );

				m_CyclicSideTarget = Math.RandomFloatInclusive( -1, 1 );
				m_CyclicForwardSpeed = Math.RandomFloatInclusive( -1, 1 );
			} else if ( IsAutoHover() )
			{
				if ( !m_HasDriver )
				{
					m_AutoHoverSpeed		= "0 0 0";
					m_AutoHoverSpeedTarget	= "0 0 0";
					m_CyclicForwardTarget 	= 0;
					m_CyclicSideTarget		= 0;
					m_BackRotorSpeedTarget	= 0;
				}

				float estT = 80.0 * pDt;
				vector estimatedPosition = m_State.EstimatePosition( estT );
				vector estimatedOrientation = m_State.EstimateOrientation( estT );
				vector targetOrientation = vector.Zero;
				
				m_MainRotorSpeedTarget = Math.Clamp( m_AutoHoverAltitude - estimatedPosition[1], -0.25 * pDt, 0.25 * pDt ) + m_MainRotorSpeed;

				if ( m_CyclicForwardTarget == 0 && m_CyclicSideTarget == 0 )
				{
					float forwardSpeed = m_State.m_LinearVelocityMS[2] - m_AutoHoverSpeed[2];
					float sideSpeed = m_State.m_LinearVelocityMS[0] - m_AutoHoverSpeed[0];

					forwardSpeed = Math.Clamp( forwardSpeed, -25.0, 25.0 );
					sideSpeed = Math.Clamp( sideSpeed, -45.0, 45.0 );

					float heightDiff = estimatedPosition[1] - m_AutoHoverAltitude;
					if ( heightDiff >= 20.0 && forwardSpeed < 0 )
						forwardSpeed *= 2.0;

					targetOrientation[1] = forwardSpeed;
					targetOrientation[2] = -sideSpeed;

					float targetPitchDiff = estimatedOrientation[1] - targetOrientation[1];
					float targetRollDiff = estimatedOrientation[2] - targetOrientation[2];

					m_CyclicForwardTarget = Math.Clamp( targetPitchDiff * 0.25, -1.0, 1.0 );
					m_CyclicSideTarget = Math.Clamp( targetRollDiff * 0.25, -1.0, 1.0 );
				}
			}
		} else
		{
			if ( m_HasDriver )
			{
				m_MainRotorSpeedTarget = 0;
				m_BackRotorSpeedTarget = 0;

				m_CyclicForwardTarget *= 0.25;
				m_CyclicSideTarget *= 0.25;
			} else
			{
				m_MainRotorSpeedTarget = 0;
				m_BackRotorSpeedTarget = 0;

				m_CyclicForwardTarget = 0;
				m_CyclicSideTarget = 0;

				// gravity can still take over if it's in the sky, or somehow dayz wheels activate randomly
				// this should still be fine though.
				if ( m_State.m_LinearVelocity.Length() < pDt && m_State.m_LastLinearVelocity.Length() < pDt )
					return;
			}
		}

		float change;

		// see the speed at which we are free falling
		float goingDown = Math.Clamp( ( -m_State.m_LinearVelocityMS[1] - m_MinAutoRotateSpeed ) / ( m_MaxAutoRotateSpeed - m_MinAutoRotateSpeed ), 0, 1 );
		float brakeRotor = Math.Max( Math.Max( m_MainRotorSpeed * 0.2, 0 ), -goingDown );

		// https://en.wikipedia.org/wiki/Autorotation (https://en.wikipedia.org/wiki/Autorotation#/media/File:Airflow_in_auto-2.jpg)
		// only in effect if the heli is falling down and the rotor is turn off
		if ( goingDown > 0 && m_RotorSpeedTarget < 0.1 )
		{
			change = ( Math.Min( goingDown, 1.0 ) - m_RotorSpeed ) * 0.08 * pDt;
		} else
		{
			change = m_RotorSpeedTarget - m_RotorSpeed;
		}

		change = Math.Clamp( change, ( -0.025 - ( brakeRotor * 0.35 ) ) * pDt, m_EngineStartDuration * pDt );
		m_RotorSpeed = Math.Clamp( m_RotorSpeed + change, 0, 1 );

		change = Math.Clamp( Math.Min( m_MainRotorSpeedTarget, m_RotorSpeed ) - m_MainRotorSpeed, -0.25 * pDt, 0.25 * pDt );
		m_MainRotorSpeed = Math.Clamp( m_MainRotorSpeed + change, -0.2, m_RotorSpeed );

		change = Math.Clamp( m_BackRotorSpeedTarget - m_BackRotorSpeed, -m_AntiTorqueSpeed * pDt, m_AntiTorqueSpeed * pDt );
		m_BackRotorSpeed = Math.Clamp( m_BackRotorSpeed + change, -m_AntiTorqueMax, m_AntiTorqueMax );

		if ( m_State.m_LinearVelocityMS.Length() > 0.05 || m_RotorSpeed != 0 )
		{
			change = Math.Clamp( Math.Clamp( m_CyclicForwardTarget, -2, 2 ) - m_CyclicForward, -m_CyclicForwardSpeed * pDt, m_CyclicForwardSpeed * pDt );
			m_CyclicForward = Math.Clamp( m_CyclicForward + change, -m_CyclicForwardMax, m_CyclicForwardMax );

			change = Math.Clamp( Math.Clamp( m_CyclicSideTarget, -2, 2 ) - m_CyclicSide, -m_CyclicSideSpeed * pDt, m_CyclicSideSpeed * pDt );
			m_CyclicSide = Math.Clamp( m_CyclicSide + change, -m_CyclicSideMax, m_CyclicSideMax );

			// collective
			{
				// rotorSpeed^2
				// so rotorSpeed=0.0, 0.0*0.0 = 0.0 | rotorSpeed=0.5, 0.5*0.5 = 0.25 | rotorSpeed=1.0, 1.0*1.0 = 1.0
				// rotorSpeed is always clamped between 0.0-1.0

				float nearGround = 1;
				if ( m_Hit )
				{
					nearGround = ( GetPosition()[1] - m_HitPosition[1] ) / m_BoundingRadius;
					nearGround = Math.Clamp( 1.5 - nearGround, 0.0, 1.0 );
					nearGround = ( nearGround * nearGround * 0.25 ) + 1.0;
				}

				float liftFactor = GetLiftFactor();

				float targetVelocity = ( m_State.m_LinearVelocityMS[1] + 3.0 ) - ( 18.0 * m_MainRotorSpeed * m_RotorSpeed * nearGround );
				if ( targetVelocity < -5 )
					targetVelocity = -5;

				targetVelocity *= pDt;
				float collectiveCoef = Math.Max( ( 1.3 * liftFactor ) - ( ( Math.SquareSign( targetVelocity ) * 5.0 ) + ( targetVelocity * 80.0 ) ), 0 );
				
				force += Vector( 0, 1, 0 ) * collectiveCoef * m_AltitudeLimiter * m_RotorSpeed * m_RotorSpeed * m_LiftForceCoef * m_State.m_Mass;
			}

			// cyclic
			{
				float cyclicForce = m_CyclicForceCoef * m_State.m_Mass * m_RotorSpeed * m_RotorSpeed * m_AltitudeLimiter;

				torque[0] = torque[0] - ( cyclicForce * m_CyclicForward * m_CyclicForwardCoef * m_BoundingRadius );
				torque[2] = torque[2] + ( cyclicForce * m_CyclicSide * m_CyclicSideCoef * m_BoundingRadius );
			}

			// bank
			{
				const float tailRotateSqCoef = 0.00048;
				const float tailRotateCoef = 0.012;
				m_TailRotateFactor = ( m_State.m_LinearVelocityMS[2] * m_State.m_LinearVelocityMS[2] * tailRotateSqCoef ) + ( Math.AbsFloat( m_State.m_LinearVelocityMS[2] ) * tailRotateCoef );

				float forwardX = m_State.m_Transform[2][0];
				float sideY = m_State.m_Transform[0][1];
				float forwardZ = m_State.m_Transform[2][2];

				if ( forwardX == 0.0 && forwardZ == 0.0 )
					m_Bank = Math.Sign( sideY );
				else
					m_Bank = sideY / ( ( forwardX * forwardX ) + ( forwardZ * forwardZ ) );
			}

			// tail
			{
				float tailRotorMalfunction = 0.0;
				if ( m_EnableTailRotorDamage ) 
					tailRotorMalfunction = GetHealthLevel() / 5.0; // GetHealthLevel( "TailRotor" ) / 5.0;

				float tailRotorMalfunctionNeg = 1.0 - tailRotorMalfunction;
				float tailRotorMalfunctionTorque = 0.5 * tailRotorMalfunction * m_RotorSpeed * ( m_RotorSpeed + 0.1 );

				const float maxSpeedTailEffect = 1.0 / 80.0; // at ~200km/h, tail rotor has no effect
				float scaledSpeedFactor = 1.0 - Math.Min( Math.AbsFloat( m_State.m_LinearVelocityMS[2] * maxSpeedTailEffect ), 1.0 );

				float bankForce = Math.Asin( m_Bank ) * m_BankForceCoef * m_TailRotateFactor;
				float tailRotorForce = m_BackRotorSpeed * m_TailForceCoef * tailRotorMalfunctionNeg;

				float tailForce = ( bankForce + tailRotorForce - tailRotorMalfunctionTorque ) * 0.5 * scaledSpeedFactor * m_RotorSpeed * m_RotorSpeed * m_BoundingRadius * m_State.m_Mass;

				// apply torque to change the heading of the heli
				torque[1] = torque[1] - ( m_BoundingRadius * tailForce );

				// apply a little bit of torque on the side of the heli to simulate some roll
				torque[2] = torque[2] - ( m_BoundingRadius * 0.01 * tailForce );
			}

			// rotate to the direction of the speed
			//TODO: back to the drawing board...
			{
				float heliSpeedY = m_State.m_LinearVelocityMS[1] * ( m_State.m_LinearVelocityMS[0] * 0.1 );

				float rotateX = m_TailRotateFactor * ( ( m_State.m_LinearVelocityMS[0] * -0.500 ) + ( Math.SquareSign( m_State.m_LinearVelocityMS[0] ) * -0.010 ) ) * m_State.m_Mass;
				float rotateY = m_TailRotateFactor * ( ( heliSpeedY * -0.005 ) + ( Math.SquareSign( heliSpeedY ) * -0.0003 ) ) * m_State.m_Mass;

				vector heliRotateDir = Vector( 0, 0, -m_BoundingRadius ) * Vector( rotateX, rotateY, 0 );

				torque[0] = torque[0] + heliRotateDir[0];
				torque[1] = torque[1] + heliRotateDir[1];
				torque[2] = torque[2] + heliRotateDir[2];
			}

			//! Linear Friction
			{
				vector friction;

				vector frictionSpeed = m_State.m_LinearVelocityMS;
				#ifdef EXPANSION_HELI_WIND
				vector windSpeed = m_WindSpeedSync.InvMultiply3( m_State.m_Transform );
				#else
				vector windSpeed = "0 0 0";
				#endif

				if ( m_WindDebugObject )
				{
					m_WindDebugObject.SetOrigin( windSpeed );
				}

				frictionSpeed += windSpeed;

				// if the helicopter is turned on then more force is applied to create psuedo-friction
				float stabilizeXY = 0.4 + ( m_RotorSpeed * m_RotorSpeed * 0.6 );

				friction[0] = Math.SquareSign( frictionSpeed[0] ) * pDt * m_LinearFrictionCoef[0] * m_State.m_Mass * stabilizeXY;
				friction[1] = Math.SquareSign( frictionSpeed[1] ) * pDt * m_LinearFrictionCoef[1] * m_State.m_Mass * stabilizeXY;
				friction[2] = Math.SquareSign( frictionSpeed[2] ) * pDt * m_LinearFrictionCoef[2] * m_State.m_Mass;

				force -= friction * m_BodyFrictionCoef;
			}

			//! convert forces to worldspace
			{
				force = force.Multiply3( m_State.m_Transform );
				torque = torque.Multiply3( m_State.m_Transform );
			}

			//! Angular Friction
			{
				vector t_friction;
				
				t_friction = m_State.m_AngularVelocity * m_BoundingRadius * ( m_RotorSpeed + 0.2 ) * ( m_AngularFrictionCoef + ( m_TailRotateFactor * 0.5 ) );
				
				torque -= t_friction;
			}
		}

		// prevent helicopter from sinking in ocean
		{
			buoyancyForce = ExpansionPhysics.CalculateBuoyancyAtPosition( GetPosition(), 2.0, m_State.m_Mass, 2.0, m_State.m_LinearVelocity, isAboveWater );
				
			if ( !isAboveWater && IsMissionHost() && CanBeDamaged() )
			{
				float buoyancyAcceleration = buoyancyForce * m_State.m_InvMass;
				if ( buoyancyAcceleration > 1.0 )
				{
					AddHealth( "", "", -0.001 * buoyancyForce );
				}

				if ( buoyancyAcceleration > 10.0 && m_EnableHelicopterExplosions )
				{
					Explode( DT_EXPLOSION, "RGD5Grenade_Ammo" );
				}
			}

			force += Vector( 0, buoyancyForce, 0 );
		}
		
		m_ExHeliForce = force;
		m_ExHeliTorque = torque;
	}

	// ------------------------------------------------------------
	override float OnSound( CarSoundCtrl ctrl, float oldValue )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleHelicopterBase::OnSound - Start");
		#endif

		if ( m_Exploded )
			return 0;

		switch ( ctrl )
		{
			case CarSoundCtrl.SPEED:
			{
				return m_RotorSpeed; // this should just be the velocity length, maybe in KM/h
			}
			case CarSoundCtrl.RPM:
			{
				return m_RotorSpeed;
			}
			case CarSoundCtrl.ENGINE:
			{
				if ( m_RotorSpeedTarget > 0 )
					return 1;

				return 0;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleHelicopterBase::OnSound - End");
		#endif

		return oldValue;
	}

	// ------------------------------------------------------------
	override void SetActions()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleHelicopterBase::SetActions - Start");
		#endif

		super.SetActions();

		AddAction( ExpansionActionSwitchAutoHover );
		AddAction( ExpansionActionSwitchAutoHoverInput );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleHelicopterBase::SetActions - End");
		#endif
	}

	// ------------------------------------------------------------
	override void OnEngineStop(int index)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleHelicopterBase::OnEngineStop - Start");
		#endif

		super.OnEngineStop(index);

		m_RotorSpeedTarget = 0;
 
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleHelicopterBase::OnEngineStop - End");
		#endif
	}

	// ------------------------------------------------------------
	override void OnEngineStart(int index)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleHelicopterBase::OnEngineStart - Start");
		#endif

		super.OnEngineStart(index);

		dBodyActive( this, ActiveState.ALWAYS_ACTIVE );
		dBodyDynamic( this, true );

		m_RotorSpeedTarget = 1;

		m_AutoHoverAltitude = GetPosition()[1];
		m_AutoHoverSpeed = m_State.m_LinearVelocityMS;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleHelicopterBase::OnEngineStart - End");
		#endif
	}

	// ------------------------------------------------------------
	override int GetAnimInstance()
	{
		return ExpansionVehicleAnimInstances.EX_HATCHBACK;
	}
	
	// ------------------------------------------------------------
	override int GetSeatAnimationType( int posIdx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleHelicopterBase::GetSeatAnimationType - Start");
		#endif

		switch( posIdx )
		{
		case 0:
			return DayZPlayerConstants.VEHICLESEAT_DRIVER;
		case 1:
			return DayZPlayerConstants.VEHICLESEAT_CODRIVER;
		default:
			return 0;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleHelicopterBase::GetSeatAnimationType - End");
		#endif

		return 0;
	}

	// ------------------------------------------------------------
	override bool CanReachSeatFromDoors( string pSeatSelection, vector pFromPos, float pDistance = 1.0 )
	{
		return true;
	}
	
	// ------------------------------------------------------------
	override int Get3rdPersonCameraType()
	{
		return DayZPlayerCameras.DAYZCAMERA_3RD_VEHICLE;
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
	override bool IsVitalHelicopterBattery()
	{
		return true;
	}
	
	// ------------------------------------------------------------
	override bool IsVitalCarBattery()
	{
		return false;
	}

	// ------------------------------------------------------------
	protected override bool CanSimulate()
	{
		//! Hack fix for heli smoke when it shouldn't until we have a better fix https://exp.thurston.pw/T524
		return ( dBodyIsActive( this ) && dBodyIsDynamic( this ) ) || ( m_DustParticle && m_DustParticle.IsPlaying() );
	}

	// ------------------------------------------------------------
	override bool IsVitalSparkPlug()
	{
		return true;
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
	override bool IsVitalIgniterPlug()
	{
		return false;
	}

	// ------------------------------------------------------------
	override bool IsVitalHydraulicHoses()
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
	override CarLightBase CreateFrontLight()
	{
		return CarLightBase.Cast( ScriptedLightBase.CreateLight( ExpansionHelicopterFrontLight ) );
	}

	// ------------------------------------------------------------
	override CarLightBase CreateRearLight()
	{
		return CarRearLightBase.Cast( ScriptedLightBase.CreateLight( ExpansionRearHelicopterLights ) );
	}

	// ------------------------------------------------------------
	override vector GetTransportCameraOffset()
	{
		return "0 5 0";
	}

	// ------------------------------------------------------------
	override float GetTransportCameraDistance()
	{
		return 15;
	}

	// ------------------------------------------------------------
	override bool IsHelicopter()
	{
		return true;
	}

	// ------------------------------------------------------------
	override bool IsCar()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	float GetAutoHoverTargetHeight()
	{
		return m_AutoHoverAltitude;
	}

	override vector GetTowCenterPosition( Object other )
	{
		vector minMax[2];
		GetCollisionBox( minMax );
		vector pos = Vector( 0.0, minMax[0][1] - GetTowLength() - GetTowExtents()[1], 0.0 );
		other.GetCollisionBox( minMax );
		return pos + Vector( 0.0, -minMax[1][1], 0.0 );
	}

	override bool IsBeingTowed()
	{
		return m_IsBeingTowed;
	}

	override bool IsTowing()
	{
		return m_IsTowing;
	}
	
	override vector GetTowPosition()
	{
		vector minMax[2];
		GetCollisionBox( minMax );

		return ModelToWorld( Vector( 0.0, minMax[0][1] - GetTowLength(), 0.0 ) );
	}

	override vector GetTowDirection()
	{
		vector transform[4];
		GetTransform( transform );
		return -transform[1];
	}

	override vector GetTowExtents()
	{
		return { 2.0, 2.0, 2.0 };
	}

	override float GetTowLength()
	{
		return 5.0;
	}

	//! Is it already towing something ? And is it locked ?
	override bool CanConnectTow( notnull Object other )
	{
		ExpansionVehicleBase evs;
		CarScript cs;
		if ( Class.CastTo( evs, other ) )
		{
			return !evs.IsTowing() && !evs.IsLocked();
		} else if ( Class.CastTo( cs, other ) )
		{
			return !cs.IsTowing() && !cs.IsLocked();
		}

		//! don't...
		return false;
	}
}