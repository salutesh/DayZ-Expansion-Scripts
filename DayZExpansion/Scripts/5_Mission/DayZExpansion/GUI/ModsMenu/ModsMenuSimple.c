/**
 * ModsMenuSimple.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ModsMenuSimple
{
	ref MainMenu m_MainMenu;
	ref ExpansionNewsfeed m_ExpansionNewsfeed;
	
	override void Select( ModInfo mod )
	{
		super.Select(mod);
		
		m_MainMenu.ShowNewsfeed(false);
	}
	
	void SetMainMenu(MainMenu menu)
	{
		m_MainMenu = menu;
	}
}