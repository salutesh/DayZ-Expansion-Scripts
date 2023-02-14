/**
 * ExpansionQuestObjectiveActionEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveActionEventData
{
	ActionBase m_ActionBase;
	ActionData m_ActionData;
	string m_ActionName;
	EntityAI m_Target;

	int m_ConditionInt;

	void SetTargetActionTarget()
	{
		m_Target = EntityAI.Cast(m_ActionData.m_Target.GetParentOrObject());
	}

	void SetTargetActionPlayer()
	{
		m_Target = m_ActionData.m_Player;
	}

	void SetTargetActionMainItem()
	{
		m_Target = m_ActionData.m_MainItem;
	}
};

class ExpansionQuestObjectiveActionEvent: ExpansionQuestObjectiveEventBase
{
	protected bool m_ActionState;
	protected int m_ExecutionAmount;
	protected int m_ExecutionCount;
	protected int m_UpdateCount;
	protected ref ExpansionQuestObjectiveActionEventData m_ActionEventData;

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveActionEvent OnEventStart
	// -----------------------------------------------------------
	override bool OnEventStart()
	{
		ObjectivePrint(ToString() + "::OnEventStart - Start");
		
		if (!super.OnEventStart())
			return false;

		m_ExecutionAmount = m_ObjectiveConfig.GetExecutionAmount();

		ObjectivePrint(ToString() + "::OnEventStart - End and return TRUE.");
		
		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveActionEvent OnContinue
	// -----------------------------------------------------------
	override bool OnContinue()
	{
		ObjectivePrint(ToString() + "::OnContinue - Start");
		
		if (!super.OnContinue())
			return false;

		m_ExecutionAmount = m_ObjectiveConfig.GetExecutionAmount();
		m_Quest.QuestCompletionCheck();

		ObjectivePrint(ToString() + "::OnContinue - End and return TRUE.");
		
		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveActionEvent OnObjectiveActionExecuted
	// -----------------------------------------------------------
	void OnObjectiveActionExecuted(ActionBase actionBase, ActionData actionData, bool isInit = false)
	{
		ObjectivePrint(ToString() + "::OnObjectiveActionExecuted - Start");

		if (IsCompleted() || m_ObjectiveConfig.GetActionNames().Find(actionBase.ClassName()) == -1)
			return;

		string methodName = GetActionMethodName(actionBase.ClassName());
		if (methodName == string.Empty)
		{
			ObjectivePrint(ToString() + "::OnActionUsed - No event handling for action " + actionBase.ClassName() + ".");
			return;
		}

		if (!m_ActionEventData)
		{
			m_ActionEventData = new ExpansionQuestObjectiveActionEventData();
			m_ActionEventData.m_ActionBase = actionBase;
			m_ActionEventData.m_ActionData = actionData;
			m_ActionEventData.m_ActionName = actionBase.ClassName();
		}

		HandleAction(methodName, isInit);

		ObjectivePrint(ToString() + "::OnObjectiveActionExecuted - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveActionEvent GetActionMethodName
	// -----------------------------------------------------------
	protected string GetActionMethodName(string actionName)
	{
		foreach (ExpansionQuestAction action: GetExpansionSettings().GetQuest().QuestActions)
		{
			if (actionName == action.ActionName)
				return action.MethodName;
		}

		return string.Empty;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveActionEvent HandleAction
	// -----------------------------------------------------------
	protected void HandleAction(string methodName, bool isInit = false)
	{
		ScriptModule module = GetGame().GetMission().MissionScript;
		module.Call(this, methodName, isInit);
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveActionEvent OnActionBandageSelf
	// -----------------------------------------------------------
	//! Action handling methods
	protected void OnActionBandageSelf(bool isInit = false)
	{
		ObjectivePrint(ToString() + "::OnActionBandageSelf - Start");

		if (!m_ActionEventData)
			return;

		PlayerBase targetPlayer;
		if (isInit)
		{
			m_ActionEventData.SetTargetActionPlayer();
			targetPlayer = PlayerBase.Cast(m_ActionEventData.m_Target);
			if (!targetPlayer)
				return;

			m_ActionEventData.m_ConditionInt = targetPlayer.GetBleedingSourceCount(); //! Get current bleeding source count.
		}
		else
		{
			targetPlayer = PlayerBase.Cast(m_ActionEventData.m_Target);
			if (m_ActionEventData.m_ConditionInt > targetPlayer.GetBleedingSourceCount()) //! Compare bleeding source count after action execution.
			{
				OnActionExecutionValid();
				ClearActionData();
			}
		}

		ObjectivePrint(ToString() + "::OnActionBandageSelf - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveActionEvent OnActionBandageTarget
	// -----------------------------------------------------------
	protected void OnActionBandageTarget(bool isInit = false)
	{
		ObjectivePrint(ToString() + "::OnActionBandageTarget - Start");

		if (!m_ActionEventData)
			return;

		PlayerBase targetPlayer;
		if (isInit)
		{
			m_ActionEventData.SetTargetActionTarget();
			targetPlayer = PlayerBase.Cast(m_ActionEventData.m_Target);
			if (!targetPlayer)
				return;

			m_ActionEventData.m_ConditionInt = targetPlayer.GetBleedingSourceCount(); //! Get current bleeding source count.
		}
		else
		{
			targetPlayer = PlayerBase.Cast(m_ActionEventData.m_Target);
			if (m_ActionEventData.m_ConditionInt > targetPlayer.GetBleedingSourceCount()) //! Compare bleeding source count after action execution.
			{
				OnActionExecutionValid();
				ClearActionData();
			}
		}

		ObjectivePrint(ToString() + "::OnActionBandageTarget - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveActionEvent OnExpansionActionPickVehicleLock
	// -----------------------------------------------------------
	protected void OnExpansionActionPickVehicleLock(bool isInit = false)
	{
		ObjectivePrint(ToString() + "::OnExpansionActionPickVehicleLock - Start");

		if (!m_ActionEventData)
			return;

		if (isInit)
		{
			m_ActionEventData.SetTargetActionTarget();
		}
		else
		{
			CarScript vehicle = CarScript.Cast(m_ActionEventData.m_Target);
			if (vehicle && vehicle.GetLockedState() == ExpansionVehicleLockState.FORCEDUNLOCKED)
			{
				OnActionExecutionValid();
				ClearActionData();
			}
		}

		ObjectivePrint(ToString() + "::OnExpansionActionPickVehicleLock - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveActionEvent OnExpansionVehicleActionPickLock
	// -----------------------------------------------------------
	protected void OnExpansionVehicleActionPickLock(bool isInit = false)
	{
		ObjectivePrint(ToString() + "::OnExpansionVehicleActionPickLock - Start");

		if (!m_ActionEventData)
			return;

		if (isInit)
		{
			m_ActionEventData.SetTargetActionTarget();
		}
		else
		{
			ExpansionVehicleBase vehicle = ExpansionVehicleBase.Cast(m_ActionEventData.m_Target);
			if (vehicle && vehicle.GetLockedState() == ExpansionVehicleLockState.FORCEDUNLOCKED)
			{
				OnActionExecutionValid();
				ClearActionData();
			}
		}

		ObjectivePrint(ToString() + "::OnExpansionVehicleActionPickLock - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveActionEvent OnActionPlantSeed
	// -----------------------------------------------------------
	protected void OnActionPlantSeed(bool isInit = false)
	{
		ObjectivePrint(ToString() + "::OnActionPlantSeed - Start");

		if (!m_ActionEventData)
			return;

		m_ActionEventData.SetTargetActionMainItem();
		ObjectivePrint(ToString() + "::OnActionPlantSeed - Item target: " + m_ActionEventData.m_Target.GetType());
		bool isExculuded = ExpansionStatic.IsAnyOf(m_ActionEventData.m_Target, m_ObjectiveConfig.GetExcludedClassNames());
		bool isAllowed = ExpansionStatic.IsAnyOf(m_ActionEventData.m_Target, m_ObjectiveConfig.GetAllowedClassNames());

		ObjectivePrint(ToString() + "::OnActionPlantSeed - Is Excluded: " + isExculuded);
		ObjectivePrint(ToString() + "::OnActionPlantSeed - Is Allowed: " + isAllowed);

		if (!isExculuded && isAllowed)
		{
			OnActionExecutionValid();
			ClearActionData();
		}

		ObjectivePrint(ToString() + "::OnActionPlantSeed - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveActionEvent OnActionExecutionValid
	// -----------------------------------------------------------
	protected void OnActionExecutionValid()
	{
		ObjectivePrint(ToString() + "::OnActionExecutionValid - Start");

		m_ExecutionCount++;
		if (m_UpdateCount != m_ExecutionCount)
		{
			m_UpdateCount = m_ExecutionCount;
			m_Quest.UpdateQuest();
		}

		if (m_ExecutionCount == m_ExecutionAmount)
		{
			m_ActionState = true;
			m_Quest.QuestCompletionCheck();
		}

		ObjectivePrint(ToString() + "::OnActionExecutionValid - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveActionEvent ClearActionData
	// -----------------------------------------------------------
	protected void ClearActionData()
	{
		m_ActionEventData = NULL;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveActionEvent SetExecutionCount
	// -----------------------------------------------------------
	void SetExecutionCount(int count)
	{
		ObjectivePrint(ToString() + "::SetExecutionCount - Start");
		
		m_ExecutionCount = count;
		
		ObjectivePrint(ToString() + "::SetExecutionCount - m_ExecutionCount: " + m_ExecutionCount);
		ObjectivePrint(ToString() + "::SetExecutionCount - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveActionEvent GetExecutionCount
	// -----------------------------------------------------------
	int GetExecutionCount()
	{
		return m_ExecutionCount;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveActionEvent GetExecutionAmount
	// -----------------------------------------------------------
	int GetExecutionAmount()
	{
		return m_ExecutionAmount;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveActionEvent GetActionState
	// -----------------------------------------------------------
	bool GetActionState()
	{
		return m_ActionState;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveActionEvent SetActionState
	// -----------------------------------------------------------
	void SetActionState(bool state)
	{
		ObjectivePrint(ToString() + "::SetActionState - Start");

		m_ActionState = state;

		ObjectivePrint(ToString() + "::SetActionState - m_ActionState: " + m_ActionState);
		ObjectivePrint(ToString() + "::SetActionState - End");
	}
	
	// -----------------------------------------------------------
	// ExpansionQuestObjectiveActionEvent CanComplete
	// -----------------------------------------------------------
	override bool CanComplete()
	{
		ObjectivePrint(ToString() + "::CanComplete - Start");
		ObjectivePrint(ToString() + "::CanComplete - m_ExecutionCount: " + m_ExecutionCount);
		ObjectivePrint(ToString() + "::CanComplete - m_ExecutionAmount: " + m_ExecutionAmount);
		ObjectivePrint(ToString() + "::CanComplete - m_ActionState: " + m_ActionState);
		
		if (m_ExecutionCount == 0)
			return false;
		
		bool conditionsResult = m_ActionState && (m_ExecutionCount == m_ExecutionAmount);
		if (!conditionsResult)
		{
			ObjectivePrint(ToString() + "::CanComplete - End and return: FALSE");
			return false;
		}

		ObjectivePrint(ToString() + "::CanComplete - End and return: TRUE");

		return super.CanComplete();
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveActionEvent GetObjectiveType
	// -----------------------------------------------------------
	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.ACTION;
	}
};