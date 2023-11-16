/**
 * ExpansionRadiationIndicator.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionRadiationIndicator: ExpansionScriptView
{
	protected int m_CurrentRadiation;
	protected ImageWidget RadiationWidget;
	
	void ExpansionRadiationIndicator(IngameHud hud)
	{
		hud.GetHudPanelWidget().FindAnyWidget("BadgesPanel").AddChild(GetLayoutRoot());
	}
	
	override typename GetControllerType()
	{
		return ExpansionRadiationIndicatorController;
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/NamalskAdventure/GUI/layouts/radiation_indicator.layout";
	}

	override float GetUpdateTickRate()
	{
		return 3.0;
	}

	protected void UpdateRadiationIndicator(PlayerBase player)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

		if (player && m_CurrentRadiation != player.GetClientRadiation())
		{
			m_CurrentRadiation = player.GetClientRadiation();
			Print(ToString() + "::UpdateRadiationIndicator - Radiation: " + m_CurrentRadiation);
			if (m_CurrentRadiation >= 1)
			{
				RadiationWidget.SetColor(ARGB(255, 220, 220, 220));
				if (m_CurrentRadiation >= 90)
				{
					RadiationWidget.SetColor(ARGB(255, 220, 0, 0));
				}
				else if (m_CurrentRadiation >= 60)
				{
					RadiationWidget.SetColor(ARGB(255, 220, 220, 0));
				}
				
				GetLayoutRoot().Show(true);
			}
			else
			{
				if (m_CurrentRadiation == 0)
					GetLayoutRoot().Show(false);
			}
		}
	}

	override void Expansion_Update()
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player)
		{
			UpdateRadiationIndicator(player);
		}
	}
};

class ExpansionRadiationIndicatorController: ExpansionViewController {};