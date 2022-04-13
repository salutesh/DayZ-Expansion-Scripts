/**
 * ExpansionDialog_DeleteTerritory.c
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
class ExpansionDialog_InviteParty: ExpansionDialogBookBase
{
	ref ExpansionDialogContent_Text m_Text;
	ref ExpansionDialogButton_Text_PartyInvite_Accept m_AcceptButton;
	ref ExpansionDialogButton_Text_PartyInvite_Cancel m_CancelButton;	
	ref ExpansionBookMenuTabParty m_PartyTab;
		
	void ExpansionDialog_InviteParty(ExpansionScriptView parentView)
	{
		m_ParentView = parentView;
		
		if (!m_PartyTab)
			m_PartyTab = ExpansionBookMenuTabParty.Cast(GetParentView());
		
		if (!m_Text)
		{
			m_Text = new ExpansionDialogContent_Text(this);
			AddContent(m_Text);
			m_Text.SetText("#STR_EXPANSION_BOOK_GROUP_INVITE_PLAYER_DESC");
			m_Text.SetTextColor(ARGB(255,0,0,0));
			m_Text.Show();
		}
		
		if (!m_AcceptButton)
		{
			m_AcceptButton = new ExpansionDialogButton_Text_PartyInvite_Accept(this);
			AddButton(m_AcceptButton);
			m_AcceptButton.Show();
		}
		
		if (!m_CancelButton)
		{
			m_CancelButton = new ExpansionDialogButton_Text_PartyInvite_Cancel(this);
			AddButton(m_CancelButton);
			m_CancelButton.Show();
		}
	}
	
	override string GetDialogTitle()
	{
		return "#STR_EXPANSION_BOOK_GROUP_INVITE";
	}
};

class ExpansionDialogButton_Text_PartyInvite_Accept: ExpansionDialogBookButton_Text
{
	ref ExpansionDialog_InviteParty m_PartyInviteDialog;
	ref ExpansionBookMenuTabParty m_PartyTab;
	ref ExpansionPartyModule m_PartyModule;
	
	void ExpansionDialogButton_Text_PartyInvite_Accept(ExpansionDialogBase dialog)
	{
		if (!m_PartyInviteDialog)
			m_PartyInviteDialog = ExpansionDialog_InviteParty.Cast(GetDialog());
		
		if (!m_PartyTab)
			m_PartyTab = ExpansionBookMenuTabParty.Cast(m_PartyInviteDialog.GetParentView());
		
		if (!m_PartyModule)
			m_PartyModule = ExpansionPartyModule.Cast(CF_ModuleCoreManager.Get(ExpansionPartyModule));
		
		SetButtonText("#STR_EXPANSION_ACCEPT");
		SetTextColor(ARGB(255,0,0,0));
	}
	
	override void OnButtonClick()
	{
		if (m_PartyTab && m_PartyModule)
		{
			m_PartyModule.InvitePlayer(m_PartyTab.GetSelectedPlayer().m_RUID);
		}
				
		m_PartyInviteDialog.Hide();
	}
};

class ExpansionDialogButton_Text_PartyInvite_Cancel: ExpansionDialogBookButton_Text
{
	ref ExpansionDialog_InviteParty m_PartyInviteDialog;
	ref ExpansionBookMenuTabParty m_PartyTab;
	
	void ExpansionDialogButton_Text_PartyInvite_Cancel(ExpansionDialogBase dialog)
	{
		if (!m_PartyInviteDialog)
			m_PartyInviteDialog = ExpansionDialog_InviteParty.Cast(GetDialog());
		
		if (!m_PartyTab)
			m_PartyTab = ExpansionBookMenuTabParty.Cast(m_PartyInviteDialog.GetParentView());
		
		SetButtonText("#STR_EXPANSION_CANCEL");
		SetTextColor(ARGB(255,0,0,0));
	}
	
	override void OnButtonClick()
	{
		m_PartyInviteDialog.Hide();
		m_PartyTab.Show();
	}
};
#endif
