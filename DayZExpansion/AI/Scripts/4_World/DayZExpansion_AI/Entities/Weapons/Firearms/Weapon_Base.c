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
	static ref map<string, float> s_Expansion_MinSafeFiringDistance = new map<string, float>;

	float m_eAI_LastFiredTime;

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

	bool eAI_Fire(int muzzleIndex, vector pos, vector dir, vector speed)
	{
	#ifdef SERVER
		return Fire(muzzleIndex, pos, dir, speed);
	#else
		return TryFireWeapon(this, muzzleIndex);
	#endif
	}

	override void EEFired(int muzzleType, int mode, string ammoType)
	{
		super.EEFired(muzzleType, mode, ammoType);

		if (GetGame().IsServer())
		{
			float time = GetGame().GetTickTime();
			if (time - m_eAI_LastFiredTime > 1.0)
			{
				//! Because shots may be fired rapidly, we only update this once every second
				m_eAI_LastFiredTime = time;
				float strengthMultiplier = GetPropertyModifierObject().eAI_GetNoiseShootModifier();
				if (strengthMultiplier)
					eAINoiseSystem.AddNoise(this, CFG_WEAPONSPATH + " " + GetType() + " NoiseShoot", strengthMultiplier, eAINoiseType.SHOT);
			}
		}

#ifdef DIAG
		if (!EXTrace.AI)
			return;

		eAIBase ai;
		if (!Class.CastTo(ai, GetHierarchyRootPlayer()) || !ai.GetTarget()) return;
		
		vector begin_point = ai.GetBonePositionWS(ai.GetBoneIndexByName("neck"));
		vector direction = ai.GetWeaponAimDirection();

		EXTrace.Print(true, this, "EEFired " + direction);

		Object hitObject;
		vector hitPosition;
		vector hitNormal;
		int contactComponent;
		bool hit = Hitscan(begin_point, direction * 1000.0, ai, hitObject, hitPosition, hitNormal, contactComponent);

		if (!hit)  //! Nothing hit
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

			ai.Expansion_DebugObject_Deferred(1818, "0 0 0", "ExpansionDebugSphereSmall");
			ai.Expansion_DebugObject_Deferred(1919, missedPosition, "ExpansionDebugSphereSmall_Red", direction, begin_point);
		}
		else
		{
			EntityAI targetEntity = ai.GetTarget().GetEntity();
			if (targetEntity && targetEntity == hitObject)
			{
				ai.Expansion_DebugObject_Deferred(1818, hitPosition, "ExpansionDebugSphereSmall", direction, begin_point);
				ai.Expansion_DebugObject_Deferred(1919, "0 0 0", "ExpansionDebugSphereSmall_Red");
			}
			else
			{
				ai.Expansion_DebugObject_Deferred(1818, "0 0 0", "ExpansionDebugSphereSmall");
				ai.Expansion_DebugObject_Deferred(1919, hitPosition, "ExpansionDebugSphereSmall_Red", direction, begin_point);
			}
		}
#endif
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

	float Expansion_GetMinSafeFiringDistance()
	{
		int muzzleIndex = GetCurrentMuzzle();

		string projectile;
		if (IsChamberFull(muzzleIndex))
			projectile = GetChamberedCartridgeMagazineTypeName(muzzleIndex);

		float minDist;

		if (!s_Expansion_MinSafeFiringDistance.Find(projectile, minDist))
		{
			float indirectHitRange = GetGame().ConfigGetFloat(CFG_AMMO + " " + projectile + " indirectHitRange");
			float indirectHitRangeMultiplier = GetGame().ConfigGetFloat(CFG_AMMO + " " + projectile + " indirectHitRangeMultiplier");

			minDist = indirectHitRange;
			if (indirectHitRangeMultiplier)
				minDist *= indirectHitRangeMultiplier;

		#ifdef DIAG
			EXTrace.Print(EXTrace.AI, this, projectile + " indirect hit range " + minDist);
		#endif

			//! TODO: Vanilla does this so awkward. The fired projectile is different from the explosive projectile
			//! which is only referenced in DayZGame::FirearmEffects, and there is no canonical way to get it from the fired projectile.
			//! Need to fall back to a hardcoded value for now :-(
			minDist = Math.Max(minDist, 20.0);

			s_Expansion_MinSafeFiringDistance[projectile] = minDist;
		}

		return minDist;
	}
};
