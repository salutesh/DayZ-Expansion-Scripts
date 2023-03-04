/**
 * ExpansionAirdropParticle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAirdropParticle: House
{
	Particle m_ParticleEfx;
	
	void ExpansionAirdropParticle()
	{
		if (!GetGame().IsDedicatedServer())
			m_ParticleEfx = Particle.PlayOnObject(ParticleList.EXPANSION_AIRDROP_SMOKE, this);
	}
	
	void ~ExpansionAirdropParticle()
	{
		if (m_ParticleEfx)
			m_ParticleEfx.Stop();
	}
};
