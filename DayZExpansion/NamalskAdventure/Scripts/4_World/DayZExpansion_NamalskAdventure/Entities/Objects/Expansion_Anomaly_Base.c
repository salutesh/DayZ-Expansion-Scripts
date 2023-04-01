/**
 * Expansion_Anomaly_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
	NOCORE = 4,
	UNSTABLE = 8
};

class ExpansionAnomalyLight extends PointLightBase
{
	void ExpansionAnomalyLight()
	{
		SetVisibleDuringDaylight(true);
		SetRadiusTo(4.0);
		SetBrightnessTo(0.5);
		SetCastShadow(false);
		SetFadeOutTime(1.0);
		SetDiffuseColor(1.0, 1.00, 1.0);
		SetAmbientColor(1.0, 1.00, 1.0);
		SetFlareVisible(false);
		SetFlickerAmplitude(1.0);
		SetFlickerSpeed(2.0);
		SetDancingShadowsMovementSpeed(0.4);
		SetDancingShadowsAmplitude(0.5);
	}
};

class Expansion_Anomaly_Base: ItemBase
{
	protected const string SOUND_IDLE = "Expansion_AnomalyRumble_Soundset";
	protected const string SOUND_ACTIVATED = "Expansion_AnomalyWindBlowActivated_Soundset";
	protected const int TRIGGER_RADIUS = 2;
	protected const string OBJECT_CLUTTER_CUTTER = "ClutterCutterFireplace";
	
	//! Item Spawning upon anomaly creation, the 4 arrays bellow have to have the same amount of elements
	//protected const ref array<string> SPAWN_ITEM_TYPE = {"Grenade_ChemGas"};//! Item classnames
	protected const ref array<int> SPAWN_ITEM_COUNT = {Math.RandomIntInclusive(2,5)}; //! How many of each type (randomized).
	protected const ref array<float> SPAWN_ITEM_RAD_MIN = {3}; //! Min distance the item will be spawned from the area position(epicenter).
	protected const ref array<float> SPAWN_ITEM_RAD_MAX = {5}; //! Max distance the item will be spawned from the area position(epicenter).

	protected vector m_Position;

	protected bool m_ContactEventProcessing;

	protected ExpansionAnomalyLight m_Light;
	protected EffectSound m_Sound;
	protected EffectSound m_SoundActivated;
	protected Particle m_ParticleIdle;
	protected Particle m_ParticleActivated;

	protected int m_AnonmalyState;
	protected int m_PrevAnonmalyState;
	protected int m_VisualState;

	protected ExpansionAnomalyTriggerBase m_AnomalyTrigger;
	protected Expansion_AnomalyCore_Base m_AnomalyCore;

	void Expansion_Anomaly_Base()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		SetEventMask(EntityEvent.CONTACT | EntityEvent.TOUCH/*| EntityEvent.ENTER | EntityEvent.LEAVE*/);

		RegisterNetSyncVariableInt("m_AnonmalyState", 0, 8);
		RegisterNetSyncVariableInt("m_PrevAnonmalyState", 0, 8);
		RegisterNetSyncVariableInt("m_VisualState", 0, 8);
	}

	void ~Expansion_Anomaly_Base()
	{
		if (m_ParticleIdle)
			ParticleIdleStop();
		
		if (m_ParticleActivated)
			ParticleActivatedStop();

		if (m_Light)
			GetGame().ObjectDelete(m_Light);

		if (m_Sound)
			SoundStop();

		if (m_AnomalyTrigger)
			GetGame().ObjectDelete(m_AnomalyTrigger);
	}

	override void EEInit()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.EEInit();

		InitAnomaly();
		
		m_PrevAnonmalyState = ExpansionAnomalyState.IDLE;
		m_AnonmalyState = ExpansionAnomalyState.IDLE;
		m_VisualState = ExpansionAnomalyState.NONE;
	}

	protected void InitAnomaly()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_Position = GetWorldPosition();

		if (!GetGame().IsDedicatedServer())
		{
			InitAnomalyClient();
		}

		if (GetGame().IsServer())
		{
			InitAnomalyServer();
		}
	}

	protected void InitAnomalyClient()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	}

	protected void InitAnomalyServer()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		//! Remove grass
		Object cc_object = GetGame().CreateObjectEx(OBJECT_CLUTTER_CUTTER , m_Position, ECE_PLACE_ON_SURFACE);
		cc_object.SetOrientation ( GetOrientation() );
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DestroyClutterCutter, 200, false, cc_object);
		
		//! Set inital anomaly state
		SetAnomalyState(ExpansionAnomalyState.IDLE);
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

	/*override bool CanReceiveItemIntoCargo( EntityAI item )
	{
		return super.CanReceiveItemIntoCargo( item );
	}

	override bool CanLoadItemIntoCargo( EntityAI item )
	{
		if (!super.CanLoadItemIntoCargo( item ))
			return false;

		if ( GetHierarchyParent() )
			return false;

		return true;
	}*/

	override void EOnTouch(IEntity other, int extra)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::EOnTouch - Entity: " + other.ToString());

		if (GetGame().IsClient())
			return;

		ContactEvent(other, GetPosition());
	}

	override void EOnContact(IEntity other, Contact extra)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::EOnContact - Entity: " + other.ToString());

		if (!GetGame().IsDedicatedServer())
			return;

		ContactEvent(other, extra.Position);
	}

	void ContactEvent(IEntity other, vector position)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::ContactEvent - Entity: " + other.ToString() + " | Position: " + position);

		if (GetGame().IsServer() && !m_ContactEventProcessing)
		{
			m_ContactEventProcessing = true;
			//MiscGameplayFunctions.ThrowAllItemsInInventory(this, 0);
			//CheckForDestroy();
			m_ContactEventProcessing = false;
		}
	}
	
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::EEItemAttached - Item: " + item.ToString() + " | Slot:" + slot_name);
		
		RefreshAnomalyCoreState(item, true);
	}
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::EEItemDetached - Item: " + item.ToString() + " | Slot:" + slot_name);
		
		RefreshAnomalyCoreState(item, false);
	}
	
	protected void RefreshAnomalyCoreState(EntityAI item, bool state)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::RefreshAnomalyCoreState - Item: " + item.ToString() + " | State:" + state);

		if (Expansion_AnomalyCore_Base.Cast(item))
		{
			if (state)
			{
				m_AnomalyCore = Expansion_AnomalyCore_Base.Cast(item);
				AnomalyCoreAdded();
			}
			else
			{
				m_AnomalyCore = null;
				AnomalyCoreRemoved();
			}
		}
	}

	protected void DestroyClutterCutter(Object clutter_cutter)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		GetGame().ObjectDelete(clutter_cutter);
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

	//! Returns true if particle stopped, false if not
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

	void SetAnomalyState(ExpansionAnomalyState state)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_PrevAnonmalyState = m_AnonmalyState;
		m_AnonmalyState = state;
		
		SetSynchDirty();
		
		DebugTrace("::SetAnomalyState - Set anomaly state [Previous: " + typename.EnumToString(ExpansionAnomalyState, m_PrevAnonmalyState) + " | Now: " + typename.EnumToString(ExpansionAnomalyState, state) + "]");
	}
	
	//! @note: Anomaly trigger event called by ExpansionAnomalyTriggerBase::OnEnterAnomalyServer when ever the anomaly has
	//! no core item anymore and a player is triggering the anomaly trigger.
	void OnAnomalyZoneEnter()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		SetAnomalyState(ExpansionAnomalyState.ACTIVATED);
		DebugTrace("::OnAnomalyZoneEnter - Anomaly state: " + typename.EnumToString(ExpansionAnomalyState, m_AnonmalyState));
	}
	
	void SetVisualState(ExpansionAnomalyState state)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::SetVisualState - Anomaly state: " + typename.EnumToString(ExpansionAnomalyState, state));

		m_VisualState = state;
	}

	//! @note: Spawn given loot items around the anomaly
	protected void SpawnItems(array<string> types)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		foreach (int j, string type: types)
		{
			for (int i = 0; i < SPAWN_ITEM_COUNT[j]; i++)
			{
				vector randomDir2d = vector.RandomDir2D();
				float randomDist = Math.RandomFloatInclusive(SPAWN_ITEM_RAD_MIN[j],SPAWN_ITEM_RAD_MAX[j]);
				vector spawnPos = m_Position + (randomDir2d * randomDist);
				InventoryLocation il = new InventoryLocation;
				vector mat[4];
				Math3D.MatrixIdentity4(mat);
				mat[3] = spawnPos;
				il.SetGround(NULL, mat);
				GetGame().CreateObjectEx(type, il.GetPos(), ECE_PLACE_ON_SURFACE);
			}
		}
	}

	//! @note: Spawns given loot item inside the core attachment slot of the anomaly.
	protected void SpawnCoreItem(string type)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		EntityAI item = GetInventory().CreateInInventory(type);
		m_AnomalyCore = Expansion_AnomalyCore_Base.Cast(item);
		if (!m_AnomalyCore)
		{
			GetGame().ObjectDelete(item);
			return;
		}
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

	override void OnVariablesSynchronized()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.OnVariablesSynchronized();

		if (m_VisualState != m_AnonmalyState)
			UpdateVisualState(m_AnonmalyState);
	}

	protected void UpdateVisualState(ExpansionAnomalyState state)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::UpdateVisualState - Anomaly state: " + typename.EnumToString(ExpansionAnomalyState, state));

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(UpdateAnomalyVFX_Deferred, 0, false, state);
	}

	protected void UpdateAnomalyVFX_Deferred(ExpansionAnomalyState state)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::UpdateAnomalyVFX_Deferred - Anomaly state: " + typename.EnumToString(ExpansionAnomalyState, state) + " | Previous anomaly state: " + typename.EnumToString(ExpansionAnomalyState, m_PrevAnonmalyState));
		
		bool hasCore = true;
		
		//! Create anomaly light
		if (!m_Light)
		{
			m_Light = ExpansionAnomalyLight.Cast(ScriptedLightBase.CreateLight(ExpansionAnomalyLight, GetPosition() + "0 1 0"));
			m_Light.AttachOnObject(this, "0 1 0");
		}
		
		//! Stop current particle effects
		ParticleIdleStop();
		ParticleActivatedStop();
		
		switch (state)
		{
			case ExpansionAnomalyState.IDLE:
			{
				if (m_PrevAnonmalyState == ExpansionAnomalyState.NOCORE && GetAnomalyCore())
				{
					PlayParticle(m_ParticleActivated, GetAnomalyActivatedParticle());
					SoundActivatedStart();
				}
								
				//! Create idle VFX particle
				PlayParticle(m_ParticleIdle, GetAnomalyIdleParticle());

				if (m_ParticleIdle && !ExpansionNamalskModule.GetModuleInstance().IsEVRStormActive())
					TurnOffUnstableEmitor();
				
				//! Create idle sound
				if (!m_Sound)
					SoundIdleStart();

				SetVisualState(state);
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
				if (m_PrevAnonmalyState < ExpansionAnomalyState.ACTIVATED)
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
					TurnOffUnstableEmitor();

					if (ExpansionNamalskModule.GetModuleInstance().IsEVRStormActive())
					{
						if (!m_AnomalyCore)
							hasCore = false;
						
						TurnOnUnstableEmitor(hasCore);
					}
				}

				SetVisualState(state);
			}
			break;
			case ExpansionAnomalyState.UNSTABLE:
			{
				//! Create idle VFX particle
				PlayParticle(m_ParticleIdle, GetAnomalyIdleParticle());
				
				//! Remove core emitor form core particle
				if (!m_AnomalyCore)
				{
					hasCore = false;
					TurnOffCoreEmitor();
				}
				
				if (m_ParticleIdle)
					TurnOnUnstableEmitor(hasCore);

				SetVisualState(state);
			}
			break;
		}
	}
	
	protected void TurnOffCoreEmitor()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (m_ParticleIdle)
		{
			m_ParticleIdle.SetParameter(2, EmitorParam.LIFETIME, 0);
			m_ParticleIdle.SetParameter(2, EmitorParam.REPEAT, 0);
			m_ParticleIdle.SetParameter(2, EmitorParam.SIZE, 0);
		}
	}
	
	protected void TurnOnUnstableEmitor(bool hasCore)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (m_ParticleIdle)
		{
			m_ParticleIdle.SetParameter(0, EmitorParam.LIFETIME, 1);
			m_ParticleIdle.SetParameter(0, EmitorParam.REPEAT, 1);
			
			if (hasCore)
			{
				m_ParticleIdle.SetParameter(1, EmitorParam.LIFETIME, 1);
				m_ParticleIdle.SetParameter(1, EmitorParam.REPEAT, 1);
			}
		}
	}
	
	protected void TurnOffUnstableEmitor()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (m_ParticleIdle)
		{
			m_ParticleIdle.SetParameter(0, EmitorParam.LIFETIME, 0);
			m_ParticleIdle.SetParameter(0, EmitorParam.REPEAT, 0);

			m_ParticleIdle.SetParameter(1, EmitorParam.LIFETIME, 0);
			m_ParticleIdle.SetParameter(1, EmitorParam.REPEAT, 0);
		}
	}

	protected void CreateAnomalyTrigger(string type)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		vector pos = GetPosition();
		Class.CastTo(m_AnomalyTrigger, GetGame().CreateObjectEx(type, pos, ECE_NONE));
		m_AnomalyTrigger.SetPosition(pos);
		m_AnomalyTrigger.SetTriggerRadius(TRIGGER_RADIUS);
		m_AnomalyTrigger.SetActive(true);
		m_AnomalyTrigger.SetAnomaly(this);
	}
	
	/*void OnAnomalyZoneEnter()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::AnomalyCoreRemoved - Anomaly state: " + typename.EnumToString(ExpansionAnomalyState, m_AnonmalyState));
	}*/
	
	void AnomalyCoreRemoved()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		SetAnomalyState(ExpansionAnomalyState.NOCORE);
		
		DebugTrace("::AnomalyCoreRemoved - Anomaly state: " + typename.EnumToString(ExpansionAnomalyState, m_AnonmalyState));
	}
	
	void AnomalyCoreAdded()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		SetAnomalyState(ExpansionAnomalyState.IDLE);
		
		DebugTrace("::AnomalyCoreAdded - Anomaly state: " + typename.EnumToString(ExpansionAnomalyState, m_AnonmalyState));
	}
	
	//! Drops all items in the anomaly cargo or that are attached to the anomaly.
	void DropAnormalyItems()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		Expansion_Anomaly_Base anomaly = Expansion_Anomaly_Base.Cast(this);
		if (anomaly && MiscGameplayFunctions.Expansion_HasAnyCargo(anomaly))
		{
			//! @note: Only call this if the entity has rly any cargo/attachment or the server crashes lol
			//! For some reason this throws also a null pointer even if we only call it 
			MiscGameplayFunctions.ThrowAllItemsInInventory(anomaly, 1);
			SetAnomalyState(ExpansionAnomalyState.NOCORE);
		}
	}
	
	//! Sets the anomaly state to unstable and synchronises it to clients so the VFX can change if needed.
	void SetAnomalyUnstable()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		SetAnomalyState(ExpansionAnomalyState.UNSTABLE);
	}
	
	void SetAnomalyExplosion(int explosionTime)
	{
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DropAnormalyItems_Deferred, explosionTime);
	}
	
	void DropAnormalyItems_Deferred()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		DropAnormalyItems();
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

	protected void DebugTrace(string text)
	{
		EXTrace.Start(EXTrace.NAMALSKADVENTURE, this, text);
	}
};