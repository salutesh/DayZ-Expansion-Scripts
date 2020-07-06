/**
 * ExpansionHelicopterWreck.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionHelicopterWreck: ExpansionWreck
{
	Particle m_ParticleEfx;

	void ExpansionHelicopterWreck()
	{
		if ( GetGame().IsServer() )
		{
			ExpansionCreateExplosion( this, "", 5, 100 );
		}

		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			m_ParticleEfx = Particle.PlayOnObject( ParticleList.EXPANSION_FIRE_HELICOPTER, this, GetMemoryPointPos( "fire_pos" ), "0 0 0", true );
		}
	}
	
	override void EEDelete(EntityAI parent)
	{
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			if ( m_ParticleEfx )
				m_ParticleEfx.Stop();
		}
	}
}