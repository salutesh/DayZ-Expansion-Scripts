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
	protected int m_TelerporterID;	 //! Unique teleporter id. Used to get and identify the teleporter in the teleporter module.

	void Expansion_Teleporter_Base()
	{
		if (IsMissionHost())
			SetAllowDamage(false);

		m_TelerporterID = -1;
		RegisterNetSyncVariableInt("m_TelerporterID");
	}

	void SetTeleporterID(int id)
	{
		m_TelerporterID = id;
		EXPrint(ToString() + "::SetTeleporterID - ID: " + m_TelerporterID);
	}

	int GetTeleporterID()
	{
		return m_TelerporterID;
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
};

enum ExpansionTeleporterState
{
	OFF = 0,
	ACTIVE = 1,
	LOW_ENERGY = 2,
	NO_ENERGY = 3,
	EVR = 4
};

class Expansion_Teleporter_Big: Expansion_Teleporter_Base
{
	protected Particle m_Particle;
	protected SoundOnVehicle m_Sound;
	protected ExpansionTeleportLight m_Light;
	
	static const string CAR_BATTERY_ATTACH_SOUND = "carbattery_attach_SoundSet";
	static const string CAR_BATTERY_DETACH_SOUND = "carbattery_detach_SoundSet";
	static const string TELEPORTER_IDLE_SOUND = "Expansion_TeleporterIdle_Sound";
	
	protected float m_ChargeEnergyPerSecond;
	protected ExpansionTeleporterState m_State = ExpansionTeleporterState.OFF;

	void Expansion_Teleporter_Big()
	{		
		/*if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			m_Particle = Particle.PlayOnObject(ParticleList.EXPANSION_PARTICLE_TELEPORTER, this);
			m_Light = ExpansionTeleportLight.Cast(ScriptedLightBase.CreateLight(ExpansionTeleportLight, GetPosition() + "0 1 0"));
			m_Light.AttachOnObject(this, "0 1 0");
			PlayIdleSound();
		}*/
		
		RegisterNetSyncVariableInt("m_State");
	}
	
	void ~Expansion_Teleporter_Big()
	{
		if (GetGame() && (GetGame().IsClient() || !GetGame().IsMultiplayer()))
		{
			if (m_Particle)
			{
				m_Particle.StopParticle();
				GetGame().ObjectDelete(m_Particle);
			}
			
			if (m_Sound)
				GetGame().ObjectDelete(m_Sound);
			
			if (m_Light)
				GetGame().ObjectDelete(m_Light);
		}
	}
	
	protected void PlayIdleSound()
	{
		if (m_Sound)
			GetGame().ObjectDelete(m_Sound);

		m_Sound = PlaySoundLoop(TELEPORTER_IDLE_SOUND, 30);
	}
	
	protected void StopIdleSound()
	{
		if (m_Sound)
			GetGame().ObjectDelete(m_Sound);
	}
	
	protected void CreateParticleEffect()
	{
		if (!GetGame().IsServer() || !GetGame().IsMultiplayer()) //! Client side
		{
			m_Particle = Particle.PlayOnObject(ParticleList.EXPANSION_PARTICLE_TELEPORTER, this);
		}
	}
	
	protected void StopParticleEffect()
	{
		if (!GetGame().IsServer() || !GetGame().IsMultiplayer()) //! Client side
		{	
			if (m_Particle)
			{
				m_Particle.Stop();
				GetGame().ObjectDelete(m_Particle);
			}
		}
	}
	
	protected void CreateLight()
	{
		if (!GetGame().IsServer() || !GetGame().IsMultiplayer()) //! Client side
		{
			m_Light = ExpansionTeleportLight.Cast(ScriptedLightBase.CreateLight(ExpansionTeleportLight, GetPosition() + "0 1 0"));
		}
	}
	
	protected void DestroyLight()
	{
		if (!GetGame().IsServer() || !GetGame().IsMultiplayer()) //! Client side
		{
			if (m_Light)
			{
				m_Light.FadeOut();
				GetGame().ObjectDelete(m_Light);
			}
		}
	}
	
	override bool IsInventoryVisible()
	{
		return true;
	}
	
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);

		if (item.IsInherited(CarBattery))
		{
			if (GetCompEM().CanWork())
				GetCompEM().SwitchOn();
		}
		
		if (GetGame().IsServer())
		{
			ExpansionCircuitBoardBase board;
			if (Class.CastTo(board, item))
			{
				int slot_id_board = InventorySlots.GetSlotIdFromString("Att_ExpansionCircuitBoard");
				GetInventory().SetSlotLock(slot_id_board, true);
			}
		}
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		
		if (item.IsInherited(CarBattery))
			GetCompEM().SwitchOff();
		
		if (GetGame().IsServer())
		{
			ExpansionCircuitBoardBase board;
			if (Class.CastTo(board, item))
			{
				int slot_id_board = InventorySlots.GetSlotIdFromString("Att_ExpansionCircuitBoard");
				GetInventory().SetSlotLock(slot_id_board, false);
			}
		}
	}
	
	override void OnWork(float consumed_energy)
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		
		if (!GetGame().IsServer())
		{
			int slot = InventorySlots.GetSlotIdFromString("CarBattery");
			CarBattery carBattery;
			if (Class.CastTo(carBattery, GetInventory().FindAttachment(slot)))
			{
				bool isEVRStormActive = IsEVRStormActive();
				bool isUnderRoof = MiscGameplayFunctions.IsUnderRoof(this);
				
				World world = GetGame().GetWorld();
				
				Weather weather = GetGame().GetWeather();
				float overcast = weather.GetOvercast().GetActual();
				float rain = weather.GetRain().GetActual();
				float fog = weather.GetFog().GetActual();
				float sunshine = ExpansionMath.LinearConversion(0.3, 0.7, overcast, 1.0, 0.0) * Math.Min(1.3 - rain, 1.0) * Math.Min(1.3 - fog, 1.0);
				
				float energy_delta = consumed_energy;
				float max = carBattery.GetCompEM().GetEnergyMax();
				
				//! If no EVR storm, consume energy during cloudy/rainy/foggy weather, at night, or if object is in building
				if (!isEVRStormActive && (isUnderRoof || world.IsNight() || sunshine < 0.3))
				{
					if (!isUnderRoof)
						energy_delta *= 1.0 - sunshine;
				#ifdef DIAG
					EXTrace.Print(EXTrace.TELEPORTER, this, "::OnWork - consuming energy " + energy_delta);
				#endif
					carBattery.GetCompEM().ConsumeEnergy(energy_delta);
				}
				else
				{
					//! Add energy to the battery during daytime if not under roof or during EVR storms
					energy_delta = m_ChargeEnergyPerSecond * (consumed_energy / GetCompEM().GetEnergyUsage());  //! Add charge each second (base value, influenced by EVR and sunshine)
					float health01 = GetHealth01("", "");
					energy_delta *= (0.5 + health01 * 0.5); //! 50% damage causes 75% efficiency
					if (isEVRStormActive)
						energy_delta * 2.0;  //! EVR gives a boost to recharging
					else
						energy_delta *= sunshine;
				#ifdef DIAG
					EXTrace.Print(EXTrace.TELEPORTER, this, "::OnWork - recharging energy " + energy_delta);
				#endif
					carBattery.GetCompEM().AddEnergy(energy_delta);
				}
				
				float energy = carBattery.GetCompEM().GetEnergy();
				float frac = energy / max;
				if (frac <= 0)
					m_State = ExpansionTeleporterState.NO_ENERGY;
				else if (isEVRStormActive)
					m_State = ExpansionTeleporterState.EVR;
				else if (frac >= 0.3)
					m_State = ExpansionTeleporterState.ACTIVE;
				else
					m_State = ExpansionTeleporterState.LOW_ENERGY;
				
				SetSynchDirty();
			}

			UpdateVisuals();
		}
	}
	
	override void OnWorkStop()
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		
		super.OnWorkStop();
		
		if (!GetGame().IsServer())
		{
			m_State = ExpansionTeleporterState.OFF;
			SetSynchDirty();
		}

		UpdateVisuals();
	}
	
	protected void UpdateVisuals()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(UpdateVisuals_Deferred, 0, false, m_State);
	}

	protected void UpdateVisuals_Deferred(ExpansionTeleporterState state)
	{
		switch (state)
		{
			case ExpansionTeleporterState.OFF:
			{
				#ifndef SERVER
				StopIdleSound();
				StopParticleEffect();
				DestroyLight();
				#endif
			}
			break;
			case ExpansionTeleporterState.ACTIVE:
			{
				#ifndef SERVER
				PlayIdleSound();
				CreateParticleEffect();
				CreateLight();
				#endif
			}
			break;
			/*case ExpansionTeleporterState.LOW_ENERGY:
			{
				
			}
			break;
			case ExpansionTeleporterState.NO_ENERGY:
			{
				
			}
			break;
			case ExpansionTeleporterState.EVR:
			{
				
			}
			break;*/
		}
	}
	
	int GetCircuitBoardTier(string typeName)
	{
		int tier = -1;
		typeName.ToLower();
		typeName.Replace("expansioncircuitboard_mk", "");
		tier = typeName.ToInt();
		return tier;
	}

	bool HasCarBattery()
	{
		int slot = InventorySlots.GetSlotIdFromString("CarBattery");

		CarBattery carBattery;
		if (Class.CastTo(carBattery, GetInventory().FindAttachment(slot)))
			return true;

		return false;
	}
};

class ExpansionTeleportLight extends PointLightBase
{
	void ExpansionTeleportLight()
	{
		SetVisibleDuringDaylight(true);
		SetRadiusTo(5);
		SetBrightnessTo(20);
		SetCastShadow(false);
		SetFadeOutTime(3);
		SetDiffuseColor(0.2, 0.5, 1.0);
		SetAmbientColor(0.2, 0.5, 1.0);
		SetFlareVisible(false);
		SetFlickerAmplitude(0.2);
		SetFlickerSpeed(10.0);
		SetDancingShadowsMovementSpeed(0.4);
		SetDancingShadowsAmplitude(0.5);
	}
};