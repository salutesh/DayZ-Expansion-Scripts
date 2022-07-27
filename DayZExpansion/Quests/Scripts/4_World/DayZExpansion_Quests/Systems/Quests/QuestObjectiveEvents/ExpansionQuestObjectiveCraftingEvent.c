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

	override void OnStart()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnStart");
	#endif

		super.OnStart();
	}

	override void OnContinue()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnContinue");
	#endif

		super.OnContinue();
	}

	void OnItemsCrafted(PlayerBase player, array<ItemBase> spawned_objects)
	{
		QuestPrint(ToString() + "::OnItemsCrafted - Start");
		
		bool hasItem;
		foreach (ItemBase item: spawned_objects)
		{
			if (GetObjectiveConfig().GetItemNames().Find(item.ClassName()) == -1)
				continue;
			
			hasItem = true;
		}
		
		if (hasItem)
		{
			m_CraftingState = true;
			SetCompleted(true);
			OnComplete();
		}

		QuestPrint(ToString() + "::OnItemsCrafted - End");
	}

	bool GetCraftingState()
	{
		return m_CraftingState;
	}

	void SetCraftingState(bool state)
	{
		m_CraftingState = state;
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.CRAFTING;
	}

	void QuestPrint(string text)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(text);
	#endif
	}
};