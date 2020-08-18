/**
 * ExpansionBookButtonWidget.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookButtonWidget extends ScriptedWidgetEventHandler
{
	protected Widget layoutRoot;
	protected Widget selectWidget;
	protected Widget optionWidget;
	protected TextWidget ruleButtonLabel;
	protected ButtonWidget ruleButton;
	protected ImageWidget ruleButtonIcon;
	protected ImageWidget background;
	
	protected ExpansionRuleButton m_RuleButton;
	
	protected ref ExpansionBookTabServerRules m_Menu;
	
	protected ref ExpansionUITooltip m_TooltipUI;;
	
	// ------------------------------------------------------------
	// ExpansionBookButtonWidget Constructor
	// ------------------------------------------------------------	
	void ExpansionBookButtonWidget( Widget parent, notnull ref ExpansionRuleButton button, ref ExpansionBookTabServerRules menu )
	{
		layoutRoot = Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/book/rules/expansion_book_rule_button.layout", parent ) );
		
		m_RuleButton = button;
		m_Menu = menu;

		ruleButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("expansion_rule_button") );
		ruleButtonIcon = ImageWidget.Cast( layoutRoot.FindAnyWidget( "expansion_rule_button_icon" ) );
		ruleButtonLabel = TextWidget.Cast( layoutRoot.FindAnyWidget("expansion_rule_button_label") );
		background = ImageWidget.Cast( layoutRoot.FindAnyWidget( "expansion_rule_button_image" ) );
		
		layoutRoot.SetHandler( this );
		
		SetButton();
	}
	
	// ------------------------------------------------------------
	// ExpansionBookButtonWidget Destructor
	// ------------------------------------------------------------
	void ~ExpansionBookButtonWidget()
	{
		if (m_TooltipUI && m_TooltipUI.IsVisible() )
			m_TooltipUI.HideTooltip();
		
		delete m_TooltipUI;
		delete layoutRoot;
	}
	
	// ------------------------------------------------------------
	// SetButton
	// ------------------------------------------------------------	
	private void SetButton()
	{	
		if (!m_RuleButton) return;
		
		ruleButtonLabel.SetText(m_RuleButton.DetailLabel);
		ruleButtonIcon.LoadImageFile( 0, m_RuleButton.Icon );
		background.LoadImageFile( 0, "set:book_assets image:menu_element_" + Math.RandomInt(1, 7) );
		
		if ( !m_TooltipUI && GetExpansionSettings().GetBook().ShowTooltipOnRuleButton )
			m_TooltipUI = new ExpansionUITooltip(m_RuleButton.DetailContent, m_RuleButton.DetailLabel); 
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if ( ruleButton && w == ruleButton )
		{
			m_Menu.SetRuleSection(m_RuleButton.RuleSection);
			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if ( ruleButton && w == ruleButton )
		{
			if (m_TooltipUI && !m_TooltipUI.IsVisible())
				m_TooltipUI.ShowTooltip();
			
			ruleButtonIcon.SetColor(GetExpansionSettings().GetBook().RuleButtonColor);
			ruleButtonLabel.SetColor(GetExpansionSettings().GetBook().RuleButtonColor);
			
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if ( ruleButton && w == ruleButton )
		{
			if (m_TooltipUI && m_TooltipUI.IsVisible())
				m_TooltipUI.HideTooltip();
			
			ruleButtonIcon.SetColor(ARGB(255,0,0,0));
			ruleButtonLabel.SetColor(ARGB(255,0,0,0));
			
			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// GetTooltip
	// ------------------------------------------------------------	
	ref ExpansionUITooltip GetTooltip()
	{
		return m_TooltipUI;
	}
}