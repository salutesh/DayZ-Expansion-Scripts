/**
 * ExpansionDirectConnectTab.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionDirectConnectTab extends ServerBrowserTab
{
	protected Widget										m_IPSetting;
	protected EditBoxWidget 								m_IPEditbox;
	protected Widget										m_PortSetting;
	protected EditBoxWidget 								m_PortEditbox;
	protected Widget										m_PasswordSetting;
	protected EditBoxWidget 								m_PasswordEditbox;
	protected string 										m_ServerIP;
	protected int 											m_ServerPort;
	protected string 										m_ServerPassword;
	
	// ------------------------------------------------------------
	// ExpansionDirectConnectTab Constructor
	// ------------------------------------------------------------
	void ExpansionDirectConnectTab( Widget parent, ServerBrowserMenuNew menu, TabType type )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionDirectConnectTab::ExpansionDirectConnectTab - Start");
		#endif

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionDirectConnectTab::ExpansionDirectConnectTab - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override Construct
	// ------------------------------------------------------------
	protected override void Construct( Widget parent, ServerBrowserMenuNew menu, TabType type )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionDirectConnectTab::Construct - Start");
		#endif
		m_Root = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/ui/server_browser/expansion_server_browser_tab_direct.layout", parent );
	
		m_IPSetting				= Widget.Cast( m_Root.FindAnyWidget( "direct_ip_setting" ) );
		m_IPEditbox 			= EditBoxWidget.Cast( m_Root.FindAnyWidget( "direct_ip_setting_editbox" ) );
		m_PortSetting			= Widget.Cast( m_Root.FindAnyWidget( "direct_port_setting" ) );
		m_PortEditbox			= EditBoxWidget.Cast( m_Root.FindAnyWidget( "direct_port_setting_editbox" ) );
		m_PasswordSetting		= Widget.Cast( m_Root.FindAnyWidget( "direct_port_setting" ) );
		m_PasswordEditbox		= EditBoxWidget.Cast( m_Root.FindAnyWidget( "direct_password_setting" ) );
		
		m_Menu = menu;
		m_TabType = type;
		
		string ip;
		string port;
		
		if ( GetGame().GetProfileString("ExpansionDirectConnectIP", ip) )
		{
			m_IPEditbox.SetText( ip );
		}
		
		if ( GetGame().GetProfileString("ExpansionDirectConnectPort", port) )
		{
			m_PortEditbox.SetText( port );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionDirectConnectTab::Construct - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override ResetFilters
	// ------------------------------------------------------------
	override void ResetFilters()
	{
	}
			
	// ------------------------------------------------------------
	// Override ApplyFilters
	// ------------------------------------------------------------
	override void ApplyFilters()
	{
	}
	
	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseButtonUp
	// ------------------------------------------------------------
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// Override RefreshList
	// ------------------------------------------------------------
	override void RefreshList()
	{
	}
	
	// ------------------------------------------------------------
	// Override IsFocusable
	// ------------------------------------------------------------
	override bool IsFocusable( Widget w )
	{
		if ( w )
		{
			if ( w == m_IPSetting || w == m_PortSetting || w == m_PasswordSetting )
			{
				return true;
			}

			return false;
		}
		return false;
	}
	
	// ------------------------------------------------------------
	// Override Focus
	// ------------------------------------------------------------
	override void Focus()
	{
	}
		
	// ------------------------------------------------------------
	// Expansion GetIP
	// ------------------------------------------------------------
	string GetIP()
	{
		string ip;
		
		if (m_IPEditbox.GetText() != "")
			ip = m_IPEditbox.GetText();
		
		return ip;
	}
			
	// ------------------------------------------------------------
	// Expansion GetPort
	// ------------------------------------------------------------
	int GetPort()
	{
		int port;
		
		if (m_PortEditbox.GetText() != "")
			port = m_PortEditbox.GetText().ToInt();
		
		return port;
	}
}