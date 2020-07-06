/**
 * ExpansionServerBrowserTab.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/
/*class ExpansionServerBrowserTab extends ServerBrowserTab
{
	protected const int SERVERS_VISIBLE_COUNT	   		= 24;
	protected const int PAGES_BUTTONS_COUNT		 		= 10;
	
	protected int											m_TotalServersCount;
	protected int											m_PageIndex;
	protected int											m_PageStartNum;
	protected int											m_PageEndNum;
	protected int											m_PagesCount;
	protected int											m_ServersEstimateCount;
		
	protected Widget										m_PnlPagesPanel;
	protected TextWidget									m_PnlPagesLoadingText;
	protected ButtonWidget									m_BtnPagesFirst;
	protected ButtonWidget									m_BtnPagesLast;
	//protected ButtonWidget								m_BtnPagePrev;
	//protected ButtonWidget								m_BtnPageNext;
	
	//protected Widget										m_FilterSearchText;
	//protected Widget										m_FilterSearchTextBox;
	protected Widget										m_FilterSearchIP;
	protected Widget										m_FilterSearchIPBox;
	protected Widget										m_FilterPanelPing;
	protected Widget										m_FilterPanelAccTime;
	
	protected ref array<ButtonWidget>						m_BtnPages;
	protected ref array<ref ServerBrowserEntry>				m_ServerListEntries;
	
	protected ref TStringArray					  		m_TempTime = new TStringArray;

	protected static ref JsonSerializer 					m_Serializer = new JsonSerializer;;
	protected autoptr array< ExpansionServerListRest > 		m_ServersRest;
	protected ref array< ref ExpansionServerRest >  		m_ExpansionServers;
	
	protected int								   		m_LoadedIndex;
	
	protected RestApi 										m_ExpansionRestApi;
	protected RestContext									m_ExpansionRestContext;
	
	// ------------------------------------------------------------
	// ExpansionServerBrowserTab Constructor
	// ------------------------------------------------------------		
	void ExpansionServerBrowserTab( Widget parent, ServerBrowserMenuNew menu, TabType type )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionDirectConnectTab::ExpansionServerBrowserTab - Start");
		#endif
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionDirectConnectTab::ExpansionServerBrowserTab - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override Construct
	// ------------------------------------------------------------
	protected override void Construct( Widget parent, ServerBrowserMenuNew menu, TabType type )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionServerBrowserTab::Construct - Start");
		#endif
		m_LoadedIndex = 0;
		m_ServersRest = new array< ExpansionServerListRest >;
		m_ExpansionServers = new array< ref ExpansionServerRest >;

		m_Root = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/ui/server_browser/expansion_server_browser_tab.layout", parent );

		m_ServerListScroller = ScrollWidget.Cast( m_Root.FindAnyWidget( "server_list_scroller" ) );
		m_ServerList = SpacerBaseWidget.Cast( m_ServerListScroller.FindAnyWidget( "server_list_content" ) );
		m_ServerListScroller.VScrollToPos01( 0 );		
		
		m_ServerListEntries = new array<ref ServerBrowserEntry>;		
		m_EntryWidgets = new map<string, ref ServerBrowserEntry>;
		m_SortInverted = new map<ESortType, ESortOrder>;
		m_EntriesSorted = new map<ESortType, ref array<ref GetServersResultRow>>;
		
		m_EntriesSorted[ESortType.HOST] 		= new array<ref GetServersResultRow>;
		m_EntriesSorted[ESortType.TIME]			= new array<ref GetServersResultRow>;
		m_EntriesSorted[ESortType.POPULATION]	= new array<ref GetServersResultRow>;
		m_EntriesSorted[ESortType.SLOTS]		= new array<ref GetServersResultRow>;
		m_EntriesSorted[ESortType.PING]			= new array<ref GetServersResultRow>;
		m_EntriesSorted[ESortType.FAVORITE]		= new array<ref GetServersResultRow>;
		m_EntriesSorted[ESortType.PASSWORDED]	= new array<ref GetServersResultRow>;
		
		m_Menu = menu;
		m_TabType = type;
		
		m_ApplyFilter			= m_Root.FindAnyWidget( "apply_filter_button" );
		m_RefreshList			= m_Root.FindAnyWidget( "refresh_list_button" );
		m_FiltersChanged		= m_Root.FindAnyWidget( "unapplied_filters_notify" );
		m_HostSort				= m_Root.FindAnyWidget( "server_list_content_header_host" );
		m_TimeSort				= m_Root.FindAnyWidget( "server_list_content_header_time" );
		m_PopulationSort		= m_Root.FindAnyWidget( "server_list_content_header_population" );
		m_SlotsSort				= m_Root.FindAnyWidget( "server_list_content_header_slots" );
		m_PingSort				= m_Root.FindAnyWidget( "server_list_content_header_ping" );
		
		m_FilterSearchText		= m_Root.FindAnyWidget( "search_name_setting" );
		m_FilterSearchTextBox	= m_Root.FindAnyWidget( "search_name_setting_option" );
		m_FilterSearchIP		= m_Root.FindAnyWidget( "search_ip_setting" );
		m_FilterSearchIPBox		= m_Root.FindAnyWidget( "search_ip_setting_option" );
		
		m_FilterPanelAccTime	= m_Root.FindAnyWidget( "accelerated_time_setting" );
		m_FilterPanelPing		= m_Root.FindAnyWidget( "ping_setting" );
		
		m_LoadingText			= TextWidget.Cast( m_Root.FindAnyWidget( "loading_servers_info" ) );	
		
		ButtonDisable( m_ApplyFilter );
		
		if( type == TabType.LAN + 1 )
		{
			m_Root.FindAnyWidget( "filters_content" ).Show( false );
			m_Root.FindAnyWidget( "spacer" ).Show( false );
			m_Root.FindAnyWidget( "spacer2" ).Show( false );
			m_Root.FindAnyWidget( "reset_filter_button" ).Show( false );
			m_ApplyFilter.Show( false );
			m_FiltersChanged.Show( false );
		}
		
		m_Root.SetHandler( this );		
		
		SetSort( ESortType.HOST, ESortOrder.ASCENDING );
		
		m_BtnPages = new array<ButtonWidget>();
		
		int page_button_num = 1;
		ButtonWidget page_button = ButtonWidget.Cast( m_Root.FindAnyWidget( "servers_navigation_page" + page_button_num ) );
		
		if ( PAGES_BUTTONS_COUNT < 3 || PAGES_BUTTONS_COUNT > 10 )
		{
			Error("ServerBrowserTabPage->PAGES_BUTTONS_COUNT must be in range 3 - 10!");
			return;
		}
		
		while ( page_button )
		{
			page_button.Show(false);
			
			if ( page_button_num <= PAGES_BUTTONS_COUNT )
			{
				m_BtnPages.Insert( page_button );
			}
			
			page_button_num++;
			page_button = ButtonWidget.Cast( m_Root.FindAnyWidget( "servers_navigation_page"+ page_button_num ) );
		}		
				
		m_PnlPagesPanel			= m_Root.FindAnyWidget( "servers_navigation_spacer" );
		m_BtnPagePrev			= ButtonWidget.Cast( m_Root.FindAnyWidget( "servers_navigation_prev" ) ) ;
		m_BtnPageNext			= ButtonWidget.Cast( m_Root.FindAnyWidget( "servers_navigation_next" ) ) ;
		m_BtnPagesFirst			= ButtonWidget.Cast( m_Root.FindAnyWidget( "servers_navigation_page_first" ) ) ;
		m_BtnPagesLast			= ButtonWidget.Cast( m_Root.FindAnyWidget( "servers_navigation_page_last" ) ) ;
		m_PageIndex				= 0;
		m_PnlPagesPanel.Show( true );
		
		m_Initialized = false;

		UpdatePageButtons();
		
		RefreshList();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionServerBrowserTab::Construct - End");
		#endif
	}
		
	// ------------------------------------------------------------
	// ExpansionServerBrowserTab Destructor
	// ------------------------------------------------------------
	void ~ExpansionServerBrowserTab()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionDirectConnectTab::~ExpansionServerBrowserTab - Start");
		#endif
		
		if(m_Root)
			delete m_Root;
		
		if(m_ExpansionRestApi)
			DestroyRestApi();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionDirectConnectTab::~ExpansionServerBrowserTab - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion GetExpansionRestContext
	// ------------------------------------------------------------
	RestContext GetExpansionRestContext(string url)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionServerBrowserTab::GetExpansionRestContext - Start");
		#endif
		m_ExpansionRestApi = CreateRestApi();
		m_ExpansionRestContext = m_ExpansionRestApi.GetRestContext( url );
		
		if ( m_ExpansionRestContext )
		{ 
			DestroyRestApi();
			#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionServerBrowserTab::GetExpansionRestContext - Return: " + m_ExpansionRestContext );
		#endif
			return m_ExpansionRestContext;
		}
		
		return NULL;
	}

	// ------------------------------------------------------------
	// Expansion GetIsServerLoadingFinished
	// ------------------------------------------------------------
	override bool GetIsServerLoadingFinished()
	{
		return m_LoadingFinished;
	}
			
	// ------------------------------------------------------------
	// Expansion RemoveInvoker
	// ------------------------------------------------------------
	void RemoveInvoker()
	{
		OnlineServices.m_ServersAsyncInvoker.Remove( m_Menu.OnLoadServersAsync );
	}
		
	// ------------------------------------------------------------
	// Expansion LoadNextServer
	// ------------------------------------------------------------
	void LoadNextServer()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionServerBrowserTab::LoadNextServer - Start");
		#endif
		if ( m_LoadedIndex >= 0 && m_LoadedIndex < m_ExpansionServers.Count() )
		{
			m_CurrentFilterInput = new GetServersInput;
			
			m_CurrentFilterInput.SetHostIp( m_ExpansionServers[m_LoadedIndex].Adress );
			m_CurrentFilterInput.SetHostPort( m_ExpansionServers[m_LoadedIndex].Port );
			m_CurrentFilterInput.SetOfficialFilter( false );
			m_CurrentFilterInput.m_Page = 0;
			m_CurrentFilterInput.m_RowsPerPage = SERVER_BROWSER_PAGE_SIZE;
			m_CurrentFilterInput.m_Platform = 1;

			OnlineServices.LoadServers( m_CurrentFilterInput );

			m_LoadedIndex++;
		}
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionServerBrowserTab::LoadNextServer - End");
		#endif
	}
		
	// ------------------------------------------------------------
	// Override RefreshList
	// ------------------------------------------------------------
	override void RefreshList()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionServerBrowserTab::RefreshList - Start");
		#endif
		for ( int i = 0; i < m_EntriesSorted.Count(); i++ )
		{
			m_EntriesSorted.GetElement(i).Clear();
		}

		m_EntryWidgets.Clear();
		
		OnlineServices.m_ServersAsyncInvoker.Insert( m_Menu.OnLoadServersAsync );

		m_Menu.SetServersLoadingTab( m_TabType );
		
		m_LoadingFinished = false;
		m_Initialized = true;
		m_BegunLoading = false;
		m_LastLoadedPage = 0;
		m_TotalPages = -1;
		m_TotalServers = 0;
		m_TotalLoadedServers = 0;
		m_CurrentLoadedPage = 0;
		m_Loading = true;
		m_TotalServersCount = 0;
		m_ServersEstimateCount = 0;
		m_PageIndex = 0;

		m_LoadedIndex = 0;
		m_ServersRest.Clear();
		m_ExpansionServers.Clear();

		RestRequestServerdata();

		m_ServerListScroller.VScrollToPos01( 0 );
		ButtonRefreshToCancel();
		UpdateServerList();
		UpdatePageButtons();
		UpdateStatusBar();		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionServerBrowserTab::RefreshList - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion RestRequestServerdata
	// ------------------------------------------------------------
	void RestRequestServerdata()
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionServerBrowserTab::RestRequestServerdata - Start");
		#endif
		
		ExpansionRestBrowserCB cbx1 = new ExpansionRestBrowserCB;
		cbx1.data_servers.Insert(this.RestObtainExpansionServers);
		RestContext curl_ctx = GetExpansionRestContext(EXPANSION_Rest_URL);
		curl_ctx.GET( cbx1, "serverinfo" );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionServerBrowserTab::RestRequestServerdata - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RestObtainExpansionServers
	// ------------------------------------------------------------
	void RestObtainExpansionServers(string data_servers)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionServerBrowserTab::RestObtainExpansionServers - Start");
		#endif
		string js_error;

		if ( !m_Serializer.ReadFromString( m_ServersRest, data_servers, js_error ) )
		{
			Print("JSON ERROR => [RestObtainExpansionServers::RestObtainExpansionServers]: " + js_error);
		}
		
		if ( m_ServersRest && !js_error )
		{
			for (int i = 0; i < m_ServersRest.Count(); ++i)
			{
				m_ExpansionServers.Insert( new ExpansionServerRest( m_ServersRest.Get(i).serverip, m_ServersRest.Get(i).serverport ) );
				// Print( "ExpansionServerBrowserTab::RestObtainExpansionServers - Added Server: [" + m_ServersRest.Get(i).serverip + ":" + m_ServersRest.Get(i).serverport.ToString() + "]" );
			}
			// Print("ExpansionServerbrowsertab::RestObtainExpansionServers - m_ExpansionServers: " + m_ExpansionServers);
			RestObtainExpansionServersCallback();
		}
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionServerBrowserTab::RestObtainExpansionServers - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RestObtainExpansionServersCallback
	// ------------------------------------------------------------
	void RestObtainExpansionServersCallback()
	{
		LoadNextServer();
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
	// Override LoadingServersStop
	// ------------------------------------------------------------
	void LoadingServersStop()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionServerBrowserTab::LoadingServersStop - Start");
		#endif
		OnlineServices.m_ServersAsyncInvoker.Remove( m_Menu.OnLoadServersAsync );
		
		m_LoadingFinished = true;
		m_Loading = false;	
		
		ButtonCancelToRefresh();
		
		UpdateStatusBar();
		UpdateServerList();
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionServerBrowserTab::LoadingServersStop - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override PassFilter
	// ------------------------------------------------------------
	override bool PassFilter( GetServersResultRow result )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionServerBrowserTab::PassFilter - Start");
		#endif
		for ( int i = 0; i < m_ExpansionServers.Count(); i++ )
		{
			ExpansionServerRest esc = m_ExpansionServers[i];
			if ( esc )
			{
				string address = esc.Adress + ":" + esc.Port;
				if ( address == result.m_Id )
				{
					return true;
				}
			}
		}
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionServerBrowserTab::PassFilter - End");
		#endif
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnLoadServersAsyncPC
	// ------------------------------------------------------------
	override void OnLoadServersAsyncPC( ref GetServersResult result_list, EBiosError error, string response )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionServerBrowserTab::OnLoadServersAsyncPC - Start");
		#endif
		if ( result_list )
		{			
			int count = result_list.m_Results.Count();
			if ( count > 0 )
			{
				for ( int i = 0; i < count; i++ )
				{
					GetServersResultRow result = result_list.m_Results[i];
					m_TotalServersCount++;
					
					result.m_SortName = result.m_Name.ToInt();
					result.m_SortTime = GetTimeOfDayEnum( result.m_TimeOfDay );			
					
					if ( PassFilter( result ) )
					{
						int sorted_index = AddSorted( result );
						
						if ( sorted_index < (m_PageIndex * SERVERS_VISIBLE_COUNT + SERVERS_VISIBLE_COUNT) )
						{
							UpdatePageButtons();
							UpdateServerList();
						}
					}
					
					if ( !m_Menu || m_Menu.GetServersLoadingTab() != m_TabType )
					{
						return;
					}
				}
			}
			else
			{
				m_LoadingText.SetText( "#servers_count " + m_TotalServersCount );
				//m_Menu.SetServersLoadingTab( TabType.NONE );
			}
		}
		else
		{
			m_LoadingText.SetText( "#servers_count " + m_TotalServersCount );
			//m_Menu.SetServersLoadingTab( TabType.NONE );
		}
		
		if ( response == "thelastone" )
		{
			OnLoadServersAsyncPCFinished();
		}
		else
		{
			m_ServersEstimateCount = result_list.m_NumServers;
		}
		
		UpdateStatusBar();
		UpdateServerList();

		LoadNextServer();
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionServerBrowserTab::OnLoadServersAsyncPC - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion OnLoadServersAsyncPCFinished
	// ------------------------------------------------------------
	void OnLoadServersAsyncPCFinished()
	{
		LoadingServersStop();
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick( Widget w, int x, int y, int button )
	{
		super.OnClick( w, x, y, button );
		
		if ( button == MouseState.LEFT )
		{
			if ( w == m_RefreshList )
			{
				if ( m_Loading && !m_LoadingFinished )
				{
					LoadingServersStop();
				}
				else
				{
					RefreshList();
				}
				
				return true;
			}
			else if ( w == m_BtnPagePrev )
			{
				OnClickPagePrev();
				return true;
			}
			else if ( w == m_BtnPageNext )
			{
				OnClickPageNext();
				return true;
			}
			else if ( w == m_BtnPagesLast )
			{
				OnClickPageEnd();
				return true;
			}
			else if ( w == m_BtnPagesFirst )
			{
				OnClickPageFirst();
				return true;
			}
			else
			{
				bool click = OnClickPage( w );
				
				if ( click )
				{
					return true;
				}
				
				return false;
			}
		}
		return false;
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
		// Print("SG OnMouseButtonUp: "+ w.GetName());
		
		if ( button == MouseState.LEFT )
		{
			if ( w == m_HostSort )
			{
				ToggleSort( ESortType.HOST );
				UpdateServerList();
				return true;
			}
			else if ( w == m_TimeSort )
			{
				ToggleSort( ESortType.TIME );
				UpdateServerList();
				return true;
			}
			else if ( w == m_PopulationSort )
			{
				ToggleSort( ESortType.POPULATION );
				UpdateServerList();
				return true;
			}
			else if ( w == m_SlotsSort )
			{
				ToggleSort( ESortType.SLOTS );
				UpdateServerList();
				return true;
			}
			else if ( w == m_PingSort )
			{
				ToggleSort( ESortType.PING );
				UpdateServerList();
				return true;
			}			
		}
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion OnFilterChanged
	// ------------------------------------------------------------
	override void OnFilterChanged()
	{
	}
	
	// ------------------------------------------------------------
	// Expansion IsNotInitialized
	// ------------------------------------------------------------
	override bool IsNotInitialized()
	{
		return !m_Initialized;
	}
	
	// ------------------------------------------------------------
	// Expansion OnClickPagePrev
	// ------------------------------------------------------------
	void OnClickPagePrev()
	{
		if ( m_PageIndex > 0 )
		{
			SetPageIndex( m_PageIndex - 1);
		}
		else
		{
			SetPageIndex( m_PagesCount - 1 );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion OnClickPageNext
	// ------------------------------------------------------------
	void OnClickPageNext()
	{
		if ( m_PageIndex < (m_PagesCount - 1) )
		{
			SetPageIndex( m_PageIndex + 1 );
		}
		else
		{
			SetPageIndex( 0 );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion OnClickPageEnd
	// ------------------------------------------------------------
	void OnClickPageEnd()
	{
		SetPageIndex( m_PagesCount - 1 );
	}
	
	// ------------------------------------------------------------
	// Expansion OnClickPageFirst
	// ------------------------------------------------------------
	void OnClickPageFirst()
	{
		SetPageIndex( 0 );
	}
			
	// ------------------------------------------------------------
	// Expansion OnClickPage
	// ------------------------------------------------------------
	bool OnClickPage(Widget w)
	{
		for ( int i = 0; i < m_BtnPages.Count(); ++i )
		{
			if ( m_BtnPages[i] == w )
			{
				SetPageIndex( m_PageStartNum + i - 1 );
				return true;
			}
		}
		
		return false;
	}
		
	// ------------------------------------------------------------
	// Expansion SetPageIndex
	// ------------------------------------------------------------
	void SetPageIndex(int page_index)
	{
		m_PageIndex = page_index;
		UpdatePageButtons();
		UpdateServerList();
	}
	
	// ------------------------------------------------------------
	// Expansion ToggleSort
	// ------------------------------------------------------------
	void ToggleSort( ESortType type )
	{
		ESortOrder order = ESortOrder.ASCENDING;
		
		if ( m_SortType != type )
		{
			if ( m_SortInverted[type] == true )
			{
				m_EntriesSorted[type].Invert();
			}
			
			m_SortInverted[type] = false;
		}
		else
		{
			if ( m_SortOrder == ESortOrder.ASCENDING )
			{
				m_EntriesSorted[type].Invert();				
				m_SortInverted[type] = true;				
				order = ESortOrder.DESCENDING;
			}
			else
			{
				m_EntriesSorted[type].Invert();				
				m_SortInverted[type] = false;			
			}
		}
		
		SetSort( type, order );
	}
		
	// ------------------------------------------------------------
	// Override SetSort
	// ------------------------------------------------------------
	override void SetSort( ESortType type, ESortOrder order )
	{
		super.SetSort( type, order);
		
		m_Root.FindAnyWidget( "host_sort" ).Show( false );
		m_Root.FindAnyWidget( "population_sort" ).Show( false );
		m_Root.FindAnyWidget( "slots_sort" ).Show( false );
		
		m_Root.FindAnyWidget( "time_sort" ).Show( false );
		m_Root.FindAnyWidget( "ping_sort" ).Show( false );
		
		TextWidget root;
		root = TextWidget.Cast( m_Root.FindAnyWidget( "server_list_content_header_host_label" ) );
		root.SetColor( ARGBF( 1, 1, 1, 1 ) );
		root = TextWidget.Cast( m_Root.FindAnyWidget( "server_list_content_header_population_label" ) );
		root.SetColor( ARGBF( 1, 1, 1, 1 ) );
		root = TextWidget.Cast( m_Root.FindAnyWidget( "server_list_content_header_slots_label" ) );
		root.SetColor( ARGBF( 1, 1, 1, 1 ) );
		
		root = TextWidget.Cast( m_Root.FindAnyWidget( "server_list_content_header_ping_label" ) );
		root.SetColor( ARGBF( 1, 1, 1, 1 ) );
		root = TextWidget.Cast( m_Root.FindAnyWidget( "server_list_content_header_time_label" ) );
		root.SetColor( ARGBF( 1, 1, 1, 1 ) );
		
		string r_name;
		string w_name;
		
		switch( type )
		{
			case ESortType.HOST:
			{
				r_name = "server_list_content_header_host_label";
				w_name = "host_sort";
				break;
			}
			case ESortType.TIME:
			{
				r_name = "server_list_content_header_time_label";
				w_name = "time_sort";
				break;
			}
			case ESortType.POPULATION:
			{
				r_name = "server_list_content_header_population_label";
				w_name = "population_sort";
				break;
			}
			case ESortType.SLOTS:
			{
				r_name = "server_list_content_header_slots_label";
				w_name = "slots_sort";
				break;
			}
			case ESortType.PING:
			{
				r_name = "server_list_content_header_ping_label";
				w_name = "ping_sort";
				break;
			}
		}
		
		root = TextWidget.Cast( m_Root.FindAnyWidget( r_name ) );
		root.SetColor( ARGBF( 1, 1, 0, 0 ) );
		
		m_Root.FindAnyWidget( w_name ).Show( true );
		m_Root.FindAnyWidget( w_name + "_asc" ).Show( !m_SortOrder );
		m_Root.FindAnyWidget( w_name + "_dsc" ).Show( m_SortOrder );
	}
	
	// ------------------------------------------------------------
	// Expansion GetTimeOfDayEnum
	// ------------------------------------------------------------
	int GetTimeOfDayEnum( string time_of_day )
	{
		m_TempTime.Clear();
		
		time_of_day.Split( ":", m_TempTime );
		
		int minutes = 0;
		
		if ( m_TempTime.Count() > 0 )
		{
			int h = m_TempTime[0].ToInt();
			
			if ( h >= 5 )
			{
				h -= 5;
			}
			else
			{
				h += 24;
			}
			
			minutes = h * 60;
		}
		
		if ( m_TempTime.Count() > 1 )
		{
			int m = m_TempTime[0].ToInt();
			minutes += m;
		}
		
		return minutes;
	}
		
	// ------------------------------------------------------------
	// Override AddSorted
	// ------------------------------------------------------------
	// Return sorted index
	override int AddSorted( GetServersResultRow entry )
	{
		int index_host = SortedInsert( entry, ESortType.HOST );
		int index_time = SortedInsert( entry, ESortType.TIME );
		int index_popu = SortedInsert( entry, ESortType.POPULATION );
		int index_slot = SortedInsert( entry, ESortType.SLOTS );
		int index_ping = SortedInsert( entry, ESortType.PING );
		int index_favo = SortedInsert( entry, ESortType.FAVORITE );
		int index_pass = SortedInsert( entry, ESortType.PASSWORDED );
		
		switch( m_SortType )
		{
			case ESortType.HOST:
				return index_host;
			case ESortType.TIME:
				return index_time;
			case ESortType.POPULATION:
				return index_popu;
			case ESortType.SLOTS:
				return index_slot;
			case ESortType.PING:
				return index_ping;
			case ESortType.FAVORITE:
				return index_favo;
			case ESortType.PASSWORDED:
				return index_pass;
		}
			
		return 0;
	}
	
	// ------------------------------------------------------------
	// Override SelectServer
	// ------------------------------------------------------------
	override void SelectServer( ServerBrowserEntry server )
	{
		super.SelectServer(server);
		
		m_SelectedServer = server;
		
		if ( !m_Menu )
			return;
		
		m_Menu.SelectServer( server );
	}
	
	// ------------------------------------------------------------
	// Expansion SortedInsert
	// ------------------------------------------------------------
	int SortedInsert( GetServersResultRow entry, ESortType sort_type )
	{
		if ( m_SortInverted[sort_type] )
		{
			return SortedInsertDesc(entry, sort_type);
		}
		
		return SortedInsertAsc(entry, sort_type);
	}
	
	// ------------------------------------------------------------
	// Expansion SortedInsertAsc
	// ------------------------------------------------------------	
	int SortedInsertAsc( GetServersResultRow entry, ESortType sort_type )
	{
		array<ref GetServersResultRow> list = m_EntriesSorted[sort_type];
		
		int i;
		int index_min = 0;
		int index_max = list.Count() - 1;
		int target_index = Math.Floor( index_max / 2 );
		
		if ( index_max == -1 )
		{
			list.Insert( entry );
			return 0;
		}
		
		if ( sort_type == ESortType.HOST )
		{
			string entry_value_str = entry.GetValueStr(ESortType.HOST);
			
			while ( true )
			{
				string target_value_str = list[target_index].GetValueStr(ESortType.HOST);
				
				if ( entry_value_str == target_value_str || ((index_max - index_min) <= 1) )
				{
					for ( i = index_min; i <= index_max; i++ )
					{
						if ( entry_value_str <= list[i].GetValueStr(ESortType.HOST) )
						{
							list.InsertAt( entry, i );
							return i;
						}
					}
					
					index_max++;
					list.InsertAt( entry, index_max );
					return target_index;
				}
				else if ( entry_value_str < target_value_str )
				{
					index_max = target_index;
					target_index = Math.Floor( target_index / 2 );
				}
				else if ( entry_value_str > target_value_str )
				{
					index_min = target_index;
					target_index += Math.Floor( (index_max - index_min) / 2 );
				}			
			}
		}
		else
		{
			int entry_value_int = entry.GetValueInt( sort_type );
			
			while ( true )
			{
				int target_value_int = list[target_index].GetValueInt( sort_type );
				
				if ( entry_value_int == target_value_int || ((index_max - index_min) <= 1) )
				{
					for ( i = index_min; i <= index_max; i++ )
					{
						if ( entry_value_int <= list[i].GetValueInt( sort_type ) )
						{
							list.InsertAt( entry, i );
							return i;
						}
					}
					
					index_max++;
					list.InsertAt( entry, index_max );
					return target_index;
				}
				else if ( entry_value_int < target_value_int )
				{
					index_max = target_index;
					target_index = Math.Floor( target_index / 2 );
				}
				else if ( entry_value_int > target_value_int )
				{
					index_min = target_index;
					target_index += Math.Floor( (index_max - index_min) / 2 );
				}			
			}
		}
		
		return target_index;
	}
	
	// ------------------------------------------------------------
	// Expansion SortedInsertDesc
	// ------------------------------------------------------------	
	int SortedInsertDesc( GetServersResultRow entry, ESortType sort_type )
	{
		array<ref GetServersResultRow> list = m_EntriesSorted[sort_type];
		
		int i;
		int index_min = 0;
		int index_max = list.Count() - 1;
		int target_index = Math.Floor( index_max / 2 );
		
		if ( index_max == -1 )
		{
			list.Insert( entry );
			return 0;
		}
		
		if ( sort_type == ESortType.HOST )
		{
			string entry_value_str = entry.GetValueStr(ESortType.HOST);
			
			while ( true )
			{
				string target_value_str = list[target_index].GetValueStr(ESortType.HOST);
				
				if ( entry_value_str == target_value_str || ((index_max - index_min) <= 1) )
				{
					for ( i = index_min; i <= index_max; i++ )
					{
						if ( entry_value_str >= list[i].GetValueStr(ESortType.HOST) )
						{
							list.InsertAt( entry, i );
							return i;
						}
					}
					
					index_max++;
					list.InsertAt( entry, index_max );
					return target_index;
				}
				else if ( entry_value_str > target_value_str )
				{
					index_max = target_index;
					target_index = Math.Floor( target_index / 2 );
				}
				else if ( entry_value_str < target_value_str )
				{
					index_min = target_index;
					target_index += Math.Floor( (index_max - index_min) / 2 );
				}			
			}
		}
		else
		{
			int entry_value_int = entry.GetValueInt( sort_type );
			
			while ( true )
			{
				int target_value_int = list[target_index].GetValueInt( sort_type );
				
				if ( entry_value_int == target_value_int || ((index_max - index_min) <= 1) )
				{
					for ( i = index_min; i <= index_max; i++ )
					{
						if ( entry_value_int >= list[i].GetValueInt( sort_type ) )
						{
							list.InsertAt( entry, i );
							return i;
						}
					}
					
					index_max++;
					list.InsertAt( entry, index_max );
					return target_index;
				}
				else if ( entry_value_int > target_value_int )
				{
					index_max = target_index;
					target_index = Math.Floor( target_index / 2 );
				}
				else if ( entry_value_int < target_value_int )
				{
					index_min = target_index;
					target_index += Math.Floor( (index_max - index_min) / 2 );
				}			
			}
		}
		
		return target_index;
	}
	
	// ------------------------------------------------------------
	// Expansion UpdatePageButtons
	// ------------------------------------------------------------
	void UpdatePageButtons()
	{
		m_PagesCount = Math.Ceil( m_EntriesSorted[m_SortType].Count() / SERVERS_VISIBLE_COUNT );
		
		// Print("page num: ("+ (m_PageIndex + 1) +") m_PageIndex="+ m_PageIndex +" m_PagesCount="+ m_PagesCount);
		
		if ( m_PagesCount > 1 )
		{
			m_PnlPagesPanel.Show( true );
			
			int i;
			int btn_index_center = Math.Round( PAGES_BUTTONS_COUNT / 2) - 1; 
			m_PageStartNum = 1;
			m_PageEndNum = PAGES_BUTTONS_COUNT;
			
			if ( m_PagesCount <= PAGES_BUTTONS_COUNT )
			{
				m_PageEndNum = m_PagesCount;
				
				// Pages are less like buttons - changing selected button
				for ( i = 1; i <= m_BtnPages.Count(); ++i )
				{
					bool is_page_button_enable = ( i <= m_PagesCount );
										
					int page_index = (i - 1);
					m_BtnPages[page_index].Show( is_page_button_enable );
					m_BtnPages[page_index].SetText( i.ToString() );
					
					if ( m_PageIndex == page_index )
					{
						m_BtnPages[page_index].SetState( true );
					}
					else
					{
						m_BtnPages[page_index].SetState( false );
					}
				}
			}
			else
			{
				// Pages are more like buttons				
				if ( m_PageIndex <= btn_index_center )
				{
					//Left side (start of pages)
					//m_PageStartNum = 1;
					//m_PageEndNum = PAGES_BUTTONS_COUNT;
				}
				else
				{
					m_PageStartNum = m_PageIndex - btn_index_center + 1;
					m_PageEndNum = m_PageIndex + (PAGES_BUTTONS_COUNT - btn_index_center);
							
					if ( m_PageEndNum > m_PagesCount )
					{
						// Right side (end of pages)
						m_PageStartNum -= m_PageEndNum - m_PagesCount;
						m_PageEndNum -= m_PageEndNum - m_PagesCount;
					}
				}
				
				
				// Button Selection
				int selected_btn_index = m_PageIndex;
				if ( m_PageIndex > btn_index_center )
				{
					if ( m_PageEndNum == m_PagesCount )
					{
						//Right
						 selected_btn_index = PAGES_BUTTONS_COUNT - (m_PagesCount - m_PageIndex);
					}
					else
					{
						//Center
						selected_btn_index = btn_index_center;
					}
				}
				
				
				for ( i = 0; i < m_BtnPages.Count(); ++i )
				{
					m_BtnPages[i].SetText( (m_PageStartNum + i).ToString() );
					m_BtnPages[i].SetState( ( i == selected_btn_index ) );
				}
			}
			
			// Button GoTo Prev Page
			if ( m_PageIndex > 0 )
			{
				m_BtnPagePrev.SetText( "<" );
				m_BtnPagePrev.Enable( true );
			}
			else
			{
				m_BtnPagePrev.SetText( "" );
				m_BtnPagePrev.Enable( false );					
			}
			
			// Button GoTo Next Page
			if ( m_PageIndex < (m_PagesCount - 1) )
			{
				m_BtnPageNext.SetText( ">" );
				m_BtnPageNext.Enable( true );
			}
			else
			{
				m_BtnPageNext.SetText( "" );
				m_BtnPageNext.Enable( false );
			}
			
			// Button GoTo First Page
			if ( m_PageStartNum == 1 )
			{
				m_BtnPagesFirst.SetText( "" );
				m_BtnPagesFirst.Enable( false );
			}
			else
			{
				m_BtnPagesFirst.SetText( "<<" );
				m_BtnPagesFirst.Enable( true );
			}
			
			// Button GoTo Last Page
			if ( m_PageEndNum == m_PagesCount )
			{
				m_BtnPagesLast.SetText( "" );
				m_BtnPagesLast.Enable( false );
			}
			else
			{
				m_BtnPagesLast.SetText( ">>" );
				m_BtnPagesLast.Enable( true );
			}
		}
		else
		{
			m_PnlPagesPanel.Show(false);
		}
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateServerList
	// ------------------------------------------------------------
	void UpdateServerList()
	{		
		int i;
		ServerBrowserEntry entry;
		
		m_EntryWidgets.Clear();
		
		//m_PnlPagesPanel.Show( (m_PagesCount > 1) );
		
		for ( i = 0; i < SERVERS_VISIBLE_COUNT; ++i )
		{
			int server_index = i + (SERVERS_VISIBLE_COUNT * m_PageIndex);
			
			if ( server_index >= 0 && server_index < m_EntriesSorted[m_SortType].Count() )
			{
				GetServersResultRow server_info = m_EntriesSorted[m_SortType][server_index];
				
				entry = GetServerEntryByIndex(i, server_info.m_Id);
				
				server_info.m_Modded = true;
				entry.Show( true );
				entry.FillInfo( server_info );
				entry.SetMods( m_EntryMods.Get( server_info.m_Id ) );
				m_TotalLoadedServers++;
			}
			else
			{
				entry = GetServerEntryByIndex(i, "-1");
				
				if ( entry )
				{
					entry.Show( false );
				}
			}
		}
		
		m_ServerList.Update();
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateStatusBar
	// ------------------------------------------------------------
	void UpdateStatusBar()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionServerBrowserTab::UpdateStatusBar - Start");
		#endif
		if ( m_LoadingFinished )
		{
			m_LoadingText.SetText( "#servers_found: " + m_EntriesSorted[m_SortType].Count() );
		}
		else
		{
			m_LoadingText.SetText( "#dayz_game_loading" );
		}
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionServerBrowserTab::UpdateStatusBar - End");
		#endif
	}
		
	// ------------------------------------------------------------
	// Expansion GetServerEntryByIndex
	// ------------------------------------------------------------
	protected ServerBrowserEntry GetServerEntryByIndex( int index, string server_id )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionServerBrowserTab::GetServerEntryByIndex - Start");
		#endif
		ref ServerBrowserEntry entry;
		
		if ( index >= 0 )
		{
			if ( index < m_ServerListEntries.Count() )
			{
				entry = m_ServerListEntries[index];
			}
			else
			{
				entry = new ServerBrowserEntry( null, index, this );
				m_ServerList.AddChild( entry.GetRoot(), false );
				
				m_ServerListEntries.Insert(entry);
			}
		}
		else
		{
			return null;
		}
		
		// Refresh list
		m_EntryWidgets.Insert( server_id, entry );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionServerBrowserTab::GetServerEntryByIndex - End");
		#endif
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionServerBrowserTab::GetServerEntryByIndex - Entry: " + entry);
		#endif
		return entry;
	}
	
	// ------------------------------------------------------------
	// Override IsFocusable
	// ------------------------------------------------------------
	override bool IsFocusable( Widget w )
	{
		if ( w )
		{
			if ( w == m_ServerListScroller )
			{
				return false;
			}
			
			if ( w == m_HostSort || w == m_TimeSort || w == m_PopulationSort || w == m_SlotsSort || w == m_PingSort )
			{
				return true;
			}

			return false;
		}
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion ButtonRefreshToCancel
	// ------------------------------------------------------------
	void ButtonRefreshToCancel()
	{
		ButtonWidget btn = ButtonWidget.Cast( m_RefreshList );
		btn.SetText( "#menu_cancel" );
	}
	
	// ------------------------------------------------------------
	// Expansion ButtonCancelToRefresh
	// ------------------------------------------------------------
	void ButtonCancelToRefresh()
	{
		ButtonWidget btn = ButtonWidget.Cast( m_RefreshList );
		btn.SetText( "#STR_server_browser_tab_root_filters_root_filters_content_refresh_list_button0" );
	}
	
	// ------------------------------------------------------------
	// Override Connect
	// ------------------------------------------------------------
	override void Connect( ServerBrowserEntry server )
	{
		if( !m_Menu )
			return;
		
		if( m_Menu.GetServersLoadingTab() != TabType.NONE )
			return;
		
		m_SelectedServer = server;
		m_Menu.Connect( server );
	}
	
	// ------------------------------------------------------------
	// Override GetSortOption
	// ------------------------------------------------------------
	override string GetSortOption()
	{
		switch( m_SortType )
		{
			case ESortType.HOST:
			{
				return "name";
			}
			case ESortType.TIME:
			{
				return "name";
			}
			case ESortType.POPULATION:
			{
				return "currentNumberPlayers";
			}
			case ESortType.SLOTS:
			{
				return "freeSlots";
			}
			case ESortType.PING:
			{
				return "name";
			}
		}
		return "";
	}
	
	// ------------------------------------------------------------
	// Override GetTabType
	// ------------------------------------------------------------
	override TabType GetTabType()
	{
		return m_TabType;
	}
	
	// ------------------------------------------------------------
	// Override OnLoadServerModsAsync
	// ------------------------------------------------------------
	override void OnLoadServerModsAsync( string server_id, array<string> mods )
	{
		super.OnLoadServerModsAsync( server_id, mods );
		
		if( m_EntryWidgets.Contains( server_id ) )
		{
			m_EntryWidgets.Get( server_id ).SetMods( mods );
		}
	}
}*/