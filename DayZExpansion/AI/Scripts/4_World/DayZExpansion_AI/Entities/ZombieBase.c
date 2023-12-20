modded class ZombieBase
{
	private autoptr eAIZombieTargetInformation m_TargetInformation;
	bool m_eAI_ProcessDamageByAI;
	bool m_Expansion_Airborne;
	float m_Expansion_AirbornePeakAltitude;

	void ZombieBase()
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "ZombieBase");
#endif

		Class.CastTo(m_TargetInformation, CreateTargetInformation());
	}

	protected eAIZombieTargetInformation CreateTargetInformation()
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
		DayZPlayerImplement sourcePlayer;
		if (!m_eAI_ProcessDamageByAI && Class.CastTo(sourcePlayer, source.GetHierarchyRootPlayer()))
		{
			switch (damageType)
			{
				case DT_CLOSE_COMBAT:
					eAIGroup group = sourcePlayer.GetGroup();
					if (group)
					{
						eAIFaction faction = group.GetFaction();
						eAIMeleeCombat.eAI_ApplyYeetForce(m_TargetInformation, faction.GetMeleeYeetForce(), sourcePlayer.GetPosition(), faction.GetMeleeYeetFactors());

						float meleeDamageMultiplier = faction.GetMeleeDamageMultiplier();
						if (meleeDamageMultiplier != 1.0)
						{
							m_eAI_ProcessDamageByAI = true;
							//! @note IMPORTANT: Do NOT pass in dmgZone here, won't generate hit! Melee dmg is a special snowflake
							ProcessDirectDamage(DT_CLOSE_COMBAT, source, "", ammo, modelPos, speedCoef * meleeDamageMultiplier);
							return false;
						}
					}

					break;
			}
		}
		else
		{
			m_eAI_ProcessDamageByAI = false;
		}

		return super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}

	override void EEKilled(Object killer)
	{
		m_TargetInformation.OnDeath();

		super.EEKilled(killer);
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
	#ifdef DIAG
		EXTrace.PrintHit(EXTrace.AI, this, "EEHitBy", damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	#endif

		m_TargetInformation.OnHit();

		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}

	override bool ModCommandHandlerBefore(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		if (m_Expansion_Airborne)
		{
			float altitude = GetPosition()[1];
			if (altitude > m_Expansion_AirbornePeakAltitude)
				m_Expansion_AirbornePeakAltitude = altitude;
			return false;
		}

		if (pCurrentCommandID == DayZInfectedConstants.COMMANDID_CRAWL)
			m_TargetInformation.m_Crawling = true;

		return super.ModCommandHandlerBefore(pDt, pCurrentCommandID, pCurrentCommandFinished);
	}

	override protected void EOnContact(IEntity other, Contact extra)
	{
		if (!IsAlive() || !m_Expansion_Airborne || m_Expansion_AirbornePeakAltitude == 0)
			return;

		float falldmg = (m_Expansion_AirbornePeakAltitude - GetPosition()[1]) * 0.333333;

		EXTrace.Print(EXTrace.AI, this, "::EOnContact falldmg " + falldmg);

		m_Expansion_Airborne = false;
		m_Expansion_AirbornePeakAltitude = 0.0;

		ProcessDirectDamage(DT_CUSTOM, this, "Torso", "FallDamageHealth", "0 0 0", falldmg);
	}

	void Expansion_SetAirborne(bool state)
	{
		m_Expansion_Airborne = state;
	}
};
