/**
 * ExpansionBookMenuTabTerritoryMemberEdit.c
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
class ExpansionBookMenuTabTerritoryMemberEdit: ExpansionBookMenuTabBase
{
	ref ExpansionBookMenuTabTerritoryMemberEditController m_MemberEditController;
	ref ExpansionTerritoryMember m_Member;
	ref ExpansionTerritory m_Territory;
	ref ExpansionBookMenuTabTerritory m_TerritoryTab;
	ref ExpansionTerritoryModule m_TerritoryModule;
	
	private ImageWidget member_status_icon;
	private PlayerPreviewWidget book_member_preview;
	private ButtonWidget option_promote_button;
	private TextWidget option_promote_label;
	private ButtonWidget option_demote_button;
	private TextWidget option_demote_label;
	private ButtonWidget option_kick_button;
	private TextWidget option_kick_label;
	private ButtonWidget option_cancel_button;
	private TextWidget option_cancel_label;
	
	vector m_PlayerPreviewOrientation = vector.Zero;
	int m_PlayerPreviewRotationX = 0;
	int m_PlayerPreviewRotationY = 0;
	
	protected bool m_MouseButtonIsDown;
	
	void ExpansionBookMenuTabTerritoryMemberEdit(ExpansionBookMenu book_menu)
	{
		m_BookMenu = book_menu;
		
		if (!m_MemberEditController)
			m_MemberEditController = ExpansionBookMenuTabTerritoryMemberEditController.Cast(GetController());
		
		if (!m_TerritoryTab)
			m_TerritoryTab = ExpansionBookMenuTabTerritory.Cast(GetParentTab());
		
		if (!m_TerritoryModule)
			m_TerritoryModule = ExpansionTerritoryModule.Cast(GetModuleManager().GetModule(ExpansionTerritoryModule));
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/territory/expansion_book_tab_territory_member_edit.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabTerritoryMemberEditController;
	}
	
	override bool IsChildTab()
	{
		return true;
	}
	
	/*override void SetParentTab(ExpansionBookMenuTabBase tab)
	{
		m_ParentTab = tab;
	}*/
	
	void SetMember(ExpansionTerritoryMember member)
	{
		m_Member = member;
	}
	
	void SetTerritory(ExpansionTerritory territory)
	{
		m_Territory = territory;
	}
	
	bool IsPlayerOnline(string uid)
	{
		if (GetGame().GetPlayer().GetIdentity().GetId() == uid)
		{
			return true;
		}
		else
		{
			for (int i = 0; i < ClientData.m_PlayerList.m_PlayerList.Count(); i++)
			{
				SyncPlayer player = ClientData.m_PlayerList.m_PlayerList[i];
				if (player)
				{
					if (player.m_RUID == uid)
					{
						return true;
					}
				}
			}
		}
		
		return false;
	}
	
	void SetTab(ExpansionTerritoryMember member)
	{
		if (!member)
			return;
		
		if (!m_MemberEditController)
			m_MemberEditController = ExpansionBookMenuTabTerritoryMemberEditController.Cast(GetController());
			
		if (IsPlayerOnline(member.GetID()))
		{
			SetMemberPreview(member.GetID());
			member_status_icon.SetColor(ARGB(255, 22, 160, 133));
			m_MemberEditController.MemberStatus = "Online";
			m_MemberEditController.NotifyPropertyChanged("MemberStatus");
		} 
		else 
		{
			member_status_icon.SetColor(ARGB(255, 192, 57, 43));
			m_MemberEditController.MemberStatus = "Offline";
			m_MemberEditController.NotifyPropertyChanged("MemberStatus");
		}
		
		m_MemberEditController.MemberName = member.GetName();
		m_MemberEditController.NotifyPropertyChanged("MemberName");
		
		m_MemberEditController.MemberRank = member.GetRankName();
		m_MemberEditController.NotifyPropertyChanged("MemberRank");
		
		if (m_TerritoryTab.GetPlayerRank() == ExpansionTerritoryRank.MEMBER)
		{
			option_promote_button.Show(false);
			option_demote_button.Show(false);
			option_kick_button.Show(false);
			option_cancel_button.Show(false);
		}
	}
	
	void SetMemberPreview(string id)
	{
		DayZPlayer player = PlayerBase.GetPlayerByUID(id);
		if (player)
		{
			m_MemberEditController.MemberPreview = player;
			m_MemberEditController.NotifyPropertyChanged("MemberPreview");
		}
	}
	
	void OnPromoteButtonClick()
	{	
		if (!m_Member)
			return;
		
		if (!m_Territory)
			return;
		
		if (m_Member.GetRank() == ExpansionTerritoryRank.MEMBER)
		{
			m_TerritoryModule.PromoteMember(m_Territory.GetTerritoryID(), m_Member);				
		}
		else if (m_Member.GetRank() == ExpansionTerritoryRank.MODERATOR || m_Member.GetRank() == ExpansionTerritoryRank.ADMIN)
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_CANT_PROMOTE").Error();
		}
	}
	
	void OnDemoteButtonClick()
	{
		switch (m_Member.GetRank())
		{
		case ExpansionTerritoryRank.MEMBER:
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_CANT_DEMOTE_SELF").Error();
			break;
		case ExpansionTerritoryRank.MODERATOR:
			m_TerritoryModule.DemoteMember(m_Territory.GetTerritoryID(), m_Member);	
			break;
		case ExpansionTerritoryRank.ADMIN:
			if (m_Member.GetID() == GetGame().GetPlayer().GetIdentity().GetId())
			{
				ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_CANT_DEMOTE_SELF").Error();
			}
			else
			{
				ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_CANT_DEMOTE").Error();
			}
			break;
		}
	}
	
	void OnKickButtonClick()
	{
		if (m_Member.GetRank() != ExpansionTerritoryRank.ADMIN)
		{
			if (m_Member.GetRank() == ExpansionTerritoryRank.MODERATOR && m_TerritoryTab.GetPlayerRank() == ExpansionTerritoryRank.MODERATOR)
			{
				ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_CANT_KICK_MODERATOR").Error();
			}
			else
			{
				m_TerritoryModule.KickMember(m_Territory.GetTerritoryID(), m_Member);
			}	
		}
		else
		{
			if (m_Member.GetID() == GetGame().GetPlayer().GetIdentity().GetId())
			{
				ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_CANT_KICK_SELF").Error();
			}
		}
	}
	
	void OnCancelButtonClick()
	{
		ExpansionBookMenuTabBase tab = GetParentTab();
		Hide();
		tab.Show();
	}
	
	void UpdatePlayerPreviewRotation(int mouse_x, int mouse_y, bool is_dragging)
	{
		vector orientation = m_PlayerPreviewOrientation;
		orientation[1] = orientation[1] - (m_PlayerPreviewRotationX - mouse_x);
		orientation[2] = orientation[2] - (m_PlayerPreviewRotationY - mouse_y);
		book_member_preview.SetModelOrientation(orientation);

		if (!is_dragging)
		{
			m_PlayerPreviewOrientation = orientation;
		}
	}
	
	override void OnShow()
	{
		super.OnShow();
		
		if (m_Member)
			SetTab(m_Member);
	}
	
	override bool OnMouseButtonDown( Widget w, int x, int y, int button )
	{		
		if (w == book_member_preview && !m_MouseButtonIsDown)
		{
			//! For some reason, OnMouseButtonDown gets called twice when holding down the mouse button, which fucks with player preview rotation.
			//! Set a flag so we can check whether mouse button is still down.
			m_MouseButtonIsDown = true;
			GetGame().GetMousePos(m_PlayerPreviewRotationX, m_PlayerPreviewRotationY);
			GetGame().GetDragQueue().Call(this, "UpdatePlayerPreviewRotation");
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		m_MouseButtonIsDown = false;

		return super.OnMouseButtonUp(w, x, y, button);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
		case option_promote_button:
			option_promote_label.SetColor(ARGB(255, 220, 220, 220));
			break;
		case option_demote_button:
			option_demote_label.SetColor(ARGB(255, 220, 220, 220));
			break;
		case option_kick_button:
			option_kick_label.SetColor(ARGB(255, 220, 220, 220));
			break;
		case option_cancel_button:
			option_cancel_label.SetColor(ARGB(255, 220, 220, 220));
			break;
		}
		
		return super.OnMouseEnter(w, x, y);;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
		case option_promote_button:
			option_promote_label.SetColor(ARGB(255, 0, 0, 0));
			break;
		case option_demote_button:
			option_demote_label.SetColor(ARGB(255, 0, 0, 0));
			break;
		case option_kick_button:
			option_kick_label.SetColor(ARGB(255, 0, 0, 0));
			break;
		case option_cancel_button:
			option_cancel_label.SetColor(ARGB(255, 0, 0, 0));
			break;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
};

class ExpansionBookMenuTabTerritoryMemberEditController: ExpansionViewController
{
	DayZPlayer MemberPreview;
	string MemberName;
	string MemberRank;
	string MemberStatus;
};
#endif
