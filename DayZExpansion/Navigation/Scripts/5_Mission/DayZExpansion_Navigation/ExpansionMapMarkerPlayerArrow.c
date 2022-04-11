/**
 * ExpansionMapMarkerPlayerArrow.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMapMarkerPlayerArrow : ExpansionMapWidgetBase
{	
	void ExpansionMapMarkerPlayerArrow(Widget parent, MapWidget mapWidget, bool autoInit = true)
	{
		SetIgnorePointer(true);
		SetIcon(EXPANSION_ICON_POSITION);
		//! Do NOT show by default, otherwise it'll pop up on the load screen in the top left corner at the end of the loading progress
		Hide();
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarkerPlayerArrow Update
	// ------------------------------------------------------------
	override void Update(float pDt)
	{
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMarkerPlayerArrow::Update - Start");
		#endif
		
		//! Steve: Dont call super here as we dont need the stuff from the parent classes?!
		SetPosition(GetGame().GetCurrentCameraPosition());
		GetDragWidget().SetRotation(0, 0, GetMapDirection(), true);
		
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMarkerPlayerArrow::Update - End");
		#endif
	}

	// ------------------------------------------------------------
	// ExpansionMapMarkerPlayerArrow GetMapDirection
	// ------------------------------------------------------------	
	private int GetMapDirection()
	{
		return Math.Round(Math.NormalizeAngle(GetGame().GetCurrentCameraDirection().VectorToAngles()[0]));
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
	// ExpansionMapMarkerPlayerArrow ShowRoot
	// ------------------------------------------------------------	
	void ShowRoot(bool show)
	{
		GetLayoutRoot().Show(show);
	}
}