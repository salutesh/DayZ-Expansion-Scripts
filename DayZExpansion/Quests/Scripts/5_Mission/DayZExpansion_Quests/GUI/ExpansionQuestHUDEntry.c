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
	protected ExpansionQuestPersistentQuestData  m_QuestData;
	protected ImageWidget QuestIcon;
	protected Widget Spacer;
	protected WrapSpacerWidget ObjectiveEntries;
	protected ref array<ref ExpansionQuestHUDObjective> m_ObjectiveEntries = new array<ref ExpansionQuestHUDObjective>;

	void ExpansionQuestHUDEntry(ExpansionQuestConfig questConfig, ExpansionQuestPersistentQuestData questData)
	{
		m_QuestConfig = questConfig;
		m_QuestData = questData;
		m_QuestHUDEntryController = ExpansionQuestHUDEntryController.Cast(GetController());
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
			Spacer.SetColor(GetQuestColor(m_QuestConfig));
			QuestIcon.SetColor(GetQuestColor(m_QuestConfig));
		}
		else if (state == ExpansionQuestState.CAN_TURNIN)
		{
			m_QuestHUDEntryController.ObjectiveText = "#STR_EXPANSION_QUEST_HUD_TURN_IN";
			Spacer.SetColor(ARGB(255, 39, 174, 96));
			QuestIcon.SetColor(ARGB(255, 39, 174, 96));
		}

		m_QuestHUDEntryController.NotifyPropertyChanged("ObjectiveText");

		foreach (ExpansionQuestHUDObjective currentEntry: m_ObjectiveEntries)
		{
			ObjectiveEntries.RemoveChild(currentEntry.GetLayoutRoot());
		}

		m_ObjectiveEntries.Clear();

		QuestPrint(ToString() + "---------------------------------------------------------------------------");
		QuestPrint(ToString() + "::SetEntry - Add objectives for quest: " + m_QuestConfig.GetTitle());
		int objectiveCount = 0;
		for (int i = 0; i < m_QuestConfig.GetObjectives().Count(); i++)
		{
			QuestPrint(ToString() + "::SetEntry - Adding objectives!");
			ExpansionQuestObjectiveConfig objectiveConfig = m_QuestConfig.GetObjectives().Get(i);
			if (!objectiveConfig)
			{
				QuestPrint(ToString() + "::SetEntry - F1");
				continue;
			}

			QuestPrint(ToString() + "::SetEntry - ExpansionQuestObjectiveConfig: " + objectiveConfig.ToString());

			ExpansionQuestObjectiveData objective = m_QuestData.QuestObjectives.Get(i);
			if (objective)
			{
				QuestPrint(ToString() + "::SetEntry - ExpansionQuestObjectiveData: " + objective.ToString());

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
		CF_Log.Debug(text);
	#endif
	}
};

class ExpansionQuestHUDEntryController: ExpansionViewController
{
	string QuestName;
	string ObjectiveText;
};
#endif