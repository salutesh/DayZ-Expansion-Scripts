/**
 * ExpansionQuestObjectiveCollectionEventBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveCollectionEventBase: ExpansionQuestObjectiveEventBase
{
	protected ExpansionTravelObjectiveSphereTrigger m_ObjectiveTrigger;
	protected bool m_DestinationReached;
	protected ref array<ref ExpansionQuestDeliveryObjectiveData> m_DeliveryData;
	protected ref map<string, int> m_ObjectiveItemsMap;
	protected ref array<ref ExpansionQuestObjectiveItem> m_ObjectiveItems;
	protected int m_ObjectiveItemsAmount;
	protected int m_ObjectiveItemsCount;
#ifdef EXPANSIONMODNAVIGATION
	protected bool m_CreatedMarker;
#endif

	void ExpansionQuestObjectiveCollectionEventBase(ExpansionQuest quest)
	{
		ObjectivePrint(ToString() + "::ExpansionQuestObjectiveCollectionEventBase - Start");

		m_ObjectiveItemsMap = new map<string, int>;
		m_DeliveryData = new array<ref ExpansionQuestDeliveryObjectiveData>;
		m_ObjectiveItems = new array<ref ExpansionQuestObjectiveItem>;

		ObjectivePrint(ToString() + "::ExpansionQuestObjectiveCollectionEventBase - End");
	}

	void ~ExpansionQuestObjectiveCollectionEventBase()
	{
		ObjectivePrint(ToString() + "::~ExpansionQuestObjectiveCollectionEventBase - Start");

		m_ObjectiveItemsMap.Clear();
		m_DeliveryData.Clear();
		m_ObjectiveItems.Clear();

		ObjectivePrint(ToString() + "::~ExpansionQuestObjectiveCollectionEventBase - End");
	}

	override bool OnCleanup()
	{
		ObjectivePrint(ToString() + "::OnCleanup - Start");

		if (!super.OnCleanup())
			return false;
		
		DeleteObjectiveTrigger();

		ObjectivePrint(ToString() + "::OnCleanup - End");

		return true;
	}

	protected void DeleteObjectiveTrigger()
	{
		ObjectivePrint(ToString() + ":: DeleteObjectiveTrigger - Start");

		if (!m_ObjectiveTrigger)
			return;

		GetGame().ObjectDelete(m_ObjectiveTrigger);

		ObjectivePrint(ToString() + ":: DeleteObjectiveTrigger - End");
	}

	void SetReachedLocation(bool state)
	{
		ObjectivePrint(ToString() + "::SetReachedLocation - Start");
		ObjectivePrint(ToString() + ":: SetReachedLocation - State: " + state);
		m_DestinationReached = state;
		m_Quest.QuestCompletionCheck();
		ObjectivePrint(ToString() + "::SetReachedLocation - End");
	}

	bool GetLocationState()
	{
		return m_DestinationReached;
	}

	protected int GetItemAmount(EntityAI item)
	{
		if (!item)
			Error(ToString() + "::GetItemAmount - Item is NULL!");

		int amount;
		ItemBase itemBase;
		if (Class.CastTo(itemBase, item))
		{
			amount = itemBase.Expansion_GetStackAmount();
		}
		else
		{
			amount = 1;
		}

		if (!MiscGameplayFunctions.Expansion_IsLooseEntity(item, true))
			amount = -amount;

		return amount;
	}

	array<ref ExpansionQuestDeliveryObjectiveData> GetDeliveryData()
	{
		return m_DeliveryData;
	}

	protected bool IsObjectiveItem(EntityAI item, out int index = -1)
	{
		for (int i = 0; i < m_ObjectiveItems.Count(); i++)
		{
			ExpansionQuestObjectiveItem objItem = m_ObjectiveItems[i];
			if (objItem.GetItem() == item)
			{
				ItemBase itemIB;
				if (Class.CastTo(itemIB, item) && !itemIB.IsQuestItem())
				{
					index = i;
					return true;
				}
			}
		}

		return false;
	}

	protected bool HasAnyCollectionCompleted()
	{
		foreach (string typeName, int needed: m_ObjectiveItemsMap)
		{
			int collectionItemCount = 0;
			foreach (ExpansionQuestObjectiveItem objItem: m_ObjectiveItems)
			{
				if (objItem.GetClassName() == typeName)
				{
					int amount = GetItemAmount(objItem.GetItem());
					collectionItemCount += amount;
				}
			}

			if (collectionItemCount >= needed)
				return true;
		}

		return false;
	}
};