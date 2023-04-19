/**
 * Expansion_SupplyCrate_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

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
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		CleanupSupplyCrate();
	}

	void CleanupSupplyCrate()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (s_Expansion_AllSupplyCrates)
			s_Expansion_AllSupplyCrates.Remove(m_Expansion_SupplyCrateNode);

		if (IsMissionHost())
			ClearCrateLoot();
	}

	override void EEInit()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.EEInit();

		m_IsLocked = true;
		m_IsOpened = false;
		m_Expansion_CanReceiveItems = false;
		m_LootDelay = false;

		SetSynchDirty();
	}

	override void EEItemAttached(EntityAI item, string slot_name)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::EEItemAttached - Item: " + item.ToString() + " | Slot:" + slot_name);

		super.EEItemAttached(item, slot_name);

		Expansion_SupplyCrate_Key key;
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
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::EEItemDetached - Item: " + item.ToString() + " | Slot:" + slot_name);

		super.EEItemDetached(item, slot_name);

		Expansion_SupplyCrate_Key key;
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
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::SetLockState - State: " + state.ToString());

		m_IsLocked = state;
		SetSynchDirty();
	}

	bool CanInsertKey()
	{
		int slot_id_key = InventorySlots.GetSlotIdFromString("Att_ExpansionSupplyCrateKey");
		Expansion_SupplyCrate_Key key;
		if (Class.CastTo(key, GetInventory().FindAttachment(slot_id_key)))
			return false;

		return true;
	}

	bool HasKey()
	{
		int slot_id_key = InventorySlots.GetSlotIdFromString("Att_ExpansionSupplyCrateKey");
		Expansion_SupplyCrate_Key key;
		if (Class.CastTo(key, GetInventory().FindAttachment(slot_id_key)))
			return true;

		return false;
	}

	void OpenCrate()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		SpawnLoot();
		SetAnimationPhase("Door1", 1.0);
		m_IsOpened = true;
		SetLastOpenTime();
		SetSynchDirty();
	}

	void CloseCrate()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		ClearCrateLoot();
		SetAnimationPhase("Door1", 0);
		m_IsOpened = false;
		m_LootDelay = true;
		SetLastCloseTime();		
		SetSynchDirty();
	}

	void ClearCrateLoot()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

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
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		int lootCount = m_CrateLoot.Count();
		int randomAmount = Math.RandomIntInclusive(1, lootCount);
		m_Expansion_CanReceiveItems = true;
		ExpansionLootSpawner.SpawnLoot(this, m_CrateLoot, randomAmount, m_LootItems, m_LootItemsMap);
		m_Expansion_CanReceiveItems = false;
	}

	void Expansion_SetCanReceiveItems(bool state)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		m_Expansion_CanReceiveItems = true;
	}

	bool HasLootDelay()
	{
		return m_LootDelay;
	}
	
	void SetLastCloseTime()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		m_LastCloseTime = CF_Date.Now(true).GetTimestamp();
	}
	
	int GetLastCloseTime()
	{
		return m_LastCloseTime;
	}
	
	void SetLastOpenTime()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		m_LastOpenTime = CF_Date.Now(true).GetTimestamp();
	}
	
	int GetLastOpenTime()
	{
		return m_LastOpenTime;
	}
	
	void EndLootDelay()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
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
	
	protected void SoundDecompressStart()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
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
		if (Expansion_SupplyCrate_Key.Cast(attachment))
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

	override void SetActions()
	{
		super.SetActions();

		AddAction(ExpansionActionOpenSupplyCrate);
		AddAction(ExpansionActionRemoveSupplyCrateKey);
	}

	override void OnVariablesSynchronized()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.OnVariablesSynchronized();
		
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(UpdateSupplyCrateVFX_Deferred, 0);
	}
	
	protected void UpdateSupplyCrateVFX_Deferred()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		DebugTrace("::UpdateSupplyCrateVFX_Deferred - Is locked: " + m_IsLocked.ToString());
		DebugTrace("::UpdateSupplyCrateVFX_Deferred - Is opened: " + m_IsOpened.ToString());
		DebugTrace("::UpdateSupplyCrateVFX_Deferred - Has loot delay: " + m_LootDelay.ToString());
		
		if (m_IsOpened)
		{
			SoundDecompressStart();
		}
	}

	override void AfterStoreLoad()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.AfterStoreLoad();
		
		ExpansionLootSpawner.RemoveContainer(this);
	}

	protected void DebugTrace(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Start(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};

class Expansion_SupplyCrate_Orange extends Expansion_SupplyCrate_Base {};
class Expansion_SupplyCrate_Green extends Expansion_SupplyCrate_Base {};
class Expansion_SupplyCrate_Brown extends Expansion_SupplyCrate_Base {};

class Expansion_SupplyCrate_Key extends ItemBase
{
	override void SetActions()
	{
		super.SetActions();

		AddAction(ExpansionActionInsertSupplyCrateKey);
	}
};
