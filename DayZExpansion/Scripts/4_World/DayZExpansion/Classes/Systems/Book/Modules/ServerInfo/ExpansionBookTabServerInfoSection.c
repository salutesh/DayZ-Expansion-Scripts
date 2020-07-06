/**
 * ExpansionBookTabServerInfoSectionData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookTabServerInfoSection extends ScriptedWidgetEventHandler
{
	Widget m_Root;
	TextWidget m_Title;
	HtmlWidget m_Content;
	
	bool m_HasHeading;
	string m_TitleText;
	string m_ContentText;
	
	// ------------------------------------------------------------
	// ExpansionBookTabServerInfoSection Constructor
	// ------------------------------------------------------------
	void ExpansionBookTabServerInfoSection(Widget parent, bool hasheading, string title, string text)
	{
		m_Root	= Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/book/expansion_book_text_section.layout", parent ) );
		m_Title = TextWidget.Cast( m_Root.FindAnyWidget( "Title" ) );
		m_Content = HtmlWidget.Cast( m_Root.FindAnyWidget( "Content" ) );
		
		m_HasHeading = hasheading;
		
		if ( m_HasHeading )
			m_TitleText = title;
		else
			m_TitleText = "";
		
		m_ContentText = text;
		
		SetSection();
		
		m_Root.SetHandler( this );
	}
	
	// ------------------------------------------------------------
	// ExpansionBookTabServerInfoSection Destructor
	// ------------------------------------------------------------
	void ~ExpansionBookTabServerInfoSection()
	{
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// ExpansionBookTabServerInfoSectionData SetButton
	// ------------------------------------------------------------
	void SetSection()
	{
		if (!m_HasHeading)
		{
			m_Title.Show( false );
		}
		else
		{
			m_Title.SetText( m_TitleText );	
		}
		
		m_Content.SetText( "<html><body>" + m_ContentText + "</body></html>" );
	}
}