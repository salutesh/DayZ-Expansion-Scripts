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
	protected ExpansionPlayerLink m_ExPlayerLinkTypeFrameDelayed;
	protected bool m_ExIsAwaitingServerLink;
	protected bool m_ExPerformClimbAttach;
	protected bool m_ExPerformClimb;

	protected bool m_IsAttaching;

	SHumanCommandClimbResult Ex_ClimbResult;
	protected int m_ExClimbType;

	protected float m_ExAttachmentRadius;

	protected bool m_ExIsPreparingVehicle;
	protected bool m_ExPerformVehicleGetIn;

	// ------------------------------------------------------------
	void DayZPlayerImplement()
	{
		Ex_ClimbResult = new SHumanCommandClimbResult;
	}

	// ------------------------------------------------------------
	void StartCommand_ExpansionFall( float pYVelocity );

	bool CallExpansionClimbCode()
	{
		Object attchObh;
		if ( !Object.CastTo( attchObh, Ex_ClimbResult.m_ClimbStandPointParent ) )
			return false;

		if ( !ExpansionAttachmentHelper.CanAttachTo( this, attchObh ) )
			return false;
		
		if ( attchObh == GetParent() )
			return false;

		return true;
	}

	// ------------------------------------------------------------
	void OnClimbStart( int climbType )
	{
		//if ( m_ExPlayerLinkType != ExpansionPlayerLink.NONE || m_ExIsAwaitingServerLink || m_ExClimbType != -1 )
		//	return;

		m_ExClimbType = climbType;

		Object.CastTo( m_ExAttachmentObject, Ex_ClimbResult.m_ClimbStandPointParent );

		if ( !ExpansionAttachmentHelper.CanAttachTo( this, m_ExAttachmentObject ) )
		{
			m_ExAttachmentObject = NULL;

			StartCommand_Climb( Ex_ClimbResult, m_ExClimbType );

			m_ExClimbType = -1;

			return;
		}

		if ( m_ExAttachmentObject == GetParent() )
		{
			m_ExAttachmentObject = NULL;

			StartCommand_Climb( Ex_ClimbResult, m_ExClimbType );

			m_ExClimbType = -1;

			return;
		}

		if ( GetParent() )
		{
			UnlinkFromLocalSpace();

			m_ExIsAwaitingServerLink = true;

			if ( GetGame().IsServer() )
			{
				DayZPlayerSyncJunctures.ExpansionSendPerformClimb( this, false, true );
			}
		} else
		{
			m_ExPerformClimbAttach = true;

			m_ExIsAwaitingServerLink = true;
		}
	}
	
	// ------------------------------------------------------------
	override bool CanClimb( int climbType, SHumanCommandClimbResult climbRes )
	{
		if ( m_ExPlayerLinkType != ExpansionPlayerLink.NONE )
			return false;

		return super.CanClimb( climbType, climbRes );
	}

	// ------------------------------------------------------------
	void PrepareForVehicleLink()
	{
		AttachmentDebugPrint( "+DayZPlayerImplement::PrepareForVehicleLink" );

		m_ExIsPreparingVehicle = true;
		m_ExPerformVehicleGetIn = false;

		m_ExPlayerLinkType = ExpansionPlayerLink.NONE;

		UnlinkFromLocalSpace();

		if ( GetGame().IsServer() )
		{
			AttachmentDebugPrint( "Sending to client" );
			DayZPlayerSyncJunctures.ExpansionSendGetInTransportUnlink( this );
		}

		AttachmentDebugPrint( "-DayZPlayerImplement::PrepareForVehicleLink" );
	}

	bool CanPerformVehicleGetIn()
	{
		return m_ExPerformVehicleGetIn;
	}

	bool IsPreparingVehicle()
	{
		return m_ExIsPreparingVehicle;
	}

	void EndVehiclePrep()
	{
		AttachmentDebugPrint( "+DayZPlayerImplement::EndVehiclePrep" );

		m_ExIsPreparingVehicle = false;
		m_ExPerformVehicleGetIn = false;

		if ( GetGame().IsClient() )
		{
			AttachmentDebugPrint( "Sending to server" );

			ScriptInputUserData ctx = new ScriptInputUserData;
			ctx.Write( EXPANSION_INPUT_UDT_GET_IN_TRANSPORT_UNLINK );

			ctx.Send();
		}

		AttachmentDebugPrint( "-DayZPlayerImplement::EndVehiclePrep" );
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
	override bool HandleDeath( int pCurrentCommandID )
	{
		if ( pCurrentCommandID == DayZPlayerConstants.COMMANDID_VEHICLE && m_DeathAnimType != -2 )
		{
			HumanCommandVehicle hcv = GetCommand_Vehicle();
			Transport transport = hcv.GetTransport();
			if ( !transport ) //! We died in a wreck, it no longer exists
			{
				if ( m_DeathAnimType != -2 && g_Game.GetMissionState() == g_Game.MISSION_STATE_GAME )
				{
					int type = GetTypeOfDeath( pCurrentCommandID );

					DayZPlayerCommandDeathCallback callback = DayZPlayerCommandDeathCallback.Cast( StartCommand_Death( type, m_DeathHitDir, DayZPlayerCommandDeathCallback ) );
					callback.m_pPlayer = PlayerBase.Cast( this );

					ResetDeathStartTime();
					GetGame().GetWorld().SetVoiceOn( false );

					return true;
				}

				return false;
			}

		}

		return super.HandleDeath( pCurrentCommandID );
	}

	// ------------------------------------------------------------
	void AttachmentDebugPrint( string message )
	{
		// Done so EXPrint can be disabled and this can still print
		#ifdef EXPANSION_PLAYER_ATTACHMENT_LOG
		Print( "[ATTACHMENT] " + Expansion_Debug_Player( this ) + message );
		#endif
	}

	bool IsAttaching()
	{
		return m_IsAttaching;
	}

	// ------------------------------------------------------------
	void AttachTo( IEntity target, bool already_validated = false )
	{
		AttachmentDebugPrint("PlayerBase::AttachTo start");

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

				AttachmentDebugPrint("PlayerBase::AttachTo success");
			} else
			{
				m_ExAttachmentObject = NULL;
				m_ExPlayerLinkType = ExpansionPlayerLink.NONE;

				AttachmentDebugPrint("PlayerBase::AttachTo failure");
			}
		}

		if (m_ExAttachmentObject)
			m_IsAttaching = m_ExAttachmentObject.IsInherited(ExpansionVehicleBase);

		AttachmentDebugPrint("PlayerBase::AttachTo end");
	}

	// ------------------------------------------------------------
	void Detach()
	{
		AttachmentDebugPrint("PlayerBase::Detach start");

		m_ExPlayerLinkType = ExpansionPlayerLink.NONE;
		m_IsAttaching = false;

		if ( IsMissionClient() )
		{
			m_ExPlayerLinkType = ExpansionPlayerLink.DETACH;
		}

		AttachmentDebugPrint("PlayerBase::Detach end");
	}

	// ------------------------------------------------------------
	override bool OnInputUserDataProcess( int userDataType, ParamsReadContext ctx ) 
	{
		AttachmentDebugPrint("PlayerBase::OnInputUserDataProcess start");
		
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
					m_ExAttachmentObject = NULL;
					m_ExPlayerLinkType == ExpansionPlayerLink.NONE;

					Error( "[ERROR] PlayerBase::OnInputUserDataProcess() -> Could not read parent object for EXPANSION_INPUT_UDT_PLAYER_LINK" );
					return false;
				}
			}

			AttachmentDebugPrint("PlayerBase::OnInputUserDataProcess end");
			return true;
		}

		if ( userDataType == EXPANSION_INPUT_UDT_GET_IN_TRANSPORT_UNLINK )
		{
			AttachmentDebugPrint( "+DayZPlayerImplement::OnInputUserDataProcess - EXPANSION_INPUT_UDT_GET_IN_TRANSPORT_UNLINK" );
			m_ExPerformVehicleGetIn = true;

			return true;
		}

		if ( userDataType == EXPANSION_INPUT_UDT_PERFORM_CLIMB )
		{
			m_ExPerformClimb = true;
			return true;
		}

		AttachmentDebugPrint("PlayerBase::OnInputUserDataProcess end");
		return false;
	}
	
	override void OnSyncJuncture( int pJunctureID, ParamsReadContext pCtx )
	{
		AttachmentDebugPrint( "+DayZPlayerImplement::OnSyncJuncture - pJunctureID=" + pJunctureID );
		
		super.OnSyncJuncture( pJunctureID, pCtx );

		switch ( pJunctureID )
		{
			case DayZPlayerSyncJunctures.EXPANSION_SJ_NEXT_LINK:
				AttachmentDebugPrint( "+DayZPlayerImplement::OnSyncJuncture - ExpansionReadNextLink" );
				DayZPlayerSyncJunctures.ExpansionReadNextLink( pCtx );

				m_ExIsAwaitingServerLink = false;
				AttachmentDebugPrint( "-DayZPlayerImplement::OnSyncJuncture - ExpansionReadNextLink" );
				break;
			case DayZPlayerSyncJunctures.EXPANSION_SJ_PERFORM_CLIMB:
				AttachmentDebugPrint( "+DayZPlayerImplement::OnSyncJuncture - ExpansionReadPerformClimb" );
				DayZPlayerSyncJunctures.ExpansionReadPerformClimb( pCtx, m_ExPerformClimb, m_ExPerformClimbAttach );

				AttachmentDebugPrint( "-DayZPlayerImplement::OnSyncJuncture - ExpansionReadPerformClimb" );
				break;
			case DayZPlayerSyncJunctures.EXPANSION_SJ_GET_IN_TRANSPORT_UNLINK:
				AttachmentDebugPrint( "+DayZPlayerImplement::OnSyncJuncture - ExpansionReadGetInTransportUnlink" );
				DayZPlayerSyncJunctures.ExpansionReadGetInTransportUnlink( pCtx );

				m_ExPerformVehicleGetIn = true;
				AttachmentDebugPrint( "-DayZPlayerImplement::OnSyncJuncture - ExpansionReadGetInTransportUnlink" );
				break;
		}

		AttachmentDebugPrint( "-DayZPlayerImplement::OnSyncJuncture - pJunctureID=" + pJunctureID );
	}

	// ------------------------------------------------------------
	private ExpansionPlayerRaycastResult AttachRaycastCheck( int pCurrentCommandID )
	{
		//DayZPlayerConstants.COMMANDID_NONE
		//DayZPlayerConstants.COMMANDID_MOVE
		//DayZPlayerConstants.COMMANDID_ACTION
		//DayZPlayerConstants.COMMANDID_MELEE
		//DayZPlayerConstants.COMMANDID_MELEE2
		//DayZPlayerConstants.COMMANDID_FALL
		//DayZPlayerConstants.COMMANDID_DEATH
		//DayZPlayerConstants.COMMANDID_DAMAGE
		//DayZPlayerConstants.COMMANDID_LADDER
		//DayZPlayerConstants.COMMANDID_SWIM
		//DayZPlayerConstants.COMMANDID_UNCONSCIOUS
		//DayZPlayerConstants.COMMANDID_VEHICLE
		//DayZPlayerConstants.COMMANDID_CLIMB
		//DayZPlayerConstants.COMMANDID_SCRIPT

		IEntity parent = GetParent();
		HumanCommandLadder ladderCommand = GetCommand_Ladder();
		HumanCommandSwim swimCommand = GetCommand_Swim();
		HumanCommandFall fallCommand = GetCommand_Fall();
		ExpansionHumanCommandVehicle exVehicleCommand = ExpansionHumanCommandVehicle.Cast( GetCommand_Script() );

		if ( m_ExPerformVehicleGetIn || m_ExIsPreparingVehicle || GetCommand_Vehicle() || exVehicleCommand || pCurrentCommandID == DayZPlayerConstants.COMMANDID_VEHICLE )
			return ExpansionPlayerRaycastResult.FALSE;

		if ( swimCommand && parent )
			return ExpansionPlayerRaycastResult.DETACH;

		if ( swimCommand )
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
		if ( s_ExpansionPlayerAttachment )
		{
			if ( pCurrentCommandFinished )
			{
				if ( pCurrentCommandID == DayZPlayerConstants.COMMANDID_UNCONSCIOUS )
				{
					if ( ( m_LastCommandBeforeUnconscious == DayZPlayerConstants.COMMANDID_VEHICLE ) && ( m_TransportCache != NULL ) )
					{
						int crew_index = m_TransportCache.CrewMemberIndex( this );
						int seat = m_TransportCache.GetSeatAnimationType( crew_index );
						StartCommand_Vehicle( m_TransportCache, crew_index, seat, true );
						m_TransportCache = NULL;
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
		}

		if ( super.ModCommandHandlerBefore( pDt, pCurrentCommandID, pCurrentCommandFinished ) )
			return true;

		return false;
	}

	// ------------------------------------------------------------
	override void CommandHandler( float pDt, int pCurrentCommandID, bool pCurrentCommandFinished )	
	{
		super.CommandHandler( pDt, pCurrentCommandID, pCurrentCommandFinished );
		
		if ( !s_ExpansionPlayerAttachment )
			return;

		if ( GetGame().IsClient() && !ScriptInputUserData.CanStoreInputUserData() )
			return;

		if ( m_ExPerformClimb )
		{
			StartCommand_Climb( Ex_ClimbResult, m_ExClimbType );

			m_ExClimbType = -1;

			m_ExPerformClimb = false;

			if ( GetGame().IsServer() )
			{
				DayZPlayerSyncJunctures.ExpansionSendNextLink( this );
			} else
			{
				ScriptInputUserData ctx1 = new ScriptInputUserData;
				ctx1.Write( EXPANSION_INPUT_UDT_PERFORM_CLIMB );

				ctx1.Send();
			}

			return;
		}

		if ( m_ExPerformClimbAttach )
		{
			vector c_tmPlayer[ 4 ];
			vector c_tmTarget[ 4 ];
			vector c_tmLocal[ 4 ];

			if ( m_ExAttachmentObject.IsInherited( CarScript ) && !dBodyIsActive( m_ExAttachmentObject ) )
			{
				//! Potential fix for player getting teleported to the sea (desync)
				EXPrint(GetType() + "::CommandHandler climb attach to " + m_ExAttachmentObject);
				dBodyActive( m_ExAttachmentObject, ActiveState.ACTIVE );
			}

			GetTransform( c_tmPlayer );
			m_ExAttachmentObject.GetTransform( c_tmTarget );
			Math3D.MatrixInvMultiply4( c_tmTarget, c_tmPlayer, c_tmLocal );

			LinkToLocalSpaceOf( m_ExAttachmentObject, c_tmLocal );

			m_ExAttachmentObject.Update();

			m_ExPerformClimbAttach = false;

			if ( GetGame().IsServer() )
			{
				DayZPlayerSyncJunctures.ExpansionSendPerformClimb( this, true, false );
			} else
			{
				m_ExIsAwaitingServerLink = true;
			}

			return;
		}

		if ( IsPreparingVehicle() )
			return;

		IEntity parent = GetParent();
		
		if ( IsMissionClient() && m_ExPlayerLinkType == ExpansionPlayerLink.NONE && !m_ExIsAwaitingServerLink )
		{
			AttachmentDebugPrint("PlayerBase::CommandHandler - Ray Trace - Start");

			AttachmentDebugPrint( "COMMANDID_NONE: " + DayZPlayerConstants.COMMANDID_NONE );
			AttachmentDebugPrint( "COMMANDID_MOVE: " + DayZPlayerConstants.COMMANDID_MOVE );
			AttachmentDebugPrint( "COMMANDID_ACTION: " + DayZPlayerConstants.COMMANDID_ACTION );
			AttachmentDebugPrint( "COMMANDID_MELEE: " + DayZPlayerConstants.COMMANDID_MELEE );
			AttachmentDebugPrint( "COMMANDID_MELEE2: " + DayZPlayerConstants.COMMANDID_MELEE2 );
			AttachmentDebugPrint( "COMMANDID_FALL: " + DayZPlayerConstants.COMMANDID_FALL );
			AttachmentDebugPrint( "COMMANDID_DEATH: " + DayZPlayerConstants.COMMANDID_DEATH );
			AttachmentDebugPrint( "COMMANDID_DAMAGE: " + DayZPlayerConstants.COMMANDID_DAMAGE );
			AttachmentDebugPrint( "COMMANDID_LADDER: " + DayZPlayerConstants.COMMANDID_LADDER );
			AttachmentDebugPrint( "COMMANDID_SWIM: " + DayZPlayerConstants.COMMANDID_SWIM );
			AttachmentDebugPrint( "COMMANDID_UNCONSCIOUS: " + DayZPlayerConstants.COMMANDID_UNCONSCIOUS );
			AttachmentDebugPrint( "COMMANDID_VEHICLE: " + DayZPlayerConstants.COMMANDID_VEHICLE );
			AttachmentDebugPrint( "COMMANDID_CLIMB: " + DayZPlayerConstants.COMMANDID_CLIMB );
			AttachmentDebugPrint( "COMMANDID_SCRIPT: " + DayZPlayerConstants.COMMANDID_SCRIPT );
			AttachmentDebugPrint( "pCurrentCommandID: " + pCurrentCommandID );
			AttachmentDebugPrint( "pCurrentCommandFinished: " + pCurrentCommandFinished );
			AttachmentDebugPrint( "parent: " + parent );

			ExpansionPlayerRaycastResult res = AttachRaycastCheck( pCurrentCommandID );

			AttachmentDebugPrint( "Res: " + res );

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
							AttachmentDebugPrint("aparent && parent != target");

							Detach();
						} else if ( !parent ) // If the parent is null, verify the target and attach the player
						{
							AttachmentDebugPrint("a!parent");

							AttachTo( target );
						}
					} else if ( parent && parent == m_ExAttachmentObject )
					{
						AttachmentDebugPrint("dparent");

						Detach();
					}
				} else if ( parent && parent == m_ExAttachmentObject ) // Currently have a parent when the raycast failed, forcefully detach the player
				{
					AttachmentDebugPrint("drparent");
					Detach();
				}
			} else if ( parent && res == ExpansionPlayerRaycastResult.DETACH )
			{
				Detach();
			}

			AttachmentDebugPrint("PlayerBase::CommandHandler - Ray Trace - End");
		}

		// Attachment code
		if ( m_ExPlayerLinkType != ExpansionPlayerLink.NONE && GetInstanceType() != DayZPlayerInstanceType.INSTANCETYPE_REMOTE )
		{
			AttachmentDebugPrint( "ExPlayerLinkType Changed" );

			vector tmPlayer[ 4 ];
			vector tmTarget[ 4 ];
			vector tmLocal[ 4 ];

			switch ( m_ExPlayerLinkType )
			{
				case ExpansionPlayerLink.DETACH:
				{
					AttachmentDebugPrint( "START DETACH" );

					UnlinkFromLocalSpace();

					//m_ExAttachmentObject.Update();
					
					m_ExAttachmentObject = NULL;

					AttachmentDebugPrint( "END DETACH" );

					break;
				}
				case ExpansionPlayerLink.ATTACH:
				{
					AttachmentDebugPrint( "START ATTACH" );

					if ( m_ExAttachmentObject.IsInherited( CarScript ) && !dBodyIsActive( m_ExAttachmentObject ) )
					{
						//! Potential fix for player getting teleported to the sea (desync)
						EXPrint(GetType() + "::CommandHandler attach to " + m_ExAttachmentObject);
						dBodyActive( m_ExAttachmentObject, ActiveState.ACTIVE );
					}

					GetTransform( tmPlayer );
					m_ExAttachmentObject.GetTransform( tmTarget );
					Math3D.MatrixInvMultiply4( tmTarget, tmPlayer, tmLocal );

					LinkToLocalSpaceOf( m_ExAttachmentObject, tmLocal );

					m_ExAttachmentObject.Update();

					AttachmentDebugPrint( "END ATTACH" );

					break;
				}
			}
			
			// Send the user input to the server
			if ( GetGame().IsClient() )
			{
				AttachmentDebugPrint( "Sending Context to Server" );

				ScriptInputUserData ctx = new ScriptInputUserData;
				ctx.Write( EXPANSION_INPUT_UDT_PLAYER_LINK );
				ctx.Write( m_ExPlayerLinkType );

				if ( m_ExPlayerLinkType == ExpansionPlayerLink.ATTACH )
				{
					ctx.Write( m_ExAttachmentObject );
				}

				ctx.Send();

				AttachmentDebugPrint( "Sent Context to Server" );
			} else if ( !GetGame().IsMultiplayer() )
			{
				DayZPlayerSyncJunctures.ExpansionSendNextLink( this );
			}
		
			m_ExPlayerLinkType = ExpansionPlayerLink.NONE;

			AttachmentDebugPrint( "ExPlayerLinkType Processed" );
		}
	}
}