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

class ExpansionObjectiveTriggerBase: Trigger
{
	protected ExpansionQuestObjectiveEventBase m_Objective;

	void ExpansionObjectiveTriggerBase()
	{
		SetEventMask(EntityEvent.ENTER | EntityEvent.LEAVE);
	}
	
	void SetObjectiveData(ExpansionQuestObjectiveEventBase objective)
	{
		m_Objective = objective;

		int radius = 10;
		if (objective.GetObjectiveConfig().GetMaxDistance() > 0)
			radius = objective.GetObjectiveConfig().GetMaxDistance();
		
		if (m_Objective.GetObjectiveType() == ExpansionQuestObjectiveType.TREASUREHUNT)
			radius = 30;

		SetCollisionSphere(radius);
	}

	//! Condition checks on given entity.
	bool EntityConditions(IEntity other)
	{
		return true;
	}

#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
	protected void TriggerDebug()
	{
		vector pos = GetPosition();
		QuestPrint(ToString() + "::TriggerDebug - Position: " + pos);
		QuestPrint(ToString() + "::TriggerDebug - Objective: " + m_Objective.ToString());
		m_Objective.QuestDebug();
	}
#endif

	override protected void AddInsider(Object obj)
	{
		//! Do nothing..
	}
	
	override protected void RemoveInsiderByObject(Object object)
	{
		//! Do nothing..
	}
	
	protected void QuestPrint(string text)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(text);
	#endif
	}
};

class ExpansionTravelObjectiveSphereTrigger: ExpansionObjectiveTriggerBase
{
	override void SetObjectiveData(ExpansionQuestObjectiveEventBase objective)
	{
		QuestPrint(ToString() + "::SetObjectiveData - Start");
		
		super.SetObjectiveData(objective);
		
		int objectiveType = objective.GetObjectiveType();
		if (objectiveType == ExpansionQuestObjectiveType.TRAVEL)
		{
			if (!objective.GetObjectiveConfig().TriggerOnEnter())
				ClearEventMask(EntityEvent.ENTER);
			
			if (!objective.GetObjectiveConfig().TriggerOnExit())
				ClearEventMask(EntityEvent.LEAVE);
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
		switch (objectiveType)
		{
			case ExpansionQuestObjectiveType.DELIVERY:
			{
				OnEnter_Delivery();
			}
			break;
			case ExpansionQuestObjectiveType.COLLECT:
			{
				OnEnter_Delivery();
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

		ExpansionQuestObjectiveCollectItemEventBase deliveryEvent;
		if (!Class.CastTo(deliveryEvent, m_Objective))
			return;

		deliveryEvent.SetReachedLocation(true);

		QuestPrint(ToString() + "::OnEnter_Delivery - End");
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
			QuestPrint(ToString() + "::EOnEnter - Entity conditions result: " + result);
			return;
		}

		int objectiveType = m_Objective.GetObjectiveType();
		switch (objectiveType)
		{
			case ExpansionQuestObjectiveType.DELIVERY:
			{
				OnLeave_Delivery();
			}
			break;
			case ExpansionQuestObjectiveType.COLLECT:
			{
				OnLeave_Delivery();
			}
			break;
			case ExpansionQuestObjectiveType.TRAVEL:
			{
				OnLeave_Travel();
			}
			break;
			case ExpansionQuestObjectiveType.TREASUREHUNT:
			{
				OnLeave_TreasureHunt();
			}
			break;
		}

		QuestPrint(ToString() + "::EOnLeave - End");
	}

	protected void OnLeave_Delivery()
	{
		QuestPrint(ToString() + "::OnLeave_Delivery - Start");

		ExpansionQuestObjectiveCollectItemEventBase deliveryEvent;
		if (!Class.CastTo(deliveryEvent, m_Objective))
			return;

		deliveryEvent.SetReachedLocation(false);

		QuestPrint(ToString() + "::OnLeave_Delivery - End");
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
	
	protected void OnLeave_TreasureHunt()
	{
		QuestPrint(ToString() + "::OnLeave_TreasureHunt - Start");

		ExpansionQuestObjectiveTreasureHuntEvent treasureHuntEvent;
		if (!Class.CastTo(treasureHuntEvent, m_Objective))
			return;

		treasureHuntEvent.SetReachedLocation(false);

		QuestPrint(ToString() + "::OnLeave_TreasureHunt - End");
	}

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
