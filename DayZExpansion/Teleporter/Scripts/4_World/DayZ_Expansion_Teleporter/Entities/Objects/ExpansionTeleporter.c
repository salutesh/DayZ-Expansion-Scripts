/**
 * ExpansionTeleporter.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionQuestSExpansionTeleporterBase
 * @brief		This class handles teleporter objects used to head-out or take-in quests
 **/

class Expansion_Teleporter_Base: BuildingSuper
{
	protected int m_TeleporterID;	 //! Unique teleporter id. Used to get and identify the teleporter in the teleporter module.
	protected bool m_IsActive;

#ifdef DIAG
#ifdef EXPANSIONMODNAVIGATION
	protected ExpansionMarkerData m_ServerMarker;
#endif
#endif

	void Expansion_Teleporter_Base()
	{
		if (IsMissionHost())
			SetAllowDamage(false);
		
		SetEventMask(EntityEvent.INIT);

		m_TeleporterID = -1;
		m_IsActive = true;
		
		RegisterNetSyncVariableInt("m_TeleporterID");
		RegisterNetSyncVariableInt("m_IsActive");
	}
	
	void ~Expansion_Teleporter_Base()
	{
	#ifdef DIAG
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
	
	override void EEInit()
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);

		super.EEInit();

		InitTeleporter();
	}
	
	protected void InitTeleporter()
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);

		if (!GetGame().IsDedicatedServer())
		{
			InitTeleporterClient();
		}

		if (GetGame().IsServer())
		{
			InitTeleporterServer();
		}
	}
	
	protected void InitTeleporterClient()
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
	}

	protected void InitTeleporterServer()
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);

	#ifdef DIAG
	#ifdef EXPANSIONMODNAVIGATION
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CreateDebugMarker, 500, false);
	#endif
	#endif
	}
	
	void SetTeleporterID(int id)
	{
		m_TeleporterID = id;
		EXPrint(ToString() + "::SetTeleporterID - ID: " + m_TeleporterID);
	}

	int GetTeleporterID()
	{
		return m_TeleporterID;
	}

	bool IsEVRStormActive()
	{
	#ifdef NAMALSK_SURVIVAL
	    NamEventManager event_manager;
	    g_Script.CallFunction(GetGame().GetMission(), "GetNamEventManager", event_manager, null);

	    if (!event_manager)
	        return false;

	    return EVRStorm.Cast(event_manager.GetEvent(EVRStorm)) || EVRStorm.Cast(event_manager.GetEvent(EVRStormDeadly));
	#endif

		return false;
	}
	
	bool IsActive()
	{
		return m_IsActive;
	}
	
	void SetActive(bool state)
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		
		m_IsActive = state;
		
		SetSynchDirty();
	}
	
	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}
	
	override bool DisableVicinityIcon()
    {
        return true;
    }

#ifdef DIAG
#ifdef EXPANSIONMODNAVIGATION
	void CreateDebugMarker()
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		
		ExpansionMarkerModule markerModule;
		CF_Modules<ExpansionMarkerModule>.Get(markerModule);
		if (markerModule)
			markerModule.CreateServerMarker(GetType(), "Moon", Vector(GetPosition()[0], GetPosition()[1] + 1.0, GetPosition()[2]), ARGB(255, 142, 68, 173), true);
	}
#endif
#endif
	
	protected void DebugTrace(string text)
	{
	#ifdef EXPANSIONMODTELEPORTERDEBUG
		EXTrace.Start(EXTrace.TELEPORTER, this, text);
	#endif
	}
};

enum ExpansionTeleporterState
{
	OFF = 0,
	IDLE = 1,
	ACTIVATED = 2,
	UNSTABLE = 3
};

class Expansion_Teleporter_Big: Expansion_Teleporter_Base
{
	protected const string SOUND_IDLE = "Expansion_Teleporter_Idle_Soundset";
	protected const string SOUND_ACTIVATED = "Expansion_Teleporter_Active_Soundset";
	
	protected EffectSound m_Sound;
	protected EffectSound m_SoundActivated;
	protected Particle m_ParticleIdle;

	protected ExpansionTeleporterState m_TeleporterState = ExpansionTeleporterState.OFF;
	protected ExpansionTeleporterState m_PrevTeleporterState = ExpansionTeleporterState.OFF;
	protected ExpansionTeleporterState m_VisualState = ExpansionTeleporterState.OFF;

	//! Particles
	protected const int PARTICLE_TELEPORTER_IDLE = ParticleList.EXPANSION_PARTICLE_TELEPORTER;
	
	void Expansion_Teleporter_Big()
	{
		RegisterNetSyncVariableInt("m_TeleporterState", 0, 3);
		RegisterNetSyncVariableInt("m_PrevTeleporterState", 0, 3);
	}
	
	override void SetActive(bool state)
	{
		if (!state)
		{
			SetTeleporterState(ExpansionTeleporterState.OFF);
		}
		else
		{
			SetTeleporterState(ExpansionTeleporterState.IDLE);
		}

		DebugTrace("::SetActive - Teleporter state: " + typename.EnumToString(ExpansionTeleporterState, m_TeleporterState));

		super.SetActive(state);
	}
	
	protected bool PlayParticle(out Particle particle, int particle_type)
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);

		if (!particle && GetGame() && (!GetGame().IsDedicatedServer()))
		{
			particle = Particle.PlayOnObject(particle_type, this, "0 0.2 0");
			return true;
		}

		return false;
	}

	//! Returns true if particle stopped, false if not
	protected bool StopParticle(out Particle particle)
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);

		if (particle && GetGame() && (!GetGame().IsDedicatedServer()))
		{
			particle.Stop();
			particle = null;

			return true;
		}

		return false;
	}

	void SetTeleporterState(ExpansionTeleporterState state)
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		DebugTrace("::SetTeleporterState - Set teleporter state: " + typename.EnumToString(ExpansionTeleporterState, state));

		m_PrevTeleporterState = m_TeleporterState;
		m_TeleporterState = state;

		SetSynchDirty();
	}
	
	protected void SetVisualState(ExpansionTeleporterState state)
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		DebugTrace("::SetVisualState - Teleporter state: " + typename.EnumToString(ExpansionTeleporterState, state));

		m_VisualState = state;
	}
	
	protected void SoundIdleStart()
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		PlaySoundSetLoop(m_Sound, SOUND_IDLE, 1.0, 1.0);
	}

	protected void SoundStop()
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		StopSoundSet(m_Sound);
	}

	protected void SoundActivatedStart()
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		PlaySoundSet(m_SoundActivated, SOUND_ACTIVATED, 1.0, 1.0);
	}

	protected void ParticleIdleStop()
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		StopParticle(m_ParticleIdle);
	}

	protected void UpdateVisualState(ExpansionTeleporterState state)
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		DebugTrace("::UpdateVisualState - Teleporter state is: " + typename.EnumToString(ExpansionTeleporterState, state));

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(UpdateTeleporterVFX_Deferred, 0, false, state);
	}
	
	//! @note: This method updates the teleporters visual effects (VFX) in a deferred manner based on the provided `state`.
	protected void UpdateTeleporterVFX_Deferred(ExpansionTeleporterState state)
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		DebugTrace("::UpdateTeleporterVFX_Deferred - Teleporter state: " + typename.EnumToString(ExpansionTeleporterState, state) + " | Previous teleporter state: " + typename.EnumToString(ExpansionTeleporterState, m_PrevTeleporterState));
		
		//! Stop current particle effects
		ParticleIdleStop();

		switch (state)
		{
			case ExpansionTeleporterState.IDLE:
			{
				//! Create idle VFX particle
			    CreateIdleParticle(state);
				TurnOffUnstableEmitor();
				TurnOffActivatedEmitor();
			}
			break;
			case ExpansionTeleporterState.ACTIVATED:
			{
				//! Create activated VFX particle
				CreateIdleParticle(state);
				SoundActivatedStart();
				TurnOffUnstableEmitor();
			}
			break;
			case ExpansionTeleporterState.UNSTABLE:
			{
				//! Create idle VFX particle
				CreateIdleParticle(state);
				TurnOffActivatedEmitor();
			}
			break;
		}
	}
	
	protected void TurnOnUnstableEmitor()
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);

		if (!m_ParticleIdle)
		{
			Error("::TurnOnUnstableEmitor - Could not get idle particle!");
			return;
		}

		m_ParticleIdle.SetParameter(0, EmitorParam.LIFETIME, 1);
		m_ParticleIdle.SetParameter(0, EmitorParam.REPEAT, 1);
	}

	protected void TurnOffUnstableEmitor()
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);

		if (!m_ParticleIdle)
		{
			Error("::TurnOnUnstableEmitor - Could not get idle particle!");
			return;
		}

		m_ParticleIdle.SetParameter(0, EmitorParam.LIFETIME, 0);
		m_ParticleIdle.SetParameter(0, EmitorParam.REPEAT, 0);
	}
	
	protected void TurnOnActivatedEmitor()
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);

		if (!m_ParticleIdle)
		{
			Error("::TurnOnActivatedEmitor - Could not get idle particle!");
			return;
		}

		m_ParticleIdle.SetParameter(1, EmitorParam.LIFETIME, 1);
		m_ParticleIdle.SetParameter(1, EmitorParam.REPEAT, 1);
	}

	protected void TurnOffActivatedEmitor()
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);

		if (!m_ParticleIdle)
		{
			Error("::TurnOnActivatedEmitor - Could not get idle particle!");
			return;
		}

		m_ParticleIdle.SetParameter(1, EmitorParam.LIFETIME, 0);
		m_ParticleIdle.SetParameter(1, EmitorParam.REPEAT, 0);
	}

	protected void CreateIdleParticle(ExpansionTeleporterState state)
	{
		//! Create idle VFX particle
		PlayParticle(m_ParticleIdle, GetTeleporterIdleParticle());

		//! Create idle sound
		if (!m_Sound)
			SoundIdleStart();

		SetVisualState(state);
	}
	
	//! @note: Synchronizes variables and updates visual state of the particle depending on the anomaly state.
	override void OnVariablesSynchronized()
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		DebugTrace("::OnVariablesSynchronized - Current teleporter visual state: " + typename.EnumToString(ExpansionTeleporterState, m_VisualState) + " | Anomaly state: " + typename.EnumToString(ExpansionTeleporterState, m_TeleporterState));

		super.OnVariablesSynchronized();

		if (m_VisualState != m_TeleporterState)
			UpdateVisualState(m_TeleporterState);
	}
	
	override bool IsHealthVisible()
	{
		return true;
	}

	override bool IsInventoryVisible()
	{
		return true;
	}

	override bool CanDisplayCargo()
	{
		return true;
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return true;
	}

	override bool CanPutIntoHands(EntityAI player)
	{
		return false;
	}

	override bool CanRemoveFromCargo(EntityAI parent)
	{
		return true;
	}

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		return true;
	}

	override bool CanLoadItemIntoCargo(EntityAI item)
	{
		return true;
	}

	override bool DisableVicinityIcon()
    {
        return true;
    }
	
	int GetTeleporterIdleParticle()
	{
		return PARTICLE_TELEPORTER_IDLE;
	}
};