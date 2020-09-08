/**
 * ExpansionAITestModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

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