/**
 * ExpansionBookMenuTabRulesRuleElement.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookMenuTabRulesRuleElement: ExpansionScriptView
{
	ref ExpansionBookMenuTabRulesRuleElementController m_EntryController;
	ref ExpansionBookRule m_Rule;
	ref ExpansionBookMenuTabRules m_RulesTab;

	RichTextWidget rule_text;
	
	void ExpansionBookMenuTabRulesRuleElement(ExpansionBookMenuTabRules tab, ExpansionBookRule rule)
	{
		m_RulesTab = tab;
		m_Rule = rule;
		
		if (!m_EntryController)
			m_EntryController = ExpansionBookMenuTabRulesRuleElementController.Cast(GetController());
		
		SetView();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/tabs/rules/expansion_book_tab_rules_rule.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuTabRulesRuleElementController;
	}
	
	void SetView()
	{
		m_EntryController.RuleParagraph = m_Rule.RuleParagraph;
		m_EntryController.NotifyPropertyChanged("RuleParagraph");
		
		m_EntryController.RuleText = m_Rule.RuleText;
		m_EntryController.NotifyPropertyChanged("RuleText");

		g_Game.GetCallQueue(CALL_CATEGORY_GUI).CallLater(SetRuleSize, 34, false);  //! DayZ 1.30 needs this in next frame else richtext doesn't size to fit contents
	}

	void SetRuleSize()
	{
		float width, height;
		rule_text.GetSize(width, height);

		rule_text.SetSize(width, rule_text.GetContentHeight());
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		return super.OnMouseEnter(w, x, y);;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		return super.OnMouseLeave(w, enterW, x, y);
	}
};

class ExpansionBookMenuTabRulesRuleElementController: ExpansionViewController
{
	string RuleParagraph;
	string RuleText;
};