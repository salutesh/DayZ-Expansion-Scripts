/**
 * ModsMenuDetailed.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ModsMenuDetailed
{
	ref MainMenu m_MainMenu;
	ref ExpansionNewsfeed m_ExpansionNewsfeed;
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		super.OnMouseButtonUp(w,x,y,button);
		
		if( w == m_CloseButton )
		{
			m_MainMenu.ShowNewsfeed(true);
			return true;
		}
		return false;
	}
	
	void SetMainMenu(MainMenu menu)
	{
		m_MainMenu = menu;
	}
}