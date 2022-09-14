class eAIZombieTargetInformation extends eAIEntityTargetInformation
{
	private ZombieBase m_Zombie;
	private DayZInfectedInputController m_DIIP;

	bool m_Crawling;

	void eAIZombieTargetInformation(EntityAI target)
	{
		Class.CastTo(m_Zombie, target);

		m_DIIP = m_Zombie.GetInputController();
	}

	override bool IsCrawling()
	{
		return m_Crawling;
	}

	override vector GetAimOffset(eAIBase ai = null)
	{
		if (m_Crawling)
			return "0 0 0";

		return "0 1.3 0";
	}

	// https://www.desmos.com/calculator/r4mqu91qff
	override float GetThreat(eAIBase ai = null)
	{
		float levelFactor = 0;

		if (m_Zombie.GetHealth("", "") <= 0.0)
			return 0.0;

		// TODO: check to see if ::GetMindState() returns int of 0->4
		int level = m_DIIP.GetMindState();
		switch (level)
		{
		case DayZInfectedConstants.MINDSTATE_CALM:
			levelFactor = 0.00;
			break;
		case DayZInfectedConstants.MINDSTATE_DISTURBED:
			levelFactor = 0.25;
			break;
		case DayZInfectedConstants.MINDSTATE_ALERTED:
			levelFactor = 0.50;
			break;
		case DayZInfectedConstants.MINDSTATE_CHASE:
			levelFactor = 1.0;
			break;
		case DayZInfectedConstants.MINDSTATE_FIGHT:
			levelFactor = 2.00;
			break;
		}

		if (ai)
		{
			// the further away the zombie, the less likely it will be a threat
			float distance = GetDistance(ai, true) + 0.1;
			levelFactor *= 10 / distance;
			if (levelFactor > 1.0)
				levelFactor = Math.Pow(levelFactor, 2.0);

			if (m_DIIP.GetTargetEntity() == ai)
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
