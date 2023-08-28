modded class AnimalBase
{
	EntityAI m_Expansion_LastDamageSource;

	//! Workaround for vanilla passing killed entity as killer to EEKilled since 1.20
	//! Just check if killer is killed entity in EEKilled and then use m_Expansion_LastDamageSource instead of killer
	//! Vanilla events are fired in this order for each hit:
	//! EEOnDamageCalculated
	//! EEHitBy (IsAlive() still true)
	//!   EEKilled (within EEHitBy)
	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		bool applyDamage = super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

#ifdef DIAG
		EXTrace.PrintHit(EXTrace.MISC, this, "EEOnDamageCalculated - apply damage? " + applyDamage, damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
#endif

		if (applyDamage)
			m_Expansion_LastDamageSource = source;

		return applyDamage;
	}

#ifdef DIAG
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		EXTrace.PrintHit(EXTrace.MISC, this, "EEHitBy", damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}
#endif
}
