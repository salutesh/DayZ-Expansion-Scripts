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
	override void SetActions()
	{
		super.SetActions();

		AddAction( ExpansionActionEnterCodeLock );
		AddAction( ExpansionActionChangeCodeLock );
	}

	override void ToggleAnimation( string selection )
	{
		super.ToggleAnimation( selection );

		if ( HasCode() )
		{
			if ( ExpansionIsOpened() )
				Unlock();
			else
				ExpansionLock();
		}
	}

	//! Only call this after settings have been loaded
	bool ExpansionCanAttachCodeLock()
	{
		int attachMode = GetExpansionSettings().GetBaseBuilding().CodelockAttachMode;
		return attachMode == ExpansionCodelockAttachMode.ExpansionAndTents || attachMode == ExpansionCodelockAttachMode.ExpansionAndFenceAndTents;
	}
	
	override bool CanReceiveItemIntoCargo(EntityAI item )
	{
        if (ExpansionIsLocked() && GetExpansionSettings().GetBaseBuilding() )
		{
			if ( ExpansionCanAttachCodeLock() )
			{
           	 	return false;
			}
		}

        return super.CanReceiveItemIntoCargo(item );
    }


    override bool CanReleaseCargo(EntityAI cargo)
	{
        if ( ExpansionIsLocked() && GetExpansionSettings().GetBaseBuilding() )
		{
			if ( ExpansionCanAttachCodeLock() )
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
				if ( !ExpansionCanAttachCodeLock() )
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
        if ( ExpansionIsLocked() && GetExpansionSettings().GetBaseBuilding() )
		{
			if ( ExpansionCanAttachCodeLock() )
			{
           	 	return false;
			}
		}

        return super.CanReleaseAttachment(attachment);
	}
	
	override bool ExpansionIsOpened()
	{
		if (!m_OpeningMask)
			return true;

		foreach (ToggleAnimations toggle, bool state: m_ToggleAnimations)
		{
			string toggle_off = toggle.GetToggleOff();
			toggle_off.ToLower();

			bool is_closed = m_OpeningMask & toggle.GetOpeningBit();

			if (!is_closed && (toggle_off.Contains("entrance") || toggle_off.Contains("door")))
				return true;
		}

		return false;
	}

	override bool ExpansionIsOpenable()
	{
		return true;
	}

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
		
		//! TODO: After DayZ 1.16 and CF 1.4 release, do no longer write this unneeded data.
		//! Cannot bump Expansion save version right now because it would break storages that are using CF-Test
		if ( EXPANSION_VERSION_CURRENT_SAVE >= 40 )
			return;

		ctx.Write( false );
		ctx.Write( false );
		ctx.Write( false );
		ctx.Write( false );
		ctx.Write( false );
		ctx.Write( false );
		ctx.Write( false );
	}

	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( Expansion_Assert_False( super.OnStoreLoad( ctx, version ), "[" + this + "] Failed reading OnStoreLoad super" ) )
			return false;

		#ifdef CF_MODSTORAGE
		if ( version > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET || m_ExpansionSaveVersion > EXPANSION_VERSION_SAVE_MODSTORAGE_TARGET )
			return true;
		#endif

		if ( m_ExpansionSaveVersion < 19 )
			return true;

		bool loadingsuccessfull = true;

		if ( m_ExpansionSaveVersion < 38 )
		{
			if ( Expansion_Assert_False( ctx.Read( m_Locked ) , "[" + this + "] Failed reading m_Locked" ))
				loadingsuccessfull = false;
			
			if ( Expansion_Assert_False( ctx.Read( m_Code ), "[" + this + "] Failed reading m_Code" ) )
				loadingsuccessfull = false;

			m_CodeLength = m_Code.Length();

			bool hasCode;
			if ( Expansion_Assert_False( ctx.Read( hasCode ), "[" + this + "] Failed reading hasCode" ) )
				loadingsuccessfull = false;
		}
		
		
		if ( m_ExpansionSaveVersion < 40 )
		{
			bool isOpenedN;

			if ( Expansion_Assert_False( ctx.Read( isOpenedN ), "[" + this + "] Failed reading isOpened" ) )
				loadingsuccessfull = false;
			
			if ( Expansion_Assert_False( ctx.Read( isOpenedN ), "[" + this + "] Failed reading isOpened1" ) )
				loadingsuccessfull = false;
			
			if ( Expansion_Assert_False( ctx.Read( isOpenedN ), "[" + this + "] Failed reading isOpened2" ) )
				loadingsuccessfull = false;
			
			if ( Expansion_Assert_False( ctx.Read( isOpenedN ), "[" + this + "] Failed reading isOpened3" ) )
				loadingsuccessfull = false;
			
			if ( Expansion_Assert_False( ctx.Read( isOpenedN ), "[" + this + "] Failed reading isOpened4" ) )
				loadingsuccessfull = false;
				
			if ( Expansion_Assert_False( ctx.Read( isOpenedN ), "[" + this + "] Failed reading isOpened5" ) )
				loadingsuccessfull = false;
			
			if ( Expansion_Assert_False( ctx.Read( isOpenedN ), "[" + this + "] Failed reading isOpened6" ) )
				loadingsuccessfull = false;
		}
		
		//! If Code Locks on the tents it will remove them Just calling later so simplify and ensure that the code lock has been created
		if ( !ExpansionCanAttachCodeLock() )
		{
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.ExpansionCodeLockRemove, 1000, false );
		}
		
		return loadingsuccessfull;
	}

	#ifdef CF_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion];
		if (!ctx) return;
		
		//! TODO: After DayZ 1.16 and CF 1.4 release, do no longer write this unneeded data.
		//! Cannot bump Expansion save version right now because it would break storages that are using CF-Test
		if ( EXPANSION_VERSION_CURRENT_SAVE >= 40 )
			return;

		ctx.Write(false);
		ctx.Write(false);
		ctx.Write(false);
		ctx.Write(false);
		ctx.Write(false);
		ctx.Write(false);
		ctx.Write(false);
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion];
		if (!ctx) return true;

		if (ctx.GetVersion() < 38)
		{
			if (!ctx.Read(m_Locked))
				return false;
				
			if (!ctx.Read(m_Code))
				return false;

			m_CodeLength = m_Code.Length();

			bool hasCode;
			if (!ctx.Read(hasCode))
				return false;
		}

		if (ctx.GetVersion() < 40)
		{
			bool isOpenedN; //! 0..6

			if (!ctx.Read(isOpenedN))
				return false;

			if (!ctx.Read(isOpenedN))
				return false;
				
			if (!ctx.Read(isOpenedN))
				return false;

			if (!ctx.Read(isOpenedN))
				return false;

			if (!ctx.Read(isOpenedN))
				return false;

			if (!ctx.Read(isOpenedN))
				return false;

			if (!ctx.Read(isOpenedN))
				return false;
		}

		if (!ExpansionCanAttachCodeLock())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.ExpansionCodeLockRemove, 1000, false);
		
		return true;
	}
	#endif
	
	void ExpansionCodeLockRemove()
	{
		if ( !ExpansionCanAttachCodeLock() )
		{
			ExpansionCodeLock codelock = ExpansionGetCodeLock();
			if (codelock)
				codelock.Delete();
		}
	}

	override bool CanDisplayAttachmentSlot( string slot_name )
	{
        if ( slot_name == "Att_ExpansionCodeLock" )
		{
			return ExpansionCanAttachCodeLock();
		}

		return super.CanDisplayAttachmentSlot( slot_name );
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
