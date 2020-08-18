/**
 * ExpansionMapMarkerIconItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMapMarkerIconItem extends ScriptedWidgetEventHandler
{
	private ExpansionIcon m_IconData;
	private ExpansionMapMarker m_Marker;

	private Widget m_LayoutRoot;
	private ButtonWidget m_Button;
	private ImageWidget m_Icon;
	private TextWidget m_Name;
	
	void ExpansionMapMarkerIconItem( Widget parent, ExpansionIcon data, ExpansionMapMarker marker )
	{
		Class.CastTo( m_LayoutRoot, GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/map/expansion_marker_entry.layout", parent ) );
		Class.CastTo( m_Button, m_LayoutRoot.FindAnyWidget( "marker_icon_button" ) );
		Class.CastTo( m_Icon, m_LayoutRoot.FindAnyWidget( "marker_icon_image" ) );
		Class.CastTo( m_Name, m_LayoutRoot.FindAnyWidget( "marker_icon_name" ) );

		m_IconData = data;
		m_Marker = marker;

		m_LayoutRoot.SetHandler( this );

		m_Icon.LoadImageFile( 0, m_IconData.Path );
		m_Icon.Show( true );

		m_Name.SetText( m_IconData.Name );
	}
	
	void ~ExpansionMapMarkerIconItem()
	{
		delete m_LayoutRoot;
	}
	
	string GetIconName()
	{
		return m_IconData.Name;
	}
	
	string GetMarkerIcon()
	{
		return m_IconData.Path;
	}

	ref ExpansionIcon GetIcon()
	{
		return m_IconData;
	}

	ButtonWidget GetButtonWidget()
	{
		return m_Button;
	}

	void SetColor( int color )
	{
		m_Icon.SetColor( color );
	}

	void UpdateSelection( ExpansionIcon icon )
	{
		if ( icon.Name == m_IconData.Name )
		{
			m_LayoutRoot.SetColor( ARGB( 100, 200, 0, 0 ) );
		} else
		{
			m_LayoutRoot.SetColor( ARGB( 100, 0, 0, 0 ) );
		}
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{
		if ( w != NULL && w == GetButtonWidget() )
		{
			m_Marker.SetIcon( GetIcon() );
			return true;
		}

		return super.OnClick( w, x, y, button );
	}
}