/**
 * ExpansionDialog_DeleteParty.c
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
class ExpansionDialog_DeleteParty: ExpansionDialogBookBase
{
	ref ExpansionDialogContent_Text m_Text;
	ref ExpansionDialogButton_Text_PartyDelete_Accept m_AcceptButton;
	ref ExpansionDialogButton_Text_PartyDelete_Cancel m_CancelButton;
		
	void ExpansionDialog_DeleteParty(ExpansionScriptView parentView)
	{
		m_ParentView = parentView;		
		if (!m_Text)
		{
			m_Text = new ExpansionDialogContent_Text(this);
			AddContent(m_Text);
			m_Text.SetText("#STR_EXPANSION_BOOK_GROUP_DELETE_GROUP");
			m_Text.SetTextColor(ARGB(255,0,0,0));
			m_Text.Show();
		}
		
		if (!m_AcceptButton)
		{
			m_AcceptButton = new ExpansionDialogButton_Text_PartyDelete_Accept(this);
			AddButton(m_AcceptButton);
			m_AcceptButton.Show();
		}
		
		if (!m_CancelButton)
		{
			m_CancelButton = new ExpansionDialogButton_Text_PartyDelete_Cancel(this);
			AddButton(m_CancelButton);
			m_CancelButton.Show();
		}
	}
	
	override string GetDialogTitle()
	{
		return "#STR_EXPANSION_BOOK_GROUP_DISSOLVE";
	}
	
	override void OnHide()
	{
		m_Text.Hide();
		m_Text = NULL;
		m_AcceptButton.Hide();
		m_AcceptButton = NULL;
		m_CancelButton.Hide();
		m_CancelButton = NULL;
		
		super.OnHide();
	}
};

class ExpansionDialogButton_Text_PartyDelete_Accept: ExpansionDialogBookButton_Text
{
	ref ExpansionDialog_DeleteParty m_DeletePartyDialog;
	ref ExpansionBookMenuTabParty m_PartyTab;
	ref ExpansionPartyModule m_PartyModule;
	
	void ExpansionDialogButton_Text_PartyDelete_Accept(ExpansionDialogBase dialog)
	{
		if (!m_DeletePartyDialog)
			m_DeletePartyDialog = ExpansionDialog_DeleteParty.Cast(GetDialog());
		
		if (!m_PartyTab)
			m_PartyTab = ExpansionBookMenuTabParty.Cast(m_DeletePartyDialog.GetParentView());
		
		if (!m_PartyModule)
			m_PartyModule = ExpansionPartyModule.Cast(CF_ModuleCoreManager.Get(ExpansionPartyModule));
		
		SetButtonText("#STR_EXPANSION_ACCEPT");
		SetTextColor(ARGB(255,0,0,0));
	}
	
	override void OnButtonClick()
	{
		if (m_PartyTab && m_PartyModule)
			m_PartyModule.DissolveParty();
		
		m_PartyTab.OnConfirmDissolve();
	}
};

class ExpansionDialogButton_Text_PartyDelete_Cancel: ExpansionDialogBookButton_Text
{
	ref ExpansionDialog_DeleteParty m_DeletePartyDialog;
	ref ExpansionBookMenuTabParty m_PartyTab;
	
	void ExpansionDialogButton_Text_PartyDelete_Cancel(ExpansionDialogBase dialog)
	{
		if (!m_DeletePartyDialog)
			m_DeletePartyDialog = ExpansionDialog_DeleteParty.Cast(GetDialog());
		
		if (!m_PartyTab)
			m_PartyTab = ExpansionBookMenuTabParty.Cast(m_DeletePartyDialog.GetParentView());
		
		SetButtonText("#STR_EXPANSION_CANCEL");
		SetTextColor(ARGB(255,0,0,0));
	}
	
	override void OnButtonClick()
	{
		m_DeletePartyDialog.Hide();
		m_PartyTab.Show();
	}
};
#endif
