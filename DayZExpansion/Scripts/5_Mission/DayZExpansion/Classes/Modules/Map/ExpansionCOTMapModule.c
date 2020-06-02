class ExpansionCOTMapModule: JMRenderableModuleBase
{
	void ExpansionCOTMapModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.Map.View" );
		GetPermissionsManager().RegisterPermission( "Expansion.Map.Set" );
	}

	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.Map.View" );
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
		return "DayZExpansion/GUI/layouts/COT/Map_Menu.layout";
	}

	override string GetTitle()
	{
		return "Map";
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