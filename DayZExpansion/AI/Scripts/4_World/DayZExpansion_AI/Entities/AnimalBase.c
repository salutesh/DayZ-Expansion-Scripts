modded class AnimalBase
{
	private autoptr eAITargetInformation m_TargetInformation;

	void AnimalBase()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "AnimalBase");
#endif

		m_TargetInformation = CreateTargetInformation();
	}

	protected eAITargetInformation CreateTargetInformation()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "CreateTargetInformation");
#endif

		return new eAICreatureTargetInformation(this);
	}

	eAITargetInformation GetTargetInformation()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "GetTargetInformation");
#endif

		return m_TargetInformation;
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
};
