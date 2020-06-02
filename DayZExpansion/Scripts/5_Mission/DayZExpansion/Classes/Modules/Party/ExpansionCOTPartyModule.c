class ExpansionCOTPartyModule: JMRenderableModuleBase
{
	void ExpansionCOTPartyModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.Party.View" );
		GetPermissionsManager().RegisterPermission( "Expansion.Party.Set" );
	}

	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.Party.View" );
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
		return "DayZExpansion/GUI/layouts/COT/Party_Menu.layout";
	}

	override string GetTitle()
	{
		return "Party";
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