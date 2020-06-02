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
		
		m_OptionsPanel = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/ui/options/expansion_options_menu_tabber.layout", layoutRoot );
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
				m_ExpansionTab.Focus();
				break;
			}
		}
	}

	override void OnChanged()
	{
		bool changed = false;
		if( m_Options.IsChanged() || m_GameTab.IsChanged() || m_SoundsTab.IsChanged() || m_ControlsTab.IsChanged() || m_ExpansionTab.IsChanged() )
		{
			changed = true;
		}
		
		if( m_VideoTab.IsChanged() )
		{
			changed = true;
		}
		
		if ( changed )
		{
			m_Apply.ClearFlags( WidgetFlags.IGNOREPOINTER );
			ColorNormal( m_Apply );
		} else
		{
			m_Apply.SetFlags( WidgetFlags.IGNOREPOINTER );
			ColorDisable( m_Apply );
		}
		
		m_Tabber.AlignTabbers();
	}

	override void Apply()
	{
		super.Apply();
		
		if ( m_ExpansionTab.IsChanged() )
		{
			m_ExpansionTab.Apply();
		}
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
