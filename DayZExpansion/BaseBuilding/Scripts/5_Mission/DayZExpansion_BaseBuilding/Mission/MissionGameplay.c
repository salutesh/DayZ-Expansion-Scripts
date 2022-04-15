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
	// ------------------------------------------------------------
	// OnUpdate
	// ------------------------------------------------------------
	override void OnUpdate( float timeslice )
	{
		super.OnUpdate( timeslice );

		if (!m_bLoaded)
		{
			return;
		}

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
		PlayerBase playerPB = PlayerBase.Cast(man);	//! Expansion reference to player
		
		if (playerPB && playerPB.GetHumanInventory()) 
		{
			//! Expansion reference to item in hands
			EntityAI itemInHands = playerPB.GetHumanInventory().GetEntityInHands();

			//! Expansion reference to hologram
			Hologram hologram;	

			if (playerPB.GetPlayerState() == EPlayerStates.ALIVE && !playerPB.IsUnconscious())
			{
				//! Basebuilding Snaping
				if (playerPB && playerPB.IsPlacingLocal() && !inputIsFocused)
				{
					hologram = playerPB.GetHologramLocal();

					if (hologram)
					{
						if (input.LocalPress("UAExpansionSnappingToggle"))
						{
							hologram.SetUsingSnap(!hologram.IsUsingSnap());
							
							if (hologram.IsUsingSnap())
							{
								ExpansionNotification("STR_EXPANSION_SNAPPING_TITLE", "STR_EXPANSION_SNAPPING_ENABLED", EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 5).Info(playerPB.GetIdentity());
							} else {
								ExpansionNotification("STR_EXPANSION_SNAPPING_TITLE", "STR_EXPANSION_SNAPPING_DISABLED", EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 5).Info(playerPB.GetIdentity());
							}
						}

						if (input.LocalValue("UAExpansionSnappingDirectionNext") != 0)
						{
							hologram.NextDirection();
						}

						if (input.LocalValue("UAExpansionSnappingDirectionPrevious") != 0)
						{
							hologram.PreviousDirection();
						}
					}
				}
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion OnKeyPress
	//! Includes key-press commands for some inputs in menus
	// ------------------------------------------------------------
	override void OnKeyPress(int key)
	{
		super.OnKeyPress(key);
		
		ExpansionLockUIBase menu;
		if (Class.CastTo(menu, GetGame().GetUIManager().FindMenu(MENU_EXPANSION_NUMPAD_MENU)))
			menu.OnKeyPress(key);
		
		menu = NULL;
		if (Class.CastTo(menu, GetGame().GetUIManager().FindMenu(MENU_EXPANSION_CODELOCK_MENU)))
			menu.OnKeyPress(key);
	}
}
