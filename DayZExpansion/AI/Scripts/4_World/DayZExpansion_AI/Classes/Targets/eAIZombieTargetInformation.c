class eAIZombieTargetInformation: eAIEntityTargetInformation
{
	private ZombieBase m_Zombie;

	bool m_Crawling;

	void eAIZombieTargetInformation(EntityAI target)
	{
		Class.CastTo(m_Zombie, target);
	}

	override bool IsZombie()
	{
		return true;
	}

	override bool IsCrawling()
	{
		return m_Crawling;
	}

	override bool IsFighting()
	{
		if (m_Zombie.m_eAI_AttackCooldown > 0 || m_Zombie.GetCommand_Attack())
			return true;

		return false;
	}

	override bool IsAcuteDanger(eAIBase ai = null)
	{
		if (m_Zombie.Expansion_GetTarget() == ai)
			return true;

		return false;
	}

	override float GetAttackCooldown()
	{
		return m_Zombie.m_eAI_AttackCooldown;
	}

	override vector GetAimOffset(eAIBase ai = null)
	{
		string boneName;

		Weapon_Base weapon;
		if (ai && Class.CastTo(weapon, ai.GetHumanInventory().GetEntityInHands()))
		{
			if (weapon.ShootsExplosiveAmmo())
				boneName = "spine3";
			else
				boneName = "neck";
		}
		else if (m_Crawling)
		{
			boneName = "pelvis";  //! Aim even lower for melee if crawling
		}
		else
		{
			boneName = "spine3";  //! Aim lower for melee
		}

		vector pos;
		int index = m_Zombie.GetBoneIndexByName(boneName);
		if (index > -1)
			pos = m_Zombie.GetBonePositionWS(index);
		else
			pos = m_Zombie.GetCenter();

		pos = pos - m_Zombie.GetPosition();

		return pos;
	}

	// https://www.desmos.com/calculator/r4mqu91qff
	override float CalculateThreat(eAIBase ai = null, eAITargetInformationState state = null)
	{
		if (m_Zombie.IsDamageDestroyed())
			return 0.0;

		if (!m_Zombie.Expansion_IsDanger())
		{
			if (ai && GetVelocity(m_Zombie).LengthSq() > 0.1388)  //! 0.5 km/h
				return ExpansionMath.LinearConversion(0.0, 15.0, GetDistance(ai, true), 0.15, 0.0);  //! 0.1 at 5 m

			return 0.0;
		}

		float levelFactor;

		int level = m_Zombie.Expansion_GetMindState();
		switch (level)
		{
		case DayZInfectedConstants.MINDSTATE_CALM:
			levelFactor = 0.00;
			break;
		case DayZInfectedConstants.MINDSTATE_DISTURBED:
			levelFactor = 0.25;  //! Threat level 0.4 at 6.25m
			break;
		case DayZInfectedConstants.MINDSTATE_ALERTED:
			levelFactor = 0.60;  //! Threat level 0.4 at 15m
			break;
		case DayZInfectedConstants.MINDSTATE_CHASE:
			levelFactor = 1.20;  //! Threat level 0.4 at 30m
			break;
		case DayZInfectedConstants.MINDSTATE_FIGHT:
			levelFactor = 2.00;
			break;
		}

		if (ai)
		{
			if (ai.eAI_GetThreatOverride(m_Target))
				return 0.0;

			//! The further away the zombie, the less likely it will be a threat
			float distance = GetDistance(ai, true);

			//! If not reachable, ignore if we don't have a gun
			if (!ai.m_eAI_HasProjectileWeaponInHands && ai.eAI_IsUnreachable(2.0, m_Target.GetPosition()))
			{
				ai.eAI_ThreatOverride(m_Target, true);
				return 0.0;
			}

			if (!levelFactor)
			{
				//! Exception: Zombie is near, not (yet) aggroed and AI has no weapon - pre-empt zombie attacking by going on the offense
				EntityAI hands = ai.GetHumanInventory().GetEntityInHands();
				if (distance <= 6.25 && (!hands || !hands.IsWeapon()))
					levelFactor = 0.25;
				else
					return ExpansionMath.LinearConversion(0.0, 45.0, distance, 0.15, 0.0);  //! 0.1 at 15 m
			}

			levelFactor *= 10 / (distance + 0.1);
			if (levelFactor > 1.0)
				levelFactor = Math.Pow(levelFactor, 2.0);

			if (m_Zombie.Expansion_GetActualTarget() == ai)
			{
				levelFactor *= 2.0;
				if (hands && hands.IsWeapon())
					eAIPlayerTargetInformation.AdjustThreatLevelBasedOnWeapon(hands, distance, levelFactor);
			}

			levelFactor *= ai.eAI_GetThreatDistanceFactor(distance);
		}

		return Math.Clamp(levelFactor, 0.0, 1000000.0);
	}

	override bool ShouldRemove(eAIBase ai = null)
	{
		return GetThreat(ai) <= 0.1;
	}

	override float GetMinDistance(eAIBase ai = null, float distance = 0.0)
	{
		//! @note at health below 30%, we are too slow to flee from Zs that are close and are better off fighting
		if (ai)
		{
			if (ai.m_eAI_AcuteDangerTargetCount > 1 && (ai.GetHealth01("", "") >= 0.3 || GetDistanceSq(ai, true) > 25))
				return 100.0;  //! Flee

			if (ai.m_eAI_AcuteDangerTargetCount <= 1 && ai.eAI_IsLowVitals() && GetDistanceSq(ai, true) > 25)
				return 100.0;  //! Flee
		}

		return m_MinDistance;
	}

	override bool ShouldAvoid(eAIBase ai = null, float distance = 0.0)
	{
		if (ai && !ai.IsRaised() && GetMinDistance(ai, distance) > 0.0)
			return true;

		return false;
	}
};
