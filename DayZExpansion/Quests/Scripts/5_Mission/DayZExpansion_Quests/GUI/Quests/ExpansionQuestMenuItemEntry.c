/**
 * ExpansionQuestMenuItemEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestMenuItemEntry: ExpansionScriptView
{
	private ref ExpansionQuestMenuItemEntryController m_QuestMenuItemEntryController;
	private string m_ClassName;
	private int m_Amount;
	private ref ExpansionItemTooltip m_ItemTooltip;

	private ButtonWidget reward_item_button;
	EntityAI m_Object;

	void ExpansionQuestMenuItemEntry(string className, int amount)
	{
		m_ClassName = className;
		m_Amount = amount;
		Class.CastTo(m_QuestMenuItemEntryController, GetController());

		SetEntry();
	}

	void ~ExpansionQuestMenuItemEntry()
	{
		if (m_Object)
			GetGame().ObjectDelete(m_Object);
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Quests/GUI/layouts/quests/expansion_quest_menu_reward_entry.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionQuestMenuItemEntryController;
	}

	void SetEntry()
	{
		if (m_ClassName == string.Empty)
			return;

		m_QuestMenuItemEntryController.ItemName = ExpansionStatic.GetItemDisplayNameWithType(m_ClassName);
		m_QuestMenuItemEntryController.NotifyPropertyChanged("ItemName");

		m_QuestMenuItemEntryController.ItemAmount = "x" + m_Amount;
		m_QuestMenuItemEntryController.NotifyPropertyChanged("ItemAmount");

		if (!m_Object)
			m_Object = EntityAI.Cast(GetGame().CreateObjectEx(m_ClassName, vector.Zero, ECE_LOCAL|ECE_NOLIFETIME));

		m_QuestMenuItemEntryController.Preview = m_Object;
		m_QuestMenuItemEntryController.NotifyPropertyChanged("Preview");
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
			case reward_item_button:
			{
				if (!m_ItemTooltip && m_Object)
				{
					m_ItemTooltip = new ExpansionItemTooltip(m_Object);
					m_ItemTooltip.Show();
				}
				break;
			}
		}

		return super.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
			case reward_item_button:
			{
				if (m_ItemTooltip) delete m_ItemTooltip;
				break;
			}
		}

		return super.OnMouseLeave(w, enterW, x, y);
	}
};

class ExpansionQuestMenuItemEntryController: ExpansionViewController
{
	string ItemName;
	string ItemAmount;
	Object Preview;
};