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
	//protected ref ExpansionServerBrowserTab m_ExpansionTab;
	protected ref ExpansionDirectConnectTab m_DirectTab;
	
	protected Widget 						m_TabberWidget;
	protected Widget 						m_TabberPanel;
	protected Widget 						m_PlayPanel;
	protected bool 							m_IsDirect;
	
	// ------------------------------------------------------------
	// Override Init
	// ------------------------------------------------------------
	override Widget Init()
	{
		layoutRoot = super.Init();

		layoutRoot.FindAnyWidget( "Tabber" ).Unlink();
		layoutRoot.FindAnyWidget( "play_panel_root" ).Unlink();
		
		m_TabberPanel = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/ui/server_browser/expansion_server_browser_tabber.layout", layoutRoot );
		
		if ( m_TabberPanel )
		{
			m_OfficialTab	= new ServerBrowserTabPc( m_TabberPanel.FindAnyWidget( "Tab_0" ), this, TabType.OFFICIAL );
			m_CommunityTab	= new ServerBrowserTabPc( m_TabberPanel.FindAnyWidget( "Tab_1" ), this, TabType.COMMUNITY );
			m_LANTab		= new ServerBrowserTabPc( m_TabberPanel.FindAnyWidget( "Tab_2" ), this, TabType.LAN );
			m_DirectTab		= new ExpansionDirectConnectTab( m_TabberPanel.FindAnyWidget( "Tab_3" ), this, ExpansionTabType.DIRECT );
			//m_ExpansionTab	= new ExpansionServerBrowserTab( m_TabberPanel.FindAnyWidget( "Tab_4" ), this, ExpansionTabType.EXPANSION );
			
			layoutRoot.FindAnyWidget( "Tabber" ).GetScript( m_Tabber );
		}
		
		m_PlayPanel				= GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/ui/server_browser/expansion_server_browser_buttons.layout", layoutRoot );
		
		if ( m_PlayPanel )
		{
			m_Play					= m_PlayPanel.FindAnyWidget( "play" );
			m_Back					= m_PlayPanel.FindAnyWidget( "back_button" );
			m_CustomizeCharacter	= m_PlayPanel.FindAnyWidget( "customize_character" );
		}
		
		m_PlayerName			= TextWidget.Cast( layoutRoot.FindAnyWidget( "character_name_text" ) );
		m_Version				= TextWidget.Cast( layoutRoot.FindAnyWidget( "version" ) );
		
		Refresh();
		
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
		
		OnlineServices.m_ServersAsyncInvoker.Insert( OnLoadServersAsync );
		OnlineServices.m_ServerModLoadAsyncInvoker.Insert( OnLoadServerModsAsync );
		m_Tabber.m_OnTabSwitch.Insert( OnTabSwitch );
				
		m_OfficialTab.RefreshList();
		//m_ExpansionTab.RefreshList();
		
		PPEffects.SetBlurMenu( 0.5 );
		
		return layoutRoot;
	}
		
	// ------------------------------------------------------------
	// Override GetSelectedTab
	// ------------------------------------------------------------
	override ServerBrowserTab GetSelectedTab()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ServerBrowserMenuNew::GetSelectedTab - Start");
		#endif
		switch( m_Tabber.GetSelectedIndex() )
		{
			case 0:
			{
				IsInDirect(false);
				return m_OfficialTab;
			}
			case 1:
			{
				IsInDirect(false);
				return m_CommunityTab;
			}
			case 2:
			{
				IsInDirect(false);
				return m_LANTab;
			}
			case 3:
			{
				IsInDirect(true);
				return m_DirectTab;
			}
			/*case 4:
			{
				IsInDirect(false);
				return m_ExpansionTab;
			}*/
		}
		#ifdef EXPANSIONEXPRINT
		EXPrint("ServerBrowserMenuNew::GetSelectedTab - End");
		#endif
		return null;
	}
		
	// ------------------------------------------------------------
	// Override Refresh
	// ------------------------------------------------------------
	override void Refresh()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ServerBrowserMenuNew::Refresh - Start");
		#endif
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
	// Override OnTabSwitch
	// ------------------------------------------------------------
	override void OnTabSwitch()
	{
		SetServersLoadingTab( TabType.NONE );
		
		if( GetSelectedTab().IsNotInitialized() )
		{
			GetSelectedTab().RefreshList();
		}
		
		/*if( GetServersLoadingTab() == TabType.LAN + 1 )
		{
			m_ExpansionTab.RemoveInvoker();
		}*/
		
		GetSelectedTab().Focus();
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
		if( button == MouseState.LEFT )
		{
			if( w == m_Play )
			{
				if(!m_IsDirect)
				{
					Play();
				}
				else
				{
					ConnectDirect();
				}
	
				return true;
			}
			else if( w == m_Back )
			{
				Back();
				return true;
			}
			else if( w == m_CustomizeCharacter )
			{
				CustomizeCharacter();
				return true;
			}
		}
		return false;
	}
}