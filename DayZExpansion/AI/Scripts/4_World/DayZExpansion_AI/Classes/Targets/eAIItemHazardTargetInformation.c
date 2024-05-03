class eAIItemHazardTargetInformation: eAIItemTargetInformation
{
	float m_Range;
	float m_MinDistance;

	override void DeferredInit()
	{
		super.DeferredInit();

		UpdateItemInfo();
	}

	override void UpdateItemInfo()
	{
		TStringArray ammoTypes = m_Item.Expansion_GetAmmoTypes();

		if (!ammoTypes)
		{
			EXError.Warn(this, "WARNING: " + m_Item + " (" + m_Item.GetType() + ") has no ammo types!", {});
			m_Range = 30.0;
		}
		else
		{
			float range;
			float mult;

			m_Range = 0.0;

			foreach (string ammoType: ammoTypes)
			{
				if (!ammoType)
					continue;

				//! @note ammo for mechanical traps (e.g. bear trap, tripwire) doesn't have range
				range = GetGame().ConfigGetFloat(CFG_AMMO + " " + ammoType + " indirectHitRange");
				mult = GetGame().ConfigGetFloat(CFG_AMMO + " " + ammoType + " indirectHitRangeMultiplier");

				if (mult)
					range *= mult;

				if (range > m_Range)
					m_Range = range;
			}
		}

		m_MinDistance = m_Range;

		if (m_Item.Expansion_IsExplosive())
			m_MinDistance += 5.0;  //! +5 m safety margin for explosives
		else
			m_MinDistance += 1.2;  //! +1.2 m safety margin for anything else

		EXTrace.Print(EXTrace.AI, this, m_Item.ToString() + " (" + m_Item.GetType() + ") range " + m_Range + " min distance " + m_MinDistance);
	}

	override bool IsActive()
	{
		if (!super.IsActive())
			return false;

		if (!m_MinDistance)
			return false;

		if (m_Item.Expansion_IsDanger())
			return true;

		return false;
	}

	override float CalculateThreat(eAIBase ai = null)
	{
		if (m_Item.IsDamageDestroyed() || m_Item.IsSetForDeletion())
			return 0.0;

		if (ai)
		{
			float dist = GetDistance(ai, true);

			if (dist < 1.0)
				dist = 1.0;

			float minDist = GetMinDistance(ai);

			if (!m_Item.Expansion_IsExplosive() || ((!m_Item.IsInherited(Grenade_Base) || !dBodyIsActive(m_Item)) && dist > minDist))
				return Math.Max(minDist, 2.0) / dist;

			float distSq = dist * dist;
			return 1000000.0 / distSq;
		}

		return 0.0;
	}

	override bool ShouldRemove(eAIBase ai = null)
	{
		//! Remove if distance is twice minimum distance, or farther than 30 m, whichever is larger
		if (GetDistanceSq(ai, true) > Math.Max(m_MinDistance * m_MinDistance * 2, 900))
			return true;

		return false;  //! @note will also be removed if no longer active (i.e. not armed/destroyed)
	}

	override float GetMinDistance(eAIBase ai = null)
	{
		return m_MinDistance;
	}

	override float GetMinDistanceSq(eAIBase ai = null)
	{
		return m_MinDistance * m_MinDistance;
	}
};
