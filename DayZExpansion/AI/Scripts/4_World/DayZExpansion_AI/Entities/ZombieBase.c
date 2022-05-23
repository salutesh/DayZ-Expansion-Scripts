modded class ZombieBase
{
	private autoptr eAIZombieTargetInformation m_TargetInformation;

	void ZombieBase()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "ZombieBase");
#endif

		Class.CastTo(m_TargetInformation, CreateTargetInformation());
	}

	protected eAITargetInformation CreateTargetInformation()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "CreateTargetInformation");
#endif

		return new eAIZombieTargetInformation(this);
	}

	eAIZombieTargetInformation GetTargetInformation()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetTargetInformation");
#endif

		return m_TargetInformation;
	}

	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		//! Ignore hit from bullet we create in WeaponBase::EEFired for bullet impact effect to prevent NULL pointers
		//! due to source not being set when trying to determine damage animation
		if (damageType == DT_FIRE_ARM && !source)
			return false;

		return super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}

	override void EEKilled(Object killer)
	{
		m_TargetInformation.OnDeath();

		super.EEKilled(killer);
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		m_TargetInformation.OnHit();

		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}

	override bool ModCommandHandlerBefore(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		if (pCurrentCommandID == DayZInfectedConstants.COMMANDID_CRAWL)
			m_TargetInformation.m_Crawling = true;

		return super.ModCommandHandlerBefore(pDt, pCurrentCommandID, pCurrentCommandFinished);
	}
};
