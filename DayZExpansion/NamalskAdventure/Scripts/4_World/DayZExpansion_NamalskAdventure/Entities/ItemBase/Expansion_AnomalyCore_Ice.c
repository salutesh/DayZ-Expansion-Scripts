/**
 * Expansion_AnomalyCore_Ice.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAnomalyCoreLight extends PointLightBase
{
	void ExpansionAnomalyCoreLight()
	{
		SetVisibleDuringDaylight(true);
		SetRadiusTo(1.0);
		SetBrightnessTo(0.2);
		SetCastShadow(false);
		SetFadeOutTime(1.0);
		SetDiffuseColor(1.0, 1.00, 1.0);
		SetAmbientColor(1.0, 1.00, 1.0);
		SetFlareVisible(false);
		SetFlickerAmplitude(0.2);
		SetFlickerSpeed(1.0);
		SetDancingShadowsMovementSpeed(0.4);
		SetDancingShadowsAmplitude(0.5);
	}
};

enum ExpansionAnomalyCoreState
{
	STABLE = 1,
	UNSTABLE = 2,
	DESTROYED = 4
};

class Expansion_AnomalyCore_Base: Grenade_Base 
{
	private static ref set<Expansion_AnomalyCore_Base> s_Expansion_AllAnomalyCores = new set<Expansion_AnomalyCore_Base>;
	
	protected bool m_Exploded;
	protected ParticleSource m_ParticleExploded;
	protected EffectSound m_ExplosionSound;
	protected Particle m_ParticleEffect;
	protected ExpansionAnomalyCoreLight m_EffectLight;
	protected int m_CoreState;
	
	void Expansion_AnomalyCore_Base()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (GetGame().IsServer()|| !GetGame().IsMultiplayer() )
		{
			s_Expansion_AllAnomalyCores.Insert(this);
		}
		
		SetParticleExplosion(ParticleList.RGD5);
		SetGrenadeType(EGrenadeType.CHEMICAL);
		m_Pinned = false;
		SetPinnable(false);
		Arm();
		
		RegisterNetSyncVariableInt("m_CoreState", 1, 3);
		
		if (GetCoreTexturePath() == string.Empty)
		{
			HideSelection("Sun_Hidden");
			//SetObjectTexture(1, GetCoreTexturePath());
		}
	}
	
	void ~Expansion_AnomalyCore_Base()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			int index = s_Expansion_AllAnomalyCores.Find(this);
			if (index > -1)
				s_Expansion_AllAnomalyCores.Remove(index);
		}

		if (m_EffectLight)
			GetGame().ObjectDelete(m_EffectLight);
		
		if (m_ParticleEffect)
			ParticleEffectStop();
	}
	
	override void HideSelection(string selection_name)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::HideSelection - Selection name: " + selection_name);
		
		super.HideSelection(selection_name);
	}
	
	override void ShowSelection(string selection_name)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::ShowSelection - Selection name: " + selection_name);
		
		super.ShowSelection(selection_name);
	}
	
	override void EEInit()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.EEInit();

		InitAnomalyCore();
	}
	
	protected void InitAnomalyCore()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!GetGame().IsDedicatedServer())
		{
			InitAnomalyCoreClient();
		}

		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			InitAnomalyCoreServer();
		}
	}
	
	protected void InitAnomalyCoreClient()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!m_EffectLight)
		{
			m_EffectLight = ExpansionAnomalyCoreLight.Cast(ScriptedLightBase.CreateLight(ExpansionAnomalyCoreLight, GetPosition() + "0 0 0"));
			m_EffectLight.AttachOnObject(this, "0 0 0");
		}
		
		if (!m_ParticleEffect)
			PlayParticle(m_ParticleEffect, GetAnomalyCoreParticle());
	}

	protected void InitAnomalyCoreServer()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		UpdateAnomalyCoreState(ExpansionAnomalyCoreState.STABLE);
	}

	override protected void OnExplode()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_Exploded = true;

		if (GetGame().IsServer())
		{
			GetGame().CreateObject("ExpansionAnomalyArea_Local", GetPosition());
		}
	}

	protected string GetExplosionSoundSet()
	{
		return "Grenade_detonation_SoundSet";
	}

	override void EOnContact(IEntity other, Contact extra)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::EOnContact - Entity: " + other.ToString());

		if (GetGame().IsServer())
		{
			if (!m_Exploded)
			{
				OnActivateFinished();
			}
		}
	}

	/*override void EEKilled(Object killer)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		super.EEKilled(killer);
		
		UpdateAnomalyCoreState(ExpansionAnomalyCoreState.DESTROYED);
	}*/

	void UpdateAnomalyCoreState(ExpansionAnomalyCoreState state)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::UpdateAnomalyCoreState - Anomaly core state: " + typename.EnumToString(ExpansionAnomalyCoreState, state));

		m_CoreState = state;
		SetSynchDirty();
	}
	
	override void OnDamageDestroyed(int oldLevel)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{			
			UpdateAnomalyCoreState(ExpansionAnomalyCoreState.DESTROYED);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Delete, 1000);
	
			int index = s_Expansion_AllAnomalyCores.Find(this);
			if (index > -1)
				s_Expansion_AllAnomalyCores.Remove(index);
		}

	#ifndef SERVER
		ClearFlags(EntityFlags.VISIBLE, false);
		m_ParticleExploded = ParticleManager.GetInstance().PlayInWorld(ParticleList.EXPANSION_PARTICLE_ANOMALY_CORE_AREA, GetPosition());
		PlaySoundSet(m_ExplosionSound, GetExplosionSoundSet(), 0, 0);
	#endif
	}

	override protected void Activate()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		//we don't want base functionality here
	}
	
	protected bool PlayParticle(out Particle particle, int particle_type)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!particle && GetGame() && (!GetGame().IsDedicatedServer()))
		{
			particle = Particle.PlayOnObject(particle_type, this);
			return true;
		}

		return false;
	}
	
	protected void ParticleEffectStop()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		StopParticle(m_ParticleEffect);
	}
	
	override void OnVariablesSynchronized()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.OnVariablesSynchronized();

		UpdateVisualState(m_CoreState);
	}
	
	protected void UpdateVisualState(ExpansionAnomalyCoreState state)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::UpdateVisualState - Anomaly core state: " + typename.EnumToString(ExpansionAnomalyCoreState, state));

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(UpdateAnomalyCoreVFX_Deferred, 0, false, state);
	}

	protected void UpdateAnomalyCoreVFX_Deferred(ExpansionAnomalyCoreState state)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::UpdateAnomalyCoreVFX_Deferred - Anomaly core state: " + typename.EnumToString(ExpansionAnomalyCoreState, state));
		
		switch (state)
		{
			case ExpansionAnomalyCoreState.STABLE:
			{
				if (!m_EffectLight)
				{
					m_EffectLight = ExpansionAnomalyCoreLight.Cast(ScriptedLightBase.CreateLight(ExpansionAnomalyCoreLight, GetPosition() + "0 1 0"));
					m_EffectLight.AttachOnObject(this, "0 0 0");
				}
				
				if (!m_ParticleEffect)
					PlayParticle(m_ParticleEffect, GetAnomalyCoreParticle());
			}
			break;
			case ExpansionAnomalyCoreState.UNSTABLE:
			{
				//! ToDo
			}
			break;
			case ExpansionAnomalyCoreState.DESTROYED:
			{
				if (m_Light)
					GetGame().ObjectDelete(m_Light);
				
				ParticleEffectStop();
			}
			break;
		}
	}
	
	protected bool StopParticle(out Particle particle)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (particle && GetGame() && (!GetGame().IsDedicatedServer()))
		{
			particle.Stop();
			particle = NULL;

			return true;
		}

		return false;
	}
	
	//! @note: Override this is your anomaly core class and set the particle ID for the particle that should get played on the core.
	//! Patricle need to be registerded in the ParticleList class.
	int GetAnomalyCoreParticle()
	{
		return -1;
	}
	
	string GetCoreTexturePath()
	{
		return "";
	}
	
	void SetAnomalyCoreUnstable()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		UpdateAnomalyCoreState(ExpansionAnomalyCoreState.UNSTABLE);
	}
	
	static set<Expansion_AnomalyCore_Base>  GetAllAnomalyCores()
	{
		return s_Expansion_AllAnomalyCores;
	}
	
	protected void DebugTrace(string text)
	{
		EXTrace.Start(EXTrace.NAMALSKADVENTURE, this, text);
	}
}

class Expansion_AnomalyCore_Ice: Expansion_AnomalyCore_Base 
{
	protected const int PARTICLE_EFFECT = ParticleList.EXPANSION_PARTICLE_ANOMALY_CORE_ICE;
	
	override string GetCoreTexturePath()
	{
		return "DayZExpansion\\NamalskAdventure\\Dta\\Items\\Anomaly_Core\\data\\anomaly_core_energy_co.paa";
	}
	
	override int GetAnomalyCoreParticle()
	{
		return PARTICLE_EFFECT;
	}
};