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
	protected ref ExpansionCooldownIndicator m_CooldownIndicator;

	override void Init(Widget hud_panel_widget)
	{
		super.Init(hud_panel_widget);
		
		Widget leftHUDPanel = m_HudPanelWidget.FindAnyWidget("LeftHUDPanel");
		m_CooldownIndicator = new ExpansionCooldownIndicator(leftHUDPanel);
		m_CooldownIndicator.Show(false);
	}
	
	void Expansion_UpdateAggressionCooldown(PlayerBase player, int cooldown)
	{
		if (!m_CooldownIndicator)
			return;
		
		m_CooldownIndicator.UpdateCooldown(player, cooldown);
	}

	void Expansion_ShowAggressionCooldown(bool state)
	{
		if (!m_CooldownIndicator)
			return;

		m_CooldownIndicator.Show(state);
	}
	
	bool Expansion_IsAggressionCooldownVisible()
	{
		if (m_CooldownIndicator)
			return m_CooldownIndicator.IsVisible();

		return false;
	}
};