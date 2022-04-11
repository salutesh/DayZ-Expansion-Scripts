class eAIPlayerTargetInformation extends eAIEntityTargetInformation
{
	private const float DISTANCE_COEF = 0.00001;

	private DayZPlayerImplement m_Player;

	void eAIPlayerTargetInformation(EntityAI target)
	{
		Class.CastTo(m_Player, target);
	}

	override float GetThreat(eAIBase ai = null)
	{
		float levelFactor = 0.5;

		if (ai)
		{
			float distance = GetDistance(ai) * DISTANCE_COEF;
			if (distance > 1.0)
				levelFactor = levelFactor / distance;

			vector direction = vector.Direction(ai.GetPosition(), GetPosition(ai));

			float dot = vector.Dot(ai.GetDirection(), direction);
			dot += 1.0;
			dot *= 0.5;
			dot -= 0.33;
			dot *= 2.0;
			dot *= dot * (dot + 0.9) * dot;
			dot += 0.07;
			dot = Math.Clamp(dot, 0, 1);

			levelFactor = levelFactor * dot;
		}

		return Math.Clamp(levelFactor, 0.0, 1.0 / DISTANCE_COEF);
	}

	override bool ShouldRemove(eAIBase ai = null)
	{
		return GetThreat(ai) <= 0.1;
	}

	override vector GetAimOffset(eAIBase ai = null)
	{
		if (m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_RAISEDERECT))
		{
			return "0 1.5 0";
		}

		if (m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_RAISEDCROUCH))
		{
			return "0 0.8 0";
		}

		// if (m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEMASK_RAISEDPRONE))
		//{
		//	return "0 0.1 0";
		// }

		return "0 0 0";
	}
};
