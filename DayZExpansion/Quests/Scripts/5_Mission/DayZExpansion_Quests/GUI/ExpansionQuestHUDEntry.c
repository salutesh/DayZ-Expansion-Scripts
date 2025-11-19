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

class ExpansionQuestHUDEntry: ExpansionScriptView
{
	protected ref ExpansionQuestHUDEntryController m_QuestHUDEntryController;
	protected ExpansionQuestConfig m_QuestConfig;
	protected ExpansionQuestPersistentQuestData  m_QuestData;
	protected ImageWidget QuestIcon;
	protected Widget Spacer;
	protected Widget Spacer0;
	protected WrapSpacerWidget ObjectiveEntries;
	protected RichTextWidget Objective; 
	protected ref map<int, ref ExpansionQuestHUDObjective> m_ObjectiveEntries = new map<int, ref ExpansionQuestHUDObjective>;

	void ExpansionQuestHUDEntry(ExpansionQuestConfig questConfig, ExpansionQuestPersistentQuestData questData)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif
		
		m_QuestConfig = questConfig;
		m_QuestData = questData;
		m_QuestHUDEntryController = ExpansionQuestHUDEntryController.Cast(GetController());
	}

	void ~ExpansionQuestHUDEntry()
	{
		if (g_Game && Objective)
			g_Game.GetCallQueue(CALL_CATEGORY_GUI).Remove(Objective.SetSize);
	}

	void SetEntry()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif
		
		if (!m_QuestConfig)
			return;
		
		m_QuestHUDEntryController.QuestName = m_QuestConfig.GetTitle();
		m_QuestHUDEntryController.NotifyPropertyChanged("QuestName");

		ExpansionQuestState state = m_QuestData.State;
		if (state == ExpansionQuestState.STARTED)
		{
			if (m_QuestConfig.GetObjectiveText() != string.Empty)
			{
				Objective.Show(true);
				m_QuestHUDEntryController.ObjectiveText = m_QuestConfig.GetObjectiveText();
				m_QuestHUDEntryController.NotifyPropertyChanged("ObjectiveText");
				g_Game.GetCallQueue(CALL_CATEGORY_GUI).CallLater(Objective.SetSize, 100, false, 0.97, 1.0, true);
				Spacer.SetColor(ExpansionQuestModule.GetQuestColor(m_QuestConfig));
				Spacer.Show(true);
			}
			else
			{
				Objective.Show(false);
				Spacer.Show(false);
			}
			
			Spacer0.SetColor(ExpansionQuestModule.GetQuestColor(m_QuestConfig));
			QuestIcon.SetColor(ExpansionQuestModule.GetQuestColor(m_QuestConfig));
		}
		else if (state == ExpansionQuestState.CAN_TURNIN)
		{
			Objective.Show(true);
			m_QuestHUDEntryController.ObjectiveText = "#STR_EXPANSION_QUEST_HUD_TURN_IN";
			m_QuestHUDEntryController.NotifyPropertyChanged("ObjectiveText");
			g_Game.GetCallQueue(CALL_CATEGORY_GUI).CallLater(Objective.SetSize, 100, false, 0.97, 1.0, true);
			Spacer.SetColor(ARGB(200, 160, 223, 59));
			Spacer.Show(true);
			Spacer0.SetColor(ARGB(200, 160, 223, 59));
			QuestIcon.SetColor(ARGB(200, 160, 223, 59));
		}
		
		array<int> updatedObjectives = new array<int>;
		int objectiveIndex;
		foreach (ExpansionQuestObjectiveData objective: m_QuestData.QuestObjectives)
		{
			//! @note: We only want to add objectives to the HUD that are currently active!
			if (!objective.IsActive())
				continue;
			
			ExpansionQuestHUDObjective objectiveEntry;
			if (m_ObjectiveEntries.Find(objectiveIndex, objectiveEntry))
			{
				objectiveEntry.UpdateObjectiveData(objective);
			}
			else
			{
				objectiveEntry = new ExpansionQuestHUDObjective(objective, m_QuestConfig, state);
				ObjectiveEntries.AddChild(objectiveEntry.GetLayoutRoot());
				m_ObjectiveEntries.Insert(objectiveIndex, objectiveEntry);
				objectiveEntry.SetEntryObjective();
			}

			updatedObjectives.Insert(objectiveIndex);
			objectiveIndex++;
		}
		
		foreach (int objectiveEntryIndex, ExpansionQuestHUDObjective objectiveHUDEntry: m_ObjectiveEntries)
		{
			if (updatedObjectives.Find(objectiveEntryIndex) == -1)
			{
				ObjectiveEntries.RemoveChild(objectiveHUDEntry.GetLayoutRoot());
				objectiveHUDEntry.Destroy();
				m_ObjectiveEntries.Remove(objectiveEntryIndex);
			}
		}
	}
	
	void UpdateQuestData(ExpansionQuestPersistentQuestData questData)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif
		
		m_QuestData = questData;
		SetEntry();
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
};

class ExpansionQuestHUDEntryController: ExpansionViewController
{
	string QuestName;
	string ObjectiveText;
};
