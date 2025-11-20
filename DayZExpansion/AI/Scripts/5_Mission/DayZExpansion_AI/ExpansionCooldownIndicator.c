/**
 * ExpansionCooldownIndicator.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCooldownIndicator: ScriptedWidgetEventHandler
{
	protected Widget m_LayoutRoot;
	protected TextWidget m_CooldownText;
	
	protected int m_LastCooldown;
	protected bool m_IsVisible;

	void ExpansionCooldownIndicator(Widget parent)
	{
		m_LayoutRoot = g_Game.GetWorkspace().CreateWidgets("DayZExpansion/AI/GUI/layouts/expansion_cooldown_indicator.layout", parent);
		m_CooldownText = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("CooldownText"));
	}
	
	void Show(bool state)
	{
		m_IsVisible = state;
		m_LayoutRoot.Show(state);
	}
	
	bool IsVisible()
	{
		return m_IsVisible;
	}
	
	void UpdateCooldown(PlayerBase player, int cooldown)
	{
		if (cooldown != m_LastCooldown)
		{
			m_LastCooldown = cooldown;

			string text;
			if (cooldown > 0)
			{
				text = ExpansionStatic.FormatTime(cooldown, false, false);
			}
			
			m_CooldownText.SetText(text);
		}
	}
}