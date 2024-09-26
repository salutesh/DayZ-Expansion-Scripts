/**
 * ExpansionTeleporter.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
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
		
		//SetEventMask(EntityEvent.INIT);

		m_TeleporterID = -1;
		
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

		#ifndef SERVER
		InitTeleporterClient();
		#else
		InitTeleporterServer();
		#endif
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

#ifdef DIAG
#ifdef EXPANSIONMODNAVIGATION
	void CreateDebugMarker()
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		
		ExpansionMarkerModule markerModule;
		CF_Modules<ExpansionMarkerModule>.Get(markerModule);
		if (markerModule)
			m_ServerMarker = markerModule.CreateServerMarker(GetType(), "Moon", Vector(GetPosition()[0], GetPosition()[1] + 1.0, GetPosition()[2]), ARGB(255, 142, 68, 173), true);
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
	//ACTIVATED = 2,
	//UNSTABLE = 3
};

class Expansion_Teleporter_Big: Expansion_Teleporter_Base
{
	protected const string SOUND_IDLE = "Expansion_Teleporter_Idle_Soundset";
	protected const string SOUND_ACTIVATED = "Expansion_Teleporter_Active_Soundset";
	
	protected EffectSound m_Sound;
	protected EffectSound m_SoundActivated;
	protected Particle m_ParticleIdle;

	protected int m_TeleporterState = ExpansionTeleporterState.OFF;
	protected int m_VisualState = ExpansionTeleporterState.OFF;

	//! Particles
	protected const int PARTICLE_TELEPORTER_IDLE = ParticleList.EXPANSION_PARTICLE_TELEPORTER;

#ifdef EXPANSION_NAMALSK_ADVENTURE
	protected bool m_NeedKeyCard;
#endif
	
	void Expansion_Teleporter_Big()
	{
		m_TeleporterState = ExpansionTeleporterState.OFF;	
		m_VisualState = ExpansionTeleporterState.OFF;
		
		RegisterNetSyncVariableInt("m_TeleporterState", 0, 1);
	}
	
	void ~Expansion_Teleporter_Big()
	{
		#ifndef SERVER
		CleanupTeleporterVFX();
		#endif
	}
	
	void CleanupTeleporterVFX()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	
		if (m_ParticleIdle)
			StopParticle();

		if (m_Sound)
			SoundStop();
	}
	
	override protected void InitTeleporterClient()
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		
		super.InitTeleporterClient();
		
		UpdateVisualState(m_TeleporterState);
	}
	
	override void SetActive(bool state)
	{
		if (!state)
			m_TeleporterState = ExpansionTeleporterState.OFF;
		else
			m_TeleporterState = ExpansionTeleporterState.IDLE;

		DebugTrace("::SetActive - Teleporter state: " + typename.EnumToString(ExpansionTeleporterState, m_TeleporterState));

		super.SetActive(state);
	}
	
	protected void SetVisualState(ExpansionTeleporterState state)
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);

		m_VisualState = state;
	}
	
	protected void PlayParticle()
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);

		if (!m_ParticleIdle && GetGame() && (!GetGame().IsDedicatedServer()))
			m_ParticleIdle = Particle.PlayOnObject(GetTeleporterIdleParticle(), this, "0 0.2 0");
	}

	//! Returns true if particle stopped, false if not
	protected bool StopParticle()
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);

		if (m_ParticleIdle && GetGame() && (!GetGame().IsDedicatedServer()))
		{
			m_ParticleIdle.Stop();
			m_ParticleIdle = null;

			return true;
		}

		return false;
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

	protected void UpdateVisualState(ExpansionTeleporterState state)
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		DebugTrace("::UpdateVisualState - Teleporter state is: " + typename.EnumToString(ExpansionTeleporterState, state));
		
		#ifndef SERVER
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Call(UpdateTeleporterVFX_Deferred, state);
		#endif
	}
	
	//! @note: This method updates the teleporters visual effects (VFX) in a deferred manner based on the provided `state`.
	protected void UpdateTeleporterVFX_Deferred(int state)
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		
		//! Stop current effects
		StopParticle();
		
		if (m_Sound)
			SoundStop();
						
		switch (state)
		{
			case ExpansionTeleporterState.OFF:
			{
				//TurnOffUnstableEmitor();
				//TurnOffActivatedEmitor();
				SetVisualState(state);
			}
			break;
			case ExpansionTeleporterState.IDLE:
			{
				//! Create idle VFX particle
			    CreateIdleParticle(state);
				TurnOffUnstableEmitor();
				TurnOffActivatedEmitor();
				SetVisualState(state);
			}
			break;
			/*case ExpansionTeleporterState.ACTIVATED:
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
			break;*/
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

	protected void CreateIdleParticle(int state)
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		
		//! Create idle VFX particle
		if (!m_ParticleIdle)
			PlayParticle();

		//! Create idle sound
		if (!m_Sound)
			SoundIdleStart();
	}
	
	//! @note: Synchronizes variables and updates visual state of the particle depending on the anomaly state.
	override void OnVariablesSynchronized()
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		
		if (m_VisualState != m_TeleporterState)
			UpdateVisualState(m_TeleporterState);
	}
	
	int GetTeleporterIdleParticle()
	{
		return PARTICLE_TELEPORTER_IDLE;
	}
	
#ifdef EXPANSION_NAMALSK_ADVENTURE
	void SetNeedKeyCard(bool state)
	{
		m_NeedKeyCard = state;
	}
	
	bool NeedKeyCard()
	{
		return m_NeedKeyCard;
	}
	
	override void SetActions()
	{
		super.SetActions();
		AddAction(ExpansionActionUseTeleporter);
	}
#endif
};