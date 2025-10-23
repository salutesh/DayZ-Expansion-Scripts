class eAIMeleeCombat : DayZPlayerImplementMeleeCombat
{
	eAIBase m_AI;
	float m_eAI_MeleeAttackType;

	void eAIMeleeCombat(DayZPlayerImplement player)
	{
		Class.CastTo(m_AI, player);
	}

	override protected bool CanObjectBeTargeted(Object obj, bool checkNonAligneAble = false)
	{
		if (super.CanObjectBeTargeted(obj, checkNonAligneAble))
			return true;

		ItemBase item;
		if (Class.CastTo(item, obj) && item.Expansion_IsMechanicalTrap())
			return true;

		return false;
	}
	
	//! Omits 2nd pass because it is not needed and EXPENSIVE on server
	override protected void TargetSelection()
	{
#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Profile(EXTrace.AI_PROFILE, this, "TargetSelection");
#endif 
		
		vector hitPos;

		// Calculate max distances
		float dist = GetRange();
		float dist2 = Math.SqrFloat(dist);
		
		// There's generally 2 TargetSelection calls per animation
		// First to obtain target to move towards during animation
		// Second one is to see if that target is still in range before applying damage to it
		// m_WasHit means the hit event occured, so this is the second call

		if (m_WasHit && GetFinisherType() == -1 && CanObjectBeTargeted(m_TargetObject))
		{
			hitPos = m_TargetObject.GetDamageZonePos(m_HitZoneName);

			// See if the component is still in range
			if (vector.DistanceSq(m_RayStart, hitPos) <= dist2)
			{
				m_AllTargetObjects.Insert(m_TargetObject);
				// This result should still be cached, so no need to fill it in again

				eAI_DebugHit(true, hitPos, m_RayStart);

				return;
			}
		}
		
		// Find a new target
		InternalResetTarget();
		
		Object target;
		int hitZone;
		
		// First pass - Prefer what the player is looking at (only aligneable ones, we will try for nonaligneable later)
		if (HitZoneSelectionRaycastHelper(hitPos, hitZone, target))
		{
			if (m_ForceUntargetable)
			{
				SetTarget(target, hitPos, hitZone);
				return;
			}

			if (CanObjectBeTargeted(target) && vector.DistanceSq(m_RayStart, hitPos) <= dist2)
			{
				m_AllTargetObjects.Insert(target);
				
				SetTarget(target, hitPos, hitZone);

				eAI_DebugHit(true, hitPos, m_RayStart);

				return;
			}
		}
		
		// Third pass - We found no aligneable target, check if the target that we are directly looking at is nonalignable (big)
		if (CanObjectBeTargeted(target, true) && vector.DistanceSq(m_RayStart, hitPos) <= dist2)
		{
			m_AllTargetObjects.Insert(target);

			SetTarget(target, hitPos, hitZone);

			eAI_DebugHit(true, hitPos, m_RayStart);

			return;
		}

		eAI_DebugHit(false, m_RayEnd, m_RayStart);
	}

	void eAI_DebugHit(bool hit, vector position, vector origin)
	{
		if (EXTrace.AI)
		{
			vector dir = vector.Direction(origin, position);

			if (hit)
			{
				m_AI.Expansion_DebugObject_Deferred(18, position, "ExpansionDebugSphereSmall", dir, origin);
				m_AI.Expansion_DebugObject_Deferred(19, "0 0 0", "ExpansionDebugSphereSmall_Red");
			}
			else
			{
				m_AI.Expansion_DebugObject_Deferred(18, "0 0 0", "ExpansionDebugSphereSmall");
				m_AI.Expansion_DebugObject_Deferred(19, position, "ExpansionDebugSphereSmall_Red", dir, origin);
			}
		}
	}

	bool eAI_IsAimingAhead(out vector pos = vector.Zero, out vector dir = vector.Zero)
	{
		//vector playerDir = m_AI.GetDirection();
		MiscGameplayFunctions.GetHeadBonePos(m_AI, pos);
		dir = vector.Direction(pos, m_AI.GetAimPosition()).Normalized();
		
		//! Prevents targeting of objects behind player
		float angle = m_AI.GetOrientation()[0];
		float aimAngle = dir.VectorToAngles()[0];
		//if (vector.Dot(dir, playerDir) < 0.5)
		if (Math.AbsFloat(ExpansionMath.AngleDiff2(angle, aimAngle)) > 60.0)
		{
			return false;
		}

		return true;
	}

	override protected bool HitZoneSelectionRaycast(out vector hitPos, out int hitZone, out Object target, bool useCamera)
	{
#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Profile(EXTrace.AI_PROFILE, this, "HitZoneSelectionRaycast");
#endif 
		
		vector pos;
		vector dir;
		
		if (!eAI_IsAimingAhead(pos, dir))
		{
			return false;
		}

		m_RayStart = pos;
		m_RayEnd = m_RayStart + GetRange() * dir;
		
		// raycast
		set<Object> hitObjects = new set<Object>;
		vector hitNormal;

		if (DayZPhysics.RaycastRV(m_RayStart, m_RayEnd, hitPos, hitNormal, hitZone, hitObjects, null, m_AI, false, false, ObjIntersectIFire, 0.0, CollisionFlags.ALLOBJECTS) && hitObjects.Count() > 0)
		{
			foreach (Object hitObject: hitObjects)
			{
				if (hitObject.IsBush() || hitObject.IsTree())
					continue;

				target = hitObject;
				m_ForceUntargetable = false;

				if (hitObject.IsBuilding())
					continue;

				//! Opponent is inside car - targeting range is shorter in that case
				PlayerBase playerTarget = PlayerBase.Cast(target);
				if (playerTarget && playerTarget.Expansion_IsInVehicleCmd())
				{
					if (vector.DistanceSq(m_RayStart, hitPos) > Math.SqrFloat(GetRange() * 0.5))
					{
						if (m_AI.GetGroup().GetFaction().GetMeleeYeetForce() >= 1.0)
						{
							target = Object.Cast(playerTarget.GetParent());
						}
						else
						{
							m_ForceUntargetable = true;
							target = null;
							hitPos = vector.Zero;
							hitZone = -1;
						}
					}
				}

				break;
			}
		}

		if (target)
			return true;

		return false;
	}

	override void Update(InventoryItem weapon, EMeleeHitType hitMask, bool wasHitEvent = false)
	{
		if (wasHitEvent)
		{
			//! Combo hit - keep weapon and reset state not related to target
			m_HitType = hitMask;
			m_SprintAttack = hitMask == EMeleeHitType.SPRINT;
			m_WeaponMode = SelectWeaponMode(weapon);
			m_WasHit = true;
			return;
		}

		Reset(weapon, hitMask, wasHitEvent);

		TargetSelection();
		SetFinisherType(TrySelectFinisherType(weapon, GetTargetEntity()));

		m_AI.SetOrientation(m_AI.GetOrientation());
	}

	//! Vanilla code is a bit inefficient, we can do better
	override protected int TrySelectFinisherType(InventoryItem weapon, EntityAI target)
	{
		if (m_WasHit)
			return -1;

		if (target && target.CanBeBackstabbed() && weapon && (weapon.IsMeleeFinisher() || m_HitType == EMeleeHitType.WPN_STAB) && !weapon.IsRuined())
		{
			vector dir = target.GetPosition() - m_DZPlayer.GetPosition();

			IEntity hitEntity;
			vector hitPos, hitNormal;
			
			float moveFraction = m_DZPlayer.CollisionMoveTest(dir, vector.Zero, 1.0, target, hitEntity, hitPos, hitNormal);
			if (moveFraction < 1.0)
				return -1;

			ZombieBase targetZombie = ZombieBase.Cast(target);;
			if (targetZombie)
			{
				//! check if attacker is in right pos and angle against victim
				if (!eAI_IsEntityBehindEntityInAngle(m_DZPlayer, target, 60))
				{
					return -1;
				}
				
				int mindState = targetZombie.GetMindStateSynced();
				//! Check if the infected is aware of the player
				if (mindState >= DayZInfectedConstants.MINDSTATE_DISTURBED)
				{
					return -1;
				}
			}

			if (weapon.IsWeapon())
			{
				return EMeleeHitType.WPN_STAB_FINISHER;
			}
			else
			{
				bool playGenericFinisherAnimation;

				if (targetZombie)
					playGenericFinisherAnimation = targetZombie.IsCrawling();
				else
					playGenericFinisherAnimation = true;

				if (playGenericFinisherAnimation)
					return EMeleeHitType.FINISHER_GENERIC;
				else
					return DetermineSpecificFinisherType(ItemBase.Cast(weapon));
			}
		}

		return -1;
	}

	bool eAI_IsEntityBehindEntityInAngle(EntityAI source, EntityAI target, float angle)
	{
		vector targetDirection = target.GetDirection();
		vector toSourceDirection = (source.GetPosition() - target.GetPosition());

		targetDirection[1] = 0;
		toSourceDirection[1] = 0;

		targetDirection.Normalize();
		toSourceDirection.Normalize();

		float cosFi = vector.Dot(targetDirection, toSourceDirection);
		vector cross = targetDirection * toSourceDirection;

		int hitDir = Math.Acos(cosFi) * Math.RAD2DEG;
		
		if (cross[1] < 0)
			hitDir = -hitDir;
		
		return hitDir <= (-180 + angle) || hitDir >= (180 - angle);
	}

	static void eAI_ApplyYeetForce(notnull eAIEntityTargetInformation info, float yeetForce, vector sourcePosition, vector yeetFactors)
	{
		if (yeetForce)
		{
			EntityAI targetEntity = info.GetEntity();
			float mass = dBodyGetMass(targetEntity);

			if (!mass)
				mass = targetEntity.GetWeightEx() / 1000.0;

			vector targetPosition = targetEntity.GetPosition() + info.GetAimOffset();

			for (int i = 0; i < 3; i++)
			{
				targetPosition[i] = ExpansionMath.LinearConversion(0.0, 1.0, yeetFactors[i], sourcePosition[i], targetPosition[i]);
			}

			vector yeetDirection = vector.Direction(sourcePosition, targetPosition).Normalized();

			EXTrace.Print(EXTrace.AI, targetEntity, "::eAI_ApplyYeetForce mass " + mass + " yeet " + yeetForce + " dir " + yeetDirection);

			dBodyApplyImpulse(targetEntity, yeetDirection * mass * yeetForce);

			AnimalBase animal;
			ZombieBase zombie;
			if (Class.CastTo(animal, targetEntity))
				animal.Expansion_SetAirborne(true);
			else if (Class.CastTo(zombie, targetEntity))
				zombie.Expansion_SetAirborne(true);
		}
	}

	//! Vanilla GetRange is protected...
	float eAI_GetRange()
	{
		return GetRange();
	}

	float eAI_GetRangeSq()
	{
		float range = GetRange();
		return range * range;
	}

	InventoryItem eAI_GetWeapon()
	{
		return m_Weapon;
	}
};
