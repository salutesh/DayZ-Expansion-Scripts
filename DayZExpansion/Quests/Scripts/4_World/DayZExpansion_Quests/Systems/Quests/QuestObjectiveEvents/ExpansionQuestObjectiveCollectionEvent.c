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

		if (!Class.CastTo(m_DeliveryConfig, m_ObjectiveConfig))
			return false;

		if (!GetObjectiveDataFromConfig())
			return false;

		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ObjectiveCheck, 500);

		return true;
	}

	override bool OnContinue()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!super.OnContinue())
			return false;

		if (!Class.CastTo(m_DeliveryConfig, m_ObjectiveConfig))
			return false;

		if (!GetObjectiveDataFromConfig())
			return false;

		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ObjectiveCheck, 500);

		return true;
	}

	override bool CanComplete()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif
		ObjectivePrint("m_ObjectiveItemsCount: " + m_ObjectiveItemsCount);
		ObjectivePrint("m_ObjectiveItemsAmount: " + m_ObjectiveItemsAmount);

		if (!Class.CastTo(m_DeliveryConfig, m_ObjectiveConfig))
			return false;

		if (!m_DeliveryConfig.NeedAnyCollection())
		{
			if (m_ObjectiveItemsAmount != 0 && m_ObjectiveItemsCount >= m_ObjectiveItemsAmount)
				return true;
		}
		else
		{
			if (HasAnyCollectionCompleted())
				return true;
		}

		return false;
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.COLLECT;
	}
};