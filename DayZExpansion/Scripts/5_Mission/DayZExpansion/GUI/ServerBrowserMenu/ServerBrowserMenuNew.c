/**
 * ServerBrowserMenuNew.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionTabType: TabType
{
	DIRECT,
	EXPANSION
}

modded class ServerBrowserMenuNew
{
	protected ref ExpansionDirectConnectTab m_DirectTab;
	
	protected int EXPANSION_DIRECT_TAB_INDEX;
	protected bool m_IsDirect;
	
	// ------------------------------------------------------------
	// Override Init
	// ------------------------------------------------------------	
	override Widget Init()
	{
		layoutRoot = super.Init();
		
		EXPANSION_DIRECT_TAB_INDEX = m_Tabber.GetTabCount();
		m_Tabber.AddTab(EXPANSION_DIRECT_TAB_INDEX.ToString());	
		TextWidget.Cast(layoutRoot.FindAnyWidget("Tab_Control_" + EXPANSION_DIRECT_TAB_INDEX + "_Title")).SetText("DIRECT");
		m_DirectTab = new ExpansionDirectConnectTab(layoutRoot.FindAnyWidget("Tab_" + EXPANSION_DIRECT_TAB_INDEX), this, ExpansionTabType.DIRECT);
		

#ifdef PLATFORM_CONSOLE
		version = "#main_menu_version" + " " + version + " (" + g_Game.GetDatabaseID() + ")";
		if( GetGame().GetInput().IsEnabledMouseAndKeyboard() )
		{
			layoutRoot.FindAnyWidget( "play_panel_root" ).Show( true );
			layoutRoot.FindAnyWidget( "MouseAndKeyboardWarning" ).Show( true );
			layoutRoot.FindAnyWidget( "toolbar_bg" ).Show( false );
		}
		m_Version.SetText( version );
#else
		string version;
		GetGame().GetVersion( version );

		string expansion_version;
		version = "#main_menu_version" + " " + version;
		expansion_version = GetDayZGame().GetExpansionClientVersion();
		m_Version.SetText( "DayZ SA #main_menu_version" + " " + version + "   DayZ Expansion #main_menu_version" + " " + expansion_version );
#endif

		return layoutRoot;
	}
		
	// ------------------------------------------------------------
	// Override GetSelectedTab
	// ------------------------------------------------------------
	override ServerBrowserTab GetSelectedTab()
	{
		switch (m_Tabber.GetSelectedIndex())
		{
			case EXPANSION_DIRECT_TAB_INDEX:
			{
				IsInDirect(true);
				return m_DirectTab;
			}
		}
		
		IsInDirect(false);
		return super.GetSelectedTab();
	}
		
	// ------------------------------------------------------------
	// Override Refresh
	// ------------------------------------------------------------
	override void Refresh()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ServerBrowserMenuNew::Refresh - Start");
		#endif

		super.Refresh();

		string name;
		
		g_Game.GetPlayerNameShort( 14, name );
		
		if ( m_PlayerName )
			m_PlayerName.SetText( name );
		
		string version;
		GetGame().GetVersion( version );
		string expansion_version;	
		
		if ( GetDayZGame() )
		{
			expansion_version = GetDayZGame().GetExpansionClientVersion();
			m_Version.SetText( "DayZ SA #main_menu_version" + " " + version + "   DayZ Expansion #main_menu_version" + " " + expansion_version );
		}
		else
		{
			m_Version.SetText( "DayZ SA #main_menu_version" + " " + version );
		}
		#ifdef EXPANSIONEXPRINT
		EXPrint("ServerBrowserMenuNew::Refresh - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ShowPlayPanel
	// ------------------------------------------------------------
	void IsInDirect(bool state)
	{
		m_IsDirect = state;
	}
	
	// ------------------------------------------------------------
	// Expansion ConnectDirect
	// ------------------------------------------------------------
	void ConnectDirect()
	{
		string ip;
		int port;
		int connected;
		
		ip = m_DirectTab.GetIP();
		port = m_DirectTab.GetPort();
		
		if (ip && port)
		{
			GetGame().SetProfileString("ExpansionDirectConnectIP", ip);
			GetGame().SetProfileString("ExpansionDirectConnectPort", port.ToString());
			GetGame().SaveProfile();
			
			g_Game.ConnectFromServerBrowser( ip, port, "" );
		}
		else if ( !ip || !port )
		{
			GetGame().GetUIManager().ShowDialog( "#STR_EXPANSION_DIRECT_CONNECT_DIALOG_TITLE", "#STR_EXPANSION_DIRECT_CONNECT_ERROR", EXPANSION_IDC_Int_DIRECT, DBT_OK, DBB_NONE, DMT_INFO, GetGame().GetUIManager().GetMenu() );
		}
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if (button == MouseState.LEFT)
		{
			if (w == m_Play)
			{
				if (m_IsDirect)
				{
					ConnectDirect();

					return true;
				}
			}
		}

		return super.OnClick(w, x, y, button);
	}
}