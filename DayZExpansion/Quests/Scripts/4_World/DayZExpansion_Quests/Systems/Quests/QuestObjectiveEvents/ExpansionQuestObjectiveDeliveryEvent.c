/**
 * ExpansionQuestObjectiveDeliveryEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveDeliveryEvent: ExpansionQuestObjectiveCollectionEventBase
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
		
		if (!SpawnObjectiveDeliveryItems())
			return false;
		
		CheckQuestPlayersForObjectiveItems();

		UpdateDeliveryData();

	#ifdef EXPANSIONMODNAVIGATION
		if (m_Config.GetMarkerName() != string.Empty)
			CreateMarkers();
	#endif
		
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
		
		CheckQuestPlayersForObjectiveItems();
		
		UpdateDeliveryData();
		
	#ifdef EXPANSIONMODNAVIGATION
		//! Only create the marker if not already completed!
		if (m_Quest.GetQuestState() == ExpansionQuestState.STARTED)
		{
			if (m_Config.GetMarkerName() != string.Empty)
				CreateMarkers();
		}
	#endif
		
		m_Quest.QuestCompletionCheck(true);
		
		return true;
	}

	override bool CanComplete()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		ObjectivePrint("m_ObjectiveItemsCount: " + m_ObjectiveItemsCount);
		ObjectivePrint("m_ObjectiveItemsAmount: " + m_ObjectiveItemsAmount);

	#ifdef EXPANSIONMODNAVIGATION
		bool markerConditionResult;
		if (m_ObjectiveItemsAmount != 0 && (m_ObjectiveItemsCount >= m_ObjectiveItemsAmount))
			markerConditionResult = true;
	#endif
		
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
		
		if (m_ObjectiveItemsAmount == 0 || m_ObjectiveItemsCount < m_ObjectiveItemsAmount || !m_DestinationReached)
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

			if (m_ObjectiveItemsAmount != 0 && m_ObjectiveItemsCount >= m_ObjectiveItemsAmount && m_Config.GetMarkerName() != string.Empty)
			{
				CreateMarkers();
				m_CreatedMarker = true;
			}
		}
	#endif

		return true;
	}

	override bool OnCancel()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		CheckQuestPlayersForObjectiveItems();
		DeleteRemainingObjectiveItems();
		
		if (!super.OnCancel())
			return false;
		
		return true;
	}

	protected void DeleteRemainingObjectiveItems()
	{
		for (int i = m_ObjectiveItems.Count() - 1; i >= 0; i--)
		{
			ExpansionQuestObjectiveItem objItem = m_ObjectiveItems[i];
			GetGame().ObjectDelete(objItem.GetItem());
			m_ObjectiveItems.RemoveOrdered(i);
		}
	}

	protected bool SpawnObjectiveDeliveryItems()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		
		PlayerBase player = m_Quest.GetPlayer();
		if (!player || !player.GetIdentity())
			return false;
		
		vector position = player.GetPosition();
		vector orientation = player.GetOrientation();
		
		array<ref ExpansionQuestObjectiveDelivery> objectiveDeliveries = m_Config.GetCollections();
		foreach (ExpansionQuestObjectiveDelivery delivery: objectiveDeliveries)
		{
			if (!SpawnDeliveryItems(delivery, player, player, position, orientation))
				return false;
		}
		
		return true;
	}

	protected bool SpawnDeliveryItems(ExpansionQuestObjectiveDelivery delivery, PlayerBase player, EntityAI parent, vector position, vector orientation)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (m_Quest.GetQuestConfig().GetID() == -1)
			return false;
		
	    int amountToSpawn = delivery.GetAmount();
		while (amountToSpawn > 0)
	    {
	        Object obj = ExpansionItemSpawnHelper.SpawnOnParent(delivery.GetClassName(), player, parent, amountToSpawn);
	        if (!obj)
	            break;

	        ItemBase questItem;
	        if (!Class.CastTo(questItem, obj))
	        {
	            GetGame().ObjectDelete(obj);
	            return false;
	        }

	        questItem.SetQuestID(m_Quest.GetQuestConfig().GetID());
			ExpansionQuestObjectiveItem objItem = new ExpansionQuestObjectiveItem(questItem);
			m_ObjectiveItems.Insert(objItem);
	    }

		return true;
	}
	
	override protected bool CheckQuestPlayersForObjectiveItems(bool continues = false)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
			
		if (!super.CheckQuestPlayersForObjectiveItems(continues))
			return false;
		
		if (continues && m_ObjectiveItemsCount < m_ObjectiveItemsAmount)
			return false;
		
		ObjectivePrint("End and return TRUE");
		
		return true;
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.DELIVERY;
	}
};