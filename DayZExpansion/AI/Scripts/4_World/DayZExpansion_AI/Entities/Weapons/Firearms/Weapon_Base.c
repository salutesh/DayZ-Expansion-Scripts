// Copyright 2021 William Bowers
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

modded class Weapon_Base
{
	bool Hitscan(out EntityAI entity, out vector hitPosition, out int contactComponent)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "Hitscan");
		#endif

		eAIBase ai;
		if (!Class.CastTo(ai, GetHierarchyRootPlayer())) return false;
		
		vector position;
		vector direction;
		
		if (!ai.GetAimingProfile().Get(position, direction))
		{
			CF_Log.Error("Data invalid.");

			hitPosition = position;

			return false;
		}

		vector begin_point = position;
		vector end_point = position + (direction * 1000.0);
				
		// Prep Raycast
		set<Object> results = new set<Object>();
		vector hitNormal;
		bool hit = DayZPhysics.RaycastRV(begin_point, end_point, hitPosition, hitNormal, contactComponent, results, null, ai, false, false, ObjIntersectIFire, 0.01);
		
		if (hit)
		{
			if (results.Count() > 0)
				Class.CastTo(entity, results[0]);
			if (entity == ai.GetTarget().GetEntity())
			{
				ai.Expansion_DebugObject_Deferred(18, hitPosition, "ExpansionDebugBox", ai.GetDirection(), position);
				ai.Expansion_DebugObject_Deferred(19, "0 0 0", "ExpansionDebugBox_Red");
			}
			else
			{
				ai.Expansion_DebugObject_Deferred(18, "0 0 0", "ExpansionDebugBox");
				ai.Expansion_DebugObject_Deferred(19, hitPosition, "ExpansionDebugBox_Red", ai.GetDirection(), position);
			}
			return true;
		}

		hitPosition = begin_point;

		if (EXTrace.AI)
		{
			vector unitPosition = ai.GetPosition();
			vector aimPosition = ai.GetAimPosition();
			float distance = vector.Distance(unitPosition, aimPosition);
			vector missedPosition = position + (direction * distance);
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
				float x1 = unitPosition[0];
				float y1 = unitPosition[2];
				float x2 = aimPosition[0];
				float y2 = aimPosition[2];
				float x = missedPosition[0];
				float y = missedPosition[2];
				if ((x2 - x1) * (y - y1) > (y2 - y1) * (x - x1))
					missed = "left";
				else
					missed = "right";
			}
			EXTrace.Print(true, ai, "didn't hit because it was aiming " + missed + " (actual " + aimPosition + " missed " + missedPosition + ")");

			ai.Expansion_DebugObject_Deferred(18, "0 0 0", "ExpansionDebugBox");
			ai.Expansion_DebugObject_Deferred(19, missedPosition, "ExpansionDebugBox_Red", ai.GetDirection(), position);
		}

		return false;
	}

	override void EEFired(int muzzleType, int mode, string ammoType)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_3(this, "EEFired").Add(muzzleType).Add(mode).Add(ammoType);
		#endif

		super.EEFired(muzzleType, mode, ammoType);

		eAIBase ai;
		if (GetGame().IsClient() || !Class.CastTo(ai, GetHierarchyRootPlayer())) return;

		EntityAI entity;
		vector hitPosition;
		int contactComponent;
		bool hit = Hitscan(entity, hitPosition, contactComponent);

		if (!hit)  //! Nothing hit
			return;

		if (entity)  //! Entity hit
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
			float distance = vector.Distance(ai.GetPosition(), hitPosition);
			float simulationStep = 0.05;  //! How fine-grained our prediction will be (could use simulationStep config value, but it's always set to 0.05 anyway, so...)
			float distanceTraveled;
			int n;
			while (distanceTraveled < distance)
			{
				float speedLoss = airFriction * (speed * speed) * simulationStep;
				speed += speedLoss;
				distanceTraveled += speed * simulationStep;
				n += 1;
				//! Limit number of iterations, may result in too high prediction for projectiles with low initSpeed and/or high airFriction
				//! Will mostly affect pistol rounds and shotgun shells where effective range is normally limited
				if (n > 100)
					break;
			}

			float dmgCoef = speed / initSpeed;
			dmgCoef *= ai.m_eAI_DamageMultiplier;
			//EXPrint(ToString() + "::EEFired " + ammoType + " coef " + dmgCoef + " damageZone " + damageZone);
			entity.ProcessDirectDamage(DT_FIRE_ARM, this, damageZone, ammoType, entity.WorldToModel(hitPosition), dmgCoef);
		}

		//! Create bullet impact as visual cue
		Object bullet = GetGame().CreateObjectEx(ammoType, hitPosition, ECE_LOCAL | ECE_KEEPHEIGHT);
		if (bullet)
		{
			bullet.SetDirection(vector.Direction(ai.GetPosition(), hitPosition));
		}
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
};
