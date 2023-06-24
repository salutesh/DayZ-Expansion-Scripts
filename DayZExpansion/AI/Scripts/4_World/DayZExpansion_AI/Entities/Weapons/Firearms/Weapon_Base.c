/**
 * Weapon_Base.c
 * 
 * Partly based on Enfusion AI Project Copyright 2021 William Bowers
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Weapon_Base
{
	bool Hitscan(vector begin_point, vector direction, eAIBase ai, out Object hitObject, out vector hitPosition, out vector hitNormal, out int contactComponent)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "Hitscan");
		#endif

		vector end_point = begin_point + direction;

		set<Object> results = new set<Object>();
		bool hit = DayZPhysics.RaycastRV(begin_point, end_point, hitPosition, hitNormal, contactComponent, results, null, ai, false, false, ObjIntersectIFire, 0.01);
		
		if (hit)
		{
			if (results.Count() > 0)
				hitObject = results[0];
			return true;
		}

		hitPosition = end_point;

		return false;
	}

	override void EEFired(int muzzleType, int mode, string ammoType)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_3(this, "EEFired").Add(muzzleType).Add(mode).Add(ammoType);
		#endif

		super.EEFired(muzzleType, mode, ammoType);

		eAIBase ai;
		if (!Class.CastTo(ai, GetHierarchyRootPlayer()) || !ai.GetTarget()) return;
		
		vector begin_point = ai.GetBonePositionWS(ai.GetBoneIndexByName("neck"));
		vector direction = ai.GetWeaponAimDirection();

	#ifdef DIAG
		EXTrace.Print(EXTrace.AI, this, "EEFired " + direction);
	#endif

		Object hitObject;
		vector hitPosition;
		vector hitNormal;
		int contactComponent;
		bool hit = Hitscan(begin_point, direction * 1000.0, ai, hitObject, hitPosition, hitNormal, contactComponent);

		if (!hit)  //! Nothing hit
		{
			if (g_Game.IsServer() && EXTrace.AI)
			{
				vector aimPosition = ai.GetAimPosition();
				float aimDistance = vector.Distance(begin_point, aimPosition);
				vector missedPosition = begin_point + (direction * aimDistance);
				string missed;
				float missedUD = Math.AbsFloat(missedPosition[1] - aimPosition[1]);
				float missedLR = vector.Distance(missedPosition, aimPosition) - missedUD;
				if (missedUD > missedLR)
				{
					if (missedPosition[1] > aimPosition[1])
						missed = "high";
					else
						missed = "low";
				}
				else
				{
					if (ExpansionMath.Side(begin_point, aimPosition, missedPosition) > 0)
						missed = "left";
					else
						missed = "right";
				}
				EXTrace.Print(true, ai, "didn't hit because it was aiming " + missed + " (actual " + aimPosition + " missed " + missedPosition + ")");

				ai.Expansion_DebugObject_Deferred(18, "0 0 0", "ExpansionDebugBox");
				ai.Expansion_DebugObject_Deferred(19, missedPosition, "ExpansionDebugBox_Red", direction, begin_point);
			}

			return;
		}

		//! Calculate projectile impact velocity and resulting damage
		//! (initSpeed x initSpeedMultiplier) / (defaultDamageOverride value x typicalSpeed) x damage
		//! defaultDamageOverride[]={{0.5, 1}} means 50% to 100% of typical speed will have no drop
		//! airFriction = acceleration/v^2
		//! airFriction x v^2 = acceleration
		//! e.g. for 545x39:
		//! -0.00125 x (880*880 m/s) = -968 m/s^2
		//! damage after drop = v/v0
		//! TODO: Add defaultDamageOverride
		//! TODO: See if interpolation could be replaced with equation or maybe use the created bullet and move it in time?
		//! https://www.wolframalpha.com/input?i=curve+fitting
		float airFriction = GetGame().ConfigGetFloat("cfgAmmo " + ammoType + " airFriction");
		float initSpeed = GetGame().ConfigGetFloat("cfgAmmo " + ammoType + " initSpeed");
		float initSpeedMultiplier = ConfigGetFloat("initSpeedMultiplier");
		if (initSpeedMultiplier)
			initSpeed *= initSpeedMultiplier;
		float speed = initSpeed;
		vector dir = vector.Direction(begin_point, hitPosition);
		float distance = dir.Length();
		float simulationStep = 0.05;  //! How fine-grained our prediction will be (could use simulationStep config value, but it's always set to 0.05 anyway, so...)
		float distanceTraveled;
		float timeTraveled;
		int n;
		while (distanceTraveled < distance)
		{
			float speedLoss = airFriction * (speed * speed) * simulationStep;
			speed += speedLoss;
			distanceTraveled += speed * simulationStep;
			timeTraveled += simulationStep;
			n += 1;
			//! Limit number of iterations, may result in too high prediction for projectiles with low initSpeed and/or high airFriction
			//! Will mostly affect pistol rounds and shotgun shells where effective range is normally limited
			if (n > 100)
				break;
		}

		EntityAI targetEntity = ai.GetTarget().GetEntity();

		DayZPlayerImplement player;
		if (g_Game.IsClient() && Class.CastTo(player, hitObject))
		{
			//! MP client only!
			vector surfNormal = hitNormal * -1;
			vector inSpeed = direction * speed;
			player.eAI_SetFirearmEffectsOnHit(this, g_Game.GetTickTime() + 0.15 + timeTraveled, contactComponent, hitPosition, surfNormal, inSpeed, ammoType);
		}
	
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(eAI_HandleShot, timeTraveled * 1000, false, begin_point, hitPosition, direction, ai, targetEntity, speed, initSpeed, ai.m_eAI_DamageMultiplier, ammoType);
	}

	void eAI_HandleShot(vector origin, vector begin_point, vector direction, eAIBase ai, EntityAI targetEntity, float speed, float initSpeed, float dmgMult, string ammoType)
	{
	#ifdef DIAG
		EXTrace.Print(EXTrace.AI, this, "eAI_HandleShot " + begin_point + " " + direction);
	#endif

		Object hitObject;
		vector hitPosition;
		vector hitNormal;
		int contactComponent;
		bool hit = Hitscan(begin_point, direction * 10.0, ai, hitObject, hitPosition, hitNormal, contactComponent);

		if (!hit)  //! Nothing hit, player must've managed to moved out of the way
			return;

		if (g_Game.IsServer() && EXTrace.AI)
		{
			if (targetEntity && targetEntity == hitObject)
			{
				ai.Expansion_DebugObject_Deferred(18, hitPosition, "ExpansionDebugBox", direction, origin);
				ai.Expansion_DebugObject_Deferred(19, "0 0 0", "ExpansionDebugBox_Red");
			}
			else
			{
				ai.Expansion_DebugObject_Deferred(18, "0 0 0", "ExpansionDebugBox");
				ai.Expansion_DebugObject_Deferred(19, hitPosition, "ExpansionDebugBox_Red", direction, origin);
			}
		}

		//! Firearm FX for player on MP client handled by DZPlayerImplement::eAI_HandleFirearmEffects,
		//! no firearm FX on server if player cannot be damaged
		DayZPlayerImplement player;
		if (Class.CastTo(player, hitObject) && (g_Game.IsClient() || !player.Expansion_CanBeDamaged(ammoType)))
			return;

		EntityAI entity;
		if (g_Game.IsServer() && Class.CastTo(entity, hitObject))  //! Entity hit
		{
			string damageZone = "";
			TStringArray componentNameList();
			if (entity.GetActionComponentNameList(contactComponent, componentNameList, "fire") > 0)
			{
				foreach (string componentName: componentNameList)
				{
					if (DamageSystem.GetDamageZoneFromComponentName(entity, componentName, damageZone))
					{
						break;
					}
				}
			}
			if (!damageZone && entity.IsInherited(DayZPlayer))
				damageZone = "Torso";  //! Make sure we have a damageZone so that damage transfer to attachments works correctly

			float dmgCoef = speed / initSpeed;
			if (entity.IsMan())  //! Could also use IsPlayer() if we wanted to restrict to PlayerBase
				dmgCoef *= dmgMult;
			//EXPrint(ToString() + "::eAI_HandleShot " + ammoType + " coef " + dmgCoef + " damageZone " + damageZone);
			entity.ProcessDirectDamage(DT_FIRE_ARM, this, damageZone, ammoType, entity.WorldToModel(hitPosition), dmgCoef);
		}

		//! Under 1.21+, call FirearmEffects the same way vanilla would (source = weapon on server, NULL on client)

		Object source;
#ifdef SERVER
	#ifndef EXPANSION_AI_FIREARMFX_ENABLE_SERVER
		//! FIXME: Server sporadically gets stuck in infinite recursion of DayZGame::FirearmEffects,
		//! but not sure if that's the cause or just a side-effect (likely the latter?).
		//! Omit server-side call of FirearmEffects for now.
		if (ShootsExplosiveAmmo())
		{
			if (ammoType == "Bullet_40mm_ChemGas")
			{
				GetGame().CreateObject("ContaminatedArea_Local", hitPosition);
			}
			else if (ammoType == "Bullet_40mm_Explosive")
			{
				DamageSystem.ExplosionDamage(this, null, "Explosion_40mm_Ammo", hitPosition, DamageType.EXPLOSION);
			}
		}
		return;
	#endif

		source = this;
#endif
		vector surfNormal = hitNormal * -1;
		vector inSpeed = direction * speed;
		string surface = ExpansionStatic.GetImpactSurfaceType(hitObject, hitPosition, inSpeed);
		GetDayZGame().eAI_HandleFirearmEffects(source, hitObject, contactComponent, surface, hitPosition, surfNormal, inSpeed, ammoType);
	}

	//! Legacy, remove after next update
	void Expansion_HandleFirearmEffects(Object source, Object hitObject, int contactComponent, string surface, vector hitPosition, vector surfNormal, vector inSpeed, string ammoType)
	{
	}

	/**
	 * @fn	ProcessWeaponEvent
	 * @brief	weapon's fsm handling of events
	 * @NOTE: warning: ProcessWeaponEvent can be called only within DayZPlayer::HandleWeapons (or ::CommandHandler)
	 */
	override bool ProcessWeaponEvent(WeaponEventBase e)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "ProcessWeaponEvent").Add(e);
		#endif

		eAIBase ai;
		if (Class.CastTo(ai, e.m_player))
		{
			ScriptRemoteInputUserData ctx = new ScriptRemoteInputUserData;
			ctx.Write(INPUT_UDT_WEAPON_REMOTE_EVENT);
			e.WriteToContext(ctx);
			ai.StoreInputForRemotes(ctx);

			// @NOTE: synchronous events not handled by fsm
			if (e.GetEventID() == WeaponEventID.SET_NEXT_MUZZLE_MODE)
			{
				SetNextMuzzleMode(GetCurrentMuzzle());
				return true;
			}

			if (m_fsm.ProcessEvent(e) == ProcessEventResult.FSM_OK)
				return true;
				
			return false;
		}

		return super.ProcessWeaponEvent(e);
	}

	override bool ProcessWeaponAbortEvent(WeaponEventBase e)
	{
		if (PlayerBase.Cast(e.m_player).IsAI())
		{
			if (GetGame().IsServer())
			{
				ProcessEventResult result;
				m_fsm.ProcessAbortEvent(e, result);
				if (result == ProcessEventResult.FSM_OK)
					return true;
			}

			return false;
		}

		return super.ProcessWeaponAbortEvent(e);
	}

	override bool Expansion_TryTurningOnAnyLightsOrNVG(out float nightVisibility, PlayerBase player, bool skipNonNVG = false, bool skipNVG = false)
	{
		auto trace = EXTrace.Start(EXTrace.AI, this);

		ItemOptics optic;
		if (!skipNVG && Class.CastTo(optic, GetAttachedOptics()) && optic.GetCurrentNVType() != NVTypes.NONE)
		{
			nightVisibility = optic.GetZeroingDistanceZoomMax() * 0.001;
			EXTrace.Print(EXTrace.AI, player, "switched on " + optic.ToString());
			return true;
		}

		if ( skipNonNVG )
			return false;

		ActionTarget atrg;
		ActionManagerClient mngr_client;
		CastTo(mngr_client, player.GetActionManager());
		atrg = new ActionTarget(this, null, -1, vector.Zero, -1.0);

		if ( mngr_client.GetAction(ActionTurnOnWeaponFlashlight).Can(player, atrg, this) )
		{
			ItemBase itemChild;

			if ( IsInherited(Rifle_Base) )
			{
				itemChild = ItemBase.Cast(FindAttachmentBySlotName("weaponFlashlight"));
			}
			else if (IsInherited(Pistol_Base))
			{
				itemChild = ItemBase.Cast(FindAttachmentBySlotName("pistolFlashlight"));
			}

			if ( itemChild && itemChild.Expansion_TryTurningOn() )
			{
				FlashlightOn();
				nightVisibility = 0.15;
				EXTrace.Print(EXTrace.AI, player, "switched on " + itemChild.ToString());
				return true;
			}
		}

		return false;
	}

	override bool Expansion_TryTurningOffAnyLightsOrNVG(PlayerBase player, bool skipNVG = false)
	{
		auto trace = EXTrace.Start(EXTrace.AI, this);

		ActionTarget atrg;
		ActionManagerClient mngr_client;
		CastTo(mngr_client, player.GetActionManager());
		atrg = new ActionTarget(this, null, -1, vector.Zero, -1.0);

		if ( mngr_client.GetAction(ActionTurnOffWeaponFlashlight).Can(player, atrg, this) )
		{
			ItemBase itemChild;
			if ( IsInherited(Rifle_Base) )
			{
				itemChild = ItemBase.Cast(FindAttachmentBySlotName("weaponFlashlight"));
			}
			else if (IsInherited(Pistol_Base))
			{
				itemChild = ItemBase.Cast(FindAttachmentBySlotName("pistolFlashlight"));
			}

			if ( itemChild && itemChild.Expansion_TryTurningOff() )
			{
				FlashlightOff();
				EXTrace.Print(EXTrace.AI, player, "switched off " + itemChild.ToString());
				return true;
			}
		}

		return false;
	}
};
