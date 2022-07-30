/**
 * ExpansionDialog_InviteTerritory.c
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
class ExpansionDialog_InviteTerritory: ExpansionDialogBookBase
{
	ref ExpansionDialogContent_Text m_Text;
	ref ExpansionDialogButton_Text_TerritroyInvite_Accept m_AcceptButton;
	ref ExpansionDialogButton_Text_TerritroyInvite_Cancel m_CancelButton;	
	ref ExpansionBookMenuTabTerritory m_TerritoryTab;
		
	void ExpansionDialog_InviteTerritory(ExpansionScriptView parentView)
	{
		m_ParentView = parentView;
		
		if (!m_TerritoryTab)
			m_TerritoryTab = ExpansionBookMenuTabTerritory.Cast(GetParentView());
		
		if (!m_Text)
		{
			m_Text = new ExpansionDialogContent_Text(this);		
			AddContent(m_Text);
			m_Text.SetText("#STR_EXPANSION_BOOK_TERRITORY_INVITE_PLAYER");
			m_Text.SetTextColor(ARGB(255,0,0,0));
			m_Text.Show();
		}
		
		if (!m_AcceptButton)
		{
			m_AcceptButton = new ExpansionDialogButton_Text_TerritroyInvite_Accept(this);
			AddButton(m_AcceptButton);
			m_AcceptButton.Show();
		}
		
		if (!m_CancelButton)
		{
			m_CancelButton = new ExpansionDialogButton_Text_TerritroyInvite_Cancel(this);
			AddButton(m_CancelButton);
			m_CancelButton.Show();
		}
	}
	
	override string GetDialogTitle()
	{
		return "#STR_EXPANSION_BOOK_GROUP_INVITE";
	}
};

class ExpansionDialogButton_Text_TerritroyInvite_Accept: ExpansionDialogBookButton_Text
{
	ref ExpansionDialog_InviteTerritory m_TerritoryInviteDialog;
	ref ExpansionBookMenuTabTerritory m_TerritoryTab;
	ref ExpansionTerritoryModule m_TerritoryModule;
	
	void ExpansionDialogButton_Text_TerritroyInvite_Accept(ExpansionDialogBase dialog)
	{
		if (!m_TerritoryInviteDialog)
			m_TerritoryInviteDialog = ExpansionDialog_InviteTerritory.Cast(GetDialog());
		
		if (!m_TerritoryTab)
			m_TerritoryTab = ExpansionBookMenuTabTerritory.Cast(m_TerritoryInviteDialog.GetParentView());
		
		if (!m_TerritoryModule)
			m_TerritoryModule = ExpansionTerritoryModule.Cast(CF_ModuleCoreManager.Get(ExpansionTerritoryModule));
		
		SetButtonText("#STR_EXPANSION_ACCEPT");
		SetTextColor(ARGB(255,0,0,0));
	}
	
	override void OnButtonClick()
	{
		if (m_TerritoryTab && m_TerritoryModule)
		{
			m_TerritoryModule.RequestInvitePlayer(m_TerritoryTab.GetSelectedPlayer().m_RUID, m_TerritoryTab.GetFlag());
		}
		
		m_TerritoryInviteDialog.Hide();
	}
};

class ExpansionDialogButton_Text_TerritroyInvite_Cancel: ExpansionDialogBookButton_Text
{
	ref ExpansionDialog_InviteTerritory m_TerritoryInviteDialog;
	ref ExpansionBookMenuTabTerritory m_TerritoryTab;
	
	void ExpansionDialogButton_Text_TerritroyInvite_Cancel(ExpansionDialogBase dialog)
	{
		if (!m_TerritoryInviteDialog)
			m_TerritoryInviteDialog = ExpansionDialog_InviteTerritory.Cast(GetDialog());
		
		if (!m_TerritoryTab)
			m_TerritoryTab = ExpansionBookMenuTabTerritory.Cast(m_TerritoryInviteDialog.GetParentView());
		
		SetButtonText("#STR_EXPANSION_CANCEL");
		SetTextColor(ARGB(255,0,0,0));
	}
	
	override void OnButtonClick()
	{
		m_TerritoryInviteDialog.Hide();
		m_TerritoryTab.Show();
	}
};
#endif
