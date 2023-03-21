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
	
    override void OnUpdate(float timeslice)   
	{
		super.OnUpdate(timeslice);
		
		if (GetExpansionSettings().GetBook(false).IsLoaded() && GetExpansionSettings().GetBook().EnableBookMenu)
		{
			//! Checking for keyboard focus
			bool inputIsFocused = false;
			//! Reference to focused windget
			Widget focusedWidget = GetFocus();
	
			if (focusedWidget)
			{
				if (focusedWidget.ClassName().Contains("EditBoxWidget"))
				{
					inputIsFocused = true;
				}
				else if (focusedWidget.ClassName().Contains("MultilineEditBoxWidget"))
				{
					inputIsFocused = true;
				}
			}
	
			Man man = GetGame().GetPlayer(); 	//! Refernce to man
			Input input = GetGame().GetInput(); 	//! Reference to input
			UIScriptedMenu topMenu = m_UIManager.GetMenu(); 	//! Expansion reference to menu
			PlayerBase playerPB = PlayerBase.Cast(man);	//! Expansion reference to player
			ExpansionScriptViewMenuBase viewMenu = GetDayZExpansion().GetExpansionUIManager().GetMenu();
			ExpansionBookMenu bookMenu = ExpansionBookMenu.Cast(viewMenu);
			
			//TODO: Make ExpansionInputs class and handle stuff there to keep this clean
			if (playerPB && playerPB.GetHumanInventory()) 
			{
				if (playerPB.GetPlayerState() == EPlayerStates.ALIVE && !playerPB.IsUnconscious())
				{
					if ((bookMenu || !topMenu) && !inputIsFocused)
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
		}
    }
	
	void OnBookTogglePressed()
	{
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