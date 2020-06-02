class ExpansionAnimalAITestModule: JMRenderableModuleBase
{
	void ExpansionAnimalAITestModule()
	{
		GetPermissionsManager().RegisterPermission( "Expansion.AnimalAITest" );

		GetPermissionsManager().RegisterPermission( "Expansion.AnimalAITest.View" );
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
		return "DayZExpansion/GUI/layouts/COT/animal_ai_test_form.layout";
	}

	override string GetTitle()
	{
		return "Animal AI Test";
	}
	
	override string GetIconName()
	{
		return "AAI";
	}

	override bool ImageIsIcon()
	{
		return false;
	}
}