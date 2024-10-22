/**
 * Expansion_EffectHolder_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_EffectHolder_Base: House
{
	protected const string SOUND_IDLE = "Expansion_AnomalyRumble_Soundset";
	protected const string SOUND_ACTIVATED = "Expansion_AnomalyWindBlowActivated_Soundset";
	protected const int TRIGGER_RADIUS = 2;
	protected const string OBJECT_CLUTTER_CUTTER = "ClutterCutterFireplace";

	static ref CF_DoublyLinkedNodes_WeakRef<Expansion_EffectHolder_Base> s_Expansion_AllEffects = new CF_DoublyLinkedNodes_WeakRef<Expansion_EffectHolder_Base>();
	ref CF_DoublyLinkedNode_WeakRef<Expansion_EffectHolder_Base> m_Expansion_EffectNode = new CF_DoublyLinkedNode_WeakRef<Expansion_EffectHolder_Base>(this);

	protected Particle m_ParticleIdle;
	protected Particle m_ParticleActivated;

	protected bool m_DelayVFX = true;
	protected float m_IdleParticleDelay;

#ifdef DIAG_DEVELOPER
#ifdef EXPANSIONMODNAVIGATION
	protected ExpansionMarkerData m_ServerMarker;
#endif
#endif

	void Expansion_EffectHolder_Base()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		SetEventMask(EntityEvent.INIT); //! @ToDO: Not sure if even needed. Check that!
	}
	
	void ~Expansion_EffectHolder_Base()
	{
		if (!GetGame())
			return;

		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		CleanupEffect();
	}

	override void EEDelete(EntityAI parent)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		CleanupEffect();
	}

	void CleanupEffect()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		
		if (GetGame() && GetGame().IsServer())
		{
		#ifdef DIAG_DEVELOPER
		#ifdef EXPANSIONMODNAVIGATION
			if (!m_ServerMarker)
				return;

			ExpansionMarkerModule markerModule;
			CF_Modules<ExpansionMarkerModule>.Get(markerModule);
			if (markerModule)
				markerModule.RemoveServerMarker(m_ServerMarker.GetUID());
		#endif
		#endif
		}

		if (GetGame() && !GetGame().IsDedicatedServer())
		{
			if (m_ParticleIdle)
				ParticleIdleStop();

			if (m_ParticleActivated)
				ParticleActivatedStop();
		}
	}

	override void EEInit()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		super.EEInit();

		InitEffect();
	}

	protected void InitEffect()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		if (!GetGame().IsDedicatedServer())
		{
			InitEffectClient();
		}

		if (GetGame().IsServer())
		{
			InitEffectServer();
		}
	}

	protected void InitEffectClient()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		
		UpdateVisualState();
	}

	protected void InitEffectServer()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		//! Remove grass
		Object cc_object = GetGame().CreateObjectEx(OBJECT_CLUTTER_CUTTER , GetWorldPosition(), ECE_PLACE_ON_SURFACE);
		cc_object.SetOrientation (GetOrientation());
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DestroyClutterCutter, 200, false, cc_object);

		//SetSynchDirty();
	}

	protected void DestroyClutterCutter(Object clutter_cutter)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		GetGame().ObjectDelete(clutter_cutter);

	#ifdef DIAG_DEVELOPER
	#ifdef EXPANSIONMODNAVIGATION
		ExpansionMarkerModule markerModule;
		CF_Modules<ExpansionMarkerModule>.Get(markerModule);
		if (markerModule)
			markerModule.CreateServerMarker(GetType(), "Anomaly", Vector(GetPosition()[0], GetPosition()[1] + 1.0, GetPosition()[2]), ARGB(255, 52, 152, 219), true);
	#endif
	#endif
	}

	protected bool PlayParticle(out Particle particle, int particle_type)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		if (!particle && GetGame() && (!GetGame().IsDedicatedServer()))
		{
			particle = Particle.PlayOnObject(particle_type, this, "0 0.7 0");
			return true;
		}

		return false;
	}

	//! Returns true if particle stopped, false if not
	protected bool StopParticle(out Particle particle)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		if (particle && GetGame() && (!GetGame().IsDedicatedServer()))
		{
			particle.Stop();
			particle = null;

			return true;
		}

		return false;
	}

	protected void ParticleIdleStop()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		StopParticle(m_ParticleIdle);
	}

	protected void ParticleActivatedStop()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		StopParticle(m_ParticleActivated);
	}

	protected void UpdateVisualState()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(UpdateEffectVFX_Deferred, 0, false);
	}

	//! @note: This method updates the anomaly visual effects (VFX) in a deferred manner based on the provided `state`.
	protected void UpdateEffectVFX_Deferred(ExpansionAnomalyState state)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		//! Stop current particle effects
		ParticleIdleStop();
		ParticleActivatedStop();
		PlayParticle(m_ParticleActivated, GetEffectActivatedParticle());
	}

	protected void CreateIdleParticle(ExpansionAnomalyState state)
	{
		//! Create idle VFX particle
		PlayParticle(m_ParticleIdle, GetEffectIdleParticle());
	}

	//! @note: Override this is your anomaly class and set the particle ID for the particle that should get played when the anomaly is in idle or without core.
	//! Patricle need to be registerded in the ParticleList class.
	int GetEffectIdleParticle()
	{
		return -1;
	}

	//! @note: Override this is your anomaly class and set the particle ID for the particle that should get played when the anomaly is activated/triggered.
	//! Patricle need to be registerded in the ParticleList class.
	int GetEffectActivatedParticle()
	{
		return -1;
	}

	//! @note: Synchronizes variables and updates visual state of the particle depending on the anomaly state.
	override void OnVariablesSynchronized()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		super.OnVariablesSynchronized();

		UpdateVisualState();
	}

	protected void ExDebugPrint(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};

class Expansion_EffectHolder_Teleport: Expansion_EffectHolder_Base
{
	protected const int PARTICLE_TELEPORT_IDLE = ParticleList.EXPANSION_PARTICLE_WARPER;
	
	override int GetEffectIdleParticle()
	{
		return PARTICLE_TELEPORT_IDLE;
	}
};