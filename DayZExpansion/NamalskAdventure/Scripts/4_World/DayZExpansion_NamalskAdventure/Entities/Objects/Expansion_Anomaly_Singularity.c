/**
 * Expansion_Anomaly_Singularity.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAnomalySingularityLightBase extends ExpansionAnomalyLightBase
{
	void ExpansionAnomalySingularityLightBase()
	{
		SetDiffuseColor(0.105, 0.446, 0.810);
		SetAmbientColor(0.105, 0.446, 0.810);
	}
};

class Expansion_Anomaly_Singularity: Expansion_Anomaly_Base
{
	//! Particles
	protected const int PARTICLE_SINGULARITY_IDLE = ParticleList.EXPANSION_PARTICLE_SINGULARITY;
	protected const int PARTICLE_SINGULARITY_ACTIVATED = ParticleList.EXPANSION_PARTICLE_SINGULARITY_ACTIVATE;

	override int GetAnomalyIdleParticle()
	{
		return PARTICLE_SINGULARITY_IDLE;
	}

	override int GetAnomalyActivatedParticle()
	{
		return PARTICLE_SINGULARITY_ACTIVATED;
	}

	override typename GetAnomalyLight()
	{
		return ExpansionAnomalySingularityLightBase;
	}

	override string GetAnomalyTriggerName()
	{
		return "ExpansionAnomalySingularityTrigger";
	}

	override string GetAmomalyCoreName()
	{
		return "Expansion_AnomalyCore_Ice";
	}

	override void TurnOffCoreEmitor()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.TurnOffCoreEmitor();

		m_ParticleIdle.SetParameter(3, EmitorParam.LIFETIME, 0);
		m_ParticleIdle.SetParameter(3, EmitorParam.REPEAT, 0);
	}
};