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

	#ifdef EXPANSIONMODNAVIGATION
		if (m_Config.GetMarkerName() != string.Empty)
			CreateMarkers();
	#endif
		
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

	#ifdef EXPANSIONMODNAVIGATION
		if (m_Config.GetMarkerName() != string.Empty)
			CreateMarkers();
	#endif
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ObjectiveCheck, 500);
		
		return true;
	}

	override bool CanComplete()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		ObjectivePrint("m_ObjectiveItemsCount: " + m_ObjectiveItemsCount);
		ObjectivePrint("m_ObjectiveItemsAmount: " + m_ObjectiveItemsAmount);
		ObjectivePrint("m_DestinationReached: " + m_DestinationReached);

		bool conditionsResult;
	#ifdef EXPANSIONMODNAVIGATION
		bool markerConditionResult;
	#endif

		if (m_ObjectiveItemsAmount != 0 && m_ObjectiveItemsCount >= m_ObjectiveItemsAmount && m_DestinationReached)
			conditionsResult = true;

	#ifdef EXPANSIONMODNAVIGATION
		if (m_ObjectiveItemsAmount != 0 && m_ObjectiveItemsCount >= m_ObjectiveItemsAmount)
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

		if (!super.OnCancel())
			return false;
		
		CheckQuestPlayersForObjectiveItems();
		DeleteRemainingObjectiveItems();
		
		return true;
	}

	protected void DeleteRemainingObjectiveItems()
	{
		for (int i = m_ObjectiveItems.Count() - 1; i >= 0; i--)
		{
			ExpansionQuestObjectiveItem objItem = m_ObjectiveItems[i];
			objItem.GetItem().Expansion_SetQuestID(-1);
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
		
		int quantityPercent = delivery.GetQuantity();
	    int amountToSpawn = delivery.GetAmount();
		while (amountToSpawn > 0)
	    {
	        Object obj = ExpansionItemSpawnHelper.SpawnOnParent(delivery.GetClassName(), player, parent, amountToSpawn, quantityPercent, null, -1, false);
	        if (!obj)
	            break;

	        ItemBase questItem;
	        if (!Class.CastTo(questItem, obj))
	        {
	            GetGame().ObjectDelete(obj);
	            return false;
	        }
			
			Magazine mag;
			if (Class.CastTo(mag, questItem))
			{
				if (!mag.IsAmmoPile())
					mag.ServerSetAmmoCount(0);
			}

	        questItem.Expansion_SetQuestID(m_Quest.GetQuestConfig().GetID());
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