/**
 * ExpansionQuestObjectiveActionEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef NAMALSK_SURVIVAL
modded class ExpansionQuestObjectiveActionEvent
{
	// -----------------------------------------------------------
	// ExpansionQuestObjectiveActionEvent OnActionFlaxControlPanel
	// -----------------------------------------------------------
	protected void OnActionFlaxControlPanel(bool isInit = false)
	{
		ObjectivePrint(ToString() + "::OnActionFlaxControlPanel - Start");

		if (!m_ActionEventData)
			return;

		m_ActionEventData.SetTargetActionTarget();

		land_a3_flaxbt_panel flaxbt_panel;
		if (!Class.CastTo(flaxbt_panel, m_ActionEventData.m_Target))
			return;

		if (!HasControlPanelItems(flaxbt_panel))
			return;

		OnActionExecutionValid();
		ClearActionData();

		ObjectivePrint(ToString() + "::OnActionFlaxControlPanel - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveActionEvent HasControlPanelItems
	// -----------------------------------------------------------
	protected bool HasControlPanelItems(land_a3_flaxbt_panel flaxbt_panel)
	{
		array<EntityAI> items = new array<EntityAI>;
		int count;
		flaxbt_panel.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		foreach (EntityAI item: items)
		{
			if (!item.IsRuined() && (item.GetType() == "dzn_module_lantia" || item.GetType() == "dzn_module_card"))
				count++;

			if (count == 2)
				break;
		}

		return (count == 2);
	}
};
#endif