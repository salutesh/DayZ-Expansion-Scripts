/**
 * Expansion_Bunker_Generator.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! @note: Can only be used when the ZStuff mod by Starlv is loaded: https://steamcommunity.com/sharedfiles/filedetails/?id=2384407442&searchtext=ZStuff
class Expansion_Bunker_Generator: House
{
	static ref CF_DoublyLinkedNodes_WeakRef<Expansion_Bunker_Generator> s_Expansion_AllBunkerGenerators = new CF_DoublyLinkedNodes_WeakRef<Expansion_Bunker_Generator>();
	ref CF_DoublyLinkedNode_WeakRef<Expansion_Bunker_Generator> m_Expansion_BunkerGeneratorNode = new CF_DoublyLinkedNode_WeakRef<Expansion_Bunker_Generator>(this);
	
	static const string	START_SOUND = "powerGeneratorTurnOn_SoundSet";
	static const string	LOOP_SOUND = "powerGeneratorLoop_SoundSet";
	static const string	STOP_SOUND = "powerGeneratorTurnOff_SoundSet";
	static const string KEY_ATTACH_SOUND = "carbattery_attach_SoundSet";
	static const string KEY_DETACH_SOUND = "carbattery_detach_SoundSet";

	protected EffectSound m_EngineLoop;
	protected EffectSound m_EngineStart;
	protected EffectSound m_EngineStop;
	ref Timer m_SoundLoopStartTimer;
	ref protected Effect m_Smoke;

	protected ref UniversalTemperatureSource m_UTSource;
	protected ref UniversalTemperatureSourceSettings m_UTSSettings;
	protected ref UniversalTemperatureSourceLambdaEngine m_UTSLEngine;

#ifdef EXPANSIONMODTELEPORTER
	protected Expansion_Teleporter_Big m_LinkedTeleporter;
#endif
	
	protected int m_LastActivationTime;
	protected bool m_IsActive;

	void Expansion_Bunker_Generator()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		m_Expansion_BunkerGeneratorNode = s_Expansion_AllBunkerGenerators.Add(this);
		
		RegisterNetSyncVariableBool("m_IsActive");
	}

	void ~Expansion_Bunker_Generator()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (s_Expansion_AllBunkerGenerators)
			s_Expansion_AllBunkerGenerators.Remove(m_Expansion_BunkerGeneratorNode);
	}

	override void EEInit()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.EEInit();

		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
 			m_UTSSettings = new UniversalTemperatureSourceSettings();
			m_UTSSettings.m_ManualUpdate = true;
			m_UTSSettings.m_TemperatureMin = 0;
			m_UTSSettings.m_TemperatureMax = 80;
			m_UTSSettings.m_RangeFull = 1;
			m_UTSSettings.m_RangeMax = 2.5;
			m_UTSSettings.m_TemperatureCap = 8;

			m_UTSLEngine = new UniversalTemperatureSourceLambdaEngine();
			m_UTSource = new UniversalTemperatureSource(this, m_UTSSettings, m_UTSLEngine);
		}

		InitGenerator();
	}

	protected void InitGenerator()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!GetGame().IsDedicatedServer())
		{
			InitGeneratorClient();
		}

		if (GetGame().IsServer())
		{
			InitGeneratorServer();
		}
	}

#ifdef EXPANSIONMODTELEPORTER
	void SetLinkedTeleporter(Expansion_Teleporter_Big teleporter)
	{
		m_LinkedTeleporter = teleporter;
	}
#endif

	void StartLoopSound()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			if (GetCompEM().IsWorking())
			{
				PlaySoundSetLoop(m_EngineLoop, LOOP_SOUND, 0, 0);

				//! Particle
				vector local_pos = "0.5 2.0 -1.3";
				vector local_ori = "0 0 0";
				m_Smoke = new EffGeneratorSmoke();
				SEffectManager.PlayOnObject(m_Smoke, this, local_pos, local_ori);
			}
		}
	}

	override void OnWorkStart()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			if (IsInitialized())
			{
				PlaySoundSet(m_EngineStart, START_SOUND, 0, 0);
			}

			if (!m_SoundLoopStartTimer)
			{
				m_SoundLoopStartTimer = new Timer(CALL_CATEGORY_SYSTEM);
			}

			if (!m_SoundLoopStartTimer.IsRunning()) //! Makes sure the timer is NOT running already
			{
				m_SoundLoopStartTimer.Run(1.5, this, "StartLoopSound", NULL, false);
			}
		}

		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			m_UTSource.SetDefferedActive(true, 20.0);

		#ifdef EXPANSIONMODTELEPORTER
			if (m_LinkedTeleporter)
				m_LinkedTeleporter.SetActive(true);
		#endif
			
			SetLastActivationTime();
			m_IsActive = true;
			SetSynchDirty();
		}
	}

	override void OnWork(float consumed_energy)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			m_UTSource.Update(m_UTSSettings, m_UTSLEngine);
		}

		if (GetGame().IsServer())
		{
			SetSynchDirty();
		}
	}

	override void OnWorkStop()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			//! Sound
			PlaySoundSet(m_EngineStop, STOP_SOUND, 0, 0);
			StopSoundSet(m_EngineLoop);

			//! Particle
			SEffectManager.DestroyEffect(m_Smoke);
		}

		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			m_UTSource.SetDefferedActive(false, 20.0);

		#ifdef EXPANSIONMODTELEPORTER
			if (m_LinkedTeleporter)
				m_LinkedTeleporter.SetActive(false);
		#endif
			
			m_IsActive = false;
			SetSynchDirty();
		}
	}

	override void EEItemAttached(EntityAI item, string slot_name)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.EEItemAttached(item, slot_name);

		Expansion_Bunker_Generator_Key key;
		if (Class.CastTo(key, item))
		{
			GetCompEM().SetEnergy(500);
			GetCompEM().SwitchOn();

			if (GetGame().IsServer())
			{
				int slot_id_key = InventorySlots.GetSlotIdFromString("Att_ExpansionGeneratorKey");
				GetInventory().SetSlotLock(slot_id_key, true);
			}
		}

		#ifndef SERVER
		EffectSound sound = SEffectManager.PlaySound(KEY_ATTACH_SOUND, GetPosition());
		sound.SetAutodestroy( true );
		#endif
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.EEItemDetached(item, slot_name);

		Expansion_Bunker_Generator_Key key;
		if (Class.CastTo(key, item))
		{
			GetCompEM().SwitchOff();

			if (GetGame().IsServer())
			{
				int slot_id_key = InventorySlots.GetSlotIdFromString("Att_ExpansionGeneratorKey");
				GetInventory().SetSlotLock(slot_id_key, false);
			}
		}

		#ifndef SERVER
		EffectSound sound = SEffectManager.PlaySound(KEY_DETACH_SOUND, GetPosition());
		sound.SetAutodestroy(true);
		#endif
	}

	bool CanInsertKey()
	{
		int slot_id_key = InventorySlots.GetSlotIdFromString("Att_ExpansionGeneratorKey");
		Expansion_Bunker_Generator_Key key;
		if (Class.CastTo(key, GetInventory().FindAttachment(slot_id_key)))
			return false;

		return true;
	}
	
	bool HasKey()
	{
		int slot_id_key = InventorySlots.GetSlotIdFromString("Att_ExpansionGeneratorKey");
		Expansion_Bunker_Generator_Key key;
		if (Class.CastTo(key, GetInventory().FindAttachment(slot_id_key)))
			return true;

		return false;
	}
	
	void SetLastActivationTime()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		m_LastActivationTime = CF_Date.Now(true).GetTimestamp();
	}
	
	int GetLastActivationTime()
	{
		return m_LastActivationTime;
	}
	
	bool IsActive()
	{
		return m_IsActive;
	}
	
	override void SetActions()
	{
		super.SetActions();

		AddAction(ExpansionActionRemoveGeneratorKey);
	}

	protected void InitGeneratorClient()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	}

	protected void InitGeneratorServer()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	}

	override void OnVariablesSynchronized()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.OnVariablesSynchronized();
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

		super.AfterStoreLoad();

		GetGame().ObjectDelete(this);
	}

	protected void DebugTrace(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Start(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};

class Expansion_Bunker_Generator_Key extends ItemBase
{
	override void SetActions()
	{
		super.SetActions();

		AddAction(ExpansionActionInsertGeneratorKey);
	}

	void OnKeyUsed()
	{
		AddHealthLevel(1);
	}
};