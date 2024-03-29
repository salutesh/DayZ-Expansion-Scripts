/**
 * ExpansionExplosions.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

/**
 * @class		ExplosionPoint
 * @brief
 */
class ExplosionPoint : Building
{
	ref Timer m_Delay;

	void ExplosionPoint()
	{
		m_Delay = new Timer;
		m_Delay.Run(0.1, this, "ExplodeNow", null, false);
	}

	void ExplodeNow()
	{
	}
};

/**
 * @class		ExplosionSmall
 * @brief
 */
class ExplosionSmall: ExplosionPoint
{
	// ------------------------------------------------------------
	override void ExplodeNow()
	{
		if (!GetGame().IsDedicatedServer())
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
	}
};
