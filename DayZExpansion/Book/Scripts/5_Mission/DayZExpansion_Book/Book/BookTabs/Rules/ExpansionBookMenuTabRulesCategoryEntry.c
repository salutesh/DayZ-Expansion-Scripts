/**
 * ExpansionBookMenuTabRulesCategoryEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookMenuTabRulesCategoryEntry: ExpansionScriptView
{
	ref ExpansionBookMenuTabRulesCategoryEntryController m_EntryController;
	ref ExpansionBookRuleCategory m_Category;
	ref ExpansionBookMenuTabRules m_RulesTab;
	
	private ButtonWidget category_entry_button;
	private TextWidget category_entry_label;
	private ImageWidget category_entry_icon;
	
	void ExpansionBookMenuTabRulesCategoryEntry(ExpansionBookMenuTabRules tab, ExpansionBookRuleCategory category)
	{
		m_RulesTab = tab;
		m_Category = category;
		
		if (!m_EntryController)
			m_EntryController = ExpansionBookMenuTabRulesCategoryEntryController.Cast(GetController());
		
		SetView();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/rules/expansion_book_tab_rules_category_entry.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabRulesCategoryEntryController;
	}
	
	void SetView()
	{
		m_EntryController.CategoryName = m_Category.CategoryName;
		m_EntryController.NotifyPropertyChanged("CategoryName");
	}
	
	void OnEntryButtonClick()
	{
		m_RulesTab.SetCategory(m_Category);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
		case category_entry_button:
			category_entry_icon.SetColor(ARGB(255,220,220,220));
			category_entry_label.SetColor(ARGB(255,220,220,220));
			break;
		}
		
		return super.OnMouseEnter(w, x, y);;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
		case category_entry_button:
			category_entry_icon.SetColor(ARGB(255,0,0,0));
			category_entry_label.SetColor(ARGB(255,0,0,0));
			break;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
};

class ExpansionBookMenuTabRulesCategoryEntryController: ExpansionViewController
{
	string CategoryName;
};