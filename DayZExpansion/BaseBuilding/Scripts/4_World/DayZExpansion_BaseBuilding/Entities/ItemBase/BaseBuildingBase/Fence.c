/**
 * Fence.c
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

modded class Fence
{
	override bool CanBeDamaged()
	{
		auto settings = GetExpansionSettings().GetRaid();
		if (settings.BaseBuildingRaidMode == ExpansionBaseBuildingRaidMode.DoorsGates || settings.BaseBuildingRaidMode == ExpansionBaseBuildingRaidMode.DoorsGatesWindowsWalls)
			return HasHinges() && settings.IsRaidableNow();
		else if (settings.BaseBuildingRaidMode == ExpansionBaseBuildingRaidMode.All)
			return settings.IsRaidableNow();
		
		return false;
	}

	override void AddAction(typename actionName)
	{
		super.AddAction(actionName);

		if (actionName == ActionOpenFence)
		{
			//! Order matters. Have ExpansionActionEnterCodeLock AFTER ActionOpenFence
			//! so that "Open locked" shows on locked doors/gates without having to cycle through actions in the UI.
			AddAction(ExpansionActionEnterCodeLock);
			AddAction(ExpansionActionChangeCodeLock);
		}

		//! Order matters. Have ActionCloseFence AFTER ExpansionActionEnterCodeLock
		//! so that "Lock" shows on opened doors/gates without having to cycle through actions in the UI.
	}

	override void RemoveAction(typename actionName)
	{
		super.RemoveAction(actionName);

		if (actionName == ActionOpenFence)
		{
			RemoveAction(ExpansionActionEnterCodeLock);
			RemoveAction(ExpansionActionChangeCodeLock);
		}
	}

	//! Only call this after settings have been loaded
	bool ExpansionCanAttachCodeLock()
	{
		int attachMode = GetExpansionSettings().GetBaseBuilding().CodelockAttachMode;
		return attachMode == ExpansionCodelockAttachMode.ExpansionAndFence || attachMode == ExpansionCodelockAttachMode.ExpansionAndFenceAndTents;
	}

    override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		if ( GetExpansionSettings().GetBaseBuilding() )
		{
			if ( attachment.IsInherited( ExpansionCodeLock ) )
			{
				if ( !ExpansionCanAttachCodeLock() )
					return false;

				//! Safety to prevent attaching Expansion Code Lock if another lock is already present (e.g. silver Code Lock from RoomService's mod)
				if ( FindAttachmentBySlotName( "Att_CombinationLock" ) )
					return false;
			}

			//! Safety to prevent attaching other locks (e.g. silver Code Lock from RoomService's mod) if Expansion Code Lock is already present
			if ( attachment.IsKindOf( "CombinationLock" ) && ExpansionGetCodeLock() )
				return false;
		}

        return super.CanReceiveAttachment(attachment, slotId);
    }

	override bool ExpansionIsOpenable()
	{
		return HasHinges();
	}

	override ExpansionCodeLock ExpansionGetCodeLock()
	{
		return ExpansionCodeLock.Cast(FindAttachmentBySlotName( "Att_CombinationLock" ));
	}

	//! @note vanilla fence overrides modded ItemBase::IsLocked, so super calls Fence::IsLocked
	override bool ExpansionIsLocked()
	{
		return super.IsLocked() || super.ExpansionIsLocked();
	}

	override bool CanOpenFence()
	{
		if (!super.CanOpenFence())
			return false;

		if (HasHinges() && !IsOpened() && !ExpansionIsLocked())
			return true;
		
		return false;
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
	
	override void OnPartDismantledServer(notnull Man player, string part_name, int action_id)
	{
		ConstructionPart constrution_part = GetConstruction().GetConstructionPart(part_name);

        if ( constrution_part.IsGate() )
		{
			ExpansionCodeLock codelock = ExpansionGetCodeLock();
			if ( codelock )
				codelock.UnlockServer( player, this );
        }

        super.OnPartDismantledServer(player, part_name, action_id);
    }
	
	override void OnPartDestroyedServer( Man player, string part_name, int action_id, bool destroyed_by_connected_part = false )
	{
		ConstructionPart constrution_part = GetConstruction().GetConstructionPart(part_name);

        if ( constrution_part.IsGate() )
		{
			ExpansionCodeLock codelock = ExpansionGetCodeLock();
			if ( codelock )
				codelock.UnlockServer( player, this );
        }

		super.OnPartDestroyedServer( player, part_name, action_id );
	}

	override void SetOpenedState( bool state )
	{
		if ( state )
			ExpansionUnlock();

		super.SetOpenedState( state );
	}

	override void CloseAndLock( string selection )
	{
		CloseFence();

		ExpansionLock();
	}
};