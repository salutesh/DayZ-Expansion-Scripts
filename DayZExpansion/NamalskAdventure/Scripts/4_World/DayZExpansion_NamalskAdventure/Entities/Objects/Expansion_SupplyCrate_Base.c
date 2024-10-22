/**
 * Expansion_SupplyCrate_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionSupplyCrateKeyType
{
	NONE = 0,
	WORLD_T1 = 1,
	WORLD_T2 = 2,
	WORLD_T3 = 3,
	BUNKER = 4
};

class Expansion_SupplyCrate_Base: Container_Base
{
	static ref CF_DoublyLinkedNodes_WeakRef<Expansion_SupplyCrate_Base> s_Expansion_AllSupplyCrates = new CF_DoublyLinkedNodes_WeakRef<Expansion_SupplyCrate_Base>();
	ref CF_DoublyLinkedNode_WeakRef<Expansion_SupplyCrate_Base> m_Expansion_SupplyCrateNode = new CF_DoublyLinkedNode_WeakRef<Expansion_SupplyCrate_Base>(this);
	
	static const string KEY_ATTACH_SOUND = "carbattery_attach_SoundSet";
	static const string KEY_DETTACH_SOUND = "carbattery_detach_SoundSet";
	static const string DECOMPRESS_SOUND = "LEHS_Pressure_Release";
	
	protected EffectSound m_SoundDecompress;
	
	protected ref array<ref ExpansionLoot> m_CrateLoot = new array<ref ExpansionLoot>;
	protected ref map<string, int> m_LootItemsMap = new map<string, int>;
	protected ref array<EntityAI> m_LootItems = new array<EntityAI>;

	protected bool m_IsLocked;
	protected bool m_IsOpened;
	protected bool m_Expansion_CanReceiveItems;
	protected bool m_LootDelay;
	
	protected int m_LastCloseTime;
	protected int m_LastOpenTime;

	protected ExpansionSupplyCrateKeyType m_ValidKeyType;
	
	#ifdef DIAG_DEVELOPER
#ifdef EXPANSIONMODNAVIGATION
	protected ExpansionMarkerData m_ServerMarker;
#endif
#endif

	void Expansion_SupplyCrate_Base()
	{
		m_Expansion_SupplyCrateNode = s_Expansion_AllSupplyCrates.Add(this);
		
		SetEventMask(EntityEvent.INIT);
		RegisterNetSyncVariableBool("m_IsLocked");
		RegisterNetSyncVariableBool("m_IsOpened");
		RegisterNetSyncVariableBool("m_LootDelay");
		
		if (IsMissionHost())
			SetAllowDamage(false);
	}
	
	void ~Expansion_SupplyCrate_Base()
	{
		if (GetGame())
			CleanupSupplyCrate();
		
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

	void CleanupSupplyCrate()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		if (s_Expansion_AllSupplyCrates)
			s_Expansion_AllSupplyCrates.Remove(m_Expansion_SupplyCrateNode);

		#ifdef SERVER
		ClearCrateLoot();
		#endif
	}

	override void EEInit()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		super.EEInit();

		m_IsLocked = true;
		m_IsOpened = false;
		m_Expansion_CanReceiveItems = false;
		m_LootDelay = false;

		SetSynchDirty();
		
	#ifdef DIAG_DEVELOPER
	#ifdef EXPANSIONMODNAVIGATION
    #ifdef SERVER
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CreateDebugMarker, 500, false);
    #endif
	#endif
	#endif
	}

	override void EEItemAttached(EntityAI item, string slot_name)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		ExDebugPrint("::EEItemAttached - Item: " + item.ToString() + " | Slot:" + slot_name);

		super.EEItemAttached(item, slot_name);

		Expansion_SupplyCrate_Key_Base key;
		if (Class.CastTo(key, item))
		{
			if (GetGame().IsServer())
			{
				int slot_id_key = InventorySlots.GetSlotIdFromString("Att_ExpansionSupplyCrateKey");
				GetInventory().SetSlotLock(slot_id_key, true);
				SetLockState(false);
			}
		}

		#ifndef SERVER
		EffectSound sound = SEffectManager.PlaySound(KEY_ATTACH_SOUND, GetPosition());
		sound.SetAutodestroy( true );
		#endif
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		ExDebugPrint("::EEItemDetached - Item: " + item.ToString() + " | Slot:" + slot_name);

		super.EEItemDetached(item, slot_name);

		Expansion_SupplyCrate_Key_Base key;
		if (Class.CastTo(key, item))
		{
			if (GetGame().IsServer())
			{
				int slot_id_key = InventorySlots.GetSlotIdFromString("Att_ExpansionSupplyCrateKey");
				GetInventory().SetSlotLock(slot_id_key, false);
				SetLockState(true);
			}
		}

		#ifndef SERVER
		EffectSound sound = SEffectManager.PlaySound(KEY_DETTACH_SOUND, GetPosition());
		sound.SetAutodestroy(true);
		#endif
	}

	void SetLockState(bool state)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		ExDebugPrint("::SetLockState - State: " + state.ToString());

		m_IsLocked = state;
		SetSynchDirty();
	}

	bool CanInsertKey()
	{
		//! Check if crate has already a key
		int slot_id_key = InventorySlots.GetSlotIdFromString("Att_ExpansionSupplyCrateKey");
		Expansion_SupplyCrate_Key_Base key;
		if (Class.CastTo(key, GetInventory().FindAttachment(slot_id_key)))
			return false;
		
		return true;
	}

	bool HasKey()
	{
		int slot_id_key = InventorySlots.GetSlotIdFromString("Att_ExpansionSupplyCrateKey");
		Expansion_SupplyCrate_Key_Base key;
		if (Class.CastTo(key, GetInventory().FindAttachment(slot_id_key)))
			return true;

		return false;
	}

	void OpenCrate()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		SpawnLoot();
		SetAnimationPhase("Door1", 1.0);
		m_IsOpened = true;
		SetLastOpenTime();
		SetSynchDirty();
	}

	void CloseCrate()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		ClearCrateLoot();
		SetAnimationPhase("Door1", 0);
		m_IsOpened = false;
		m_LootDelay = true;
		SetLastCloseTime();		
		SetSynchDirty();
	}

	void ClearCrateLoot()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		array<EntityAI> items = new array<EntityAI>;
		items.Reserve(GetInventory().CountInventory());
		GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		
		foreach (EntityAI invItem: items)
		{
			if (invItem == this)
				continue;
	
			GetGame().ObjectDelete(invItem);
		}
	}
	
	void SpawnLoot()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		
		int lootCount = m_CrateLoot.Count();

		if (lootCount > 5)
			lootCount = 5;

		m_Expansion_CanReceiveItems = true;
		ExpansionLootSpawner.SpawnLoot(this, m_CrateLoot, lootCount, m_LootItems, m_LootItemsMap, 0.1, 0.6);
		m_Expansion_CanReceiveItems = false;
	}

	void Expansion_SetCanReceiveItems(bool state)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		m_Expansion_CanReceiveItems = true;
	}

	bool HasLootDelay()
	{
		return m_LootDelay;
	}
	
	void SetLastCloseTime()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		
		m_LastCloseTime = CF_Date.Now(true).GetTimestamp();
	}
	
	int GetLastCloseTime()
	{
		return m_LastCloseTime;
	}
	
	void SetLastOpenTime()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		
		m_LastOpenTime = CF_Date.Now(true).GetTimestamp();
	}
	
	int GetLastOpenTime()
	{
		return m_LastOpenTime;
	}
	
	void EndLootDelay()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		
		m_LootDelay = false;
		
		SetSynchDirty();
	}
	
	void SetCrateLoot(array<ref ExpansionLoot> crateLoot)
	{
		foreach (ExpansionLoot loot: crateLoot)
		{
			m_CrateLoot.Insert(loot);
		}
	}
	
	void SetValidKeyType(ExpansionSupplyCrateKeyType keyType)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		ExDebugPrint("::SetValidKeyType - Key type: " + typename.EnumToString(ExpansionSupplyCrateKeyType, keyType));
		
		m_ValidKeyType = keyType;
	}
	
	protected void SoundDecompressStart()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		PlaySoundSet(m_SoundDecompress, DECOMPRESS_SOUND, 0, 0);
	}
	
	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		if (Expansion_SupplyCrate_Key_Base.Cast(attachment))
			return true;
		
		return m_Expansion_CanReceiveItems;
	}

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{		
		return m_Expansion_CanReceiveItems;
	}

	override bool CanSwapItemInCargo(EntityAI child_entity, EntityAI new_entity)
	{
		return false;
	}

	override bool IsInventoryVisible()
	{
		return m_IsOpened;
	}

	override bool CanDisplayCargo()
	{
		return m_IsOpened;
	}

	bool IsCrateOpened()
	{
		return m_IsOpened;
	}

	bool IsCrateLocked()
	{
		return m_IsLocked;
	}
	
	bool Expansion_CanReceiveItems()
	{
		return m_Expansion_CanReceiveItems;
	}
	
	ExpansionSupplyCrateKeyType GetValidKeyType()
	{
		return m_ValidKeyType;
	}

	override void SetActions()
	{
		super.SetActions();

		AddAction(ExpansionActionOpenSupplyCrate);
		AddAction(ExpansionActionRemoveSupplyCrateKey);
	}

	override void OnVariablesSynchronized()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		super.OnVariablesSynchronized();
		
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(UpdateSupplyCrateVFX_Deferred, 0);
	}
	
	protected void UpdateSupplyCrateVFX_Deferred()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		
		ExDebugPrint("::UpdateSupplyCrateVFX_Deferred - Is locked: " + m_IsLocked.ToString());
		ExDebugPrint("::UpdateSupplyCrateVFX_Deferred - Is opened: " + m_IsOpened.ToString());
		ExDebugPrint("::UpdateSupplyCrateVFX_Deferred - Has loot delay: " + m_LootDelay.ToString());
		
		if (m_IsOpened)
		{
			SoundDecompressStart();
		}
	}

	override void AfterStoreLoad()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		super.AfterStoreLoad();
		
		GetGame().ObjectDelete(this);
	}

	protected void ExDebugPrint(string text)
	{
		#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
		#endif
	}
	
#ifdef DIAG_DEVELOPER
#ifdef EXPANSIONMODNAVIGATION
	void CreateDebugMarker()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		ExpansionMarkerModule markerModule;
		CF_Modules<ExpansionMarkerModule>.Get(markerModule);
		if (markerModule)
			m_ServerMarker = markerModule.CreateServerMarker(GetType(), "Tent", Vector(GetPosition()[0], GetPosition()[1] + 1.0, GetPosition()[2]), ARGB(255, 41, 128, 185), true);
	}
#endif
#endif
};

class Expansion_SupplyCrate_Bunker_Orange extends Expansion_SupplyCrate_Base 
{
	void Expansion_SupplyCrate_Bunker_Orange()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		SetValidKeyType(ExpansionSupplyCrateKeyType.BUNKER);
	}
};

class Expansion_SupplyCrate_Bunker_Green extends Expansion_SupplyCrate_Base 
{
	void Expansion_SupplyCrate_Bunker_Green()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		SetValidKeyType(ExpansionSupplyCrateKeyType.BUNKER);
	}
};

class Expansion_SupplyCrate_Bunker_Brown extends Expansion_SupplyCrate_Base 
{
	void Expansion_SupplyCrate_Bunker_Brown()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		SetValidKeyType(ExpansionSupplyCrateKeyType.BUNKER);
	}
};

class Expansion_SupplyCrate_World_T1_Orange extends Expansion_SupplyCrate_Base 
{
	void Expansion_SupplyCrate_World_T1_Orange()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		SetValidKeyType(ExpansionSupplyCrateKeyType.WORLD_T1);
	}
};

class Expansion_SupplyCrate_World_T1_Green extends Expansion_SupplyCrate_Base 
{
	void Expansion_SupplyCrate_World_T1_Green()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		SetValidKeyType(ExpansionSupplyCrateKeyType.WORLD_T1);
	}
};

class Expansion_SupplyCrate_World_T1_Brown extends Expansion_SupplyCrate_Base 
{
	void Expansion_SupplyCrate_World_T1_Brown()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		SetValidKeyType(ExpansionSupplyCrateKeyType.WORLD_T1);
	}
};

class Expansion_SupplyCrate_Key_Base extends ItemBase
{
	protected ExpansionSupplyCrateKeyType m_KeyType;
	
	void SetKeyType(ExpansionSupplyCrateKeyType keyType)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		ExDebugPrint("::SetKeyType - Key type: " + typename.EnumToString(ExpansionSupplyCrateKeyType, keyType));
		
		m_KeyType = keyType;
	}
	
	override void SetActions()
	{
		super.SetActions();

		AddAction(ExpansionActionInsertSupplyCrateKey);
	}
	
	ExpansionSupplyCrateKeyType GetKeyType()
	{
		return m_KeyType;
	}
	
	protected void ExDebugPrint(string text)
	{
		#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
		#endif
	}
};

class Expansion_SupplyCrate_Bunker_Key extends Expansion_SupplyCrate_Key_Base 
{
	void Expansion_SupplyCrate_Bunker_Key()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		SetKeyType(ExpansionSupplyCrateKeyType.BUNKER);
	}
};

class Expansion_SupplyCrate_WorldT1_Key extends Expansion_SupplyCrate_Key_Base 
{
	void Expansion_SupplyCrate_WorldT1_Key()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		SetKeyType(ExpansionSupplyCrateKeyType.WORLD_T1);
	}
};
