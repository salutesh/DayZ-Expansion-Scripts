/**
 * Attachment_DayZPlayerImplement.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZPlayerImplement
{
	protected Object m_ExAttachmentObject;
	protected ExpansionPlayerLink m_ExPlayerLinkType;

	protected SHumanCommandClimbResult m_ExClimbRes;
	protected int m_ExClimbType;

	protected float m_ExAttachmentRadius;

	protected bool m_ExAttachingBlockingCommandStart;
	protected bool m_ExAttachingBlockingCommandFinished;

	protected ref ExpansionHumanCommandLeavingVehicle_ST m_ExpansionLeavingVehicleCommandST;

	// ------------------------------------------------------------
	void DayZPlayerImplement()
	{
	}

	// ------------------------------------------------------------
	void StartCommand_ExpansionFall( float pYVelocity );

	// ------------------------------------------------------------
	void OnClimbStart( SHumanCommandClimbResult pClimbRes, int climbType )
	{
		if ( m_ExPlayerLinkType != ExpansionPlayerLink.NONE )
			return;

		Object.CastTo( m_ExAttachmentObject, pClimbRes.m_ClimbStandPointParent );

		if ( !ExpansionAttachmentHelper.CanAttachTo( this, m_ExAttachmentObject ) )
		{
			m_ExAttachmentObject = NULL;

			StartCommand_Climb( pClimbRes, climbType );

			return;
		}

		m_ExPlayerLinkType = ExpansionPlayerLink.CLIMB_START;
		m_ExClimbRes = pClimbRes;
		m_ExClimbType = climbType;
	}
	
	// ------------------------------------------------------------
	override bool CanClimb( int climbType, SHumanCommandClimbResult climbRes )
	{
		if ( m_ExPlayerLinkType != ExpansionPlayerLink.NONE )
			return false;

		return super.CanClimb( climbType, climbRes );
	}

	// ------------------------------------------------------------
	override bool IsInTransport()
	{
		if ( GetCommand_Vehicle() )
			return true;
		
		if ( ExpansionHumanCommandVehicle.Cast( GetCommand_Script() ) )
			return true;

		return false;
	}
	
	// ------------------------------------------------------------
	ExpansionHumanCommandLeavingVehicle GetCommand_ExpansionLeaveVehicle()
	{
		return ExpansionHumanCommandLeavingVehicle.Cast( GetCommand_Script() );
	}
	
	// ------------------------------------------------------------
	void StartCommand_ExpansionLeaveVehicle( Object obj )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerImplement::StartCommand_ExpansionLeaveVehicle - Start");
		#endif
		
		if ( m_ExpansionLeavingVehicleCommandST == NULL )
		{
			m_ExpansionLeavingVehicleCommandST = new ExpansionHumanCommandLeavingVehicle_ST( this );
		}
	
		PlayerBase player = PlayerBase.Cast( this );
		StartCommand_Script( new ExpansionHumanCommandLeavingVehicle( player, obj, m_ExpansionLeavingVehicleCommandST ) );
	
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerImplement::StartCommand_ExpansionLeaveVehicle - End");
		#endif
	}

	// ------------------------------------------------------------
	void AttachmentDebugPrint( string message )
	{
		// done so EXPrint can be disabled and this can still print
		Print( "[Expansion] PlayerBase Attachment Debug: " + message );
	}

	// ------------------------------------------------------------
	void AttachTo( IEntity target, bool already_validated = false )
	{
		// AttachmentDebugPrint("PlayerBase::AttachTo start");

		m_ExPlayerLinkType = ExpansionPlayerLink.NONE;

		if ( IsMissionClient() && Class.CastTo( m_ExAttachmentObject, target ) )
		{
			m_ExPlayerLinkType = ExpansionPlayerLink.ATTACH;

			// vector size[2];
			// float radius = target.ClippingInfo( size );

			if ( already_validated || ExpansionAttachmentHelper.CanAttachTo( this, m_ExAttachmentObject ) )
			{
				vector size[2];
				m_ExAttachmentRadius = m_ExAttachmentObject.ClippingInfo( size );

				// AttachmentDebugPrint("PlayerBase::AttachTo success");
			} else
			{
				m_ExAttachmentObject = NULL;
				m_ExPlayerLinkType = ExpansionPlayerLink.NONE;

				// AttachmentDebugPrint("PlayerBase::AttachTo failure");
			}
		}

		// AttachmentDebugPrint("PlayerBase::AttachTo end");
	}

	// ------------------------------------------------------------
	void Detach()
	{
		// AttachmentDebugPrint("PlayerBase::Detach start");

		m_ExPlayerLinkType = ExpansionPlayerLink.NONE;

		if ( IsMissionClient() )
		{
			m_ExPlayerLinkType = ExpansionPlayerLink.DETACH;
		}

		// AttachmentDebugPrint("PlayerBase::Detach end");
	}

	// ------------------------------------------------------------
	override bool OnInputUserDataProcess( int userDataType, ParamsReadContext ctx ) 
	{
		// AttachmentDebugPrint("PlayerBase::OnInputUserDataProcess start");
		
		if ( super.OnInputUserDataProcess( userDataType, ctx ) )
			return true;

		if ( userDataType == EXPANSION_INPUT_UDT_PLAYER_LINK )
		{
			if ( !ctx.Read( m_ExPlayerLinkType ) )
			{
				m_ExPlayerLinkType == ExpansionPlayerLink.NONE;

				Error( "[ERROR] PlayerBase::OnInputUserDataProcess() -> Could not read task type for EXPANSION_INPUT_UDT_PLAYER_LINK" );
				return false;
			}

			if ( m_ExPlayerLinkType == ExpansionPlayerLink.ATTACH )
			{
				if ( !ctx.Read( m_ExAttachmentObject ) )
				{
					m_ExPlayerLinkType == ExpansionPlayerLink.NONE;

					Error( "[ERROR] PlayerBase::OnInputUserDataProcess() -> Could not read parent object for EXPANSION_INPUT_UDT_PLAYER_LINK" );
					return false;
				}
			}

			// AttachmentDebugPrint("PlayerBase::OnInputUserDataProcess end");
			return true;
		}

		// AttachmentDebugPrint("PlayerBase::OnInputUserDataProcess end");
		return false;
	}

	// ------------------------------------------------------------
	private ExpansionPlayerRaycastResult AttachRaycastCheck()
	{
		IEntity parent = GetParent();
		HumanCommandLadder ladderCommand = GetCommand_Ladder();
		HumanCommandSwim swimCommand = GetCommand_Swim();
		HumanCommandFall fallCommand = GetCommand_Fall();
		ExpansionHumanCommandFall exFallCommand = ExpansionHumanCommandFall.Cast( GetCommand_Script() );

		if ( swimCommand && parent )
			return ExpansionPlayerRaycastResult.DETACH;

		if ( swimCommand )
			return ExpansionPlayerRaycastResult.FALSE;

		if ( exFallCommand && exFallCommand.IsParachuteDeployed() )
			return ExpansionPlayerRaycastResult.FALSE;

		if ( ( exFallCommand || fallCommand ) && parent )
			return ExpansionPlayerRaycastResult.DETECT_FALL;

		if ( exFallCommand || fallCommand )
			return ExpansionPlayerRaycastResult.FALSE;

		if ( ladderCommand )
			return ExpansionPlayerRaycastResult.FALSE;

		return ExpansionPlayerRaycastResult.DETECT;
	}
	
	// ------------------------------------------------------------
	override bool ModCommandHandlerBefore( float pDt, int pCurrentCommandID, bool pCurrentCommandFinished )	
	{
		if ( pCurrentCommandFinished )
		{
			if ( pCurrentCommandID == DayZPlayerConstants.COMMANDID_UNCONSCIOUS )
			{
				if ( ( m_LastCommandBeforeUnconscious == DayZPlayerConstants.COMMANDID_VEHICLE || m_LastCommandBeforeUnconscious == DayZPlayerConstants.COMMANDID_DAMAGE ) && ( m_TransportCache != null ) )
				{
					int crew_index = m_TransportCache.CrewMemberIndex( this );
					int seat = m_TransportCache.GetSeatAnimationType( crew_index );
					StartCommand_Vehicle( m_TransportCache, crew_index, seat );
					m_TransportCache = NULL;
					return true;
				}
			}

			if ( pCurrentCommandID == DayZPlayerConstants.COMMANDID_VEHICLE )
			{
				m_ExPlayerLinkType = ExpansionPlayerLink.NONE;

				CarScript car;
				HumanCommandVehicle hcv = GetCommand_Vehicle();

				if ( hcv && Class.CastTo( car, hcv.GetTransport() ) && car.CanObjectAttach( this ) && car.LeavingSeatDoesAttachment( hcv.GetVehicleSeat() ) )
				{
					StartCommand_Move();
					
					return true;
				}
			}
			
			if ( PhysicsIsFalling( true ) )
			{
				StartCommand_ExpansionFall( 0 );
				m_FallYDiff = GetPosition()[1];

				return true;
			}

			if ( m_Swimming.m_bWasSwimming )
			{
				StartCommand_Swim();

				return true;
			}

			StartCommand_Move();
				
			return true;
		}

		if ( super.ModCommandHandlerBefore( pDt, pCurrentCommandID, pCurrentCommandFinished ) )
		{
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	override void CommandHandler( float pDt, int pCurrentCommandID, bool pCurrentCommandFinished )	
	{
		ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_ATTACHMENT, "Attached to: " + m_ExAttachmentObject );

		bool attachmentSuccess = true;
		IEntity parent = GetParent();

		super.CommandHandler( pDt, pCurrentCommandID, pCurrentCommandFinished );
		
		if ( IsMissionClient() && m_ExPlayerLinkType == ExpansionPlayerLink.NONE )
		{
			// AttachmentDebugPrint("PlayerBase::CommandHandler - Ray Trace - Start");

			bool canStore = ScriptInputUserData.CanStoreInputUserData();

			// AttachmentDebugPrint( "COMMANDID_NONE: " + DayZPlayerConstants.COMMANDID_NONE );
			// AttachmentDebugPrint( "COMMANDID_MOVE: " + DayZPlayerConstants.COMMANDID_MOVE );
			// AttachmentDebugPrint( "COMMANDID_ACTION: " + DayZPlayerConstants.COMMANDID_ACTION );
			// AttachmentDebugPrint( "COMMANDID_MELEE: " + DayZPlayerConstants.COMMANDID_MELEE );
			// AttachmentDebugPrint( "COMMANDID_MELEE2: " + DayZPlayerConstants.COMMANDID_MELEE2 );
			// AttachmentDebugPrint( "COMMANDID_FALL: " + DayZPlayerConstants.COMMANDID_FALL );
			// AttachmentDebugPrint( "COMMANDID_DEATH: " + DayZPlayerConstants.COMMANDID_DEATH );
			// AttachmentDebugPrint( "COMMANDID_DAMAGE: " + DayZPlayerConstants.COMMANDID_DAMAGE );
			// AttachmentDebugPrint( "COMMANDID_LADDER: " + DayZPlayerConstants.COMMANDID_LADDER );
			// AttachmentDebugPrint( "COMMANDID_SWIM: " + DayZPlayerConstants.COMMANDID_SWIM );
			// AttachmentDebugPrint( "COMMANDID_UNCONSCIOUS: " + DayZPlayerConstants.COMMANDID_UNCONSCIOUS );
			// AttachmentDebugPrint( "COMMANDID_VEHICLE: " + DayZPlayerConstants.COMMANDID_VEHICLE );
			// AttachmentDebugPrint( "COMMANDID_CLIMB: " + DayZPlayerConstants.COMMANDID_CLIMB );
			// AttachmentDebugPrint( "COMMANDID_SCRIPT: " + DayZPlayerConstants.COMMANDID_SCRIPT );
			// AttachmentDebugPrint( "pCurrentCommandID: " + pCurrentCommandID );
			// AttachmentDebugPrint( "pCurrentCommandFinished: " + pCurrentCommandFinished );
			// AttachmentDebugPrint( "m_ExAttachingBlockingCommandStart: " + m_ExAttachingBlockingCommandStart );
			// AttachmentDebugPrint( "m_ExAttachingBlockingCommandFinished: " + m_ExAttachingBlockingCommandFinished );
			// AttachmentDebugPrint( "parent: " + parent );
			// AttachmentDebugPrint( "canStore: " + canStore );

			if ( !canStore )
			{
				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint("Expansion Player attachment info can't be sent, not performing raycast!");
				#endif
			} else
			{
				ExpansionPlayerRaycastResult res = AttachRaycastCheck();

				// AttachmentDebugPrint( "Res: " + res );

				float castDistance = 1.0;
				float castRadius = 0.2;

				if ( res == ExpansionPlayerRaycastResult.DETECT_FALL )
				{
					res = ExpansionPlayerRaycastResult.DETECT;

					if ( parent )
					{
						castDistance += 1.0 * m_ExAttachmentRadius;
						castRadius = 1.0;
					}
				}
				
				if ( res == ExpansionPlayerRaycastResult.DETECT )
				{
					vector start = GetPosition() + Vector( 0, 1.0, 0 );
					vector end = GetPosition() - Vector( 0, castDistance, 0 );

					ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_ATTACHMENT, "Start Trace: " + start );
					ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_ATTACHMENT, "End Trace: " + end );

					RaycastRVParams params = new RaycastRVParams( start, end, GetGame().GetPlayer(), castRadius );
					params.sorted = true;
					params.type = ObjIntersectGeom;
					params.flags = CollisionFlags.ALLOBJECTS;

					// Initiate the raycast
					array< ref RaycastRVResult > results = new array< ref RaycastRVResult >();
					if ( DayZPhysics.RaycastRVProxy( params, results ) )
					{
						Object target = ExpansionAttachmentHelper.FindBestAttach( this, results );

						if ( target != NULL )
						{
							if ( parent && parent != target ) // Detach the player if the parent object is not the same as the target, we have to deal with attachment in next frame.
							{
								// AttachmentDebugPrint("aparent && parent != target");

								Detach();
							} else if ( !parent ) // If the parent is null, verify the target and attach the player
							{
								// AttachmentDebugPrint("a!parent");

								AttachTo( target );
							}
						} else if ( parent && parent == m_ExAttachmentObject )
						{
							// AttachmentDebugPrint("dparent");

							Detach();
						}
					} else if ( parent && parent == m_ExAttachmentObject ) // Currently have a parent when the raycast failed, forcefully detach the player
					{
						// AttachmentDebugPrint("drparent");
						Detach();
					}
					
					ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_ATTACHMENT, "Size: " + results.Count() );
					for ( int j = 0; j < results.Count(); j++ )
					{
						ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_ATTACHMENT, "results[" + j + "]: " + results[j] );
						ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_ATTACHMENT, "  obj: " + results[j].obj );
						ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_ATTACHMENT, "  parent: " + results[j].parent );
						ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_ATTACHMENT, "  pos: " + results[j].pos );
						ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_ATTACHMENT, "  dir: " + results[j].dir );
						ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_ATTACHMENT, "  hierLevel: " + results[j].hierLevel );
						ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_ATTACHMENT, "  component: " + results[j].component );
						ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_ATTACHMENT, "  entry: " + results[j].entry );
						ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_ATTACHMENT, "  exit: " + results[j].exit );
					}
				} else if ( res == ExpansionPlayerRaycastResult.DETACH )
				{
					Detach();
				}
			}

			// AttachmentDebugPrint("PlayerBase::CommandHandler - Ray Trace - End");
		}

		// Attachment code
		if ( m_ExPlayerLinkType != ExpansionPlayerLink.NONE && GetInstanceType() != DayZPlayerInstanceType.INSTANCETYPE_REMOTE )
		{
			// AttachmentDebugPrint( "ExPlayerLinkType Changed" );

			bool isClimbing = false;
			if ( m_ExPlayerLinkType == ExpansionPlayerLink.CLIMB_START )
			{
				isClimbing = true;

				m_ExPlayerLinkType = ExpansionPlayerLink.ATTACH;
			}

			vector tmPlayer[ 4 ];
			vector tmTarget[ 4 ];
			vector tmLocal[ 4 ];

			switch ( m_ExPlayerLinkType )
			{
				case ExpansionPlayerLink.DETACH:
				{
					// AttachmentDebugPrint( "START DETACH" );

					UnlinkFromLocalSpace();

					m_ExAttachmentObject.Update();
					
					m_ExAttachmentObject = NULL;

					// AttachmentDebugPrint( "END DETACH" );
					break;
				}
				case ExpansionPlayerLink.ATTACH:
				{
					// AttachmentDebugPrint( "START ATTACH" );

					GetTransform( tmPlayer );
					m_ExAttachmentObject.GetTransform( tmTarget );
					Math3D.MatrixInvMultiply4( tmTarget, tmPlayer, tmLocal );

					LinkToLocalSpaceOf( m_ExAttachmentObject, tmLocal );

					m_ExAttachmentObject.Update();

					// AttachmentDebugPrint( "END ATTACH" );
					break;
				}
			}

			if ( m_ExPlayerLinkType == ExpansionPlayerLink.CLIMB_FINISHED )
			{
				m_ExPlayerLinkType = ExpansionPlayerLink.NONE;

				StartCommand_Climb( m_ExClimbRes, m_ExClimbType );

				isClimbing = true;
			} else if ( isClimbing )
			{
				m_ExPlayerLinkType = ExpansionPlayerLink.CLIMB_FINISHED;
			}
			
			// Send the user input to the server
			if ( !isClimbing && GetGame().IsClient() && ScriptInputUserData.CanStoreInputUserData() )
			{
				// AttachmentDebugPrint( "Sending Context to Server" );

				ScriptInputUserData ctx = new ScriptInputUserData;
				ctx.Write( EXPANSION_INPUT_UDT_PLAYER_LINK );
				ctx.Write( m_ExPlayerLinkType );

				if ( m_ExPlayerLinkType == ExpansionPlayerLink.ATTACH )
				{
					ctx.Write( m_ExAttachmentObject );
				} else if ( m_ExPlayerLinkType == ExpansionPlayerLink.REQUEST_LINK )
				{
				}

				ctx.Send();
			
				// AttachmentDebugPrint( "Sent Context to Server" );
			}

			if ( m_ExPlayerLinkType != ExpansionPlayerLink.CLIMB_FINISHED )
			{
				// Reset the data
				// m_ExAttachmentObject = NULL;
				m_ExPlayerLinkType = ExpansionPlayerLink.NONE;
			}

			// AttachmentDebugPrint( "ExPlayerLinkType Processed" );
		}

		ExpansionDebugger.Push( EXPANSION_DEBUG_PLAYER_ATTACHMENT );
	}
}