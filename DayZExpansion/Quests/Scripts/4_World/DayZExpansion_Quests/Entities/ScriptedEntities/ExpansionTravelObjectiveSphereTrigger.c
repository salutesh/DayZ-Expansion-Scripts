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
		QuestPrint(ToString() + "::SetObjectiveData - Start");

		super.SetObjectiveData(objective);
		
		float distance;
		int objectiveType = objective.GetObjectiveType();
		if (objectiveType == ExpansionQuestObjectiveType.TRAVEL)
		{
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
		}
		else if (objectiveType == ExpansionQuestObjectiveType.COLLECT)
		{
			ExpansionQuestObjectiveCollectionConfig collectionConfig;
			if (Class.CastTo(collectionConfig, objective.GetObjectiveConfig()))
			{
				distance = collectionConfig.GetMaxDistance();
				if (distance <= 0)
					distance = 5.0;

				SetTriggerRadius(distance);
			}
		}
		else if (objectiveType == ExpansionQuestObjectiveType.DELIVERY)
		{
			ExpansionQuestObjectiveDeliveryConfig deliveryConfig;
			if (Class.CastTo(deliveryConfig, objective.GetObjectiveConfig()))
			{
				distance = deliveryConfig.GetMaxDistance();
				if (distance <= 0)
					distance = 5.0;
				
				SetTriggerRadius(distance);
			}
		}
		else if (objectiveType == ExpansionQuestObjectiveType.TREASUREHUNT)
		{
			ExpansionQuestObjectiveTreasureHuntConfig treasureHuntConfig;
			if (Class.CastTo(treasureHuntConfig, objective.GetObjectiveConfig()))
			{
				distance = treasureHuntConfig.GetMaxDistance();
				if (distance <= 0)
					distance = 20.0;

				SetTriggerRadius(distance);
				ClearEventMask(EntityEvent.LEAVE);
			}
		}

		QuestPrint(ToString() + "::SetObjectiveData - End");
	}

	//! Event called when an entity enters the trigger area.
	override void EOnEnter(IEntity other, int extra)
	{
		QuestPrint(ToString() + "::EOnEnter - Start");
		QuestPrint(ToString() + "::EOnEnter - Entity: " + other.Type());

		if (GetGame().IsClient())
			return;

		if (!m_Objective)
			return;

		bool result = EntityConditions(other);
		if (!result)
		{
			QuestPrint(ToString() + "::EOnEnter - Entity conditions result: " + result);
			return;
		}

		int objectiveType = m_Objective.GetObjectiveType();
		string objectiveTypeText = typename.EnumToString(ExpansionQuestObjectiveType, objectiveType);
		QuestPrint(ToString() + "::EOnEnter - Objective type: " + objectiveTypeText);
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

		QuestPrint(ToString() + "::EOnEnter - End");
	}
	
	protected void OnEnter_Delivery()
	{
		QuestPrint(ToString() + "::OnEnter_Delivery - Start");

		ExpansionQuestObjectiveDeliveryEvent deliveryEvent;
		if (!Class.CastTo(deliveryEvent, m_Objective))
			return;

		deliveryEvent.SetReachedLocation(true);

		QuestPrint(ToString() + "::OnEnter_Delivery - End");
	}

	protected void OnEnter_Collection()
	{
		QuestPrint(ToString() + "::OnEnter_Collection - Start");

		ExpansionQuestObjectiveCollectionEvent collectionEvent;
		if (!Class.CastTo(collectionEvent, m_Objective))
			return;

		collectionEvent.SetReachedLocation(true);

		QuestPrint(ToString() + "::OnEnter_Collection - End");
	}
	
	protected void OnEnter_Travel()
	{
		QuestPrint(ToString() + "::OnEnter_Travel - Start");

		ExpansionQuestObjectiveTravelEvent travelEvent;
		if (!Class.CastTo(travelEvent, m_Objective))
			return;

		travelEvent.SetReachedLocation(true);

		QuestPrint(ToString() + "::OnEnter_Travel - End");
	}

	protected void OnEnter_TreasureHunt()
	{
		QuestPrint(ToString() + "::OnEnter_TreasureHunt - Start");

		ExpansionQuestObjectiveTreasureHuntEvent treasureHuntEvent;
		if (!Class.CastTo(treasureHuntEvent, m_Objective))
			return;

		treasureHuntEvent.SetReachedLocation(true);

		QuestPrint(ToString() + "::OnEnter_TreasureHunt - End");
	}

	//! Event called when an entity leaves the trigger area.
	override void EOnLeave(IEntity other, int extra)
	{
		QuestPrint(ToString() + "::EOnLeave - Start");

		if (GetGame().IsClient())
			return;

		if (!m_Objective)
			return;

		bool result = EntityConditions(other);
		if (!result)
		{
			QuestPrint(ToString() + "::EOnLeave - Entity conditions result: " + result);
			return;
		}

		int objectiveType = m_Objective.GetObjectiveType();
		string objectiveTypeText = typename.EnumToString(ExpansionQuestObjectiveType, objectiveType);
		QuestPrint(ToString() + "::EOnLeave - Objective type: " + objectiveTypeText);
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

		QuestPrint(ToString() + "::EOnLeave - End");
	}
	
	protected void OnLeave_Delivery()
	{
		QuestPrint(ToString() + "::OnLeave_Delivery - Start");

		ExpansionQuestObjectiveDeliveryEvent deliveryEvent;
		if (!Class.CastTo(deliveryEvent, m_Objective))
			return;

		deliveryEvent.SetReachedLocation(false);

		QuestPrint(ToString() + "::OnLeave_Delivery - End");
	}

	protected void OnLeave_Collection()
	{
		QuestPrint(ToString() + "::OnLeave_Collection - Start");

		ExpansionQuestObjectiveCollectionEvent collectionEvent;
		if (!Class.CastTo(collectionEvent, m_Objective))
			return;

		collectionEvent.SetReachedLocation(false);

		QuestPrint(ToString() + "::OnLeave_Collection - End");
	}

	protected void OnLeave_Travel()
	{
		QuestPrint(ToString() + "::OnLeave_Travel - Start");

		ExpansionQuestObjectiveTravelEvent travelEvent;
		if (!Class.CastTo(travelEvent, m_Objective))
			return;

		travelEvent.SetReachedLocation(false);

		QuestPrint(ToString() + "::OnLeave_Travel - End");
	}

	/*protected void OnLeave_TreasureHunt()
	{
		QuestPrint(ToString() + "::OnLeave_TreasureHunt - Start");

		ExpansionQuestObjectiveTreasureHuntEvent treasureHuntEvent;
		if (!Class.CastTo(treasureHuntEvent, m_Objective))
			return;

		treasureHuntEvent.SetReachedLocation(false);

		QuestPrint(ToString() + "::OnLeave_TreasureHunt - End");
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
