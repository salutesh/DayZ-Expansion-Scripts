class eAICreatureTargetInformation extends eAIEntityTargetInformation
{
	private DayZCreatureAI m_Creature;

	void eAICreatureTargetInformation(EntityAI target)
	{
		Class.CastTo(m_Creature, target);
	}

	override vector GetAimOffset(eAIBase ai = null)
	{
		//! TODO: Use collision box

		if (m_Creature.IsInherited(Animal_UrsusArctos))
			return "0 1.3 0";

		return "0 0.4 0";
	}

	override float CalculateThreat(eAIBase ai = null)
	{
		if (m_Creature.GetHealth("", "") <= 0.0)
			return 0.0;

		if (!m_Creature.IsDanger())
			return 0.0;

		float levelFactor = 0.5;

		if (ai)
		{
#ifdef DIAG
			auto hitch = EXHitch(ai.ToString() + " eAICreatureTargetInformation::CalculateThreat ", 20000);
#endif

			// the further away the creature, the less likely it will be a threat
			float distance = GetDistance(ai, true) + 0.1;
			levelFactor = 10 / distance;
			if (levelFactor > 1.0)
				levelFactor = Math.Pow(levelFactor, 2.0);

			if (levelFactor > 0.4)
			{
				levelFactor *= 2.0;
				auto hands = ai.GetHumanInventory().GetEntityInHands();
				if (hands)
					eAIPlayerTargetInformation.AdjustThreatLevelBasedOnWeapon(hands, distance, levelFactor);
			}
		}

		return Math.Clamp(levelFactor, 0.0, 1000000.0);
	}

	override bool ShouldRemove(eAIBase ai = null)
	{
		return GetThreat(ai) <= 0.1;
	}
};
