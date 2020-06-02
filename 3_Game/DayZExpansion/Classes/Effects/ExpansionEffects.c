/**
 * ExpansionEffects.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class PPEffects
{
	override static void UpdateSaturation()
	{
		if ( GetExpansionClientSettings().ColorGrading )
		{
			m_MatColors.SetParam("Saturation", m_BloodSaturation + 0.25);
		}
		else
		{
			m_MatColors.SetParam("Saturation", m_BloodSaturation);
		}
	}

	override static void UpdateVignette()
	{
		float color[4];
		float intesity;
		
		color[0] = m_UnconsciousVignetteColor[0];
		color[1] = m_UnconsciousVignetteColor[1];
		color[2] = m_UnconsciousVignetteColor[2];

		if ( m_UnconsciousVignetteIntesity > 0 )
		{
			intesity = m_UnconsciousVignetteIntesity;
		}
		else if ( GetExpansionClientSettings().ColorVignette )
		{
			intesity = 0.5;
		}
		
		SetVignette( intesity, color[0], color[1], color[2] );
	}
}
