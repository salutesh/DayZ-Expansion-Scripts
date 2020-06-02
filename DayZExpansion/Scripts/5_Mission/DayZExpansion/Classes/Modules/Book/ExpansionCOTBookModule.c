class ExpansionCOTBookModule: JMRenderableModuleBase
{
	void ExpansionCOTBookModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.Book.View" );
		GetPermissionsManager().RegisterPermission( "Expansion.Book.Set" );
	}

	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.Book.View" );
	}
	
	override void OnSettingsUpdated()
	{
		super.OnSettingsUpdated();
	}

	override void OnMissionLoaded()
	{
		super.OnMissionLoaded();
	}

	override void OnMissionFinish()
	{
		super.OnMissionFinish();
	}

	override string GetLayoutRoot()
	{
		return "DayZExpansion/GUI/layouts/COT/Book_Menu.layout";
	}

	override string GetTitle()
	{
		return "Book";
	}
	
	override string GetIconName()
	{
		return "EX";
	}

	override bool ImageIsIcon()
	{
		return false;
	}
}