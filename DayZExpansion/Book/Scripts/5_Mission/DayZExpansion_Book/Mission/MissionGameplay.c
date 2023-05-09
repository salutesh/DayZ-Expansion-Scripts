/**
 * MissionGameplay.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		MissionGameplay
 * @brief		
 **/
modded class MissionGameplay
{
	//! Book menu check
	protected bool m_Expansion_BookMenuTogglePressed;
	
	override void Expansion_OnUpdate(float timeslice, PlayerBase player, bool isAliveConscious, Input input, bool inputIsFocused, UIScriptedMenu menu, ExpansionScriptViewMenuBase viewMenu)
	{
		super.Expansion_OnUpdate(timeslice, player, isAliveConscious, input, inputIsFocused, menu, viewMenu);
		
		if (isAliveConscious && !inputIsFocused)
		{
			ExpansionBookMenu bookMenu = ExpansionBookMenu.Cast(viewMenu);
	
			if (bookMenu || !menu)
			{
				//! Toggle Book menu
				if (input.LocalPress("UAExpansionBookToggle", false) && !m_Expansion_BookMenuTogglePressed)
				{
					m_Expansion_BookMenuTogglePressed = true;
					OnBookTogglePressed();
				}
				else if (input.LocalRelease("UAExpansionBookToggle", false) || input.LocalValue("UAExpansionBookToggle", false) == 0)
				{
					m_Expansion_BookMenuTogglePressed = false;
				}
			}
		}
    }
	
	void OnBookTogglePressed()
	{
		if (!GetExpansionSettings().GetBook(false).IsLoaded() || !GetExpansionSettings().GetBook().EnableBookMenu)
			return;

		ExpansionUIManager uiManager = GetDayZGame().GetExpansionGame().GetExpansionUIManager();	//! Reference to expansion ui manager
		ScriptView menu = uiManager.GetMenu();	//! Reference to current opened script view menu
		
		ExpansionBookMenu bookMenu = ExpansionBookMenu.Cast(menu);
		if (!bookMenu)
		{
			uiManager.CreateSVMenu("ExpansionBookMenu");
		}
		else if (bookMenu && bookMenu.IsVisible())
		{
			uiManager.CloseMenu();
		}
	}
}