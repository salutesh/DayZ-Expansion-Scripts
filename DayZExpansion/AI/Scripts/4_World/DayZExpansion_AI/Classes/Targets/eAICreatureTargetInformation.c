class eAICreatureTargetInformation: eAIEntityTargetInformation
{
	private AnimalBase m_Creature;
	bool m_IsActive = true;

	void eAICreatureTargetInformation(EntityAI target)
	{
		Class.CastTo(m_Creature, target);
	}

	override bool IsCreature()
	{
		return true;
	}

	override bool IsFighting()
	{
		if (m_Creature.m_eAI_AttackCooldown > 0)
			return true;

		return false;
	}

	override bool IsAcuteDanger(eAIBase ai = null)
	{
		return m_Creature.Expansion_IsDanger();
	}

	override float GetAttackCooldown()
	{
		return m_Creature.m_eAI_AttackCooldown;
	}

	override vector GetAimOffset(eAIBase ai = null)
	{
		vector pos;

		int index = m_Creature.GetBoneIndexByName("Chest");
		if (index > -1)
			pos = m_Creature.GetBonePositionWS(index);
		else
			pos = m_Creature.GetCenter();

		pos = pos - m_Creature.GetPosition();

	#ifdef DIAG_DEVELOPER
		if (EXTrace.AI && ai)
			ai.Expansion_DebugObject(1234567890, m_Creature.GetPosition() + pos, "ExpansionDebugSphereSmall_Orange", m_Creature.GetDirection(), ai.GetPosition() + "0 1.5 0", 3.0, ShapeFlags.NOZBUFFER);
	#endif

		return pos;
	}

	override bool IsActive()
	{
		if (!m_Creature)
			return false;

		return m_IsActive;
	}

	override float CalculateThreat(eAIBase ai = null, eAITargetInformationState state = null)
	{
		if (m_Creature.IsDamageDestroyed())
		{
			if (ai && ai.eAI_ShouldProcureFood() && ai.m_eAI_MeleeWeapons.Count() > 0)
			{
				float dist = GetDistance(ai, true);
				if (dist > 4.0)
					return ExpansionMath.PowerConversion(0.0, 227.968982, dist, 1.0, 0.0, 3.0);  //! Go skinning
			}

			m_IsActive = false;

			return 0.0;
		}

		if (!m_Creature.Expansion_IsDanger())
		{
			if (ai)
			{
				if (ai.eAI_ShouldProcureFood() && ai.m_eAI_Firearms.Count() > 0 && ai.m_eAI_MeleeWeapons.Count() > 0)
					return ExpansionMath.PowerConversion(0.0, 227.968982, GetDistance(ai, true), 1.0, 0.0, 3.0);  //! Go hunting

				if (GetVelocity(m_Creature).LengthSq() > 0.277777)  //! 1 km/h
					return ExpansionMath.LinearConversion(0.0, 90.0, GetDistance(ai, true), 0.15, 0.0);  //! 0.1 at 30 m
			}

			return 0.0;
		}

		float levelFactor = 0.5;

		if (ai)
		{
#ifdef EXTRACE_DIAG
			auto hitch = new EXHitch(ai.ToString() + " eAICreatureTargetInformation::CalculateThreat ", 20000);
#endif
			if (ai.eAI_GetThreatOverride(m_Target))
				return 0.0;

			// the further away the creature, the less likely it will be a threat
			float distance = GetDistance(ai, true);

			//! If not reachable, ignore if we don't have a gun
			if (!ai.m_eAI_HasProjectileWeaponInHands && ai.eAI_IsUnreachable(2.0, m_Target.GetPosition()))
			{
				ai.eAI_ThreatOverride(m_Target, true);
				return 0.0;
			}

			levelFactor = 10 / (distance + 0.1);
			if (levelFactor > 1.0)
				levelFactor = Math.Pow(levelFactor, 2.0);

			if (levelFactor >= 0.4)
			{
				levelFactor *= 2.0;
				auto hands = ai.GetHumanInventory().GetEntityInHands();
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
		if (ai && m_Creature.IsAlive())
		{
			if (ai.m_eAI_AcuteDangerTargetCount > ai.GetGroup().Count() + 1)
				return 100.0;  //! Flee

			if (ai.eAI_IsLowVitals())
				return 100.0;  //! Flee

			if (!ai.m_eAI_HasProjectileWeaponInHands && m_Creature.m_Expansion_IsBigGame)
			{
				if (ai.GetGroup().GetFaction().GetMeleeDamageMultiplier() < 100)
					return 100.0;  //! Flee
			}
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
