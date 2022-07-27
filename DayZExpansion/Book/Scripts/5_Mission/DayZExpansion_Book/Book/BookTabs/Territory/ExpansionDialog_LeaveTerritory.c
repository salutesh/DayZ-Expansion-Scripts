/**
 * ExpansionDialog_LeaveTerritory.c
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
class ExpansionDialog_LeaveTerritory: ExpansionDialogBookBase
{
	ref ExpansionDialogContent_Text m_Text;
	ref ExpansionDialogButton_Text_TerritroyLeave_Accept m_AcceptButton;
	ref ExpansionDialogButton_Text_TerritroyLeave_Cancel m_CancelButton;
	ref ExpansionBookMenuTabTerritory m_TerritoryTab;
	
	void ExpansionDialog_LeaveTerritory(ExpansionScriptView parentView)
	{
		m_ParentView = parentView;
		
		if (!m_TerritoryTab)
			m_TerritoryTab = ExpansionBookMenuTabTerritory.Cast(GetParentView());
		
		if (!m_Text)
		{
			m_Text = new ExpansionDialogContent_Text(this);
			AddContent(m_Text);
			m_Text.SetText("#STR_EXPANSION_BOOK_TERRITORY_LEAVE_DESC");
			m_Text.SetTextColor(ARGB(255,0,0,0));
			m_Text.Show();
		}
		
		if (!m_AcceptButton)
		{
			m_AcceptButton = new ExpansionDialogButton_Text_TerritroyLeave_Accept(this);
			AddButton(m_AcceptButton);
			m_AcceptButton.Show();
		}
		
		if (!m_CancelButton)
		{
			m_CancelButton = new ExpansionDialogButton_Text_TerritroyLeave_Cancel(this);
			AddButton(m_CancelButton);
			m_CancelButton.Show();
		}
	}
	
	override string GetDialogTitle()
	{
		return "#STR_EXPANSION_BOOK_TERRITORY_LEAVE_TERRITORY";
	}
};

class ExpansionDialogButton_Text_TerritroyLeave_Accept: ExpansionDialogBookButton_Text
{
	ref ExpansionDialog_LeaveTerritory m_LeaveTerritoryDialog;
	ref ExpansionBookMenuTabTerritory m_TerritoryTab;
	ref ExpansionTerritoryModule m_TerritoryModule;
	
	void ExpansionDialogButton_Text_TerritroyLeave_Accept(ExpansionDialogBase dialog)
	{
		if (!m_LeaveTerritoryDialog)
			m_LeaveTerritoryDialog = ExpansionDialog_LeaveTerritory.Cast(GetDialog());
		
		if (!m_TerritoryTab)
			m_TerritoryTab = ExpansionBookMenuTabTerritory.Cast(m_LeaveTerritoryDialog.GetParentView());
		
		if (!m_TerritoryModule)
			m_TerritoryModule = ExpansionTerritoryModule.Cast(CF_ModuleCoreManager.Get(ExpansionTerritoryModule));
		
		SetButtonText("#STR_EXPANSION_ACCEPT");
		SetTextColor(ARGB(255,0,0,0));
	}
	
	override void OnButtonClick()
	{
		if (m_TerritoryTab && m_TerritoryModule)
		{
			m_TerritoryModule.Leave( m_TerritoryTab.GetTerritory().GetTerritoryID() );
		}
		
		m_LeaveTerritoryDialog.Hide();
		
		ExpansionUIManager uiManager = GetDayZExpansion().GetExpansionUIManager();
		ExpansionBookMenu bookMenu = ExpansionBookMenu.Cast(uiManager.GetMenu());
		if (bookMenu)
			GetDayZExpansion().GetExpansionUIManager().CloseMenu();
	}
};

class ExpansionDialogButton_Text_TerritroyLeave_Cancel: ExpansionDialogBookButton_Text
{
	ref ExpansionDialog_LeaveTerritory m_DeleteTerritoryDialog;
	ref ExpansionBookMenuTabTerritory m_TerritoryTab;
	
	void ExpansionDialogButton_Text_TerritroyLeave_Cancel(ExpansionDialogBase dialog)
	{
		if (!m_DeleteTerritoryDialog)
			m_DeleteTerritoryDialog = ExpansionDialog_LeaveTerritory.Cast(GetDialog());
		
		if (!m_TerritoryTab)
			m_TerritoryTab = ExpansionBookMenuTabTerritory.Cast(m_DeleteTerritoryDialog.GetParentView());
		
		SetButtonText("#STR_EXPANSION_CANCEL");
		SetTextColor(ARGB(255,0,0,0));
	}
	
	override void OnButtonClick()
	{
		m_DeleteTerritoryDialog.Hide();
		m_TerritoryTab.Show();
	}
};
#endif
