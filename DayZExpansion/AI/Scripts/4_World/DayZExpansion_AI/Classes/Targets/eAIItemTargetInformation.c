class eAIItemTargetInformation: eAIEntityTargetInformation
{
	protected ItemBase m_Item;

	void eAIItemTargetInformation(EntityAI target)
	{
		Class.CastTo(m_Item, target);
	}

	void UpdateItemInfo()
	{
	}

	override bool IsInanimate()
	{
		return true;
	}

	override bool IsExplosive()
	{
		return m_Item.Expansion_IsExplosive();
	}

	override bool IsMechanicalTrap()
	{
		return m_Item.Expansion_IsMechanicalTrap();
	}

	override bool IsActive()
	{
		if (!super.IsActive())
			return false;

		return m_Item.GetHierarchyRootPlayer() == null;
	}

	override vector GetPosition(eAIBase ai = null, bool actual = false)
	{
		return m_Item.GetCenter();
	}

	override float CalculateThreat(eAIBase ai = null)
	{
		if (m_Item.IsDamageDestroyed() || m_Item.IsSetForDeletion())
			return 0.0;

		if (m_Item.GetHierarchyRootPlayer())
			return 0.0;  //! Has been picked up since we last saw it

		if (ai)
		{
			if (ai.eAI_GetItemThreatOverride(m_Item) || ai.IsRestrained())
				return 0.1;

			float distance = GetDistanceSq(ai, true);

			if (distance > 900.0)
				return 0.0;

			eAITarget target = ai.GetTarget();
			//! @note The dBodyIsactive check is to avoid checking items while they are thrown physically
			if (target && target.info == this && !dBodyIsActive(m_Item))
			{
				if (ai.eAI_IsUnreachable(distance, 4.0, m_Item.GetCenter()) || (distance <= 4.0 && ai.eAI_IsItemObstructed(m_Item)))
				{
					//! Item is above or below where AI can reach or item is obstructed
					ai.eAI_ItemThreatOverride(m_Item, true);
					return 0.1;
				}
			}

			bool canBandage;
			//! https://feedback.bistudio.com/T173348
			if (ai.IsBleeding() && m_Item.Expansion_CanBeUsedToBandage())
				canBandage = true;

			Magazine targetMag;

			if (canBandage && ai.GetBandageToUse())
			{
				//! Don't need to pick up bandage if AI already has one in inventory
				return 0.0;
			}
			else if (m_Item.IsWeapon() || m_Item.Expansion_IsMeleeWeapon() || canBandage || (Class.CastTo(targetMag, m_Item) && targetMag.GetAmmoCount()))
			{
				ItemBase itemInHands = ai.GetItemInHands();
				Weapon_Base gunInHands;

				if (!canBandage && itemInHands && !itemInHands.IsDamageDestroyed())
				{
					if (Class.CastTo(gunInHands, itemInHands))
					{
						if (ai.IsRaised() || ai.GetWeaponManager().IsRunning() || ai.GetActionManager().GetRunningAction())
							return 0.0;

						if (gunInHands.Expansion_HasAmmo())
							return 0.0;
					}
					else if (itemInHands.Expansion_IsMeleeWeapon() && m_Item.Expansion_IsMeleeWeapon())
					{
						return 0.0;
					}
				}

				//! Ignore magazines and ammo if AI has no gun
				if (m_Item.IsMagazine() && !gunInHands && !ai.eAI_GetAnyWeaponToUse())
					return 0.0;

				Weapon_Base gun;
				if (Class.CastTo(gun, m_Item))
				{
					if (target && target.IsMechanicalTrap() && (!itemInHands || itemInHands.Expansion_IsMeleeWeapon()))
						return 0.0;

					Magazine mag;

					//! Ignore guns we don't have ammo for so we don't keep picking up guns that we can't use
					if (!ai.eAI_HasAmmoForFirearm(gun, mag, false))
						return 0.0;
				}

				if (ai.GetPathFinding().IsBlocked(ai.GetPosition(), m_Item.GetCenter()))
				{
					//! Something is blocking the direct path to item
					return 0.1;
				}

				if (distance < 1.0)
					distance = 1.0;

				float q;
				if (m_Item.IsWeapon())
				{
					q = 1000000;
				}
				else if (m_Item.IsAmmoPile())
				{
					q = 1000;
				}
				else if (m_Item.Expansion_IsMeleeWeapon() || m_Item.IsMagazine())
				{
					q = 10000;
				}
				else if (canBandage && ai.eAI_ShouldBandage())
				{
					q = 900000900;  //! URGENT - other targets are clamped to 1000000, maxdist squared = 900 (30m) so adding 1 to make AI go for the bandage
				}
				else
				{
					distance = Math.Sqrt(distance);
					q = 12;  //! Threat level 0.4 at 30 m
				}

				return q / distance;
			}
		}

		return 0.0;
	}

	override bool ShouldRemove(eAIBase ai = null)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_1(this, "ShouldRemove").Add(ai);
#endif

		return GetThreat(ai) <= 0.1;
	}
};
