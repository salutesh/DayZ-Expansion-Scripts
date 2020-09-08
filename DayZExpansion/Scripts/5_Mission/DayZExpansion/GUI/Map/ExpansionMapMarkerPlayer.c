/**
 * ExpansionMapMarkerPlayer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMapMarkerPlayer : ExpansionMapMarker
{		
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
};