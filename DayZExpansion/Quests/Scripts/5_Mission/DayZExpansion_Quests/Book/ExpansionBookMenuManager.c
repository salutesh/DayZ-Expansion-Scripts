/**
 * ExpansionBookMenuManager.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODBOOK
modded class ExpansionBookMenuManager
{
	autoptr array<ref ExpansionBookMenuTabBase> m_Tabs = new array<ref ExpansionBookMenuTabBase>;

	ref ExpansionBookMenuTabQuests m_QuestsTab;

	override void RegisterBookMenuTabs(ExpansionBookMenu book_menu)
	{
		super.RegisterBookMenuTabs(book_menu);

		if (GetExpansionSettings().GetQuest().EnableQuestLogTab)
		{
			m_QuestsTab = new ExpansionBookMenuTabQuests(book_menu);
			m_QuestsTab.Hide();
			m_Tabs.Insert(m_QuestsTab);
		}
	}

	ExpansionBookMenuTabQuests GetQuestsTab()
	{
		return m_QuestsTab;
	}
};
#endif
