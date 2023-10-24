/**
 * bldr_Candle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODMAPASSETS
modded class bldr_Candle
{
	Particle m_Particle;
	
    override void DeferredInit()
	{
		super.DeferredInit();

	#ifndef SERVER
		CreateParticle();
	#endif
	}
	
	protected void CreateParticle()
	{
		if (!m_Particle)
			m_Particle = Particle.PlayOnObject(ParticleList.EXPANSION_PARTICLE_CANDLE_FIRE, this, "0 -1.085 0");
	}
}
#endif