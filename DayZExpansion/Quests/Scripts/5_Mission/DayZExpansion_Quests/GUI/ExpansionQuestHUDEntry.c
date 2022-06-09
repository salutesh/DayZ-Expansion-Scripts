/**
 * ExpansionQuestHUDEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODQUESTS_HUD_ENABLE
class ExpansionQuestHUDEntry: ExpansionScriptView
{
	protected ref ExpansionQuestHUDEntryController m_QuestHUDEntryController;
	protected ExpansionQuestConfig m_QuestConfig;
	protected ExpansionQuestPlayerData  m_PlayerQuestData;
	protected ImageWidget QuestIcon;
	protected Widget Spacer;

	void ExpansionQuestHUDEntry(ExpansionQuestConfig questConfig, ExpansionQuestPlayerData playerData)
	{
		m_QuestConfig = questConfig;
		m_PlayerQuestData = playerData;
		m_QuestHUDEntryController = ExpansionQuestHUDEntryController.Cast(GetController());
	}
	
	void ~ExpansionQuestHUDEntry()
	{
		m_QuestHUDEntryController.ObjectiveEntries.Clear();
	}

	void SetEntry()
	{
		QuestPrint(ToString() + "::SetEntryQuest - Start");
			
		m_QuestHUDEntryController.QuestName = m_QuestConfig.GetTitle();
		m_QuestHUDEntryController.NotifyPropertyChanged("QuestName");
		
		int state = m_PlayerQuestData.GetQuestStateByQuestID(m_QuestConfig.GetID());
		if (state < ExpansionQuestState.CAN_TURNIN)
		{
			m_QuestHUDEntryController.ObjectiveText = m_QuestConfig.GetObjectiveText();
		}
		else if (state == ExpansionQuestState.CAN_TURNIN)
		{
			m_QuestHUDEntryController.ObjectiveText = "COMPLETED -> TURN-IN";
		}
		
		m_QuestHUDEntryController.NotifyPropertyChanged("ObjectiveText");

		Spacer.SetColor(GetQuestColor(m_QuestConfig));
		QuestIcon.SetColor(GetQuestColor(m_QuestConfig));

		for (int i = 0; i < m_PlayerQuestData.GetQuestObjectives().Count(); i++)
		{
			ExpansionQuestObjectivePlayerData objective = m_PlayerQuestData.GetQuestObjectives()[i];
			if (!objective)
				continue;

			if (objective.GetQuestID() == m_QuestConfig.GetID())			
				AddObjective(objective, m_QuestConfig);
		}
		
		QuestPrint(ToString() + "::SetEntryQuest - End");
	}

	void AddObjective(ExpansionQuestObjectivePlayerData objective, ExpansionQuestConfig questConfig)
	{
		if (!m_QuestHUDEntryController.ObjectiveEntries)
			m_QuestHUDEntryController.ReInit();
		
		QuestPrint(ToString() + "::AddObjective - Start");
		ExpansionQuestHUDObjective objectiveEntry = new ExpansionQuestHUDObjective(objective, questConfig);	
		m_QuestHUDEntryController.ObjectiveEntries.Insert(objectiveEntry);

		QuestPrint(ToString() + "::AddObjective - End");
	}
	
	private int GetQuestColor(ExpansionQuestConfig quest)
	{
		int color;
	#ifdef EXPANSIONMODHARDLINE
		if (quest.IsRepeatable() && !quest.IsGroupQuest() && !quest.IsBanditQuest() && !quest.IsHeroQuest())
	#else
		if (quest.IsRepeatable() && !quest.IsGroupQuest())
	#endif
		{
			color = ARGB(120, 52, 152, 219);
		}
		else if (quest.IsGroupQuest())
		{
			color = ARGB(120, 226, 65, 66);
		}
		else if (!quest.IsGroupQuest() && !quest.IsRepeatable())
		{
			color = ARGB(120, 241, 196, 15);
		}
	#ifdef EXPANSIONMODHARDLINE
		else if (quest.IsBanditQuest())
		{
			color = ARGB(120, 225, 112, 85);
		}
		else if (quest.IsHeroQuest())
		{
			color = ARGB(120, 26, 188, 156);
		}
	#endif
	#ifdef EXPANSIONMODAI
		else if (quest.GetType() == ExpansionQuestType.AIPATROL || quest.GetType() == ExpansionQuestType.AICAMP)
		{
			color = ARGB(120, 75, 101, 132);
		}
	#endif

		return color;
	}

	int GetEntryQuestID()
	{
		return m_QuestConfig.GetID();
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Quests/GUI/layouts/quests/expansion_quest_hud_entry.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionQuestHUDEntryController;
	}

	void QuestPrint(string text)
	{
	#ifdef EXPANSIONMODQUESTSUIDEBUG
		Print(text);
	#endif
	}
};

class ExpansionQuestHUDEntryController: ExpansionViewController
{
	string QuestName;
	string ObjectiveText;
	ref ObservableCollection<ref ExpansionQuestHUDObjective> ObjectiveEntries = new ObservableCollection<ref ExpansionQuestHUDObjective>(this);
	
	void ReInit()
	{
		if (!ObjectiveEntries)
			ObjectiveEntries = new ObservableCollection<ref ExpansionQuestHUDObjective>(this);
	}
};
#endif