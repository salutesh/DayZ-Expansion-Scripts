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
	override void Expansion_OnUpdate(float timeslice, PlayerBase player, bool isAliveConscious, Input input, bool inputIsFocused, UIScriptedMenu menu, ExpansionScriptViewMenuBase viewMenu)
	{
		super.Expansion_OnUpdate(timeslice, player, isAliveConscious, input, inputIsFocused, menu, viewMenu);
		
		//! Basebuilding Snapping
		if (isAliveConscious && !inputIsFocused && player.IsPlacingLocal())
		{
			Hologram hologram = player.GetHologramLocal();

			if (hologram)
			{
				if (input.LocalPress("UAExpansionSnappingToggle"))
				{
					hologram.SetUsingSnap(!hologram.IsUsingSnap());
					
					if (hologram.IsUsingSnap())
					{
						ExpansionNotification("STR_EXPANSION_SNAPPING_TITLE", "STR_EXPANSION_SNAPPING_ENABLED", EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 5).Info(player.GetIdentity());
					} else {
						ExpansionNotification("STR_EXPANSION_SNAPPING_TITLE", "STR_EXPANSION_SNAPPING_DISABLED", EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 5).Info(player.GetIdentity());
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
