/**
 * IngameHud.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class IngameHud
{
	//! @note DayZ Experimental 1.19 update from 2022-10-24 made a not-modding-friendly change
	//! that introduced the bug of always showing vehicle HUD (even vanilla) in inventory menu.
	//! In the *hopefully* (AHEM) unlekliy event that this bug makes it to DayZ stable (AHEM),
	//! just uncomment the below commented out code and REMOVE the super.
	//! 2022-10-26: Bug made it to stable :/
	override void SetLeftStatsVisibility(bool visible)
	{
		//super.SetLeftStatsVisibility(visible);  //! REMOVE this super call if uncommenting below code

		m_LeftHudPanelWidget.Show(visible);
	}
}
