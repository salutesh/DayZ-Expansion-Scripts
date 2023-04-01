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

class Expansion_Anomaly_Singularity: Expansion_Anomaly_Base
{
	protected ref UniversalTemperatureSource m_UTSource;
	protected ref UniversalTemperatureSourceSettings m_UTSSettings;
	protected ref UniversalTemperatureSourceLambdaConstant m_UTSLConst;

	//! Particles
	protected const int PARTICLE_SINGULARITY_IDLE = ParticleList.EXPANSION_PARTICLE_SINGULARITY;
	protected const int PARTICLE_SINGULARITY_ACTIVATED = ParticleList.EXPANSION_PARTICLE_SINGULARITY_ACTIVATE;

	void Expansion_Anomaly_Singularity()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	}
	
	override void EEInit()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		//! Does this even work?!
		//! Create tempreature source lambada
		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
 			m_UTSSettings = new UniversalTemperatureSourceSettings();
			m_UTSSettings.m_ManualUpdate	= true;
			m_UTSSettings.m_TemperatureMin	= 0;
			m_UTSSettings.m_TemperatureMax	= 0;
			m_UTSSettings.m_RangeFull		= 3;
			m_UTSSettings.m_RangeMax		= 5;
			m_UTSSettings.m_TemperatureCap	= 1;

			m_UTSLConst	= new UniversalTemperatureSourceLambdaConstant();
			m_UTSource = new UniversalTemperatureSource(this, m_UTSSettings, m_UTSLConst);
			m_UTSource.Update(m_UTSSettings, m_UTSLConst);
		}

		super.EEInit();
	}

	override protected void InitAnomalyServer()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		//! Spawn items on ground in anomaly radius
		//SpawnItems();

		//! Spawn the core item into the anomaly.
		SpawnCoreItem("Expansion_AnomalyCore_Ice");

		//! Spawn anomaly trigger on anomaly position.
		CreateAnomalyTrigger("ExpansionAnomalySingularityTrigger");
		
		super.InitAnomalyServer();
	}
	
	override int GetAnomalyIdleParticle()
	{
		return PARTICLE_SINGULARITY_IDLE;
	}

	override int GetAnomalyActivatedParticle()
	{
		return PARTICLE_SINGULARITY_ACTIVATED;
	}
};