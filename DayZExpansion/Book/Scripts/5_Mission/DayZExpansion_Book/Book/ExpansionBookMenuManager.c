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

class ExpansionBookMenuManager
{
	autoptr array<ref ExpansionBookMenuTabBase> m_Tabs = new array<ref ExpansionBookMenuTabBase>;
	
	//TODO: move to main Mod
	#ifdef EXPANSIONMONITORMODULE	
	ref ExpansionBookMenuTabPlayerProfile m_PlayerProfileTab;
	#endif

	#ifdef EXPANSIONMODBASEBUILDING
	ref ExpansionBookMenuTabTerritory m_TerritoryTab;
	ref ExpansionBookMenuTabTerritoryInvites m_TerritoryInvitesTab;
	#endif

	//TODO: move to Party mod
	#ifdef EXPANSIONMODGROUPS
	ref ExpansionBookMenuTabParty m_PartyTab;
	ref ExpansionBookMenuTabPartyInvites m_PartyInvitesTab;
	#endif
	
	ref ExpansionBookMenuTabRules m_RulesTab;
	ref ExpansionBookMenuTabServerInfo m_ServerInfoTab;
	ref ExpansionBookMenuTabCrafting m_CraftingTab;
	
	void ~ExpansionBookMenuManager()
	{
		DestroyBookMenuTabs();
	}
	
	void RegisterBookMenuTabs(ExpansionBookMenu book_menu)
	{
		#ifdef EXPANSIONMONITORMODULE
		if (GetExpansionSettings().GetBook().EnableStatusTab)
		{
			m_PlayerProfileTab = new ExpansionBookMenuTabPlayerProfile(book_menu);
			m_Tabs.Insert(m_PlayerProfileTab);
		}
		#endif
		
		#ifdef EXPANSIONMODBASEBUILDING
		if (GetExpansionSettings().GetBook().EnableTerritoryTab)
		{
			m_TerritoryTab = new ExpansionBookMenuTabTerritory(book_menu);
			m_Tabs.Insert(m_TerritoryTab);
			
			m_TerritoryInvitesTab = new ExpansionBookMenuTabTerritoryInvites(book_menu)	;
			m_Tabs.Insert(m_TerritoryInvitesTab);
		}
		#endif
		
		#ifdef EXPANSIONMODGROUPS
		if (GetExpansionSettings().GetBook().EnablePartyTab)
		{
			m_PartyTab = new ExpansionBookMenuTabParty(book_menu);
			m_Tabs.Insert(m_PartyTab);
			
			m_PartyInvitesTab = new ExpansionBookMenuTabPartyInvites(book_menu);
			m_Tabs.Insert(m_PartyInvitesTab);
		}
		#endif
		
		if (GetExpansionSettings().GetBook().EnableServerRulesTab)
		{
			m_RulesTab = new ExpansionBookMenuTabRules(book_menu);
			m_Tabs.Insert(m_RulesTab);
		}
		
		if (GetExpansionSettings().GetBook().EnableServerInfoTab)
		{
			m_ServerInfoTab = new ExpansionBookMenuTabServerInfo(book_menu);
			m_Tabs.Insert(m_ServerInfoTab);
		}
				
		m_CraftingTab = new ExpansionBookMenuTabCrafting(book_menu);
		m_Tabs.Insert(m_CraftingTab);
		
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
				tab.Destroy();
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
		
	#ifdef EXPANSIONMONITORMODULE
	ExpansionBookMenuTabPlayerProfile GetPlayerProfileTab()
	{
		return m_PlayerProfileTab;
	}
	#endif
	
	#ifdef EXPANSIONMODBASEBUILDING
	ExpansionBookMenuTabTerritory GetTerritoryTab()
	{
		return m_TerritoryTab;
	}
	
	ExpansionBookMenuTabTerritoryInvites GetTerritoryInvitesTab()
	{
		return m_TerritoryInvitesTab;
	}
	#endif
	
	#ifdef EXPANSIONMODGROUPS
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
	
	ExpansionBookMenuTabCrafting GetCraftingTab()
	{
		return m_CraftingTab;
	}
};
