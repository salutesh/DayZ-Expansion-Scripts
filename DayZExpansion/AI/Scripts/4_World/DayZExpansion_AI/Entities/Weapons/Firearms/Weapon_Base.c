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
	static const int MAX_RESYNC_ATTEMPTS = 12;
	static const int MIN_RESYNC_INTERVAL = 3000; //! ms
	static const int RESET_MAX_RESYNC_ATTEMPTS_THRESHOLD = 3600000; //! ms

	static ref map<string, float> s_Expansion_MinSafeFiringDistance = new map<string, float>;

	float m_eAI_LastFiredTime;
	float m_eAI_LastBulletImpactTime;
	int m_eAI_ShotID;
	int m_eAI_ResyncAttempts;
	int m_eAI_LastResyncTime;

	eAINoiseParams m_eAI_NoiseParams;

	bool Hitscan(vector begin_point, vector direction, eAIBase ai, out Object hitObject, out vector hitPosition, out vector hitNormal, out int contactComponent)
	{
		return Hitscan(begin_point, direction, 1100.0, ai, this, hitObject, hitPosition, hitNormal, contactComponent);
	}

	bool Hitscan(vector begin_point, vector direction, float distance, eAIBase ai, Object ignore, out Object hitObject, out vector hitPosition, out vector hitNormal, out int contactComponent)
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "Hitscan");
		#endif

		vector end_point = begin_point + direction * distance;

		set<Object> results = new set<Object>();
		bool hit = DayZPhysics.RaycastRV(begin_point, end_point, hitPosition, hitNormal, contactComponent, results, ai, ignore, false, false, ObjIntersectFire, 0.01);
		
		if (hit)
		{
			if (results.Count() > 0)
				hitObject = results[0];
			return true;
		}

		hitPosition = end_point;

		return false;
	}

	bool eAI_Fire(int muzzleIndex, eAIBase ai)
	{
	#ifdef SERVER
		vector pos = ai.GetBonePositionWS(ai.GetBoneIndexByName("neck"));
		vector dir = ai.GetWeaponAimDirection();

		eAITarget target = ai.GetTarget();
		EntityAI targetEntity = target.GetEntity();

		typename type = target.m_Info.Type();

#ifdef DIAG_DEVELOPER
		if (EXTrace.AI && ai.m_eAI_LastEngagedTargetType != type)
			EXTrace.Print(true, this, "Last engaged target type " + type);
#endif

		ai.m_eAI_LastEngagedTargetType = type;

		Object ignore = this;
		Object hitObject;
		vector hitPosition;
		vector hitNormal;
		int contactComponent;

		//! We check if we hit something in aim direction.
		//! Because projectiles may penetrate bushes, trees and even buildings,
		//! we continue the check until the hit object is not one of those, we exhaust the max distance or we reach max iterations.
		//! Note that it's not relevant here if the projectile cannot actually penetrate an obstacle, since that is checked by the damage handler.
		//! We are only interested in the object that could potentially be hit in the projectile's path.
		vector begPos = pos;
		float distance = 1100.0;
		bool hit;

		for (int i = 0; i < 3; i++)
		{
			hit = Hitscan(begPos, dir, distance, ai, ignore, hitObject, hitPosition, hitNormal, contactComponent);

			if (hitObject && hitObject != targetEntity)
			{
				if (hitObject.IsBush() || hitObject.IsTree() || hitObject.IsBuilding() || hitObject.IsPlainObject())
				{
					float hitDist = vector.Distance(begPos, hitPosition);

					if (hitDist > 0)
					{
						distance -= hitDist;

						if (distance > 0)
						{
							begPos = begPos + dir * hitDist;
							ignore = hitObject;
							continue;
						}
					}
				}
			}

			break;
		}

		eAIShot shot;

		if (hitObject)
		{
			shot = new eAIShot(this, muzzleIndex, pos, dir, hitObject, hitPosition, contactComponent);
			ai.m_eAI_FiredShots.Insert(shot);
		}
		else
		{
			//! We didn't hit a valid target, but we still want to correct for bullet drop.
			//! Just set hit position to aim position as we only use it for drop calc.
			shot = new eAIShot(this, muzzleIndex, pos, dir, null, ai.GetAimPosition(), contactComponent);
		}

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.AI, ai, shot.GetInfo());
	#endif

		eAI_DebugFire(hit, pos, dir, ai, target, hitObject, hitPosition);

		//! Compensate for bullet drop
		float drop = eAI_CalculateProjectileDrop(shot.m_TravelTime);
		if (drop > 0.1)
		{
			vector projectedPosition = pos + dir * shot.m_Distance;
			projectedPosition[1] = projectedPosition[1] + drop * 0.8;
			dir = vector.Direction(pos, projectedPosition).Normalized();
		}

		pos = pos + dir * 0.2;

		//! Hacky workaround for client FSM desync. Request firing of weapon on client outside FSM.
		if (GetExpansionSettings().GetAI().OverrideClientWeaponFiring)
			ai.eAI_FireWeaponOnClient(muzzleIndex, m_eAI_ShotID);

		++m_eAI_ShotID;

		return Fire(muzzleIndex, pos, dir, dir);
	#else
		if (GetExpansionSettings().GetAI().OverrideClientWeaponFiring)
			return true;

		return eAI_FireOnClient(muzzleIndex, ai, m_eAI_ShotID, true);
	#endif
	}

	bool eAI_FireOnClient(int muzzleIndex, eAIBase ai, int shotID, bool fromFSM = false)
	{
		//! XXX: Shot validation via ID doesn't work like this because obviously m_eAI_ShotID is reset on client when
		//! weapon leaves netbubble (and networking it would have the issue of potential desync).
		//! Maybe not really needed though, would just be nice to get an idea which shots are actually handled properly
		//! via client weapon FSM and which ones aren't.
		//if (shotID != m_eAI_ShotID)
		//{
			////! Shot was already fired, can safely skip.
			////! We always return true; in case this was called from FSM, it will help maintain appropriate state.
			//EXError.Warn(this, "Skip eAI_FireOnClient(fromFSM=" + fromFSM.ToString() + "), shotID " + shotID + " != " + m_eAI_ShotID + ", owner " + GetHierarchyRootPlayer(), {});
			//return true;
		//}

		//++m_eAI_ShotID;

		int mode = GetCurrentMode(muzzleIndex);
		string ammoType = GetChamberedCartridgeMagazineTypeName(muzzleIndex);

		bool result = TryFireWeapon(this, muzzleIndex);

		if (!result)
		{
		#ifdef DIAG_DEVELOPER
			EXError.Warn(ai, string.Format("TryFireWeapon(%1, %2) failed, shotID %3", this, muzzleIndex, shotID), {});
		#endif

			string msg;

			if (IsChamberEmpty(muzzleIndex) || IsChamberFiredOut(muzzleIndex))
			{
				//! Desynched - chamber - AI would not have fired if chamber was truly empty on authority (server)
				msg = "eAI_FireOnClient(fromFSM=" + fromFSM.ToString() + "), shotID " + shotID + ", owner " + GetHierarchyRootPlayer() + " - desynched (chamber empty or fired out)";
			}
			else
			{
				//! Desynched - other reason? (chamber still full)
				msg = "eAI_FireOnClient(fromFSM=" + fromFSM.ToString() + "), shotID " + shotID + ", owner " + GetHierarchyRootPlayer() + " - desynched (chamber full and not fired out)";
			}

			if (m_eAI_ResyncAttempts < MAX_RESYNC_ATTEMPTS)
				EXError.Warn(this, msg, {});
			else
				EXError.Error(this, msg + " - max resync attempts reached", {});

			vector pos = ai.GetBonePositionWS(ai.GetBoneIndexByName("neck"));
			vector dir = ai.Expansion_GetAimDirectionClient();

			//! This takes care of muzzle flash if successful (but no sound)
			result = Fire(muzzleIndex, pos, dir, dir);

		/*
			if (result)
			{
				//! XXX: The game won't play weapon soundsets manually via EffectSound >:(
				//! E.g.
				//! SCRIPT    (E): [EffectSound::SoundError] :: [ERROR] :: EffectSound<a43a3b30>: SoundSetName: 'IZH43_Shot_SoundSet' :: m_SoundObject is null.
				//! Also, no way to get the contents of soundSetShotExt and soundSetShotExt1st in script (nested arrays in config.cpp)

				ExpansionFireMode mode = Expansion_GetFireMode();

				switch (mode)
				{
					case ExpansionFireMode.INVALID:
						break;

					default:
						string modeClsName = typename.EnumToString(ExpansionFireMode, mode);
						string path = string.Format("%1 %2 %3 soundSetShot", CFG_WEAPONSPATH, GetType(), modeClsName);

						if (g_Game.ConfigIsExisting(path))
						{
							TStringArray soundSetShot = {};
							g_Game.ConfigGetTextArray(path, soundSetShot);

							foreach (string soundSet: soundSetShot)
							{
								SEffectManager.Expansion_PlaySound(soundSet, ai.GetPosition());
							}
						}

						break;
				}
			}
		*/

			if (!result)
			{
			#ifdef DIAG_DEVELOPER
				EXError.Warn(ai, string.Format("%1::Fire(%2, '%3', '%4', '%4') failed, shotID %5", this, muzzleIndex, pos.ToString(false), dir.ToString(false), shotID), {});
			#endif

				//! Ensure muzzle flash
				EEFired(muzzleIndex, mode, ammoType);
			}

			//! If the initial TryFireWeapon failed on client, we are definitely desynced since server side AI wouldn't have fired if
			//! weapon truly wasn't able to. Force (re-)sync but limit max attempts and interval (mirroring values in vanilla WeaponFSM)
			if (m_eAI_ResyncAttempts < MAX_RESYNC_ATTEMPTS)
			{
				int currentTime = g_Game.GetTime();
				int timeDiff = currentTime - m_eAI_LastResyncTime;

				if (timeDiff > MIN_RESYNC_INTERVAL)
				{
					if (timeDiff > RESET_MAX_RESYNC_ATTEMPTS_THRESHOLD)
						m_eAI_ResyncAttempts = 0;

					Synchronize();

					++m_eAI_ResyncAttempts;
					m_eAI_LastResyncTime = currentTime;
				}
			}
		}
		else if (m_eAI_ResyncAttempts > 0)
		{
			--m_eAI_ResyncAttempts;  //! Re-allow one resync attempt per succesful shot
		}

		return result;
	}

	override void EEFired(int muzzleType, int mode, string ammoType)
	{
		super.EEFired(muzzleType, mode, ammoType);

		if (g_Game.IsServer())
		{
			float time = g_Game.GetTickTime();
			if (time - m_eAI_LastFiredTime > 1.0)
			{
				//! Because shots may be fired rapidly, we only update this once every second
				m_eAI_LastFiredTime = time;
				float strengthMultiplier = GetPropertyModifierObject().eAI_GetNoiseShootModifier();
				if (strengthMultiplier)
					eAINoiseSystem.AddNoiseEx(this, eAI_GetNoiseParams(), strengthMultiplier);
			}
		}
		else
		{
			Man owner = GetHierarchyRootPlayer();

			if (!owner)
				return;

			ExpansionGame exGame = GetDayZGame().GetExpansionGame();
			if (!exGame.m_FirearmFXSource || owner.GetIdentity())
				exGame.m_FirearmFXSource = this;
		}
	}

	eAINoiseParams eAI_GetNoiseParams()
	{
		if (!m_eAI_NoiseParams)
			m_eAI_NoiseParams = eAINoiseSystem.GetNoiseParams(CFG_WEAPONSPATH + " " + GetType() + " NoiseShoot", eAINoiseType.SHOT);

		return m_eAI_NoiseParams;
	}

	void eAI_FirearmEffects(Object directHit, int componentIndex, string surface, vector pos, vector surfNormal,
		 vector exitPos, vector inSpeed, vector outSpeed, bool isWater, bool deflected, string ammoType)
	{
		float time = g_Game.GetTickTime();
		if (time - m_eAI_LastBulletImpactTime > 1.0)
		{
			//! Because shots may be fired rapidly, we only update this once every second
			m_eAI_LastBulletImpactTime = time;
			
			float surfaceCoef = g_Game.SurfaceGetNoiseMultiplier(directHit, pos, componentIndex);
			float coefAdjusted = surfaceCoef * inSpeed.Length() / g_Game.ConfigGetFloat("cfgAmmo " + ammoType + " initSpeed");
			if (coefAdjusted == 0)
				coefAdjusted = 1;
			coefAdjusted *= g_Game.GetWeather().GetNoiseReductionByWeather();
			eAINoiseSystem.AddNoise(this, pos, string.Format("cfgAmmo %1 NoiseHit", ammoType), coefAdjusted, eAINoiseType.BULLETIMPACT);
		}
	}

	override bool eAI_IsSilent()
	{
		//! Vanilla suppressors reduce noise by -0.85 (improvised) to -0.93 (AK/M4/pistol)
		//! We consider anything that results in a noise strength below 250 as silent (to have some headroom)
		//! @note vanilla noise strength values are multiplied by a certain factor in eAINoiseSystem if noise type is shot!
		float strengthMultiplier = GetPropertyModifierObject().eAI_GetNoiseShootModifier();
		if (strengthMultiplier)
		{
			eAINoiseParams params = eAI_GetNoiseParams();
			if (params.m_Strength * strengthMultiplier >= 250)
				return false;
		}

		return true;
	}

	void eAI_DebugFire(bool hit, vector begin_point, vector direction, eAIBase ai, eAITarget target, Object hitObject, vector hitPosition)
	{
#ifdef DIAG_DEVELOPER
		if (!EXTrace.AI)
			return;
		
		EXTrace.Print(true, this, "eAI_DebugFire " + direction);

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

			ai.Expansion_DebugObject_Deferred(1818, "0 0 0", "ExpansionDebugBox");
			ai.Expansion_DebugObject_Deferred(1919, missedPosition, "ExpansionDebugBox_Red", direction, begin_point);
		}
		else
		{
			//EntityAI targetEntity = target.GetEntity();
			//if (targetEntity && targetEntity == hitObject)
			if (hitObject)
			{
				ai.Expansion_DebugObject_Deferred(1818, hitPosition, "ExpansionDebugBox", direction, begin_point);
				ai.Expansion_DebugObject_Deferred(1919, "0 0 0", "ExpansionDebugBox_Red");
			}
			else
			{
				ai.Expansion_DebugObject_Deferred(1818, "0 0 0", "ExpansionDebugBox");
				ai.Expansion_DebugObject_Deferred(1919, hitPosition, "ExpansionDebugBox_Red", direction, begin_point);
			}
		}
#endif
	}

	//! @brief forces recreation of network representation (delete + create on client)
	void eAI_RemoteRecreate()
	{
		g_Game.RemoteObjectTreeDelete(this);
		g_Game.RemoteObjectTreeCreate(this);
	}

	/**
	 * @brief calculate projectile speed coefficient at position based on ammo
	 * 
	 * @param ammoType  e.g. "Bullet_308"
	 * @param hitPosition  position of bullet impact
	 * @param [out] airFriction
	 * @param [out] distance
	 * 
	 * @return speed coefficient
	 */
	float eAI_CalculateProjectileSpeedCoefAtPosition(string ammoType, vector hitPosition, out float airFriction = 0.0, out float distance = 0.0)
	{
		//! Cannot use barrel position since highly inaccurate on server
		vector origin = GetPosition();

		return eAI_CalculateProjectileSpeedCoefAtPosition(origin, ammoType, hitPosition, airFriction, distance);
	}

	float eAI_CalculateProjectileSpeedCoefAtPosition(vector origin, string ammoType, vector hitPosition, out float airFriction = 0.0, out float distance = 0.0)
	{
		airFriction = g_Game.ConfigGetFloat("CfgAmmo " + ammoType + " airFriction");
		distance = vector.Distance(origin, hitPosition);

		return Math.Pow(Math.EULER, airFriction * distance);
	}

	/**
	 * @brief calculate projectile damage coefficient at position based on ammo
	 * 
	 * @param ammoType  e.g. "Bullet_308"
	 * @param hitPosition  position of bullet impact
	 * @param damageOverride
	 * @param [out] airFriction
	 * @param [out] distance
	 * @param [out] speedCoef
	 * @param [out] initSpeed
	 * @param [out] speed
	 * 
	 * @return damage coefficient
	 */
	float eAI_CalculateProjectileDamageCoefAtPosition(string ammoType, vector hitPosition, float damageOverride = 1.0, out float airFriction = 0.0, out float distance = 0.0, out float speedCoef = 0.0, out float initSpeed = 0.0, out float speed = 0.0)
	{
		//! Cannot use barrel position since highly inaccurate on server
		vector origin = GetPosition();

		return eAI_CalculateProjectileDamageCoefAtPosition(origin, ammoType, hitPosition, damageOverride, airFriction, distance, speedCoef, initSpeed, speed);
	}

	float eAI_CalculateProjectileDamageCoefAtPosition(vector origin, string ammoType, vector hitPosition, float damageOverride = 1.0, out float airFriction = 0.0, out float distance = 0.0, out float speedCoef = 0.0, out float initSpeed = 0.0, out float speed = 0.0)
	{
		initSpeed = g_Game.ConfigGetFloat("CfgAmmo " + ammoType + " initSpeed");
		float initSpeedMultiplier = ConfigGetFloat("initSpeedMultiplier");

		if (initSpeedMultiplier)
			initSpeed *= initSpeedMultiplier;

		float typicalSpeed = g_Game.ConfigGetFloat("CfgAmmo " + ammoType + " typicalSpeed");

		typicalSpeed *= damageOverride;

		speedCoef = eAI_CalculateProjectileSpeedCoefAtPosition(origin, ammoType, hitPosition, airFriction, distance);

		speed = initSpeed * speedCoef;

		float dmgCoef;

		if (typicalSpeed != initSpeed)
		{
			if (speed > typicalSpeed)
				dmgCoef = 1.0;
			else
				dmgCoef = speed / typicalSpeed;
		}
		else
		{
			dmgCoef = speedCoef;
		}

		return dmgCoef;
	}

	/**
	 * @brief Calculate projectile travel time over distance in seconds
	 */
	float eAI_CalculateProjectileTravelTime(float airFriction, float distance, float initSpeed, float simulationStep = 0.05)
	{
		float distanceTraveled;
		float distanceTraveledPrev;

		float timeTraveled;
		float timeTraveledPrev;

		float speed;

		//! In DayZ, max projectile travel time is 6 seconds
		while (distanceTraveled < distance && timeTraveled < 6.0)
		{
			timeTraveledPrev = timeTraveled;
			timeTraveled += simulationStep;
			speed = Math.Pow(Math.EULER, airFriction * distanceTraveled) * initSpeed;
			if (speed <= 0)  //! speed should never reach zero with properly configured airFriction, initSpeed and initSpeedMultiplier in ammo
				break;
			distanceTraveledPrev = distanceTraveled;
			distanceTraveled += speed * simulationStep;
		}

		if (distanceTraveled > distanceTraveledPrev)
			return ExpansionMath.LinearConversion(distanceTraveledPrev, distanceTraveled, distance, timeTraveledPrev, timeTraveled);

		return timeTraveled;
	}

	/**
	 * @brief Calculate projectile drop after travel time in meters
	 */
	float eAI_CalculateProjectileDrop(float travelTime)
	{
		float g = 9.81;  //! Gravity in m/s^2

		return 0.5 * g * Math.Pow(travelTime, 2.0);
	}

	/**
	 * @brief Sets firemode to fullauto or burst, if supported
	 */
	void eAI_SetFireModeAuto()
	{
		if (!Expansion_SetFireMode(ExpansionFireMode.FullAuto))
			Expansion_SetFireMode(ExpansionFireMode.Burst);
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
			if (g_Game.IsServer())
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

	override ItemBase Expansion_TryTurningOnAnyLightOrNVG(bool skipNonNVG = false, bool skipNVG = false)
	{
		ItemBase item = Expansion_GetAnyLightOrNVG(skipNonNVG, skipNVG);

		if (item)
		{
			if (item.IsInherited(ItemOptics) && (!item.HasEnergyManager() || item.GetCompEM().CanWork()))
				return item;

			if (item.Expansion_TryTurningOn())
			{
				FlashlightOn();
				return item;
			}
		}

		return null;
	}

	override ItemBase Expansion_GetAnyLightOrNVG(bool skipNonNVG = false, bool skipNVG = false)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
#endif

		ItemOptics optic;
		if (!skipNVG && Class.CastTo(optic, GetAttachedOptics()) && optic.GetCurrentNVType() != NVTypes.NONE)
			return optic;

		if (!skipNonNVG)
		{
			ItemBase flashlight;

			if (IsInherited(Rifle_Base))
				flashlight = ItemBase.Cast(FindAttachmentBySlotName("weaponFlashlight"));
			else if (IsInherited(Pistol_Base))
				flashlight = ItemBase.Cast(FindAttachmentBySlotName("pistolFlashlight"));

			if (flashlight)
				return flashlight;
		}

		return null;
	}

	override ItemBase Expansion_TryTurningOffAnyLightOrNVG(bool skipNVG = false)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
#endif

		ItemBase item = Expansion_GetAnyLightOrNVG(false, skipNVG);

		if (item)
		{
			if (item.IsInherited(ItemOptics))
				return item;

			if (item.Expansion_TryTurningOff())
			{
				FlashlightOff();
				return item;
			}
		}

		return null;
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
			float indirectHitRange = g_Game.ConfigGetFloat(CFG_AMMO + " " + projectile + " indirectHitRange");
			float indirectHitRangeMultiplier = g_Game.ConfigGetFloat(CFG_AMMO + " " + projectile + " indirectHitRangeMultiplier");

			minDist = indirectHitRange;
			if (indirectHitRangeMultiplier)
				minDist *= indirectHitRangeMultiplier;

		#ifdef DIAG_DEVELOPER
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
