/**
 * MissionBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class MissionBase
{
	// ------------------------------------------------------------
	// MissionBase
	// ------------------------------------------------------------
	void MissionBase()
	{
		if ( !GetGame().IsMultiplayer() )
		{
			m_WorldLighting	= new WorldLighting;
		}
	}

	// ------------------------------------------------------------
	// Override UIScriptedMenu
	// Menu handling
	// ------------------------------------------------------------
	override UIScriptedMenu CreateScriptedMenu(int id)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionBase::CreateScriptedMenu - Start");
		#endif
		
		UIScriptedMenu menu = NULL; 
		
		switch ( id )
		{
		case MENU_MAP:
			if (GetExpansionSettings().GetMap().UseMapOnMapItem)
				menu = new ExpansionMapMenu;
			else
				menu = new MapMenu;
			break;
		case MENU_EXPANSION_MAP:
			menu = new ExpansionMapMenu;
			break;
		case MENU_CHAT_INPUT:
			menu = new ChatInputMenu;
			break;
		case MENU_EXPANSION_CODELOCK_MENU:
			menu = new ExpansionCodeLockUI;
			break;
		case MENU_EXPANSION_SPAWN_SELECTION_MENU:
			menu = new ExpansionSpawnSelectionMenu;
			break;
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

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionBase::CreateScriptedMenu - End");
		#endif

		return menu;
	}
}
