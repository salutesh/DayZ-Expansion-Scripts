/**
 * ExpansionBookMenuTabTerritoryInviteEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMOD
class ExpansionBookMenuTabPartyInviteEntry: ExpansionScriptView
{
	ref ExpansionBookMenuTabPartyInvites m_InvitesTab;
	ref ExpansionPartyInviteData m_Invite;
	ref ExpansionBookMenuTabPartyInviteEntryController m_InviteEntryController;
	
	private ImageWidget invite_entry_icon;
	private ButtonWidget invite_entry_button;
	private TextWidget invite_entry_label;
	
	void ExpansionBookMenuTabPartyInviteEntry(ExpansionBookMenuTabPartyInvites tab, ExpansionPartyInviteData invite)
	{
		m_InvitesTab = tab;
		m_Invite = invite;
		
		if (!m_InviteEntryController)
			m_InviteEntryController = ExpansionBookMenuTabPartyInviteEntryController.Cast(GetController());
		
		SetView();
	}
	
	void SetView()
	{
		m_InviteEntryController.PartyName = m_Invite.partyName;
		m_InviteEntryController.NotifyPropertyChanged("PartyName");
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/party/expansion_book_tab_party_invite_entry.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabPartyInviteEntryController;
	}
	
	void OnInviteEntryButtonClick()
	{
		m_InvitesTab.SetSelectedInvite(m_Invite);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
		case invite_entry_button:
			invite_entry_icon.SetColor(ARGB(255,220,220,220));
			invite_entry_label.SetColor(ARGB(255,220,220,220));
			break;
		}
		
		return super.OnMouseEnter(w, x, y);;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
		case invite_entry_button:
			invite_entry_icon.SetColor(ARGB(255,0,0,0));
			invite_entry_label.SetColor(ARGB(255,0,0,0));
			break;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	ExpansionPartyInviteData GetInvite()
	{
		return m_Invite;
	}
};

class ExpansionBookMenuTabPartyInviteEntryController: ExpansionViewController
{
	string PartyName;
};
#endif
