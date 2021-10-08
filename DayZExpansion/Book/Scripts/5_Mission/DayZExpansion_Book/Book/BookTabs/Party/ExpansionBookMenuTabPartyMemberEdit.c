/**
 * ExpansionBookMenuTabPartyMemberEdit.c
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
class ExpansionBookMenuTabPartyMemberEdit: ExpansionBookMenuTabBase
{
	ExpansionBookMenuTabPartyMemberEditController m_MemberEditController;
	ExpansionPartyPlayerData m_Member;
	ExpansionPartyPlayerData m_Player;
	ExpansionPartyData m_Party;
	ExpansionBookMenuTabParty m_PartyTab;
	ExpansionPartyModule m_PartyModule;
	
	private ImageWidget member_status_icon;
	private PlayerPreviewWidget book_member_preview;
	private ButtonWidget option_kick_button;
	private TextWidget option_kick_label;
	private ButtonWidget option_cancel_button;
	private TextWidget option_cancel_label;
	
	private Widget member_perm_edit_spacer;
	private CheckBoxWidget member_perm_edit_checkbox;
	
	private Widget member_perm_kick_spacer;
	private CheckBoxWidget member_perm_kick_checkbox;
	
	private Widget member_perm_delete_spacer;
	private CheckBoxWidget member_perm_delete_checkbox;
	
	private Widget member_perm_invite_spacer;
	private CheckBoxWidget member_perm_invite_checkbox;

#ifdef EXPANSIONMODMARKET	
	private Widget member_perm_withdraw_spacer;
	private CheckBoxWidget member_perm_withdraw_checkbox;
#endif
	
	private TextWidget option_save_label;
	private ButtonWidget option_save_button;
	
	vector m_PlayerPreviewOrientation = vector.Zero;
	int m_PlayerPreviewRotationX = 0;
	int m_PlayerPreviewRotationY = 0;
	
	protected bool m_MouseButtonIsDown;
	
	ExpansionPartyPlayerPermissions m_Permissions;
	
	void ExpansionBookMenuTabPartyMemberEdit(ExpansionBookMenu book_menu)
	{
		m_BookMenu = book_menu;
		
		if (!m_MemberEditController)
			m_MemberEditController = ExpansionBookMenuTabPartyMemberEditController.Cast(GetController());
		
		if (!m_PartyTab)
			m_PartyTab = ExpansionBookMenuTabParty.Cast(GetParentTab());
		
		if (!m_PartyModule)
			m_PartyModule = ExpansionPartyModule.Cast(GetModuleManager().GetModule(ExpansionPartyModule));
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/party/expansion_book_tab_party_member_edit.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabPartyMemberEditController;
	}
	
	override bool IsChildTab()
	{
		return true;
	}
	
	void SetMember(ExpansionPartyPlayerData member)
	{
		m_Member = member;
	}
	
	void SetParty(ExpansionPartyData party)
	{
		m_Party = party;
	}
	
	void SetPlayer(ExpansionPartyPlayerData player)
	{
		m_Player = player;
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
	
	void SetTab()
	{
		if (!GetMemberPartyData())
			return;
	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionBookMenuTabPartyMemberEdit::SetTab - Member Can Edit: " + GetMemberPartyData().CanEdit());
		EXLogPrint("ExpansionBookMenuTabPartyMemberEdit::SetTab - Member Can Kick: " + GetMemberPartyData().CanKick());
		EXLogPrint("ExpansionBookMenuTabPartyMemberEdit::SetTab - Member Can Delete: " + GetMemberPartyData().CanDelete());
		EXLogPrint("ExpansionBookMenuTabPartyMemberEdit::SetTab - Member Can Invite: " + GetMemberPartyData().CanInvite());
		#ifdef EXPANSIONMODMARKET
		EXLogPrint("ExpansionBookMenuTabPartyMemberEdit::SetTab - Member Can Withdraw Money: " + GetMemberPartyData().CanWithdrawMoney());
		EXLogPrint("ExpansionBookMenuTabPartyMemberEdit::SetTab - Member Permissions: " + GetMemberPartyData().GetPermissions().ToString());
		#endif
		#endif
		
		if (!m_MemberEditController)
			m_MemberEditController = ExpansionBookMenuTabPartyMemberEditController.Cast(GetController());
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionBookMenuTabTerritoryMemberEdit::SetTab - Member: " + GetMemberPartyData());
		#endif
		
		m_MemberEditController.MemberName = GetMemberPartyData().GetName();
		m_MemberEditController.NotifyPropertyChanged("MemberName");
		
		if (IsPlayerOnline(GetMemberPartyData().GetID()))
		{
			SetMemberPreview(GetMemberPartyData().GetID());
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
				
		//! If editing member is owner of the party or if the player using the menu dont has edit permissions the hide the permisson checkboxes
		if (!GetPlayerPartyData().CanEdit() || GetMemberPartyData().GetID() == m_Party.GetOwnerUID())
		{
			option_kick_button.Show(false);
			member_perm_edit_spacer.Show(false);
			member_perm_kick_spacer.Show(false);
			member_perm_delete_spacer.Show(false);
			member_perm_invite_spacer.Show(false);
			option_save_button.Show(false);
			option_save_label.Show(false);
			//! No need to hide the withdraw elements as its hidden in the layout by default
		}
		else if (GetPlayerPartyData().CanEdit()) //! If player has the right to edit members then display the permisson checkboxes
		{
			member_perm_edit_checkbox.SetChecked(GetMemberPartyData().CanEdit());
			member_perm_kick_checkbox.SetChecked(GetMemberPartyData().CanKick());
			member_perm_delete_checkbox.SetChecked(GetMemberPartyData().CanDelete());
			member_perm_invite_checkbox.SetChecked(GetMemberPartyData().CanInvite());
		#ifdef EXPANSIONMODMARKET
			member_perm_withdraw_spacer.Show(true); //! Show withdraw elements when market mod is loaded
			member_perm_withdraw_checkbox.SetChecked(GetMemberPartyData().CanWithdrawMoney());
		#endif
		
			if (m_Party.GetOwnerUID() == GetMemberPartyData().GetID())
			{
				option_kick_button.Show(false);
			}
			else
			{
				option_save_button.Show(true);
				option_save_label.Show(true);
			}
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
	
	void OnKickButtonClick()
	{
		m_PartyModule.RemovePartyMember(GetMemberPartyData().GetID());
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
	
	void OnSaveButtonClick()
	{				
		//! If current member that we edit is the party owner then block all further actions
		if (GetMemberPartyData().UID == m_Party.GetOwnerUID())
		{
			return;
		}
		
		ExpansionPartyPlayerPermissions perm = GetMemberPartyData().GetPermissions();
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionBookMenuTabPartyMemberEdit::OnSaveButtonClick - Member Permissions before change: " + perm.ToString());
		#endif
		//! Edit
		if (member_perm_edit_checkbox.IsChecked())
		{
			perm |= ExpansionPartyPlayerPermissions.CAN_EDIT;
		}
		else
		{
			perm = GetMemberPartyData().RemovePermission(ExpansionPartyPlayerPermissions.CAN_EDIT);
		}
		
		//! Kick
		if (member_perm_kick_checkbox.IsChecked())
		{
			perm |= ExpansionPartyPlayerPermissions.CAN_KICK;
		}
		else
		{
			perm = GetMemberPartyData().RemovePermission(ExpansionPartyPlayerPermissions.CAN_KICK);
		}
		
		//! Delete
		if (member_perm_delete_checkbox.IsChecked())
		{
			perm |= ExpansionPartyPlayerPermissions.CAN_DELETE;
		}
		else
		{
			perm = GetMemberPartyData().RemovePermission(ExpansionPartyPlayerPermissions.CAN_DELETE);
		}
		
		//! Invite
		if (member_perm_invite_checkbox.IsChecked())
		{
			perm |= ExpansionPartyPlayerPermissions.CAN_INVITE;
		}
		else
		{
			perm = GetMemberPartyData().RemovePermission(ExpansionPartyPlayerPermissions.CAN_DELETE);
		}
		
	#ifdef EXPANSIONMODMARKET
		//! Withdraw
		if (member_perm_withdraw_checkbox.IsChecked())
		{
			if (!GetMemberPartyData().CanWithdrawMoney())
				perm |= ExpansionPartyPlayerPermissions.CAN_WITHDRAW_MONEY;
		}
		else
		{
			if (GetMemberPartyData().CanWithdrawMoney())
				perm = GetMemberPartyData().RemovePermission(ExpansionPartyPlayerPermissions.CAN_WITHDRAW_MONEY);
		}
	#endif
		
		m_PartyModule.UpdatePermissions(GetMemberPartyData().GetID(), perm);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionBookMenuTabPartyMemberEdit::OnSaveButtonClick - Member Permissions after change: " + perm.ToString());
		EXLogPrint("ExpansionBookMenuTabPartyMemberEdit::OnSaveButtonClick - Member Permissions: " + GetMemberPartyData().Permissions.ToString());
		#endif
	}
	
	override void OnShow()
	{
		super.OnShow();
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
		case option_kick_button:
			option_kick_label.SetColor(ARGB(255, 220, 220, 220));
			break;
		case option_cancel_button:
			option_cancel_label.SetColor(ARGB(255, 220, 220, 220));
			break;
		case option_save_button:
			option_save_label.SetColor(ARGB(255, 220, 220, 220));
			break;
		}
		
		return super.OnMouseEnter(w, x, y);;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
		case option_kick_button:
			option_kick_label.SetColor(ARGB(255, 0, 0, 0));
			break;
		case option_cancel_button:
			option_cancel_label.SetColor(ARGB(255, 0, 0, 0));
			break;
		case option_save_button:
			option_save_label.SetColor(ARGB(255, 0, 0, 0));
			break;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	ExpansionPartyPlayerData GetMemberPartyData()
	{
		return m_Member;
	}
	
	ExpansionPartyPlayerData GetPlayerPartyData()
	{
		return m_Player;
	}
};

class ExpansionBookMenuTabPartyMemberEditController: ExpansionViewController
{
	DayZPlayer MemberPreview;
	string MemberName;
	string MemberStatus;
};
#endif
