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

#ifdef EXPANSIONMODQUESTS_HUD_ENABLE
class ExpansionQuestHUD: ExpansionScriptView
{
	protected ref ExpansionQuestHUDController m_QuestHUDController;
	protected ref map<int, bool> m_QuestEntriesVisibilityPreference = new map<int, bool>;
	protected WrapSpacerWidget QuestEntriesWraper;
	protected ref map<int, ref ExpansionQuestHUDEntry> m_QuestEntries = new map<int, ref ExpansionQuestHUDEntry>;
	
	void ExpansionQuestHUD()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS & EXTrace.UI, this);
		
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
		auto trace = EXTrace.Start(true, this);
		
		if (!GetGame().GetPlayer())
		{
			EXTrace.Print(true, this, "no player - skipping");
			return;
		}

		if (!IsVisible())
		{
			EXTrace.Print(true, this, "not visible - skipping");
			return;
		}

		ExpansionQuestPersistentData playerData = ExpansionQuestModule.GetModuleInstance().GetClientQuestData();
		if (!playerData)
		{
			EXTrace.Print(true, this, "no client quest data - skipping");
			return;
		}

		array<ref ExpansionQuestPersistentQuestData> questDatas = playerData.GetQuestDatas();
		array<int> updatedIDs = new array<int>;
		bool isVisible;
		foreach (ExpansionQuestPersistentQuestData data: questDatas)
		{
			int questID = data.QuestID;
			ExpansionQuestState state = data.State;

			if (state == ExpansionQuestState.NONE || state == ExpansionQuestState.COMPLETED)
				continue;

			ExpansionQuestConfig questConfig = ExpansionQuestModule.GetModuleInstance().GetQuestConfigClientByID(questID);
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
					entry.Show();
				}
				else
				{
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
		}
	}

	void ShowHud(bool state)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS & EXTrace.UI, this);
		
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
		auto trace = EXTrace.Start(EXTrace.QUESTS & EXTrace.UI, this);
		
		super.OnShow();
		
		UpdateView();
	}
	
	override void OnHide()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS & EXTrace.UI, this);
		
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
		if (!IsEntryHidden(questID))
		{
			m_QuestEntriesVisibilityPreference[questID] = false;
			entry.Hide();
		}
		else
		{
			m_QuestEntriesVisibilityPreference[questID] = true;
			entry.Show();
		}
	}

	bool IsEntryHidden(int questID)
	{
		bool isVisible;
		return m_QuestEntriesVisibilityPreference.Find(questID, isVisible) && !isVisible;
	}
};

class ExpansionQuestHUDController: ExpansionViewController {};
#endif