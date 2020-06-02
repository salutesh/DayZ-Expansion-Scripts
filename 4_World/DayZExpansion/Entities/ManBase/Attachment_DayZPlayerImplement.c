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

	protected bool m_ExAttachAfterSeat;

	protected vector m_ExClimbPosition;
	protected bool m_IsClimbAttaching;

	protected int m_ExSeatPosition;

	protected bool m_ExCanPerformRaycast;

	protected float m_ExAttachmentRadius;

	protected bool m_ExAttachingBlockingCommandStart;
	protected bool m_ExAttachingBlockingCommandFinished;

	protected ref ExpansionHumanLeavingVehicleCommand_ST m_ExpansionLeavingVehicleCommandST;

	// ------------------------------------------------------------
	void DayZPlayerImplement()
	{
		m_ExCanPerformRaycast = true;
	}

	// ------------------------------------------------------------
	void StartCommand_ExpansionFall( float pYVelocity );

	// ------------------------------------------------------------
	void OnClimbStart( SHumanCommandClimbResult res )
	{
		m_IsClimbAttaching = false;

		if ( res.m_bFinishWithFall )
			return;

		if ( !ExpansionAttachmentHelper.CanAttachTo( this, res.m_ClimbStandPointParent ) )
			return;

		m_ExAttachmentObject = res.m_ClimbStandPointParent;
		m_ExClimbPosition = res.m_ClimbStandPoint;

		m_ExPlayerLinkType = ExpansionPlayerLink.CLIMB_START;

		m_IsClimbAttaching = true;
	}
	
	// ------------------------------------------------------------
	override bool CanClimb( int climbType, SHumanCommandClimbResult climbRes )
	{
		// it's too buggy, prevent climbing while attached to anything
		// unfortunately it means no climbing while on the LHD
		if ( GetParent() )
			return false;

		return super.CanClimb( climbType, climbRes );
	}

	// ------------------------------------------------------------
	override bool IsInTransport()
	{
		if ( GetCommand_Vehicle() )
			return true;
		
		if ( ExpansionHumanVehicleCommand.Cast( GetCommand_Script() ) )
			return true;

		return false;
	}
	
	// ------------------------------------------------------------
	ExpansionHumanLeavingVehicleCommand GetCommand_ExpansionLeaveVehicle()
	{
		return ExpansionHumanLeavingVehicleCommand.Cast( GetCommand_Script() );
	}
	
	// ------------------------------------------------------------
	void StartCommand_ExpansionLeaveVehicle( Object obj )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerImplement::StartCommand_ExpansionLeaveVehicle - Start");
		#endif
		
		if ( m_ExpansionLeavingVehicleCommandST == NULL )
		{
			m_ExpansionLeavingVehicleCommandST = new ExpansionHumanLeavingVehicleCommand_ST( this );
		}
	
		PlayerBase player = PlayerBase.Cast( this );
		StartCommand_Script( new ExpansionHumanLeavingVehicleCommand( player, obj, m_ExpansionLeavingVehicleCommandST ) );
	
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
				Error( "[ERROR] PlayerBase::OnInputUserDataProcess() -> Could not read task type for EXPANSION_INPUT_UDT_PLAYER_LINK" );
				return false;
			}

			if ( m_ExPlayerLinkType == ExpansionPlayerLink.ATTACH || m_ExPlayerLinkType == ExpansionPlayerLink.GET_OUT_VEHICLE || m_ExPlayerLinkType == ExpansionPlayerLink.CLIMB_FINISHED )
			{
				if ( !ctx.Read( m_ExAttachmentObject ) )
				{
					Error( "[ERROR] PlayerBase::OnInputUserDataProcess() -> Could not read parent object for EXPANSION_INPUT_UDT_PLAYER_LINK" );
					return false;
				}

				if ( m_ExPlayerLinkType == ExpansionPlayerLink.GET_OUT_VEHICLE )
				{
					if ( !ctx.Read( m_ExSeatPosition ) )
					{
						Error( "[ERROR] PlayerBase::OnInputUserDataProcess() -> Could not read parent object for EXPANSION_INPUT_UDT_PLAYER_LINK" );
						return false;
					}
				}
			} else if ( m_ExPlayerLinkType == ExpansionPlayerLink.CLIMB_START )
			{
				m_ExPlayerLinkType = ExpansionPlayerLink.NONE;

				if ( !ctx.Read( m_ExClimbPosition ) )
				{
					Error( "[ERROR] PlayerBase::OnInputUserDataProcess() -> Could not read climb position for EXPANSION_INPUT_UDT_PLAYER_CLIMB" );
					return false;
				}

			} else if ( m_ExPlayerLinkType == ExpansionPlayerLink.REQUEST_LINK )
			{
				m_ExPlayerLinkType = ExpansionPlayerLink.NONE;

				m_ExCanPerformRaycast = true;
				
				ScriptRPC rpc = new ScriptRPC();
				rpc.Send( this, EXPANSION_INPUT_RPC_PLAYER_LINK, true, GetIdentity() );
			}

			// AttachmentDebugPrint("PlayerBase::OnInputUserDataProcess end");
			return true;
		}

		// AttachmentDebugPrint("PlayerBase::OnInputUserDataProcess end");
		return false;
	}

	// ------------------------------------------------------------	
	override void OnRPC( PlayerIdentity sender, int rpc_type, ParamsReadContext ctx )
	{
		if ( rpc_type == EXPANSION_INPUT_RPC_PLAYER_LINK )
		{
			if ( GetGame().IsServer() && sender != GetIdentity() )
			{
				return;
			}

			m_ExCanPerformRaycast = true;

			return;
		}

		super.OnRPC( sender, rpc_type, ctx );
	}

	// ------------------------------------------------------------
	private ExpansionPlayerRaycastResult AttachRaycastCheck()
	{
		IEntity parent = GetParent();
		HumanCommandLadder ladderCommand = GetCommand_Ladder();
		HumanCommandSwim swimCommand = GetCommand_Swim();
		ExpansionHumanFallCommand fallCommand = ExpansionHumanFallCommand.Cast( GetCommand_Script() );

		if ( swimCommand && parent )
			return ExpansionPlayerRaycastResult.DETACH;

		if ( swimCommand )
			return ExpansionPlayerRaycastResult.FALSE;

		if ( fallCommand && fallCommand.IsParachuteDeployed() )
			return ExpansionPlayerRaycastResult.FALSE;

		if ( fallCommand && parent )
			return ExpansionPlayerRaycastResult.DETECT_FALL;

		if ( fallCommand )
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
				if ( m_LastCommandBeforeUnconscious == DayZPlayerConstants.COMMANDID_SWIM)
				{
					StartCommand_Swim();
					return true;
				}

				if ( ( m_LastCommandBeforeUnconscious == DayZPlayerConstants.COMMANDID_VEHICLE || m_LastCommandBeforeUnconscious == DayZPlayerConstants.COMMANDID_DAMAGE ) && ( m_TransportCache != null ) )
				{
					int crew_index = m_TransportCache.CrewMemberIndex( this );
					int seat = m_TransportCache.GetSeatAnimationType( crew_index );
					StartCommand_Vehicle( m_TransportCache, crew_index, seat );
					m_TransportCache = NULL;
					return true;
				}
			}

			if ( pCurrentCommandID == DayZPlayerConstants.COMMANDID_CLIMB && m_IsClimbAttaching )
			{
				m_IsClimbAttaching = false;
				m_ExCanPerformRaycast = false;

				m_ExPlayerLinkType = ExpansionPlayerLink.CLIMB_FINISHED;

				StartCommand_Move();

				return true;
			}

			if ( pCurrentCommandID == DayZPlayerConstants.COMMANDID_VEHICLE )
			{
				CarScript car;
				int seatIdx = -1;
				HumanCommandVehicle hcv = GetCommand_Vehicle();
				if ( hcv )
				{
					m_ExSeatPosition = hcv.GetVehicleSeat();

					Class.CastTo( car, hcv.GetTransport() );
					
					if ( !car.CanObjectAttach( this ) )
						car = NULL;
				}
				
				if ( m_ExSeatPosition > -1 && car != NULL && car.LeavingSeatDoesAttachment( m_ExSeatPosition ) )
				{
					m_ExAttachAfterSeat = true;

					m_ExCanPerformRaycast = false;
					m_ExPlayerLinkType = ExpansionPlayerLink.REQUEST_LINK;

					m_ExAttachmentObject = car;

					StartCommand_Move();

					vector crewTrans[4];
					car.CrewTransform( m_ExSeatPosition, crewTrans );

					vector tmPlayer[4];
					vector tmTarget[4];
					vector tmLocal[4];

					GetTransform( tmPlayer );
					m_ExAttachmentObject.GetTransform( tmTarget );
					Math3D.MatrixInvMultiply4( tmTarget, tmPlayer, tmLocal );
					// tmLocal[3] = crewTrans[3];

					LinkToLocalSpaceOf( m_ExAttachmentObject, tmLocal );

					m_ExAttachmentObject.Update();
					Update();

					return true;
				} else
				{
					// Hopeful 108 prep
					// UnlinkFromLocalSpace();
				}
			}
			
			if ( PhysicsIsFalling( true ) )
			{
				StartCommand_ExpansionFall( 0 );
				m_FallYDiff = GetPosition()[1];

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

		if ( IsMissionClient() )
		{
			if ( pCurrentCommandID == DayZPlayerConstants.COMMANDID_VEHICLE )
			{
				m_ExCanPerformRaycast = false;
				m_ExAttachingBlockingCommandStart = true;
			}

			if ( pCurrentCommandID == DayZPlayerConstants.COMMANDID_CLIMB )
			{
				m_ExCanPerformRaycast = false;
				m_ExAttachingBlockingCommandStart = true;
			}

			if ( m_ExAttachingBlockingCommandStart && pCurrentCommandFinished )
			{
				m_ExAttachingBlockingCommandStart = false;
				m_ExAttachingBlockingCommandFinished = true;
			}
		}

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
			// AttachmentDebugPrint( "m_ExCanPerformRaycast: " + m_ExCanPerformRaycast );
			// AttachmentDebugPrint( "m_ExAttachingBlockingCommandStart: " + m_ExAttachingBlockingCommandStart );
			// AttachmentDebugPrint( "m_ExAttachingBlockingCommandFinished: " + m_ExAttachingBlockingCommandFinished );
			// AttachmentDebugPrint( "parent: " + parent );
			// AttachmentDebugPrint( "canStore: " + canStore );

			if ( !canStore )
			{
				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint("Expansion Player attachment info can't be sent, not performing raycast!");
				#endif
				
			} else if ( m_ExAttachAfterSeat && m_ExCanPerformRaycast )
			{
				m_ExPlayerLinkType = ExpansionPlayerLink.GET_OUT_VEHICLE;
				m_ExAttachAfterSeat = false;
			} else if ( !m_ExAttachAfterSeat && !m_ExCanPerformRaycast )
			{
				if ( m_ExAttachingBlockingCommandFinished && !parent )
				{					
					if ( GetGame().IsServer() )
					{
						m_ExCanPerformRaycast = true;
					}

					m_ExPlayerLinkType = ExpansionPlayerLink.REQUEST_LINK;

					m_ExAttachingBlockingCommandFinished = false;
				}
			} else if ( m_ExCanPerformRaycast )
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
				case ExpansionPlayerLink.GET_OUT_VEHICLE:
				{
					// AttachmentDebugPrint( "START GET_OUT_VEHICLE" );

					CarScript ex_link_to_car;
					if ( Class.CastTo( ex_link_to_car, m_ExAttachmentObject ) )
					{
						HumanCommandMove hcm = GetCommand_Move();
						if ( hcm )
							hcm.ForceStance( DayZPlayerConstants.STANCEIDX_CROUCH );

						/*
						vector contact_pos;
						vector contact_dir;
						float contact_amt;
						Object contact_obj;

						GetTransform( tmPlayer );
						m_ExAttachmentObject.GetTransform( tmTarget );

						vector rayVehStart = tmPlayer[3] * (tmTarget[1] * 1.0 );
						vector rayVehEnd = tmPlayer[3] * (tmTarget[1] * -1.0 ); 

						if ( DayZPhysics.RayCastBullet( rayVehStart, rayVehEnd, PhxInteractionLayers.VEHICLE, this, contact_obj, contact_pos, contact_dir, contact_amt ) )
						{
							tmPlayer[3] = contact_pos;
						}

						Math3D.MatrixInvMultiply4( tmTarget, tmPlayer, tmLocal );

						LinkToLocalSpaceOf( m_ExAttachmentObject, tmLocal );
						*/

						/*
						vector crewTrans[4];
						ex_link_to_car.CrewTransform( m_ExSeatPosition, crewTrans );

						GetTransform( tmPlayer );
						m_ExAttachmentObject.GetTransform( tmTarget );
						Math3D.MatrixInvMultiply4( tmTarget, tmPlayer, tmLocal );
						tmLocal[3] = crewTrans[3];

						LinkToLocalSpaceOf( m_ExAttachmentObject, tmLocal );

						m_ExAttachmentObject.Update();
						*/
					}

					// AttachmentDebugPrint( "END GET_OUT_VEHICLE" );
					break;
				}
				case ExpansionPlayerLink.CLIMB_FINISHED:
				{
					// AttachmentDebugPrint( "START CLIMB_FINISHED" );

					GetTransform( tmPlayer );
					m_ExAttachmentObject.GetTransform( tmTarget );
					Math3D.MatrixInvMultiply4( tmTarget, tmPlayer, tmLocal );
					tmLocal[3] = m_ExClimbPosition;

					LinkToLocalSpaceOf( m_ExAttachmentObject, tmLocal );

					m_ExAttachmentObject.Update();

					// AttachmentDebugPrint( "END CLIMB_FINISHED" );
					break;
				}
			}

			// Send the user input to the server
			if ( GetGame().IsClient() && ScriptInputUserData.CanStoreInputUserData() )
			{
				// AttachmentDebugPrint( "Sending Context to Server" );

				ScriptInputUserData ctx = new ScriptInputUserData;
				ctx.Write( EXPANSION_INPUT_UDT_PLAYER_LINK );
				ctx.Write( m_ExPlayerLinkType );

				if ( m_ExPlayerLinkType == ExpansionPlayerLink.ATTACH || m_ExPlayerLinkType == ExpansionPlayerLink.GET_OUT_VEHICLE || m_ExPlayerLinkType == ExpansionPlayerLink.CLIMB_FINISHED )
				{
					ctx.Write( m_ExAttachmentObject );

					if ( m_ExPlayerLinkType == ExpansionPlayerLink.GET_OUT_VEHICLE )
					{
						ctx.Write( m_ExSeatPosition );
					}
				} else if ( m_ExPlayerLinkType == ExpansionPlayerLink.CLIMB_START )
				{
					ctx.Write( m_ExClimbPosition );
				} else if ( m_ExPlayerLinkType == ExpansionPlayerLink.REQUEST_LINK )
				{
				}

				ctx.Send();

				// AttachmentDebugPrint( "Sent Context to Server" );
			}

			// Reset the data
			// m_ExAttachmentObject = NULL;
			m_ExPlayerLinkType = ExpansionPlayerLink.NONE;

			if ( GetGame().IsServer() )
			{
				// AttachmentDebugPrint( "Attempting to send update" );

				m_ExCanPerformRaycast = true;

				ScriptRPC rpc = new ScriptRPC();
				rpc.Send( this, EXPANSION_INPUT_RPC_PLAYER_LINK, true, GetIdentity() );

				// AttachmentDebugPrint( "Hopefully sent update" );
			} else
			{
				m_ExCanPerformRaycast = false;
			}

			// AttachmentDebugPrint( "ExPlayerLinkType Processed" );
		}

		ExpansionDebugger.Push( EXPANSION_DEBUG_PLAYER_ATTACHMENT );
	}
}