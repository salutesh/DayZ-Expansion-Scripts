class ExpansionGameMissionSelecterModule: JMRenderableModuleBase
{
	void ExpansionGameMissionSelecterModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.GameMission" );
		GetPermissionsManager().RegisterPermission( "Expansion.GameMission.View" );
	}

	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.GameMission.View" );
	}

	override string GetLayoutRoot()
	{
		return "DayZExpansion/GUI/layouts/COT/mission_selector_form.layout";
	}

	override string GetTitle()
	{
		return "Mission Selecter";
	}
	
	override string GetIconName()
	{
		return "MS";
	}

	override bool ImageIsIcon()
	{
		return false;
	}
}