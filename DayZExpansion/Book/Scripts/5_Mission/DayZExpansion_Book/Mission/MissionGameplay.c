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
    override void OnUpdate(float timeslice)   
	{
		super.OnUpdate(timeslice);
		
		if (GetExpansionSettings().GetBook().EnableBookMenu)
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
			ExpansionBookMenu bookMenu = ExpansionBookMenu.Cast(GetDayZExpansion().GetExpansionUIManager().GetMenu());
			
			//TODO: Make ExpansionInputs class and handle stuff there to keep this clean
			if (playerPB && playerPB.GetHumanInventory()) 
			{
				if (playerPB.GetPlayerState() == EPlayerStates.ALIVE && !playerPB.IsUnconscious())
				{
					if ((bookMenu || !topMenu) && !inputIsFocused)
					{
						if (input.LocalPress("UAExpansionBookToggle", false))
						{
							OnBookTogglePressed();
						}
					}
				}
			}
		}
    }
	
	void OnBookTogglePressed()
	{
		ExpansionUIManager uiManager = GetDayZGame().GetExpansionGame().GetExpansionUIManager();	//! Reference to expansion ui manager
		ScriptView menu	= uiManager.GetMenu();														//! Reference to current opened script view menu
		
		ExpansionBookMenu bookMenu = ExpansionBookMenu.Cast(menu);
		if (!bookMenu)
		{
			uiManager.CreateSVMenu("ExpansionBookMenu");
		} 
		else if (bookMenu && bookMenu.IsVisible())
		{
			bookMenu.OnBackButtonClick();
		}
	}
}