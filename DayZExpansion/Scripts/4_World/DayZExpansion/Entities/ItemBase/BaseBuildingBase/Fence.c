/**
 * Fence.c
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

modded class Fence
{
	protected bool m_WasSynced;
	protected bool m_WasLocked;

	void Fence()
	{
		RegisterNetSyncVariableBool( "m_Locked" );
		RegisterNetSyncVariableBool( "m_HasCode" );
		RegisterNetSyncVariableInt( "m_CodeLength" );
	}
	
	override void SetActions()
	{
		super.SetActions();

		RemoveAction( ActionOpenFence );
		RemoveAction( ActionCloseFence );

		AddAction( ActionOpenFence );

		//! Order matters. Have ExpansionActionEnterCodeLock AFTER ActionOpenFence
		//! so that "Open locked" shows on locked doors/gates without having to cycle through actions in the UI.
		AddAction( ExpansionActionEnterCodeLock );
		AddAction( ExpansionActionChangeCodeLock );

		//! Order matters. Have ActionCloseFence AFTER ExpansionActionEnterCodeLock
		//! so that "Lock" shows on opened doors/gates without having to cycle through actions in the UI.
		AddAction( ActionCloseFence );
	}

	//! Only call this after settings have been loaded
	bool CanAttachCodelock()
	{
		int attachMode = GetExpansionSettings().GetBaseBuilding().CodelockAttachMode;
		return attachMode == ExpansionCodelockAttachMode.ExpansionAndFence || attachMode == ExpansionCodelockAttachMode.ExpansionAndFenceAndTents;
	}

    override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		if ( attachment.IsInherited( ExpansionCodeLock ) && GetExpansionSettings().GetBaseBuilding() )
		{
			if ( !CanAttachCodelock() )
				return false;
		}

        return super.CanReceiveAttachment(attachment, slotId);
    }

	override bool ExpansionIsOpenable()
	{
		return HasHinges();
	}
	
	override bool ExpansionCanOpen( PlayerBase player, string selection )
	{
		return ExpansionIsOpenable() && !IsOpened() && ( !IsLocked() || IsKnownUser( player ) );
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
		ExpansionCodeLock codelock = ExpansionCodeLock.Cast(FindAttachmentBySlotName( "Att_CombinationLock" )) ;

		//! check if attachment is code lock
		if ( codelock )
			return true;

		return super.ExpansionHasCodeLock( selection );
	}

	override ExpansionCodeLock ExpansionGetCodeLock()
	{
		return ExpansionCodeLock.Cast(FindAttachmentBySlotName( "Att_CombinationLock" ));
	}

	override bool IsLocked()
	{
		if ( m_Locked )
			return true;

		return super.IsLocked();
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

		if ( Expansion_Assert_False( ctx.Read( m_Locked ), "[" + this + "] Failed reading m_Locked" ) )
			return false;
		if ( Expansion_Assert_False( ctx.Read( m_Code ), "[" + this + "] Failed reading m_Code" ) )
			return false;

		m_CodeLength = m_Code.Length();

		if ( Expansion_Assert_False( ctx.Read( m_HasCode ), "[" + this + "] Failed reading m_HasCode" ) )
			return false;
		
		//! If Code Locks on the Fence it will remove them Just calling later so simplify and ensure that the code lock has been created
		if ( !CanAttachCodelock() )
		{
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.ExpansionCodeLockRemove, 1000, false );
		}
		
		return true;
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
	}
	
	override bool CF_OnStoreLoad( CF_ModStorage storage, string modName )
	{
		if ( !super.CF_OnStoreLoad( storage, modName ) )
			return false;

		if ( modName != "DZ_Expansion" )
			return true;

		if ( storage.GetVersion() < 19 )
			return true;

		if ( Expansion_Assert_False( storage.Read( m_Locked ), "[" + this + "] Failed reading m_Locked" ) )
			return false;
		if ( Expansion_Assert_False( storage.Read( m_Code ), "[" + this + "] Failed reading m_Code" ) )
			return false;

		m_CodeLength = m_Code.Length();

		if ( Expansion_Assert_False( storage.Read( m_HasCode ), "[" + this + "] Failed reading m_HasCode" ) )
			return false;
		
		//! If Code Locks on the Fence it will remove them Just calling later so simplify and ensure that the code lock has been created
		if ( !CanAttachCodelock() )
		{
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.ExpansionCodeLockRemove, 1000, false );
		}

		return true;
	}
	#endif
	
	void ExpansionCodeLockRemove()
	{
		if ( !CanAttachCodelock() )
		{
			if ( m_Locked || m_HasCode || ExpansionHasCodeLock("codelock") )
			{
				SetCode("");  //! Will unlock as well

				ExpansionCodeLock codelock = ExpansionCodeLock.Cast(FindAttachmentBySlotName( "Att_CombinationLock" ));
				if (codelock)
				{
					codelock.Delete();
				}

			}
		}
	}
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		
		if ( item && ( slot_name == "Att_CombinationLock" ) && HasCode() )
		{
			SetCode("");
		}
	}
	
	override void OnPartDismantledServer(notnull Man player, string part_name, int action_id)
	{
		ConstructionPart constrution_part = GetConstruction().GetConstructionPart(part_name);

        if ( constrution_part.IsGate() )
		{
            SetCode("");
        }

        super.OnPartDismantledServer(player, part_name, action_id);
    }
	
	override void OnPartDestroyedServer( Man player, string part_name, int action_id, bool destroyed_by_connected_part = false )
	{
		ConstructionPart constrution_part = GetConstruction().GetConstructionPart(part_name);

        if ( constrution_part.IsGate() )
		{
			SetCode("");  //! Will unlock as well

			if ( ExpansionHasCodeLock("") )
			{
			 	ExpansionCodeLock codelock = ExpansionCodeLock.Cast(FindAttachmentBySlotName( "Att_CombinationLock" ));

				if ( codelock )
				{
					float health = codelock.GetHealth("", "");
					codelock.AddHealth("", "Health", -health);
				}
			}
        }

		super.OnPartDestroyedServer( player, part_name, action_id );
	}

	override void OpenFence()
	{
		Unlock();

		super.OpenFence();
	}

	override void CloseAndLock( string selection )
	{
		CloseFence();

		Lock();
	}
};