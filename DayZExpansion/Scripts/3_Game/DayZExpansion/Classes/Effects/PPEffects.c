/**
 * PPEffects.c
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
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint( "PPEffects::UpdateSaturation - Start" );
		#endif
		
		if ( GetExpansionClientSettings().ColorGrading )
		{
			m_MatColors.SetParam("Saturation", m_BloodSaturation + 0.25);
		}
		else
		{
			m_MatColors.SetParam("Saturation", m_BloodSaturation);
		}
		
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint( "PPEffects::UpdateSaturation - End" );
		#endif
	}

	override static void UpdateVignette()
	{
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint( "PPEffects::UpdateVignette - Start" );
		#endif
		
		float color[4];
		float intesity;
		
		#ifdef DAYZ_1_10

		float intensity_value_total = 0; //use just the highest?
		if( m_VignetteEffects )
		{
			for ( int i = 0; i < m_VignetteEffects.Count(); ++i )
			{
				if (m_VignetteValues.Get(i))
				{
					color[0] = m_VignetteValues.Get(i).Get(1); //red
					color[1] = m_VignetteValues.Get(i).Get(2); //green
					color[2] = m_VignetteValues.Get(i).Get(3); //blue
					color[3] = m_VignetteValues.Get(i).Get(4); //alpha
					intesity = m_VignetteValues.Get(i).Get(0);
					intensity_value_total += intesity;
				}
			}
		}

		#else
		color[0] = m_UnconsciousVignetteColor[0];
		color[1] = m_UnconsciousVignetteColor[1];
		color[2] = m_UnconsciousVignetteColor[2];
		#endif

		#ifdef DAYZ_1_10

		if ( intensity_value_total <= 0 && GetExpansionClientSettings().ColorVignette )
		{
			intensity_value_total = 0.5;
		}

		#else

		if ( m_UnconsciousVignetteIntesity > 0 )
		{
			intesity = m_UnconsciousVignetteIntesity;
		}
		else if ( GetExpansionClientSettings().ColorVignette )
		{
			intesity = 0.5;
		}

		#endif
		
		#ifdef DAYZ_1_10
		SetVignette( intensity_value_total, color[0], color[1], color[2], color[3]  );
		#else
		SetVignette( intesity, color[0], color[1], color[2] );
		#endif
		
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXLogPrint( "PPEffects::UpdateVignette - End" );
		#endif
	}
};