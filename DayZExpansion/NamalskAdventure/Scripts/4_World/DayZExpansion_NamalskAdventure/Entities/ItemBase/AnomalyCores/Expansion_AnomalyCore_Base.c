/**
 * Expansion_AnomalyCore_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAnomalyCoreLightBase extends PointLightBase
{
	void ExpansionAnomalyCoreLightBase()
	{
		SetVisibleDuringDaylight(true);
		SetRadiusTo(4.0);
		SetBrightnessTo(0.2);
		SetCastShadow(false);
		SetFadeOutTime(1.0);
		SetDiffuseColor(1.0, 1.00, 1.0);
		SetAmbientColor(1.0, 1.00, 1.0);
		SetFlareVisible(false);
		//SetFlickerAmplitude(0.5);
		SetFlickerSpeed(0.9);
		SetDancingShadowsMovementSpeed(0.010);
		SetDancingShadowsAmplitude(0.008);
	}
};

enum ExpansionAnomalyCoreState
{
	STABLE = 1,
	UNSTABLE = 2,
	ACTIVATED = 3,
	DESTROYED = 4
};

class Expansion_AnomalyCore_Base: Grenade_Base
{
	static const int CHUNK_SIZE = 10; //! Process 10 anomaly cores at a time.

	static ref CF_DoublyLinkedNodes_WeakRef<Expansion_AnomalyCore_Base> s_Expansion_AllAnomalyCores = new CF_DoublyLinkedNodes_WeakRef<Expansion_AnomalyCore_Base>();
	ref CF_DoublyLinkedNode_WeakRef<Expansion_AnomalyCore_Base> m_Expansion_AnomalyCoreNode = new CF_DoublyLinkedNode_WeakRef<Expansion_AnomalyCore_Base>(this);

	protected ref TStringArray m_Items = {"ItemBase"};
	protected ref TStringArray m_Players = {"SurvivorBase"};
	protected ref TStringArray m_Animals = {"AnimalBase"};
	protected ref TStringArray m_Vehicles = {"Transport"};
	protected ref TStringArray m_Infected = {"ZombieBase"};

	protected bool m_Exploded;
	protected ParticleSource m_ParticleExploded;
	protected EffectSound m_ExplosionSound;
	protected Particle m_ParticleEffect;
	protected ExpansionAnomalyCoreLightBase m_EffectLight;
	protected ExpansionAnomalyCoreState m_CoreState;
	
	static ScriptCaller s_EVRStormStartSC;
	static ScriptCaller s_EVRStormBlowoutSC;

	void Expansion_AnomalyCore_Base()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_Expansion_AnomalyCoreNode = s_Expansion_AllAnomalyCores.Add(this);

		SetParticleExplosion(ParticleList.RGD5);
		SetGrenadeType(EGrenadeType.CHEMICAL);
		m_Pinned = false;
		SetPinnable(false);
		Arm();

		RegisterNetSyncVariableInt("m_CoreState", 1, 3);

		if (GetCoreTexturePath() != string.Empty)
		{
			SetObjectTexture(1, GetCoreTexturePath());
		}
		else
		{
			HideSunSelection();
		}

		if (!ExpansionNamalskModule.GetModuleInstance().IsEVRStormActive())
		{
			UpdateAnomalyCoreState(ExpansionAnomalyCoreState.STABLE);
		}
		else
		{
			UpdateAnomalyCoreState(ExpansionAnomalyCoreState.UNSTABLE);
		}
	}

	void ~Expansion_AnomalyCore_Base()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		CleanupAnomalyCore();
	}

	void CleanupAnomalyCore()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (s_Expansion_AllAnomalyCores)
			s_Expansion_AllAnomalyCores.Remove(m_Expansion_AnomalyCoreNode);

		if (GetGame() && !GetGame().IsDedicatedServer())
		{
			if (m_EffectLight)
				GetGame().ObjectDelete(m_EffectLight);

			if (m_ParticleEffect)
				ParticleEffectStop();
		}
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
			m_EffectLight = ExpansionAnomalyCoreLightBase.Cast(ScriptedLightBase.CreateLight(GetAnomalyCoreLight(), GetPosition() + "0 0 0"));
			m_EffectLight.AttachOnObject(this, "0 0 0");
		}

		if (!m_ParticleEffect)
			PlayParticle(m_ParticleEffect, GetAnomalyCoreParticle());
	}

	protected void InitAnomalyCoreServer()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!ExpansionNamalskModule.GetModuleInstance().IsEVRStormActive())
		{
			UpdateAnomalyCoreState(ExpansionAnomalyCoreState.STABLE);
		}
		else
		{
			if (ExpansionAnomalyCoreProtectiveCase.Cast(GetParent()))
			{
				UpdateAnomalyCoreState(ExpansionAnomalyCoreState.STABLE);
			}
			else
			{
				UpdateAnomalyCoreState(ExpansionAnomalyCoreState.UNSTABLE);
			}
		}
	}

	void HideSunSelection()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		SetAnimationPhase("Sun_Shown", 0);
		SetAnimationPhase("Sun_Hidden", 1);
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
	
	static void EVRStormStart()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE);

		if (!s_EVRStormStartSC)
     		s_EVRStormStartSC = ScriptCaller.Create(OnEVRStormStart);

       	s_Expansion_AllAnomalyCores.Each(s_EVRStormStartSC, CHUNK_SIZE);
	}
	
	static void OnEVRStormStart(Expansion_AnomalyCore_Base anomalyCore)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE);
		
		anomalyCore.SetAnomalyCoreUnstable();
	}
	
	static void EVRStormFinalBlowout()
    {
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE);
	
		if (!s_EVRStormBlowoutSC)
     		s_EVRStormBlowoutSC = ScriptCaller.Create(OnEVRStormFinalBlowout);

       	s_Expansion_AllAnomalyCores.Each(s_EVRStormBlowoutSC, CHUNK_SIZE);
    }
	
	static void OnEVRStormFinalBlowout(Expansion_AnomalyCore_Base anomalyCore)
    {
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE);

		if (!anomalyCore.CanUnsatabilize())
			return;
		
		int gamble2 = Math.RandomInt(0, 2); //! Gamble if this core will explode or not if on player
		int randomTime2 = Math.RandomInt(1, 3);
		if (gamble2 == 0)
		{
			anomalyCore.ActivateAnomalyCore(randomTime2); //! Let the core explode after 1-3 seconds.
		}
		else
		{
			anomalyCore.SetAnomalyCoreStable(); //! Set the core state back to stable
		}
    }

	override protected void OnExplode()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_Exploded = true;

		if (GetGame().IsServer())
		{
			GetGame().CreateObject("ExpansionAnomalyAreaSingularity_Local", GetPosition());
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

	void UpdateAnomalyCoreState(ExpansionAnomalyCoreState state)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::UpdateAnomalyCoreState - Anomaly core state: " + typename.EnumToString(ExpansionAnomalyCoreState, state));

		if (GetGame().IsServer())
		{
			m_CoreState = state;
			SetSynchDirty();
		}
	}

	override void OnDamageDestroyed(int oldLevel)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			UpdateAnomalyCoreState(ExpansionAnomalyCoreState.DESTROYED);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Delete, 1000);
			
			if (s_Expansion_AllAnomalyCores)
				s_Expansion_AllAnomalyCores.Remove(m_Expansion_AnomalyCoreNode);
		}

	#ifndef SERVER
		ClearFlags(EntityFlags.VISIBLE, false);
		m_ParticleExploded = ParticleManager.GetInstance().PlayInWorld(ParticleList.EXPANSION_PARTICLE_ANOMALY_CORE_AREA, GetPosition());
		PlaySoundSet(m_ExplosionSound, GetExplosionSoundSet(), 0, 0);
	#endif
	}

	//! ------------------------------------------------------------------------------------------------------
	//! TESTING
	//! ------------------------------------------------------------------------------------------------------

	override void OnExplosionEffects(Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal, float energyFactor, float explosionFactor, bool isWater, string ammoType)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.OnExplosionEffects(source, directHit, componentIndex, surface, pos, surfNormal, energyFactor, explosionFactor, isWater, ammoType);
	}

	override protected void Activate()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		//we don't want base functionality here
	}

	override void EEDelete(EntityAI parent)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.EEDelete(parent);
	}

	override void EEKilled(Object killer)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.EEKilled(killer);
	}

	override void OnCEUpdate()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.OnCEUpdate();
	}

	override void SetActions()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.SetActions();
	}

	override bool IsInventoryVisible()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!super.IsInventoryVisible())
			return false;

		return true;
	}

	override bool IsTakeable()
	{
		return true;
	}

	override void EEItemLocationChanged(notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.EEItemLocationChanged(oldLoc, newLoc);

		if (newLoc.GetParent() && ExpansionAnomalyCoreProtectiveCase.Cast(newLoc.GetParent()) && m_CoreState == ExpansionAnomalyCoreState.UNSTABLE)
		{
			SetAnomalyCoreStable();
		}

		if (newLoc.GetParent() == null && ExpansionNamalskModule.GetModuleInstance().IsEVRStormActive() && m_CoreState == ExpansionAnomalyCoreState.UNSTABLE)
		{
			SetAnomalyCoreUnstable();
		}
	}

	override void OnInventoryExit(Man player)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		//! @note: Make sure state is properly synchronized or VFX might bug out
		SetSynchDirty();
	}

	override void OnInventoryEnter(Man player)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		//! @note:  Make sure to stop particles once in inventory
		if (m_ParticleEffect)
			StopParticle(m_ParticleEffect);
	}

	override void OnWasAttached(EntityAI parent, int slot_id)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.OnWasAttached(parent, slot_id);
	}

	override void OnStoreSave(ParamsWriteContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.OnStoreSave(ctx);
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!super.OnStoreLoad(ctx, version))
			return false;

		return true;
	}

	//! ------------------------------------------------------------------------------------------------------

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
					m_EffectLight = ExpansionAnomalyCoreLightBase.Cast(ScriptedLightBase.CreateLight(GetAnomalyCoreLight(), GetPosition() + "0 1 0"));
					m_EffectLight.AttachOnObject(this, "0 0 0");
				}

				if (!m_ParticleEffect)
					PlayParticle(m_ParticleEffect, GetAnomalyCoreParticle());

				if (m_ParticleEffect && !ExpansionNamalskModule.GetModuleInstance().IsEVRStormActive())
					TurnOffUnstableEmitor();
			}
			break;
			case ExpansionAnomalyCoreState.UNSTABLE:
			{
				if (m_ParticleEffect)
					TurnOnUnstableEmitor();
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

	protected void TurnOnUnstableEmitor()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (m_ParticleEffect)
		{
			m_ParticleEffect.SetParameter(0, EmitorParam.LIFETIME, 1);
			m_ParticleEffect.SetParameter(0, EmitorParam.REPEAT, 1);
		}
	}

	protected void TurnOffUnstableEmitor()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (m_ParticleEffect)
		{
			m_ParticleEffect.SetParameter(0, EmitorParam.LIFETIME, 0);
			m_ParticleEffect.SetParameter(0, EmitorParam.REPEAT, 0);
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

	void SetAnomalyCoreStable()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		UpdateAnomalyCoreState(ExpansionAnomalyCoreState.STABLE);
	}

	//! @note: Let the anomaly explode and create a anormal effect zone.
	void ActivateAnomalyCore(int explosionTime)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OnActivateFinished, explosionTime);
	}

	bool CanUnsatabilize()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (GetParent() && ExpansionAnomalyCoreProtectiveCase.Cast(GetParent()))
			return false;

		if (m_CoreState == ExpansionAnomalyCoreState.UNSTABLE)
			return false;

		/*if (GetHierarchyRoot() && Expansion_Anomaly_Base.Cast(GetHierarchyRoot()))
			return false;*/

		return true;
	}

	override void OnItemLocationChanged(EntityAI old_owner, EntityAI new_owner)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::OnItemLocationChanged - Old owner : " + old_owner.ToString() + " | New owner: " + new_owner.ToString());

		super.OnItemLocationChanged(old_owner, new_owner);

		if (m_CoreState != ExpansionAnomalyCoreState.UNSTABLE && (GetParent() == null && ExpansionNamalskModule.GetModuleInstance().IsEVRStormActive()))
		{
			SetAnomalyCoreUnstable();
		}
		else
		{
			if (m_CoreState != ExpansionAnomalyCoreState.STABLE && (ExpansionAnomalyCoreProtectiveCase.Cast(GetParent()) || !ExpansionNamalskModule.GetModuleInstance().IsEVRStormActive()))
				SetAnomalyCoreStable();
		}
	}

	//! @note: Event after core explosion.
	override void OnActivateFinished()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		UpdateAnomalyCoreState(ExpansionAnomalyCoreState.DESTROYED);

		super.OnActivateFinished();
	}

	ExpansionAnomalyCoreState GetAnomalyCoreState()
	{
		return m_CoreState;
	}

	typename GetAnomalyCoreLight()
	{
		return ExpansionAnomalyCoreLightBase;
	}

	protected void DebugTrace(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Start(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};