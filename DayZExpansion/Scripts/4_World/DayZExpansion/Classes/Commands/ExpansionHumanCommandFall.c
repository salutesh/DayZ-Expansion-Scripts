/**
 * ExpansionHumanCommandFall.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionHumanCommandFall_ST
{
	int m_FallCommand;
	int m_ParachutePullCommand;
	int m_ParachuteCutCommand;
	int m_JumpCommand;
	int m_LandCommand;

	int m_MovementSpeed;
	int m_MovementDirection;

	int m_LandEarlyExit;

	void ExpansionHumanCommandFall_ST( Human human )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandFall_ST::ExpansionHumanCommandFall_ST Start");
		#endif
		
		HumanAnimInterface hai = human.GetAnimInterface();
		
		m_FallCommand = hai.BindCommand( "CMD_Fall" );
		m_JumpCommand = hai.BindCommand( "CMD_Jump" );
		m_LandCommand = hai.BindCommand( "CMD_Land" );

		m_ParachutePullCommand = hai.BindCommand( "CMD_Expansion_Parachute_Pull" );
		m_ParachuteCutCommand = hai.BindCommand( "CMD_Expansion_Parachute_Cut" );

		m_MovementSpeed = hai.BindVariableFloat( "MovementSpeed" );
		m_MovementDirection = hai.BindVariableFloat( "MovementDirection" );

		m_LandEarlyExit = hai.BindEvent( "LandEarlyExit" );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandFall_ST::ExpansionHumanCommandFall_ST End");
		#endif
	}
}

class ExpansionHumanCommandFall extends HumanCommandScript
{
	PlayerBase m_Player;
	ExpansionHumanCommandFall_ST m_Table;
	HumanInputController m_Input;

	float m_JumpTime;
	float m_JumpMaxTime;
	float m_JumpVelocity;

	bool m_NeedFinish;

	int m_LandType;

	float m_InputX;
	float m_InputZ;

	bool m_IsJumping;
	bool m_IsFalling;

	ExpansionParachute m_Parachute;
	bool m_IsParachuteDeployedPrevious;

	float m_Time;

	private float m_PreviousHeading;

	private bool m_LandEarlyExit;

	void ExpansionHumanCommandFall( Human pHuman, float pYVelocity, ExpansionHumanCommandFall_ST table )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandFall::ExpansionHumanCommandFall Start");
		#endif

		Class.CastTo( m_Player, pHuman );

		m_Table = table;
		m_JumpVelocity = pYVelocity;

		if ( m_JumpVelocity > 0 )
			m_JumpMaxTime = m_JumpVelocity / 4.905;

		m_Input = m_Player.GetInputController();
		m_IsFalling = true;

		m_LandEarlyExit = false;

		m_LandType = -1;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandFall::ExpansionHumanCommandFall End");
		#endif
	}

	bool IsParachuteDeployed()
	{
		return m_Parachute != NULL;
	}

	bool IsParachuteCut()
	{
		return m_Parachute != NULL && m_Parachute.IsCut();
	}

	ExpansionParachute DeployParachute( ExpansionParachuteBag bag )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandFall::DeployParachute Start");
		#endif

		if ( m_Parachute == NULL && bag != NULL )
		{
			m_Parachute = bag.DeployParachute();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandFall::DeployParachute End");
		#endif

		return m_Parachute;
	}

	void CutParachute()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandFall::CutParachute Start");
		#endif

		if ( m_Parachute )
		{
			m_Parachute.CutParachute();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandFall::CutParachute End");
		#endif
	}

	override void OnActivate()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandFall::OnActivate - Start");
		#endif

		ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_FALL_COMMAND, "Activated" );

		if ( m_JumpVelocity > 0 )
		{
			PreAnim_CallCommand( m_Table.m_JumpCommand, 0, 0 );

			m_IsJumping = true;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandFall::OnActivate - End");
		#endif
	}

	override void OnDeactivate()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandFall::OnDeactivate - Start");
		#endif

		ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_FALL_COMMAND, "Deactivated" );

		if ( m_Parachute && !m_Parachute.IsCut() )
		{
			m_Parachute.CutParachute();
		}

		m_Parachute = NULL;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandFall::OnDeactivate - End");
		#endif
	}

	override void PreAnimUpdate( float pDt )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandFall::PreAnimUpdate - Start");
		#endif

		if ( m_Parachute && !m_IsParachuteDeployedPrevious )
		{
			m_IsParachuteDeployedPrevious = true;

			PreAnim_CallCommand( m_Table.m_ParachutePullCommand, 5, 0 );
		}

		if ( m_Parachute && m_Parachute.IsCut() && m_IsParachuteDeployedPrevious )
		{
			m_IsParachuteDeployedPrevious = false;

			PreAnim_CallCommand( m_Table.m_ParachuteCutCommand, 5, 0 );
		}

		if ( m_Parachute && !m_Parachute.IsCut() )
		{
			PreAnim_SetFilteredHeading( 0, 0.1, 180 );
		} else
		{
			float heading = m_Input.GetHeadingAngle();
			PreAnim_SetFilteredHeading( heading, 0.1, 180 );
		}

		if ( !m_IsJumping && m_IsFalling && m_Time == 0 )
		{
			PreAnim_CallCommand( m_Table.m_FallCommand, 0, 0 );
		}

		if ( !m_IsFalling && m_LandType != -1 && m_Time == 0 )
		{		
			ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_FALL_COMMAND, "Not Falling" );
			PreAnim_CallCommand( m_Table.m_LandCommand, m_LandType, 0 );	

			m_NeedFinish = true;
			m_IsJumping = false;
		}

		m_Time += pDt;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandFall::PreAnimUpdate - End");
		#endif
	}

	override void PrePhysUpdate( float pDt )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandFall::PrePhysUpdate - Start");
		#endif

		//Print( m_LandEarlyExit );
		//Print( m_Table.m_LandEarlyExit );

		if ( !m_LandEarlyExit )
			m_LandEarlyExit = PrePhys_IsEvent( m_Table.m_LandEarlyExit );

		float speed;
		vector direction;
		m_Input.GetMovement( speed, direction );
		float heading = m_Input.GetHeadingAngle() * Math.RAD2DEG;

		if ( m_Parachute && !m_Parachute.IsCut() )
		{
			PrePhys_SetTranslation( vector.Zero );

			HumanCommandWeapons hcw = m_Player.GetCommandModifier_Weapons();
			if ( hcw )
			{
				heading = hcw.GetBaseAimingAngleLR() + heading;

				m_Parachute.Input( Vector( heading, direction[2], direction[0] ) );

				m_PreviousHeading = heading;
			}

			// if ( IsMissionHost() )
			{
				m_Parachute.Simulate( pDt );
			}
		}

		if ( m_IsJumping && m_JumpTime < m_JumpMaxTime )
		{
			PrePhys_SetTranslation( vector.Zero );

			vector impulse = vector.Zero;

			if ( m_JumpVelocity == 0 )
			{
				impulse[1] = 0.5 * 9.81 * pDt * ( m_JumpMaxTime - m_JumpTime ) / m_JumpMaxTime;
			} else
			{
				impulse[1] = m_JumpVelocity - Math.Max( GetVelocity( m_Player )[1] / pDt, 0 );
			}

			m_JumpVelocity = 0;

			dBodyApplyImpulse( m_Player, impulse * dBodyGetMass( m_Player ) );

			m_JumpTime += pDt;
		} else
		{
			m_JumpTime = 0;
			m_IsJumping = false;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandFall::PrePhysUpdate - End");
		#endif
	}

	override bool PostPhysUpdate( float pDt )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandFall::PostPhysUpdate - Start");
		#endif

		if ( !m_NeedFinish )
		{
			vector contactPos;
			vector contactDir;
			int contactComponent;
			set<Object> objects = new set<Object>;
			vector start = vector.Zero;
			vector end = vector.Zero;
			float distFraction;
			Object hitObj;

			if ( m_Parachute && !m_Parachute.IsCut() )
			{
				start = m_Parachute.GetPosition();
				end = start - "0 5.0 0";

				if ( DayZPhysics.SphereCastBullet( start, end, 0.25, 0xFFFFFFFF, m_Parachute, hitObj, contactPos, contactDir, distFraction ) )
				{
					m_Player.SetAllowDamage( false );

					m_Player.CutParachute();

					// Temp fix
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater( m_Player.SetAllowDamage, 2500, false, true );
				}
			} else if ( m_IsFalling && GetVelocity( m_Player )[1] <= 0 )
			{
				start = m_Player.GetPosition();
				end = start - "0 0.001 0";

				bool cast = DayZPhysics.SphereCastBullet( start, end, 0.05, 0xFFFFFFFF, m_Player, hitObj, contactPos, contactDir, distFraction );

				if ( cast )
				{
					SetLanded();
				} else if ( !m_Player.PhysicsIsFalling( true ) )
				{
					SetLanded();
				}
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandFall::PostPhysUpdate - End - " + m_NeedFinish );
		#endif
		
		ExpansionDebugger.Push( EXPANSION_DEBUG_PLAYER_FALL_COMMAND );

		if ( m_LandEarlyExit )
		{
			return m_NeedFinish == false;
		}

		return true;
	}

	void SetLanded()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandFall::SetLanded Start");
		#endif

		ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_FALL_COMMAND, "SetLanded" );
		
		m_Time = 0;
		m_IsFalling = false;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandFall::SetLanded End");
		#endif
	}

	bool HasLanded()
	{
		return !m_IsFalling && !m_NeedFinish;
	}

	void Land( int type )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandFall::Land - Start");
		#endif

		ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_FALL_COMMAND, "Land: " + type );
		ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_FALL_COMMAND, "m_IsFalling: " + m_IsFalling );

		m_LandType = type;
		m_Time = 0;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandFall::Land - End");
		#endif
	}
}
