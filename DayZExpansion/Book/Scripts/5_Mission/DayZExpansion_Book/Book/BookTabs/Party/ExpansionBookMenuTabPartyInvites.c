/**
 * ExpansionBookMenuTabPartyInvites.c
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
class ExpansionBookMenuTabPartyInvites: ExpansionBookMenuTabBase
{
	ref ExpansionBookMenuTabPartyInvitesController m_InvitesController;
	ref ExpansionPartyInviteData m_Invite;
	ref ExpansionPartyModule m_PartyModule;
	
	private ButtonWidget option_accept_button;
	private TextWidget option_accept_label;
	private ButtonWidget option_decline_button;
	private TextWidget option_decline_label;
	private ButtonWidget option_cancel_button;
	private TextWidget option_cancel_label;
	private Widget invite_info_content_left;
	
	void ExpansionBookMenuTabPartyInvites(ExpansionBookMenu book_menu)
	{
		m_BookMenu = book_menu;
		
		if (!m_InvitesController)
			m_InvitesController = ExpansionBookMenuTabPartyInvitesController.Cast(GetController());
		
		if (!m_PartyModule)
			m_PartyModule = ExpansionPartyModule.Cast(CF_ModuleCoreManager.Get(ExpansionPartyModule));
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/party/expansion_book_tab_party_invites.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabPartyInvitesController;
	}
	
	override string GetTabIconName()
	{
		return "Group";
	}
	
	override string GetTabName()
	{
		return "Party Invites";
	}
	
	override int GetTabColor()
	{
		return ARGB(255,0,0,0);
	}
	
	override bool IsParentTab()
	{
		return true;
	}
	
	void SetView()
	{	
		LoadInvites();
				
		option_accept_button.Show(false);
		option_decline_button.Show(false);
		invite_info_content_left.Show(false);
	}
	
	void LoadInvites()
	{	
		if (!m_InvitesController)
			return;
		
		if (m_InvitesController.PartyInviteEntrys.Count() > 0)
			m_InvitesController.PartyInviteEntrys.Clear();
		
		if (m_PartyModule.GetPartyInvites())
		{
			for (int i = 0; i < m_PartyModule.GetPartyInvites().Count(); ++i)
			{
				ExpansionPartyInviteData invite = m_PartyModule.GetPartyInvites()[i];
				if (invite)
				{
					ExpansionBookMenuTabPartyInviteEntry entry = new ExpansionBookMenuTabPartyInviteEntry(this ,invite);
					if (entry)
					{
						m_InvitesController.PartyInviteEntrys.Insert(entry);
					}
				}
			}
		}
	}
	
	void SetSelectedInvite(ExpansionPartyInviteData invite)
	{
		m_Invite = invite;
		
		option_accept_button.Show(true);
		option_decline_button.Show(true);
		invite_info_content_left.Show(true);
		
		m_InvitesController.PartyName = m_Invite.partyName;
		m_InvitesController.NotifyPropertyChanged("PartyName");
		m_InvitesController.PartyID = m_Invite.partyID.ToString();
		m_InvitesController.NotifyPropertyChanged("PartyID");
	}
	
	void OnAcceptButtonClick()
	{
		if (!m_Invite)
			return;
		
		if (!m_PartyModule)
			return;
		
		m_PartyModule.AcceptInvite(m_Invite.partyID);
		
		for (int i = 0; i < m_InvitesController.PartyInviteEntrys.Count(); ++i)
		{
			ExpansionBookMenuTabPartyInviteEntry entry = m_InvitesController.PartyInviteEntrys[i];
			if (entry)
			{
				if (entry.GetInvite() == m_Invite)
					delete entry;
			}
		}
		
		option_accept_button.Show(false);
		option_decline_button.Show(false);
		invite_info_content_left.Show(false);
		m_Invite = null;
	}
	
	void OnDeclineButtonClick()
	{
		if (!m_Invite)
		{
			return;
		}
		
		if (!m_PartyModule)
			return;
		
		m_PartyModule.DeclineInvite(m_Invite.partyID);
		
		for (int i = 0; i < m_InvitesController.PartyInviteEntrys.Count(); ++i)
		{
			ExpansionBookMenuTabPartyInviteEntry entry = m_InvitesController.PartyInviteEntrys[i];
			if (entry)
			{
				if (entry.GetInvite() == m_Invite)
					delete entry;
			}
		}
		
		option_accept_button.Show(false);
		option_decline_button.Show(false);
		invite_info_content_left.Show(false);
		m_Invite = null;
	}
	
	void OnCancelButtonClick()
	{
		Hide();
		if (GetParentTab())
			GetParentTab().Hide();
	}
		
	override void OnShow()
	{
		super.OnShow();
		
		SetView();
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
		case option_accept_button:
			option_accept_label.SetColor(ARGB(255,220,220,220));
			break;
		case option_decline_button:
			option_decline_label.SetColor(ARGB(255,220,220,220));
			break;
		case option_cancel_button:
			option_cancel_label.SetColor(ARGB(255,220,220,220));
			break;
		}
		
		return super.OnMouseEnter(w, x, y);;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
		case option_accept_button:
			option_accept_label.SetColor(ARGB(255,0,0,0));
			break;
		case option_decline_button:
			option_decline_label.SetColor(ARGB(255,0,0,0));
			break;
		case option_cancel_button:
			option_cancel_label.SetColor(ARGB(255,0,0,0));
			break;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override void OnBackButtonClick()
	{
		super.OnBackButtonClick();
		
		if (GetParentTab())
			GetParentTab().Hide();
	}
	
	override bool CanShow()
	{
		return !m_PartyModule.HasParty() && GetExpansionSettings().GetBook().EnablePartyTab;
	}
	
	override void Refresh()
	{
		LoadInvites();
	}
};

class ExpansionBookMenuTabPartyInvitesController: ExpansionViewController
{
	ref ObservableCollection<ref ExpansionBookMenuTabPartyInviteEntry> PartyInviteEntrys = new ObservableCollection<ref ExpansionBookMenuTabPartyInviteEntry>(this);
	string PartyName;
	string PartyID;
};
#endif
