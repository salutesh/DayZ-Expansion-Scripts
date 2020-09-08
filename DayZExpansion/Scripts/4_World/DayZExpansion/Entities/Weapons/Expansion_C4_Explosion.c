/**
 * Expansion_C4_Explosion.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_C4_Explosion: BuildingBase
{
	protected ref Timer m_Delay;
	protected Particle m_ParticleExplosion;

	void Expansion_C4_Explosion()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_C4_Explosion::Expansion_C4_Explosion Start");
		#endif
		
		m_Delay = new Timer;
		m_Delay.Run(0.1, this, "ExplodeNow", null, false); //just simply running ExplodeNow() here doesnt work for some reason? copying explosiontest for now
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_C4_Explosion::Expansion_C4_Explosion End");
		#endif
	}
	
	void ExplodeNow()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_C4_Explosion::ExplodeNow Start");
		#endif
		
		Explode(DT_EXPLOSION, "ExpansionC4_Ammo");
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
			m_ParticleExplosion = Particle.PlayInWorld(ParticleList.EXPANSION_EXPLOSION_ROCKET, this.GetPosition());
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_C4_Explosion::ExplodeNow End");
		#endif
	}
}