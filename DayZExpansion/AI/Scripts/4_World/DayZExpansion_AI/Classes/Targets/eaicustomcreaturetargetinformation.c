class eAICustomCreatureTargetInformation: eAIEntityTargetInformation
{
	static const float CREATURE_AGGRO_RANGE = 100.0;

	static ref CF_DoublyLinkedNodes_WeakRef<eAICustomCreatureTargetInformation> s_AllCustomCreatures = new CF_DoublyLinkedNodes_WeakRef<eAICustomCreatureTargetInformation>;

	ref CF_DoublyLinkedNode_WeakRef<eAICustomCreatureTargetInformation> m_Node;
	private DayZCreature m_Creature;

	void eAICustomCreatureTargetInformation(EntityAI target)
	{
		m_Node = s_AllCustomCreatures.Add(this);
		Class.CastTo(m_Creature, target);
	}

	void ~eAICustomCreatureTargetInformation()
	{
	#ifndef DIAG_DEVELOPER
		if (!g_Game)
			return;
	#endif

		if (s_AllCustomCreatures)
			s_AllCustomCreatures.Remove(m_Node);
	}

	override void RemoveFromAll()
	{
		super.RemoveFromAll();

		if (m_Target && s_CustomAI.Contains(m_Target))
		{
			s_CustomAI.Remove(m_Target);
		#ifdef DIAG_DEVELOPER
			EXPrint(this, "[CustomAI] removed " + m_Target);
		#endif
		}
	}

	override bool IsAcuteDanger(eAIBase ai = null)
	{
		if (ai)
			return ai.CanBeTargetedByAI(m_Creature);

		return true;
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
		else
		{
			boneName = "spine3";  //! Aim lower for melee
		}

		vector pos;
		int index = m_Creature.GetBoneIndexByName(boneName);
		if (index > -1)
			pos = m_Creature.GetBonePositionWS(index);
		else
			pos = m_Creature.GetCenter();

		pos = pos - m_Creature.GetPosition();

		return pos;
	}

	override float CalculateThreat(eAIBase ai = null, eAITargetInformationState state = null)
	{
		if (m_Creature.IsDamageDestroyed())
			return 0.0;

		float levelFactor = CREATURE_AGGRO_RANGE * 0.022;

		if (ai)
		{
			if (ai.eAI_GetThreatOverride(m_Target))
				return 0.0;

			if (!ai.CanBeTargetedByAI(m_Creature))
				return 0.0;

			//! The further away the creature, the less likely it will be a threat
			float distance = GetDistance(ai, true);

			if (ai.IsSwimming())
				return ExpansionMath.LinearConversion(0, 100, distance, 0.15, 0.1);

			levelFactor *= 10 / (distance + 0.1);  //! Threat level 0.2 at 110 m, 0.1 at 220 m (removal threshold)

			if (levelFactor > 0.152)
			{
				levelFactor *= 2.0;
				EntityAI hands = ai.GetHumanInventory().GetEntityInHands();
				bool hasLOS;
				if (state)
					hasLOS = state.m_SearchOnLOSLost;
				if (hands)
					eAIPlayerTargetInformation.AdjustThreatLevelBasedOnWeapon(hands, distance, levelFactor, hasLOS);
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
		//! @note at health below 30%, we are too slow to flee from creatures that are close and are better off fighting
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
