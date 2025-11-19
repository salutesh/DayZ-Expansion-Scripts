/**
 * OptionsMenuExpansion.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class OptionsMenuExpansion: ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	
	protected Widget m_DetailsRoot;
	protected TextWidget m_DetailsLabel;
	protected RichTextWidget m_DetailsText;

	protected GameOptions m_Options;
	protected ref OptionsMenu m_Menu;
	
	protected ref array<ref ExpansionOptionSettingWidget> m_OptionsWidgets;
	
	protected bool m_IsChanged;
	
	// -----------------------------------------------------------
	// OptionsMenuExpansion Constructor
	// -----------------------------------------------------------
	void OptionsMenuExpansion( Widget parent, Widget details_root, GameOptions options, OptionsMenu menu )
	{
		m_Root = g_Game.GetWorkspace().CreateWidgets( "DayZExpansion/Core/GUI/layouts/ui/options/expansion_tab.layout", parent );
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
			ExpansionClientSettingCategory category = GetExpansionClientSettings().m_Categories[i];
			Widget categoryWidget = Widget.Cast( g_Game.GetWorkspace().CreateWidgets( "DayZExpansion/Core/GUI/layouts/ui/options/expansion_options_category.layout", rootContent ) );
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
	
	void ~OptionsMenuExpansion()
	{
		if (m_OptionsWidgets)
		{
			foreach (auto w: m_OptionsWidgets)
			{
				if (w)
					delete w;
			}

			m_OptionsWidgets.Clear();
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
		if ( m_IsChanged )
		{
			m_IsChanged = false;
	
			for ( int i = 0; i < m_OptionsWidgets.Count(); i++ )
			{
				m_OptionsWidgets[i].Apply();
			}
	
			CF_ModuleCoreManager.OnSettingsChanged(this, CF_EventArgs.Empty);
			GetExpansionClientSettings().Save();
		}
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
		
		return true;
	}
	
	// -----------------------------------------------------------
	// OptionsMenuExpansion Override OnFocus
	// -----------------------------------------------------------
	override bool OnFocus( Widget w, int x, int y )
	{		
		return false;
	}
	
	// -----------------------------------------------------------
	// OptionsMenuExpansion Override OnFocusLost
	// -----------------------------------------------------------
	override bool OnFocusLost( Widget w, int x, int y )
	{
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