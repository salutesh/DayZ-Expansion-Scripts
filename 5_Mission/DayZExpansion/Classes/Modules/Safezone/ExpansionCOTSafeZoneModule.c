class ExpansionCOTSafeZoneModule: JMRenderableModuleBase
{
	void ExpansionCOTSafeZoneModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.SafeZone.View" );
		GetPermissionsManager().RegisterPermission( "Expansion.SafeZone.Set" );
	}

	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.SafeZone.View" );
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
		return "DayZExpansion/GUI/layouts/COT/SafeZone_Menu.layout";
	}

	override string GetTitle()
	{
		return "SafeZone";
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