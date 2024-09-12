/**
 * ExpansionBookMenuTabPartyMemberEdit.c
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
class ExpansionBookMenuTabPartyMemberEdit: ExpansionBookMenuTabBase
{
	protected ref ExpansionBookMenuTabPartyMemberEditController m_MemberEditController;
	protected ref ExpansionPartyPlayerData m_Member;
	protected ref ExpansionPartyPlayerData m_Player;
	protected ref ExpansionPartyData m_Party;
	protected ref ExpansionBookMenuTabParty m_PartyTab;
	protected ref ExpansionPartyModule m_PartyModule;
	
	protected ImageWidget member_status_icon;
	protected PlayerPreviewWidget book_member_preview;
	protected ButtonWidget option_kick_button;
	protected TextWidget option_kick_label;
	protected ButtonWidget option_cancel_button;
	protected TextWidget option_cancel_label;
	
	protected Widget member_perm_edit_spacer;
	protected CheckBoxWidget member_perm_edit_checkbox;
	
	protected Widget member_perm_kick_spacer;
	protected CheckBoxWidget member_perm_kick_checkbox;
	
	protected Widget member_perm_delete_spacer;
	protected CheckBoxWidget member_perm_delete_checkbox;
	
	protected Widget member_perm_invite_spacer;
	protected CheckBoxWidget member_perm_invite_checkbox;

#ifdef EXPANSIONMODMARKET	
	protected Widget member_perm_withdraw_spacer;
	protected CheckBoxWidget member_perm_withdraw_checkbox;
#endif
	
	protected TextWidget option_save_label;
	protected ButtonWidget option_save_button;
	
	protected SliderWidget color_picker_r;
	protected SliderWidget color_picker_g;
	protected SliderWidget color_picker_b;
	protected ImageWidget color_preview;
	
	protected vector m_PlayerPreviewOrientation = vector.Zero;
	protected int m_PlayerPreviewRotationX = 0;
	protected int m_PlayerPreviewRotationY = 0;
	
	protected bool m_MouseButtonIsDown;
	
	void ExpansionBookMenuTabPartyMemberEdit(ExpansionBookMenu book_menu)
	{
		m_BookMenu = book_menu;
		
		if (!m_MemberEditController)
			m_MemberEditController = ExpansionBookMenuTabPartyMemberEditController.Cast(GetController());
		
		if (!m_PartyModule)
			m_PartyModule = ExpansionPartyModule.Cast(CF_ModuleCoreManager.Get(ExpansionPartyModule));
	}
	
	override void SetParentTab(ExpansionBookMenuTabBase tab)
	{
		super.SetParentTab(tab);
		m_PartyTab = ExpansionBookMenuTabParty.Cast(tab);
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
	
	void SetTab()
	{
		if (!GetMemberPartyData())
		{
			HideAllAdminControls();
			return;
		}
	
		#ifdef EXPANSIONMODGROUPS_DEBUG
		EXLogPrint("ExpansionBookMenuTabPartyMemberEdit::SetTab - Member Can Edit: " + GetMemberPartyData().CanEdit());
		EXLogPrint("ExpansionBookMenuTabPartyMemberEdit::SetTab - Member Can Kick: " + GetMemberPartyData().CanKick());
		EXLogPrint("ExpansionBookMenuTabPartyMemberEdit::SetTab - Member Can Delete: " + GetMemberPartyData().CanDelete());
		EXLogPrint("ExpansionBookMenuTabPartyMemberEdit::SetTab - Member Can Invite: " + GetMemberPartyData().CanInvite());
		#ifdef EXPANSIONMODMARKET
		EXLogPrint("ExpansionBookMenuTabPartyMemberEdit::SetTab - Member Can Withdraw Money: " + GetMemberPartyData().CanWithdrawMoney());
		EXLogPrint("ExpansionBookMenuTabPartyMemberEdit::SetTab - Member Permissions: " + GetMemberPartyData().GetPermissions().ToString());
		#endif
		#endif
		
		#ifdef EXPANSIONMODGROUPS_DEBUG
		EXLogPrint("ExpansionBookMenuTabTerritoryMemberEdit::SetTab - Member: " + GetMemberPartyData());
		#endif
		
		m_MemberEditController.MemberName = GetMemberPartyData().GetName();
		m_MemberEditController.NotifyPropertyChanged("MemberName");
		
		if (PlayerBase.Expansion_IsOnline(GetMemberPartyData().GetID()))
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
		
		if ( GetPlayerPartyData().CanEdit() )
		{
			//! If player has the right to edit members then display the permisson checkboxes
			bool isOwner = GetMemberPartyData().GetID() == m_Party.GetOwnerUID();

			option_kick_button.Show(!isOwner);
			member_perm_edit_spacer.Show(!isOwner);
			member_perm_kick_spacer.Show(!isOwner);
			member_perm_delete_spacer.Show(!isOwner);
			member_perm_invite_spacer.Show(!isOwner);
		#ifdef EXPANSIONMODMARKET
			member_perm_withdraw_spacer.Show(!isOwner); //! Show withdraw elements when market mod is loaded
		#endif

			if (!isOwner)
			{
				member_perm_edit_checkbox.SetChecked(GetMemberPartyData().CanEdit());
				member_perm_kick_checkbox.SetChecked(GetMemberPartyData().CanKick());
				member_perm_delete_checkbox.SetChecked(GetMemberPartyData().CanDelete());
				member_perm_invite_checkbox.SetChecked(GetMemberPartyData().CanInvite());
			#ifdef EXPANSIONMODMARKET
				member_perm_withdraw_checkbox.SetChecked(GetMemberPartyData().CanWithdrawMoney());
			#endif
			}

			option_save_button.Show(true);
			option_save_label.Show(true);
			
			color_picker_r.Show(true);
			color_picker_g.Show(true);
			color_picker_b.Show(true);
			SetPrimaryColor(GetMemberPartyData().GetColor());
		}
		else
		{
			HideAllAdminControls();
		}
	}
	
	void HideAllAdminControls()
	{
		option_kick_button.Show(false);
		option_save_button.Show(false);
		option_save_label.Show(false);

		member_perm_edit_spacer.Show(false);
		member_perm_kick_spacer.Show(false);
		member_perm_delete_spacer.Show(false);
		member_perm_invite_spacer.Show(false);
	#ifdef EXPANSIONMODMARKET
		member_perm_withdraw_spacer.Show(false);
	#endif

		color_picker_r.Show(false);
		color_picker_g.Show(false);
		color_picker_b.Show(false);
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
		Hide();
		m_PartyTab.Show();
	}
	
	void OnCancelButtonClick()
	{
		Hide();
		m_PartyTab.Show();
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
		m_PartyModule.ChangeMemberColor(GetMemberPartyData().GetID(), color_preview.GetColor(), true);

		//! If current member that we edit is the party owner then block all further actions
		if (GetMemberPartyData().UID == m_Party.GetOwnerUID())
			return;
		
		int perm = GetMemberPartyData().GetPermissions();
		#ifdef EXPANSIONMODGROUPS_DEBUG
		EXLogPrint("ExpansionBookMenuTabPartyMemberEdit::OnSaveButtonClick - Member Permissions before change: " + perm.ToString());
		#endif
		//! Edit
		if (member_perm_edit_checkbox.IsChecked())
		{
			perm |= ExpansionPartyPlayerPermissions.CAN_EDIT;
		}
		else
		{
			perm &= ~ExpansionPartyPlayerPermissions.CAN_EDIT;
		}
		
		//! Kick
		if (member_perm_kick_checkbox.IsChecked())
		{
			perm |= ExpansionPartyPlayerPermissions.CAN_KICK;
		}
		else
		{
			perm &= ~ExpansionPartyPlayerPermissions.CAN_KICK;
		}
		
		//! Delete
		if (member_perm_delete_checkbox.IsChecked())
		{
			perm |= ExpansionPartyPlayerPermissions.CAN_DELETE;
		}
		else
		{
			perm &= ~ExpansionPartyPlayerPermissions.CAN_DELETE;
		}
		
		//! Invite
		if (member_perm_invite_checkbox.IsChecked())
		{
			perm |= ExpansionPartyPlayerPermissions.CAN_INVITE;
		}
		else
		{
			perm &= ~ExpansionPartyPlayerPermissions.CAN_INVITE;
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
				perm &= ~ExpansionPartyPlayerPermissions.CAN_WITHDRAW_MONEY;
		}
	#endif
		
		m_PartyModule.UpdatePermissions(GetMemberPartyData().GetID(), perm);
		
		#ifdef EXPANSIONMODGROUPS_DEBUG
		EXLogPrint("ExpansionBookMenuTabPartyMemberEdit::OnSaveButtonClick - Member Permissions after change: " + perm.ToString());
		EXLogPrint("ExpansionBookMenuTabPartyMemberEdit::OnSaveButtonClick - Member Permissions: " + GetMemberPartyData().Permissions.ToString());
		#endif
	}
	
	override bool OnMouseButtonDown( Widget w, int x, int y, int button )
	{		
		if (w == book_member_preview && !m_MouseButtonIsDown)
		{
			//! For some reason, OnMouseButtonDown gets called twice when holding down the mouse button, which fucks with player preview rotation.
			//! Set a flag so we can check whether mouse button is still down.
			m_MouseButtonIsDown = true;
			GetMousePos(m_PlayerPreviewRotationX, m_PlayerPreviewRotationY);
			GetGame().GetDragQueue().Call(this, "UpdatePlayerPreviewRotation");
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		m_MouseButtonIsDown = false;

		return super.OnMouseButtonUp(w, x, y, button);
	}

	bool IsRGBSlider(Widget w)
	{
		return w == color_picker_r || w == color_picker_g || w == color_picker_b;
	}

	void SetPrimaryColor(int color)
	{
		if (color_picker_r && color_picker_g && color_picker_b)
		{
			int a = (color >> 24) & 0xFF;
			int r = (color >> 16) & 0xFF;
			int g = (color >> 8) & 0xFF;
			int b = (color)&0xFF;

			color_picker_r.SetCurrent(r);
			color_picker_g.SetCurrent(g);
			color_picker_b.SetCurrent(b);
		}

		color_preview.SetColor(color);
	}

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if (w != NULL && IsRGBSlider(w))
		{
			SetPrimaryColor(ARGB(255, color_picker_r.GetCurrent(), color_picker_g.GetCurrent(), color_picker_b.GetCurrent()));

			return true;
		}

		return super.OnChange(w, x, y, finished);
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
