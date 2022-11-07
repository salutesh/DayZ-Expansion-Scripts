/**
 * ExpansionQuestMenuListEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestMenuListEntry: ExpansionScriptView
{
	protected ref ExpansionQuestMenuListEntryController m_QuestMenuListEntryController;
	protected ExpansionQuestConfig m_Quest;
	protected ExpansionQuestMenu m_QuestMenu;
	protected ExpansionQuestModule m_QuestModule;

	protected Widget Background;
	protected ButtonWidget Button;
	protected TextWidget Text;
	protected ImageWidget QuestIcon;
	protected ImageWidget CooldownIcon;

	void ExpansionQuestMenuListEntry(ExpansionQuestConfig quest, ExpansionQuestMenu menu)
	{
		m_Quest = quest;
		m_QuestMenu = menu;

		Class.CastTo(m_QuestMenuListEntryController, GetController());
		Class.CastTo(m_QuestModule, CF_ModuleCoreManager.Get(ExpansionQuestModule));

		SetEntry();
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Quests/GUI/layouts/quests/expansion_quest_menu_list_entry.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionQuestMenuListEntryController;
	}

	void SetEntry()
	{
		if (!m_Quest || !m_QuestModule || !m_QuestModule.GetClientQuestData())
			return;

		QuestIcon.LoadImageFile(0, ExpansionIcons.GetPath("Exclamationmark"));
		QuestIcon.LoadImageFile(1, ExpansionIcons.GetPath("Questionmark 2"));

		m_QuestMenuListEntryController.QuestTitle = m_Quest.GetTitle();
		m_QuestMenuListEntryController.NotifyPropertyChanged("QuestTitle");

		int questState = m_QuestModule.GetClientQuestData().GetQuestStateByQuestID(m_Quest.GetID());
		if (questState == ExpansionQuestState.NONE)
		{
			QuestIcon.SetImage(1);
			QuestIcon.SetColor(ExpansionQuestModule.GetQuestColor(m_Quest));
		}
		else if (questState == ExpansionQuestState.STARTED)
		{
			QuestIcon.SetImage(0);
			QuestIcon.SetColor(ExpansionQuestModule.GetQuestColor(m_Quest));
		}
		else if (questState == ExpansionQuestState.CAN_TURNIN)
		{
			QuestIcon.SetImage(0);
			QuestIcon.SetColor(ARGB(200, 160, 223, 59));
		}
		else if (questState == ExpansionQuestState.COMPLETED)
		{
			QuestIcon.SetImage(0);
			QuestIcon.SetColor(ExpansionQuestModule.GetQuestColor(m_Quest));
		}

		int timestamp;
		if (m_QuestModule.GetClientQuestData().HasCooldownOnQuest(m_Quest.GetID(), timestamp))
		{
			CooldownIcon.Show(true);
		}
		else
		{
			CooldownIcon.Show(false);
		}
	}

	void OnEntryClick()
	{
		if (!m_Quest)
			return;

		m_QuestMenu.SetQuest(m_Quest);
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
		case Button:
			SetHighlight();
			break;
		}

		return super.OnMouseEnter(w, x, y);;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
		case Button:
			SetNormal();
			break;
		}

		return super.OnMouseLeave(w, enterW, x, y);
	}

	void SetHighlight()
	{
		Text.SetColor(ARGB(255, 0, 0, 0));
		Background.SetColor(ARGB(200, 220, 220, 220));
	}

	void SetNormal()
	{
		Text.SetColor(ARGB(255, 220, 220, 220));
		Background.SetColor(ARGB(200, 0, 0, 0));
	}
};

class ExpansionQuestMenuListEntryController: ExpansionViewController
{
	string QuestIcon;
	string QuestTitle;
};