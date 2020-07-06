/**
 * OptionsMenuExpansion.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class OptionsMenuExpansion extends ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	
	protected Widget m_DetailsRoot;
	protected TextWidget m_DetailsLabel;
	protected RichTextWidget m_DetailsText;

	protected GameOptions m_Options;
	protected OptionsMenu m_Menu;
	
	protected ref array<ref ExpansionOptionSettingWidget> m_OptionsWidgets;
	
	protected bool m_IsChanged;
	
	//! Expansion Hud
	ref ExpansionIngameHud 						m_ExpansionHud;
	
	// -----------------------------------------------------------
	// OptionsMenuExpansion Constructor
	// -----------------------------------------------------------
	void OptionsMenuExpansion( Widget parent, Widget details_root, GameOptions options, OptionsMenu menu )
	{
		m_Root = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/ui/options/expansion_tab.layout", parent );
		m_Root.SetHandler( this );
		
		if ( !m_OptionsWidgets )
			m_OptionsWidgets = new array<ref ExpansionOptionSettingWidget>;
		else
			m_OptionsWidgets.Clear();

		m_DetailsRoot = details_root;
		m_DetailsLabel = TextWidget.Cast( m_DetailsRoot.FindAnyWidget( "details_label" ) );
		m_DetailsText = RichTextWidget.Cast( m_DetailsRoot.FindAnyWidget( "details_content" ) );
		
		m_Options = options;
		m_Menu = menu;

		Widget rootContent = Widget.Cast( m_Root.FindAnyWidget( "expansion_tab_content" ) );
		
		for ( int i = 0; i < GetExpansionClientSettings().m_Categories.Count(); i++ )
		{
			ref ExpansionClientSettingCategory category = GetExpansionClientSettings().m_Categories[i];
			Widget categoryWidget = Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/ui/options/expansion_options_category.layout", rootContent ) );
			TextWidget categoryLable = TextWidget.Cast( categoryWidget.FindAnyWidget( "expansion_options_category_header_text" ) );
			categoryLable.SetText( category.m_DisplayName );
			GridSpacerWidget categoryGrid = GridSpacerWidget.Cast( categoryWidget.FindAnyWidget( "expansion_options_category_content" ) );
			
			for ( int j = 0; j < category.m_Settings.Count(); j++ )
			{
				ExpansionOptionSettingWidget setting = new ExpansionOptionSettingWidget( categoryGrid, category.m_Settings[j], this );
				
				if ( setting )
				{
					m_OptionsWidgets.Insert( setting );
				}
			}
		}
	}
	
	// -----------------------------------------------------------
	// OptionsMenuExpansion IsChanged
	// -----------------------------------------------------------
	bool IsChanged()
	{
		return m_IsChanged;
	}
	
	// -----------------------------------------------------------
	// OptionsMenuExpansion SetChanged
	// -----------------------------------------------------------
	void SetChanged()
	{
		m_IsChanged = true;

		m_Menu.OnChanged();
	}
	
	// -----------------------------------------------------------
	// OptionsMenuExpansion Apply
	// -----------------------------------------------------------
	void Apply()
	{
		m_IsChanged = false;

		for( int i = 0; i < m_OptionsWidgets.Count(); i++ )
		{
			m_OptionsWidgets[i].Apply();
		}

		GetModuleManager().OnSettingsUpdated();

		GetExpansionClientSettings().Save();

		//GetGame().GetWorld().SetPreferredViewDistance(GetExpansionClientSettings().DrawDistance);
		//GetGame().GetWorld().SetPreferredViewDistance(1600.0);

		PPEffects.UpdateSaturation();
		PPEffects.UpdateVignette();
		
		 
		MissionGameplay missiongameplay = MissionGameplay.Cast( GetGame().GetMission() );
		if (missiongameplay.GetExpansionHud().GetEarplugsState())
			missiongameplay.GetExpansionHud().UpdateEarplugs();
	}
	
	// -----------------------------------------------------------
	// OptionsMenuExpansion Override OnMouseEnter
	// -----------------------------------------------------------
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		if ( w && w.IsInherited( ScrollWidget ) )
		{
			return false;
		}
		
		if ( w && w.IsInherited( SliderWidget ) )
		{
			return false;
		}
		
		//m_Menu.ColorHighlight( w );
		
		return true;
	}
	
	// -----------------------------------------------------------
	// OptionsMenuExpansion Override OnMouseLeave
	// -----------------------------------------------------------
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		if ( w && w.IsInherited( ScrollWidget ) )
		{
			return false;
		}
		
		//m_Menu.ColorNormal( w );

		return true;
	}
	
	// -----------------------------------------------------------
	// OptionsMenuExpansion Revert
	// -----------------------------------------------------------
	void Revert()
	{
	
	}
	
	// -----------------------------------------------------------
	// OptionsMenuExpansion Override OnFocus
	// -----------------------------------------------------------
	override bool OnFocus( Widget w, int x, int y )
	{		
		/*if( IsFocusable( w ) )
		{
			ColorRed( w );
			return true;
		}*/
		
		//m_DetailsRoot.Show( false );
		
		return ( w != null );
	}
	
	// -----------------------------------------------------------
	// OptionsMenuExpansion Override OnFocusLost
	// -----------------------------------------------------------
	override bool OnFocusLost( Widget w, int x, int y )
	{
		/*if ( IsFocusable( w ) )
		{
			ColorWhite( w, null );
			return true;
		}*/

		return false;
	}
	
	// -----------------------------------------------------------
	// OptionsMenuExpansion IsFocusable
	// -----------------------------------------------------------
	bool IsFocusable( Widget w )
	{
		if ( w )
		{
			return true;
		}
		return false;
	}
	
	// -----------------------------------------------------------
	// OptionsMenuExpansion ColorRed
	// -----------------------------------------------------------
	void ColorRed( Widget w )
	{
		SetFocus( w );
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
			button.SetTextColor( ARGB( 255, 255, 0, 0 ) );
			button.SetAlpha( 0.9 );
		}
	}
	
	// -----------------------------------------------------------
	// OptionsMenuExpansion ColorWhite
	// -----------------------------------------------------------
	void ColorWhite( Widget w, Widget enterW )
	{
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
			button.SetTextColor( ARGB( 255, 255, 255, 255 ) );
			button.SetAlpha( 0.75 );
		}
	}
	
	// -----------------------------------------------------------
	// OptionsMenuExpansion Focus
	// -----------------------------------------------------------
	void Focus()
	{
	
	}
	
	// -----------------------------------------------------------
	// OptionsMenuExpansion ShowDetails
	// -----------------------------------------------------------
	void ShowDetails( string title, string text )
	{
		m_DetailsRoot.Show( true );
		m_DetailsLabel.SetText( title );
		m_DetailsText.SetText( text );
	}

	// -----------------------------------------------------------
	// OptionsMenuExpansion ShowDetails
	// -----------------------------------------------------------
	void HideDetails()
	{
		m_DetailsRoot.Show( false );
		m_DetailsLabel.SetText( "" );
		m_DetailsText.SetText( "" );
	}
}