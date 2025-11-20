/**
 * ExpansionQuestObjectiveActionEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveActionEvent: ExpansionQuestObjectiveEventBase
{
	protected bool m_ActionState;
	protected int m_ExecutionAmount;
	protected int m_ExecutionCount;
	protected int m_UpdateCount;
	protected ref ExpansionQuestObjectiveActionConfig m_ActionConfig;

	override bool OnEventStart()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!super.OnEventStart())
			return false;

		if (!Class.CastTo(m_ActionConfig, m_ObjectiveConfig))
			return false;

		return true;
	}

	override bool OnContinue()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!super.OnContinue())
			return false;

		if (!Class.CastTo(m_ActionConfig, m_ObjectiveConfig))
			return false;

		m_Quest.QuestCompletionCheck();

		return true;
	}

	void OnObjectiveActionExecuted(ActionBase actionBase, ActionData actionData)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (IsCompleted() || m_ActionConfig.GetActionNames().Find(actionBase.ClassName()) == -1)
			return;

		if (!MissionBaseWorld.Cast(g_Game.GetMission()).Expansion_CheckActionSuccess(actionBase, actionData))
			return;

		if (actionData.m_Target || actionData.m_MainItem)
		{
			array<Object> objects = {};

			if (actionData.m_Target)
			{
				Object targetObject = actionData.m_Target.GetObject();
				Object targetParent = actionData.m_Target.GetParent();

				if (targetObject)
					objects.Insert(targetObject);

				if (targetParent && targetParent != targetObject)
					objects.Insert(targetParent);
			}

			if (actionData.m_MainItem)
				objects.Insert(actionData.m_MainItem);

			TStringArray excluded = m_ActionConfig.GetExcludedClassNames();
			TStringArray allowed = m_ActionConfig.GetAllowedClassNames();

			bool isAllowed = allowed.Count() == 0;

			foreach (Object obj: objects)
			{
				if (ExpansionStatic.IsAnyOf(obj, excluded))
					return;

				if (!isAllowed && ExpansionStatic.IsAnyOf(obj, allowed))
				{
					isAllowed = true;
					break;
				}
			}

			if (!isAllowed)
				return;
		}

		OnActionExecutionValid();
	}

	protected void OnActionExecutionValid()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		m_ExecutionCount++;
		bool updateQuest;
		if (m_UpdateCount != m_ExecutionCount)
		{
			m_UpdateCount = m_ExecutionCount;
			updateQuest = true;
		}

		if (m_ExecutionCount == m_ExecutionAmount)
		{
			m_ActionState = true;
			m_Quest.QuestCompletionCheck(true);
		}
		else if (updateQuest)
		{
			m_Quest.UpdateQuest(false);
		}
	}

	void SetExecutionCount(int count)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		m_ExecutionCount = count;

		ObjectivePrint("m_ExecutionCount: " + m_ExecutionCount);
	}

	int GetExecutionCount()
	{
		return m_ExecutionCount;
	}

	void SetExecutionAmount(int amount)
	{
		m_ExecutionAmount = amount;
	}

	int GetExecutionAmount()
	{
		return m_ExecutionAmount;
	}

	bool GetActionState()
	{
		return m_ActionState;
	}

	void SetActionState(bool state)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		m_ActionState = state;

		ObjectivePrint("m_ActionState: " + m_ActionState);
	}

	override bool CanComplete()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		ObjectivePrint("m_ExecutionCount: " + m_ExecutionCount);
		ObjectivePrint("m_ExecutionAmount: " + m_ExecutionAmount);
		ObjectivePrint("m_ActionState: " + m_ActionState);

		if (m_ExecutionCount == 0)
			return false;

		if (m_ActionState && m_ExecutionCount == m_ExecutionAmount)
			return true;

		return false;
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.ACTION;
	}
};