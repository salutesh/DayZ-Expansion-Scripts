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

	float eAI_SetupMelee()
	{
		m_Hands = m_AI.GetHumanInventory().GetEntityInHands();

		Reset(InventoryItem.Cast(m_Hands), GetMeleeHitType());
		TargetSelection();
		if (m_HitType == EMeleeHitType.NONE)
		{
			return false;
		}

		m_eAI_MeleeAttackType = 1.0;
		if (m_TargetObject && m_TargetType != EMeleeTargetType.NONALIGNABLE && vector.DistanceSq(m_AI.GetPosition(), m_TargetObject.GetPosition()) > Math.SqrFloat(DayZPlayerMeleeFightLogic_LightHeavy.CLOSE_TARGET_DISTANCE))
		{
			m_eAI_MeleeAttackType = 0.0;
		}

		if (m_TargetObject)
		{
			vector dir = vector.Direction(m_AI.GetPosition(), m_TargetObject.GetPosition());
			vector angles = dir.VectorToAngles();
			vector ori = m_AI.GetOrientation();
			ori[0] = angles[0];
			m_AI.SetOrientation(ori);
		}

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
	}

	void Combo(HumanCommandMelee2 hcm2)
	{
		if (!eAI_SetupMelee())
			return;

		hcm2.ContinueCombo(m_eAI_MeleeHeavy, m_eAI_MeleeAttackType, EntityAI.Cast(m_TargetObject), m_HitPositionWS);
	}

	void OnHit()
	{
		m_Hands = m_AI.GetHumanInventory().GetEntityInHands();
		
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

	EMeleeHitType GetMeleeHitType()
	{
		if (m_Hands && m_Hands.IsWeapon())
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
