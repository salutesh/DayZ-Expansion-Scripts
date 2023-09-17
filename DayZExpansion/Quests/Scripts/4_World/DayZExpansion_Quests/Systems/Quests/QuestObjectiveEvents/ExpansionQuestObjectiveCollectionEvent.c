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
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!super.OnEventStart())
			return false;

		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return false;

		if (!CreateObjectiveTrigger())
			return false;

		if (!GetObjectiveDataFromConfig())
			return false;

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ObjectiveCheck, 500);

		return true;
	}

	override bool OnContinue()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!super.OnContinue())
			return false;

		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return false;

		if (!CreateObjectiveTrigger())
			return false;

		if (!GetObjectiveDataFromConfig())
			return false;

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ObjectiveCheck, 500);

		return true;
	}

	override bool CanComplete()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		ObjectivePrint("m_ObjectiveItemsCount: " + m_ObjectiveItemsCount);
		ObjectivePrint("m_ObjectiveItemsAmount: " + m_ObjectiveItemsAmount);
		ObjectivePrint("m_DestinationReached: " + m_DestinationReached);

		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return false;

		bool conditionsResult;
	#ifdef EXPANSIONMODNAVIGATION
		bool markerConditionResult;
	#endif
		if (!m_Config.NeedAnyCollection())
		{
			if (m_ObjectiveItemsAmount != 0 && m_ObjectiveItemsCount >= m_ObjectiveItemsAmount && m_DestinationReached)
				conditionsResult = true;

		#ifdef EXPANSIONMODNAVIGATION
			if (m_ObjectiveItemsAmount != 0 && m_ObjectiveItemsCount >= m_ObjectiveItemsAmount)
				markerConditionResult = true;
		#endif
		}
		else
		{
			if (HasAnyCollectionCompleted() && m_DestinationReached)
				conditionsResult = true;

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

		return conditionsResult;
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
				if ( m_ObjectiveItemsAmount != 0 && m_ObjectiveItemsCount >= m_ObjectiveItemsAmount)
					markerConditionResult = true;
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