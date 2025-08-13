/**
 * IngameHud.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class IngameHud
{
	override bool Expansion_CanShowHUDElements(DayZPlayer player = null)
	{
		if (!super.Expansion_CanShowHUDElements(player))
			return false;

		if (player && (player.GetPlayerState() != EPlayerStates.ALIVE || player.IsUnconscious()))
			return false;

		if (GetGame().GetUIManager().GetMenu())
			return false;

	#ifdef EXPANSIONUI
		if (GetDayZExpansion().GetExpansionUIManager().GetMenu())
			return false;
	#endif

	#ifndef DAYZ_1_26
		//! 1.27+
		if (!m_HudVisibility)
			return false;

		if (m_HudVisibility.IsContextFlagActive(EHudContextFlags.HUD_DISABLE) || m_HudVisibility.IsContextFlagActive(EHudContextFlags.HUD_HIDE))
			return false;

		if (m_HudVisibility.IsContextFlagActive(EHudContextFlags.MENU_OPEN) || m_HudVisibility.IsContextFlagActive(EHudContextFlags.INVENTORY_OPEN))
			return false;
		
		return true;
	#else
		return GetHudState();
	#endif
	}

	Widget Expansion_GetStancePanel()
	{
	#ifndef DAYZ_1_26
		//! 1.27+
		if (m_HudVisibility)
			return m_HudVisibility.Expansion_GetElementWidget(EHudElement.LHUD_STANCE);
		return null;
	#else
		return m_StancePanel;
	#endif
	}
}