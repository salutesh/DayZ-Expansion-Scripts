/**
 * Expansion_AnomalyCore_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
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

	void Expansion_AnomalyCore_Base()
	{
		m_Expansion_AnomalyCoreNode = s_Expansion_AllAnomalyCores.Add(this);

		SetParticleExplosion(ParticleList.RGD5);
		SetGrenadeType(EGrenadeType.CHEMICAL);
		m_Pinned = false;
		SetPinnable(false);
		Arm();

		RegisterNetSyncVariableInt("m_CoreState", 1, 4);

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
		if (GetGame())
			CleanupAnomalyCore();
	}

	void CleanupAnomalyCore()
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

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
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

		super.EEInit();

		InitAnomalyCore();
	}

	protected void InitAnomalyCore()
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

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
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

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
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

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

	override protected void OnExplode()
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

		if (GetGame().IsServer())
		{
			m_Exploded = true;
			GetGame().CreateObject("ExpansionAnomalyAreaSingularity_Local", GetPosition());
		}
	}

	protected string GetExplosionSoundSet()
	{
		return "Grenade_detonation_SoundSet";
	}

	override void EOnContact(IEntity other, Contact extra)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		ExDebugPrint("::EOnContact - Entity: " + other.ToString());

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
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		ExDebugPrint("::UpdateAnomalyCoreState - Anomaly core state: " + typename.EnumToString(ExpansionAnomalyCoreState, state));

		if (GetGame().IsServer())
		{
			m_CoreState = state;
			SetSynchDirty();
		}
	}

	override void OnDamageDestroyed(int oldLevel)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

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

	override protected void Activate()
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		//! We don't want base functionality here
	}

	override bool IsTakeable()
	{
		return true;
	}

	override void EEItemLocationChanged(notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

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
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

		super.OnInventoryExit(player);

		//! @note: Make sure state is properly synchronized or VFX might bug out
		SetSynchDirty();
	}

	override void OnInventoryEnter(Man player)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

		super.OnInventoryEnter(player);

		//! @note:  Make sure to stop particles once in inventory
		if (m_ParticleEffect)
			StopParticle(m_ParticleEffect);
	}

	//! ------------------------------------------------------------------------------------------------------

	protected bool PlayParticle(out Particle particle, int particle_type)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

		if (!particle && GetGame() && (!GetGame().IsDedicatedServer()))
		{
			particle = Particle.PlayOnObject(particle_type, this);
			return true;
		}

		return false;
	}

	protected void ParticleEffectStop()
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		StopParticle(m_ParticleEffect);
	}

	override void OnVariablesSynchronized()
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

		super.OnVariablesSynchronized();
		UpdateVisualState(m_CoreState);
	}

	protected void UpdateVisualState(ExpansionAnomalyCoreState state)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		ExDebugPrint("::UpdateVisualState - Anomaly core state: " + typename.EnumToString(ExpansionAnomalyCoreState, state));
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(UpdateAnomalyCoreVFX_Deferred, 0, false, state);
	}

	protected void UpdateAnomalyCoreVFX_Deferred(ExpansionAnomalyCoreState state)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		ExDebugPrint("::UpdateAnomalyCoreVFX_Deferred - Anomaly core state: " + typename.EnumToString(ExpansionAnomalyCoreState, state));

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
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

		if (m_ParticleEffect)
		{
			m_ParticleEffect.SetParameter(0, EmitorParam.LIFETIME, 1);
			m_ParticleEffect.SetParameter(0, EmitorParam.REPEAT, 1);
		}
	}

	protected void TurnOffUnstableEmitor()
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

		if (m_ParticleEffect)
		{
			m_ParticleEffect.SetParameter(0, EmitorParam.LIFETIME, 0);
			m_ParticleEffect.SetParameter(0, EmitorParam.REPEAT, 0);
		}
	}

	protected bool StopParticle(out Particle particle)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

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

	void SetAnomalyCoreUnstable()
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

		UpdateAnomalyCoreState(ExpansionAnomalyCoreState.UNSTABLE);
	}

	void SetAnomalyCoreStable()
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

		UpdateAnomalyCoreState(ExpansionAnomalyCoreState.STABLE);
	}

	//! @note: Let the anomaly explode and create a anormal effect zone.
	void ActivateAnomalyCore(int explosionTime)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OnActivateFinished, explosionTime);
	}

	bool CanUnsatabilize()
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

		if (GetParent() && ExpansionAnomalyCoreProtectiveCase.Cast(GetParent()))
			return false;

		if (m_CoreState == ExpansionAnomalyCoreState.UNSTABLE)
			return false;

		return true;
	}

	override void OnItemLocationChanged(EntityAI old_owner, EntityAI new_owner)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		ExDebugPrint("::OnItemLocationChanged - Old owner : " + old_owner.ToString() + " | New owner: " + new_owner.ToString());

		super.OnItemLocationChanged(old_owner, new_owner);

		if (m_CoreState != ExpansionAnomalyCoreState.UNSTABLE && (GetParent() == null && ExpansionNamalskModule.GetModuleInstance().IsEVRStormActive()))
			SetAnomalyCoreUnstable();

		if (m_CoreState != ExpansionAnomalyCoreState.STABLE && (ExpansionAnomalyCoreProtectiveCase.Cast(GetParent()) || !ExpansionNamalskModule.GetModuleInstance().IsEVRStormActive()))
			SetAnomalyCoreStable();
	}

	//! @note: Event after core explosion.
	override void OnActivateFinished()
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

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

	void SetSunSelectionMaterial(string material_name)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		ExDebugPrint("::SetSunSelectionMaterial - Material: " + material_name);

		SetObjectMaterial(1, material_name);
		SetObjectTexture(1, "");
	}

	protected void ExDebugPrint(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};
