/**
 * ExpansionTravelObjectiveSphereTrigger.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionTravelObjectiveSphereTrigger: ExpansionObjectiveTriggerBase
{
	void ExpansionTravelObjectiveSphereTrigger()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		SetEventMask(EntityEvent.ENTER | EntityEvent.LEAVE);
		m_TriggerType = ExpansionObjectiveTriggerType.TRAVEL;
	}

	override void SetObjectiveData(int questID, int objectiveType, int objectiveID)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		super.SetObjectiveData(questID, objectiveType, objectiveID);
		
		float distance;
		switch (objectiveType)
		{
		case ExpansionQuestObjectiveType.TRAVEL:
			ExpansionQuestObjectiveTravelConfig travelConfig = ExpansionQuestModule.GetModuleInstance().GetTravelObjectiveConfigByID(objectiveID);
			if (travelConfig)
			{
				if (!travelConfig.TriggerOnEnter())
					ClearEventMask(EntityEvent.ENTER);
	
				if (!travelConfig.TriggerOnExit())
					ClearEventMask(EntityEvent.LEAVE);
				
				distance = travelConfig.GetMaxDistance();
				if (distance <= 0)
					distance = 5.0;

				SetTriggerRadius(distance);
			}
			break;
		case ExpansionQuestObjectiveType.DELIVERY:
			ExpansionQuestObjectiveDeliveryConfig deliveryConfig = ExpansionQuestModule.GetModuleInstance().GetDeliveryObjectiveConfigByID(objectiveID);
			if (deliveryConfig)
			{
				distance = deliveryConfig.GetMaxDistance();
				if (distance <= 0)
					distance = 5.0;
				
				SetTriggerRadius(distance);
			}
			break;
		case ExpansionQuestObjectiveType.TREASUREHUNT:
			ExpansionQuestObjectiveTreasureHuntConfig treasureHuntConfig = ExpansionQuestModule.GetModuleInstance().GetTreasureHuntObjectiveConfigByID(objectiveID);
			if (treasureHuntConfig)
			{
				distance = treasureHuntConfig.GetMaxDistance();
				if (distance <= 0)
					distance = 20.0;

				SetTriggerRadius(distance);
			}
			break;
		}

		EXTrace.Print(EXTrace.QUESTS, this, "Position: " + GetPosition() + " | Radius: " + distance);
	}

	//! Event called when an entity enters the trigger area.
	override void EOnEnter(IEntity other, int extra)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, "Entity: " + other.Type());
	#endif

	#ifdef SERVER		
		PlayerBase player;
		if (!Class.CastTo(player, other))
			return;

		if (!player.GetIdentity())
			return;
		
		ExpansionQuest quest = ExpansionQuestModule.GetModuleInstance().GetActiveQuestWithQuestID(player, m_QuestID);
		if (!quest)
			return;
		
		ExpansionQuestObjectiveEventBase questObjective = quest.GetObjective(m_ObjectiveType, m_ObjectiveID);
		if (!questObjective)
			return;

		if (!questObjective || !questObjective.IsActive() || questObjective.IsCompleted())
			return;
		
		string objectiveTypeText = typename.EnumToString(ExpansionQuestObjectiveType, m_ObjectiveType);
		QuestPrint("::EOnEnter - Objective type: " + objectiveTypeText);
		switch (m_ObjectiveType)
		{
			case ExpansionQuestObjectiveType.DELIVERY:
			{
				OnEnter_Delivery(questObjective);
			}
			break;
			case ExpansionQuestObjectiveType.TRAVEL:
			{
				ExpansionQuestObjectiveTravelConfig travelConfig;
				if (Class.CastTo(travelConfig, questObjective.GetObjectiveConfig()))
				{
					QuestPrint("::EOnLeave - Trigger on enter: " + travelConfig.TriggerOnEnter() + " | Objective ID: " + travelConfig.GetID());
					if (travelConfig.TriggerOnEnter())
						OnEnter_Travel(questObjective);
				}
			}
			break;
			case ExpansionQuestObjectiveType.TREASUREHUNT:
			{
				OnEnter_TreasureHunt(questObjective);
			}
			break;
		}
	#endif
	}
	
	protected void OnEnter_Delivery(ExpansionQuestObjectiveEventBase questObjective)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		ExpansionQuestObjectiveDeliveryEvent deliveryEvent;
		if (!Class.CastTo(deliveryEvent, questObjective))
			return;

		deliveryEvent.SetReachedLocation(true);
	}
	
	protected void OnEnter_Travel(ExpansionQuestObjectiveEventBase questObjective)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		ExpansionQuestObjectiveTravelEvent travelEvent;
		if (!Class.CastTo(travelEvent, questObjective))
			return;

		travelEvent.SetReachedLocation(true);
	}

	protected void OnEnter_TreasureHunt(ExpansionQuestObjectiveEventBase questObjective)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		ExpansionQuestObjectiveTreasureHuntEvent treasureHuntEvent;
		if (!Class.CastTo(treasureHuntEvent, questObjective))
			return;

		treasureHuntEvent.SetReachedLocation(true);
	}

	//! Event called when an entity leaves the trigger area.
	override void EOnLeave(IEntity other, int extra)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, "Entity: " + other.Type());
	#endif

	#ifdef SERVER
		PlayerBase player;
		if (!Class.CastTo(player, other))
			return;

		if (!player.GetIdentity())
			return;
		
		ExpansionQuest quest = ExpansionQuestModule.GetModuleInstance().GetActiveQuestWithQuestID(player, m_QuestID);
		if (!quest)
			return;
		
		ExpansionQuestObjectiveEventBase questObjective = quest.GetObjective(m_ObjectiveType, m_ObjectiveID);
		//! @note IsInitialized will be false during cleanup, and we don't want EOnLeave to do anything in that case
		if (!questObjective || !questObjective.IsInitialized())
			return;

		string objectiveTypeText = typename.EnumToString(ExpansionQuestObjectiveType, m_ObjectiveType);
		QuestPrint("::EOnLeave - Objective type: " + objectiveTypeText);
		switch (m_ObjectiveType)
		{
			case ExpansionQuestObjectiveType.DELIVERY:
			{
				OnLeave_Delivery(questObjective);
			}
			break;
			case ExpansionQuestObjectiveType.TRAVEL:
			{
				ExpansionQuestObjectiveTravelConfig travelConfig;
				if (Class.CastTo(travelConfig, questObjective.GetObjectiveConfig()))
				{
					QuestPrint("::EOnLeave - Trigger on exit: " + travelConfig.TriggerOnExit() + " | Objective ID: " + travelConfig.GetID());
					if (travelConfig.TriggerOnExit())
						OnLeave_Travel(questObjective);
				}
			}
			break;
			case ExpansionQuestObjectiveType.TREASUREHUNT:
			{
				OnLeave_TreasureHunt(questObjective);
			}
			break;
		}
	#endif
	}
	
	protected void OnLeave_Delivery(ExpansionQuestObjectiveEventBase questObjective)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		ExpansionQuestObjectiveDeliveryEvent deliveryEvent;
		if (!Class.CastTo(deliveryEvent, questObjective))
			return;

		deliveryEvent.SetReachedLocation(false);
	}

	protected void OnLeave_Travel(ExpansionQuestObjectiveEventBase questObjective)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		ExpansionQuestObjectiveTravelEvent travelEvent;
		if (!Class.CastTo(travelEvent, questObjective))
			return;

		travelEvent.SetReachedLocation(false);
	}

	protected void OnLeave_TreasureHunt(ExpansionQuestObjectiveEventBase questObjective)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		ExpansionQuestObjectiveTreasureHuntEvent treasureHuntEvent;
		if (!Class.CastTo(treasureHuntEvent, questObjective))
			return;

		treasureHuntEvent.SetReachedLocation(false);		
	}
};
