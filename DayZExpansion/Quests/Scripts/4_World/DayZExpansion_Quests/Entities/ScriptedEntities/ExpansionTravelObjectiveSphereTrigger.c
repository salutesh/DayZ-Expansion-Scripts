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
	override void SetObjectiveData(ExpansionQuestObjectiveEventBase objective)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		super.SetObjectiveData(objective);
		
		float distance;
		switch (objective.GetObjectiveType())
		{
		case ExpansionQuestObjectiveType.TRAVEL:
			ExpansionQuestObjectiveTravelConfig travelConfig;
			if (Class.CastTo(travelConfig, objective.GetObjectiveConfig()))
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
		case ExpansionQuestObjectiveType.COLLECT:
			ExpansionQuestObjectiveCollectionConfig collectionConfig;
			if (Class.CastTo(collectionConfig, objective.GetObjectiveConfig()))
			{
				distance = collectionConfig.GetMaxDistance();
				if (distance <= 0)
					distance = 5.0;

				SetTriggerRadius(distance);
			}
			break;
		case ExpansionQuestObjectiveType.DELIVERY:
			ExpansionQuestObjectiveDeliveryConfig deliveryConfig;
			if (Class.CastTo(deliveryConfig, objective.GetObjectiveConfig()))
			{
				distance = deliveryConfig.GetMaxDistance();
				if (distance <= 0)
					distance = 5.0;
				
				SetTriggerRadius(distance);
			}
			break;
		case ExpansionQuestObjectiveType.TREASUREHUNT:
			ExpansionQuestObjectiveTreasureHuntConfig treasureHuntConfig;
			if (Class.CastTo(treasureHuntConfig, objective.GetObjectiveConfig()))
			{
				distance = treasureHuntConfig.GetMaxDistance();
				if (distance <= 0)
					distance = 20.0;

				SetTriggerRadius(distance);
				ClearEventMask(EntityEvent.LEAVE);
			}
			break;
		}

		EXTrace.Print(EXTrace.QUESTS, this, "Position: " + GetPosition() + " | Radius: " + distance);
	}

	//! Event called when an entity enters the trigger area.
	override void EOnEnter(IEntity other, int extra)
	{
	//#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, "Entity: " + other.Type());
	//#endif

		if (GetGame().IsClient())
			return;

		if (!m_Objective || !m_Objective.IsActive() || m_Objective.IsCompleted())
			return;

		bool result = EntityConditions(other);
		if (!result)
		{
			QuestPrint("Entity conditions result: " + result);
			return;
		}

		int objectiveType = m_Objective.GetObjectiveType();
		string objectiveTypeText = typename.EnumToString(ExpansionQuestObjectiveType, objectiveType);
		QuestPrint("Objective type: " + objectiveTypeText);
		switch (objectiveType)
		{
			case ExpansionQuestObjectiveType.DELIVERY:
			{
				OnEnter_Delivery();
			}
			break;
			case ExpansionQuestObjectiveType.COLLECT:
			{
				OnEnter_Collection();
			}
			break;
			case ExpansionQuestObjectiveType.TRAVEL:
			{
				OnEnter_Travel();
			}
			break;
			case ExpansionQuestObjectiveType.TREASUREHUNT:
			{
				OnEnter_TreasureHunt();
			}
			break;
		}
	}
	
	protected void OnEnter_Delivery()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		ExpansionQuestObjectiveDeliveryEvent deliveryEvent;
		if (!Class.CastTo(deliveryEvent, m_Objective))
			return;

		deliveryEvent.SetReachedLocation(true);
	}

	protected void OnEnter_Collection()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		ExpansionQuestObjectiveCollectionEvent collectionEvent;
		if (!Class.CastTo(collectionEvent, m_Objective))
			return;

		collectionEvent.SetReachedLocation(true);
	}
	
	protected void OnEnter_Travel()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		ExpansionQuestObjectiveTravelEvent travelEvent;
		if (!Class.CastTo(travelEvent, m_Objective))
			return;

		travelEvent.SetReachedLocation(true);
	}

	protected void OnEnter_TreasureHunt()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		ExpansionQuestObjectiveTreasureHuntEvent treasureHuntEvent;
		if (!Class.CastTo(treasureHuntEvent, m_Objective))
			return;

		treasureHuntEvent.SetReachedLocation(true);
	}

	//! Event called when an entity leaves the trigger area.
	override void EOnLeave(IEntity other, int extra)
	{
	//#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, "Entity: " + other.Type());
	//#endif

		if (GetGame().IsClient())
			return;

		//! @note IsInitialized will be false during cleanup, and we don't want EOnLeave to do anything in that case
		if (!m_Objective || !m_Objective.IsInitialized())
			return;

		bool result = EntityConditions(other);
		if (!result)
		{
			QuestPrint("Entity conditions result: " + result);
			return;
		}

		int objectiveType = m_Objective.GetObjectiveType();
		string objectiveTypeText = typename.EnumToString(ExpansionQuestObjectiveType, objectiveType);
		QuestPrint("Objective type: " + objectiveTypeText);
		switch (objectiveType)
		{
			case ExpansionQuestObjectiveType.DELIVERY:
			{
				OnLeave_Delivery();
			}
			break;
			case ExpansionQuestObjectiveType.COLLECT:
			{
				OnLeave_Collection();
			}
			break;
			case ExpansionQuestObjectiveType.TRAVEL:
			{
				OnLeave_Travel();
			}
			break;
			/*case ExpansionQuestObjectiveType.TREASUREHUNT:
			{
				OnLeave_TreasureHunt();
			}
			break;*/
		}
	}
	
	protected void OnLeave_Delivery()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		ExpansionQuestObjectiveDeliveryEvent deliveryEvent;
		if (!Class.CastTo(deliveryEvent, m_Objective))
			return;

		deliveryEvent.SetReachedLocation(false);
	}

	protected void OnLeave_Collection()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		ExpansionQuestObjectiveCollectionEvent collectionEvent;
		if (!Class.CastTo(collectionEvent, m_Objective))
			return;

		collectionEvent.SetReachedLocation(false);
	}

	protected void OnLeave_Travel()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		ExpansionQuestObjectiveTravelEvent travelEvent;
		if (!Class.CastTo(travelEvent, m_Objective))
			return;

		travelEvent.SetReachedLocation(false);
	}

	/*protected void OnLeave_TreasureHunt()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		ExpansionQuestObjectiveTreasureHuntEvent treasureHuntEvent;
		if (!Class.CastTo(treasureHuntEvent, m_Objective))
			return;

		treasureHuntEvent.SetReachedLocation(false);

		
	}*/

	//! Condition checks on given entity.
	override bool EntityConditions(IEntity other)
	{
		PlayerBase player;
		if (!Class.CastTo(player, other))
			return false;

		if (!player.GetIdentity())
			return false;

		string playerUID = player.GetIdentity().GetId();
		if (playerUID == string.Empty)
			return false;

		if (!m_Objective.GetQuest())
			return false;

		if (!m_Objective.GetQuest().IsQuestPlayer(player.GetIdentity().GetId()))
			return false;

		return super.EntityConditions(other);
	}
};
