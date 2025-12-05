class eAIState_DropItem: eAIState
{
	ItemBase m_Item;
	float m_Time;

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (unit.IsUnconscious()) return EXIT;
		
		if (unit.GetActionManager().GetRunningAction())
		{
			m_Time += DeltaTime;
			if (m_Time > 10)  //! Looks like something went terribly wrong
			{
				unit.eAI_Unbug(m_Name + "(" + m_Item + ") - timeout");
				m_Time = 0;
				return EXIT;
			}
			
			return CONTINUE;
		}
		else if (m_Item && m_Item.GetHierarchyRoot() == unit)
		{
			m_Time = 0;
			unit.eAI_ThreatOverride(m_Item, true);
			unit.eAI_DropItem(m_Item, true);
			return CONTINUE;
		}
		
		return EXIT;
	}

	int Guard()
	{
		if (unit.IsFighting()) return eAITransition.FAIL;
		
		if (unit.IsRestrained()) return eAITransition.FAIL;
		if (unit.IsUnconscious()) return eAITransition.FAIL;
		if (unit.IsSwimming()) return eAITransition.FAIL;
		
		if (!unit.GetCommand_Move()) return eAITransition.FAIL;

		if (unit.IsRaised()) return eAITransition.FAIL;
		if (unit.GetActionManager().GetRunningAction()) return eAITransition.FAIL;
		if (unit.GetEmoteManager().IsEmotePlaying()) return eAITransition.FAIL;
		
		if (unit.eAI_IsChangingStance())
			return eAITransition.FAIL;
		
		foreach (int i, ItemBase item: unit.m_eAI_ItemsToDrop)
		{
			if (!item || item.GetHierarchyRoot() != unit)
			{
				unit.m_eAI_ItemsToDrop.Remove(i);
				break;
			}

			m_Item = item;
			return eAITransition.SUCCESS;
		}

		return eAITransition.FAIL;
	}
}
