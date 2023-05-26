/**
 * Expansion_Anomaly_Teleport.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAnomalyTeleporterLightBase extends ExpansionAnomalyLightBase
{
	void ExpansionAnomalyTeleporterLightBase()
	{
		SetDiffuseColor(0.751, 0.196, 0.890);
		SetAmbientColor(0.751, 0.196, 0.890);
	}
};

class Expansion_Anomaly_Teleport: Expansion_Anomaly_Base
{
	//! Particles
	protected const int PARTICLE_TELEPORT_IDLE = ParticleList.EXPANSION_PARTICLE_TELEPORT;
	protected const int PARTICLE_WARPER_ACTIVATED = ParticleList.EXPANSION_PARTICLE_WARPER_ACTIVATE;

	override int GetAnomalyIdleParticle()
	{
		return PARTICLE_TELEPORT_IDLE;
	}

	override int GetAnomalyActivatedParticle()
	{
		return PARTICLE_WARPER_ACTIVATED;
	}

	override typename GetAnomalyLight()
	{
		return ExpansionAnomalyTeleporterLightBase;
	}

	override string GetAnomalyTriggerName()
	{
		return "ExpansionAnomalyTeleportTrigger";
	}

	override string GetAmomalyCoreName()
	{
		return "Expansion_AnomalyCore_Warper";
	}
};