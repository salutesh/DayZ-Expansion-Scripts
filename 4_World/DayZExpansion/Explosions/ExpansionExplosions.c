/**
 * ExpansionExplosions.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExplosionPoint
 * @brief		
 **/
class ExplosionPoint: Building
{
	ref Timer m_Delay;

	void ExplosionPoint()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExplosionPoint] Constructor start");
		#endif

		m_Delay = new Timer;
		m_Delay.Run(0.1, this, "ExplodeNow", null, false);

		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExplosionPoint] Constructor end");
		#endif
	}

	void ExplodeNow()
	{

	}
}

/**@class		ExplosionSmall
 * @brief		
 **/
class ExplosionSmall extends ExplosionPoint
{
	// ------------------------------------------------------------
	override void ExplodeNow()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExplosionSmall] ExplodeNow start");
		#endif
		
		if (GetGame().IsClient() || !GetGame().IsMultiplayer()) 
		{
			SEffectManager.PlaySound("Expansion_Explosive_Small_SoundSet", GetPosition());

			vector n = GetPosition().VectorToAngles();
			Particle p2 = Particle.Play(ParticleList.EXPLOSION_LANDMINE, GetPosition());
			p2.SetOrientation(n);

			Particle p3 = Particle.Play(ParticleList.IMPACT_METAL_RICOCHET, GetPosition());
			p3.SetOrientation(n);
		
			Particle p4 = Particle.Play(ParticleList.IMPACT_GRAVEL_RICOCHET, GetPosition());
			p4.SetOrientation(n);
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExplosionSmall] ExplodeNow end");
		#endif
	}
}