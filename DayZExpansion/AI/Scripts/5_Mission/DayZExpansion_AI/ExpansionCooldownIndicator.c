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

class ExpansionCooldownIndicator: ExpansionScriptView
{
	protected ref ExpansionCooldownIndicatorController m_CooldownController;
	protected int m_LastCooldown;

	void ExpansionCooldownIndicator()
	{
		Class.CastTo(m_CooldownController, GetController());
	}

	override typename GetControllerType()
	{
		return ExpansionCooldownIndicatorController;
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/AI/GUI/layouts/expansion_cooldown_indicator.layout";
	}

	protected void SetView()
	{	
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player)
			return;
		
		int cooldown = player.eAI_GetLastAggressionCooldown();
		if (cooldown != m_LastCooldown)
		{
			m_LastCooldown = cooldown;
			if (cooldown > 0)
				m_CooldownController.Text = ExpansionStatic.FormatTimestamp(cooldown, false, false);
			else
				m_CooldownController.Text = "";
			m_CooldownController.NotifyPropertyChanged("Text");
		}
	}

	override float GetUpdateTickRate()
	{
		return 0.5;
	}

	override void Update()
	{
		if (!IsVisible())
			return;

		SetView();
	}
};

class ExpansionCooldownIndicatorController: ExpansionViewController
{
	string Text;
};