class eAIPlayerTargetInformation extends eAIEntityTargetInformation
{
	private const float DISTANCE_COEF = 0.0001;

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

			if (m_Player.IsUnconscious())
				return 0.0;

			// the further away the player, the less likely they will be a threat
			float distance = GetDistance(ai) + DISTANCE_COEF;

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
				//! Check if target is facing AI or AI is facing target when not in near range (30 m)
				vector fromTargetDirection = vector.Direction(m_Player.GetPosition(), ai.GetPosition()).Normalized();
				float fromTargetDot = vector.Dot(m_Player.GetDirection(), fromTargetDirection);
				vector toTargetDirection = vector.Direction(ai.GetPosition(), m_Player.GetPosition()).Normalized();
				float toTargetDot = vector.Dot(ai.GetDirection(), toTargetDirection);
				if (fromTargetDot < 0.75 && toTargetDot < 0.75)  //! Target is facing away and AI is facing away
					return Math.Clamp(levelFactor, 0.0, 10.0 / DISTANCE_COEF);
			}

			//! Enemy is within 30 m, or farher than 30 m but looking in our direction,
			//! or we are looking in their direction

			//! Only adjust threat level based on enemy weapon if AI has a weapon in hands to fight back to begin with
			//! Threat level from lowest to highest:
			//! AI bare fists: Distance, look direction and enemy weapon if close affect threat level
			//! AI weapon, enemy bare fists or unraised weapon: Distance, look direction and AI weapon affect threat level
			//! AI weapon, enemy raised weapon: Distance, look direction, AI weapon and enemy weapon affect threat level
			//! This should ensure that AI doesn't take fist fights over long distances or against armed enemies unless they are close,
			//! and prioritizes enemies with raised weapons
			auto hands = ai.GetHumanInventory().GetEntityInHands();
			bool hasLOS = ai.eAI_HasLOS();
			if ((hands && AdjustThreatLevelBasedOnWeapon(hands, distance, levelFactor, hasLOS)) || distance <= 30)
			{
				if (enemyHands)
				{
					//! Enemy weapon is raised or enemy is close
					if (m_Player.IsRaised() || distance <= 30)
					{
						AdjustThreatLevelBasedOnWeapon(enemyHands, distance, levelFactor, hasLOS);

						if (m_Player.IsRaised())
							levelFactor *= ExpansionMath.LinearConversion(0, 1000, distance, 2.0, 1.0);
					}
				}
			}
		}

		return Math.Clamp(levelFactor, 0.0, 10.0 / DISTANCE_COEF);
	}

	static bool AdjustThreatLevelBasedOnWeapon(EntityAI weapon, float distance, inout float levelFactor, bool hasLOS = true)
	{
		Weapon gun;
		if (!Class.CastTo(gun, weapon))
			return false;

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

		//! Multiplicator based on weapon and attachments
		ItemOptics optics;
		if (weapon.IsInherited(BoltActionRifle_Base) || weapon.IsInherited(BoltRifle_Base) || (Class.CastTo(optics, weapon.GetAttachmentByType(ItemOptics)) && optics.GetZeroingDistanceZoomMax() >= distance))
		{
			levelFactor *= 10.0;  //! If either AI or target have a 5.56x45 mm bolt rifle, threat level 0.4 at 500 m
		}
		else if (weapon.IsInherited(Rifle_Base))  //! Rifle_Base also includes shotguns
		{
			levelFactor *= 5.0;  //! If either AI or target have a 5.56x45 mm rifle, threat level 0.4 at 250 m
		}
		else if (weapon.IsKindOf("Pistol_Base"))
		{
			levelFactor *= 5.0;  //! If either AI or target have a 19x9 mm pistol, threat level 0.4 at 50 m
		}
		else if (weapon.IsInherited(Weapon_Base))  //! In theory this condition should never be reached
		{
			levelFactor *= 5.0;  //! If either AI or target have a 5.56x45 mm weapon, threat level 0.4 at 300 m
		}
		else
		{
			return false;
		}

		if (!hasLOS)
			levelFactor *= 0.5;

		//! Now the fun part, scale threat level by health damage applied by ammo.
		//! Use 110 damage w/ 1.0 init speed mult as baseline (1.0).
		string ammoTypeName = gun.GetChamberAmmoTypeName(mi);
		if (ammoTypeName)
		{
			float damage = ExpansionWeaponUtils.GetDamageAppliedByAmmo(ammoTypeName);

			if (damage)
			{
				float initSpeedMult = ExpansionWeaponUtils.GetWeaponInitSpeedMultiplier(gun.GetType());
				damage *= initSpeedMult;
				if (damage < 22.0)
					damage = 22;  //! In combination with lowest multiplicator above, makes sure overall level factor can not go lower than input value
				levelFactor *= damage / 55.0;
			}
		}

		return true;
	}

	override bool ShouldRemove(eAIBase ai = null)
	{
		return GetThreat(ai) <= 0.01;  //! Will remove if 1000 m away and no LOS
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
