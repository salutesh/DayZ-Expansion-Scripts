/**
 * Expansion3DMarker.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion3DMarker: ScriptedWidgetEventHandler
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
#ifdef EXPANSIONMODGROUPS
	private ExpansionPartyModule m_PartyModule;
#endif
	
	private string m_LayoutPath;
	
	private bool m_ShowQuickName = true;
	private bool m_ShowQuickDistance = true;

	private bool m_ShowMemberName = true;
	private bool m_ShowMemberDistance = true;
	
	private bool m_ShowServerName = true;
	private bool m_ShowServerDistance = true;

	private bool m_ShowPersonalDistance = true;
	
	private bool m_Show3DPartyMemberIcon = true;
	
	void Expansion3DMarker( ExpansionMarkerData data = NULL )
	{
		GetSettings();
		
		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets( m_LayoutPath );
		OnWidgetScriptInit( m_LayoutRoot );

		SetMarkerData( data );

		CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule);
	#ifdef EXPANSIONMODGROUPS
		CF_Modules<ExpansionPartyModule>.Get(m_PartyModule);
	#endif
		
		GetExpansionClientSettings().SI_UpdateSetting.Insert( RefreshAlphaMinColor );
		GetExpansionClientSettings().SI_UpdateSetting.Insert( OnSettingChanged );
	}

	void ~Expansion3DMarker()
	{
		GetExpansionClientSettings().SI_UpdateSetting.Remove( RefreshAlphaMinColor );
		GetExpansionClientSettings().SI_UpdateSetting.Remove( OnSettingChanged );
		
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
		m_Frame.GetSize( m_OriginalWidth, m_OriginalHeight );
	}

	/**
	 * /return	boolean		False to remove the marker
	 */
	bool Update( float timeslice )
	{
		if ( !m_LayoutRoot || !m_MarkerData )
			return false;

	#ifdef EXPANSIONMODGROUPS
		if ( m_MarkerData.GetType() == ExpansionMapMarkerType.PARTY || m_MarkerData.GetType() == ExpansionMapMarkerType.PLAYER )
		{
			if ( !m_PartyModule.HasParty() )
				return false;
		}
	#endif

		if ( m_MarkerData.GetType() != ExpansionMapMarkerType.PARTY_QUICK )
		{
			if ( !m_MarkerData.Is3D() || !m_MarkerData.IsWorldVisible() )
				return false;

			if ( !m_MarkerModule.IsWorldVisible( m_MarkerData.GetType() ) )
				return false;
		}

		Hud inGameHUD = GetGame().GetMission().GetHud();
		if ( inGameHUD && !inGameHUD.Expansion_GetHudState() )
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
		m_Transparency = ExpansionMath.LinearConversion( 0, 0.15, dist, m_TransparencyMin, m_TransparencyMax );
		
		if ( m_MarkerData.GetType() == ExpansionMapMarkerType.SERVER )
		{
			m_Text_Name.Show( m_ShowServerName );
			m_Text_Distance.Show( m_ShowServerDistance );
		} 
		else if ( m_MarkerData.GetType() == ExpansionMapMarkerType.PERSONAL )
		{
			m_Text_Distance.Show( m_ShowPersonalDistance );
		}
	
	#ifdef EXPANSIONMODGROUPS
		if ( m_MarkerData.GetType() == ExpansionMapMarkerType.PARTY_QUICK )
		{
			m_Text_Name.Show( m_ShowQuickName );
			m_Text_Distance.Show( m_ShowQuickDistance );
		} 
		else if ( m_MarkerData.GetType() == ExpansionMapMarkerType.PLAYER )
		{
			m_Text_Name.Show( m_ShowMemberName );
			m_Text_Distance.Show( m_ShowMemberDistance );
			m_Image_Icon.Show( m_Show3DPartyMemberIcon );
		} 
	#endif

		m_Text_Name.SetText( m_MarkerData.GetName() );
		m_Text_Name.SetColor( ARGB( m_Transparency, 255, 255, 255) );
		
		//! Set distance
		float distance = vector.Distance( GetGame().GetCurrentCameraPosition(), position );
		m_Text_Distance.SetText( Math.Ceil( distance ).ToString() + "m" );
		m_Text_Distance.SetColor( ARGB( m_Transparency, 255, 255, 255) );

		float scale = ExpansionMath.LinearConversion( 2000, 100, distance, 0.6, 1 );
		m_Frame.SetSize( m_OriginalWidth * scale, m_OriginalHeight * scale );
		
		m_LayoutRoot.Show( true );
		m_LayoutRoot.SetPos( screen_position[0], screen_position[1] );

		m_Image_Icon.LoadImageFile( 0, m_MarkerData.GetIcon() );
		m_Image_Icon.SetImage( 0 );

		int markerColorA = 0;
		int markerColorR = 0;
		int markerColorG = 0;
		int markerColorB = 0;
		ExpansionStatic.IntToARGB( m_MarkerData.GetColor(), markerColorA, markerColorR, markerColorG, markerColorB );
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

	// ------------------------------------------------------------
	// Marker OnSettingChanged
	// ------------------------------------------------------------
	void OnSettingChanged()
	{		
		if ( m_LayoutRoot )
		{			
			m_LayoutRoot.Unlink();
		}

		GetSettings();
		
		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets( m_LayoutPath );
		
		OnWidgetScriptInit( m_LayoutRoot );
	}

	// ------------------------------------------------------------
	// Marker GetClientSettings
	// ------------------------------------------------------------
	void GetSettings()
	{
		if ( GetExpansionClientSettings() )
		{
			ExpansionClientUIMarkerSize markersize = GetExpansionClientSettings().MarkerSize;
			switch ( markersize )
			{
				case ExpansionClientUIMarkerSize.VERYSMALL:
					m_LayoutPath = "DayZExpansion/Navigation/GUI/layouts/expansion_dynamic_marker_16.layout";
					break;
				case ExpansionClientUIMarkerSize.SMALL:
					m_LayoutPath = "DayZExpansion/Navigation/GUI/layouts/expansion_dynamic_marker_24.layout";
					break;
				case ExpansionClientUIMarkerSize.MEDIUM:
					m_LayoutPath = "DayZExpansion/Navigation/GUI/layouts/expansion_dynamic_marker_32.layout";
					break;
				case ExpansionClientUIMarkerSize.LARGE:
					m_LayoutPath = "DayZExpansion/Navigation/GUI/layouts/expansion_dynamic_marker_40.layout";
					break;
			}
		}
		
		if ( GetExpansionSettings() )
		{
		#ifdef EXPANSIONMODGROUPS
			m_ShowQuickName = GetExpansionSettings().GetParty().ShowNameOnQuickMarkers;
			if (m_ShowQuickName)
			{
				m_ShowQuickName = GetExpansionClientSettings().ShowNameQuickMarkers;
			}

			m_ShowQuickDistance = GetExpansionSettings().GetParty().ShowDistanceUnderQuickMarkers;
			if (m_ShowQuickDistance)
			{
				m_ShowQuickDistance = GetExpansionClientSettings().ShowDistanceQuickMarkers;
			}
			
			m_ShowMemberName = GetExpansionSettings().GetParty().ShowNameOnPartyMembersMarkers;
			if ( GetExpansionSettings().GetParty().ShowNameOnPartyMembersMarkers )
			{
				m_ShowMemberName = GetExpansionClientSettings().ShowMemberNameMarker;
			}

			m_ShowMemberDistance = GetExpansionSettings().GetParty().ShowDistanceUnderPartyMembersMarkers;
			if (m_ShowMemberDistance)
			{
				m_ShowMemberDistance = GetExpansionClientSettings().ShowMemberDistanceMarker;
			}
		#endif
			
			m_ShowServerName = GetExpansionSettings().GetMap().ShowNameOnServerMarkers;
			m_ShowServerDistance = GetExpansionSettings().GetMap().ShowDistanceOnServerMarkers;

			m_ShowPersonalDistance = GetExpansionSettings().GetMap().ShowDistanceOnPersonalMarkers;
			
			m_Show3DPartyMemberIcon = GetExpansionClientSettings().Show3DPartyMemberIcon;
		}
	}
}