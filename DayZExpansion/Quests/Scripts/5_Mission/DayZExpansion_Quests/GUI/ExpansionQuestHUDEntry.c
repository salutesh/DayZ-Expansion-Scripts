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
	protected ExpansionQuestPersistentPlayerData  m_QuestData;
	protected ImageWidget QuestIcon;
	protected Widget Spacer;
	protected WrapSpacerWidget ObjectiveEntries;
	protected ref array<ref ExpansionQuestHUDObjective> m_ObjectiveEntries = new array<ref ExpansionQuestHUDObjective>;

	void ExpansionQuestHUDEntry(ExpansionQuestConfig questConfig, ExpansionQuestPersistentPlayerData questData)
	{
		m_QuestConfig = questConfig;
		m_QuestData = questData;
		m_QuestHUDEntryController = ExpansionQuestHUDEntryController.Cast(GetController());
	}

	void ~ExpansionQuestHUDEntry()
	{
		m_ObjectiveEntries.Clear();
	}

	void SetEntry()
	{
		QuestPrint(ToString() + "::SetEntry - Start");

		m_QuestHUDEntryController.QuestName = m_QuestConfig.GetTitle();
		m_QuestHUDEntryController.NotifyPropertyChanged("QuestName");

		int state = m_QuestData.State;
		if (state == ExpansionQuestState.STARTED)
		{
			m_QuestHUDEntryController.ObjectiveText = m_QuestConfig.GetObjectiveText();
		}
		else if (state == ExpansionQuestState.CAN_TURNIN)
		{
			m_QuestHUDEntryController.ObjectiveText = "#STR_EXPANSION_QUEST_HUD_TURN_IN";
		}

		m_QuestHUDEntryController.NotifyPropertyChanged("ObjectiveText");

		Spacer.SetColor(GetQuestColor(m_QuestConfig));
		QuestIcon.SetColor(GetQuestColor(m_QuestConfig));

		foreach (ExpansionQuestHUDObjective currentEntry: m_ObjectiveEntries)
		{
			ObjectiveEntries.RemoveChild(currentEntry.GetLayoutRoot());
			delete currentEntry;
		}

		m_ObjectiveEntries.Clear();

		QuestPrint(ToString() + "---------------------------------------------------------------------------");
		QuestPrint(ToString() + "::SetEntry - Add objectives for quest: " + m_QuestConfig.GetTitle());
		int objectiveCount = 0;
		for (int i = 0; i < m_QuestConfig.GetObjectives().Count(); i++)
		{
			QuestPrint(ToString() + "::SetEntry - Adding objectives!");
			ExpansionQuestObjectiveConfigBase objectiveConfig = m_QuestConfig.GetObjectives().Get(i);
			if (!objectiveConfig)
			{
				QuestPrint(ToString() + "::SetEntry - F1");
				continue;
			}
			
			QuestPrint(ToString() + "::SetEntry - ExpansionQuestObjectiveConfigBase: " + objectiveConfig.ToString());
			
			ExpansionQuestObjectivePlayerData objective = m_QuestData.QuestObjectives.Get(i)
			if (objective)
			{
				QuestPrint(ToString() + "::SetEntry - ExpansionQuestObjectivePlayerData: " + objective.ToString());
				
				if (!objective.IsActive())
				{
					QuestPrint(ToString() + "::SetEntry - F2");
					continue;
				}
				
				objectiveCount++;
				QuestPrint(ToString() + "::SetEntry - Add objective entry for objective: " + objective.ToString() + " | Index: " + objectiveCount);
				ExpansionQuestHUDObjective objectiveEntry = new ExpansionQuestHUDObjective(objective, m_QuestConfig);
				ObjectiveEntries.AddChild(objectiveEntry.GetLayoutRoot());
				m_ObjectiveEntries.Insert(objectiveEntry);
				objectiveEntry.SetEntryObjective();
			}
		}
		QuestPrint(ToString() + "---------------------------------------------------------------------------");
		QuestPrint(ToString() + "::SetEntry - Added " + objectiveCount +  " objectives!");

		ObjectiveEntries.Update();
		GetLayoutRoot().Update();

		QuestPrint(ToString() + "::SetEntry - End");
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
};
#endif