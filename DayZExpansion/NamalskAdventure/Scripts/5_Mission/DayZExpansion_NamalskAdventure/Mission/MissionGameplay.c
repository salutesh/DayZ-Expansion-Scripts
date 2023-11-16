/**
 * MissionGameplay.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class MissionGameplay
{
	protected ImageWidget m_Radiation;
	protected float m_CurrentRadiation;
	protected ref ExpansionRadiationIndicator m_ExpansionRadiationIndicator;

	override void OnInit()
	{
		super.OnInit();

		InitExpansionRadiationIndicator();
	}

	void InitExpansionRadiationIndicator()
	{
		if (!m_ExpansionRadiationIndicator)
		{
			m_ExpansionRadiationIndicator = new ExpansionRadiationIndicator(m_Hud);
		}
	}
};