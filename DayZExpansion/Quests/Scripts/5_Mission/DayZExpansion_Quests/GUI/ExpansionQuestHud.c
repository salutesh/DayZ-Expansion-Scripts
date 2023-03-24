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
	protected ref array<ref ExpansionQuestHUDEntry> m_QuestEntries = new array<ref ExpansionQuestHUDEntry>;

	void ExpansionQuestHUD()
	{
		if (!m_QuestHUDController)
			m_QuestHUDController = ExpansionQuestHUDController.Cast(GetController());
	}

	void SetView()
	{
		ExpansionQuestPersistentData playerData = ExpansionQuestModule.GetModuleInstance().GetClientQuestData();
		if (!playerData)
		{
			return;
		}

		if (m_QuestEntries && m_QuestEntries.Count() > 0)
		{
			foreach (ExpansionQuestHUDEntry hudEntry: m_QuestEntries)
			{
				hudEntry.Destroy();
			}

			m_QuestEntries.Clear();
		}

		array<ref ExpansionQuestPersistentQuestData> questDatas = playerData.GetQuestDatas();
		foreach (ExpansionQuestPersistentQuestData data: questDatas)
		{
			if (!data)
			{
				continue;
			}

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

			ExpansionQuestHUDEntry entry = new ExpansionQuestHUDEntry(questConfig, data);
			QuestEntriesWraper.AddChild(entry.GetLayoutRoot());
			m_QuestEntries.Insert(entry);
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
	}

	void ShowHud(bool state)
	{
		if (state)
		{
			Show();
		}
		else
		{
			Hide();
		}
	}

	override float GetUpdateTickRate()
	{
		return 0.5;
	}

	override void Update()
	{
		if (!GetGame())
			return;

		if (!GetGame().GetPlayer())
			return;

		SetView();
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