class eAIState_Craft: eAIState
{
	int m_LastAttemptTime;
	string m_RecipeClassName;
	ItemBase m_Item;
	EntityAI m_TargetEntity;
	float m_Timeout;
	float m_Time;

	override void OnEntry(string Event, ExpansionState From)
	{
		int recipeID = unit.m_ModuleRecipesManager.RecipeIDFromClassname(m_RecipeClassName);
		RecipeBase recipe = unit.m_ModuleRecipesManager.m_RecipeList[recipeID];
		m_Timeout = recipe.GetLengthInSecs() + 8;
		unit.SetCraftingRecipeID(recipeID);
	}

	bool UpdateState()
	{
		return false;
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (unit.IsUnconscious()) return EXIT;
		
		if (unit.eAI_IsChangingStance())
			return CONTINUE;
		
		if (unit.GetActionManager().GetRunningAction())
		{
			if (UpdateState())
				m_Time = 0;

			m_Time += DeltaTime;
			if (m_Time > m_Timeout)  //! Looks like something went terribly wrong
			{
				unit.eAI_Unbug(m_Name + "(" + m_RecipeClassName + ", " + m_Item + ", " + m_TargetEntity + ") - timeout");
				m_Time = 0;
				return EXIT;
			}
			
			return CONTINUE;
		}
		else if (Guard() == eAITransition.SUCCESS)
		{
			m_LastAttemptTime = g_Game.GetTime();
			m_Time = 0;
			
			if (m_TargetEntity.GetHierarchyRoot() == unit)
				unit.StartActionObject(ActionWorldCraft, m_Item, ItemBase.Cast(m_TargetEntity));
			else
				unit.StartActionObject(ActionWorldCraft, m_TargetEntity, m_Item);
			
			return CONTINUE;
		}
		
		return EXIT;
	}

	int Guard()
	{
		if (unit.IsFighting()) return eAITransition.FAIL;
		if (unit.IsRaised()) return eAITransition.FAIL;
		
		if (unit.IsRestrained()) return eAITransition.FAIL;
		if (unit.IsUnconscious()) return eAITransition.FAIL;
		if (unit.IsSwimming()) return eAITransition.FAIL;
		
		if (!unit.GetCommand_Move()) return eAITransition.FAIL;

		if (unit.m_eAI_DangerousAreaCount > 0 && unit.m_eAI_IsInDangerByArea)
			return eAITransition.FAIL;

		if (unit.GetThreatToSelf() > 0.2)
			return eAITransition.FAIL;
		
		if (g_Game.GetTime() - m_LastAttemptTime < 4000) return eAITransition.FAIL;
		
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
