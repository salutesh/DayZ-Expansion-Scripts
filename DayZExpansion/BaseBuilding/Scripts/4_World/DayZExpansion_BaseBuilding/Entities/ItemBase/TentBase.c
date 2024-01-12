/**
 * TentBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
				ExpansionUnlock();
			else
				ExpansionLock();
		}
	}

	//! Only call this after settings have been loaded
	bool ExpansionCanAttachCodeLock()
	{
		if (!GetExpansionSettings().GetBaseBuilding(false).IsLoaded())
			return false;

		int attachMode = GetExpansionSettings().GetBaseBuilding().CodelockAttachMode;
		if (attachMode == ExpansionCodelockAttachMode.ExpansionAndTents || attachMode == ExpansionCodelockAttachMode.ExpansionAndFenceAndTents)
			return true;

		return false;
	}
	
	override bool CanReceiveItemIntoCargo(EntityAI item )
	{
        if (ExpansionIsLocked() )
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
        if ( ExpansionIsLocked() )
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
		if (!super.CanReceiveAttachment(attachment, slotId))
			return false;

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

        return true;
    }
	
	bool IsEntranceRuined()
	{
		TStringArray selections = new TStringArray;
		GetSelectionList( selections );

		foreach ( string selection : selections )
		{
			if ( selection.IndexOf( "door" ) == 0 || selection.IndexOf( "entrance" ) == 0 )
			{
				if (Expansion_IsSelectionRuined(selection))
					return true;
			}
		}

		return false;
	}
	
	override bool CanReleaseAttachment( EntityAI attachment )
	{
        if ( ExpansionIsLocked() )
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

			if (toggle_off.Contains("entrance") || toggle_off.Contains("door"))
			{
				bool is_closed = m_OpeningMask & toggle.GetOpeningBit();

				if (!is_closed || Expansion_IsSelectionRuined(toggle_off))
					return true;

			}
		}

		return false;
	}

	override bool ExpansionIsOpenable()
	{
		return true;
	}

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return;
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_BaseBuilding];
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

	override bool CanDisplayAttachmentSlot(int slot_id)
	{
		string slot_name = InventorySlots.GetSlotName(slot_id);

        if ( slot_name == "Att_ExpansionCodeLock" )
		{
			return ExpansionCanAttachCodeLock();
		}

		return super.CanDisplayAttachmentSlot(slot_id);
	}

	override bool CanDisplayCargo()
	{
		return !ExpansionIsLocked();
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
