/**
 * ExpansionMapMarkerPlayerArrow.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMapMarkerPlayerArrow : ExpansionMapMarker 
{	
	void ExpansionMapMarkerPlayerArrow( Widget parent, MapWidget mapWidget, bool autoInit = true )
	{
		SetIgnorePointer(true);
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarkerPlayerArrow OnEditInit
	// ------------------------------------------------------------	
	/*protected override void OnEditInit( Widget layoutRoot )
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerPlayerArrow::OnEditInit - Start");
		#endif
		
		Class.CastTo( m_RightButton, layoutRoot.FindAnyWidget( "marker_button_right" ) );

		Class.CastTo( m_ColorRGB_Red, layoutRoot.FindAnyWidget( "marker_color_picker_rgb_red_slider" ) );
		Class.CastTo( m_ColorRGB_Green, layoutRoot.FindAnyWidget( "marker_color_picker_rgb_green_slider" ) );
		Class.CastTo( m_ColorRGB_Blue, layoutRoot.FindAnyWidget( "marker_color_picker_rgb_blue_slider" ) );

		Class.CastTo( m_State3DContainer, layoutRoot.FindAnyWidget( "marker_state_3d_container" ) );
		Class.CastTo( m_StatePartyContainer, layoutRoot.FindAnyWidget( "marker_state_party_container" ) );

		Class.CastTo( m_State3DCheckbox, layoutRoot.FindAnyWidget( "marker_state_3d_checkbox" ) );
		Class.CastTo( m_StatePartyCheckbox, layoutRoot.FindAnyWidget( "marker_state_party_checkbox" ) );
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerPlayerArrow::OnEditInit - End");
		#endif
	}*/
	
	// ------------------------------------------------------------
	// ExpansionMapMarkerPlayerArrow Update
	// ------------------------------------------------------------
	override void Update( float pDt )
	{
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMarkerPlayerArrow::Update - Start");
		#endif
		//! Steve: Dont call super here as we dont need the stuff from the parent classes?!
		
		SetPosition( GetGame().GetCurrentCameraPosition() );

		GetIconWidget().LoadImageFile( 0, EXPANSION_NOTIFICATION_ICON_POSITION );
		GetDragWidget().SetRotation( 0, 0, GetMapDirection(), true );
		
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMarkerPlayerArrow::Update - End");
		#endif
	}

	// ------------------------------------------------------------
	// ExpansionMapMarkerPlayerArrow GetMapDirection
	// ------------------------------------------------------------	
	private int GetMapDirection()
	{
		return Math.Round( Math.NormalizeAngle( GetGame().GetCurrentCameraDirection().VectorToAngles()[0] ) );
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker IsEditButton
	// ------------------------------------------------------------	
	override bool IsEditButton( Widget w )
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker IsDragWidget
	// ------------------------------------------------------------	
	override bool IsDragWidget( Widget w )
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker CanEditName
	// ------------------------------------------------------------	
	override bool CanEditName()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker CanEdit
	// ------------------------------------------------------------	
	override bool CanEdit()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarkerPlayerArrow ShowRoot
	// ------------------------------------------------------------	
	void ShowRoot(bool show)
	{
		GetLayoutRoot().Show(show);
	}
}