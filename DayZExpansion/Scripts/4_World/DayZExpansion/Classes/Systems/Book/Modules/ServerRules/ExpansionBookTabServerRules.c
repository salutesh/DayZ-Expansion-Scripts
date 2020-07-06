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
	protected Widget m_RulesMainPanel;
	protected TextWidget m_ServerName;
	protected WrapSpacerWidget m_RulesListContent;
	
	protected Widget m_RulesPagesPanel;
	protected WrapSpacerWidget m_Book_Spacer_Left;
	protected WrapSpacerWidget m_Book_Spacer_Right;
	
	protected Widget m_PageSwitchButtonsPanel;
	
	protected ref array<ref ExpansionBookTabRulesEntry> m_RulesEntrys;
	protected ref ExpansionServerRulesSection m_CurrentSection;
	protected ref ExpansionServerRulesPageSection m_CurrentPageSection;
	protected ref array<ref ExpansionBookTabRulesPage> m_CurrentPages;
	
	static int m_CurrentPage = 0;
	static int LastPageOpened = 1;
	static const int NumberOfPages = 2;
	static const int NumberOfPagesOnOnePage = 1;
	
	// ------------------------------------------------------------
	// ExpansionBookTabServerRules Contructor
	// ------------------------------------------------------------
	void ExpansionBookTabServerRules()
	{
		SetButtonIconPath( "set:expansion_iconset image:icon_info" );
		SetButtonText( "SERVER RULES" );
		SetButtonColor( ARGB( 255, 26, 188, 156 ) );
		SetLayoutPath( "DayZExpansion/GUI/layouts/book/expansion_book_page_serverrules.layout" );
		
		m_RulesEntrys = new array<ref ExpansionBookTabRulesEntry>;
		m_CurrentPages = new array<ref ExpansionBookTabRulesPage>;
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
		m_RulesMainPanel = Widget.Cast( m_RootLayout.FindAnyWidget( "book_rules_main_tab" ) );
		m_ServerName = TextWidget.Cast( m_RootLayout.FindAnyWidget( "rules_main_server_name" ) );
		m_RulesListContent = WrapSpacerWidget.Cast( m_RootLayout.FindAnyWidget( "rules_main_rules_list_content" ) );
		
		m_RulesPagesPanel = Widget.Cast( m_RootLayout.FindAnyWidget( "book_rules_tab" ) );
		m_Book_Spacer_Left = WrapSpacerWidget.Cast( m_RootLayout.FindAnyWidget( "rules_content_left" ) );
		m_Book_Spacer_Right = WrapSpacerWidget.Cast( m_RootLayout.FindAnyWidget( "rules_content_right" ) );
		
		m_PageSwitchButtonsPanel = Widget.Cast( m_RootLayout.FindAnyWidget( "book_page_frame" ) ); 
		
		Init();
	}
	
	// ------------------------------------------------------------
	// Init
	// ------------------------------------------------------------
	void Init()
	{		
		if ( GetExpansionSettings().GetBook().ServerRules.RuleSections.Count() > 0 )
		{
			//! Create rule section entrys from settings
			for (int i = 0; i < GetExpansionSettings().GetBook().ServerRules.RuleSections.Count(); ++i)
			{
				if (m_RulesEntrys && m_RulesEntrys.Count() > 0)
					m_RulesEntrys.Clear();
				
				ExpansionBookTabRulesEntry entry = new ExpansionBookTabRulesEntry(m_RulesListContent, this, GetExpansionSettings().GetBook().ServerRules.RuleSections[i]);
				m_RulesEntrys.Insert( entry );
			}
		}
		
		if ( GetExpansionSettings().GetBook().ServerInfo.ServerName )
		{
			m_ServerName.SetText( GetExpansionSettings().GetBook().ServerInfo.ServerName );
		}
	}
		
	// ------------------------------------------------------------
	// SetRuleSection
	// Events called when a rule section entry is clicked
	// ------------------------------------------------------------
	void SetRuleSection(ExpansionServerRulesSection section)
	{
		ShowRuleSection();
		
		m_CurrentSection = section;
		m_CurrentPageSection = m_CurrentSection.GetPageSection( 0 );
		
		Print( "ExpansionBookTabServerRules::SetRuleSection - m_CurrentPageSection: " + m_CurrentPageSection.ToString() );
		
		Refresh();
		
		if ( m_CurrentSection.NumberOfPages() > 2 )
			m_PageSwitchButtonsPanel.Show( true );
	}
	
	// ------------------------------------------------------------
	// Refresh
	// ------------------------------------------------------------
	void Refresh()
	{
		if (m_CurrentPages.Count() > 0)
			m_CurrentPages.Clear();
		
		if (m_CurrentPageSection.Left != -1)
		{
			ExpansionServerRulesPage left_page =  m_CurrentSection.GetPage( m_CurrentPageSection.Left );
			ExpansionBookTabRulesPage left = new ExpansionBookTabRulesPage(m_Book_Spacer_Left, left_page.HeadText, left_page.BodyText );
			m_CurrentPages.Insert(left);
		}
		
		if (m_CurrentPageSection.Right != -1)
		{
			ExpansionServerRulesPage right_page =  m_CurrentSection.GetPage( m_CurrentPageSection.Right );
			ExpansionBookTabRulesPage right = new ExpansionBookTabRulesPage(m_Book_Spacer_Right, right_page.HeadText, right_page.BodyText );
			m_CurrentPages.Insert(right);
		}
	}
	
	// ------------------------------------------------------------
	// ShowRuleSection
	// ------------------------------------------------------------
	void ShowRuleSection()
	{
		m_RulesMainPanel.Show( false );
		m_RulesPagesPanel.Show( true );
	}
	
	// ------------------------------------------------------------
	// ShowMain
	// ------------------------------------------------------------
	void ShowMain()
	{
		m_RulesMainPanel.Show( true );
		m_RulesPagesPanel.Show( false );
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