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
	protected ref array<int> m_HiddenIDs = new array<int>;
	protected WrapSpacerWidget QuestEntriesWraper;
	protected ref map<int, ref ExpansionQuestHUDEntry> m_QuestEntries = new map<int, ref ExpansionQuestHUDEntry>;
	
	void ExpansionQuestHUD()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		
		m_QuestHUDController = ExpansionQuestHUDController.Cast(GetController());
		
		ExpansionQuestModule.GetModuleInstance().GetQuestHUDCallbackSI().Insert(UpdateView);
	}

	void UpdateView()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		
		if (!IsVisible())
		{
			EXTrace.Print(EXTrace.QUESTS, this, "not visible - skipping");
			return;
		}

		ExpansionQuestPersistentData playerData = ExpansionQuestModule.GetModuleInstance().GetClientQuestData();
		if (!playerData)
		{
			EXTrace.Print(EXTrace.QUESTS, this, "no client quest data - skipping");
			return;
		}

		array<ref ExpansionQuestPersistentQuestData> questDatas = playerData.GetQuestDatas();
		array<int> updatedIDs = new array<int>;
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
	
				int findeIndexHidden = -1;
				findeIndexHidden = m_HiddenIDs.Find(questID);
				if (findeIndexHidden == -1)
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
		
		foreach (int entryQuestID, ExpansionQuestHUDEntry hudEntry: m_QuestEntries)
		{
			if (updatedIDs.Find(entryQuestID) == -1)
			{
				hudEntry.Destroy();
				m_QuestEntries.Remove(entryQuestID);
			}
		}
	}

	void ShowHud(bool state)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		
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
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		
		super.OnShow();
		
		UpdateView();
	}
	
	override void OnHide()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		
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
		ExpansionQuestHUDEntry entry;
		int findIndex = -1;
		if (!IsEntryHidden(questID, entry, findIndex))
		{
			m_HiddenIDs.Insert(questID);
			entry.Hide();
		}
		else
		{
			m_HiddenIDs.Remove(findIndex);
			entry.Show();
		}
	}

	bool IsEntryHidden(int questID, out ExpansionQuestHUDEntry entry, out int findIndex)
	{
		findIndex = m_HiddenIDs.Find(questID);
		foreach (ExpansionQuestHUDEntry hudEntry: m_QuestEntries)
		{
			if (hudEntry.GetEntryQuestID() == questID)
			{
				entry = hudEntry;
				break;
			}
		}

		if (findIndex == -1)
			return false;
		return true;
	}
};

class ExpansionQuestHUDController: ExpansionViewController {};
#endif