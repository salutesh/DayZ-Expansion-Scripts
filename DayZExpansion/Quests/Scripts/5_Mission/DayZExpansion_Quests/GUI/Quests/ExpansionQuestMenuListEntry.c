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
	private ref ExpansionQuestMenuListEntryController m_QuestMenuListEntryController;
	private ExpansionQuestConfig m_Quest;
	private ExpansionQuestMenu m_QuestMenu;
	private ExpansionQuestModule m_QuestModule;

	private ButtonWidget Button;
	private TextWidget Text;
	private ImageWidget QuestIcon;

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
			QuestIcon.SetColor(GetQuestColor(m_Quest));
		}
		else if (questState == ExpansionQuestState.STARTED)
		{
			QuestIcon.SetImage(0);
			QuestIcon.SetColor(GetQuestColor(m_Quest));
		}
		else if (questState == ExpansionQuestState.CAN_TURNIN)
		{
			QuestIcon.SetImage(1);
			QuestIcon.SetColor(ARGB(255, 39, 174, 96));
		}
	}

	int GetQuestColor(ExpansionQuestConfig quest)
	{
		int color;
	#ifdef EXPANSIONMODHARDLINE
	#ifdef EXPANSIONMODAI
		if (quest.IsRepeatable() && !quest.IsGroupQuest() && !quest.IsBanditQuest() && !quest.IsHeroQuest() && quest.GetType() != ExpansionQuestType.AIPATROL && quest.GetType() != ExpansionQuestType.AICAMP)
	#else
		if (quest.IsRepeatable() && !quest.IsGroupQuest() && !quest.IsBanditQuest() && !quest.IsHeroQuest())
	#endif
	#else
		if (quest.IsRepeatable() && !quest.IsGroupQuest())
	#endif
		{
			color = ARGB(255, 52, 152, 219);
		}
		else if (quest.IsGroupQuest())
		{
			color = ARGB(255, 192, 57, 43);
		}
		else if (!quest.IsGroupQuest() && !quest.IsRepeatable())
		{
			color = ARGB(255, 241, 196, 15);
		}
	#ifdef EXPANSIONMODHARDLINE
		else if (quest.IsBanditQuest())
		{
			color = COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE;
		}
		else if (quest.IsHeroQuest())
		{
			color = COLOR_EXPANSION_NOTIFICATION_TURQUOISE;
		}
	#endif
	#ifdef EXPANSIONMODAI
		else if (quest.GetType() == ExpansionQuestType.AIPATROL || quest.GetType() == ExpansionQuestType.AICAMP)
		{
			color = ARGB(255, 142, 68, 173);
		}
	#endif

		return color;
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
		Text.SetColor(ARGB(255, 220, 220, 220));
	}

	void SetNormal()
	{
		Text.SetColor(ARGB(255, 0, 0, 0));
	}
};

class ExpansionQuestMenuListEntryController: ExpansionViewController
{
	string QuestIcon;
	string QuestTitle;
};