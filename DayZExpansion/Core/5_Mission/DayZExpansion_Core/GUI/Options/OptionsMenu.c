/**
 * OptionsMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class OptionsMenu
{
	protected ref OptionsMenuExpansion m_ExpansionTab;
	
	protected Widget m_OptionsPanel;
	protected Widget m_TabberPanel;
	
	override Widget Init()
	{
		m_Options		= new GameOptions();
		
		layoutRoot		= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/options/pc/options_menu.layout", null );
		
		layoutRoot.FindAnyWidget( "Tabber" ).Unlink();
		layoutRoot.FindAnyWidget( "play_panel_root" ).Unlink();
		
		m_OptionsPanel = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/Core/GUI/layouts/ui/options/expansion_options_menu_tabber.layout", layoutRoot );
		m_TabberPanel = m_OptionsPanel.FindAnyWidget( "ExpansionTabber" );
		m_TabberPanel.GetScript( m_Tabber );
		
		m_Details			= layoutRoot.FindAnyWidget( "settings_details" );
		m_Version			= TextWidget.Cast( layoutRoot.FindAnyWidget( "version" ) );
		
		m_GameTab			= new OptionsMenuGame( layoutRoot.FindAnyWidget( "Tab_0" ), m_Details, m_Options, this );
		m_SoundsTab			= new OptionsMenuSounds( layoutRoot.FindAnyWidget( "Tab_1" ), m_Details, m_Options, this );
		m_VideoTab			= new OptionsMenuVideo( layoutRoot.FindAnyWidget( "Tab_2" ), m_Details, m_Options, this );
		m_ControlsTab		= new OptionsMenuControls( layoutRoot.FindAnyWidget( "Tab_3" ), m_Details, m_Options, this );
		m_ExpansionTab		= new OptionsMenuExpansion( layoutRoot.FindAnyWidget( "Tab_4" ), m_Details, m_Options, this );
		
		m_Apply				= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "apply" ) );
		m_Back				= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "back" ) );
		m_Reset				= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "reset" ) );
		
		SetGameVersion();
		
		SetFocus( layoutRoot );
		
		m_Tabber.m_OnTabSwitch.Insert( OnTabSwitch );
		
		OnChanged();
		
		return layoutRoot;
	}
	
	override void OnTabSwitch( int tab )
	{
		switch( tab )
		{
			case 0:
			{
				m_GameTab.Focus();
				break;
			}
			case 1:
			{
				m_SoundsTab.Focus();
				break;
			}
			case 2:
			{
				m_VideoTab.Focus();
				break;
			}
			case 3:
			{
				m_ControlsTab.Focus();
				break;
			}
			case 4:
			{
				// Just do nothing
				break;
			}
		}
	}
	
	override void OnChanged()
	{
		bool changed = false;
		if( m_Options.IsChanged() || m_GameTab.IsChanged() || m_SoundsTab.IsChanged() || m_ControlsTab.IsChanged() )
		{
			changed = true;
		}
		#ifdef PLATFORM_WINDOWS
		#ifndef PLATFORM_CONSOLE
		else if( m_VideoTab.IsChanged() )
		{
			changed = true;
		}
		else if( m_ExpansionTab.IsChanged() )
		{
			changed = true;
		}
		#endif
		#endif
		
		#ifdef PLATFORM_CONSOLE
			layoutRoot.FindAnyWidget( "Apply" ).Show( changed );
			layoutRoot.FindAnyWidget( "Reset" ).Show( changed );
		#else
		#ifdef PLATFORM_WINDOWS
			//m_Apply.Enable( changed );
			//m_Reset.Enable( changed );
		
			if( changed )
			{
				//m_Reset.ClearFlags( WidgetFlags.IGNOREPOINTER );
				//ColorNormal( m_Reset );
				m_Apply.ClearFlags( WidgetFlags.IGNOREPOINTER );
				ColorNormal( m_Apply );
			}
			else
			{
				m_Apply.SetFlags( WidgetFlags.IGNOREPOINTER );
				ColorDisable( m_Apply );
				//m_Reset.SetFlags( WidgetFlags.IGNOREPOINTER );
				//ColorDisable( m_Reset );
				
			}
		#endif
		#endif
		
		m_Tabber.AlignTabbers();
	}

	override void Apply()
	{
		m_ControlsTab.Apply();
		
		if( m_Options.IsChanged() || m_GameTab.IsChanged() )
		{
			m_Options.Test();
			m_Options.Apply();
		}
		
		if( m_GameTab.IsChanged() )
		{
			m_GameTab.Apply();
		}
		
		if( m_ExpansionTab.IsChanged() )
		{
			m_ExpansionTab.Apply();
		}
		
		// save input configuration
		GetUApi().Export();	
		
		#ifdef PLATFORM_CONSOLE
			layoutRoot.FindAnyWidget( "Reset" ).Show( false );
			layoutRoot.FindAnyWidget( "Apply" ).Show( false );
		#else
		#ifdef PLATFORM_WINDOWS
			//m_Apply.Enable( false );
			m_Apply.SetFlags( WidgetFlags.IGNOREPOINTER );
			ColorDisable(m_Apply);
			//m_Reset.Enable( false );
			//m_Reset.SetFlags( WidgetFlags.IGNOREPOINTER );
			//ColorDisable(m_Reset);
		#endif
		#endif
		
		if( m_Options.NeedRestart() )
			g_Game.GetUIManager().ShowDialog("#main_menu_configure", "#menu_restart_needed", 117, DBT_YESNO, DBB_YES, DMT_QUESTION, this);
	}
	
	protected void SetGameVersion()
	{
		TextWidget version_widget = TextWidget.Cast( layoutRoot.FindAnyWidget("version") );

		string version;
		GetGame().GetVersion( version );
		string expansion_version;	
		
		if ( GetDayZGame() )
		{
			expansion_version = GetDayZGame().GetExpansionClientVersion();
			m_Version.SetText( "DayZ SA #main_menu_version" + " " + version + "   DayZ Expansion #main_menu_version" + " " + expansion_version );
		} else
		{
			m_Version.SetText( "DayZ SA #main_menu_version" + " " + version );
		}
	}
	
	override void Refresh()
	{
		SetGameVersion();
	}
}
