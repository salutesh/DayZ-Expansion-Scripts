class eAIPlayerTargetInformation extends eAIEntityTargetInformation
{
	private const float DISTANCE_COEF = 0.00001;

	private PlayerBase m_Player;

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

			if (m_Player && m_Player.IsUnconscious())
				return 0.0;

			// the further away the player, the less likely they will be a threat
			float distance = GetDistance(ai) + 0.0001;

			//! Enemy weapon
			auto enemyHands = ItemBase.Cast(m_Player.GetHumanInventory().GetEntityInHands());

			//! Guards are friendly to everyone until the other player raises their weapon
			if (ai.GetGroup() && ai.GetGroup().GetFaction().IsGuard() && !m_Player.IsRaised())
			{
				if (!enemyHands)
					levelFactor = 0.15;  //! They eyeball you menacingly
				else if (enemyHands.IsWeapon() || enemyHands.Expansion_IsMeleeWeapon())
					levelFactor = 0.2;  //! They aim at you
				else
					return 0.15;

				if (distance > 30)
					levelFactor *= (30 / distance);

				return levelFactor;
			}

			levelFactor = 10 / distance;

			if (distance > 30)
			{
				//! Check if target is facing AI when not in near range (30 m)
				vector targetDirection = vector.Direction(m_Player.GetPosition(), ai.GetPosition()).Normalized();
				float dot = vector.Dot(m_Player.GetDirection(), targetDirection);
				if (dot < 0.75)  //! Target is facing away
					return Math.Clamp(levelFactor, 0.0, 1.0 / DISTANCE_COEF);
			}

			if (enemyHands)
			{
				AdjustThreatLevelBasedOnWeapon(enemyHands, distance, levelFactor);

				if (enemyHands.IsWeapon() && !m_Player.IsRaised())
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
		Weapon gun;
		if (!Class.CastTo(gun, weapon))
			return;

		int mi = gun.GetCurrentMuzzle();

		//! Check if gun has available bullets either in chamber or internal/attached mag
		/* XXX: Not sure if going to use this, seems a bit redundant. Disabled for now
		if (gun.IsChamberEmpty(mi) || gun.IsChamberFiredOut(mi))
		{
			if (gun.HasInternalMagazine(mi))
			{
				if (!gun.GetInternalMagazineCartridgeCount(mi))
					return;
			}
			else
			{
				Magazine mag;
				if (Class.CastTo(mag, gun.GetMagazine()))
				{
					if (!mag.GetAmmoCount())
						return;
				}
				else
				{
					return;
				}
			}
		}
		*/

		if (weapon.IsInherited(BoltActionRifle_Base) || weapon.IsInherited(BoltRifle_Base))
		{
			levelFactor *= 4.472136;  //! If both AI and target have a 7.62x39 mm bolt rifle, threat level 0.4 at 500 m
		}
		else if (weapon.IsInherited(Rifle_Base))
		{
			levelFactor *= 3.0;  //! If both AI and target have a 7.62x39 mm rifle, threat level 0.4 at 225 m
		}
		else if (weapon.IsKindOf("Pistol_Base"))
		{
			levelFactor *= 2.0;  //! If both AI and target have a 19x9 mm pistol, threat level 0.4 at 36 m
		}
		else if (weapon.IsInherited(Weapon_Base))
		{
			levelFactor *= 2.0;
		}

		//! Now the fun part, scale threat level by health damage applied by ammo.
		//! Use 7.62x39 mm round w/ 0.9 init speed mult as baseline (1.0).
		string ammoTypeName = gun.GetChamberAmmoTypeName(mi);
		if (ammoTypeName)
		{
			float damage = ExpansionWeaponUtils.GetDamageAppliedByAmmo(ammoTypeName);

			if (damage)
			{
				float initSpeedMult = ExpansionWeaponUtils.GetWeaponInitSpeedMultiplier(gun.GetType());
				levelFactor *= damage * initSpeedMult / 99.0;
			}
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

		if (m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEMASK_RAISEDPRONE))
		{
			return "0 0.1 0";
		}

		return "0 0 0";
	}
};
