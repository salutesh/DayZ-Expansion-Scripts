/**
 * Expansion_Anomaly_Fire.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAnomalyFireLightBase extends ExpansionAnomalyLightBase
{
	void ExpansionAnomalyFireLightBase()
	{
		SetDiffuseColor(0.751, 0.196, 0.890);
		SetAmbientColor(0.751, 0.196, 0.890);
	}
};

class Expansion_Anomaly_Fire: Expansion_Anomaly_Base
{
	//! Particles
	protected const int PARTICLE_FIRE_IDLE = ParticleList.EXPANSION_PARTICLE_FIRE;
	protected const int PARTICLE_FIRE_ACTIVATED = ParticleList.EXPANSION_PARTICLE_FIRE_ACTIVATE;

	override int GetAnomalyIdleParticle()
	{
		return PARTICLE_FIRE_IDLE;
	}

	override int GetAnomalyActivatedParticle()
	{
		return PARTICLE_FIRE_ACTIVATED;
	}

	override typename GetAnomalyLight()
	{
		return ExpansionAnomalyFireLightBase;
	}

	override string GetAnomalyTriggerName()
	{
		return "ExpansionAnomalyFireTrigger";
	}

	override string GetAmomalyCoreName()
	{
		return "Expansion_AnomalyCore_Fire";
	}
};