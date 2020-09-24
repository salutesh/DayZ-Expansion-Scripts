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
	
	protected ImageWidget m_book_ServerLogo;
	
	protected WrapSpacerWidget m_ServerContentWraper;
	
	protected Widget m_ServerSettingsPanel;
	protected WrapSpacerWidget m_ServerSettingsWraper;

	ref array<ref ExpansionBookTabServerInfoButton> m_ServerButtons;
	ref array<ref ExpansionBookTabServerInfoSection> m_ServerSections;
	
	ref array<ref ExpansionServerInfoSetting> m_Settings;
	ref array<ref ExpansionBookTabServerInfoSetting> m_ServerSettings;
	
	// ------------------------------------------------------------
	// ExpansionBookTabServerInfo Contructor
	// ------------------------------------------------------------
	void ExpansionBookTabServerInfo()
	{
		SetButtonIconPath( ExpansionIcons.GetPath("Info"));
		SetButtonText( "#STR_BOOK_SERVERINFOMENU" );
		SetButtonColor( COLOR_EXPANSION_SERVERINFO_BOOKMARK_ICON );
		SetLayoutPath( "DayZExpansion/GUI/layouts/book/expansion_book_page_serverinfo.layout" );
		
		m_ServerButtons = new array<ref ExpansionBookTabServerInfoButton>;
		m_ServerSections = new array<ref ExpansionBookTabServerInfoSection>;
		
		m_Settings = new array<ref ExpansionServerInfoSetting>;
		m_ServerSettings = new array<ref ExpansionBookTabServerInfoSetting>;
	}
	
	// ------------------------------------------------------------
	// Override CanBeShow
	// ------------------------------------------------------------
	override bool CanBeShow()
	{
		return ( GetExpansionSettings().GetBook().EnableServerInfoTab && !GetExpansionClientSettings().StreamerMode );
	}
	
	// ------------------------------------------------------------
	// Override OnCreatedLayout
	// ------------------------------------------------------------
	//Layout root var is m_RootLayout
	override void OnCreatedLayout()
	{
		m_book_ServerName						= TextWidget.Cast( m_RootLayout.FindAnyWidget( "server_name_label" ) );
	
		m_book_ServerLogo						= ImageWidget.Cast( m_RootLayout.FindAnyWidget( "server_info_logo" ) );
		
		m_ServerContentWraper					= WrapSpacerWidget.Cast( m_RootLayout.FindAnyWidget( "server_description_scroller_content" ) );
		
		m_ServerSettingsPanel					= WrapSpacerWidget.Cast( m_RootLayout.FindAnyWidget( "server_settings_content" ) );
		m_ServerSettingsWraper					= WrapSpacerWidget.Cast( m_RootLayout.FindAnyWidget( "server_settings_scroller_content" ) );
		
		LoadServerInfos();
		LoadServerLogo();
		SetServerSettings();
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
			RefreshButtons();
			RefreshSections();	
		}
	}
	
	// ------------------------------------------------------------
	// Expansion SetServerSettings
	// ------------------------------------------------------------
	void SetServerSettings()
	{
		if ( !GetExpansionSettings().GetBook().ShowServerSettings )
		{
			m_ServerSettingsPanel.Show( false );
		}
		else
		{
			RefreshSettings();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion RefreshSettings
	// ------------------------------------------------------------
	void RefreshSettings()
	{
		if ( m_ServerSettings.Count() > 0 )
		{
			for ( int i = 0; i < m_ServerSettings.Count(); i++ )
			{
				m_ServerSettings.Remove( i );
			}
		}	
		
		m_ServerSettings.Clear();
		
		GetSettings();
		
		for ( int j = 0; j < m_Settings.Count(); j++ )
		{
			ExpansionBookTabServerInfoSetting setting = new ExpansionBookTabServerInfoSetting(m_ServerSettingsWraper, m_Settings[j]);
			m_ServerSettings.Insert( setting );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion GetSettings
	// ------------------------------------------------------------
	void GetSettings()
	{
		if ( m_Settings.Count() > 0 )
		{
			for ( int i = 0; i < m_Settings.Count(); i++ )
			{
				m_Settings.Remove( i );
			}
		}	
		
		m_Settings.Clear();
		
		ExpansionServerInfoSetting setting;
		
		//! CanCraftVanillaBasebuilding
		setting = new ExpansionServerInfoSetting( "#STR_EXPANSION_SETTINGS_BB_CANCRAFTVANILLABASEBUILDING", "#STR_EXPANSION_SETTINGS_BB_CANCRAFTVANILLABASEBUILDING_DESC", true, GetExpansionSettings().GetBaseBuilding().CanCraftVanillaBasebuilding );
		m_Settings.Insert( setting );
		
		//! CanCraftExpansionBasebuilding
		setting = new ExpansionServerInfoSetting( "#STR_EXPANSION_SETTINGS_BB_CANCRAFTEXPANSIONBASEBUILDING", "#STR_EXPANSION_SETTINGS_BB_CANCRAFTEXPANSIONBASEBUILDING_DESC", true, GetExpansionSettings().GetBaseBuilding().CanCraftExpansionBasebuilding );
		m_Settings.Insert( setting );
		
		//! CanRaidSafes
		setting = new ExpansionServerInfoSetting( "#STR_EXPANSION_SETTINGS_BB_CANRAIDSAFES", "#STR_EXPANSION_SETTINGS_BB_CANRAIDSAFES_DESC", true, GetExpansionSettings().GetRaid().CanRaidSafes );
		m_Settings.Insert( setting );
		
		//! SafeExplosionDamageMultiplier
		setting = new ExpansionServerInfoSetting( "#STR_EXPANSION_SETTINGS_BB_SAFE_EXPLOSIONDAMAGEMULTIPLIER", "#STR_EXPANSION_SETTINGS_BB_SAFE_EXPLOSIONDAMAGEMULTIPLIER_DESC", false, false, GetExpansionSettings().GetRaid().SafeExplosionDamageMultiplier.ToString() );
		m_Settings.Insert( setting );
		
		//! SafeProjectileDamageMultiplier
		setting = new ExpansionServerInfoSetting( "#STR_EXPANSION_SETTINGS_BB_SAFE_PROJECTILEDAMAGEMULTIPLIER", "#STR_EXPANSION_SETTINGS_BB_SAFE_PROJECTILEDAMAGEMULTIPLIER_DESC", false, false, GetExpansionSettings().GetRaid().SafeProjectileDamageMultiplier.ToString() );
		m_Settings.Insert( setting );
		
		//! ExplosionTime
		setting = new ExpansionServerInfoSetting( "#STR_EXPANSION_SETTINGS_BB_EXPLOSIONTIME", "#STR_EXPANSION_SETTINGS_BB_EXPLOSIONTIME_DESC", false, false, GetExpansionSettings().GetRaid().ExplosionTime.ToString() + " s" );
		m_Settings.Insert( setting );
		
		//! ExplosionDamageMultiplier
		setting = new ExpansionServerInfoSetting( "#STR_EXPANSION_SETTINGS_BB_EXPLOSIONDAMAGEMULTIPLIER", "#STR_EXPANSION_SETTINGS_BB_EXPLOSIONDAMAGEMULTIPLIER_DESC", false, false, GetExpansionSettings().GetRaid().ExplosionDamageMultiplier.ToString() );
		m_Settings.Insert( setting );

		//! ProjectileDamageMultiplier
		setting = new ExpansionServerInfoSetting( "#STR_EXPANSION_SETTINGS_BB_PROJECTILEDAMAGEMULTIPLIER", "#STR_EXPANSION_SETTINGS_BB_PROJECTILEDAMAGEMULTIPLIER_DESC", false, false, GetExpansionSettings().GetRaid().ProjectileDamageMultiplier.ToString() );
		m_Settings.Insert( setting );
		
		//! NeedGPSItemForKeyBinding
		setting = new ExpansionServerInfoSetting( "#STR_EXPANSION_SETTINGS_GEN_NEEDGPSITEMFORKEYBINDING", "#STR_EXPANSION_SETTINGS_GEN_NEEDGPSITEMFORKEYBINDING_DESC", true, GetExpansionSettings().GetMap().NeedGPSItemForKeyBinding );
		m_Settings.Insert( setting );
		
		//! NeedMapItemForKeyBinding
		setting = new ExpansionServerInfoSetting( "#STR_EXPANSION_SETTINGS_GEN_NEEDMAPITEMFORKEYBINDING", "#STR_EXPANSION_SETTINGS_GEN_NEEDMAPITEMFORKEYBINDING_DESC", true, GetExpansionSettings().GetMap().NeedMapItemForKeyBinding );
		m_Settings.Insert( setting );

		//! MaxInParty
		setting = new ExpansionServerInfoSetting( "#STR_EXPANSION_SETTINGS_P_MAXINPARTY", "#STR_EXPANSION_SETTINGS_P_MAXINPARTY_DESC", false, false, GetExpansionSettings().GetParty().MaxMembersInParty.ToString() );
		m_Settings.Insert( setting );
		
		//! UseWholeMapForInviteList
		setting = new ExpansionServerInfoSetting( "#STR_EXPANSION_SETTINGS_P_USEWHOLEMAPFORINVITELIST", "#STR_EXPANSION_SETTINGS_P_USEWHOLEMAPFORINVITELIST_DESC", true, GetExpansionSettings().GetParty().UseWholeMapForInviteList );
		m_Settings.Insert( setting );

		//! TerritorySize
		setting = new ExpansionServerInfoSetting( "#STR_EXPANSION_SETTINGS_T_TERRITORYSIZE", "#STR_EXPANSION_SETTINGS_T_TERRITORYSIZE_DESC", false, false, GetExpansionSettings().GetTerritory().TerritorySize.ToString() + " m" );
		m_Settings.Insert( setting );
	}
	
	// ------------------------------------------------------------
	// Expansion RefreshButtons
	// ------------------------------------------------------------
	void RefreshButtons()
	{
		if ( m_ServerButtons.Count() > 0 )
		{
			for ( int i = 0; i < m_ServerButtons.Count(); i++ )
			{
				m_ServerButtons.Remove( i );
			}
		}	
		
		m_ServerButtons.Clear();
		
		ExpansionBook bookmenu = ExpansionBook.Cast( GetGame().GetUIManager().FindMenu( MENU_EXPANSION_BOOK_MENU ) );
		for ( int j = 0; j < m_ServerInfo.ServerButtons.Count(); j++ )
		{
			ExpansionBookTabServerInfoButton button = new ExpansionBookTabServerInfoButton(bookmenu.GetLeftBookMarkGrid(), m_ServerInfo.ServerButtons[j].IconPath, m_ServerInfo.ServerButtons[j].URL, m_ServerInfo.ServerButtons[j].Tooltip,  m_ServerInfo.ServerButtons[j].Color);
			m_ServerButtons.Insert( button );
		}
	}
		
	// ------------------------------------------------------------
	// Expansion RefreshSections
	// ------------------------------------------------------------
	void RefreshSections()
	{
		if ( m_ServerSections.Count() > 0 )
		{
			for ( int i = 0; i < m_ServerSections.Count(); i++ )
			{
				m_ServerSections.Remove( i );
			}
		}
		
		m_ServerSections.Clear();
		
		for ( int j = 0; j < m_ServerInfo.ServerSections.Count(); j++ )
		{
			ExpansionBookTabServerInfoSection section = new ExpansionBookTabServerInfoSection(m_ServerContentWraper, m_ServerInfo.ServerSections[j].HasHeading, m_ServerInfo.ServerSections[j].HeadText, m_ServerInfo.ServerSections[j].BodyText);
			m_ServerSections.Insert( section );
		}
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
	
	// ------------------------------------------------------------
	// Override IsServerInfoTab
	// ------------------------------------------------------------
	override bool IsServerInfoTab()
	{
		return true;
	}
	
	// ------------------------------------------------------------
	// Expansion HideTooltips
	// ------------------------------------------------------------	
	void HideTooltips()
	{
		if (m_ServerSettings && m_ServerSettings.Count() > 0)
		{
			for (int i = 0; i < m_ServerSettings.Count(); i++)
			{
				if ( m_ServerSettings[i].GetTooltip().IsVisible() )
					m_ServerSettings[i].GetTooltip().HideTooltip();
			}
		}
		
		if (m_ServerButtons && m_ServerButtons.Count() > 0)
		{
			for (int j = 0; j < m_ServerButtons.Count(); j++)
			{
				if ( m_ServerButtons[i].GetTooltip().IsVisible() )
					m_ServerButtons[i].GetTooltip().HideTooltip();
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion OnTabOpen
	// ------------------------------------------------------------	
	override void OnTabOpen()
	{

	}
	
	// ------------------------------------------------------------
	// Expansion OnTabClose
	// ------------------------------------------------------------	
	override void OnTabClose()
	{
		m_ServerSettings.Clear();
		m_ServerButtons.Clear();
		HideTooltips();
	}
}