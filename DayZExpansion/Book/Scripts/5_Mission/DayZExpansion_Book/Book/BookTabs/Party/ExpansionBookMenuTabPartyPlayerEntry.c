/**
 * ExpansionBookMenuTabTerritoryPlayerEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODGROUPS
class ExpansionBookMenuTabPartyPlayerEntry: ExpansionScriptView
{
	protected ref ExpansionBookMenuTabParty m_PartyTab;
	protected ref SyncPlayer m_Player;
	protected ref ExpansionBookMenuTabPartyPlayerEntryController m_EntryController;
	
	protected TextWidget player_entry_label;
	protected ButtonWidget player_entry_button;
	protected ImageWidget player_entry_icon;
	
	void ExpansionBookMenuTabPartyPlayerEntry(ExpansionBookMenuTabParty tab, SyncPlayer player)
	{
		m_PartyTab = tab;
		m_Player = player;
		
		if (!m_EntryController)
			m_EntryController = ExpansionBookMenuTabPartyPlayerEntryController.Cast(GetController());
		
		SetEntry(m_Player);
	}
	
	void SetEntry(SyncPlayer playerSync)
	{
		m_EntryController.PlayerName = playerSync.m_PlayerName;
		m_EntryController.NotifyPropertyChanged("PlayerName");
		
		m_EntryController.Background = "set:book_assets image:menu_element_1";
		m_EntryController.NotifyPropertyChanged("Background");
	}

	void OnElementButtonClick()
	{
		if (!m_PartyTab)
			return;
		
		if (!m_Player)
			return;
		
		m_PartyTab.InvitePlayer(m_Player);
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/party/expansion_book_tab_party_player_entry.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabPartyPlayerEntryController;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w == player_entry_button)
		{
			player_entry_label.SetColor(ARGB(255, 220, 220, 220));
			player_entry_icon.SetColor(ARGB(255, 220, 220, 220));
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w == player_entry_button)
		{
			player_entry_label.SetColor(ARGB(255, 0, 0, 0));
			player_entry_icon.SetColor(ARGB(255, 0, 0, 0));
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	SyncPlayer GetSyncPlayer()
	{
		return m_Player;
	}
};

class ExpansionBookMenuTabPartyPlayerEntryController: ExpansionViewController
{
	string PlayerName;
	string Background;
};
#endif
