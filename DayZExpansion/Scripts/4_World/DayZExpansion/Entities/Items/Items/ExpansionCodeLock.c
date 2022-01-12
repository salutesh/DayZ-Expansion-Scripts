/**
 * ExpansionCodeLock.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionCodeLock
 * @brief		
 **/
class ExpansionCodeLock extends ItemBase
{
	static protected const string RED_LIGHT_GLOW	= "dz\\gear\\camping\\data\\battery_charger_light_r.rvmat";
	static protected const string GREEN_LIGHT_GLOW	= "dz\\gear\\camping\\data\\battery_charger_light_g.rvmat";
	static protected const string DEFAULT_MATERIAL 	= "DayZExpansion\\Objects\\Basebuilding\\Items\\Codelock\\data\\codelock.rvmat";

	void ExpansionCodeLock()
	{
		m_KnownUIDs = new TStringArray;
	}

	override void EEInit()
	{
		super.EEInit();

		UpdateVisuals();

		ItemBase parent = ItemBase.Cast( GetHierarchyParent() );
		if ( parent && parent.IsLocked() )
		{
			SetSlotLock( parent, true );
			SetTakeable( false );
		}
	}

	override void OnStoreSave( ParamsWriteContext ctx )
	{
		#ifdef CF_MODULE_MODSTORAGE
		if ( GetGame().SaveVersion() >= EXPANSION_VERSION_GAME_MODSTORAGE_TARGET )
		{
			super.OnStoreSave( ctx );
			return;
		}
		#endif

		super.OnStoreSave( ctx );

		ctx.Write( m_KnownUIDs );
	}

	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( Expansion_Assert_False( super.OnStoreLoad( ctx, version ), "[" + this + "] Failed reading OnStoreLoad super" ) )
			return false;

		#ifdef CF_MODULE_MODSTORAGE
		if ( version > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET || m_ExpansionSaveVersion > EXPANSION_VERSION_SAVE_MODSTORAGE_TARGET )
			return true;
		#endif

		if ( m_ExpansionSaveVersion >= 20 )
		{
			if ( Expansion_Assert_False( ctx.Read( m_KnownUIDs ), "[" + this + "] Failed reading m_KnownUIDs" ) )
				return false;
		}

		return true;
	}

	#ifdef CF_MODULE_MODSTORAGE
	override void CF_OnStoreSave(map<string, CF_ModStorage> storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[ModStructure.DZ_Expansion];
		if (!ctx) return;

		ctx.Write( m_KnownUIDs.Count() );
		for ( int i = 0; i < m_KnownUIDs.Count(); i++ )
		{
			ctx.Write( m_KnownUIDs[i] );
		}
	}
	
	override bool CF_OnStoreLoad(map<string, CF_ModStorage> storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[ModStructure.DZ_Expansion];
		if (!ctx) return true;

		int count;
		if (!ctx.Read(count))
			return false;

		for (int i = 0; i < count; i++)
		{
			string knownUID;
			if (!ctx.Read(knownUID))
				return false;

			m_KnownUIDs.Insert( knownUID );
		}

		return true;
	}
	#endif

	override void SetActions()
	{
		super.SetActions();
		
		AddAction( ExpansionActionAttachCodeLock );
	}
	
	override bool CanDetachAttachment(EntityAI parent)
	{
		if ( !super.CanDetachAttachment( parent ) )
		{
			return false;
		}
		
		ItemBase target = ItemBase.Cast( parent );

		if ( target && target.IsLocked() )
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
	}

	void SetSlotLock( EntityAI parent, bool state )
	{
		InventoryLocation inventory_location = new InventoryLocation;
		GetInventory().GetCurrentInventoryLocation( inventory_location );			
		parent.GetInventory().SetSlotLock( inventory_location.GetSlot(), state );
	}
	
	bool IsLockAttached()
	{
		EntityAI parent = GetHierarchyParent();

		if ( !parent )
			return false;

		return parent.IsInherited( BaseBuildingBase ) || parent.IsInherited( TentBase );
	}

	void UnlockServer( EntityAI player, EntityAI parent )
	{
		if ( IsLockAttached() )
		{
			ItemBase.Cast( parent ).Unlock();
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
		
	// --- VISUALS
	void UpdateVisuals()
	{
		//Client/Server
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( UpdateVisuals_Deferred, 0, false );
	}
	
	protected void UpdateVisuals_Deferred()
	{
		ItemBase parent = ItemBase.Cast( GetHierarchyParent() );

		if ( parent && parent.IsInherited( Fence ) )
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

		if ( parent && parent.IsLocked() )
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
}