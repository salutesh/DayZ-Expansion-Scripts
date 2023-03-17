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
	protected ref ExpansionCooldownIndicator m_CooldownIndicator;

	override void Init(Widget hud_panel_widget)
	{
		super.Init(hud_panel_widget);

		if (m_LeftHudPanelWidget)
		{
			m_CooldownIndicator = new ExpansionCooldownIndicator();
			m_LeftHudPanelWidget.AddChild(m_CooldownIndicator.GetLayoutRoot(), true);
		}
	}

	ExpansionCooldownIndicator Expansion_GetCooldownIndicator()
	{
		return m_CooldownIndicator;
	}

	bool Expansion_CooldownVisible()
	{
		return m_CooldownIndicator.IsVisible();
	}
};