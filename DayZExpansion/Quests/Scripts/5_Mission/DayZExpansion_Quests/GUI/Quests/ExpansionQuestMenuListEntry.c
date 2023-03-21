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

class ExpansionQuestMenuLogEntry: ExpansionScriptView
{
	protected ref ExpansionQuestMenuLogEntryController m_QuestMenuLogEntryController;
	protected ref ExpansionQuestConfig m_Quest;
	protected ExpansionQuestMenu m_QuestMenu;
	protected ExpansionQuestModule m_QuestModule;

	protected Widget Background;
	protected ButtonWidget Button;
	protected ButtonWidget HideButton;
	protected ImageWidget HideIcon;
	protected TextWidget Text;
	protected ImageWidget QuestIcon;
	protected ImageWidget CooldownIcon;
	protected CheckBoxWidget HideCheckbox;

	void ExpansionQuestMenuLogEntry(ExpansionQuestConfig quest, ExpansionQuestMenu menu)
	{
		m_Quest = quest;
		m_QuestMenu = menu;

		Class.CastTo(m_QuestMenuLogEntryController, GetController());
		Class.CastTo(m_QuestModule, CF_ModuleCoreManager.Get(ExpansionQuestModule));

		SetEntry();
	}
	
	override string GetLayoutFile()
	{
		return "DayZExpansion/Quests/GUI/layouts/quests/expansion_quest_menu_log_entry.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionQuestMenuLogEntryController;
	}
	
	void SetEntry()
	{
		if (!m_Quest || !m_QuestModule || !m_QuestModule.GetClientQuestData())
			return;

		QuestIcon.LoadImageFile(0, ExpansionIcons.GetPath("Exclamationmark"));
		QuestIcon.LoadImageFile(1, ExpansionIcons.GetPath("Questionmark 2"));

		m_QuestMenuLogEntryController.QuestTitle = m_Quest.GetTitle();
		m_QuestMenuLogEntryController.NotifyPropertyChanged("QuestTitle");

		ExpansionQuestState questState = m_QuestModule.GetClientQuestData().GetQuestStateByQuestID(m_Quest.GetID());
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
		
		MissionGameplay mission;
		if (!Class.CastTo(mission, GetGame().GetMission()))
			return;
		
		ExpansionQuestHUD questHUD = mission.GetQuestHUD();
		if (!questHUD)
			return;
		
		ExpansionQuestHUDEntry questEntry;
		int findIndex = -1;
		
		if (!questHUD.IsEntryHidden(m_Quest.GetID(), questEntry, findIndex))
		{
			//HideCheckbox.SetChecked(true);
			m_QuestMenuLogEntryController.HideIcon = ExpansionIcons.GetPath("Eye");
		}
		else
		{ 
			//HideCheckbox.SetChecked(false);
			m_QuestMenuLogEntryController.HideIcon = ExpansionIcons.GetPath("Cross");
		}
		m_QuestMenuLogEntryController.NotifyPropertyChanged("HideIcon");
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
			{
				Text.SetColor(ARGB(255, 0, 0, 0));
				Background.SetColor(ARGB(200, 220, 220, 220));
				return true;
			}
			case HideButton:
			{
				HideIcon.SetColor(ARGB(255, 220, 220, 220));
				return true;
			}
		}

		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
			case Button:
			{
				Text.SetColor(ARGB(255, 220, 220, 220));
				Background.SetColor(ARGB(200, 0, 0, 0));
				return true;
			}
			case HideButton:
			{
				HideIcon.SetColor(ARGB(255, 220, 220, 220));
				return true;
			}
		}

		return false;
	}
	
	void OnHideEntryClick()
	{
		Print(ToString() + "::OnHideEntryClick - Start");

		if (!m_Quest)
			return;

		MissionGameplay mission;
		if (!Class.CastTo(mission, GetGame().GetMission()))
			return;

		ExpansionQuestHUD questHUD = mission.GetQuestHUD();
		if (!questHUD)
			return;

		questHUD.ToggleQuestEntryVisibilityByID(m_Quest.GetID());

		ExpansionQuestHUDEntry questEntry;
		int findIndex = -1;

		if (!questHUD.IsEntryHidden(m_Quest.GetID(), questEntry, findIndex))
		{
			//HideCheckbox.SetChecked(true);
			m_QuestMenuLogEntryController.HideIcon = ExpansionIcons.GetPath("Eye");
		}
		else
		{ 
			//HideCheckbox.SetChecked(false);
			m_QuestMenuLogEntryController.HideIcon = ExpansionIcons.GetPath("Cross");
		}
		m_QuestMenuLogEntryController.NotifyPropertyChanged("HideIcon");
		
		Print(ToString() + "::OnHideEntryClick - End");
	}
};
class ExpansionQuestMenuLogEntryController: ExpansionViewController
{
	string QuestIcon;
	string QuestTitle;
	string HideIcon;
};

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

		ExpansionQuestState questState = m_QuestModule.GetClientQuestData().GetQuestStateByQuestID(m_Quest.GetID());
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
			{
				Text.SetColor(ARGB(255, 0, 0, 0));
				Background.SetColor(ARGB(200, 220, 220, 220));
				return true;
			}
		}

		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
			case Button:
			{
				Text.SetColor(ARGB(255, 220, 220, 220));
				Background.SetColor(ARGB(200, 0, 0, 0));
				return true;
			}
		}

		return false;
	}
};

class ExpansionQuestMenuListEntryController: ExpansionViewController
{
	string QuestIcon;
	string QuestTitle;
};