/**
 * Expansion_Anomaly_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionAnomalyState
{
	NONE = 0,
	IDLE = 1,
	ACTIVATED = 2,
	NOCORE = 3,
	UNSTABLE = 4,
	UNSTABLENOCORE = 5
};

class ExpansionAnomalyLightBase extends PointLightBase
{
	void ExpansionAnomalyLightBase()
	{
		SetVisibleDuringDaylight(true);
		SetRadiusTo(4.0);
		SetBrightnessTo(0.15);
		SetCastShadow(false);
		SetFadeOutTime(1.0);
		SetDiffuseColor(1.0, 1.00, 1.0);
		SetAmbientColor(1.0, 1.00, 1.0);
		SetFlareVisible(false);
		SetFlickerAmplitude(1.0);
		SetFlickerSpeed(0.9);
		SetDancingShadowsMovementSpeed(0.010);
		SetDancingShadowsAmplitude(0.008);
	}
};

class Expansion_Anomaly_Base: ItemBase
{
	protected const string SOUND_IDLE = "Expansion_AnomalyRumble_Soundset";
	protected const string SOUND_ACTIVATED = "Expansion_AnomalyWindBlowActivated_Soundset";
	protected const int TRIGGER_RADIUS = 2;
	protected const string OBJECT_CLUTTER_CUTTER = "ClutterCutterFireplace";

	static ref CF_DoublyLinkedNodes_WeakRef<Expansion_Anomaly_Base> s_Expansion_AllAnomalies = new CF_DoublyLinkedNodes_WeakRef<Expansion_Anomaly_Base>();
	ref CF_DoublyLinkedNode_WeakRef<Expansion_Anomaly_Base> m_Expansion_AnomalyNode = new CF_DoublyLinkedNode_WeakRef<Expansion_Anomaly_Base>(this);

	protected ExpansionAnomalyLightBase m_Light;
	protected EffectSound m_Sound;
	protected EffectSound m_SoundActivated;
	protected Particle m_ParticleIdle;
	protected Particle m_ParticleActivated;

	protected bool m_DelayVFX = true;
	protected float m_IdleParticleDelay;

	protected ExpansionAnomalyTriggerBase m_AnomalyTrigger;
	protected Expansion_AnomalyCore_Base m_AnomalyCore;

	protected ExpansionAnomalyState m_AnonmalyState = ExpansionAnomalyState.NONE;
	protected ExpansionAnomalyState m_PrevAnonmalyState = ExpansionAnomalyState.NONE;
	protected ExpansionAnomalyState m_VisualState = ExpansionAnomalyState.NONE;

	#ifdef DIAG
	#ifdef EXPANSIONMODNAVIGATION
	protected ExpansionMarkerData m_ServerMarker;
	#endif
	#endif

	protected ref array <ref ExpansionLoot> m_LootConfig;
	protected ref array<EntityAI> m_LootItems;
	protected int m_LootItemsMin;
	protected int m_LootItemsMax;
	protected int m_LootSpawnType;

	protected int m_SurviviedEVRStorms;

	void Expansion_Anomaly_Base()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_Expansion_AnomalyNode = s_Expansion_AllAnomalies.Add(this);
		
		#ifdef SERVER
		m_LootConfig = new array <ref ExpansionLoot>;
		m_LootItems = new array<EntityAI>;
		#endif

		//! @note: Add this to the class of your anomaly when inhertiting from this base and you plan to use the EOnTouch and EOnContact methods.
		//! Also make sure you anomaly has additonaly set the event flags "EntityEvent.CONTACT" & "EntityEvent.TOUCH" via "SetEventMask(EntityEvent.CONTACT | EntityEvent.TOUCH);".
		//SetFlags(EntityFlags.TOUCHTRIGGERS, false);

		SetEventMask(EntityEvent.INIT);
		
		RegisterNetSyncVariableInt("m_AnonmalyState", 0, 5);
		RegisterNetSyncVariableInt("m_PrevAnonmalyState", 0, 5);
	}
	
	void ~Expansion_Anomaly_Base()
	{
		if (GetGame())
			CleanupAnomaly();
	}
	
	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (!super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef))
			return false;

		if (m_AnomalyTrigger && ammo != string.Empty)
		{
			if (ammo.IndexOf("Bullet") > -1)
				m_AnomalyTrigger.EOnEnter(source, 0);
		}
		
		return false;
	}

	void CleanupAnomaly()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (s_Expansion_AllAnomalies)
			s_Expansion_AllAnomalies.Remove(m_Expansion_AnomalyNode);
		
		#ifdef SERVER
		if (m_AnomalyTrigger)
			GetGame().ObjectDelete(m_AnomalyTrigger);

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

		foreach (EntityAI lootItem: m_LootItems)
		{
			if (!lootItem.GetParent())
				GetGame().ObjectDelete(lootItem);
		}
		#endif
	
		#ifndef SERVER
		if (m_ParticleIdle)
			ParticleIdleStop();

		if (m_ParticleActivated)
			ParticleActivatedStop();

		if (m_Light)
			GetGame().ObjectDelete(m_Light);

		if (m_Sound)
			SoundStop();
		#endif
	}

	override void EEInit()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.EEInit();

		m_AnonmalyState = ExpansionAnomalyState.IDLE;
		m_VisualState = ExpansionAnomalyState.NONE;

		InitAnomaly();
	}

	protected void InitAnomaly()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		#ifdef SERVER
		InitAnomalyServer();
		#endif
	}

	protected void InitAnomalyServer()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		//! Spawn anomaly trigger on anomaly position.
		CreateAnomalyTrigger();

		//! Remove grass
		Object cc_object = GetGame().CreateObjectEx(OBJECT_CLUTTER_CUTTER , GetWorldPosition(), ECE_PLACE_ON_SURFACE);
		cc_object.SetOrientation(GetOrientation());
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DestroyClutterCutter, 200, false, cc_object);

		//! Spawn the core item into the anomaly.
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SpawnCoreItem, 500, false);

		SetSynchDirty();
	}

	protected void DestroyClutterCutter(Object clutter_cutter)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		GetGame().ObjectDelete(clutter_cutter);

		#ifdef DIAG
		#ifdef EXPANSIONMODNAVIGATION
		ExpansionMarkerModule markerModule;
		CF_Modules<ExpansionMarkerModule>.Get(markerModule);
		if (markerModule)
			markerModule.CreateServerMarker(GetType(), "Anomaly", Vector(GetPosition()[0], GetPosition()[1] + 1.0, GetPosition()[2]), ARGB(255, 235, 59, 90), true);
		#endif
		#endif
	}

	protected bool PlayParticle(out Particle particle, int particle_type)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

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
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (particle && GetGame() && (!GetGame().IsDedicatedServer()))
		{
			particle.Stop();
			particle = null;

			return true;
		}

		return false;
	}

	void SetAnomalyState(ExpansionAnomalyState state)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::SetAnomalyState - Set anomaly state: " + typename.EnumToString(ExpansionAnomalyState, state));

		m_PrevAnonmalyState = m_AnonmalyState;
		m_AnonmalyState = state;

		SetSynchDirty();
	}

	//! @note: Anomaly trigger event called by ExpansionAnomalyTriggerBase::OnEnterAnomalyServer when ever the anomaly has
	//! no core item anymore and a player is triggering the anomaly trigger.
	void OnAnomalyZoneEnter()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		SetAnomalyState(ExpansionAnomalyState.ACTIVATED);
		ExDebugPrint("::OnAnomalyZoneEnter - Anomaly state: " + typename.EnumToString(ExpansionAnomalyState, m_AnonmalyState));
	}

	protected void SetVisualState(ExpansionAnomalyState state)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::SetVisualState - Anomaly state: " + typename.EnumToString(ExpansionAnomalyState, state));

		m_VisualState = state;
	}

	//! @note: Spawns given loot item inside the core attachment slot of the anomaly.
	protected void SpawnCoreItem()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		EntityAI item = ExpansionItemSpawnHelper.SpawnInInventory(GetAmomalyCoreName(), this);
		m_AnomalyCore = Expansion_AnomalyCore_Base.Cast(item);
		if (!m_AnomalyCore)
		{
			GetGame().ObjectDelete(item);
			return;
		}

		ExDebugPrint("::SpawnCoreItem - Spawned amomaly core item: " + m_AnomalyCore.GetType() + " | Position: " + GetPosition().ToString());
	}

	Expansion_AnomalyCore_Base GetAnomalyCore()
	{
		return m_AnomalyCore;
	}

	protected void SoundIdleStart()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		PlaySoundSetLoop(m_Sound, SOUND_IDLE, 1.0, 1.0);
	}

	protected void SoundStop()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		StopSoundSet(m_Sound);
	}

	protected void SoundActivatedStart()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		PlaySoundSet(m_SoundActivated, SOUND_ACTIVATED, 1.0, 1.0);
	}

	protected void SoundCoreStart()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		PlaySoundSet(m_SoundActivated, SOUND_ACTIVATED, 1.0, 1.0);
	}

	protected void ParticleIdleStop()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		StopParticle(m_ParticleIdle);
	}

	protected void ParticleActivatedStop()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		StopParticle(m_ParticleActivated);
	}

	protected void UpdateVisualState(ExpansionAnomalyState state)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::UpdateVisualState - Anomaly state is: " + typename.EnumToString(ExpansionAnomalyState, state));

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Call(UpdateAnomalyVFX_Deferred, state);
	}

	//! @note: This method updates the anomaly visual effects (VFX) in a deferred manner based on the provided `state`.
	protected void UpdateAnomalyVFX_Deferred(ExpansionAnomalyState state)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::UpdateAnomalyVFX_Deferred - Anomaly state: " + typename.EnumToString(ExpansionAnomalyState, state) + " | Previous anomaly state: " + typename.EnumToString(ExpansionAnomalyState, m_PrevAnonmalyState));
		
		//! Create anomaly light
		if (!m_Light)
		{
			m_Light = ExpansionAnomalyLightBase.Cast(ScriptedLightBase.CreateLight(GetAnomalyLight(), GetPosition() + "0 1 0"));
			m_Light.AttachOnObject(this, "0 1 0");
		}

		//! Stop current particle effects
		ParticleIdleStop();
		ParticleActivatedStop();

		if (state == ExpansionAnomalyState.IDLE)
		{
		    //! Generate a random delay value between 0.0 and 2.0 seconds
		    m_IdleParticleDelay = Math.RandomFloat(0.0, 2.0);
		}

		switch (state)
		{
			case ExpansionAnomalyState.IDLE:
			{
				if (m_PrevAnonmalyState == ExpansionAnomalyState.NOCORE)
				{
					PlayParticle(m_ParticleActivated, GetAnomalyActivatedParticle());
					SoundActivatedStart();
				}

				 //! Delay the idle particle creation by the randomly generated delay value
			    if (m_DelayVFX && m_IdleParticleDelay > 0.0)
			    {
					m_DelayVFX = false;
			        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CreateIdleParticle, m_IdleParticleDelay * 1000, false, state);
			    }
			    else
			    {
			       	CreateIdleParticle(state);
			    }

				//! Reset the delay value for the next time the idle state is entered and we want to the delay the idle
			    m_IdleParticleDelay = 0.0;
			}
			break;
			case ExpansionAnomalyState.ACTIVATED:
			{
				//! Create activated VFX particle
				PlayParticle(m_ParticleActivated, GetAnomalyActivatedParticle());
				SoundActivatedStart();
				SetVisualState(state);
			}
			break;
			case ExpansionAnomalyState.NOCORE:
			{
				//! Play activated VFX and sound when prevoius state was IDLE as that means the
				//! anomaly core item status has changed when this case is called and we only want to trigger the activation VFX and sound here when that is the case.
				//! We dont want to call the activated VFX when the player
				if (m_PrevAnonmalyState != ExpansionAnomalyState.NOCORE)
				{
					PlayParticle(m_ParticleActivated, GetAnomalyActivatedParticle());
					SoundActivatedStart();
				}

				//! Create idle VFX particle
				PlayParticle(m_ParticleIdle, GetAnomalyIdleParticle());

				//! Remove core emitor form core particle
				if (m_ParticleIdle)
				{
					TurnOffCoreEmitor();

					if (!ExpansionNamalskModule.GetModuleInstance().IsEVRStormActive())
					{
						TurnOffUnstableEmitor();
					}
					else
					{
						TurnOnUnstableEmitor();
					}
				}
				
				SetVisualState(state);
			}
			break;
			case ExpansionAnomalyState.UNSTABLE:
			{
				if (m_PrevAnonmalyState != ExpansionAnomalyState.UNSTABLE)
				{
					PlayParticle(m_ParticleActivated, GetAnomalyActivatedParticle());
					SoundActivatedStart();
				}

				//! Create idle VFX particle
				PlayParticle(m_ParticleIdle, GetAnomalyIdleParticle());

				//! Remove core emitor form core particle
				if (m_ParticleIdle)
					TurnOnUnstableEmitor();

				SetVisualState(state);
			}
			break;
			case ExpansionAnomalyState.UNSTABLENOCORE:
			{
				if (m_PrevAnonmalyState != ExpansionAnomalyState.UNSTABLENOCORE)
				{
					PlayParticle(m_ParticleActivated, GetAnomalyActivatedParticle());
					SoundActivatedStart();
				}

				//! Create idle VFX particle
				PlayParticle(m_ParticleIdle, GetAnomalyIdleParticle());

				//! Remove core emitor form core particle
				if (m_ParticleIdle)
				{
					TurnOnUnstableEmitor();
					TurnOffCoreEmitor();
				}
				
				SetVisualState(state);
			}
			break;
		}
	}

	protected void CreateIdleParticle(ExpansionAnomalyState state)
	{
		//! Create idle VFX particle
		PlayParticle(m_ParticleIdle, GetAnomalyIdleParticle());

		if (m_ParticleIdle && !ExpansionNamalskModule.GetModuleInstance().IsEVRStormActive())
			TurnOffUnstableEmitor();

		//! Create idle sound
		if (!m_Sound)
			SoundIdleStart();

		SetVisualState(state);
	}

	protected void TurnOffCoreEmitor()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!m_ParticleIdle)
		{
			Error("::TurnOffCoreEmitor - Could not get idle particle!");
			return;
		}

		m_ParticleIdle.SetParameter(2, EmitorParam.LIFETIME, 0);
		m_ParticleIdle.SetParameter(2, EmitorParam.REPEAT, 0);
		m_ParticleIdle.SetParameter(2, EmitorParam.SIZE, 0);
	}

	protected void TurnOnUnstableEmitor()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!m_ParticleIdle)
		{
			Error("::TurnOnUnstableEmitor - Could not get idle particle!");
			return;
		}

		m_ParticleIdle.SetParameter(0, EmitorParam.LIFETIME, 1);
		m_ParticleIdle.SetParameter(0, EmitorParam.REPEAT, 1);

		if (m_AnomalyCore)
		{
			m_ParticleIdle.SetParameter(1, EmitorParam.LIFETIME, 1);
			m_ParticleIdle.SetParameter(1, EmitorParam.REPEAT, 1);
		}
		else
		{
			m_ParticleIdle.SetParameter(1, EmitorParam.LIFETIME, 0);
			m_ParticleIdle.SetParameter(1, EmitorParam.REPEAT, 0);
		}
	}

	protected void TurnOffUnstableEmitor()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!m_ParticleIdle)
		{
			Error("::TurnOnUnstableEmitor - Could not get idle particle!");
			return;
		}

		m_ParticleIdle.SetParameter(0, EmitorParam.LIFETIME, 0);
		m_ParticleIdle.SetParameter(0, EmitorParam.REPEAT, 0);

		m_ParticleIdle.SetParameter(1, EmitorParam.LIFETIME, 0);
		m_ParticleIdle.SetParameter(1, EmitorParam.REPEAT, 0);
	}

	protected void CreateAnomalyTrigger()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		vector pos = GetPosition();
		Class.CastTo(m_AnomalyTrigger, GetGame().CreateObjectEx(GetAnomalyTriggerName(), pos, ECE_NONE));
		m_AnomalyTrigger.SetPosition(pos);
		m_AnomalyTrigger.SetTriggerRadius(TRIGGER_RADIUS);
		m_AnomalyTrigger.SetActive(true);
		m_AnomalyTrigger.SetAnomaly(this);
	}

	//! Drops all items in the anomaly cargo or that are attached to the anomaly.
	void DropAnormalyItems()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		//! @note: MiscGameplayFunctions.Expansion_HasAnyCargo does not return true when the anomaly has a attached core in its core slot so we check for the core entity also here.
		if (MiscGameplayFunctions.Expansion_HasAnyCargo(this) || m_AnomalyCore)
		{
			//! @note: Only call this if the entity has rly any cargo/attachment or the server crashes lol
			MiscGameplayFunctions.ThrowAllItemsInInventory(this, ThrowEntityFlags.SPLIT);
		}

		ExDebugPrint("::DropAnormalyItems - Check if loot should spawn. Loot spawn type: " + typename.EnumToString(ExpansionAnomalyLootSpawnType, m_LootSpawnType) + " | Loot config: " + m_LootConfig.ToString() + " | Loot item min: " + m_LootItemsMin + " | Loot item max:" + m_LootItemsMax + " | Is loot spawned: " + IsLootSpawned());
		
		if (m_LootSpawnType == ExpansionAnomalyLootSpawnType.DYNAMIC && m_LootConfig && m_LootItemsMin > 0 && m_LootItemsMax && !IsLootSpawned())
		{
			ExDebugPrint("::DropAnormalyItems - Spawn configured loot: " + m_LootConfig.ToString());
			SpawnLoot();
		}
	}

	//! Sets the anomaly state to unstable and synchronises it to clients so the VFX can change if needed.
	void SetAnomalyUnstable()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (m_AnomalyCore)
		{
			SetAnomalyState(ExpansionAnomalyState.UNSTABLE);

			//! @note: Lock the anomaly core slot when anomaly gets set to unstable.
			int slot_id_core = InventorySlots.GetSlotIdFromString("Att_ExpansionAnomalyCore");
			GetInventory().SetSlotLock(slot_id_core, true);
		}
		else
		{
			SetAnomalyState(ExpansionAnomalyState.UNSTABLENOCORE);
		}
	}

	//! Sets the anomaly state to stable and synchronises it to clients so the VFX can change if needed.
	void SetAnomalyStable()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (m_AnomalyCore)
		{
			SetAnomalyState(ExpansionAnomalyState.IDLE);

			//! @note: Unlock the anomaly core slot when anomaly gets set to unstable.
			int slot_id_core = InventorySlots.GetSlotIdFromString("Att_ExpansionAnomalyCore");
			GetInventory().SetSlotLock(slot_id_core, false);
		}
		else
		{
			SetAnomalyState(ExpansionAnomalyState.NOCORE);
		}
	}

	void SetAnomalyExplosion(int explosionTime)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::SetAnomalyExplosion - Explosion time: " + explosionTime);

		if (MiscGameplayFunctions.Expansion_HasAnyCargo(this) || m_AnomalyCore || m_LootConfig && m_LootItemsMin > 0 && m_LootItemsMax)
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DropAnormalyItems_Deferred, explosionTime);
	}

	protected void DropAnormalyItems_Deferred()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		DropAnormalyItems();
	}

	void SetLoot(array <ref ExpansionLoot> lootConfig, int itemsMin, int itemsMax, ExpansionAnomalyLootSpawnType lootType)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (!m_LootConfig)
			m_LootConfig = new array <ref ExpansionLoot>;
		
		foreach (ExpansionLoot config: lootConfig)
		{
			m_LootConfig.Insert(config);
		}
		
		m_LootItemsMin = itemsMin;
		m_LootItemsMax = itemsMax;
		m_LootSpawnType = lootType;
		ExDebugPrint("::SetLoot - Loot config: " + m_LootConfig.ToString() + " | Min: " + m_LootItemsMin + " | Max: " + m_LootItemsMax + " | Loot type: " + typename.EnumToString(ExpansionAnomalyLootSpawnType, m_LootSpawnType));
	}

	void SpawnLoot()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SpawnLootItems, 200);
	}

	//! @note: Spawn given loot items around the anomaly
	protected void SpawnLootItems()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		int randomLoot = Math.RandomInt(m_LootItemsMin, m_LootItemsMax);
		ExDebugPrint("::SpawnLootItems - Spawn loot items from loot config: " + m_LootConfig.ToString() + " | Amount: " + randomLoot);

		ExpansionLootSpawner.SpawnLoot(this, m_LootConfig, randomLoot, m_LootItems);

		#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		foreach (EntityAI lootItem: m_LootItems)
		{
			ExDebugPrint("::SpawnLootItems - Spawned loot item from loot config: " + lootItem.GetType() + " | Position: " + lootItem.GetPosition());
		}
		#endif
	}

	bool HasAnomalyCore()
	{
		return (m_AnomalyCore != null);
	}

	ExpansionAnomalyState GetAnomalyState()
	{
		return m_AnonmalyState;
	}

	ExpansionAnomalyState GetPreviousAnomalyState()
	{
		return m_PrevAnonmalyState;
	}

	ExpansionAnomalyState GetVisualState()
	{
		return m_VisualState;
	}

	ExpansionAnomalyLootSpawnType GetLootType()
	{
		return m_LootSpawnType;
	}
	
	ExpansionAnomalyTriggerBase GetAnomalyTrigger()
	{
		return m_AnomalyTrigger;
	}

	bool HasLoot()
	{
		return (m_LootConfig && m_LootItemsMin > 0 && m_LootItemsMax > 0);
	}

	bool IsLootSpawned()
	{
		return (m_LootItems && m_LootItems.Count() > 0);
	}

	//! @note: Override this is your anomaly class and set the particle ID for the particle that should get played when the anomaly is in idle or without core.
	//! Patricle need to be registerded in the ParticleList class.
	int GetAnomalyIdleParticle()
	{
		return -1;
	}

	//! @note: Override this is your anomaly class and set the particle ID for the particle that should get played when the anomaly is activated/triggered.
	//! Patricle need to be registerded in the ParticleList class.
	int GetAnomalyActivatedParticle()
	{
		return -1;
	}

	typename GetAnomalyLight()
	{
		return ExpansionAnomalyLightBase;
	}

	string GetAmomalyCoreName()
	{
		return "";
	}

	string GetAnomalyTriggerName()
	{
		return "";
	}
	
	void IncreaseEVRStormsCount()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_SurviviedEVRStorms = m_SurviviedEVRStorms + 1;

		ExDebugPrint("::IncreaseEVRStormCount - Count: " + m_SurviviedEVRStorms);
	}

	int SurviviedEVRStormsCount()
	{
		return m_SurviviedEVRStorms;
	}
	
	//! @note: The following commented methods only work when the entity has the "EntityFlags.TRIGGER" flag set with "SetFlags(EntityFlags.TRIGGER, false);" 
	//! and when the entity has set the event flags "EntityEvent.CONTACT" & "EntityEvent.TOUCH" via "SetEventMask(EntityEvent.CONTACT | EntityEvent.TOUCH);".
	//! Otherwise these events will not work or fire!
	/*override void EOnTouch(IEntity other, int extra)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::EOnTouch - Entity: " + other.ToString());

		if (GetGame().IsServer())
		{
			ContactEvent(other, GetPosition());
		}
	}

	override void EOnContact(IEntity other, Contact extra)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::EOnContact - Entity: " + other.ToString());

		if (GetGame().IsServer())
		{
			ContactEvent(other, extra.Position);
		}
	}

	void ContactEvent(IEntity other, vector position)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::ContactEvent - Entity: " + other.ToString() + " | Position: " + position);

		if (GetGame().IsServer() && !m_ContactEventProcessing)
		{
			m_ContactEventProcessing = true;
			//MiscGameplayFunctions.ThrowAllItemsInInventory(this, ThrowEntityFlags.NONE);
			//CheckForDestroy();
			m_ContactEventProcessing = false;
		}
	}*/

	override void EEItemAttached(EntityAI item, string slot_name)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::EEItemAttached - Item: " + item.ToString() + " | Slot:" + slot_name);
		
		RefreshAnomalyCoreState(item, true);
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::EEItemDetached - Item: " + item.ToString() + " | Slot:" + slot_name);

		RefreshAnomalyCoreState(item, false);
	}

	protected void RefreshAnomalyCoreState(EntityAI item, bool state)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::RefreshAnomalyCoreState - Item: " + item.ToString() + " | State:" + state);

		bool evrStormActive = ExpansionNamalskModule.GetModuleInstance().IsEVRStormActive();
		Expansion_AnomalyCore_Base anomalyCore = Expansion_AnomalyCore_Base.Cast(item);
		if (anomalyCore)
		{
			if (state)
			{
				m_AnomalyCore = anomalyCore;
				if (GetGame().IsServer())
				{
					if (evrStormActive)
					{
						SetAnomalyState(ExpansionAnomalyState.UNSTABLE);

						//! @note: Lock the anomaly core slot when core gets attached and there is a EVR storm active.
						int slot_id_core = InventorySlots.GetSlotIdFromString("Att_ExpansionAnomalyCore");
						GetInventory().SetSlotLock(slot_id_core, true);
					}
					else
					{
						SetAnomalyState(ExpansionAnomalyState.IDLE);
					}
				}
			}
			else
			{
				m_AnomalyCore = null;
				if (GetGame().IsServer())
				{
					if (evrStormActive)
					{
						SetAnomalyState(ExpansionAnomalyState.UNSTABLENOCORE);
					}
					else
					{
						SetAnomalyState(ExpansionAnomalyState.NOCORE);
					}
				}
			}
		}
	}

	//! @note: Synchronizes variables and updates visual state of the particle depending on the anomaly state.
	override void OnVariablesSynchronized()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::OnVariablesSynchronized - Current anomaly visual state: " + typename.EnumToString(ExpansionAnomalyState, m_VisualState) + " | Anomaly state: " + typename.EnumToString(ExpansionAnomalyState, m_AnonmalyState));

		if (m_VisualState != m_AnonmalyState)
			UpdateVisualState(m_AnonmalyState);
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
		return false;
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

	override void AfterStoreLoad()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		GetGame().ObjectDelete(this);
	}

	protected void ExDebugPrint(string text)
	{
		#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
		#endif
	}
};