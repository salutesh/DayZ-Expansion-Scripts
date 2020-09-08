/**
 * ExpansionGameMissionSelecterModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

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
		return "[EX] Offline Mission";
	}
	
	override string GetIconName()
	{
		return "M";
	}

	override bool ImageIsIcon()
	{
		return false;
	}
}