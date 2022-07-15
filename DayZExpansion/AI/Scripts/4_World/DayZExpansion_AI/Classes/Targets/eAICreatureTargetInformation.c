class eAICreatureTargetInformation extends eAIEntityTargetInformation
{
	private const float DISTANCE_COEF = 0.001;

	private DayZCreatureAI m_Creature;

	void eAICreatureTargetInformation(EntityAI target)
	{
		Class.CastTo(m_Creature, target);
	}

	override vector GetAimOffset(eAIBase ai = null)
	{
		if (m_Creature.IsInherited(Animal_UrsusArctos))
			return "0 1.3 0";

		return "0 0.4 0";
	}

	override float GetThreat(eAIBase ai = null)
	{
		if (m_Creature.GetHealth("", "") <= 0.0)
			return 0.0;

		if (!m_Creature.IsDanger())
			return 0.0;

		float levelFactor = 0.5;

		if (ai)
		{
			// the further away the creature, the less likely it will be a threat
			float distance = GetDistance(ai) + DISTANCE_COEF;
			levelFactor = 10 / distance;

			if (levelFactor > 0.4)
			{
				auto hands = ai.GetHumanInventory().GetEntityInHands();
				if (hands)
					eAIPlayerTargetInformation.AdjustThreatLevelBasedOnWeapon(hands, distance, levelFactor);
			}
		}

		return Math.Clamp(levelFactor, 0.0, 10.0 / DISTANCE_COEF);
	}

	override bool ShouldRemove(eAIBase ai = null)
	{
		return GetThreat(ai) <= 0.1;
	}
};
