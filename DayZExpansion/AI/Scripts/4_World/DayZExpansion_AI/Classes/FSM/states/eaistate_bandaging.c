class eAIState_Bandaging_Self: eAIState
{
	int m_LastBandageAttemptTime;
	ItemBase m_Bandage;
	float m_Timeout;
	float m_Time;
	int m_BleedingSourceCount;

	override void OnEntry(string Event, ExpansionState From)
	{
		m_LastBandageAttemptTime = g_Game.GetTime();
		m_BleedingSourceCount = unit.GetBleedingSourceCount();
		m_Time = 0;
		float effectivity = m_Bandage.GetBandagingEffectivity();
		if (effectivity > 0)
			m_Timeout = UATimeSpent.BANDAGE / effectivity + 8.0;
		else
			m_Timeout = 16.0;
		unit.OverrideStance(DayZPlayerConstants.STANCEIDX_CROUCH);
	}

	override void OnExit(string Event, bool Aborted, ExpansionState To)
	{
		//! If we had dropped another item to take bandage to hands, restore item threat
		if (unit.m_eAI_LastDroppedItem && !unit.m_eAI_LastDroppedItem.Expansion_CanBeUsedToBandage())
			unit.eAI_ThreatOverride(unit.m_eAI_LastDroppedItem, false);
		
		if (m_Bandage)
		{
			if (m_Bandage.IsDamageDestroyed() || !unit.eAI_TakeItemToInventory(m_Bandage))
			{
				unit.eAI_DropItem(m_Bandage);
			}
		}
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (unit.IsUnconscious()) return EXIT;
		
		if (!unit.GetCommand_Move())
			return CONTINUE;
		
		if (unit.eAI_IsChangingStance())
			return CONTINUE;
		
		if (unit.GetActionManager().GetRunningAction())
		{
			if (unit.GetBleedingSourceCount() != m_BleedingSourceCount)
			{
				m_BleedingSourceCount = unit.GetBleedingSourceCount();
				m_Time = 0;
			}

			m_Time += DeltaTime;
			if (m_Time > m_Timeout)  //! Looks like something went terribly wrong
			{
				unit.eAI_Unbug(m_Name + "(" + m_Bandage + ") - timeout");
				m_Time = 0;
				return EXIT;
			}
			
			return CONTINUE;
		}
		else if (unit.eAI_ShouldBandage())
		{
			if (m_Bandage)
			{
				if (unit.IsBleeding() && !m_Bandage.IsDamageDestroyed())
				{
					m_LastBandageAttemptTime = g_Game.GetTime();
					m_Time = 0;
					
					unit.StartActionObject(ActionBandageSelf, null);
					
					return CONTINUE;
				}
			}
		}
		
		if (unit.Expansion_GetUp(true))
			return CONTINUE;
		
		return EXIT;
	}

	int Guard()
	{
		if (unit.IsFighting()) return eAITransition.FAIL;
		
		if (unit.IsRestrained()) return eAITransition.FAIL;
		if (unit.IsUnconscious()) return eAITransition.FAIL;
		if (unit.IsSwimming()) return eAITransition.FAIL;
		
		if (!unit.eAI_ShouldBandage()) return eAITransition.FAIL;
		
		if (g_Game.GetTime() - m_LastBandageAttemptTime < 4000) return eAITransition.FAIL;
		
		auto hands = unit.GetItemInHands();
		if (!hands) return eAITransition.FAIL;
		
		if (!hands.Expansion_CanBeUsedToBandage() || hands.IsDamageDestroyed()) return eAITransition.FAIL;
		
		m_Bandage = hands;
		
		return eAITransition.SUCCESS;
	}

	int GuardIdle()
	{
		if (unit.GetActionManager().GetRunningAction()) return eAITransition.FAIL;
		
		if (g_Game.GetTime() - m_LastBandageAttemptTime < 4000) return eAITransition.FAIL;
		
		return eAITransition.SUCCESS;
	}
}
