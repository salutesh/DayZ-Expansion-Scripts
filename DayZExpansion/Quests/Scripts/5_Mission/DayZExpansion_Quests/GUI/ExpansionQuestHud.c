/**
 * ExpansionQuestHUD.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestHUD: ExpansionScriptView
{
	protected ref ExpansionQuestHUDController m_QuestHUDController;
	protected ref map<int, bool> m_QuestEntriesVisibilityPreference = new map<int, bool>;
	protected WrapSpacerWidget QuestEntriesWraper;
	protected ref map<int, ref ExpansionQuestHUDEntry> m_QuestEntries = new map<int, ref ExpansionQuestHUDEntry>;
	
	void ExpansionQuestHUD()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS & EXTrace.UI, this);
#endif

		m_QuestHUDController = ExpansionQuestHUDController.Cast(GetController());
		
		ExpansionQuestModule.GetModuleInstance().GetQuestHUDCallbackSI().Insert(UpdateView);
	}

	void ~ExpansionQuestHUD()
	{
		if (ExpansionQuestModule.GetModuleInstance())
			ExpansionQuestModule.GetModuleInstance().GetQuestHUDCallbackSI().Remove(UpdateView);
	}

	void UpdateView()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		if (!g_Game.GetPlayer())
		{
			EXTrace.Print(EXTrace.QUESTS, this, "::UpdateView - no player - skipping");
			return;
		}

		if (!IsVisible())
		{
			EXTrace.Print(EXTrace.QUESTS, this, "::UpdateView - not visible - skipping");
			return;
		}
		
		map<int, bool> visStates = GetExpansionClientSettings().QuestVisibilityStates;
		EXTrace.Print(EXTrace.QUESTS, this, "::UpdateView - Quest entry visibility states: " + visStates.Count());
		foreach (int visQuestID, bool visState: visStates)
		{
			EXTrace.Print(EXTrace.QUESTS, this, "::UpdateView - Set quest entry visibility state for quest entry: " + visQuestID + " | State: " + visState);
			m_QuestEntriesVisibilityPreference[visQuestID] = visState;
		}

		ExpansionQuestPersistentData playerData = ExpansionQuestModule.GetModuleInstance().GetClientQuestData();
		if (!playerData)
		{
			EXTrace.Print(EXTrace.QUESTS, this, "::UpdateView - no client quest data - skipping");
			return;
		}

		array<int> updatedIDs = new array<int>;
		bool isVisible;
		foreach (ExpansionQuestPersistentQuestData data: playerData.QuestData)
		{
			int questID = data.QuestID;
			ExpansionQuestState state = data.State;

			if (state == ExpansionQuestState.NONE || state == ExpansionQuestState.COMPLETED)
				continue;

			ExpansionQuestConfig questConfig = ExpansionQuestModule.GetModuleInstance().GetQuestConfigByID(questID);
			if (!questConfig)
			{
				continue;
			}

			if (questConfig.IsAchievement())
				continue;

			ExpansionQuestHUDEntry entry;
			if (m_QuestEntries.Find(questConfig.GetID(), entry))
			{
				entry.UpdateQuestData(data);
			}
			else
			{
				entry = new ExpansionQuestHUDEntry(questConfig, data);
				QuestEntriesWraper.AddChild(entry.GetLayoutRoot());
				m_QuestEntries.Insert(questConfig.GetID(), entry);
				entry.SetEntry();
	
				if (!m_QuestEntriesVisibilityPreference.Find(questID, isVisible) || isVisible)
				{
					EXTrace.Print(EXTrace.QUESTS, this, "::UpdateView - Show entry for quest with ID " + questID + ".");
					entry.Show();
				}
				else
				{
					EXTrace.Print(EXTrace.QUESTS, this, "::UpdateView - Hide entry for quest with ID " + questID + ".");
					entry.Hide();
				}
			}
			
			updatedIDs.Insert(questID);
		}

		array<int> toRemoveIds = {};
		foreach (int entryQuestID, ExpansionQuestHUDEntry hudEntry: m_QuestEntries)
		{
			if (updatedIDs.Find(entryQuestID) == -1)
			{
				hudEntry.Destroy();
				toRemoveIds.Insert(entryQuestID);
			}
		}

		foreach (int toRemoveId: toRemoveIds)
		{
			m_QuestEntries.Remove(toRemoveId);
			m_QuestEntriesVisibilityPreference.Remove(toRemoveId);
			
			ExpansionQuestState questState = ExpansionQuestModule.GetModuleInstance().GetClientQuestData().GetQuestStateByQuestID(toRemoveId);
			if (questState == ExpansionQuestState.NONE || questState == ExpansionQuestState.COMPLETED)
				GetExpansionClientSettings().SetQuestVisibilityState(toRemoveId, true);
		}
	}

	void ShowHud(bool state)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS & EXTrace.UI, this);
#endif

		if (state)
		{
			Show();
		}
		else
		{
			Hide();
		}
	}
	
	override void OnShow()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS & EXTrace.UI, this);
#endif

		super.OnShow();
		
		UpdateView();
	}
	
	override void OnHide()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS & EXTrace.UI, this);
#endif

		super.OnHide();
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Quests/GUI/layouts/quests/expansion_quest_hud.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionQuestHUDController;
	}

	void ToggleQuestEntryVisibilityByID(int questID)
	{
		ExpansionQuestHUDEntry entry = m_QuestEntries[questID];
		bool isEntryVisible = IsEntryHidden(questID);
		if (!isEntryVisible)
		{
			m_QuestEntriesVisibilityPreference[questID] = false;
			entry.Hide();
		}
		else
		{
			m_QuestEntriesVisibilityPreference[questID] = true;
			entry.Show();
		}
		
		GetExpansionClientSettings().SetQuestVisibilityState(questID, isEntryVisible);
	}

	bool IsEntryHidden(int questID)
	{
		bool isVisible;
		return m_QuestEntriesVisibilityPreference.Find(questID, isVisible) && !isVisible;
	}
};

class ExpansionQuestHUDController: ExpansionViewController {};
