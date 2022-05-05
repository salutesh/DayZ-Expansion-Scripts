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
		if (ai == this || m_Player.GetHealth("", "") <= 0.0)
			return 0.0;

		float levelFactor = 0.5;

		if (ai)
		{
			if (!ai.PlayerIsEnemy(m_Player))
				return 0.0;

			auto player = PlayerBase.Cast(m_Player);
			if (player && player.IsUnconscious())
				return 0.0;

			// the further away the player, the less likely they will be a threat
			float distance = GetDistance(ai);
			if (distance)
				levelFactor = 10 / distance;

			//! Enemy weapon
			auto enemyHands = m_Player.GetHumanInventory().GetEntityInHands();
			if (enemyHands)
			{
				AdjustThreatLevelBasedOnWeapon(enemyHands, distance, levelFactor);

				if (!m_Player.IsRaised())
					levelFactor *= 0.5;

				//! AI weapon
				auto hands = ai.GetHumanInventory().GetEntityInHands();
				if (hands)
				{
					AdjustThreatLevelBasedOnWeapon(hands, distance, levelFactor);
				}
			}
		}

		return Math.Clamp(levelFactor, 0.0, 1.0 / DISTANCE_COEF);
	}

	void AdjustThreatLevelBasedOnWeapon(EntityAI weapon, float distance, inout float levelFactor)
	{
		if (weapon.IsInherited(BoltActionRifle_Base) || weapon.IsInherited(BoltRifle_Base))
		{
			levelFactor *= 4.472136;  //! If both AI and target have a bolt rifle, threat level 0.4 at 500 m
		}
		else if (weapon.IsInherited(Rifle_Base))
		{
			levelFactor *= 3.0;  //! If both AI and target have a rifle, threat level 0.4 at 225 m
		}
		else if (weapon.IsInherited(Pistol_Base))
		{
			levelFactor *= 2.0;  //! If both AI and target have a pistol, threat level 0.4 at 100 m
		}
		else if (weapon.IsInherited(Weapon_Base))
		{
			levelFactor *= 2.0;
		}
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
