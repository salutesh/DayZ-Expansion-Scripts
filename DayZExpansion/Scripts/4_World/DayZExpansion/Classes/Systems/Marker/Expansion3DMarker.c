/**
 * Expansion3DMarker.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion3DMarker extends ScriptedWidgetEventHandler
{
	private Widget m_LayoutRoot;
	private Widget m_Frame;
	private TextWidget m_Text_Distance;
	private TextWidget m_Text_Name;
	private ImageWidget m_Image_Icon;

	private ExpansionMarkerData m_MarkerData;

	private int m_Transparency;
	private int m_TransparencyMin;
	private int m_TransparencyMax;

	private float m_OriginalWidth;
	private float m_OriginalHeight;

	private ExpansionMarkerModule m_MarkerModule;

	void Expansion3DMarker( ExpansionMarkerData data = NULL )
	{
		//Print( "Expansion3DMarker::Expansion3DMarker" );
		//Print( this );

	   	m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/expansion_dynamic_marker.layout" );
		OnWidgetScriptInit( m_LayoutRoot );

		SetMarkerData( data );

		Class.CastTo( m_MarkerModule, GetModuleManager().GetModule( ExpansionMarkerModule ) );
		
		GetExpansionClientSettings().SI_UpdateSetting.Insert( RefreshAlphaMinColor );
	}

	void ~Expansion3DMarker()
	{
		//Print( "Expansion3DMarker::~Expansion3DMarker" );
		//Print( this );

		GetExpansionClientSettings().SI_UpdateSetting.Remove( RefreshAlphaMinColor );
		
		if ( m_LayoutRoot )
		{
			m_LayoutRoot.Show( false );
			
			m_LayoutRoot.Unlink();
		}
	}

	Widget GetLayoutRoot()
	{
		return m_LayoutRoot;
	}

	void OnWidgetScriptInit( Widget w )
	{
		m_LayoutRoot = w;
		m_LayoutRoot.SetHandler( this );

		Class.CastTo( m_Image_Icon, m_LayoutRoot.FindAnyWidget( "MarkerImage" ) );
		Class.CastTo( m_Text_Name, m_LayoutRoot.FindAnyWidget( "MarkerText" ) );
		Class.CastTo( m_Text_Distance, m_LayoutRoot.FindAnyWidget( "MarkerDistance" ) );
		Class.CastTo( m_Frame, m_LayoutRoot.FindAnyWidget( "MarkerFrame" ) );
						
		m_Text_Distance.Show( true );
	
		m_Frame.GetSize( m_OriginalWidth, m_OriginalHeight );
	}

	/**
	 * /return	boolean		False to remove the marker
	 */
	bool Update( float timeslice )
	{
		if ( !m_LayoutRoot || !m_MarkerData )
			return false;

		if ( m_MarkerData.GetType() != ExpansionMapMarkerType.PARTY_QUICK )
		{
			if ( !m_MarkerData.Is3D() || !m_MarkerData.IsWorldVisible() )
				return false;

			if ( !m_MarkerModule.IsWorldVisible( m_MarkerData.GetType() ) )
				return false;
		}

		Hud inGameHUD = GetGame().GetMission().GetHud();
		if ( inGameHUD && !inGameHUD.GetHudState() )
		{
			m_LayoutRoot.Show( false );
			return true;
		}

		vector position = m_MarkerData.GetAdjustedPosition();
		vector screen_position = GetGame().GetScreenPosRelative( position );
		
		if ( screen_position[0] >= 1 || screen_position[0] <= 0 || screen_position[1] >= 1 || screen_position[1] <= 0 || screen_position[2] <= 0 )
		{
			m_LayoutRoot.Show( false );
			return true;
		}

		m_TransparencyMax = ( m_MarkerData.GetColor() >> 24 ) & 0xFF;

		float dist = vector.Distance( screen_position, Vector( 0.5, 0.5, screen_position[2] ) );
		m_Transparency = LinearConversion( 0, 0.15, dist, m_TransparencyMin, m_TransparencyMax );

		if ( m_MarkerData.GetName() == "" || m_MarkerData.GetType() == ExpansionMapMarkerType.PARTY_QUICK && !GetExpansionSettings().GetParty().ShowNameOnQuickMarkers )
		{
			m_Text_Name.Show( false );
		}
		else if (m_MarkerData.GetName() != "")
		{
			m_Text_Name.Show( true );
			m_Text_Name.SetText( m_MarkerData.GetName() );
			m_Text_Name.SetColor( ARGB( m_Transparency, 255, 255, 255) );
		}

		if ( m_MarkerData.GetType() == ExpansionMapMarkerType.PARTY_QUICK && !GetExpansionSettings().GetParty().ShowDistanceUnderQuickMarkers )
		{
			m_Text_Distance.Show( false );
		}
		else
		{
			float distance = vector.Distance( GetGame().GetCurrentCameraPosition(), position );
			m_Text_Distance.SetText( Math.Ceil( distance ).ToString() + "m" );
			m_Text_Distance.SetColor( ARGB( m_Transparency, 255, 255, 255) );
		}

		float scale = LinearConversion( 2000, 100, distance, 0.6, 1 );
		m_Frame.SetSize( m_OriginalWidth * scale, m_OriginalHeight * scale );
		
		m_LayoutRoot.Show( true );
		m_LayoutRoot.SetPos( screen_position[0], screen_position[1] );

		m_Image_Icon.LoadImageFile( 0, m_MarkerData.GetIcon().Path );
		m_Image_Icon.SetImage( 0 );

		int markerColorA = 0;
		int markerColorR = 0;
		int markerColorG = 0;
		int markerColorB = 0;
		IntToARGB( m_MarkerData.GetColor(), markerColorA, markerColorR, markerColorG, markerColorB );
		m_Image_Icon.SetColor( ARGB( m_Transparency, markerColorR, markerColorG, markerColorB ) );

		return true;
	}

	void SetMarkerData( ExpansionMarkerData data )
	{
		m_MarkerData = data;
		
		RefreshAlphaMinColor();

		m_Text_Name.SetColor( ARGB( m_TransparencyMax, 255, 255, 255 ) );
		m_Text_Distance.SetColor( ARGB( m_TransparencyMax, 255, 255, 255 ) );
	}

	ExpansionMarkerData GetMarkerData()
	{
		return m_MarkerData;
	}
	
	void RefreshAlphaMinColor()
	{
		m_TransparencyMin = GetExpansionClientSettings().AlphaColorLookAtMinimum;
		
		m_Transparency = Math.Clamp( m_Transparency, m_TransparencyMin, m_TransparencyMax );
	}
}