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
		QuestDebugPrint(ToString() + "::SetEntry - Start");

		m_QuestHUDEntryController.QuestName = m_QuestConfig.GetTitle();
		m_QuestHUDEntryController.NotifyPropertyChanged("QuestName");

		int state = m_QuestData.State;
		if (state == ExpansionQuestState.STARTED)
		{
			m_QuestHUDEntryController.ObjectiveText = m_QuestConfig.GetObjectiveText();
			Spacer.SetColor(ExpansionQuestModule.GetQuestColor(m_QuestConfig));
			QuestIcon.SetColor(ExpansionQuestModule.GetQuestColor(m_QuestConfig));
		}
		else if (state == ExpansionQuestState.CAN_TURNIN)
		{
			m_QuestHUDEntryController.ObjectiveText = "#STR_EXPANSION_QUEST_HUD_TURN_IN";
			Spacer.SetColor(ARGB(200, 160, 223, 59));
			QuestIcon.SetColor(ARGB(200, 160, 223, 59));
		}

		m_QuestHUDEntryController.NotifyPropertyChanged("ObjectiveText");

		foreach (ExpansionQuestHUDObjective currentEntry: m_ObjectiveEntries)
		{
			ObjectiveEntries.RemoveChild(currentEntry.GetLayoutRoot());
			currentEntry.Destroy();
		}

		m_ObjectiveEntries.Clear();

		QuestDebugPrint(ToString() + "---------------------------------------------------------------------------");
		QuestDebugPrint(ToString() + "::SetEntry - Add objectives for quest: " + m_QuestConfig.GetTitle());
		int objectiveCount = 0;
		for (int i = 0; i < m_QuestConfig.GetObjectives().Count(); i++)
		{
			QuestDebugPrint(ToString() + "::SetEntry - Adding objectives!");
			ExpansionQuestObjectiveConfig objectiveConfig = m_QuestConfig.GetObjectives().Get(i);
			if (!objectiveConfig)
			{
				QuestDebugPrint(ToString() + "::SetEntry - F1");
				continue;
			}

			QuestDebugPrint(ToString() + "::SetEntry - ExpansionQuestObjectiveConfig: " + objectiveConfig.ToString());

			ExpansionQuestObjectiveData objective = m_QuestData.QuestObjectives.Get(i);
			if (objective)
			{
				QuestDebugPrint(ToString() + "::SetEntry - ExpansionQuestObjectiveData: " + objective.ToString());

				if (!objective.IsActive())
				{
					QuestDebugPrint(ToString() + "::SetEntry - F2");
					continue;
				}

				objectiveCount++;
				QuestDebugPrint(ToString() + "::SetEntry - Add objective entry for objective: " + objective.ToString() + " | Index: " + objectiveCount);
				ExpansionQuestHUDObjective objectiveEntry = new ExpansionQuestHUDObjective(objective, m_QuestConfig);
				ObjectiveEntries.AddChild(objectiveEntry.GetLayoutRoot());
				m_ObjectiveEntries.Insert(objectiveEntry);
				objectiveEntry.SetEntryObjective();
			}
		}
		QuestDebugPrint(ToString() + "---------------------------------------------------------------------------");
		QuestDebugPrint(ToString() + "::SetEntry - Added " + objectiveCount +  " objectives!");

		ObjectiveEntries.Update();
		GetLayoutRoot().Update();

		QuestDebugPrint(ToString() + "::SetEntry - End");
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

	void QuestDebugPrint(string text)
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