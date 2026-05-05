/**
 * ExpansionPlayerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class PlayerBase
{
	private int m_ExpansionSaveVersion;

	protected bool m_Expansion_ProcessTransportHit;
	protected bool m_Expansion_WasInVehicleSeatOrAttached;
	protected int m_Expansion_SessionTimeStamp;

	ref GetOutTransportActionData m_Expansion_QueuedJumpOutVehicleDamageActionData;
	
	ref array<ExpansionCarKey> m_Expansion_CarKeys;

	void PlayerBase()
	{
		if (IsMissionHost())
			m_Expansion_SessionTimeStamp = GetDayZGame().ExpansionGetStartTimestamp(true);
		
		m_Expansion_CarKeys = new array<ExpansionCarKey>;
	}

	override void Init()
	{
		super.Init();
		
		if (!m_ExpansionST)
			m_ExpansionST = new ExpansionHumanST(this);
	}

	override void SetActions( out TInputActionMap InputActionMap )
	{
		super.SetActions( InputActionMap );

		AddAction( ExpansionActionHelicopterHoverRefill, InputActionMap );

		AddAction( ExpansionActionNextEngine, InputActionMap );
		AddAction( ExpansionActionNextEngineInput, InputActionMap );

		//! Unlock action on player so we can use it inside vehicle as well
		AddAction(ExpansionActionUnlockVehicle, InputActionMap);

		AddAction( ExpansionActionPickVehicleLock, InputActionMap );
		AddAction( ExpansionActionChangeVehicleLock, InputActionMap );

		#ifdef EXPANSION_VEHICLE_TOWING
		AddAction( ExpansionActionConnectTow, InputActionMap );
		AddAction( ExpansionActionDisconnectTow, InputActionMap );
		#endif

		AddAction( ExpansionActionSwitchSeats, InputActionMap );
	}
	
	override void RegisterTransportHit( Transport transport )
	{
		if ( m_TransportHitRegistered )
			return;

		if ( PhysicsGetLinkedEntity() == transport || PhysicsGetFloorEntity() == transport )
			m_TransportHitRegistered = true;

		super.RegisterTransportHit(transport);
	}

	[Obsolete("DEPRECATED")]
	void ExpansionRegisterTransportHit( EntityAI transport )
	{
	}

	void Expansion_ProcessTransportHit(int damageType, EntityAI source, string dmgZone, vector modelPos, float damageCoef = 1.0)
	{
		if (!source)
			source = this;

		m_Expansion_ProcessTransportHit = true;
		ProcessDirectDamage(damageType, source, dmgZone, "TransportHit", modelPos, damageCoef);
	}

	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef))
			return false;

		if (ammo == "TransportHit")
		{
			float mult = GetExpansionSettings().GetVehicle().VehicleRoadKillDamageMultiplier;

			if (mult != 1.0 && !m_Expansion_ProcessTransportHit)
			{
				float damage;
				if (m_TransportHitRegistered)
					damage = m_TransportHitVelocity.Length();
				else
					damage = damageResult.GetDamage(dmgZone, "Health");
				damage *= mult;
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(Expansion_ProcessTransportHit, damageType, source, dmgZone, modelPos, damage);
				return false;
			}
		}

		return true;
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		
		m_Expansion_ProcessTransportHit = false;
	}

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
				ExpansionVehicleBase expansionVehicle;
				if (Class.CastTo(expansionVehicle, ehcv.GetObject()))
				{
					if (!ehcv.IsGettingIn() && !ehcv.IsGettingOut() && !ehcv.IsSwitchSeat())
					{
						expansionVehicle.HandleController(this, pDt);
					}
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
		#ifndef DAYZ_1_27
			// 1.28+
			// check if the car is not using new networking
			if (Class.CastTo(car, hcv.GetTransport()) && car.GetNetworkMoveStrategy() != NetworkMoveStrategy.PHYSICS)
		#else
			if (Class.CastTo(car, hcv.GetTransport()))
		#endif
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

	override ExpansionHumanCommandVehicle StartCommand_ExpansionVehicle( ExpansionVehicleBase vehicle, int seatIdx, int seat_anim )
	{
		//WARNING: memory leak
		ExpansionHumanCommandVehicle cmd = new ExpansionHumanCommandVehicle( this, m_ExpansionST, vehicle, seatIdx, seat_anim, false );
		StartCommand_Script( cmd );
		return cmd;
	}

    override void OnDisconnect()
    {
        ExpansionVehicle vehicle;

        if (ExpansionVehicle.Get(vehicle, this))
        {
            if (vehicle.CrewMemberIndex(this) == DayZPlayerConstants.VEHICLESEAT_DRIVER)
            {
				if (vehicle.IsTowing())
					vehicle.DestroyTow();

				if (vehicle.IsHelicopter())
				{
					if (!vehicle.IsAutoHover())
						vehicle.SwitchAutoHover();  //! Turn autohover on

					vehicle.EngineStop();  //! Stop engine. Heli will autorotate to ground.
				}
            }
        }

        super.OnDisconnect();
    }

	override void OnVehicleSeatDriverEnter()
	{
	#ifdef DIAG_DEVELOPER
		EXPrint(this, "OnVehicleSeatDriverEnter");
	#endif

		super.OnVehicleSeatDriverEnter();

		if (!g_Game.IsDedicatedServer())  //! Client or offline/SP
		{
			auto vehicle = ExpansionVehicle.Get(this);

			if (vehicle)
			{
				if (vehicle.IsHelicopter())
					g_Game.GetMission().AddActiveInputExcludes({"expansionhelicopter"});
				else if (vehicle.GetEntity().IsInherited(ExpansionBoatScript) && !vehicle.IsCar())
					g_Game.GetMission().AddActiveInputExcludes({"expansionboat"});
				else if (vehicle.IsPlane())
					g_Game.GetMission().AddActiveInputExcludes({"expansionplane"});
				else
				{
					//! Shouldn't technically be needed, but since 3rd party mods can break stuff by
					//! overriding OnVehicleSeatDriverLeft and not calling super, better be safe than sorry
					g_Game.GetMission().RemoveActiveInputExcludes({"expansionhelicopter"});
					g_Game.GetMission().RemoveActiveInputExcludes({"expansionboat"});
					g_Game.GetMission().RemoveActiveInputExcludes({"expansionplane"});
				}
			}
		}
	}

	override void OnVehicleSeatDriverLeft()
	{
	#ifdef DIAG_DEVELOPER
		EXPrint(this, "OnVehicleSeatDriverLeft");
	#endif

		super.OnVehicleSeatDriverLeft();

		if (!g_Game.IsDedicatedServer())  //! Client or offline/SP
		{
			g_Game.GetMission().RemoveActiveInputExcludes({"expansionhelicopter"});
			g_Game.GetMission().RemoveActiveInputExcludes({"expansionboat"});
			g_Game.GetMission().RemoveActiveInputExcludes({"expansionplane"});
		}
	}

	override bool OnLand(int pCurrentCommandID, FallDamageData fallDamageData)
	{
		GetOutTransportActionData gotActionData = m_Expansion_QueuedJumpOutVehicleDamageActionData;

		if (gotActionData)
		{
			ExpansionHelicopterScript heli;
			if (Class.CastTo(heli, gotActionData.m_Vehicle))
				heli.OnVehicleJumpOutServer(gotActionData);
		}

		m_Expansion_QueuedJumpOutVehicleDamageActionData = null;

		return super.OnLand(pCurrentCommandID, fallDamageData);
	}

	override void OnCommandSwimStart()
	{
		super.OnCommandSwimStart();

		m_Expansion_QueuedJumpOutVehicleDamageActionData = null;
	}

#ifdef DAYZ_1_25
	override void TryHideItemInHands(bool hide, bool force = false)
	{
		if (!hide && Expansion_IsAttached())
		{
			//! Vanilla turns false (= show) into true (= hide) if parent is transport (additional check added to IsInVehicle with 1.16). Force show item in hands.
			super.TryHideItemInHands(false, true);
			return;
		}
		
		super.TryHideItemInHands(hide, force);
	}
#endif

	bool Expansion_IsInVehicleSeatOrAttached( )
	{
	#ifndef DAYZ_1_25
		EXError.WarnOnce(this, "DEPRECATED");
	#endif

		return m_Expansion_WasInVehicleSeatOrAttached;
	}

	void Expansion_SetIsInVehicleSeatOrAttached( bool state )
	{
	#ifndef DAYZ_1_25
		EXError.WarnOnce(this, "DEPRECATED");
	#endif

		m_Expansion_WasInVehicleSeatOrAttached = state;
	}

	override bool IsInVehicle()
	{
#ifdef DAYZ_1_25
		if (Expansion_IsAttached())
			return false;
#endif
		
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

	override void OnCommandExpansionVehicleStart()
	{
		m_AnimCommandStarting = HumanMoveCommandID.CommandVehicle;
		
		if ( GetInventory() )
			GetInventory().LockInventory(LOCK_FROM_SCRIPT);
		
		ItemBase itemInHand = GetItemInHands();
		EntityAI itemOnHead = FindAttachmentBySlotName("Headgear");

		if ( itemInHand && itemInHand.GetCompEM() )
			itemInHand.GetCompEM().SwitchOff();

		TryHideItemInHands(true);

		if ( itemOnHead && itemOnHead.GetCompEM() )
			itemOnHead.GetCompEM().SwitchOff();
		
		auto hcv = GetCommand_ExpansionVehicle();
		if ( hcv && hcv.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER )
			OnVehicleSeatDriverEnter();
	}
	
	override void OnCommandExpansionVehicleFinish()
	{
		if ( GetInventory() )
			GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
		
		TryHideItemInHands(false, true);
		
		if ( m_IsVehicleSeatDriver )
			OnVehicleSeatDriverLeft();
	}

#ifdef DAYZ_1_25
	override void OnExpansionAttachTo( Object obj, vector transform[4] )
	{
		super.OnExpansionAttachTo(obj, transform);
		
		Expansion_SetIsInVehicleSeatOrAttached( true );
	}

	override void OnExpansionDetachFrom( Object obj )
	{
		super.OnExpansionDetachFrom(obj);

		Expansion_SetIsInVehicleSeatOrAttached( false );
	}
#endif

	override bool HeadingModel( float pDt, SDayZPlayerHeadingModel pModel )
	{
		return super.HeadingModel( pDt, pModel );
	}

	void SetHasCarKey(ExpansionCarKey key, bool state = true)
	{
		int idx = m_Expansion_CarKeys.Find(key);
		if (idx == -1 && state)
			m_Expansion_CarKeys.Insert(key);
		else if (!state)
			m_Expansion_CarKeys.Remove(idx);
	}

	ExpansionCarKey GetKeyForCar(CarScript car)
	{
		return GetKeyForCar((EntityAI) car);
	}

	ExpansionCarKey GetKeyForCar(EntityAI entity)
	{
		auto vehicle = ExpansionVehicle.Get(entity, true);

		if (!vehicle)
			return null;

		return GetKeyForCar(vehicle);
	}
	
	ExpansionCarKey GetKeyForCar(ExpansionVehicleBase vehicle)
	{
		return GetKeyForCar((EntityAI) vehicle);
	}

	ExpansionCarKey GetKeyForCar(ExpansionVehicle vehicle)
	{
		if (!vehicle.HasKey())
			return NULL;

		foreach (ExpansionCarKey key: m_Expansion_CarKeys)
		{
			if (key.IsInherited(ExpansionCarAdminKey))
				return key;

			if (key.IsPairedTo(vehicle))
				return key;
		}

		return NULL;
	}

	bool HasKeyForCar(CarScript car)
	{
		return HasKeyForCar((EntityAI) car);
	}

	bool HasKeyForCar(EntityAI entity)
	{
		if (GetKeyForCar(entity))
			return true;

		return false;
	}

	bool HasKeyForCar(ExpansionVehicle vehicle)
	{
		if (GetKeyForCar(vehicle))
			return true;

		return false;
	}
	
	bool HasKeyForCar(ExpansionVehicleBase vehicle)
	{
		return HasKeyForCar((EntityAI) vehicle);
	}

	/**
	 * @deprecated
	 */
	int GetExpansionSaveVersion()
	{
		return m_ExpansionSaveVersion;
	}

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_Vehicles];
		if (!ctx) return;

	#ifndef DAYZ_1_25
		//! 1.26+
		if (Expansion_GetParent())
			m_Expansion_WasInVehicleSeatOrAttached = true;
		else
			m_Expansion_WasInVehicleSeatOrAttached = false;
	#endif

		ctx.Write(m_Expansion_WasInVehicleSeatOrAttached);
		ctx.Write(m_Expansion_SessionTimeStamp);
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_Vehicles];
		if (!ctx) return true;

		if (!ctx.Read(m_Expansion_WasInVehicleSeatOrAttached))
			return false;

		if (!ctx.Read(m_Expansion_SessionTimeStamp))
			return false;

		return true;
	}
	#endif
	
	override void AfterStoreLoad()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CE, this, "LongDeferredInit");
#endif

		super.AfterStoreLoad();

		int sessionTimeStamp = m_Expansion_SessionTimeStamp;

		m_Expansion_SessionTimeStamp = GetDayZGame().ExpansionGetStartTimestamp(true);

		if ( m_Expansion_WasInVehicleSeatOrAttached )
		{
			ExpansionPPOGORIVMode mode = GetExpansionSettings().GetVehicle().PlacePlayerOnGroundOnReconnectInVehicle;

			if (mode == ExpansionPPOGORIVMode.Disabled)
				return;

			if (mode == ExpansionPPOGORIVMode.OnlyOnServerRestart && sessionTimeStamp == GetDayZGame().ExpansionGetStartTimestamp(true))
				return;

			//! Temporarily disable fall damage
			//! @note `FallDamage` is just a placeholder we use, it doesn't actually exist as ammo since DayZ 1.21
			Expansion_SetAllowDamageEx("FallDamage", false);

			//! CallLater so vehicle attachment code etc has a chance to run first
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(PlacePlayerOnGround, 1500);
		}
	}

	void PlacePlayerOnGround()
	{
		//todo: branchless ??
		if ( g_Game.IsServer() ) 
		{
			IEntity attachmentParent = Expansion_GetParent();

			EXPrint(ToString() + "::PlacePlayerOnGround - player pos " + GetPosition() + " was in vehicle " + m_Expansion_WasInVehicleSeatOrAttached + " is attached to " + attachmentParent);
			
			if (!attachmentParent)
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

				m_Expansion_WasInVehicleSeatOrAttached = false;
			}

			//! Enable fall damage again - after a delay or player may still die from it
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Expansion_SetAllowDamageEx, 1500, false, "FallDamage", true);
		}
	}
};
