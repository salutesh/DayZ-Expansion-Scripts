/**
 * ExpansionQuestObjectiveCraftingEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveCraftingEvent: ExpansionQuestObjectiveEventBase
{
	private bool m_CraftingState;
	private bool m_CallLater;

	void OnItemsCrafted(PlayerBase player, array<ItemBase> spawned_objects)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnItemsCrafted");
	#endif
		
		bool hasItem;
		foreach (ItemBase item: spawned_objects)
		{
			if (ExpansionStatic.IsAnyOf(item, GetObjectiveConfig().GetItemNames(), true))
			{
				hasItem = true;
				break;
			}
		}
		
		if (hasItem)
		{
			m_CraftingState = true;
			SetCompleted(true);
			OnComplete();
		}
	}

	bool GetCraftingState()
	{
		return m_CraftingState;
	}

	void SetCraftingState(bool state)
	{
		m_CraftingState = state;
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.CRAFTING;
	}
};