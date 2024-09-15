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
	static protected const string RED_LIGHT_GLOW	= "dz\\gear\\camping\\data\\battery_charger_light_r.rvmat";
	static protected const string GREEN_LIGHT_GLOW	= "dz\\gear\\camping\\data\\battery_charger_light_g.rvmat";
	static protected const string DEFAULT_MATERIAL 	= "DayZExpansion\\Objects\\Basebuilding\\Items\\Codelock\\data\\codelock.rvmat";

	protected static ref TStringArray s_Expansion_InventorySlots = new TStringArray;

	protected EffectSound m_Expansion_Sound;
	
	protected bool m_Expansion_WasSynced;
	protected bool m_Expansion_WasLocked;

	void ExpansionCodeLock()
	{
		m_Expansion_KnownUIDs = new TStringArray;

		RegisterNetSyncVariableBool( "m_Expansion_Locked" );
		RegisterNetSyncVariableInt( "m_Expansion_CodeLength" );
	}

	override void DeferredInit()
	{
		super.DeferredInit();

		UpdateVisuals();

		if (!GetGame().IsServer())
			return;

		ItemBase parent = ItemBase.Cast( GetHierarchyParent() );
		if ( parent && parent.ExpansionGetCodeLock() == this )
		{
			if ( !HasCode() && parent.HasCode() )
			{
				EXPrint(ToString() + "::DeferredInit - migrating code from " + parent.ToString() + " " + parent.GetPosition());

				//! Migrate code
				SetCode(parent.GetCode(), NULL, false, false);

				//! Migrate locked state
				if (parent.ExpansionIsLocked())
					ExpansionLock();

				parent.SetCode("", NULL, true, false);
			}
			else if (HasCode())
			{
				SetSlotLock( parent, true );
				SetTakeable( false );
			}
		}
		else if ( HasCode() )
		{
			EXPrint(ToString() + "::DeferredInit - removing code from unattached " + ToString() + " " + GetPosition());
			SetCode("");
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
		
		AddAction( ExpansionActionAttachCodeLock );
		//AddAction( ExpansionActionEnterCodeLock );
		//AddAction( ExpansionActionChangeCodeLock );
	}
	
	override bool CanDetachAttachment(EntityAI parent)
	{
		if ( !super.CanDetachAttachment( parent ) )
		{
			return false;
		}
		
		ItemBase target = ItemBase.Cast( parent );

		if ( target && target.ExpansionIsLocked() )
		{
			return false;
		}
		
		return true;
	}

	override void OnWasAttached( EntityAI parent, int slot_id )
	{
		super.OnWasAttached( parent, slot_id );

		UpdateVisuals();
	}

	override void OnWasDetached( EntityAI parent, int slot_id )
	{
		super.OnWasDetached( parent, slot_id );

		UpdateVisuals();

		if (GetGame().IsServer())
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
			GetGame().ConfigGetTextArray("CfgVehicles ExpansionCodeLock inventorySlot", s_Expansion_InventorySlots);

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
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( UpdateVisuals_Deferred, 0, false );
	}
	
	protected void UpdateVisuals_Deferred()
	{
		ItemBase parent = ItemBase.Cast( GetHierarchyParent() );

		if ( parent && parent.IsInherited( BaseBuildingBase ) && !parent.IsInherited( ExpansionBaseBuilding ) )
		{
			HideSelection( "camo" );
			HideSelection( "Codelock" );
			ShowSelection( "attach_fence" );
		} else
		{
			ShowSelection( "camo" );
			ShowSelection( "Codelock" );
			HideSelection( "attach_fence" );
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
		if ( !GetGame().IsDedicatedServer() )
		{
			m_Expansion_Sound = SEffectManager.PlaySound("Expansion_CodeLock_Lock1_SoundSet", GetPosition());
			m_Expansion_Sound.SetSoundAutodestroy( true );
		}
	}
	
	protected void SoundCodeLockUnlocked()
	{
		if ( !GetGame().IsDedicatedServer() ) // client side
		{
			m_Expansion_Sound = SEffectManager.PlaySound("Expansion_CodeLock_Unlock1_SoundSet", GetPosition());
			m_Expansion_Sound.SetSoundAutodestroy( true );
		}
	}
}
