/**
 * ExpansionBookTabServerInfo.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookTabServerInfo extends ExpansionBookTabBase
{
	protected int COLOR_EXPANSION_SERVERINFO_BOOKMARK_ICON = ARGB( 255, 26, 188, 156 );
	
	protected TextWidget m_book_ServerName;
	protected Widget m_book_ServerDescriptionRoot;
	
	protected HtmlWidget m_book_ServerDescriptionTitle1;
	protected HtmlWidget m_book_ServerDescriptionText1;
	protected HtmlWidget m_book_ServerDescriptionTitle2;
	protected HtmlWidget m_book_ServerDescriptionText2;
	protected HtmlWidget m_book_ServerDescriptionTitle3;
	protected HtmlWidget m_book_ServerDescriptionText3;
	protected ref ExpansionServerInfos m_ServerInfo;
	
	protected ButtonWidget m_book_ServerPageButton;
	protected ButtonWidget m_book_ServerForumsButton;
	protected ButtonWidget m_book_ServerDiscordButton;
	
	protected Widget m_book_ServerPageButtonSpacer;
	protected Widget m_book_ServerForumsButtonSpacer;
	protected Widget m_book_ServerDiscordButtonSpacer;
	protected string m_book_ServerPageURL;
	protected string m_book_ServerForumsURL;
	protected string m_book_ServerDiscordURL;
	
	protected Widget m_book_ServerUptimePanel;
	protected TextWidget m_book_ServerUptimeValue;
	
	protected Widget m_book_PlayerCountPanel;
	protected TextWidget m_book_PlayerCountValue;
	
	protected ImageWidget m_book_ServerLogo;
	
	// ------------------------------------------------------------
	// ExpansionBookTabServerInfo Contructor
	// ------------------------------------------------------------
	void ExpansionBookTabServerInfo()
	{
		SetButtonIconPath( "set:expansion_iconset image:icon_info" );
		SetButtonText( "#STR_BOOK_SERVERINFOMENU" );
		SetButtonColor( COLOR_EXPANSION_SERVERINFO_BOOKMARK_ICON );
		SetLayoutPath( "DayZExpansion/GUI/layouts/book/expansion_book_page_info.layout" );
	}

	// ------------------------------------------------------------
	// Override OnCreatedLayout
	// ------------------------------------------------------------
	//Layout root var is m_RootLayout
	override void OnCreatedLayout()
	{
		m_book_ServerName						= TextWidget.Cast( m_RootLayout.FindAnyWidget( "server_name_label" ) );
		m_book_ServerPageButtonSpacer			= Widget.Cast( m_RootLayout.FindAnyWidget( "sticky_homepage_panel" ) );
		m_book_ServerForumsButtonSpacer			= Widget.Cast( m_RootLayout.FindAnyWidget( "sticky_forums_panel" ) );
		m_book_ServerDiscordButtonSpacer		= Widget.Cast( m_RootLayout.FindAnyWidget( "sticky_discord_panel" ) );
		m_book_ServerPageButton					= ButtonWidget.Cast( m_RootLayout.FindAnyWidget( "sticky_homepage_button" ) );
		m_book_ServerForumsButton				= ButtonWidget.Cast( m_RootLayout.FindAnyWidget( "sticky_forums_button" ) );
		m_book_ServerDiscordButton				= ButtonWidget.Cast( m_RootLayout.FindAnyWidget( "sticky_discord_button" ) );
		
		m_book_ServerPageURL					= GetExpansionSettings().GetBook().WebsiteURL;
		m_book_ServerForumsURL					= GetExpansionSettings().GetBook().ForumsURL;
		m_book_ServerDiscordURL					= GetExpansionSettings().GetBook().DiscordURL;
	
		m_book_ServerPageButtonSpacer.Show( GetExpansionSettings().GetBook().WebsiteButton ); 
		m_book_ServerForumsButtonSpacer.Show( GetExpansionSettings().GetBook().ForumsButton ); 
		m_book_ServerDiscordButtonSpacer.Show( GetExpansionSettings().GetBook().DiscordButton );
		
		m_book_ServerLogo						= ImageWidget.Cast( m_RootLayout.FindAnyWidget( "server_info_logo" ) );

		/*
		if ( GetExpansionSettings().GetBook().DisplayServerUptimeInStatusTab )
		{
			m_book_ServerUptimePanel				= Widget.Cast( m_RootLayout.FindAnyWidget( "info_uptime_spacer" ) );
			m_book_ServerUptimeValue				= TextWidget.Cast( m_RootLayout.FindAnyWidget( "info_uptime_value" ) );
			
			m_book_ServerUptimePanel.Show( true );
		}
		*/
		
		if ( GetExpansionSettings().GetBook().DisplayPlayerCountInStatusTab )
		{
			m_book_PlayerCountPanel				= Widget.Cast( m_RootLayout.FindAnyWidget( "info_players_spacer" ) );
			m_book_PlayerCountValue				= TextWidget.Cast( m_RootLayout.FindAnyWidget( "info_players_value" ) );
			
			m_book_PlayerCountPanel.Show( true );
		}
		
		m_book_ServerDescriptionRoot			= m_RootLayout.FindAnyWidget( "server_description_content" );
		if (m_book_ServerDescriptionRoot)
		{
			m_book_ServerDescriptionTitle1 			= HtmlWidget.Cast( m_book_ServerDescriptionRoot.FindAnyWidget( "server_info_T1" ) );
			m_book_ServerDescriptionText1 			= HtmlWidget.Cast( m_book_ServerDescriptionRoot.FindAnyWidget( "server_info_C1" ) );
			m_book_ServerDescriptionTitle2 			= HtmlWidget.Cast( m_book_ServerDescriptionRoot.FindAnyWidget( "server_info_T2" ) );
			m_book_ServerDescriptionText2 			= HtmlWidget.Cast( m_book_ServerDescriptionRoot.FindAnyWidget( "server_info_C2" ) );
			m_book_ServerDescriptionTitle3 			= HtmlWidget.Cast( m_book_ServerDescriptionRoot.FindAnyWidget( "server_info_T3" ) );
			m_book_ServerDescriptionText3 			= HtmlWidget.Cast( m_book_ServerDescriptionRoot.FindAnyWidget( "server_info_C3" ) );
		}
		
		LoadServerInfos();
		LoadServerLogo();
	}
		
	// ------------------------------------------------------------
	// Override OnUpdate
	// ------------------------------------------------------------
	override void OnUpdate( float timeslice )
	{
		if ( GetExpansionSettings().GetBook().DisplayServerUptimeInStatusTab )
		{
			SetServerUpTime();
		}
		
		if ( GetExpansionSettings().GetBook().DisplayPlayerCountInStatusTab )
		{
			SetPlayerCount();
		}
	}
	
	// ------------------------------------------------------------
	// SetPlayerCount
	// ------------------------------------------------------------
	void SetPlayerCount()
	{
		int count = ClientData.m_PlayerList.m_PlayerList.Count();
		
		if ( count == 1 )
			m_book_PlayerCountValue.SetText("" + count + " #STR_EXPANSION_PLAYER" );
		else if ( count > 1 || count == 0 )
			m_book_PlayerCountValue.SetText("" + count + " #STR_EXPANSION_PLAYERS" );
	}
	
	// ------------------------------------------------------------
	// SetServerUpTime
	// ------------------------------------------------------------
	void SetServerUpTime()
	{
		/*int missionTime = GetGame().GetTime();
		
		m_book_ServerUptimeValue.SetText( GetUpTimeString( missionTime ) );
		*/
	}
	
	// ------------------------------------------------------------
	// Expansion GetUpTimeString
	// ------------------------------------------------------------
	protected string GetUpTimeString( int total_time )
	{
		int milliseconds = total_time;
		string time_string;
		
		int seconds = milliseconds / 1000;
		milliseconds = milliseconds % 1000;
		
		int minutes = seconds / 60;
		seconds = seconds % 60;
		
		int hours = minutes / 60;
		minutes = minutes % 60;
		
		if ( hours > 0 )
		{
			time_string += GetValueString( hours ) + "H";		//hours
		}
		
		time_string += " ";										//separator
		
		if ( minutes > 0 )
		{
			time_string += GetValueString( minutes ) + "M";		//minutes
		}
		
		time_string += " ";										//separator
		
		time_string += GetValueString( seconds ) + "S";			//seconds
		
		return time_string;
	}
	
	
	// ------------------------------------------------------------
	// Expansion GetValueString
	// ------------------------------------------------------------
	protected string GetValueString( float total_value )
	{
		if( total_value < 0 )
			return "0";
	
		int value = total_value;
		string out_string;
		
		if ( value >= 1000 )
		{
			string value_string = value.ToString();
			
			int count;		
			int first_length = value_string.Length() % 3;		//calculate position of the first separator
			if ( first_length > 0 )
			{
				count = 3 - first_length;
			}
			
			for ( int i = 0; i < value_string.Length(); ++i )
			{
				out_string += value_string.Get( i );
				count ++;
				
				if ( count >= 3 )
				{
					out_string += " ";			//separator
					count = 0;
				}
			}
		}
		else
		{
			out_string = value.ToString();
		}
		
		return out_string;
	}
	
	// ------------------------------------------------------------
	// Override CanBeShow
	// ------------------------------------------------------------
	override bool CanBeShow()
	{
		return (GetExpansionSettings().GetBook().EnableServerInfoTab && !GetExpansionClientSettings().StreamerMode);
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick( Widget w, int x, int y, int button )
	{
		super.OnClick(w, x, y, button);

		if (w == m_book_ServerPageButton)
		{
			OnServerPageButtonClick();
			return true;
		}
		if (w == m_book_ServerForumsButton)
		{
			OnServerForumsButtonClick();
			return true;			
		}
		if (w == m_book_ServerDiscordButton)
		{
			OnServerDiscordButtonClick();
			return true;				
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion LoadServerInfos
	// ------------------------------------------------------------
	void LoadServerInfos()
	{
		if ( !m_ServerInfo )
		{
			if (GetExpansionSettings() && GetExpansionSettings().GetBook().ServerInfo != NULL )
				m_ServerInfo = GetExpansionSettings().GetBook().ServerInfo;
		}

		if ( m_ServerInfo )
		{
			string serverName = m_ServerInfo.serverName;
			string title1 = m_ServerInfo.infoTitle1;
			string text1 = m_ServerInfo.infoContent1;
			string title2 = m_ServerInfo.infoTitle2;
			string text2 = m_ServerInfo.infoContent2;
			string title3 = m_ServerInfo.infoTitle3;
			string text3 = m_ServerInfo.infoContent3;
			
			if ( serverName )
			{
				m_book_ServerName.Show(true);
				m_book_ServerName.SetText(serverName);
			}
			else
			{
				m_book_ServerName.Show(false);
			}
				
			if ( title1 )
			{
				m_book_ServerDescriptionTitle1.Show(true);
				m_book_ServerDescriptionTitle1.SetText( title1 );
			}
			else
			{
				m_book_ServerDescriptionTitle1.Show(false);
			}
				
			if ( text1 )
			{
				m_book_ServerDescriptionText1.Show(true);
				m_book_ServerDescriptionText1.SetText( text1 );
			}
			else
			{
				m_book_ServerDescriptionText1.Show(false);
			}
				
			if ( title2 )
			{
				m_book_ServerDescriptionTitle2.Show(true);
				m_book_ServerDescriptionTitle2.SetText( title2 );
			}
			else
			{
				m_book_ServerDescriptionTitle2.Show(false);
			}
				
			if ( text2 )
			{
				m_book_ServerDescriptionText2.Show(true);
				m_book_ServerDescriptionText2.SetText( text2 );
			}
			else
			{
				m_book_ServerDescriptionText2.Show(false);
			}
				
			if ( title3 )
			{
				m_book_ServerDescriptionTitle3.Show(true);
				m_book_ServerDescriptionTitle3.SetText( title3 );
			}
			else
			{
				m_book_ServerDescriptionTitle3.Show(false);
			}
				
			if ( text3 )
			{
				m_book_ServerDescriptionText3.Show(true);
				m_book_ServerDescriptionText3.SetText( text3 );
			}
			else
			{
				m_book_ServerDescriptionText3.Show(false);
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion OnServerPageButtonClick
	// ------------------------------------------------------------
	void OnServerPageButtonClick()
	{
		GetGame().OpenURL( m_book_ServerPageURL );
	}
	
	// ------------------------------------------------------------
	// Expansion OnServerForumsButtonClick
	// ------------------------------------------------------------
	void OnServerForumsButtonClick()
	{
		GetGame().OpenURL( m_book_ServerForumsURL );
	}
	
	// ------------------------------------------------------------
	// Expansion OnServerDiscordButtonClick
	// ------------------------------------------------------------
	void OnServerDiscordButtonClick()
	{
		GetGame().OpenURL( m_book_ServerDiscordURL );
	}
	
	// ------------------------------------------------------------
	// Expansion LoadServerLogo
	// ------------------------------------------------------------
	void LoadServerLogo()
	{
		if ( GetExpansionSettings().GetBook().ServerLogoPath )
		{
			m_book_ServerLogo.LoadImageFile(0, GetExpansionSettings().GetBook().ServerLogoPath);
		}
		else
		{
			m_book_ServerLogo.LoadImageFile(0, "set:expansion_gui_logos image:expansion_logo_black");
		}
	}
	
	// ------------------------------------------------------------
	// Override IsFocusable
	// ------------------------------------------------------------
	override bool IsFocusable( Widget w )
	{
		if( w )
		{
			return ( w == m_book_ServerPageButton || w == m_book_ServerForumsButton || w == m_book_ServerDiscordButton );
		}
		
		return false;
	}
}