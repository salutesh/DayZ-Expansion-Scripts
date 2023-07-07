class eAIPlayerTargetInformation: eAIEntityTargetInformation
{
	private DayZPlayerImplement m_Player;

	void eAIPlayerTargetInformation(EntityAI target)
	{
		Class.CastTo(m_Player, target);
	}

	override float CalculateThreat(eAIBase ai = null)
	{
		if (m_Player.IsDamageDestroyed())
			return 0.0;

		float levelFactor = 0.1;

		if (ai)
		{
			if (ai == m_Player)
				return 0.0;

#ifdef DIAG
			auto hitch = new EXHitch(ai.ToString() + " eAIPlayerTargetInformation::CalculateThreat ", 20000);
#endif

			// the further away the player, the less likely they will be a threat
			float distance = GetDistance(ai, true) + 0.1;

			if (m_Player.IsUnconscious() || m_Player.IsRestrained())
				return ExpansionMath.LinearConversion(0, 100, distance, 0.15, 0.1);

			if (distance <= 100.0 && m_Player.GetParent() != ai.GetParent())
			{
				//! Any AI, even passive, will react if vehicle is speeding towards them
				//! Vehicles WITHOUT drivers are handled by vehicle target info
				levelFactor = ProcessVehicleThreat(ai, distance);
				if (levelFactor > 0.0)
					return levelFactor;
			}

			if (!ai.PlayerIsEnemy(m_Player))
				return 0.0;

			vector fromTargetDirection = vector.Direction(m_Player.GetPosition(), ai.GetPosition()).Normalized();
			float fromTargetDot = vector.Dot(m_Player.Expansion_GetAimDirection(), fromTargetDirection);

			//! Enemy weapon
			auto enemyHands = ItemBase.Cast(m_Player.GetHumanInventory().GetEntityInHands());

			//! Threat handling for guards and observers
			//! Guards won't aggro until the other player raises their weapon in their direction, starts melee fighting or shoots another player
			//! Observers will never aggro and just look at the player
			if (ai.GetGroup())
			{
				bool canEnterFightingState;

				if (ai.GetGroup().GetFaction().IsGuard())
				{
					if (m_Player.IsRaised() && fromTargetDot >= 0.9 && ((enemyHands && enemyHands.IsWeapon()) || m_Player.IsFighting()))
						canEnterFightingState = true;
					else if (m_Player.eAI_UpdateAgressionTimeout(150.0 - distance))
						canEnterFightingState = true;

					if (!canEnterFightingState && m_Player.IsRaised())
					{
						//! They aim at you
						return ExpansionMath.PowerConversion(0.5, 30, distance, 0.2, 0.0, 0.1);
					}
				}
				else if (!ai.GetGroup().GetFaction().IsObserver() && !m_Player.Expansion_IsInSafeZone())
				{
					canEnterFightingState = true;
				}

				if (!canEnterFightingState)
				{
					//! They eyeball you menacingly
					return ExpansionMath.PowerConversion(0.5, 30, distance, 0.152, 0.0, 0.1);
				}
			}

			levelFactor = 10 / distance;

			if (distance > 30)
			{
				//! Check if target is facing AI or AI is facing target when not in near range (30 m)
				vector toTargetDirection = vector.Direction(ai.GetPosition(), m_Player.GetPosition()).Normalized();
				float toTargetDot = vector.Dot(ai.GetAimDirection(), toTargetDirection);
				if (fromTargetDot < 0.75 && toTargetDot < 0.75)  //! Target is facing away and AI is facing away
					return Math.Clamp(levelFactor, 0.0, 1000000.0);
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
			if ((hands && AdjustThreatLevelBasedOnWeapon(hands, distance, levelFactor, hasLOS)) || distance <= 30 || hasLOS)
			{
				if (enemyHands)
				{
					//! Enemy weapon is raised or enemy is close
					if (m_Player.IsRaised() || distance <= 30 || hasLOS)
					{
						AdjustThreatLevelBasedOnWeapon(enemyHands, distance, levelFactor, hasLOS);

						if (m_Player.IsRaised())
							levelFactor *= ExpansionMath.LinearConversion(0, 1000, distance, 2.0, 1.0);
					}
				}
			}

			levelFactor *= ai.Expansion_GetVisibility(distance);
		}

		return Math.Clamp(levelFactor, 0.0, 1000000.0);
	}

	float ProcessVehicleThreat(eAIBase ai, float distance)
	{
		Transport transport;
		if (!Class.CastTo(transport, m_Player.GetParent()))
			return 0.0;

		if (transport.CrewMemberIndex(m_Player) != DayZPlayerConstants.VEHICLESEAT_DRIVER)
			return 0.0;

		float speed, fromTargetDot;
		float levelFactor = eAIVehicleTargetInformation.ProcessVehicleThreat(transport, ai, distance, speed, fromTargetDot);
		//PrintFormat("eAIPlayerTargetInformation dist %1 spd %2 dot %3 lvl %4", distance, speed, fromTargetDot, levelFactor);
		return levelFactor;
	}

	static bool AdjustThreatLevelBasedOnWeapon(EntityAI weapon, float distance, inout float levelFactor, bool hasLOS = true)
	{
		Weapon_Base gun;
		if (!Class.CastTo(gun, weapon))
			return false;

		int mi = gun.GetCurrentMuzzle();

		//! Check if gun has available bullets either in chamber or internal/attached mag
		/* XXX: Not sure if going to use this, seems a bit redundant. Disabled for now
		if (!gun.Expansion_HasAmmo())
			return;
		*/

		//! Multiplicator based on weapon and attachments
		ItemOptics optics;
		if (gun.IsInherited(BoltActionRifle_Base) || gun.IsInherited(BoltRifle_Base) || (Class.CastTo(optics, gun.GetAttachedOptics()) && optics.GetZeroingDistanceZoomMax() >= distance))
		{
			levelFactor *= 7.333333;  //! If either AI or target have a 7.62x54 mm bolt rifle, threat level 0.4 at 500 m
		}
		else if (gun.IsInherited(Rifle_Base))  //! Rifle_Base also includes shotguns
		{
			levelFactor *= 5.0;  //! If either AI or target have a 5.56x45 mm rifle, threat level 0.4 at 250 m
		}
		else if (gun.IsKindOf("Pistol_Base"))
		{
			levelFactor *= 5.0;  //! If either AI or target have a 19x9 mm pistol, threat level 0.4 at 50 m
		}
		else if (gun.IsInherited(Weapon_Base))  //! In theory this condition should never be reached
		{
			levelFactor *= 5.0;  //! If either AI or target have a 5.56x45 mm weapon, threat level 0.4 at 250 m
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
				//! In combination with lowest multiplicator above, makes sure overall level factor can not go lower than input value
				levelFactor *= Math.Clamp(damage, 22.0, 10000.0) / 55.0;
			}
		}

		return true;
	}

	override bool ShouldRemove(eAIBase ai = null)
	{
		return GetThreat(ai) <= 0.02;  //! Will remove if 500 m away and no LOS
	}

	override vector GetAimOffset(eAIBase ai = null)
	{
		string boneName;
		if (ai && Weapon_Base.Cast(ai.GetHumanInventory().GetEntityInHands()))
			boneName = "neck";
		else
			boneName = "spine3";  //! Aim lower for melee
		vector pos = m_Player.GetBonePositionWS(m_Player.GetBoneIndexByName(boneName));
		pos = pos - m_Player.GetPosition();
		return pos;
	}
};
