/**
 * bldr_expansion_Sign_RoadBarrier_LightOn.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		bldr_expansion_Sign_RoadBarrier_LightOn
 * @brief		
 **/
class bldr_expansion_Sign_RoadBarrier_LightOn  extends House
{	
	protected ScriptedLightBase m_Light;

	// ------------------------------------------------------------
	// bldr_expansion_Sign_RoadBarrier_LightOn Constructor
	// ------------------------------------------------------------
	void bldr_expansion_Sign_RoadBarrier_LightOn()
	{		
		if ( GetGame().IsClient() || !GetGame().IsMultiplayer() )
		{
			if ( !m_Light )
			{
				m_Light = ScriptedLightBase.CreateLight( ExpansionPointLight, "0 0 0" );

				m_Light.FadeIn(0.3);			
				m_Light.SetCastShadow(true);				
				m_Light.SetFlareVisible(false);

				vector colorRGB = Vector( 1.0, 0.5, 0 );
				m_Light.SetDiffuseColor( colorRGB[0], colorRGB[1], colorRGB[2] );
				m_Light.SetAmbientColor( colorRGB[0], colorRGB[1], colorRGB[2] );
				m_Light.SetRadiusTo( 2.5 );
				m_Light.SetBrightnessTo( 1 );

				m_Light.AttachOnMemoryPoint( this, "zluty pozicni blik" );

				m_Light.SetEnabled( true );
			}
		}
	}
} 