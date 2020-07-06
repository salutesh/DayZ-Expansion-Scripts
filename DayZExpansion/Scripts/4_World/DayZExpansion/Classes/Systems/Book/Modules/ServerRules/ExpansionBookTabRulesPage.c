/**
 * ExpansionBookTabRulesPage.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/
	
class ExpansionBookTabRulesPage extends ScriptedWidgetEventHandler
{
	Widget m_Root;
	TextWidget m_TitleWidget;
	HtmlWidget m_HtmlWidget;
	
	string m_Title;
	string m_Text;
	
	// ------------------------------------------------------------
	// ExpansionBookTabRulesPage Constructor
	// ------------------------------------------------------------
	void ExpansionBookTabRulesPage(Widget parent, string title, string text)
	{
		m_Root = Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/book/expansion_book_rule_page.layout", parent) );
		
		m_TitleWidget = TextWidget.Cast( m_Root.FindAnyWidget( "page_description_label" ) );
		m_HtmlWidget = HtmlWidget.Cast( m_Root.FindAnyWidget( "page_text" ) );
		
		m_Title = title;
		m_Text = text;
		
		SetPage();
		
		m_Root.SetHandler( this );
		
		m_Root.Show( false );
	}
	
	// ------------------------------------------------------------
	// ExpansionBookTabRulesPage Destructor
	// ------------------------------------------------------------
	void ~ExpansionBookTabRulesPage()
	{
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// ExpansionBookTabRulesPage SetPage
	// ------------------------------------------------------------
	void SetPage()
	{
		m_TitleWidget.SetText( m_Title );
		m_HtmlWidget.SetText( "<html><body><p>" + m_Text + "</p></body></html>" );
	}
	
	// ------------------------------------------------------------
	// ExpansionBookTabRulesPage Show
	// ------------------------------------------------------------
	void Show()
	{
		m_Root.Show( true );
	}
	
	// ------------------------------------------------------------
	// ExpansionBookTabRulesPage Hide
	// ------------------------------------------------------------
	void Hide()
	{
		m_Root.Show( false );
	}
}