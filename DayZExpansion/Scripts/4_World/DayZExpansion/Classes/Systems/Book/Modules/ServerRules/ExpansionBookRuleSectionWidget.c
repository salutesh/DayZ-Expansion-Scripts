/**
 * ExpansionBookRuleSectionWidget.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookRuleSectionWidget extends ScriptedWidgetEventHandler
{
	protected ref ExpansionRuleSection m_RuleSection;
	protected ref ExpansionBookTabServerRules m_Menu;
	
	protected Widget layoutRoot;
	protected TextWidget m_Title;
	protected GridSpacerWidget m_Content;
	
	void ExpansionBookRuleSectionWidget( Widget parent, notnull ref ExpansionRuleSection section, ref ExpansionBookTabServerRules menu )
	{
		layoutRoot = Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/book/rules/expansion_book_rule_section.layout", parent ) );
		
		m_RuleSection = section;
		m_Menu = menu;

		m_Title = TextWidget.Cast(layoutRoot.FindAnyWidget("expansion_rule_section_text"));
		m_Content = GridSpacerWidget.Cast(layoutRoot.FindAnyWidget("expansion_options_category_content"));
		
		layoutRoot.SetHandler( this );
		
		SetRuleSection();
	}
	
	void ~ExpansionBookRuleSectionWidget()
	{
		delete layoutRoot;
	}
	
	private void SetRuleSection()
	{
		if (!m_RuleSection) return;
		
		m_Title.SetText(m_RuleSection.DetailLabel);
		
		for ( int i = 0; i < m_RuleSection.Rules.Count(); i++ )
		{
			Widget ruleWidget = Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/book/rules/expansion_book_rule.layout", m_Content ) );
			TextWidget ruleParagraph = TextWidget.Cast( ruleWidget.FindAnyWidget( "expansion_rule_paragraph" ) );
			ruleParagraph.SetText( (i + 1).ToString() + "." );
			ruleParagraph.SetColor(GetExpansionSettings().GetBook().RuleParagraphColor);
			TextWidget ruleText = TextWidget.Cast( ruleWidget.FindAnyWidget( "expansion_rule_text" ) );
			ruleText.SetText( m_RuleSection.Rules[i].DetailContent );
		}
	}
}