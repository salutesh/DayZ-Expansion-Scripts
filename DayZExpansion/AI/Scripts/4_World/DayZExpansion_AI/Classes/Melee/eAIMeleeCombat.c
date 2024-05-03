class eAIMeleeCombat : DayZPlayerImplementMeleeCombat
{
	eAIBase m_AI;
	EntityAI m_Hands;
	bool m_eAI_MeleeHeavy;
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
	
	override protected void TargetSelection()
	{
		auto trace = EXTrace.Profile(EXTrace.AI, this);

		vector hitPos;

		// Calculate max distances
		float dist = GetRange();
		float dist2 = Math.SqrFloat(dist);
		
		// There's generally 2 TargetSelection calls per animation
		// First to obtain target to move towards during animation
		// Second one is to see if that target is still in range before applying damage to it
		// m_WasHit means the hit event occured, so this is the second call

		if (m_WasHit && CanObjectBeTargeted(m_TargetObject))
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

	override protected bool HitZoneSelectionRaycast(out vector hitPos, out int hitZone, out Object target, bool useCamera)
	{
		auto trace = EXTrace.Profile(EXTrace.AI, this);

		vector pos;
		vector playerDir = m_AI.GetDirection();
		MiscGameplayFunctions.GetHeadBonePos(m_AI, pos);
		vector dir = vector.Direction(pos, m_AI.GetAimPosition()).Normalized();
		
		//! Prevents targeting of objects behind player
		if (vector.Dot(dir, playerDir) < 0.5)
		{
			return false;
		}

		m_RayStart = pos;
		m_RayEnd = m_RayStart + GetRange() * dir;
		
		// raycast
		set<Object> hitObjects = new set<Object>;
		vector hitNormal;

		if (DayZPhysics.RaycastRV(m_RayStart, m_RayEnd, hitPos, hitNormal, hitZone, hitObjects, null, m_AI, true, false, ObjIntersectIFire, 0.0, CollisionFlags.ALLOBJECTS) && hitObjects.Count() > 0)
		{
			EntityAI hitEntity;

			foreach (Object hitObject: hitObjects)
			{
				if (hitObject.IsBush())
					continue;

				target = hitObject;
				m_ForceUntargetable = false;

				//! Opponent is inside car - targeting range is shorter in that case
				PlayerBase playerTarget = PlayerBase.Cast(target);
				if (playerTarget && playerTarget.IsInVehicle())
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

				return true;
			}
		}

		return false;
	}

	bool eAI_SetupMelee(bool wasHit = false)
	{
		m_Hands = m_AI.GetHumanInventory().GetEntityInHands();

		Reset(InventoryItem.Cast(m_Hands), GetMeleeHitType(), wasHit);
		TargetSelection();
		if (m_HitType == EMeleeHitType.NONE || !m_TargetObject)
		{
			return false;
		}

		ItemBase item;
		if (Class.CastTo(item, m_TargetObject) && item.Expansion_IsMechanicalTrap())
			m_eAI_MeleeAttackType = 1.0;  //! Always use in-place attack for mechanical traps, as we don't want to step into it due to the forward movement
		else
			m_eAI_MeleeAttackType = m_AI.GetMeleeFightLogic().eAI_GetAttackTypeByDistanceToTarget(EntityAI.Cast(m_TargetObject), m_TargetType);

		m_AI.SetOrientation(m_AI.GetOrientation());

		//! https://feedback.bistudio.com/T173348
		if (m_HitType == EMeleeHitType.HEAVY || m_HitType == EMeleeHitType.WPN_STAB)
			m_eAI_MeleeHeavy = true;
		else
			m_eAI_MeleeHeavy = false;

		return true;
	}

	void Start()
	{
		if (!eAI_SetupMelee())
			return;

		m_AI.StartCommand_Melee2(EntityAI.Cast(m_TargetObject), m_eAI_MeleeHeavy, m_eAI_MeleeAttackType, m_HitPositionWS);

		eAI_DepleteStaminaAndApplyShock();
	}

	void Combo(HumanCommandMelee2 hcm2)
	{
		bool wasHit;
		if (GetFinisherType() == -1)
			wasHit = true;

		if (!eAI_SetupMelee(wasHit))
			return;

		hcm2.ContinueCombo(m_eAI_MeleeHeavy, m_eAI_MeleeAttackType, EntityAI.Cast(m_TargetObject), m_HitPositionWS);

		eAI_DepleteStaminaAndApplyShock();
	}

	void OnHit()
	{
		m_AI.GetMeleeFightLogic().eAI_EvaluateHit(InventoryItem.Cast(m_Hands));
	}

	void eAI_DepleteStaminaAndApplyShock()
	{
		int shock;

		switch (m_HitType)
		{
		case EMeleeHitType.HEAVY:
		case EMeleeHitType.SPRINT:
		case EMeleeHitType.WPN_STAB:
		case EMeleeHitType.WPN_HIT_BUTTSTOCK:
		case EMeleeHitType.WPN_HIT:
			m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_HEAVY);
			shock = PlayerConstants.BROKEN_LEGS_HEAVY_MELEE_SHOCK;
			break;
		default:
			m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_LIGHT);
			shock = PlayerConstants.BROKEN_LEGS_LIGHT_MELEE_SHOCK;
			break;
		}

		if (m_AI.GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS)
		{
			m_AI.m_ShockHandler.SetShock(shock);
			m_AI.m_ShockHandler.CheckValue(true);
		}
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

	EMeleeHitType GetMeleeHitType()
	{
		if (m_Hands && m_Hands.IsWeapon())
		{
			if (m_AI.CanConsumeStamina(EStaminaConsumers.MELEE_HEAVY))
			{
				if (m_Hands.HasBayonetAttached())
				{
					return EMeleeHitType.WPN_STAB;
				}
				else if (m_Hands.HasButtstockAttached())
				{
					return EMeleeHitType.WPN_HIT_BUTTSTOCK;
				}
		
				return EMeleeHitType.WPN_HIT;
			}
		
			return EMeleeHitType.NONE;
		}

		if (m_AI.CanConsumeStamina(EStaminaConsumers.MELEE_HEAVY))
		{
			if (m_AI.IsSprintFull())
			{
				return EMeleeHitType.SPRINT;
			}

			return EMeleeHitType.HEAVY;
		}

		return EMeleeHitType.LIGHT;
	}
};
