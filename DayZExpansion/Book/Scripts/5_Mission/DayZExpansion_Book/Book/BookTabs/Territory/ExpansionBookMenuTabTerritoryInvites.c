/**
 * ExpansionBookMenuTabTerritoryInvites.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODBASEBUILDING
class ExpansionBookMenuTabTerritoryInvites: ExpansionBookMenuTabBase
{
	ref ExpansionBookMenuTabTerritoryInvitesController m_InvitesController;
	ref ExpansionTerritoryInvite m_Invite;
	ref ExpansionTerritoryModule m_TerritoryModule;
	
	private ButtonWidget option_accept_button;
	private TextWidget option_accept_label;
	private ButtonWidget option_decline_button;
	private TextWidget option_decline_label;
	private ButtonWidget option_cancel_button;
	private TextWidget option_cancel_label;
	private Widget invite_info_content_left;
	
	void ExpansionBookMenuTabTerritoryInvites(ExpansionBookMenu book_menu)
	{
		m_BookMenu = book_menu;
		
		if (!m_InvitesController)
			m_InvitesController = ExpansionBookMenuTabTerritoryInvitesController.Cast(GetController());
		
		if (!m_TerritoryModule)
			m_TerritoryModule = ExpansionTerritoryModule.Cast(GetModuleManager().GetModule(ExpansionTerritoryModule));
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/territory/expansion_book_tab_territory_invites.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabTerritoryInvitesController;
	}
	
	override string GetTabIconName()
	{
		return "Territory";
	}
	
	override string GetTabName()
	{
		return "#STR_EXPANSION_BOOK_TAB_TERRITORY_INVITES";
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
		
		if (m_InvitesController.TerritoryInviteEntrys.Count() > 0)
			m_InvitesController.TerritoryInviteEntrys.Clear();
		
		if (m_TerritoryModule.GetTerritoryInvites().Count() > 0)
		{
			for (int i = 0; i < m_TerritoryModule.GetTerritoryInvites().Count(); ++i)
			{
				ExpansionTerritoryInvite invite = m_TerritoryModule.GetTerritoryInvites()[i];
				if (invite)
				{
					ExpansionBookMenuTabTerritoryInviteEntry entry = new ExpansionBookMenuTabTerritoryInviteEntry(this ,invite);
					if (entry)
					{
						m_InvitesController.TerritoryInviteEntrys.Insert(entry);
					}
				}
			}
		}
	}
	
	void SetSelectedInvite(ExpansionTerritoryInvite invite)
	{
		m_Invite = invite;
		
		option_accept_button.Show(true);
		option_decline_button.Show(true);
		
		invite_info_content_left.Show(true);
		
		m_InvitesController.TerritoryName = m_Invite.TerritoryName;
		m_InvitesController.NotifyPropertyChanged("TerritoryName");
		m_InvitesController.TerritoryID = m_Invite.TerritoryID.ToString();
		m_InvitesController.NotifyPropertyChanged("TerritoryID");
	}
	
	void OnAcceptButtonClick()
	{
		if (!m_Invite)
		{
			return;
		}
		
		if (!m_TerritoryModule)
			return;
		
		m_TerritoryModule.AcceptInvite(m_Invite.TerritoryID);
		
		option_accept_button.Show(false);
		option_decline_button.Show(false);
		invite_info_content_left.Show(false);
		m_Invite = null;
		
		Hide();
	}
	
	void OnDeclineButtonClick()
	{
		if (!m_Invite)
		{
			return;
		}
		
		if (!m_TerritoryModule)
			return;
		
		
		m_TerritoryModule.DeclineInvite(m_Invite.TerritoryID);
		
		for (int i = 0; i < m_InvitesController.TerritoryInviteEntrys.Count(); ++i)
		{
			ExpansionBookMenuTabTerritoryInviteEntry entry = m_InvitesController.TerritoryInviteEntrys[i];
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
		
		Hide();
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
		
		Hide();
		if (GetParentTab())
			GetParentTab().Hide();
	}
	
	override bool CanShow()
	{
		return GetExpansionSettings().GetBook().EnableTerritoryTab;
	}
};

class ExpansionBookMenuTabTerritoryInvitesController: ExpansionViewController
{
	ref ObservableCollection<ref ExpansionBookMenuTabTerritoryInviteEntry> TerritoryInviteEntrys = new ObservableCollection<ref ExpansionBookMenuTabTerritoryInviteEntry>(this);
	string TerritoryName;
	string TerritoryID;
};
#endif
