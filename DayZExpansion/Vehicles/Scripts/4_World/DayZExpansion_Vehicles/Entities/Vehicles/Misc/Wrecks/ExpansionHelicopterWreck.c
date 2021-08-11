/**
 * ExpansionHelicopterWreck.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionHelicopterWreck: ExpansionWreck
{
	protected Particle m_ParticleEfx;

	protected FireplaceLight m_Light;
	protected float m_LightDistance = 2000;

	void ExpansionHelicopterWreck()
	{
		if ( GetGame().IsServer() )
		{
			ExpansionCreateExplosion( this, "", 5, 100 );
		}

		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			m_ParticleEfx = Particle.PlayOnObject( ParticleList.EXPANSION_FIRE_HELICOPTER, this, GetMemoryPointPos( "fire_pos" ), "0 0 0", true );
			
			//turn light on
			if ( !GetLightEntity()  &&  ( GetGame().IsClient() || !GetGame().IsMultiplayer() ) )
			{
				SetLightEntity( FireplaceLight.Cast( ScriptedLightBase.CreateLight(FireplaceLight, GetPosition(), 20) ) );
				GetLightEntity().AttachOnMemoryPoint(this, "light");

				if ( GetLightEntity() )
				{
					Object player = GetGame().GetPlayer();
					if ( player )
					{
						if ( vector.Distance( player.GetPosition(), this.GetPosition() ) > m_LightDistance )
						{
							GetLightEntity().FadeBrightnessTo( 0, 5 );
						}
						else
						{
							GetLightEntity().FadeBrightnessTo( FireplaceLight.m_FireplaceBrightness, 5 );
						}
					}					
					GetLightEntity().SetExteriorMode();
				}
			}
		}
	}
	
	override void EEDelete(EntityAI parent)
	{
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			//turn light off
			if ( GetLightEntity() )
				GetLightEntity().FadeOut();

			//turn fire off
			if ( m_ParticleEfx )
				m_ParticleEfx.Stop();
		}
	}	
	
	FireplaceLight GetLightEntity()
	{
		return m_Light;
	}
	
	void SetLightEntity( FireplaceLight light )
	{
		m_Light = light;
	}
}