/**
 * Expansion_RPG_Explosion.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_RPG_Explosion: BuildingBase
{
	ref Timer m_Delay;

	protected Particle m_ParticleExplosion;

	void Expansion_RPG_Explosion()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_RPG_Explosion::Expansion_RPG_Explosion Start");
		#endif
		
		m_Delay = new Timer;
		m_Delay.Run(0.1, this, "ExplodeNow", null, false); 
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_RPG_Explosion::Expansion_RPG_Explosion End");
		#endif
	}
	
	void ExplodeNow()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_RPG_Explosion::ExplodeNow Start");
		#endif
		
		Explode(DT_EXPLOSION, "ExpansionRocket_Ammo");
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
			m_ParticleExplosion = Particle.PlayInWorld(ParticleList.EXPANSION_EXPLOSION_ROCKET, this.GetPosition());
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_RPG_Explosion::ExplodeNow End");
		#endif
	}
}