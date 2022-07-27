/**
 * InGameMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class InGameMenu
{
	override protected void SetGameVersion()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.UI, this, "SetGameVersion");
#endif

		TextWidget version_widget = TextWidget.Cast( layoutRoot.FindAnyWidget("version") );
		string version;
		GetGame().GetVersion( version );
		string expansion_version;	
		
		if ( GetDayZGame() )
			expansion_version = GetDayZGame().GetExpansionClientVersion();

		if (expansion_version)
			version_widget.SetText( "DayZ SA #main_menu_version" + " " + version + "   DayZ Expansion #main_menu_version" + " " + expansion_version );
		else
			version_widget.SetText( "DayZ SA #main_menu_version" + " " + version );
	}
}
