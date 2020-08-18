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