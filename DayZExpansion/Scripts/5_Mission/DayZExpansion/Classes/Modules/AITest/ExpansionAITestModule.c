class ExpansionAITestModule: JMRenderableModuleBase
{
	void ExpansionAITestModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.AITest" );

		GetPermissionsManager().RegisterPermission( "Expansion.AITest.View" );
	}

	override void OnMissionLoaded()
	{
		super.OnMissionLoaded();
	}

	override bool HasAccess()
	{
		return GetPermissionsManager().HasPermission( "Expansion.AITest.View" );
	}

	override string GetLayoutRoot()
	{
		return "DayZExpansion/GUI/layouts/COT/ai_test_form.layout";
	}

	override string GetTitle()
	{
		return "AI Test";
	}
	
	override string GetIconName()
	{
		return "AI";
	}

	override bool ImageIsIcon()
	{
		return false;
	}
}