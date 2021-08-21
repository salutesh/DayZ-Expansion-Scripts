/**
 * ExpansionPlayerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class PlayerBase
{
	private int m_ExpansionSaveVersion;

	protected bool m_WasInVehicle;
	
	// ------------------------------------------------------------
	// PlayerBase Constructor
	// ------------------------------------------------------------
	void PlayerBase()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::PlayerBase - Start");
		#endif
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::PlayerBase - End");
		#endif
	}

	// ------------------------------------------------------------
	// PlayerBase Destructor
	// ------------------------------------------------------------
	void ~PlayerBase()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::~PlayerBase - Start");
		#endif

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::~PlayerBase - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion SetActions
	// ------------------------------------------------------------
	override void SetActions( out TInputActionMap InputActionMap )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::SetActions start");
		#endif

		super.SetActions( InputActionMap );
		
		AddAction( ExpansionActionGetOutExpansionVehicle, InputActionMap );
		
		AddAction( ExpansionActionCarHorn, InputActionMap );
		AddAction( ExpansionActionHelicopterHoverRefill, InputActionMap );

		AddAction( ExpansionVehicleActionStartEngine, InputActionMap );
		AddAction( ExpansionVehicleActionStopEngine, InputActionMap );

		AddAction( ExpansionActionSwitchSeats, InputActionMap );
		
		AddAction( ExpansionActionStartMotor, InputActionMap );
		AddAction( ExpansionActionStopMotor, InputActionMap );

		AddAction( ExpansionVehicleActionStartMotor, InputActionMap );
		AddAction( ExpansionVehicleActionStopMotor, InputActionMap );

		AddAction( ExpansionActionSwitchBoatController, InputActionMap );
		AddAction( ExpansionActionSwitchBoatControllerInput, InputActionMap );

		AddAction( ExpansionActionPickVehicleLock, InputActionMap );
		AddAction( ExpansionVehicleActionPickLock, InputActionMap );
		AddAction( ExpansionActionChangeVehicleLock, InputActionMap );

		#ifdef EXPANSION_VEHICLE_TOWING
		AddAction( ExpansionActionConnectTow, InputActionMap );
		AddAction( ExpansionActionDisconnectTow, InputActionMap );
		#endif

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::SetActions end");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion EOnContact
	// ------------------------------------------------------------
	override private void EOnContact( IEntity other, Contact extra )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::EOnContact - Start");
		#endif

		Transport transport;
		if ( Class.CastTo( transport, other ) )
		{
			ExpansionRegisterTransportHit( transport );
		}

		ExpansionVehicleBase vehicle;
		if ( Class.CastTo( vehicle, other ) )
		{
			ExpansionRegisterTransportHit( vehicle );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::EOnContact - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionRegisterTransportHit
	// ------------------------------------------------------------
	override void RegisterTransportHit( Transport transport )
	{
		// Preventing vanilla (and other mods) code from running
	}
	
	// ------------------------------------------------------------
	// PlayerBase ExpansionRegisterTransportHit
	// ------------------------------------------------------------
	void ExpansionRegisterTransportHit( EntityAI transport )
	{
		bool hasParent = false;

		if ( GetParent() || GetCommand_Vehicle() )
			hasParent = true;

		if ( m_ExPlayerLinkType != ExpansionPlayerLink.NONE )
			hasParent = true;

		if ( m_TransportHitRegistered )
			return;

		m_TransportHitRegistered = hasParent;

		if ( !m_TransportHitRegistered )
		{
			m_TransportHitRegistered = true;
			m_TransportHitVelocity = GetVelocity( transport );

			if ( m_TransportHitVelocity.Length() > 2.5 )
			{
				float damage = m_TransportHitVelocity.Length();
				if ( transport && damage )
					ProcessDirectDamage( DT_CUSTOM, transport, "", "TransportHit", "0 0 0", damage );
			} else
			{
				m_TransportHitRegistered = false;
			}

			if ( m_TransportHitVelocity.Length() > 2.5 )
			{
				// dBodyApplyImpulse( this, dBodyGetMass( this ) * m_TransportHitVelocity * 40.0 );
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion ModCommandHandlerInside
	// ------------------------------------------------------------
	override bool ModCommandHandlerInside( float pDt, int pCurrentCommandID, bool pCurrentCommandFinished )	
	{
		if ( super.ModCommandHandlerInside( pDt, pCurrentCommandID, pCurrentCommandFinished ) )
		{
			return true;
		}

		if ( pCurrentCommandID == DayZPlayerConstants.COMMANDID_SCRIPT )
		{
			ExpansionHumanCommandVehicle ehcv = ExpansionHumanCommandVehicle.Cast( GetCommand_Script() );
			if ( ehcv != NULL )
			{
				if ( ehcv.WasGearChange() )
				{
					ExpansionGearChangeActionCallback cb = ExpansionGearChangeActionCallback.Cast( AddCommandModifier_Action( DayZPlayerConstants.CMD_ACTIONMOD_SHIFTGEAR, ExpansionGearChangeActionCallback ) );
					cb.SetVehicleCommand( ehcv );
				}
				return true;
			}
		}

		return false;
	}

	// ------------------------------------------------------------
	// Expansion StartCommand_ExpansionFall
	// ------------------------------------------------------------
	override void StartCommand_ExpansionFall( float pYVelocity )
	{
		#ifndef EXPANSION_DISABLE_FALL
		if ( !s_ExpansionPlayerAttachment )
		{
			StartCommand_Fall( pYVelocity );
			return;
		}

		if ( m_ExpansionST == NULL )
			m_ExpansionST = new ExpansionHumanST( this );

		ExpansionHumanCommandFall cmd = new ExpansionHumanCommandFall( this, pYVelocity, m_ExpansionST );
		StartCommand_Script( cmd );
		#else
		StartCommand_Fall( pYVelocity );
		#endif
	}

	// ------------------------------------------------------------
	// Expansion StartCommand_ExpansionVehicle
	// ------------------------------------------------------------
	override ExpansionHumanCommandVehicle StartCommand_ExpansionVehicle( ExpansionVehicleBase vehicle, int seatIdx, int seat_anim )
	{
		if ( m_ExpansionST == NULL )
			m_ExpansionST = new ExpansionHumanST( this );
	
		ExpansionHumanCommandVehicle cmd = new ExpansionHumanCommandVehicle( this, vehicle, seatIdx, seat_anim, m_ExpansionST );
		StartCommand_Script( cmd );
		return cmd;
	}

	// ------------------------------------------------------------
	// PlayerBase GetInVehicle
	// ------------------------------------------------------------
	bool GetInVehicle( )
	{
		return m_WasInVehicle;
	}

	// ------------------------------------------------------------
	// PlayerBase SetInVehicle
	// ------------------------------------------------------------
	void SetInVehicle( bool state )
	{
		EXPrint(ToString() + "::SetInVehicle " + state);
		m_WasInVehicle = state;
	}

	override bool IsInVehicle()
	{
		if (super.IsInVehicle())
			return true;

		if (GetCommand_ExpansionVehicle())
			return true;

		return false;
	}

	// ------------------------------------------------------------
	// PlayerBase OnCommandExpansionVehicleStart
	// ------------------------------------------------------------
	override void OnCommandExpansionVehicleStart()
	{
		super.OnCommandExpansionVehicleStart();

		if ( GetInventory() )
			GetInventory().LockInventory( LOCK_FROM_SCRIPT );

		ItemBase itemInHand = GetItemInHands();
		EntityAI itemOnHead = FindAttachmentBySlotName( "Headgear" );

		if ( itemInHand )
		{
			if ( itemInHand.GetCompEM() )
			{
				itemInHand.GetCompEM().SwitchOff();
			}

			GetItemAccessor().HideItemInHands(true);
		}

		if ( itemOnHead )
		{
			if ( itemOnHead.GetCompEM() )
			{
				itemOnHead.GetCompEM().SwitchOff();
			}
		}
		
		ExpansionHumanCommandVehicle hcv = GetCommand_ExpansionVehicle();
		if ( hcv && hcv.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER )
		{
			OnVehicleSeatDriverEnter();
		}
	}
	
	// ------------------------------------------------------------
	// PlayerBase OnCommandExpansionVehicleFinish
	// ------------------------------------------------------------
	override void OnCommandExpansionVehicleFinish()
	{
		super.OnCommandExpansionVehicleFinish();

		if ( GetInventory() )
			GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
			
		if ( GetItemInHands() )
		{
			GetItemAccessor().HideItemInHands(false);
		}
		
		if ( m_IsVehicleSeatDriver )
		{
			OnVehicleSeatDriverLeft();
		}
	}

	//! Called on both server + client when attaching to vehicle
	override void OnExpansionAttachTo( Object attachedToObj, vector tmLocal[4] )
	{
		EXPrint(ToString() + "::OnExpansionAttachTo " + attachedToObj);
		SetInVehicle( true );
	}

	//! Called on both server + client when detaching from vehicle
	override void OnExpansionDetachFrom( Object detachedFromObj )
	{
		EXPrint(ToString() + "::OnExpansionDetachFrom " + detachedFromObj);
		SetInVehicle( false );
	}

	// ------------------------------------------------------------
	// Expansion HeadingModel
	// ------------------------------------------------------------
	override bool HeadingModel( float pDt, SDayZPlayerHeadingModel pModel )
	{
		if ( GetCommand_ExpansionVehicle() )
		{
			m_fLastHeadingDiff = 0;

			pModel.m_fOrientationAngle = 0;
			//pModel.m_fHeadingAngle = 0;
			return true;
		}

		return super.HeadingModel( pDt, pModel );
	}

	// ------------------------------------------------------------
	// Expansion GetExpansionSaveVersion
	// OBSOLETE
	// ------------------------------------------------------------
	int GetExpansionSaveVersion()
	{
		return m_ExpansionSaveVersion;
	}

	// ------------------------------------------------------------
	// Expansion OnStoreSave
	// ------------------------------------------------------------
	override void OnStoreSave( ParamsWriteContext ctx )
	{
		#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("[VEHICLES] PlayerBase::OnStoreSave " + this + " " + GetGame().SaveVersion());
		#endif

		//! If we are saving after game version target for ModStorage support (1st stable)
		#ifdef CF_MODULE_MODSTORAGE
		if ( GetGame().SaveVersion() > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET )
		{
			super.OnStoreSave( ctx );
			return;
		}
		#endif

		m_ExpansionSaveVersion = EXPANSION_VERSION_CURRENT_SAVE;
		ctx.Write( m_ExpansionSaveVersion );

		//! If we are saving game version target for ModStorage support (1st stable) or later
		#ifdef CF_MODULE_MODSTORAGE
		if ( GetGame().SaveVersion() >= EXPANSION_VERSION_GAME_MODSTORAGE_TARGET )
		{
			super.OnStoreSave( ctx );
			return;
		}
		#endif

		super.OnStoreSave( ctx );
		
		ctx.Write( m_WasInVehicle );
	}
	
	// ------------------------------------------------------------
	// Expansion OnStoreLoad
	// ------------------------------------------------------------
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("[VEHICLES] PlayerBase::OnStoreLoad " + this + " " + version);
		#endif

		#ifdef CF_MODULE_MODSTORAGE
		if ( version > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET )
			return super.OnStoreLoad( ctx, version );
		#endif

		if ( Expansion_Assert_False( ctx.Read( m_ExpansionSaveVersion ), "[" + this + "] Failed reading m_ExpansionSaveVersion" ) )
			return false;

		#ifdef CF_MODULE_MODSTORAGE
		if ( m_ExpansionSaveVersion > EXPANSION_VERSION_SAVE_MODSTORAGE_TARGET )
			return super.OnStoreLoad( ctx, version );
		#endif
		
		//! With CF_ModStorage enabled, the code below won't be ran unless an old CE is loaded. To prevent server wipes, the code below will stay.

		if ( Expansion_Assert_False( super.OnStoreLoad( ctx, version ), "[" + this + "] Failed reading OnStoreLoad super" ) )
			return false;
		
		if ( Expansion_Assert_False( ctx.Read( m_WasInVehicle ), "[" + this + "] Failed reading m_WasInVehicle" ) )
			return false;

		return true;
	}

	#ifdef CF_MODULE_MODSTORAGE
	override void CF_OnStoreSave( CF_ModStorage storage, string modName )
	{
		#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("[VEHICLES] PlayerBase::CF_OnStoreSave " + this + " " + modName);
		#endif

		super.CF_OnStoreSave( storage, modName );

		if ( modName != "DZ_Expansion_Vehicles" )
			return;

		storage.Write( m_WasInVehicle );
	}
	
	override bool CF_OnStoreLoad( CF_ModStorage storage, string modName )
	{
		#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("[VEHICLES] PlayerBase::CF_OnStoreLoad " + this + " " + modName);
		#endif

		if ( !super.CF_OnStoreLoad( storage, modName ) )
			return false;

		if ( modName != "DZ_Expansion_Vehicles" )
			return true;

		if ( Expansion_Assert_False( storage.Read( m_WasInVehicle ), "[" + this + "] Failed reading m_WasInVehicle" ) )
			return false;

		return true;
	}
	#endif
	
	// ------------------------------------------------------------
	// Expansion AfterStoreLoad
	// ------------------------------------------------------------
	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();
		
		if ( GetGame().IsServer() )
		{
			if ( m_WasInVehicle )
			{
				vector rayStart = GetPosition();
				vector rayEnd = rayStart + "0 -6000 0";
				vector ground;
				
				RaycastRVParams params = new RaycastRVParams( rayStart, rayEnd, this, 0.2 );
				params.sorted = true;
				//params.type = ObjIntersectFire;
				params.flags = CollisionFlags.ALLOBJECTS;
				
				array< ref RaycastRVResult > results = new array< ref RaycastRVResult >();
				if ( DayZPhysics.RaycastRVProxy( params, results ) )
				{
					vector hitPos = vector.Zero;
					for (int i = 0; i < results.Count(); ++i)
					{
						RaycastRVResult currResult = results[i];
						if (!currResult)
							continue;
						
						Object obj;
						if (currResult.hierLevel > 0)
						{
							obj = currResult.parent;
						}	
						else
						{
							obj = currResult.obj;
						}
						
						if ( !obj || obj.IsTree() || obj.IsBush() || obj.IsScriptedLight() || obj.IsTransport() || obj.GetType() == string.Empty)
							continue;
						
						hitPos = currResult.pos;
						break;
					}
					
					if (hitPos != vector.Zero)
					{
						SetPosition( hitPos );
					} else
					{
						ground = Vector( GetPosition()[0], GetGame().SurfaceY( GetPosition()[0], GetPosition()[2] ), GetPosition()[2] );
						SetPosition( ground );
					}
				} else
				{
					ground = Vector( GetPosition()[0], GetGame().SurfaceY( GetPosition()[0], GetPosition()[2] ), GetPosition()[2] );
					SetPosition( ground );
				}

				m_WasInVehicle = false;
			}
		}
	}
}
