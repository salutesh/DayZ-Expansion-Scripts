/**
 * ExpansionCodeLock.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionCodeLock
 * @brief		
 **/
class ExpansionCodeLock: ItemBase
{
	static protected const string RED_LIGHT_GLOW	= "dz/gear/camping/data/battery_charger_light_r.rvmat";
	static protected const string GREEN_LIGHT_GLOW	= "dz/gear/camping/data/battery_charger_light_g.rvmat";
	static protected const string DEFAULT_MATERIAL 	= "DayZExpansion/Objects/Basebuilding/Items/Codelock/data/codelock.rvmat";

	protected static ref TStringArray s_Expansion_InventorySlots = new TStringArray;

	protected EffectSound m_Expansion_Sound;
	
	protected bool m_Expansion_WasSynced;
	protected bool m_Expansion_WasLocked;

	protected int m_Expansion_DoorIndex = -1;
	protected bool m_Expansion_Reversed;  //! Reversed state (front -> back) when attached to certain rebuildable building doors

	void ExpansionCodeLock()
	{
		m_Expansion_KnownUIDs = new TStringArray;

		RegisterNetSyncVariableBool( "m_Expansion_Locked" );
		RegisterNetSyncVariableInt( "m_Expansion_CodeLength" );
		RegisterNetSyncVariableBool( "m_Expansion_Reversed" );

		if ( !g_Game.IsMultiplayer() )
			m_Expansion_WasSynced = true;
	}

	override bool Expansion_IsLock()
	{
		return true;
	}

	override bool Expansion_IsCodeLock()
	{
		return true;
	}

	override bool ExpansionHasCodeLock(int component)
	{
		EntityAI parent = GetHierarchyParent();
		ItemBase parentItem;
		BuildingBase parentBuilding;
		if (Class.CastTo(parentItem, parent))
		{
			return parentItem.ExpansionHasCodeLock(component);
		}
		else if (Class.CastTo(parentBuilding, parent))
		{
			int doorIndex = parentBuilding.GetDoorIndex(component);
			if (doorIndex != -1)
				return parentBuilding.Expansion_HasCodeLock(doorIndex);
		}

		return false;
	}

	override ItemBase Expansion_GetCodeLock(int component)
	{
		EntityAI parent = GetHierarchyParent();
		ItemBase parentItem;
		BuildingBase parentBuilding;
		if (Class.CastTo(parentItem, parent))
		{
			return parentItem.Expansion_GetCodeLock(component);
		}
		else if (Class.CastTo(parentBuilding, parent))
		{
			int doorIndex = parentBuilding.GetDoorIndex(component);
			if (doorIndex != -1)
				return parentBuilding.Expansion_GetCodeLock(doorIndex);
		}

		return null;
	}

	override void ExpansionLock()
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.BASEBUILDING, this);
	#endif

		super.ExpansionLock();

		BuildingBase parentBuilding;
		if (Class.CastTo(parentBuilding, GetHierarchyParent()))
		{
			int doorIndex = parentBuilding.Expansion_FindAttachmentDoorIndex(this);
			if (doorIndex != -1 && !parentBuilding.IsDoorLocked(doorIndex))
				parentBuilding.LockDoor(doorIndex, true);
		}
	}

	override void ExpansionUnlock()
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.BASEBUILDING, this);
	#endif

		super.ExpansionUnlock();

		BuildingBase parentBuilding;
		if (Class.CastTo(parentBuilding, GetHierarchyParent()))
		{
			int doorIndex = parentBuilding.Expansion_FindAttachmentDoorIndex(this);
			if (doorIndex != -1)
				parentBuilding.UnlockDoor(doorIndex, false);
		}
	}

	override void CloseAndLock(int component)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.BASEBUILDING, this);
	#endif

		EntityAI parent = GetHierarchyParent();
		ItemBase parentItem;
		BuildingBase parentBuilding;
		if (Class.CastTo(parentItem, parent))
		{
			parentItem.CloseAndLock(component);
		}
		else if (Class.CastTo(parentBuilding, parent))
		{
			int doorIndex = parentBuilding.GetDoorIndex(component);
			if (doorIndex != -1)
				parentBuilding.Expansion_CloseAndLock(doorIndex);
		}
		else
			super.ExpansionLock();
	}

	override void UnlockAndOpen(int component)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.BASEBUILDING, this);
	#endif

		EntityAI parent = GetHierarchyParent();
		ItemBase parentItem;
		BuildingBase parentBuilding;
		if (Class.CastTo(parentItem, parent))
		{
			parentItem.UnlockAndOpen(component);
		}
		else if (Class.CastTo(parentBuilding, parent))
		{
			int doorIndex = parentBuilding.GetDoorIndex(component);
			if (doorIndex != -1)
				parentBuilding.Expansion_UnlockAndOpen(doorIndex);
		}
		else
			super.ExpansionUnlock();
	}

	override bool ExpansionCanOpen(PlayerBase player, int component)
	{
		EntityAI parent = GetHierarchyParent();
		ItemBase parentItem;
		BuildingBase parentBuilding;
		if (Class.CastTo(parentItem, parent))
		{
			return parentItem.ExpansionCanOpen(player, component);
		}
		else if (Class.CastTo(parentBuilding, parent))
		{
			int doorIndex = parentBuilding.GetDoorIndex(component);
			if (doorIndex != -1)
				return parentBuilding.Expansion_CanOpen(player, doorIndex);
		}

		return false;
	}

	override bool ExpansionCanClose(PlayerBase player, int component)
	{
		EntityAI parent = GetHierarchyParent();
		ItemBase parentItem;
		BuildingBase parentBuilding;
		if (Class.CastTo(parentItem, parent))
		{
			return parentItem.ExpansionCanClose(player, component);
		}
		else if (Class.CastTo(parentBuilding, parent))
		{
			int doorIndex = parentBuilding.GetDoorIndex(component);
			if (doorIndex != -1)
				return parentBuilding.Expansion_CanClose(player, doorIndex);
		}

		return false;
	}

	override void Expansion_Open(int component)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.BASEBUILDING, this);
	#endif

		EntityAI parent = GetHierarchyParent();
		ItemBase parentItem;
		BuildingBase parentBuilding;
		if (Class.CastTo(parentItem, parent))
		{
			parentItem.Expansion_Open(component);
		}
		else if (Class.CastTo(parentBuilding, parent))
		{
			int doorIndex = parentBuilding.GetDoorIndex(component);
			if (doorIndex != -1)
				parentBuilding.Expansion_Open(doorIndex);
		}
	}

	override void Expansion_Close(int component)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.BASEBUILDING, this);
	#endif

		EntityAI parent = GetHierarchyParent();
		ItemBase parentItem;
		BuildingBase parentBuilding;
		if (Class.CastTo(parentItem, parent))
		{
			parentItem.Expansion_Close(component);
		}
		else if (Class.CastTo(parentBuilding, parent))
		{
			int doorIndex = parentBuilding.GetDoorIndex(component);
			if (doorIndex != -1)
				parentBuilding.Expansion_Close(doorIndex);
		}
	}

	override bool IsOpen()
	{
		EntityAI parent = GetHierarchyParent();
		ItemBase parentItem;
		BuildingBase parentBuilding;
		
		if (Class.CastTo(parentItem, parent))
		{
			return parentItem.IsOpen();
		}
		else if (Class.CastTo(parentBuilding, parent))
		{
			int doorIndex = parentBuilding.Expansion_FindAttachmentDoorIndex(this);
			if (doorIndex != -1 && parentBuilding.IsDoorOpen(doorIndex))
				return true;
		}

		return false;
	}

	bool Expansion_GetReversedState()
	{
		return m_Expansion_Reversed;
	}

	override bool Expansion_IsPlayerInside(PlayerBase player, string selection)
	{
		EntityAI parent = GetHierarchyParent();
		ItemBase parentItem;
		BuildingBase parentBuilding;
		if (Class.CastTo(parentItem, parent))
		{
			return parentItem.Expansion_IsPlayerInside(player, selection);
		}
		else if (Class.CastTo(parentBuilding, parent))
		{
			if (player && MemoryPointExists("ce_center") && MemoryPointExists("invview"))
			{
				bool isPlayerInside = MiscGameplayFunctions.MemPointDirectionalCheck(player, this, "ce_center", "invview");
				return isPlayerInside != m_Expansion_Reversed;
			}
		}

		return super.Expansion_IsPlayerInside(player, selection);
	}
	
	protected void Expansion_SetDoorInfoFromSlot(BuildingBase buildingBase, string slotName)
	{
		map<int, ref AdditionalDoorInfo> doorsInfoMap = buildingBase.GetDoorInfoHolder().GetDoorInfoMap();
		if (doorsInfoMap && doorsInfoMap.Count() > 0)
		{
			int doorIndex = -1;
			AdditionalDoorInfo doorInfo;
			foreach (int doorInfoIndex, AdditionalDoorInfo doorInfoData: doorsInfoMap)
			{
				foreach (string relatedSlotName: doorInfoData.m_RelatedInventorySlotNames)
				{
					if (relatedSlotName == slotName)
					{
						doorIndex = doorInfoIndex;
						doorInfo = doorInfoData;
						break;
					}
				}

				if (doorIndex != -1)
					break;
			}

			if (doorIndex != -1)
				m_Expansion_DoorIndex = doorIndex;
			
			//! Check for door part rotation
			Rebuilding rebuilding = Rebuilding.Cast(buildingBase.GetConstructionBasic());
			if (rebuilding && doorInfo)
			{
				ConstructionPart constructionPart = rebuilding.GetConstructionPart(doorInfo.m_DoorConstructionPartName);
				ConstructionPartRebuildReversibleDoor reversibleDoorPart;
				if (constructionPart && Class.CastTo(reversibleDoorPart, constructionPart))
					m_Expansion_Reversed = reversibleDoorPart.IsReversedLocal();
			#ifdef DIAG_DEVELOPER
				EXError.Info(this, "" + buildingBase.GetType() + " door " + doorInfo.m_DoorConstructionPartName + " reversed " + m_Expansion_Reversed.ToString());
			#endif
			}
		}	
	}
	
	//! Reversed state comes from the door part so it must be read again once the parent has loaded its construction data
	void Expansion_RefreshDoorInfoServer()
	{
		InventoryLocation il = new InventoryLocation();
		if (!GetInventory().GetCurrentInventoryLocation(il) || il.GetType() != InventoryLocationType.ATTACHMENT)
			return;

		string slotName = InventorySlots.GetSlotName(il.GetSlot());
		BuildingBase buildingBase = BuildingBase.Cast(il.GetParent());
		if (!buildingBase || slotName.IndexOf("Att_CodeLock") == -1)
			return;

		bool wasReversed = m_Expansion_Reversed;
		Expansion_SetDoorInfoFromSlot(buildingBase, slotName);
		if (m_Expansion_Reversed == wasReversed)
			return;

	#ifdef DIAG_DEVELOPER
		EXError.Info(this, "Reversed state changed after parent building storage load");
	#endif
		SetSynchDirty();
	}

	override void DeferredInit()
	{
		super.DeferredInit();

		UpdateVisuals();

		if (!g_Game.IsServer())
			return;

		EntityAI parent = GetHierarchyParent();

		ItemBase parentItem;
		if ( Class.CastTo( parentItem, parent ) && parentItem.ExpansionGetCodeLock() == this )
		{
			if ( !HasCode() && parentItem.HasCode() )
			{
				EXPrint(ToString() + "::DeferredInit - migrating code from " + parentItem.ToString() + " " + parentItem.GetPosition());

				//! Migrate code
				SetCode(parentItem.GetCode(), NULL, false, false);

				//! Migrate locked state
				if (parentItem.ExpansionIsLocked())
					ExpansionLock();

				parentItem.SetCode("", NULL, true, false);
			}
		}

		if ( HasCode() )
		{
			if ( parent )
			{
				SetSlotLock( parent, true );
				SetTakeable( false );
			}
			else
			{
				EXPrint(ToString() + "::DeferredInit - removing code from unattached " + ToString() + " " + GetPosition());
				SetCode("");
			}
		}
	}

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return;

		ctx.Write( m_Expansion_Locked );
		ctx.Write( m_Expansion_Code );

		ctx.Write( m_Expansion_KnownUIDs.Count() );
		for ( int i = 0; i < m_Expansion_KnownUIDs.Count(); i++ )
		{
			ctx.Write( m_Expansion_KnownUIDs[i] );
		}
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return true;

		if ( ctx.GetVersion() >= 38 )
		{
			if ( Expansion_Assert_False( ctx.Read( m_Expansion_Locked ), "[" + this + "] Failed reading m_Expansion_Locked" ) )
				return false;
			if ( Expansion_Assert_False( ctx.Read( m_Expansion_Code ), "[" + this + "] Failed reading m_Expansion_Code" ) )
				return false;

			m_Expansion_CodeLength = m_Expansion_Code.Length();
		}

		int count;
		if (!ctx.Read(count))
			return false;

		for (int i = 0; i < count; i++)
		{
			string knownUID;
			if (!ctx.Read(knownUID))
				return false;

			m_Expansion_KnownUIDs.Insert( knownUID );
		}

		return true;
	}
	#endif

	override void SetActions()
	{
		super.SetActions();
		
		AddAction( ExpansionActionAttachCodeLock );  //! For tents and items w/o construction (e.g. modded containers)
		AddAction( ActionAttachToConstruction );  //! For BB and rebuildable buildings
	}
	
	override bool CanDetachAttachment(EntityAI parent)
	{
		if ( !super.CanDetachAttachment( parent ) )
		{
			return false;
		}
		
		if ( ExpansionIsLocked() )
		{
			return false;
		}
		
		return true;
	}

	override void OnWasAttached( EntityAI parent, int slot_id )
	{
		super.OnWasAttached( parent, slot_id );
		
		if (g_Game.IsServer())
		{
			string slotName = InventorySlots.GetSlotName(slot_id);
			if (slotName.IndexOf("Att_CodeLock") > -1)
			{
				if (parent && parent.CanUseConstruction())
				{
					BuildingBase buildingBase = BuildingBase.Cast(parent);
					if (buildingBase)
						Expansion_SetDoorInfoFromSlot(buildingBase, slotName);
				}
			}
		}

		UpdateVisuals();
	}

	override void OnWasDetached( EntityAI parent, int slot_id )
	{
		super.OnWasDetached( parent, slot_id );

		m_Expansion_DoorIndex = -1;
		m_Expansion_Reversed = false;

		UpdateVisuals();

		if (g_Game.IsServer())
			SetCode("");
	}

	void UnlockServer( EntityAI player, EntityAI parent )
	{
		if ( parent && GetInventory().IsAttachment() )
		{
			ExpansionUnlock();
			ItemBase item;
			if (parent.GetInventory() && parent.GetInventory().IsInventoryLocked() && Class.CastTo(item, parent) && !item.IsOpen())
				item.Expansion_Open("");
			ExpansionDropServer( PlayerBase.Cast( player ) );
		}
	}

	override protected string GetDestroySound()
	{
		return "combinationlock_open_SoundSet";
	}

	override void ExpansionOnDestroyed( Object killer )
	{
		if ( IsDamageDestroyed() )
			return;

		SetHealth( 0 );
		UnlockServer( EntityAI.Cast( killer ), GetHierarchyParent() );
	}
		
	static TStringArray Expansion_GetCodeLockInventorySlots()
	{
		if (!s_Expansion_InventorySlots.Count())
			g_Game.ConfigGetTextArray("CfgVehicles ExpansionCodeLock inventorySlot", s_Expansion_InventorySlots);

		return s_Expansion_InventorySlots;
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if ( m_Expansion_WasSynced && m_Expansion_WasLocked != m_Expansion_Locked )
		{
			if ( m_Expansion_Locked )
				SoundCodeLockLocked();
			else
				SoundCodeLockUnlocked();
		}

		m_Expansion_WasLocked = m_Expansion_Locked;
		m_Expansion_WasSynced = true;

		UpdateVisuals();
	}

	// --- VISUALS
	void UpdateVisuals()
	{
		//Client/Server
		g_Game.GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( UpdateVisuals_Deferred, 0, false );
	}
	
	protected void UpdateVisuals_Deferred()
	{
		EntityAI parent = GetHierarchyParent();

		if ( parent && ( parent.IsInherited( BaseBuildingBase ) || parent.IsBuilding() ) )
		{
			HideSelection( "camo" );
			HideSelection( "Codelock" );

			InventoryLocation il = new InventoryLocation();
			GetInventory().GetCurrentInventoryLocation( il );
			if ( parent.IsInherited( ExpansionBaseBuilding ) )
			{
				HideSelection( "attach_door_inside" );
				HideSelection( "attach_door_reversed_inside" );
				HideSelection( "attach_fence" );
				HideSelection( "attach_fence_outside" );
				HideSelection( "attach_fence_digital_inside" );
				HideSelection( "attach_fence_digital_outside" );
				ShowSelection( "attach_wall" );
			}
			else
			{
				if ( parent.IsBuilding() )
				{
					if ( m_Expansion_Reversed )
					{
						HideSelection( "attach_door_inside" );
						ShowSelection( "attach_door_reversed_inside" );
						ShowSelection( "attach_fence" );  //! outside
						HideSelection( "attach_fence_digital_outside" );
					}
					else
					{
						ShowSelection( "attach_door_inside" );
						HideSelection( "attach_door_reversed_inside" );
						HideSelection( "attach_fence" );
						ShowSelection( "attach_fence_digital_outside" );
					}

					HideSelection( "attach_fence_outside" );
					HideSelection( "attach_fence_digital_inside" );
				}
				else
				{
					HideSelection( "attach_door_inside" );
					HideSelection( "attach_door_reversed_inside" );

					if ( InventorySlots.GetSlotName( il.GetSlot() ).IndexOf( "Att_CodeLock" ) == 0 )
					{
						HideSelection( "attach_fence" );
						HideSelection( "attach_fence_outside" );
						ShowSelection( "attach_fence_digital_inside" );
						ShowSelection( "attach_fence_digital_outside" );
					}
					else
					{
						ShowSelection( "attach_fence" );
						ShowSelection( "attach_fence_outside" );
						HideSelection( "attach_fence_digital_inside" );
						HideSelection( "attach_fence_digital_outside" );
					}
				}

				HideSelection( "attach_wall" );
			}
		} else
		{
			ShowSelection( "camo" );
			ShowSelection( "Codelock" );
			HideSelection( "attach_door_inside" );
			HideSelection( "attach_door_reversed_inside" );
			HideSelection( "attach_fence" );
			HideSelection( "attach_fence_outside" );
			HideSelection( "attach_fence_digital_inside" );
			HideSelection( "attach_fence_digital_outside" );
			HideSelection( "attach_wall" );
		}

		if ( !IsMissionClient() )
			return;

		if ( parent && ExpansionIsLocked() )
		{
			SetObjectMaterial( 0, DEFAULT_MATERIAL );
			SetObjectMaterial( 1, GREEN_LIGHT_GLOW );
			SetObjectMaterial( 2, DEFAULT_MATERIAL );
		} else
		{
			SetObjectMaterial( 0, RED_LIGHT_GLOW );
			SetObjectMaterial( 1, DEFAULT_MATERIAL );
			SetObjectMaterial( 2, DEFAULT_MATERIAL );
		}
	}
	
	protected void SoundCodeLockLocked()
	{
		if ( !g_Game.IsDedicatedServer() )
		{
			m_Expansion_Sound = SEffectManager.PlaySound("Expansion_CodeLock_Lock1_SoundSet", GetPosition());
			m_Expansion_Sound.SetSoundAutodestroy( true );
		}
	}
	
	protected void SoundCodeLockUnlocked()
	{
		if ( !g_Game.IsDedicatedServer() ) // client side
		{
			m_Expansion_Sound = SEffectManager.PlaySound("Expansion_CodeLock_Unlock1_SoundSet", GetPosition());
			m_Expansion_Sound.SetSoundAutodestroy( true );
		}
	}
}
