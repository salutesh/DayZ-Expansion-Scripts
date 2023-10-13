/**
 * ExpansionAnomalyAreaFire_Local.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAnomalyAreaFire_Local: ExpansionAnomalyAreaBase_Dynamic
{
	const float TICK_RATE 	= 1;
	ref Timer 	m_Timer1 	= new Timer;
	float 		m_Lifetime 	= 360;

	// ----------------------------------------------
	// 				INITIAL SETUP
	// ----------------------------------------------

	override void SetupZoneData(EffectAreaParams params)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		params.m_ParamPartId 		= ParticleList.EXPANSION_PARTICLE_ANOMALY_AREA_FIRE;
		params.m_ParamInnerRings 	= 0;
		params.m_ParamPosHeight 	= 3;
		params.m_ParamNegHeight 	= 5;
		params.m_ParamRadius 		= 10;
		params.m_ParamOuterToggle 	= false;
		params.m_ParamTriggerType 	= "ExpansionAnomalyTriggerFire_Dynamic";

		params.m_ParamAroundPartId 	= 0;
		params.m_ParamTinyPartId 	= 0;

		super.SetupZoneData(params);
	}

	override void EEInit()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			SetupZoneData(new EffectAreaParams);
			m_Timer1.Run(TICK_RATE, this, "Tick", NULL, true);
		}
	}

	override void SpawnItems()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		//! Override base funcionality as we don't want any items spawned here
	}

	override float GetStartDecayLifetime()
	{
		return 20;
	}

	override float GetFinishDecayLifetime()
	{
		return 10;
	}

	override float GetRemainingTime()
	{
		return m_Lifetime;
	}

	override void Tick()
	{
		super.Tick();
		m_Lifetime -= TICK_RATE;
		if (m_Lifetime <= 0)
		{
			Delete();
		}
	}
};