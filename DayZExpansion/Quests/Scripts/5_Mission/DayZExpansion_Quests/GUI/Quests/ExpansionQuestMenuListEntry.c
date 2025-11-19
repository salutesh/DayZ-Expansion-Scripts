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
	protected ref ExpansionQuestMenu m_QuestMenu;
	protected ExpansionQuestModule m_QuestModule;

	protected Widget Background;
	protected ButtonWidget Button;
	protected ButtonWidget HideButton;
	protected ImageWidget HideIcon;
	protected TextWidget Text;
	protected ImageWidget QuestIcon;

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

		m_QuestMenuLogEntryController.QuestTitle = m_Quest.GetTitle();
		m_QuestMenuLogEntryController.NotifyPropertyChanged("QuestTitle");

		int questState = m_QuestModule.GetClientQuestData().GetQuestStateByQuestID(m_Quest.GetID());
		if (questState == ExpansionQuestState.NONE)
		{
			m_QuestMenuLogEntryController.QuestIconPath = ExpansionIcons.GetPath("Exclamationmark");
			QuestIcon.SetColor(ExpansionQuestModule.GetQuestColor(m_Quest));
		}
		else if (questState == ExpansionQuestState.STARTED)
		{
			m_QuestMenuLogEntryController.QuestIconPath = ExpansionIcons.GetPath("Questionmark 2");
			QuestIcon.SetColor(ExpansionQuestModule.GetQuestColor(m_Quest));
		}
		else if (questState == ExpansionQuestState.CAN_TURNIN)
		{
			m_QuestMenuLogEntryController.QuestIconPath = ExpansionIcons.GetPath("Questionmark 2");
			QuestIcon.SetColor(ARGB(255, 160, 223, 59));
		}

		m_QuestMenuLogEntryController.NotifyPropertyChanged("QuestIconPath");

		MissionGameplay mission;
		if (!Class.CastTo(mission, g_Game.GetMission()))
			return;

		ExpansionQuestHUD questHUD = mission.GetQuestHUD();
		if (!questHUD)
			return;

		if (!questHUD.IsEntryHidden(m_Quest.GetID()))
		{
			m_QuestMenuLogEntryController.HideIcon = "set:dayz_gui image:PasswordEyeOpen";
		}
		else
		{
			m_QuestMenuLogEntryController.HideIcon = "set:dayz_gui image:PasswordEyeClose";
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
				HideIcon.SetColor(ARGB(255, 255, 255, 255));
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
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		if (!m_Quest)
			return;

		MissionGameplay mission;
		if (!Class.CastTo(mission, g_Game.GetMission()))
			return;

		ExpansionQuestHUD questHUD = mission.GetQuestHUD();
		if (!questHUD)
			return;

		questHUD.ToggleQuestEntryVisibilityByID(m_Quest.GetID());

		if (!questHUD.IsEntryHidden(m_Quest.GetID()))
		{
			m_QuestMenuLogEntryController.HideIcon = "set:dayz_gui image:PasswordEyeOpen";
		}
		else
		{
			m_QuestMenuLogEntryController.HideIcon = "set:dayz_gui image:PasswordEyeClose";
		}
		m_QuestMenuLogEntryController.NotifyPropertyChanged("HideIcon");
	}
};
class ExpansionQuestMenuLogEntryController: ExpansionViewController
{
	string QuestIconPath;
	string QuestTitle;
	string HideIcon;
};

class ExpansionQuestMenuListEntry: ExpansionScriptView
{
	protected ref ExpansionQuestMenuListEntryController m_QuestMenuListEntryController;
	protected ref ExpansionQuestConfig m_Quest;
	protected ref ExpansionQuestMenu m_QuestMenu;
	protected ExpansionQuestModule m_QuestModule;

	protected Widget Background;
	protected ButtonWidget Button;
	protected TextWidget Text;
	protected ImageWidget QuestIcon;
	protected Widget CooldownPanel;
	protected ImageWidget CooldownIcon;
	protected TextWidget CooldownText;
	
	protected bool m_HasCooldown;
	protected int m_ServerTime;

	void ExpansionQuestMenuListEntry(ExpansionQuestConfig quest, ExpansionQuestMenu menu, int serverTime = 0)
	{
		m_Quest = quest;
		m_QuestMenu = menu;
		m_ServerTime = serverTime;

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
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		if (!m_Quest || !m_QuestModule || !m_QuestModule.GetClientQuestData())
			return;

		m_QuestMenuListEntryController.QuestTitle = m_Quest.GetTitle();
		m_QuestMenuListEntryController.NotifyPropertyChanged("QuestTitle");

		int questState = m_QuestModule.GetClientQuestData().GetQuestStateByQuestID(m_Quest.GetID());
		if (questState == ExpansionQuestState.NONE)
		{
			m_QuestMenuListEntryController.QuestIconPath = ExpansionIcons.GetPath("Exclamationmark");
			QuestIcon.SetColor(ExpansionQuestModule.GetQuestColor(m_Quest));
		}
		else if (questState == ExpansionQuestState.STARTED)
		{
			m_QuestMenuListEntryController.QuestIconPath = ExpansionIcons.GetPath("Questionmark 2");
			QuestIcon.SetColor(ExpansionQuestModule.GetQuestColor(m_Quest));
		}
		else if (questState == ExpansionQuestState.CAN_TURNIN)
		{
			m_QuestMenuListEntryController.QuestIconPath = ExpansionIcons.GetPath("Questionmark 2");
			QuestIcon.SetColor(ARGB(255, 160, 223, 59));
		}
		
		m_QuestMenuListEntryController.NotifyPropertyChanged("QuestIconPath");
		
		SetCooldown(m_ServerTime);
	}
	
	protected void SetCooldown(int serverTime)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif
		m_HasCooldown = false;
		
		int timedif;
		if (m_Quest.IsDailyQuest() || m_Quest.IsWeeklyQuest())
		{
			int timestamp = m_QuestModule.GetClientQuestData().GetQuestTimestampByQuestID(m_Quest.GetID());
			if (timestamp > serverTime)
			{
				timedif = timestamp - serverTime;
				m_HasCooldown = true;
			}
		}
		
		if (m_HasCooldown)
		{
			CooldownPanel.Show(true);
			m_QuestMenuListEntryController.Cooldown = ExpansionStatic.GetTimeString(timedif);
			m_QuestMenuListEntryController.NotifyPropertyChanged("Cooldown");
		}
		else
		{
			CooldownPanel.Show(false);
		}
	}

	void OnEntryClick()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		if (!m_Quest)
			return;
		
		if (!m_HasCooldown)
		{
			m_QuestMenu.SetQuest(m_Quest);
		}
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
			case Button:
			{
				Text.SetColor(ARGB(255, 0, 0, 0));
				Background.SetColor(ARGB(200, 220, 220, 220));
				if (m_HasCooldown)
				{
					CooldownIcon.SetColor(ARGB(255, 0, 0, 0));
					CooldownText.SetColor(ARGB(255, 0, 0, 0));
				}
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
				if (m_HasCooldown)
				{
					CooldownIcon.SetColor(ARGB(255, 226, 65, 66));
					CooldownText.SetColor(ARGB(255, 220, 220, 220));
				}
				return true;
			}
		}

		return false;
	}

	override float GetUpdateTickRate()
	{
		return 10.0;
	}
	
	override void Expansion_Update()
	{
		if (!IsVisible() || !m_HasCooldown)
			return;

		m_ServerTime = m_ServerTime + 10;
		SetCooldown(m_ServerTime);
	}
};

class ExpansionQuestMenuListEntryController: ExpansionViewController
{
	string QuestIconPath;
	string QuestTitle;
	string Cooldown;
};