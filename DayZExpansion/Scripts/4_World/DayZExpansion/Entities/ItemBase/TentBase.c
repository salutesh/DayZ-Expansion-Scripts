/**
 * TentBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 * Special thanks to DaemonForge for his scripts.
 *
*/

modded class TentBase
{
	protected bool m_IsOpened = true;
	protected bool m_IsOpened1 = true;
	protected bool m_IsOpened2 = true;
	protected bool m_IsOpened3 = true;
	protected bool m_IsOpened4 = true;
	protected bool m_IsOpened5 = true;
	protected bool m_IsOpened6 = true;
	
	protected bool m_WasSynced;
	protected bool m_WasLocked;

	protected EffectSound m_Sound;

	void TentBase()
	{
		RegisterNetSyncVariableBool( "m_Locked" );
		RegisterNetSyncVariableBool( "m_HasCode" );
		RegisterNetSyncVariableInt( "m_CodeLength" );
		RegisterNetSyncVariableBool( "m_IsOpened" );
		RegisterNetSyncVariableBool( "m_IsOpened1" );
		RegisterNetSyncVariableBool( "m_IsOpened2" );
		RegisterNetSyncVariableBool( "m_IsOpened3" );
		RegisterNetSyncVariableBool( "m_IsOpened4" );
		RegisterNetSyncVariableBool( "m_IsOpened5" );
		RegisterNetSyncVariableBool( "m_IsOpened6" );
	}
	
	override void SetActions()
	{
		super.SetActions();

		AddAction( ExpansionActionEnterCodeLock );
		AddAction( ExpansionActionChangeCodeLock );
	}

	override void ToggleAnimation( string selection )
	{
		super.ToggleAnimation( selection );

		ToggleTentOpening( selection );
	}
	
	protected void ToggleTentOpening( string state )
	{
		bool wasOpened = m_IsOpened;

		//! toggleing an closed door so it would now be open
		if (state == "entrancec")
		{
			m_IsOpened = true;
		}
		//! toggleing an open door so it would now be closed
		else if (state == "entranceo")
		{
			m_IsOpened = false;
		}
		else //! Might Be Party Tent
		{
			//Party Tent Logic
			if ( state.Contains("door") )
			{
				if (state == "door1o"){      m_IsOpened1 = false; }
				else if (state == "door1c"){ m_IsOpened1 = true;  }
				else if (state == "door2o"){ m_IsOpened2 = false; }
				else if (state == "door2c"){ m_IsOpened2 = true;  }
				else if (state == "door3o"){ m_IsOpened3 = false; }
				else if (state == "door3c"){ m_IsOpened3 = true;  }
				else if (state == "door4o"){ m_IsOpened4 = false; }
				else if (state == "door4c"){ m_IsOpened4 = true;  }
				else if (state == "door5o"){ m_IsOpened5 = false; }
				else if (state == "door5c"){ m_IsOpened5 = true;  }
				else if (state == "door6o"){ m_IsOpened6 = false; }
				else if (state == "door6c"){ m_IsOpened6 = true;  }
				else { return; } //Wasn't Party Tent :) No need to continue any more :)
			} else { return; } //Wasn't Party Tent :) Probly a large tent window, no need to continue any more :)
			
			//! if any doors are open its now a closed door
			if ( m_IsOpened1 || m_IsOpened2 || m_IsOpened3 || m_IsOpened4 || m_IsOpened5 || m_IsOpened6 )
			{
				m_IsOpened = true;
			} else {
				m_IsOpened = false;
			}
		}

		
		if ( m_HasCode && m_IsOpened != wasOpened )
		{
			if ( m_IsOpened )
				Unlock();
			else
				Lock();
		} else
		{
			SetSynchDirty();
		}
	}

	//! Only call this after settings have been loaded
	bool CanAttachCodelock()
	{
		int attachMode = GetExpansionSettings().GetBaseBuilding().CanAttachCodelock;
		return attachMode == CodelockAttachMode.ExpansionAndTents || attachMode == CodelockAttachMode.ExpansionAndTentsAndFence;
	}
	
	override bool CanReceiveItemIntoCargo(EntityAI item )
	{
        if (m_Locked && GetExpansionSettings().GetBaseBuilding() )
		{
			if ( CanAttachCodelock() )
			{
           	 	return false;
			}
		}

        return super.CanReceiveItemIntoCargo(item );
    }


    override bool CanReleaseCargo(EntityAI cargo)
	{
        if ( m_Locked && GetExpansionSettings().GetBaseBuilding() )
		{
			if ( CanAttachCodelock() )
			{
           	 	return false;
			}
		}

        return super.CanReleaseCargo(cargo);
    }

    override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		if ( GetExpansionSettings().GetBaseBuilding() )
		{
			if ( attachment.IsInherited( ExpansionCodeLock ) )
			{
				if ( !CanAttachCodelock() )
					return false;

				//! Safety to prevent attaching Expansion Code Lock if another lock is already present in different slot (e.g. silver Code Lock from RoomService's mod)
				if ( FindAttachmentBySlotName( "Att_CombinationLock" ) )
					return false;

				if ( IsEntranceRuined() )
					return false;
			}

			//! Safety to prevent attaching other locks (e.g. silver Code Lock from RoomService's mod) if Expansion Code Lock is already present
			if ( attachment.IsKindOf( "CombinationLock" ) && ExpansionGetCodeLock() )
				return false;
		}

        return super.CanReceiveAttachment(attachment, slotId);
    }
	
	bool IsEntranceRuined()
	{
		TStringArray selections = new TStringArray;
		GetSelectionList( selections );

		foreach ( string selection : selections )
		{
			if ( selection.IndexOf( "door" ) == 0 || selection.IndexOf( "entrance" ) == 0 )
			{
				string zone;
				DamageSystem.GetDamageZoneFromComponentName( this, selection, zone );
				if ( GetHealthLevel( zone ) == GameConstants.STATE_RUINED )
					return true;
			}
		}

		return false;
	}
	
	override bool CanReleaseAttachment( EntityAI attachment )
	{
        if ( m_Locked && GetExpansionSettings().GetBaseBuilding() )
		{
			if ( CanAttachCodelock() )
			{
           	 	return false;
			}
		}

        return super.CanReleaseAttachment(attachment);
	}
	
	override bool IsOpened()
	{
		return m_IsOpened;
	}

	override bool ExpansionIsOpenable()
	{
		return true;
	}
	
	override bool ExpansionCanOpen( PlayerBase player, string selection )
	{
		return !m_IsOpened && ( !IsLocked() || IsKnownUser( player ) );
	}
	
	protected void SoundCodeLockLocked()
	{
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			m_Sound = SEffectManager.PlaySound("Expansion_CodeLock_Lock1_SoundSet", GetPosition());
			m_Sound.SetSoundAutodestroy( true );
		}
	}
	
	protected void SoundCodeLockUnlocked()
	{
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) // client side
		{
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

		ExpansionCodeLock codelock = ExpansionGetCodeLock();
		if ( codelock )
			codelock.UpdateVisuals();
	}
	
	override bool ExpansionHasCodeLock( string selection )
	{
		if ( ExpansionCodeLock.Cast(FindAttachmentBySlotName( "Att_ExpansionCodeLock" )) )
		{
			return true;
		}

		return false;
	}

	override ExpansionCodeLock ExpansionGetCodeLock()
	{
		return ExpansionCodeLock.Cast(FindAttachmentBySlotName( "Att_ExpansionCodeLock" ));
	}

	override bool IsLocked()
	{
		return m_HasCode && m_Locked;
	}
	
	override bool CanBePacked()
	{
		if ( IsLocked() )
		{
			return false;
		}

		return super.CanBePacked();
	}


	override void OnStoreSave(ParamsWriteContext ctx)
	{
		#ifdef CF_MODULE_MODSTORAGE
		if ( GetGame().SaveVersion() >= EXPANSION_VERSION_GAME_MODSTORAGE_TARGET )
		{
			super.OnStoreSave( ctx );
			return;
		}
		#endif

		super.OnStoreSave( ctx );
		
		ctx.Write( m_Locked );
		ctx.Write( m_Code );
		ctx.Write( m_HasCode );
		ctx.Write( m_IsOpened );
		ctx.Write( m_IsOpened1 );
		ctx.Write( m_IsOpened2 );
		ctx.Write( m_IsOpened3 );
		ctx.Write( m_IsOpened4 );
		ctx.Write( m_IsOpened5 );
		ctx.Write( m_IsOpened6 );
	}


	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( Expansion_Assert_False( super.OnStoreLoad( ctx, version ), "[" + this + "] Failed reading OnStoreLoad super" ) )
			return false;

		#ifdef CF_MODULE_MODSTORAGE
		if ( version > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET || m_ExpansionSaveVersion > EXPANSION_VERSION_SAVE_MODSTORAGE_TARGET )
			return true;
		#endif

		if ( m_ExpansionSaveVersion < 19 )
			return true;

		bool loadingsuccessfull = true;

		if ( Expansion_Assert_False( ctx.Read( m_Locked ) , "[" + this + "] Failed reading m_Locked" ))
		{
			loadingsuccessfull = false;
		}
		
		if ( Expansion_Assert_False( ctx.Read( m_Code ), "[" + this + "] Failed reading m_Code" ) )
		{
			loadingsuccessfull = false;
		}

		m_CodeLength = m_Code.Length();

		if ( Expansion_Assert_False( ctx.Read( m_HasCode ), "[" + this + "] Failed reading m_HasCode" ) )
		{
			loadingsuccessfull = false;
		}
		
		if ( Expansion_Assert_False( ctx.Read( m_IsOpened ), "[" + this + "] Failed reading m_IsOpened" ) )
		{
			m_IsOpened = true;
			loadingsuccessfull = false;
		}
		
		if ( Expansion_Assert_False( ctx.Read( m_IsOpened1 ), "[" + this + "] Failed reading m_IsOpened1" ) )
		{
			m_IsOpened1 = true;
			loadingsuccessfull = false;
		}
		
		if ( Expansion_Assert_False( ctx.Read( m_IsOpened2 ), "[" + this + "] Failed reading mm_IsOpened2_Locked" ) )
		{
			m_IsOpened2 = true;
			loadingsuccessfull = false;
		}
		
		if ( Expansion_Assert_False( ctx.Read( m_IsOpened3 ), "[" + this + "] Failed reading m_IsOpened3" ) )
		{
			m_IsOpened3 = true;
			loadingsuccessfull = false;
		}
		
		if ( Expansion_Assert_False( ctx.Read( m_IsOpened4 ), "[" + this + "] Failed reading m_IsOpened4" ) )
		{
			m_IsOpened4 = true;
			loadingsuccessfull = false;
		}
			
		if ( Expansion_Assert_False( ctx.Read( m_IsOpened5 ), "[" + this + "] Failed reading m_IsOpened5" ) )
		{
			m_IsOpened5 = true;
			loadingsuccessfull = false;
		}
		
		if ( Expansion_Assert_False( ctx.Read( m_IsOpened6 ), "[" + this + "] Failed reading m_IsOpened6" ) )
		{
			m_IsOpened6 = true;
			loadingsuccessfull = false;
		}
		
		//! If Code Locks on the tents it will remove them Just calling later so simplify and ensure that the code lock has been created
		if ( !CanAttachCodelock() )
		{
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.ExpansionCodeLockRemove, 1000, false );
		}
		
		return loadingsuccessfull;
	}

	#ifdef CF_MODULE_MODSTORAGE
	override void CF_OnStoreSave( CF_ModStorage storage, string modName )
	{
		super.CF_OnStoreSave( storage, modName );

		if ( modName != "DZ_Expansion" )
			return;

		storage.Write( m_Locked );
		storage.Write( m_Code );
		storage.Write( m_HasCode );
		storage.Write( m_IsOpened );
		storage.Write( m_IsOpened1 );
		storage.Write( m_IsOpened2 );
		storage.Write( m_IsOpened3 );
		storage.Write( m_IsOpened4 );
		storage.Write( m_IsOpened5 );
		storage.Write( m_IsOpened6 );
	}
	
	override bool CF_OnStoreLoad( CF_ModStorage storage, string modName )
	{
		if ( !super.CF_OnStoreLoad( storage, modName ) )
			return false;

		if ( modName != "DZ_Expansion" )
			return true;

		if ( storage.GetVersion() < 19 )
			return true;

		bool loadingsuccessfull = true;

		if ( Expansion_Assert_False( storage.Read( m_Locked ) , "[" + this + "] Failed reading m_Locked" ))
		{
			loadingsuccessfull = false;
		}
		
		if ( Expansion_Assert_False( storage.Read( m_Code ), "[" + this + "] Failed reading m_Code" ) )
		{
			loadingsuccessfull = false;
		}

		m_CodeLength = m_Code.Length();

		if ( Expansion_Assert_False( storage.Read( m_HasCode ), "[" + this + "] Failed reading m_HasCode" ) )
		{
			loadingsuccessfull = false;
		}
		
		if ( Expansion_Assert_False( storage.Read( m_IsOpened ), "[" + this + "] Failed reading m_IsOpened" ) )
		{
			m_IsOpened = true;
			loadingsuccessfull = false;
		}
		
		if ( Expansion_Assert_False( storage.Read( m_IsOpened1 ), "[" + this + "] Failed reading m_IsOpened1" ) )
		{
			m_IsOpened1 = true;
			loadingsuccessfull = false;
		}
		
		if ( Expansion_Assert_False( storage.Read( m_IsOpened2 ), "[" + this + "] Failed reading mm_IsOpened2_Locked" ) )
		{
			m_IsOpened2 = true;
			loadingsuccessfull = false;
		}
		
		if ( Expansion_Assert_False( storage.Read( m_IsOpened3 ), "[" + this + "] Failed reading m_IsOpened3" ) )
		{
			m_IsOpened3 = true;
			loadingsuccessfull = false;
		}
		
		if ( Expansion_Assert_False( storage.Read( m_IsOpened4 ), "[" + this + "] Failed reading m_IsOpened4" ) )
		{
			m_IsOpened4 = true;
			loadingsuccessfull = false;
		}
			
		if ( Expansion_Assert_False( storage.Read( m_IsOpened5 ), "[" + this + "] Failed reading m_IsOpened5" ) )
		{
			m_IsOpened5 = true;
			loadingsuccessfull = false;
		}
		
		if ( Expansion_Assert_False( storage.Read( m_IsOpened6 ), "[" + this + "] Failed reading m_IsOpened6" ) )
		{
			m_IsOpened6 = true;
			loadingsuccessfull = false;
		}
		
		//! If Code Locks on the tents it will remove them Just calling later so simplify and ensure that the code lock has been created
		if ( !CanAttachCodelock() )
		{
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.ExpansionCodeLockRemove, 1000, false );
		}
		
		return loadingsuccessfull;
	}
	#endif
	
	void ExpansionCodeLockRemove()
	{
		if ( !CanAttachCodelock() )
		{
			if ( m_Locked || m_HasCode || ExpansionHasCodeLock("codelock") )
			{
				ExpansionCodeLock codelock = ExpansionCodeLock.Cast(FindAttachmentBySlotName( "Att_ExpansionCodeLock" ));
				if (codelock)
				{
					codelock.Delete();
				}

				ExpansionCodeLock codelock2 = ExpansionCodeLock.Cast(GetAttachmentByConfigTypeName("ExpansionCodeLock"));
				if (codelock2)
				{
					codelock2.Delete();
				}

				SetCode( "" );
			}
		}
	}

	override bool CanDisplayAttachmentSlot( string slot_name )
	{
        if ( slot_name == "Att_ExpansionCodeLock" )
		{
			return CanAttachCodelock();
		}

		return super.CanDisplayAttachmentSlot( slot_name );
	}
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		
		if ( item && ( slot_name == "Att_ExpansionCodeLock" ) && HasCode() )
		{
			SetCode("");
		}
	}

	override void EEHitBy( TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef )
	{
		super.EEHitBy( damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef );

		ExpansionCodeLock codelock = ExpansionGetCodeLock();
		if ( codelock && IsEntranceRuined() )
			codelock.UnlockServer( null, this );
	}

	override void EEKilled( Object killer )
	{
		super.EEKilled( killer );

		ExpansionCodeLock codelock = ExpansionGetCodeLock();
		if ( codelock )
			codelock.UnlockServer( null, this );
	}
};
