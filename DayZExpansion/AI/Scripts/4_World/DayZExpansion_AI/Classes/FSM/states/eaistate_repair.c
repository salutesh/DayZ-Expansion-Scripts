class eAIState_Repair: eAIState_Craft
{
	int m_EntityHealthLevel;

	override void OnEntry(string Event, ExpansionState From)
	{
		m_RecipeClassName = "CleanWeapon";

		super.OnEntry(Event, From);
	}

	override bool UpdateState()
	{
		if (m_TargetEntity.GetHealthLevel() != m_EntityHealthLevel)
		{
			m_EntityHealthLevel = m_TargetEntity.GetHealthLevel();
			return true;
		}

		return false;
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (super.OnUpdate(DeltaTime, SimulationPrecision) == CONTINUE) return CONTINUE;
		
		if (!unit.IsUnconscious() && unit.Expansion_GetUp(true))
			return CONTINUE;
		
		return EXIT;
	}

	override int Guard()
	{
		if (super.Guard() == eAITransition.FAIL) return eAITransition.FAIL;
		
		m_TargetEntity = unit.GetHumanInventory().GetEntityInHands();
		if (!m_TargetEntity) return eAITransition.FAIL;
		
		m_EntityHealthLevel = m_TargetEntity.GetHealthLevel();
		
		if (m_EntityHealthLevel == GameConstants.STATE_RUINED) return eAITransition.FAIL;
		
		if (m_TargetEntity.CanBeRepairedToPristine())
		{
			if (m_EntityHealthLevel == GameConstants.STATE_PRISTINE) return eAITransition.FAIL;
		}
		else
		{
			if (m_EntityHealthLevel <= GameConstants.STATE_WORN) return eAITransition.FAIL;
		}

		if (!m_Item || !CanReach(m_Item) || !m_Item.Expansion_CanRepair(m_TargetEntity))
		{
			m_Item = unit.eAI_GetRepairKit(m_TargetEntity);
			if (!m_Item) return eAITransition.FAIL;
		}
		
		return eAITransition.SUCCESS;
	}
}
