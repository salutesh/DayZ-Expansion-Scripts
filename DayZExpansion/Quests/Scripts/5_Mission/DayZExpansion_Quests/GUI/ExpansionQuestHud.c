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
	protected ExpansionQuestModule m_QuestModule;
	protected ref array<int> m_HiddenIDs = new array<int>;
	protected WrapSpacerWidget QuestEntriesWraper;
	protected ref array<ref ExpansionQuestHUDEntry> m_QuestEntries = new array<ref ExpansionQuestHUDEntry>;

	void ExpansionQuestHUD()
	{
		if (!m_QuestHUDController)
			m_QuestHUDController = ExpansionQuestHUDController.Cast(GetController());

		if (!m_QuestModule)
			m_QuestModule = ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));
	}

	void SetView(ExpansionQuestPlayerData playerData)
	{
		QuestPrint(ToString() + "::SetView - Start");

		m_QuestEntries.Clear();

		if (!playerData)
			return;

		if (!m_QuestModule)
			m_QuestModule = ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));

		if (!m_QuestEntries)
		{
			m_QuestEntries = new array<ref ExpansionQuestHUDEntry>;
		}

		for (int i = 0; i < playerData.GetQuestDatas().Count(); i++)
		{
			ExpansionQuestPersistentPlayerData data = playerData.GetQuestDatas().Get(i);
			int questID = data.QuestID;
			int state = data.State;

			QuestPrint(ToString() + "::SetView - Quest ID: " + questID);
			QuestPrint(ToString() + "::SetView - Quest state: " + state);

			if (state > ExpansionQuestState.NONE && state < ExpansionQuestState.COMPLETED)
			{
				ExpansionQuestConfig questConfig = m_QuestModule.GetQuestConfigClientByID(questID);
				if (!questConfig || questConfig.IsAchivement())
					continue;

				QuestPrint(ToString() + "::SetView - Quest config: " + questConfig);
				QuestPrint(ToString() + "::SetView - Add new entry for quest: " + questID);

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

		QuestPrint(ToString() + "::SetView - End");
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

	override string GetLayoutFile()
	{
		return "DayZExpansion/Quests/GUI/layouts/quests/expansion_quest_hud.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionQuestHUDController;
	}

	override float GetUpdateTickRate()
	{
		return 0.5;
	}

	override void Update()
	{
		if (m_QuestModule)
		{
			SetView(m_QuestModule.GetClientQuestData());
		}
	}

	void QuestPrint(string text)
	{
	#ifdef EXPANSIONMODQUESTSUIDEBUG
		Print(text);
	#endif
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
		for (int i = 0; i < m_QuestEntries.Count(); i++)
		{
			entry = m_QuestEntries[i];
			if (entry.GetEntryQuestID() == questID)
			{
				if (findIndex == -1)
				{
					return false;
				}
			}
		}

		return true;
	}
};

class ExpansionQuestHUDController: ExpansionViewController {};
#endif