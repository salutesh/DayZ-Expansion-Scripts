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
#ifdef DIAG_DEVELOPER
	static bool s_Expansion_OverrideCanShowHUDElements_Enabled;
	static bool s_Expansion_OverrideCanShowHUDElements_Value;

	static void Expansion_OverrideCanShowHUDElements(bool enable, bool value)
	{
		s_Expansion_OverrideCanShowHUDElements_Enabled = enable;
		s_Expansion_OverrideCanShowHUDElements_Value = value;
	}
#endif

	override bool Expansion_CanShowHUDElements(DayZPlayer player = null)
	{
#ifdef DIAG_DEVELOPER
		if (s_Expansion_OverrideCanShowHUDElements_Enabled)
			return s_Expansion_OverrideCanShowHUDElements_Value;
#endif

		if (!super.Expansion_CanShowHUDElements(player))
			return false;

		if (player && (player.GetPlayerState() != EPlayerStates.ALIVE || player.IsUnconscious()))
			return false;

		if (g_Game.GetUIManager().GetMenu())
			return false;

	#ifdef EXPANSIONUI
		if (GetDayZExpansion().GetExpansionUIManager().GetMenu())
			return false;
	#endif

		if (!m_HudVisibility)
			return false;

		if (m_HudVisibility.IsContextFlagActive(EHudContextFlags.HUD_DISABLE) || m_HudVisibility.IsContextFlagActive(EHudContextFlags.HUD_HIDE))
			return false;

		if (m_HudVisibility.IsContextFlagActive(EHudContextFlags.MENU_OPEN) || m_HudVisibility.IsContextFlagActive(EHudContextFlags.INVENTORY_OPEN))
			return false;
		
		return true;
	}

	Widget Expansion_GetStancePanel()
	{
		if (m_HudVisibility)
			return m_HudVisibility.Expansion_GetElementWidget(EHudElement.LHUD_STANCE);
		return null;
	}

	override bool Expansion_IsVisible()
	{
		return !m_HudVisibility.IsContextFlagActive(IngameHudVisibility.HUD_HIDE_FLAGS);
	}
}