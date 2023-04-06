/**
 * ExpansionQuestObjectiveCollectionEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveCollectionEvent: ExpansionQuestObjectiveCollectionEventBase
{
	override bool OnEventStart()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!super.OnEventStart())
			return false;
		
		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return false;

		if (!CreateObjectiveTrigger())
			return false;

		if (!GetObjectiveDataFromConfig())
			return false;

		if (!CheckQuestPlayersForObjectiveItems())
			return false;

		UpdateDeliveryData();

		m_Quest.QuestCompletionCheck(true);

		return true;
	}

	override bool OnContinue()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!super.OnContinue())
			return false;
		
		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return false;

		//! Only create the trigger if not already completed!
		if (m_Quest.GetQuestState() == ExpansionQuestState.STARTED)
		{
			if (!CreateObjectiveTrigger())
				return false;
		}

		if (!GetObjectiveDataFromConfig())
			return false;

		if (!CheckQuestPlayersForObjectiveItems())
			return false;

		UpdateDeliveryData();
		
		m_Quest.QuestCompletionCheck(true);

		return true;
	}

	override bool CanComplete()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		ObjectivePrint("m_ObjectiveItemsCount: " + m_ObjectiveItemsCount);
		ObjectivePrint("m_ObjectiveItemsAmount: " + m_ObjectiveItemsAmount);
		
		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return false;

		bool conditionsResult;
	#ifdef EXPANSIONMODNAVIGATION
		bool markerConditionResult;
	#endif
		if (!m_Config.NeedAnyCollection())
		{
			conditionsResult = m_ObjectiveItemsAmount != 0 && (m_ObjectiveItemsCount >= m_ObjectiveItemsAmount) && m_DestinationReached;
		#ifdef EXPANSIONMODNAVIGATION
			markerConditionResult = m_ObjectiveItemsAmount != 0 && (m_ObjectiveItemsCount >= m_ObjectiveItemsAmount);
		#endif
		}
		else
		{
			conditionsResult = HasAnyCollectionCompleted() && m_DestinationReached;
		#ifdef EXPANSIONMODNAVIGATION
			markerConditionResult = HasAnyCollectionCompleted();
		#endif
		}

	#ifdef EXPANSIONMODNAVIGATION
		if (markerConditionResult && !m_CreatedMarker && m_Config.GetMarkerName() != string.Empty)
		{
			CreateMarkers();
			m_CreatedMarker = true;
		}
		else if (m_CreatedMarker && !markerConditionResult)
		{
			RemoveObjectiveMarkers();
			m_CreatedMarker = false;
		}
	#endif

		if (!conditionsResult)
		{
			ObjectivePrint("End and return: FALSE");
			return false;
		}

		ObjectivePrint("End and return: TRUE");

		return true;
	}

	override bool OnIncomplete()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!super.OnIncomplete())
			return false;

	#ifdef EXPANSIONMODNAVIGATION
		if (m_Config.GetMarkerName() != string.Empty)
		{
			RemoveObjectiveMarkers();
			m_CreatedMarker = false;

			bool markerConditionResult;
			if (!m_Config.NeedAnyCollection() || m_Config.GetObjectiveType() == ExpansionQuestObjectiveType.DELIVERY)
			{
				markerConditionResult = m_ObjectiveItemsAmount != 0 && (m_ObjectiveItemsCount >= m_ObjectiveItemsAmount);
			}
			else if (m_Config.NeedAnyCollection() && m_Config.GetObjectiveType() == ExpansionQuestObjectiveType.COLLECT)
			{
				markerConditionResult = HasAnyCollectionCompleted();
			}

			if (markerConditionResult && m_Config.GetMarkerName() != string.Empty)
			{
				CreateMarkers();
				m_CreatedMarker = true;
			}
		}
	#endif

		return true;
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.COLLECT;
	}
};