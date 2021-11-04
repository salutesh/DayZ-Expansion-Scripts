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
	protected int m_Expansion_SessionTimeStamp;
	
	// ------------------------------------------------------------
	// PlayerBase Constructor
	// ------------------------------------------------------------
	void PlayerBase()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::PlayerBase - Start");
		#endif
		
		if (IsMissionHost())
			m_Expansion_SessionTimeStamp = GetDayZGame().ExpansionGetStartTime();
		
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
		
		//AddAction( ExpansionActionGetOutExpansionVehicle, InputActionMap );
		
		AddAction( ExpansionActionCarHorn, InputActionMap );
		AddAction( ExpansionActionHelicopterHoverRefill, InputActionMap );

		//AddAction( ExpansionVehicleActionStartEngine, InputActionMap );
		//AddAction( ExpansionVehicleActionStopEngine, InputActionMap );

		//AddAction( ExpansionActionSwitchSeats, InputActionMap );

		AddAction( ExpansionActionNextEngine, InputActionMap );
		AddAction( ExpansionActionNextEngineInput, InputActionMap );

		AddAction( ExpansionActionPickVehicleLock, InputActionMap );
		//AddAction( ExpansionVehicleActionPickLock, InputActionMap );
		AddAction( ExpansionActionChangeVehicleLock, InputActionMap );

		#ifdef EXPANSION_VEHICLE_TOWING
		AddAction( ExpansionActionConnectTow, InputActionMap );
		AddAction( ExpansionActionDisconnectTow, InputActionMap );

		//AddAction( ExpansionActionVehicleConnectTow, InputActionMap );
		//AddAction( ExpansionActionVehicleDisconnectTow, InputActionMap );
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

		if( !IsAlive() || GetParent() == other || !IsMissionHost() )
			return;

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

			if ( IsDamageDestroyed() && m_TransportHitVelocity.Length() > 3 )
			{
				vector impulse = 40 * m_TransportHitVelocity;
				impulse[1] = 40 * 1.5;
				dBodyApplyImpulse(this, impulse);
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
			auto ehcv = ExpansionHumanCommandVehicle.Cast( GetCommand_Script() );
			if (ehcv)
			{
				if (!ehcv.IsGettingIn() && !ehcv.IsGettingOut() && !ehcv.IsSwitchSeat())
				{
					ehcv.GetTransport().HandleController(this, pDt);
				}

				if ( ehcv.WasGearChange() )
				{
					ExpansionGearChangeActionCallback cb = ExpansionGearChangeActionCallback.Cast( AddCommandModifier_Action( DayZPlayerConstants.CMD_ACTIONMOD_SHIFTGEAR, ExpansionGearChangeActionCallback ) );
					cb.SetVehicleCommand( ehcv );
				}

				// Don't allow vanilla command handler to run from this point on, this is a replacement
				return true;
			}
		}

		if ( pCurrentCommandID == DayZPlayerConstants.COMMANDID_VEHICLE )
		{
			auto hcv = GetCommand_Vehicle();

			CarScript car;
			if (Class.CastTo(car, hcv.GetTransport()))
			{
				if (!hcv.IsGettingIn() && !hcv.IsGettingOut() && !hcv.IsSwitchSeat())
				{
					car.Expansion_HandleController(this, pDt);
				}
			}

			// Let vanilla handle the break for vehicles, this is an addition to functionality, not replacement
			return false;
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
	
		ExpansionHumanCommandVehicle cmd = new ExpansionHumanCommandVehicle( this, m_ExpansionST, vehicle, seatIdx, seat_anim );
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

	bool Expansion_IsDriver()
	{
		return m_IsVehicleSeatDriver;
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
	override void OnExpansionAttachTo( Object obj, vector transform[4] )
	{
		super.OnExpansionAttachTo(obj, transform);
		
		SetInVehicle( true );
	}

	//! Called on both server + client when detaching from vehicle
	override void OnExpansionDetachFrom( Object obj )
	{
		super.OnExpansionDetachFrom(obj);
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
		ctx.Write( m_Expansion_SessionTimeStamp );
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

		if ( m_ExpansionSaveVersion < 30 )
			return true;
		
		if ( Expansion_Assert_False( ctx.Read( m_Expansion_SessionTimeStamp ), "[" + this + "] Failed reading m_Expansion_SessionTimeStamp" ) )
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
		storage.Write( m_Expansion_SessionTimeStamp );
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

		if ( storage.GetVersion() < 30 )
			return true;
		
		if ( Expansion_Assert_False( storage.Read( m_Expansion_SessionTimeStamp ), "[" + this + "] Failed reading m_Expansion_SessionTimeStamp" ) )
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

		int sessionTimeStamp = m_Expansion_SessionTimeStamp;

		m_Expansion_SessionTimeStamp = GetDayZGame().ExpansionGetStartTime();

		if ( m_WasInVehicle )
		{
			ExpansionPPOGORIVMode mode = GetExpansionSettings().GetVehicle().PlacePlayerOnGroundOnReconnectInVehicle;

			if (mode == ExpansionPPOGORIVMode.Disabled)
				return;

			if (mode == ExpansionPPOGORIVMode.OnlyOnServerRestart && sessionTimeStamp == GetDayZGame().ExpansionGetStartTime())
				return;

			//! Temp god mode just to be safe
			SetAllowDamage(false);

			//! CallLater so vehicle attachment code etc has a chance to run first
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(PlacePlayerOnGround, 1500);
		}
	}

	void PlacePlayerOnGround()
	{
		if ( GetGame().IsServer() )
		{
			EXPrint(ToString() + "::PlacePlayerOnGround - player pos " + GetPosition() + " was in vehicle " + m_WasInVehicle + " is attached " + IsAttached() + " " + GetParent());

			if ( !IsAttached() && !GetParent() )
			{
				vector rayStart = GetPosition() + "0 0.6 0";

				//! Ground or water surface position
				vector ground = ExpansionStatic.GetSurfaceWaterPosition(GetPosition());
				//EXPrint(ToString() + "::PlacePlayerOnGround - ground " + ground);

				//! Move ray end up a bit from ground, so that if (e.g.) we are standing on the LHD,
				//! our 1st raycast doesn't hit the (water) surface below (RaycastRV will ignore the LHD),
				//! which would place the player in the water, inside the LHD's model
				vector rayEnd = ground + "0 1.5 0";

				Object ignoreObj = this;
				
				RaycastRVParams params = new RaycastRVParams( rayStart, rayEnd, ignoreObj, 0.2 );
				params.sorted = true;
				//params.type = ObjIntersectFire;
				params.flags = CollisionFlags.ALLOBJECTS;
				
				array< ref RaycastRVResult > results = new array< ref RaycastRVResult >();
				bool haveValidResult;
				if ( DayZPhysics.RaycastRVProxy( params, results ) )
				{
					//EXPrint(ToString() + "::PlacePlayerOnGround - ray 1 hit results: " + results.Count());
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
						
						//EXPrint(ToString() + "::PlacePlayerOnGround - ray 1 hit result hierlvl " + currResult.hierLevel + " parent " + currResult.parent + " obj " + currResult.obj + " hit pos " + currResult.pos);
						
						if ( !obj || obj.IsTree() || obj.IsBush() || obj.IsScriptedLight() || obj.GetType() == string.Empty)
							continue;
						
						//EXPrint(ToString() + "::PlacePlayerOnGround - ray 1 hit obj " + obj + " pos " + obj.GetPosition());
						if (obj.IsTransport())
						{
							//! If hit obj is more than 6m above ground, ignore it for next raycast
							if (ground[1] + 6 < obj.GetPosition()[1])
								ignoreObj = obj;
							//! Move up from hit pos for next raycast so that if we are standing on a vehicle,
							//! we don't get placed inside the vehicle's model
							rayStart[1] = currResult.pos[1] + 3;
							haveValidResult = false;
						}
						else
						{
							ground[1] = currResult.pos[1];
							haveValidResult = true;
						}
					}
				}

				if (!haveValidResult)
				{
					//! Do another raycast

					PhxInteractionLayers layerMask;

					layerMask |= PhxInteractionLayers.BUILDING;
					layerMask |= PhxInteractionLayers.VEHICLE;
					layerMask |= PhxInteractionLayers.ROADWAY;
					layerMask |= PhxInteractionLayers.TERRAIN;
					layerMask |= PhxInteractionLayers.ITEM_LARGE;

					vector hitPos;

					if (DayZPhysics.SphereCastBullet(rayStart, ground, 0.2, layerMask, ignoreObj, NULL, hitPos, NULL, NULL))
					{
						//EXPrint(ToString() + "::PlacePlayerOnGround - ray 2 hit pos " + hitPos);
						ground[1] = hitPos[1];
					}
				}

				//! If player is more than 1.5m above ground, place them safely,
				//! else just let them fall (no damage at that height)
				if (ground[1] + 1.5 < GetPosition()[1])
				{
					EXPrint(ToString() + "::PlacePlayerOnGround - placing player " + GetPosition() + " -> " + ground);
					SetPosition( ground );
				}

				m_WasInVehicle = false;
			}

			//! Disable temp god mode again if not in safezone - after a delay or player may still die from fall dmg
			if (!IsInSafeZone())
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetAllowDamage, 1500, false, true);
		}
	}
}
