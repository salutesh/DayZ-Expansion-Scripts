/**
 * ExpansionBookMenuTabQuestsListEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODBOOK
class ExpansionBookMenuTabQuestsListEntry: ExpansionScriptView
{
	protected ref ExpansionBookMenuTabQuestsListEntryController m_QuestMenuListEntryController;
	protected ref ExpansionQuestConfig m_Quest;
	protected ExpansionBookMenuTabQuests m_QuestMenu;

	protected ButtonWidget Button;
	protected TextWidget Text;
	protected ImageWidget QuestIcon;
	protected ButtonWidget HideButton;
	protected ImageWidget HideIcon;

	void ExpansionBookMenuTabQuestsListEntry(ExpansionQuestConfig quest, ExpansionBookMenuTabQuests menu)
	{
		m_Quest = quest;
		m_QuestMenu = menu;

		Class.CastTo(m_QuestMenuListEntryController, GetController());

		SetEntry();
	}

	void SetEntry()
	{
		if (!m_Quest)
			return;

		MissionGameplay mission;
		if (!Class.CastTo(mission, GetGame().GetMission()))
			return;

		m_QuestMenuListEntryController.QuestTitle = m_Quest.GetTitle();
		m_QuestMenuListEntryController.NotifyPropertyChanged("QuestTitle");

		int questState = ExpansionQuestModule.GetModuleInstance().GetClientQuestData().GetQuestStateByQuestID(m_Quest.GetID());
		if (questState == ExpansionQuestState.COMPLETED || ExpansionQuestModule.GetModuleInstance().GetClientQuestData().GetQuestCompletionCountByQuestID(m_Quest.GetID()) > 0)
		{
			m_QuestMenuListEntryController.QuestIcon = ExpansionIcons.GetPath("Exclamationmark");
			QuestIcon.SetColor(ExpansionQuestModule.GetQuestColor(m_Quest));
		}
		else
		{
			if (questState == ExpansionQuestState.NONE)
			{
				m_QuestMenuListEntryController.QuestIcon = ExpansionIcons.GetPath("Questionmark 2");
				QuestIcon.SetColor(ExpansionQuestModule.GetQuestColor(m_Quest));
			}
			else if (questState == ExpansionQuestState.STARTED)
			{
				m_QuestMenuListEntryController.QuestIcon = ExpansionIcons.GetPath("Exclamationmark");
				QuestIcon.SetColor(ExpansionQuestModule.GetQuestColor(m_Quest));
			}
			else if (questState == ExpansionQuestState.CAN_TURNIN)
			{
				m_QuestMenuListEntryController.QuestIcon = ExpansionIcons.GetPath("Exclamationmark");
				QuestIcon.SetColor(ARGB(255, 160, 223, 59));
			}
			else if (questState == ExpansionQuestState.COMPLETED)
			{
				m_QuestMenuListEntryController.QuestIcon = ExpansionIcons.GetPath("Exclamationmark");
				QuestIcon.SetColor(ExpansionQuestModule.GetQuestColor(m_Quest));
			}
		}

		m_QuestMenuListEntryController.NotifyPropertyChanged("QuestIcon");

	#ifdef EXPANSIONMODQUESTS_HUD_ENABLE
		HideIcon.Show(true);
		HideButton.Show(true);

		ExpansionQuestHUDEntry questEntry;
		int findIndex = -1;

		ExpansionQuestHUD questHUD = mission.GetQuestHUD();
		if (!questHUD)
			return;

		if (!questHUD.IsEntryHidden(m_Quest.GetID(), questEntry, findIndex))
		{
			m_QuestMenuListEntryController.HideIcon = ExpansionIcons.GetPath("Eye");
		}
		else
		{
			m_QuestMenuListEntryController.HideIcon = ExpansionIcons.GetPath("Cross");
		}

		m_QuestMenuListEntryController.NotifyPropertyChanged("HideIcon");
	#endif
	}

	void OnEntryClick()
	{
		if (m_Quest)
			m_QuestMenu.SetQuest(m_Quest);
	}

#ifdef EXPANSIONMODQUESTS_HUD_ENABLE
	void OnHideButtonClick()
	{
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

		if (!questHUD.IsEntryHidden(m_Quest.GetID(), questEntry, findIndex)) m_QuestMenuListEntryController.HideIcon = ExpansionIcons.GetPath("Eye");
		else m_QuestMenuListEntryController.HideIcon = ExpansionIcons.GetPath("Cross");

		m_QuestMenuListEntryController.NotifyPropertyChanged("HideIcon");
	}
#endif

	override string GetLayoutFile()
	{
		return "DayZExpansion/Quests/GUI/layouts/tabs/quests/expansion_book_tab_quests_list_entry.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionBookMenuTabQuestsListEntryController;
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w == Button)
		{
			Text.SetColor(ARGB(255, 220, 220, 220));
		}
	#ifdef EXPANSIONMODQUESTS_HUD_ENABLE
		else if (w == HideButton)
		{
			HideIcon.SetColor(ARGB(255, 220, 220, 220));
		}
	#endif

		return super.OnMouseEnter(w, x, y);;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w == Button)
		{
			Text.SetColor(ARGB(255, 0, 0, 0));
		}
	#ifdef EXPANSIONMODQUESTS_HUD_ENABLE
		else if (w == HideButton)
		{
			HideIcon.SetColor(ARGB(255, 0, 0, 0));
		}
	#endif

		return super.OnMouseLeave(w, enterW, x, y);
	}
};

class ExpansionBookMenuTabQuestsListEntryController: ExpansionViewController
{
	string QuestTitle;
	string HideIcon;
	string QuestIcon;
};
#endif

