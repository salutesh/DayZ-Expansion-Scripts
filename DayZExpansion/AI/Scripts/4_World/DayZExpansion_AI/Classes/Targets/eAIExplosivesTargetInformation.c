class eAIExplosivesTargetInformation: eAIItemTargetInformation
{
	ExplosivesBase m_Explosive;
	float m_ExplosionRange;

	void eAIExplosivesTargetInformation(EntityAI target)
	{
		Class.CastTo(m_Explosive, target);
	}

	override void DeferredInit()
	{
		super.DeferredInit();

		//! Ammo types are set in respective explosive constructor,
		//! but target info is initialized in ExplosivesBase constructor,
		//! so we need a delay to get correct ammo types
		TStringArray ammoTypes = m_Explosive.Expansion_GetAmmoTypes();

		if (!ammoTypes)
		{
			EXError.Warn(this, "WARNING: " + m_Explosive + " (" + m_Explosive.GetType() + ") has no ammo types!", {});
			m_ExplosionRange = 30.0;
			return;
		}

		float explosionRange;
		float explosionDropoffRange;
		bool directional;
		float indirectHitAngle1;
		float indirectHitAngle2;

		foreach (string ammoType: ammoTypes)
		{
			if (!ammoType)
				continue;

			ExpansionDamageSystem.GetExplosionConfigValues(m_Item, ammoType, explosionRange, explosionDropoffRange, directional, indirectHitAngle1, indirectHitAngle2);

			if (explosionRange > m_ExplosionRange)
				m_ExplosionRange = explosionRange;
		}

		EXTrace.Print(EXTrace.AI, this, m_Explosive.ToString() + " (" + m_Explosive.GetType() + ") explosion range " + m_ExplosionRange);
	}

	override bool IsActive()
	{
		if (!super.IsActive())
			return false;

		if (!m_ExplosionRange)
			return false;

		if (m_Explosive.Expansion_IsLive())
			return true;

		return false;
	}

	override float CalculateThreat(eAIBase ai = null)
	{
		if (m_Item.IsDamageDestroyed() || m_Item.IsSetForDeletion())
			return 0.0;

		if (ai)
		{
			float distSq = GetDistanceSq(ai, true);

			float minDist = GetMinDistance(ai);

			if ((!m_Item.IsInherited(Grenade_Base) || !dBodyIsActive(m_Item)) && distSq > minDist * minDist)
				return 0.4;

			if (distSq < 1.0)
				distSq = 1.0;

			return 1000000.0 / distSq;
		}

		return 0.0;
	}

	override bool ShouldRemove(eAIBase ai = null)
	{
		return false;  //! @note will still be removed if no longer active (i.e. not armed/destroyed)
	}

	override float GetMinDistance(eAIBase ai = null)
	{
		return m_ExplosionRange;
	}
};
