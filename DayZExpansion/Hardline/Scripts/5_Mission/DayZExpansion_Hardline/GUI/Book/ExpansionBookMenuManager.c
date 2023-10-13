/**
 * ExpansionBookMenuTabFactionRep.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODBOOK
#ifdef EXPANSIONMODAI
modded class ExpansionBookMenuManager
{
	ref ExpansionBookMenuTabFactions m_FactionRepTab;

	override void RegisterBookMenuTabs(ExpansionBookMenu book_menu)
	{
		m_FactionRepTab = new ExpansionBookMenuTabFactions(book_menu);
		m_Tabs.Insert(m_FactionRepTab);
		
		super.RegisterBookMenuTabs(book_menu);
	}

	ExpansionBookMenuTabFactions GetFactionRepTab()
	{
		return m_FactionRepTab;
	}
};
#endif
#endif