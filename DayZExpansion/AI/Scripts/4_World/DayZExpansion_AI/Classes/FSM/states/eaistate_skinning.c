class eAIState_Skinning: eAIState
{
	int m_LastAttemptTime;
	ItemBase m_Item;
	EntityAI m_TargetEntity;
	float m_Timeout = UATimeSpent.SKIN + 8;
	float m_Time;

	override void OnEntry(string Event, ExpansionState From)
	{
		unit.OverrideStance(DayZPlayerConstants.STANCEIDX_CROUCH);
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (unit.IsUnconscious()) return EXIT;
		
		if (unit.eAI_IsChangingStance())
			return CONTINUE;
		
		if (unit.GetActionManager().GetRunningAction())
		{
			m_Time += DeltaTime;
			if (m_Time > m_Timeout)  //! Looks like something went terribly wrong
			{
				unit.eAI_Unbug(m_Name + "(" + m_Item + ", " + m_TargetEntity + ") - timeout");
				m_Time = 0;
				return EXIT;
			}
			
			return CONTINUE;
		}
		else if (m_TargetEntity && CanReach(m_TargetEntity))
		{
			m_LastAttemptTime = g_Game.GetTime();
			m_Time = 0;
			
			unit.StartActionObject(ActionSkinning, m_TargetEntity, m_Item);
			
			return CONTINUE;
		}
		
		//if (unit.Expansion_GetUp(true))
			//return CONTINUE;
		
		return EXIT;
	}

	int Guard()
	{
		if (unit.IsFighting()) return eAITransition.FAIL;
		
		if (unit.IsRestrained()) return eAITransition.FAIL;
		if (unit.IsUnconscious()) return eAITransition.FAIL;
		if (unit.IsSwimming()) return eAITransition.FAIL;
		
		if (!unit.GetCommand_Move()) return eAITransition.FAIL;

		if (unit.m_eAI_DangerousAreaCount > 0 && unit.m_eAI_IsInDangerByArea)
			return eAITransition.FAIL;

		if (unit.GetThreatToSelf() > 0.2) return eAITransition.FAIL;
		
		if (g_Game.GetTime() - m_LastAttemptTime < 4000) return eAITransition.FAIL;

		eAITarget target = unit.GetTarget();

		if (!target) return eAITransition.FAIL;
		
		m_TargetEntity = target.GetEntity();
		if (!m_TargetEntity) return eAITransition.FAIL;
		if (!m_TargetEntity.IsCorpse() && (!m_TargetEntity.IsAnimal() || !m_TargetEntity.IsDamageDestroyed())) return eAITransition.FAIL;

		if (!unit.eAI_ShouldProcureFood()) return eAITransition.FAIL;

		EntityAI entityInHands = unit.GetHumanInventory().GetEntityInHands();

		if (!entityInHands || entityInHands.IsWeapon()) return eAITransition.FAIL;

		if (!Class.CastTo(m_Item, entityInHands) || !m_Item.Expansion_IsMeleeWeapon()) return eAITransition.FAIL;

		if (!CanReach(m_TargetEntity)) return eAITransition.FAIL;
		
		return eAITransition.SUCCESS;
	}

	bool CanReach(EntityAI item)
	{
		EntityAI root = item.GetHierarchyRoot();

		if (root == unit)
			return true;

		if (root == item && vector.DistanceSq(unit.GetPosition(), item.GetPosition()) < 4.0)
			return true;

		return false;
	}

	int GuardIdle()
	{
		return eAITransition.SUCCESS;
	}
}
