/**
 * ExpansionBookMenuManager.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookMenuManager
{
	autoptr array<ref ExpansionBookMenuTabBase> m_Tabs = new array<ref ExpansionBookMenuTabBase>;
	
	#ifdef EXPANSIONMOD	
	ref ExpansionBookMenuTabPlayerProfile m_PlayerProfileTab;
	ref ExpansionBookMenuTabTerritory m_TerritoryTab;
	ref ExpansionBookMenuTabTerritoryInvites m_TerritoryInvitesTab;
	ref ExpansionBookMenuTabParty m_PartyTab;
	ref ExpansionBookMenuTabPartyInvites m_PartyInvitesTab;
	#endif
	
	ref ExpansionBookMenuTabRules m_RulesTab;
	ref ExpansionBookMenuTabServerInfo m_ServerInfoTab;
	//ref ExpansionBookMenuTabNotes m_NotesTab;
	ref ExpansionBookMenuTabCrafting m_CraftingTab;
	
	void ~ExpansionBookMenuManager()
	{
		DestroyBookMenuTabs();
	}
	
	void RegisterBookMenuTabs(ExpansionBookMenu book_menu)
	{
		#ifdef EXPANSIONMOD
		m_PlayerProfileTab = new ExpansionBookMenuTabPlayerProfile(book_menu);
		m_Tabs.Insert(m_PlayerProfileTab);
		
		m_TerritoryTab = new ExpansionBookMenuTabTerritory(book_menu);
		m_Tabs.Insert(m_TerritoryTab);
		
		m_TerritoryInvitesTab = new ExpansionBookMenuTabTerritoryInvites(book_menu)	;
		m_Tabs.Insert(m_TerritoryInvitesTab);
		
		m_PartyTab = new ExpansionBookMenuTabParty(book_menu);
		m_Tabs.Insert(m_PartyTab);
		
		m_PartyInvitesTab = new ExpansionBookMenuTabPartyInvites(book_menu);
		m_Tabs.Insert(m_PartyInvitesTab);
		#endif
		
		m_RulesTab = new ExpansionBookMenuTabRules(book_menu);
		m_Tabs.Insert(m_RulesTab);
		
		m_ServerInfoTab = new ExpansionBookMenuTabServerInfo(book_menu);
		m_Tabs.Insert(m_ServerInfoTab);
		
		//m_NotesTab = new ExpansionBookMenuTabNotes(book_menu);
		//m_Tabs.Insert(m_NotesTab);
		
		m_CraftingTab = new ExpansionBookMenuTabCrafting(book_menu);
		m_Tabs.Insert(m_CraftingTab);	//! WIP
		
		foreach (ExpansionBookMenuTabBase tab: m_Tabs)
		{
			tab.Hide();
		}
	}
	
	void DestroyBookMenuTabs()
	{
		if (m_Tabs && m_Tabs.Count() > 0)
		{
			foreach (ExpansionBookMenuTabBase tab: m_Tabs)
			{
				delete tab;
			}
			
			m_Tabs.Clear();
		}
	}
	
	void UpdateBookMenuTabs(ExpansionBookMenu book_menu)
	{
		DestroyBookMenuTabs();
		RegisterBookMenuTabs(book_menu);
	}
	
	void RegisterBookMenuTab(ExpansionBookMenuTabBase tab)
	{
		m_Tabs.Insert(tab);
	}
	
	void RemoveBookMenuTab(ExpansionBookMenuTabBase tab)
	{
		int index = -1;
		index = m_Tabs.Find(tab);
		if (index > -1)
			m_Tabs.Remove(index);
	}
	
	array<ref ExpansionBookMenuTabBase> GetBookTabs()
	{
		return m_Tabs;
	}
		
	#ifdef EXPANSIONMOD
	ExpansionBookMenuTabPlayerProfile GetPlayerProfileTab()
	{
		return m_PlayerProfileTab;
	}
	
	ExpansionBookMenuTabTerritory GetTerritoryTab()
	{
		return m_TerritoryTab;
	}
	
	ExpansionBookMenuTabTerritoryInvites GetTerritoryInvitesTab()
	{
		return m_TerritoryInvitesTab;
	}
	
	ExpansionBookMenuTabParty GetPartyTab()
	{
		return m_PartyTab;
	}
	 
	ExpansionBookMenuTabPartyInvites GetPartyInvitesTab()
	{
		return m_PartyInvitesTab;
	}
	#endif
	
	ExpansionBookMenuTabRules GetRulesTab()
	{
		return m_RulesTab;
	}
	
	ExpansionBookMenuTabServerInfo GetServerInfoTab()
	{
		return m_ServerInfoTab;
	}
	
	/*ExpansionBookMenuTabNotes GetNotesTab()
	{
		return m_NotesTab;
	}*/
	
	ExpansionBookMenuTabCrafting GetCraftingTab()
	{
		return m_CraftingTab;
	}
};