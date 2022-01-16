/**
 * ExpansionSafeBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSafeBase extends Container_Base
{
	protected EffectSound m_Sound;
	
	protected bool m_WasSynced;
	protected bool m_IsOpen;
	protected bool m_WasLocked;

	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionSafeBase()
	{
		m_KnownUIDs = new TStringArray;

		RegisterNetSyncVariableBool( "m_IsOpen" );
		RegisterNetSyncVariableBool( "m_Locked" );
		RegisterNetSyncVariableBool( "m_HasCode" );
		RegisterNetSyncVariableInt( "m_CodeLength" );
	}

	override void EEInit()
	{
		super.EEInit();

		GetInventory().LockInventory(HIDE_INV_FROM_SCRIPT);
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~ExpansionSafeBase()
	{
	}

	// ------------------------------------------------------------
	// SetActions
	// ------------------------------------------------------------
	override void SetActions()
	{
		super.SetActions();

		AddAction( ActionTogglePlaceObject );
		AddAction( ActionPlaceObject );

		AddAction( ExpansionActionOpen );

		AddAction( ExpansionActionCloseAndLockSafe );
		AddAction( ExpansionActionCloseSafe );
		
		AddAction( ExpansionActionEnterSafeLock );
		AddAction( ExpansionActionChangeSafeLock );
	}

	// ------------------------------------------------------------
	// CanBeRepairedToPristine
	// ------------------------------------------------------------
	override bool CanBeRepairedToPristine()
	{
		return true;
	}
	// ------------------------------------------------------------
	// Expansion ExpansionIsOpenable
	// ------------------------------------------------------------
	override bool ExpansionIsOpenable()
	{
		return true;
	}
	
	// ------------------------------------------------------------
	// Expansion CanOpen
	// ------------------------------------------------------------
	override bool ExpansionCanOpen( PlayerBase player, string selection )
	{
		if ( IsLocked() && !IsKnownUser( player ) )
			return false;

		if ( !IsOpened() )
			return true;

		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion CanClose
	// ------------------------------------------------------------
	override bool ExpansionCanClose( PlayerBase player, string selection )
	{
		return CanClose( selection );
	}

	override bool CanClose( string selection )
	{
		if ( IsOpened() && !IsRuined())
			return true;
			
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionHasCodeLock
	// ------------------------------------------------------------
	override bool ExpansionHasCodeLock( string selection )
	{
		return (selection == "codelock");
	}

	// ------------------------------------------------------------
	// Expansion Open
	// ------------------------------------------------------------
	override void Open( string selection ) 
	{
		m_IsOpen = true;

		//! Door open animation
		SetAnimationPhase( "safe_door", 1 );
	
		GetInventory().UnlockInventory(HIDE_INV_FROM_SCRIPT);
		SetSynchDirty();

		super.Open( selection );
	}

	// ------------------------------------------------------------
	// Expansion Close
	// ------------------------------------------------------------
	override void Close( string selection ) 
	{
		m_IsOpen = false;

		//! Door close animation
		SetAnimationPhase( "safe_door", 0 );
		
		GetInventory().LockInventory(HIDE_INV_FROM_SCRIPT);
		SetSynchDirty();
		
		super.Close( selection );
	}

	// ------------------------------------------------------------
	// Expansion IsOpened
	// ------------------------------------------------------------
	override bool IsOpened()
	{
		return m_IsOpen;
	}
	
	/**
	\brief Returning if safe is locked
		\param 	
	*/
	override bool IsLocked()
	{
		return m_HasCode && m_Locked;
	}
	
	protected void SoundCodeLockLocked()
	{
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			m_Sound = SEffectManager.PlaySound("Expansion_CodeLock_Lock3_SoundSet", GetPosition());
			m_Sound.SetSoundAutodestroy( true );
		}
	}
	
	protected void SoundCodeLockUnlocked()
	{
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) // client side
		{
			// Expansion_Succes_SoundSet
			m_Sound = SEffectManager.PlaySound("Expansion_CodeLock_Unlock1_SoundSet", GetPosition());
			m_Sound.SetSoundAutodestroy( true );
		}
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if ( m_WasSynced && m_WasLocked != m_Locked )
		{
			if ( m_Locked )
				SoundCodeLockLocked();
			else
				SoundCodeLockUnlocked();
		}

		m_WasLocked = m_Locked;
		m_WasSynced = true;
	}

	override bool CanUseConstruction()
	{
		return false;
	}
	
	override bool IsDeployable()
	{
		return false;
	}

	/**
	\brief Saving class to storage
		\param 	
	*/
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		#ifdef CF_MODSTORAGE
		if ( GetGame().SaveVersion() >= EXPANSION_VERSION_GAME_MODSTORAGE_TARGET )
		{
			super.OnStoreSave( ctx );
			return;
		}
		#endif

		super.OnStoreSave( ctx );
		
		ctx.Write( m_IsOpen );
		ctx.Write( m_Locked );
		ctx.Write( m_Code );
		ctx.Write( m_HasCode );

		ctx.Write( m_KnownUIDs );
	}

	/**
	\brief Loading class to storage
		\param 	
	*/
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( Expansion_Assert_False( super.OnStoreLoad( ctx, version ), "[" + this + "] Failed reading OnStoreLoad super" ) )
			return false;

		#ifdef CF_MODSTORAGE
		if ( version > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET || m_ExpansionSaveVersion > EXPANSION_VERSION_SAVE_MODSTORAGE_TARGET )
			return true;
		#endif
		
		if ( Expansion_Assert_False( ctx.Read( m_IsOpen ), "[" + this + "] Failed reading m_IsOpen" ) )
			return false;
		if ( Expansion_Assert_False( ctx.Read( m_Locked ), "[" + this + "] Failed reading m_Locked" ) )
			return false;
		if ( Expansion_Assert_False( ctx.Read( m_Code ), "[" + this + "] Failed reading m_Code" ) )
			return false;

		m_CodeLength = m_Code.Length();

		if ( Expansion_Assert_False( ctx.Read( m_HasCode ), "[" + this + "] Failed reading m_HasCode" ) )
			return false;

		if ( m_ExpansionSaveVersion >= 20 )
		{
			if ( Expansion_Assert_False( ctx.Read( m_KnownUIDs ), "[" + this + "] Failed reading m_KnownUIDs" ) )
				return false;
		}

		return true;
	}

	#ifdef CF_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion];
		if (!ctx) return;

		ctx.Write( m_IsOpen );
		ctx.Write( m_Locked );
		ctx.Write( m_Code );
		ctx.Write( m_HasCode );

		ctx.Write( m_KnownUIDs.Count() );
		for ( int i = 0; i < m_KnownUIDs.Count(); i++ )
		{
			ctx.Write( m_KnownUIDs[i] );
		}
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion];
		if (!ctx) return true;

		if (!ctx.Read(m_IsOpen))
			return false;

		if (!ctx.Read(m_Locked))
			return false;

		if (!ctx.Read(m_Code))
			return false;

		if (!ctx.Read(m_HasCode))
			return false;

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

	// ------------------------------------------------------------
	// Override AfterStoreLoad
	// ------------------------------------------------------------
	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();

		if ( m_IsOpen )
		{
			GetInventory().UnlockInventory( HIDE_INV_FROM_SCRIPT );
			SetAnimationPhase( "safe_door", 1 );
		} else
		{
			SetAnimationPhase( "safe_door", 0 );
		}
	}

	// ------------------------------------------------------------
	// IsHeavyBehaviour
	// ------------------------------------------------------------
	override bool IsHeavyBehaviour() 
	{
		return true;
	}

	// ------------------------------------------------------------
	// OnPlacementComplete
	// ------------------------------------------------------------
	override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" )
	{
		if ( IsMissionHost() )
		{			
			SetPosition( position );
			SetOrientation( orientation );

			if( HasCode() && !IsLocked() && !IsOpened() )
				Lock();

			SetSynchDirty();
		}	
			
		SetIsPlaceSound( true );
	}

	// ------------------------------------------------------------
	// GetPlaceSoundset
	// ------------------------------------------------------------
	override string GetPlaceSoundset()
	{
		return "placeBarrel_SoundSet";
	}

	// ------------------------------------------------------------
	// CanPutInCargo
	// ------------------------------------------------------------
	override bool CanPutInCargo( EntityAI parent )
	{
		if ( !super.CanPutInCargo(parent) )
		{
			return false;
		}

		if ( GetNumberOfItems() == 0 && !IsOpened() && !IsLocked() )
		{
			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// CanPutIntoHands
	// ------------------------------------------------------------
	override bool CanPutIntoHands(EntityAI parent)
	{
		if ( !super.CanPutIntoHands( parent ) )
		{
			return false;
		}
		
		if ( GetNumberOfItems() == 0 && !IsOpened() && !IsLocked() )
		{
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// CanReceiveItemIntoCargo
	// ------------------------------------------------------------
	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		if ( IsOpened() )
			return super.CanReceiveItemIntoCargo( item );

		return false;
	}
	
	// ------------------------------------------------------------
	// CanReleaseCargo
	// ------------------------------------------------------------
	override bool CanReleaseCargo(EntityAI cargo)
	{
		return IsOpened();
	}

	// ------------------------------------------------------------
	// DamageItemInCargo
	// ------------------------------------------------------------
	override bool DamageItemInCargo(float damage)
	{
		return false;
	}

	// ------------------------------------------------------------
	// EEKilled
	// ------------------------------------------------------------
	override void EEKilled( Object killer )
	{
		bool canRaidSafes = GetExpansionSettings().GetRaid().CanRaidSafes;

		if ( !canRaidSafes )
			return;

		super.EEKilled( killer );
	}

	override void ExpansionOnDestroyed( Object killer )
	{
		SetCode("");  //! Will unlock as well
	}
}