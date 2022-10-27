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
	protected ref ExpansionQuestMenuItemEntryController m_QuestMenuItemEntryController;
	protected string m_ClassName;
	protected int m_Amount;
	protected array<string> m_Attachments;
	protected ref ExpansionItemTooltip m_ItemTooltip;
	protected bool m_IsRewardEntry = false;
	protected ExpansionQuestMenu m_QuestMenu;
	protected ExpansionQuestRewardConfig m_QuestRewardConfig;

	protected ButtonWidget reward_item_button;
	EntityAI m_Object;

	void ExpansionQuestMenuItemEntry(string className, int amount, TStringArray attachments = NULL)
	{
		m_ClassName = className;
		m_Amount = amount;
		m_Attachments = attachments;

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

		if (m_Attachments && m_Attachments.Count() > 0)
			SpawnAttachments(m_Attachments, m_Object);

		m_QuestMenuItemEntryController.Preview = m_Object;
		m_QuestMenuItemEntryController.NotifyPropertyChanged("Preview");
	}

	void SpawnAttachments(array<string> attachments, EntityAI parent)
	{
		foreach (string attachmentName: attachments)
		{
			ExpansionItemSpawnHelper.SpawnAttachment(attachmentName, parent);
		}
	}

	void OnItemButtonClick()
	{
		if (!m_IsRewardEntry || !m_QuestMenu || !m_QuestRewardConfig)
			return;

		if (!m_QuestMenu.GetSelectedQuest())
			return;

		if (!m_QuestMenu.GetSelectedQuest().NeedToSelectReward())
			return;

		m_QuestMenu.ResetRewardElements();
		reward_item_button.SetColor(ARGB(140, 226, 65, 66));
		m_QuestMenu.SetSelectedReward(m_QuestRewardConfig);
	}

	void Reset()
	{
		reward_item_button.SetColor(ARGB(0, 0, 0, 0));
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
					m_ItemTooltip.SetContentOffset(-600.0, 0.0);
					m_ItemTooltip.Show();
				}
				break;
			}
		}


		return super.OnMouseEnter(w, x, y);
	}

	void SetQuestMenu(ExpansionQuestMenu questMenu)
	{
		m_QuestMenu = questMenu;
	}

	void SetIsRewardEntry(bool state)
	{
		m_IsRewardEntry = state;
	}

	bool IsRewardEntry()
	{
		return m_IsRewardEntry;
	}

	void SetQuestRewardConfig(ExpansionQuestRewardConfig config)
	{
		m_QuestRewardConfig = config;
	}

	ButtonWidget GetItemButton()
	{
		return reward_item_button;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
			case reward_item_button:
			{
				if (m_ItemTooltip) m_ItemTooltip.Destroy();
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