/**
 * ExpansionBookMenuTabElement.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookMenuTabBase: ExpansionScriptView
{
	ref ExpansionBookMenu m_BookMenu;
	ref ExpansionBookMenuTabBase m_ParentTab;	
	
	ref array<ref ExpansionBookMenuTabBase> m_TabChildren = new array<ref ExpansionBookMenuTabBase>;
	
	void ExpansionBookMenuTabBase(ExpansionBookMenu book_menu)
	{
		m_BookMenu = book_menu;
		
		Hide();
	}
	
	string GetTabIconName()
	{
		return "Questionmark";
	}
	
	string GetTabName()
	{
		return "Tab Base";
	}
	
	int GetTabColor()
	{
		return ARGB(255,0,0,0);
	}
	
	bool IsParentTab()
	{
		return false;
	}
	
	void SetParentTab(ExpansionBookMenuTabBase tab)
	{
		m_ParentTab = tab;
	}
	
	ExpansionBookMenuTabBase GetParentTab()
	{
		return m_ParentTab;
	}
	
	void AddChildTab(ExpansionBookMenuTabBase child)
	{
		m_TabChildren.Insert(child);
	}
	
	bool IsChildTab()
	{
		return false;
	}
	
	array<ref ExpansionBookMenuTabBase> GetChildTabs()
	{
		return m_TabChildren;
	}
	
	override void OnShow()
	{
		super.OnShow();
		
		if (m_BookMenu)
		{	
			m_BookMenu.SetLastOpenedTab(this);
			m_BookMenu.PlaySwitchPageSound();
		}
	}
	
	override void OnHide()
	{
		super.OnHide();
		
		if (m_BookMenu)
			m_BookMenu.SetLastOpenedTab(NULL);
	}
	
	ExpansionBookMenu GetBookMenu()
	{
		return m_BookMenu;
	}
	
	void OnBackButtonClick()
	{
		if (m_BookMenu)
			m_BookMenu.PlaySwitchPageSound();
	}
	
	bool IgnoreBackButtonBase()
	{
		return false;
	}
	
	void SwitchMovementLockState(bool lockMovement)
	{			
		GetBookMenu().SwitchMovementLockState(lockMovement);
	}
};