/**
 * ExpansionBookTabServerRules.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookTabServerRules extends ExpansionBookTabBase
{	
	protected Widget m_RulesPagesPanel;
	protected WrapSpacerWidget m_Book_Spacer_Left;
	protected WrapSpacerWidget m_Book_Spacer_Right;
	
	protected ref array<ref ExpansionBookButtonWidget> m_ButtonWidgets;
	
	protected ref ExpansionBookRuleSectionWidget m_CurrentSection;
	
	// ------------------------------------------------------------
	// ExpansionBookTabServerRules Contructor
	// ------------------------------------------------------------
	void ExpansionBookTabServerRules()
	{
		SetButtonIconPath( ExpansionIcons.GetPath("Questionmark") );
		SetButtonText( "SERVER RULES" );
		SetButtonColor( ARGB( 255, 155, 89, 182 ) );
		SetLayoutPath( "DayZExpansion/GUI/layouts/book/expansion_book_page_serverrules.layout" );
		
		if ( !m_ButtonWidgets )
			m_ButtonWidgets = new array<ref ExpansionBookButtonWidget>;
		else
			m_ButtonWidgets.Clear();
	}
	
	// ------------------------------------------------------------
	// Override CanBeShow
	// ------------------------------------------------------------
	override bool CanBeShow()
	{
		return ( GetExpansionSettings().GetBook().EnableServerRulesTab );
	}
	
	// ------------------------------------------------------------
	// Override OnCreatedLayout
	// ------------------------------------------------------------
	//Layout root var is m_RootLayout
	override void OnCreatedLayout()
	{
		m_RulesPagesPanel = Widget.Cast( m_RootLayout.FindAnyWidget( "book_rules_tab" ) );
		m_Book_Spacer_Left = WrapSpacerWidget.Cast( m_RootLayout.FindAnyWidget( "rules_content_left" ) );
		m_Book_Spacer_Right = WrapSpacerWidget.Cast( m_RootLayout.FindAnyWidget( "rules_content_right" ) );
		
		CreateRules();
	}
	
	// ------------------------------------------------------------
	// CreateRules
	// ------------------------------------------------------------
	private void CreateRules()
	{
		for ( int i = 0; i < GetExpansionSettings().GetBook().RuleCategories.Count(); i++ )
		{
			ref ExpansionRulesCategory category = GetExpansionSettings().GetBook().RuleCategories[i];
			Widget categoryWidget = Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/book/rules/expansion_book_rule_section.layout", m_Book_Spacer_Left ) );
			TextWidget categoryLable = TextWidget.Cast( categoryWidget.FindAnyWidget( "expansion_rule_section_text" ) );
			categoryLable.SetText( category.DisplayName );
			GridSpacerWidget categoryGrid = GridSpacerWidget.Cast( categoryWidget.FindAnyWidget( "expansion_options_category_content" ) );
			
			for ( int j = 0; j < category.RuleButtons.Count(); j++ )
			{
				ExpansionBookButtonWidget button = new ExpansionBookButtonWidget( categoryGrid, category.RuleButtons[j], this );
				
				if ( button )
				{
					m_ButtonWidgets.Insert( button );
				}
			}
		}
	}
	
	// ------------------------------------------------------------
	// SetRuleSection
	// ------------------------------------------------------------	
	void SetRuleSection(ref ExpansionRuleSection section)
	{
		if ( !section ) return;

		if ( m_CurrentSection )
		{
			delete m_CurrentSection;
			m_CurrentSection = null;
		}
		
		ExpansionBookRuleSectionWidget sectionWidget = new ExpansionBookRuleSectionWidget(m_Book_Spacer_Right, section, this);
		m_CurrentSection = sectionWidget;
	}
	
	// ------------------------------------------------------------
	// OnTabClose
	// ------------------------------------------------------------	
	override void OnTabClose()
	{			
		if ( GetExpansionSettings().GetBook().ShowTooltipOnRuleButton )
			HideTooltips();
		
		super.OnTabClose();
	}
	
	// ------------------------------------------------------------
	// HideTooltips
	// ------------------------------------------------------------	
	void HideTooltips()
	{
		if (m_ButtonWidgets && m_ButtonWidgets.Count() > 0)
		{
			for (int i = 0; i < m_ButtonWidgets.Count(); i++)
			{
				if ( m_ButtonWidgets[i].GetTooltip().IsVisible() )
					m_ButtonWidgets[i].GetTooltip().HideTooltip();
			}
		}
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick( Widget w, int x, int y, int button )
	{
		super.OnClick(w, x, y, button);

		return false;
	}
	
	// ------------------------------------------------------------
	// Override IsFocusable
	// ------------------------------------------------------------
	override bool IsFocusable( Widget w )
	{
		return false;
	}
}