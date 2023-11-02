/**
 * ExpansionQuestObjectiveCollectionEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveCollectionEvent: ExpansionQuestObjectiveCollectionEventBase
{
	override bool OnEventStart()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!super.OnEventStart())
			return false;

		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return false;

		if (!GetObjectiveDataFromConfig())
			return false;

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ObjectiveCheck, 500);

		return true;
	}

	override bool OnContinue()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!super.OnContinue())
			return false;

		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return false;

		if (!GetObjectiveDataFromConfig())
			return false;

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ObjectiveCheck, 500);

		return true;
	}

	override bool CanComplete()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif
		ObjectivePrint("m_ObjectiveItemsCount: " + m_ObjectiveItemsCount);
		ObjectivePrint("m_ObjectiveItemsAmount: " + m_ObjectiveItemsAmount);

		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return false;

		bool conditionsResult;
		if (!m_Config.NeedAnyCollection())
		{
			if (m_ObjectiveItemsAmount != 0 && m_ObjectiveItemsCount >= m_ObjectiveItemsAmount)
				conditionsResult = true;
		}
		else
		{
			if (HasAnyCollectionCompleted())
				conditionsResult = true;
		}

		return conditionsResult;
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.COLLECT;
	}
};