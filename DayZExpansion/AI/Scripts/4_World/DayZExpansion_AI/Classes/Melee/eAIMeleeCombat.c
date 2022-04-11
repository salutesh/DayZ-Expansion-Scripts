class eAIMeleeCombat : DayZPlayerImplementMeleeCombat
{
	eAIBase m_AI;
	EntityAI m_Hands;

	void eAIMeleeCombat(DayZPlayerImplement player)
	{
		Class.CastTo(m_AI, player);
	}

	void Start()
	{
		m_Hands = m_AI.GetHumanInventory().GetEntityInHands();

		//GetMeleeTarget();
		//GetHitZone();
		//m_HitType = GetMeleeHitType();
		//m_SprintAttack = m_HitType == EMeleeHitType.SPRINT;
		Reset(InventoryItem.Cast(m_Hands), GetMeleeHitType());
		TargetSelection();
		if (m_HitType == EMeleeHitType.NONE)
		{
			return;
		}

		float meleeAttackType = 1.0;
		if (m_TargetObject && m_TargetType != EMeleeTargetType.NONALIGNABLE && vector.DistanceSq(m_AI.GetPosition(), m_TargetObject.GetPosition()) > 1.44)
		{
			meleeAttackType = 0.0;
		}

		m_AI.StartCommand_Melee2(EntityAI.Cast(m_TargetObject), m_HitType == EMeleeHitType.HEAVY || m_HitType == EMeleeHitType.SPRINT, meleeAttackType, m_HitPositionWS);
	}

	void Combo(HumanCommandMelee2 hcm2)
	{
		m_Hands = m_AI.GetHumanInventory().GetEntityInHands();

		//GetMeleeTarget();
		//GetHitZone();
		//m_HitType = GetMeleeHitType();
		Reset(InventoryItem.Cast(m_Hands), GetMeleeHitType());
		TargetSelection();
		if (m_HitType == EMeleeHitType.NONE)
		{
			return;
		}

		float meleeAttackType = 1.0;
		if (m_TargetObject && m_TargetType != EMeleeTargetType.NONALIGNABLE && vector.DistanceSq(m_AI.GetPosition(), m_TargetObject.GetPosition()) > 1.44)
		{
			meleeAttackType = 0.0;
		}

		hcm2.ContinueCombo(m_HitType == EMeleeHitType.HEAVY || m_HitType == EMeleeHitType.SPRINT, meleeAttackType, EntityAI.Cast(m_TargetObject), m_HitPositionWS);
	}

	void OnHit()
	{
		m_Hands = m_AI.GetHumanInventory().GetEntityInHands();
		
		//GetMeleeTarget();
		m_HitType = GetMeleeHitType();
		m_AI.GetMeleeFightLogic().eAI_EvaluateHit(InventoryItem.Cast(m_Hands));

		switch (m_HitType)
		{
		case EMeleeHitType.HEAVY:
		case EMeleeHitType.SPRINT:
		case EMeleeHitType.WPN_STAB:
		case EMeleeHitType.WPN_HIT_BUTTSTOCK:
		case EMeleeHitType.WPN_HIT:
			m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_HEAVY);
			break;
		default:
			m_DZPlayer.DepleteStamina(EStaminaModifiers.MELEE_LIGHT);
			break;
		}
	}

//	void GetMeleeTarget()
//	{
//		vector pos = m_AI.GetPosition();
//		vector dir = MiscGameplayFunctions.GetHeadingVector(m_AI);
//
//		float dist = m_WeaponRange + RANGE_EXTENDER_NORMAL;
//		float tgtAngle = TARGETING_ANGLE_NORMAL;
//		if (m_SprintAttack)
//		{
//			dist = m_WeaponRange + RANGE_EXTENDER_SPRINT;
//			tgtAngle = TARGETING_ANGLE_SPRINT;
//		}
//
//		m_TargetObject = DayZPlayerUtils.GetMeleeTarget(pos, dir, tgtAngle, dist, TARGETING_MIN_HEIGHT, TARGETING_MAX_HEIGHT, m_AI, m_TargetableObjects, m_AllTargetObjects);
//EXPrint("GetMeleeTarget " + m_TargetObject);
//		if (IsObstructed(m_TargetObject))
//			m_TargetObject = null;
//EXPrint("    GetMeleeTarget " + m_TargetObject);
//
//		m_WeaponMode = SelectWeaponMode(InventoryItem.Cast(m_Hands));
//		m_WeaponRange = GetWeaponRange(InventoryItem.Cast(m_Hands), m_WeaponMode);
//		m_AllTargetObjects.Clear();
//	}

//	void GetHitZone()
//	{
//		vector pos = m_AI.GetPosition() + "0.0 1.5 0.0";
//		vector dir = m_AI.GetDirection();
//
//		m_RayStart = pos;
//		m_RayEnd = pos + (dir * TARGETING_RAY_DIST);
//
//		set<Object> hitObjects = new set<Object>;
//		int hitComponentIndex;
//		float hitFraction;
//		vector start, end, hitNormal, hitPosObstructed;
//
//		DayZPhysics.RaycastRV(m_RayStart, m_RayEnd, m_HitPositionWS, hitNormal, m_HitZoneIdx, hitObjects, null, m_AI, false, false, ObjIntersectIFire, TARGETING_RAY_RADIUS);
//	}

	EMeleeHitType GetMeleeHitType()
	{
		bool isFireWeapon = m_Hands && m_Hands.IsWeapon();
		
		if (isFireWeapon)
		{
			//if (m_AI.CanConsumeStamina(EStaminaConsumers.MELEE_HEAVY))
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