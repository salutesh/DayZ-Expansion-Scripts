/**
 * ExpansionBookMenuTabRules.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookMenuTabRules: ExpansionBookMenuTabBase
{
	ref ExpansionBookMenuTabRulesController m_RulesTabController;
	private ScrollWidget rules_list_scroller;
	
	void ExpansionBookMenuTabRules(ExpansionBookMenu book_menu)
	{
		m_BookMenu = book_menu;
		
		if (!m_RulesTabController)
			m_RulesTabController = ExpansionBookMenuTabRulesController.Cast(GetController());
		
		SetView();
	}
		
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/rules/expansion_book_tab_rules.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabRulesController;
	}
	
	override string GetTabIconName()
	{
		return "Book 1";
	}
	
	override string GetTabName()
	{
		return "#STR_EXPANSION_BOOK_TAB_SERVER_RULES";
	}
	
	override int GetTabColor()
	{
		return ARGB(255,0,0,0);
	}
	
	override bool CanShow()
	{
		return GetExpansionSettings().GetBook().EnableServerRulesTab;
	}
	
	override void OnShow()
	{
		super.OnShow();
	}
	
	override void OnHide()
	{
		super.OnHide();
	}
	
	override bool IsParentTab()
	{
		return true;
	}
	
	void SetView()
	{		
		//! Add setting category elements and there setting entries
		if (GetExpansionSettings().GetBook().RuleCategories.Count() == 0)
			return;
		
		m_RulesTabController.RulesCategories.Clear();
			
		for (int i = 0; i < GetExpansionSettings().GetBook().RuleCategories.Count(); i++)
		{
			AddRuleCategory(GetExpansionSettings().GetBook().RuleCategories[i]);
		}
	}
	
	void AddRuleCategory(ExpansionBookRuleCategory category)
	{
		ExpansionBookMenuTabRulesCategoryEntry entry = new ExpansionBookMenuTabRulesCategoryEntry(this, category);
		m_RulesTabController.RulesCategories.Insert(entry);
	}
	
	void SetCategory(ExpansionBookRuleCategory category)
	{
		if (category.Rules.Count() == 0)
			return;
		
		m_RulesTabController.Rules.Clear();
		
		for (int i = 0; i < category.Rules.Count(); i++)
		{
			AddRule(category.Rules[i]);
		}
		
		m_BookMenu.PlaySwitchPageSound();
	}
		
	void AddRule(ExpansionBookRule rule)
	{		
		ExpansionBookMenuTabRulesRuleElement entry = new ExpansionBookMenuTabRulesRuleElement(this, rule);
		m_RulesTabController.Rules.Insert(entry);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		return super.OnMouseEnter(w, x, y);;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override void OnBackButtonClick()
	{
		super.OnBackButtonClick();
		
		Hide();
	}
};

class ExpansionBookMenuTabRulesController: ExpansionViewController 
{
	ref ObservableCollection<ref ExpansionBookMenuTabRulesCategoryEntry> RulesCategories = new ObservableCollection<ref ExpansionBookMenuTabRulesCategoryEntry>(this);
	ref ObservableCollection<ref ExpansionBookMenuTabRulesRuleElement> Rules = new ObservableCollection<ref ExpansionBookMenuTabRulesRuleElement>(this);
};