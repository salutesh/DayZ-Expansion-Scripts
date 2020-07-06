/**
 * ExpansionBookTabRulesEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookTabRulesEntry extends ScriptedWidgetEventHandler
{
	Widget m_Root;
	TextWidget m_TextWidget;
	ButtonWidget m_Button;
	
	string m_Lable;
	ref ExpansionServerRulesSection m_RulesSection;
	ExpansionBookTabServerRules m_BookTabServerRules;
	
	// ------------------------------------------------------------
	// ExpansionBookTabRulesEntry Constructor
	// ------------------------------------------------------------
	void ExpansionBookTabRulesEntry(Widget parent,ExpansionBookTabServerRules tab, ExpansionServerRulesSection section)
	{
		m_Root = Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/book/expansion_book_menu_element.layout", parent ) );
		m_TextWidget = TextWidget.Cast( m_Root.FindAnyWidget("book_menu_label") );
		m_Button = ButtonWidget.Cast( m_Root.FindAnyWidget("book_menu_button") );
		
		m_RulesSection = section;
		m_BookTabServerRules = tab;
		
		m_Root.SetHandler( this );
		
		SetEntry();
	}
	
	// ------------------------------------------------------------
	// ExpansionBookTabRulesPage Destructor
	// ------------------------------------------------------------
	void ~ExpansionBookTabRulesEntry()
	{
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// ExpansionBookTabRulesPage SetEntry
	// ------------------------------------------------------------
	void SetEntry()
	{
		if (m_RulesSection)
		{
			m_TextWidget.SetText( m_RulesSection.Lable );
		}
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if ( m_Button && w == m_Button )
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionBookTabRulesPage::OnClick - Clicked m_Button");
			#endif
			
			m_BookTabServerRules.SetRuleSection( m_RulesSection );
			
			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if ( m_Button && w == m_Button )
		{
			
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if ( m_Button && w == m_Button )
		{

			return true;
		}

		return false;
	}
}