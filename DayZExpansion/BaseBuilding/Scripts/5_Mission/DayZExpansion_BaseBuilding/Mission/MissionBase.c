/**
 * MissionBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class MissionBase
{
	// ------------------------------------------------------------
	// Override UIScriptedMenu
	// Menu handling
	// ------------------------------------------------------------
	override UIScriptedMenu CreateScriptedMenu(int id)
	{
		UIScriptedMenu menu = NULL; 
		
		switch ( id )
		{
		case MENU_EXPANSION_CODELOCK_MENU:
			menu = new ExpansionCodeLockUI;
			break;
		/*case MENU_EXPANSION_SPAWN_SELECTION_MENU:
			menu = new ExpansionSpawnSelectionMenu;
			break;*/
		case MENU_EXPANSION_NUMPAD_MENU:
			menu = new ExpansionNumpadUI;
			break;
		case MENU_EXPANSION_FLAG_MENU:
			menu = new ExpansionFlagMenu;
			break;
		}	

		if ( menu )
		{
			menu.SetID( id );
		} else
		{
			menu = super.CreateScriptedMenu( id );
		}

		return menu;
	}
}
