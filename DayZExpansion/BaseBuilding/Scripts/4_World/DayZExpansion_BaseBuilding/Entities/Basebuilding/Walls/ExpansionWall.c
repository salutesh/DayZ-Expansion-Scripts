/**
 * ExpansionWallBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionWallBase: ExpansionBaseBuilding
{
	protected bool m_Expansion_IsOpened1;
	protected bool m_Expansion_IsOpened2;
	protected bool m_Expansion_IsOpened3;
	protected bool m_Expansion_IsOpened4;

	protected bool m_WasOpen;
	protected bool m_WasOpen1;
	protected bool m_WasOpen2;

	protected bool m_HasWindow;
	protected bool m_HasDoor;
	protected bool m_HasGate;
	protected bool m_HasWall;


	void ExpansionWallBase()
	{
		RegisterNetSyncVariableBool( "m_Expansion_IsOpened1" );
		RegisterNetSyncVariableBool( "m_Expansion_IsOpened2" );
		RegisterNetSyncVariableBool( "m_Expansion_IsOpened3" );
		RegisterNetSyncVariableBool( "m_Expansion_IsOpened4" );
		RegisterNetSyncVariableBool( "m_HasWindow" );
		RegisterNetSyncVariableBool( "m_HasDoor" );
		RegisterNetSyncVariableBool( "m_HasGate" );
		RegisterNetSyncVariableBool( "m_HasWall" );

		m_CurrentBuild = "wood";
	}

	override bool IsLastStage()
	{
		return m_HasDoor || m_HasGate || m_HasWindow || m_HasWall;
	}

	override bool IsLastStageBuilt()
	{
		return IsPartBuilt( m_CurrentBuild + "_wallfinished" ) || IsPartBuilt( m_CurrentBuild + "_wallfinished_half" ) || IsPartBuilt( m_CurrentBuild + "_wallfinished_third" );
	}

	override string GetConstructionKitType()
	{
		return "ExpansionWallKit";
	}

	override bool NameOverride(out string output)
	{
		if (IsLastStage())
			output = "#STR_EXPANSION_BB_" + m_CurrentBuild + "_WALL_FINISHED";
		else
			output = "#STR_EXPANSION_BB_" + m_CurrentBuild + "_WALL_BASE";
		return true;
	}

	override bool CanBeDamaged()
	{
		auto settings = GetExpansionSettings().GetRaid();
		if (settings.BaseBuildingRaidMode == ExpansionBaseBuildingRaidMode.DoorsGates)
		{
			return (m_HasDoor || m_HasGate) && settings.IsRaidableNow();
		}
		else if (settings.BaseBuildingRaidMode == ExpansionBaseBuildingRaidMode.DoorsGatesWindows)
		{
			return (m_HasDoor || m_HasGate || m_HasWindow) && settings.IsRaidableNow();
		}
		else if (settings.BaseBuildingRaidMode == ExpansionBaseBuildingRaidMode.DoorsGatesWindowsWalls)
		{
			return settings.IsRaidableNow();
		}
		
		return super.CanBeDamaged();
	}

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return;

		ctx.Write(m_HasWindow);
		ctx.Write(m_HasDoor);
		ctx.Write(m_HasGate);
		ctx.Write(m_HasWall);
		ctx.Write(m_Expansion_IsOpened);
		ctx.Write(m_Expansion_IsOpened1);
		ctx.Write(m_Expansion_IsOpened2);
		ctx.Write(m_Expansion_IsOpened3);
		ctx.Write(m_Expansion_IsOpened4);
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return true;

		if (!ctx.Read(m_HasWindow))
			return false;
			
		if (!ctx.Read(m_HasDoor))
			return false;

		if (!ctx.Read(m_HasGate))
			return false;

		if (!ctx.Read(m_HasWall))
			return false;

		if (!ctx.Read(m_Expansion_IsOpened))
			return false;

		if (!ctx.Read(m_Expansion_IsOpened1))
			return false;
			
		if (!ctx.Read(m_Expansion_IsOpened2))
			return false;

		if (!ctx.Read(m_Expansion_IsOpened3))
			return false;

		if (!ctx.Read(m_Expansion_IsOpened4))
			return false;

		return true;
	}
	#endif

	override bool HasBase()
	{
		return (GetConstruction().IsPartConstructed( "wood_base" ) || GetConstruction().IsPartConstructed( "wood_base_third" ) || GetConstruction().IsPartConstructed( "wood_base_half" ));
	}

	bool HasDoor()
	{
		return m_HasDoor;
	}

	bool HasGate()
	{
		return m_HasGate;
	}

	bool HasWindow()
	{
		return m_HasWindow;
	}

	override void OnVariablesSynchronized()
	{
		bool wasSynched = m_WasSynced;

		super.OnVariablesSynchronized();

		bool door_changed = m_HasDoor && m_WasOpen != m_Expansion_IsOpened;
		bool door_opened = door_changed && m_Expansion_IsOpened;

		bool gate_l_changed = m_HasGate && m_WasOpen1 != m_Expansion_IsOpened1;
		bool gate_l_opened = gate_l_changed && m_Expansion_IsOpened1;

		bool gate_r_changed = m_HasGate && m_WasOpen2 != m_Expansion_IsOpened2;
		bool gate_r_opened = gate_r_changed && m_Expansion_IsOpened2;

		if ( wasSynched && ( door_changed || gate_l_changed || gate_r_changed ) )
		{
			if ( door_opened || gate_l_opened || gate_r_opened )
				SoundGateOpenStart();
			else
				SoundGateCloseStart();
		}

		m_WasOpen = m_Expansion_IsOpened;
		m_WasOpen1 = m_Expansion_IsOpened1;
		m_WasOpen2 = m_Expansion_IsOpened2;

		UpdateVisuals();
	}
	
	override void AfterStoreLoad()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CE, this, "AfterStoreLoad");
#endif
	
		super.AfterStoreLoad();

		if ( m_ExpansionSaveVersion < 18 )
			m_HasWall = IsLastStageBuilt();
		
		UpdateVisuals();

		//! Restore state of opened windows/door/gate
		if ( m_Expansion_IsOpened || m_Expansion_IsOpened1 || m_Expansion_IsOpened2 || m_Expansion_IsOpened3 || m_Expansion_IsOpened4 )
			Open( "" );
	}

	override bool CanReleaseAttachment( EntityAI attachment )
	{
		if ( attachment.IsInherited( ExpansionCodeLock ) )
		{
			ExpansionCodeLock codelock = ExpansionCodeLock.Cast( attachment );
			if ( !codelock.ExpansionIsLocked() )
			{
				return true;
			}
		}

		return super.CanReleaseAttachment( attachment );
	}

	override bool ExpansionHasCodeLock( string selection )
	{
		if ( selection == "codelock_door" || GetExpansionSettings().GetBaseBuilding() && GetExpansionSettings().GetBaseBuilding().CodelockActionsAnywhere )
		{
			if ( m_HasDoor && FindAttachmentBySlotName( "Att_ExpansionCodeLock_1" ) )
			{
				return true;
			}
		}

		if ( selection == "codelock_gate" || GetExpansionSettings().GetBaseBuilding() && GetExpansionSettings().GetBaseBuilding().CodelockActionsAnywhere )
		{
			if ( m_HasGate && FindAttachmentBySlotName( "Att_ExpansionCodeLock_2" ) )
			{
				return true;
			}
		}

		return false;
	}

	override ExpansionCodeLock ExpansionGetCodeLock()
	{
		if (m_HasDoor)
			return ExpansionCodeLock.Cast(FindAttachmentBySlotName("Att_ExpansionCodeLock_1"));

		if (m_HasGate)
			return ExpansionCodeLock.Cast(FindAttachmentBySlotName("Att_ExpansionCodeLock_2"));

		return null;
	}

	override bool ExpansionGetCollisionBox( out vector minMax[2] )
	{
		minMax[0] = "-3 0 -0.125";
		minMax[1] = "3 3 0.125";
		return true;
	}

	override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}

	override bool IsPlayerInside( PlayerBase player, string selection )
	{
		if ( !IsLastStage() )
			return true;

		vector player_pos = player.GetPosition();
		vector wall_pos = GetPosition();

		//! Don't allow dismantling from below/above
		if (player_pos[1] >= wall_pos[1] + 3 || player_pos[1] + 3 <= wall_pos[1])
			return false;

		vector minMax[2];
		ExpansionGetCollisionBox(minMax);
		minMax[0][2] = -3;
		minMax[1][2] = 0;

		vector test_position = WorldToModel(player_pos);

		return Math.IsPointInRectangle(minMax[0], minMax[1], test_position);
	}

	override bool ExpansionIsOpenable()
	{
		return m_HasWindow || m_HasDoor || m_HasGate;
	}

	override bool ExpansionIsOpenable( string selection )
	{
		if ( m_HasWindow )
		{
			if ( selection == m_CurrentBuild + "_window_ll" )
				return true;
			if ( selection == m_CurrentBuild + "_window_lr" )
				return true;
			if ( selection == m_CurrentBuild + "_window_rl" )
				return true;
			if ( selection == m_CurrentBuild + "_window_rr" )
				return true;
		}
		else if ( m_HasDoor )
		{
			if ( selection == m_CurrentBuild + "_door" )
				return true;
		} else if ( m_HasGate )
		{
			if ( selection == m_CurrentBuild + "_gate_l" )
				return true;
			if ( selection == m_CurrentBuild + "_gate_r" )
				return true;
		}

		return false;
	}

	bool ExpansionHasGate()
	{
		return m_HasGate;
	}

	bool ExpansionHasDoor()
	{
		return m_HasDoor;
	}
	
	void ExpansionResetBaseBuildingState()
	{
		m_HasWindow = false;
		m_HasDoor = false;
		m_HasGate = false;
		m_HasWall = false;

		m_Expansion_IsOpened = false;
		m_Expansion_IsOpened1 = false;
		m_Expansion_IsOpened2 = false;
		m_Expansion_IsOpened3 = false;
		m_Expansion_IsOpened4 = false;
	}

	override void OnPartBuiltServer( notnull Man player, string part_name, int action_id )
	{
		ExpansionResetBaseBuildingState();

		ExpansionUpdateBaseBuildingStateFromPartBuilt( part_name );

		super.OnPartBuiltServer(player, part_name, action_id );

		UpdateVisuals();
	}

	override void ExpansionUpdateBaseBuildingStateFromPartBuilt( string part_name )
	{
		if ( part_name == m_CurrentBuild + "_windowfinished" )
		{
			m_HasWindow = true;
		}

		if ( part_name == m_CurrentBuild + "_doorfinished" )
		{
			m_HasDoor = true;
		}

		if ( part_name == m_CurrentBuild + "_gatefinished" )
		{
			m_HasGate = true;
		}

		if ( part_name == m_CurrentBuild + "_wallfinished" || part_name == m_CurrentBuild + "_wallfinished_third" || part_name == m_CurrentBuild + "_wallfinished_half" )
		{
			m_HasWall = true;
		}
	}

	override void OnPartDismantledServer( notnull Man player, string part_name, int action_id )
	{
		ExpansionCodeLock codelock = ExpansionGetCodeLock();
		if ( codelock )
			codelock.UnlockServer( player, this );

		ExpansionResetBaseBuildingState();

		super.OnPartDismantledServer( player, part_name, action_id );

		UpdateVisuals();
	}

	override void OnPartDestroyedServer( Man player, string part_name, int action_id, bool destroyed_by_connected_part = false )
	{
		ExpansionCodeLock codelock = ExpansionGetCodeLock();
		if ( codelock )
			codelock.UnlockServer( player, this );

		ExpansionResetBaseBuildingState();

		super.OnPartDestroyedServer( player, part_name, action_id, destroyed_by_connected_part );

		UpdateVisuals();
	}

	override void UpdateVisuals()
	{
		if ( m_HasWindow )
		{
			ShowSelectionProxy( m_CurrentBuild + "_window_ll" );
			ShowSelectionProxy( m_CurrentBuild + "_window_lr" );
			ShowSelectionProxy( m_CurrentBuild + "_window_rl" );
			ShowSelectionProxy( m_CurrentBuild + "_window_rr" );
		} else
		{
			HideSelectionProxy( m_CurrentBuild + "_window_ll" );
			HideSelectionProxy( m_CurrentBuild + "_window_lr" );
			HideSelectionProxy( m_CurrentBuild + "_window_rl" );
			HideSelectionProxy( m_CurrentBuild + "_window_rr" );
		}

		if ( m_HasDoor )
		{
			ShowSelectionProxy( m_CurrentBuild + "_door" );
		} else
		{
			HideSelectionProxy( m_CurrentBuild + "_door" );
		}

		if ( m_HasGate )
		{
			ShowSelectionProxy( m_CurrentBuild + "_gate_l" );
			ShowSelectionProxy( m_CurrentBuild + "_gate_r" );
		} else
		{
			HideSelectionProxy( m_CurrentBuild + "_gate_l" );
			HideSelectionProxy( m_CurrentBuild + "_gate_r" );
		}

		super.UpdateVisuals();
	}

	private bool ExpansionCanAcceptCodeLock( string slot_name )
	{
		if ( slot_name == "Att_ExpansionCodeLock_1" )
		{
			if ( !m_HasDoor )
			{
				return false;
			}
		}
		
		if ( slot_name == "Att_ExpansionCodeLock_2" )
		{
			if ( !m_HasGate )
			{
				return false;
			}
		}

		return true;
	}
	
	override bool CanDisplayAttachmentCategory( string category_name )
	{
		if ( !super.CanDisplayAttachmentCategory( category_name ) )
		{
			return false;
		}

		if ( category_name == "Material" )
		{
			if ( ExpansionHasCodeLock() && ( m_HasGate || m_HasDoor ) )
			{
				//! GetHealth cannot be called on client. Using GetHealthLevel instead
				return GetHealthLevel() != GameConstants.STATE_PRISTINE || GetInventory().AttachmentCount() > 1;
			} else {
				//! GetHealth cannot be called on client. Using GetHealthLevel instead
				return GetHealthLevel() != GameConstants.STATE_PRISTINE || !IsLastStage() || GetInventory().AttachmentCount() > 0;
			}
		}

		if ( category_name == "Attachments" )
		{
			if ( !( m_HasGate || m_HasDoor ) )
			{
				return false;
			}
		}

		return true;
	}

	override bool CanDisplayAttachmentSlot(int slot_id)
	{
		if (!super.CanDisplayAttachmentSlot(slot_id))
		{
			return false;
		}

		string slot_name = InventorySlots.GetSlotName(slot_id);

		if ( !ExpansionCanAcceptCodeLock( slot_name ) )
		{
			return false;
		}
		
		return true;
	}

	override bool CanReceiveAttachment( EntityAI attachment, int slotId )
	{
		if ( !super.CanReceiveAttachment( attachment, slotId ) )
		{
			return false;
		}

		string slot_name = InventorySlots.GetSlotName( slotId );

		if ( !ExpansionCanAcceptCodeLock( slot_name ) )
		{
			return false;
		}
		
		return true;
	}
	
	override bool ExpansionCanOpen( PlayerBase player, string selection )
	{
		if ( !ExpansionIsOpenable() )
			return false;

		if ( m_HasWindow && !IsFacingPlayer( player, selection ) && ( !IsMissionClient() || !IsFacingCamera( selection ) ) )
		{
			if ( selection == (m_CurrentBuild + "_window_ll") && GetAnimationPhase( m_CurrentBuild + "_window_ll_rotate" ) < 0.5 )
				return true;
			if ( selection == (m_CurrentBuild + "_window_lr") && GetAnimationPhase( m_CurrentBuild + "_window_lr_rotate" ) < 0.5 )
				return true;
			if ( selection == (m_CurrentBuild + "_window_rl") && GetAnimationPhase( m_CurrentBuild + "_window_rl_rotate" ) < 0.5 )
				return true;
			if ( selection == (m_CurrentBuild + "_window_rr") && GetAnimationPhase( m_CurrentBuild + "_window_rr_rotate" ) < 0.5 )
				return true;
		}

		if ( m_HasDoor && ( !ExpansionIsLocked() || IsKnownUser( player ) ) )
		{
			if ( selection == (m_CurrentBuild + "_door") && GetAnimationPhase( m_CurrentBuild + "_door_rotate" ) < 0.5 )
				return true;
		}

		if ( m_HasGate && ( !ExpansionIsLocked() || IsKnownUser( player ) ) )
		{
			if ( selection == (m_CurrentBuild + "_gate_l") && GetAnimationPhase( m_CurrentBuild +"_gate_l_rotate" ) < 0.5 )
				return true;
			if ( selection == (m_CurrentBuild + "_gate_r") && GetAnimationPhase( m_CurrentBuild + "_gate_r_rotate" ) < 0.5 )
				return true;
		}

		return false;
	}
	
	override bool ExpansionCanClose( PlayerBase player, string selection )
	{
		return CanClose( selection );
	}

	override bool CanClose( string selection )
	{
		if ( !ExpansionIsOpenable() )
			return false;

		if ( m_HasWindow )
		{
			if ( selection == (m_CurrentBuild + "_window_ll") && GetAnimationPhase( m_CurrentBuild + "_window_ll_rotate" ) > 0.5 )
				return true;
			if ( selection == (m_CurrentBuild + "_window_lr") && GetAnimationPhase( m_CurrentBuild + "_window_lr_rotate" ) > 0.5 )
				return true;
			if ( selection == (m_CurrentBuild + "_window_rl") && GetAnimationPhase( m_CurrentBuild + "_window_rl_rotate" ) > 0.5 )
				return true;
			if ( selection == (m_CurrentBuild + "_window_rr") && GetAnimationPhase( m_CurrentBuild + "_window_rr_rotate" ) > 0.5 )
				return true;
		}

		if ( m_HasDoor )
		{
			if ( selection == (m_CurrentBuild + "_door") && GetAnimationPhase( m_CurrentBuild + "_door_rotate" ) > 0.5 )
				return true;
		}

		if ( m_HasGate )
		{
			if ( selection == (m_CurrentBuild + "_gate_l") && GetAnimationPhase( m_CurrentBuild +"_gate_l_rotate" ) > 0.5 )
				return true;
			if ( selection == (m_CurrentBuild + "_gate_r") && GetAnimationPhase( m_CurrentBuild + "_gate_r_rotate" ) > 0.5 )
				return true;
		}

		return false;
	}
	
	override void Open( string selection ) 
	{	
		if ( IsMissionHost() && m_HasWindow )
		{
			if (selection == (m_CurrentBuild + "_window_ll") || m_Expansion_IsOpened1)
			{
				m_Expansion_IsOpened1 = true;
				SetAnimationPhase( m_CurrentBuild + "_window_ll_rotate", 1 );
			}
			if (selection == (m_CurrentBuild + "_window_lr") || m_Expansion_IsOpened2)
			{
				m_Expansion_IsOpened2 = true;
				SetAnimationPhase( m_CurrentBuild + "_window_lr_rotate", 1 );
			}
			if (selection == (m_CurrentBuild + "_window_rl") || m_Expansion_IsOpened3)
			{
				m_Expansion_IsOpened3 = true;
				SetAnimationPhase( m_CurrentBuild + "_window_rl_rotate", 1 );
			}
			if (selection == (m_CurrentBuild + "_window_rr") || m_Expansion_IsOpened4)
			{
				m_Expansion_IsOpened4 = true;
				SetAnimationPhase( m_CurrentBuild + "_window_rr_rotate", 1 );
			}

			m_Expansion_IsOpened = true;
		}

		if ( IsMissionHost() && m_HasDoor )
		{
			if (selection == (m_CurrentBuild + "_door") || m_Expansion_IsOpened)
				SetAnimationPhase( m_CurrentBuild + "_door_rotate", 1 );

			m_Expansion_IsOpened = true;
		}

		if ( IsMissionHost() && m_HasGate )
		{
			if (selection == (m_CurrentBuild + "_gate_l") || m_Expansion_IsOpened1)
			{
				m_Expansion_IsOpened1 = true;
				SetAnimationPhase( m_CurrentBuild + "_gate_l_rotate", 1 );
			}
			if (selection == (m_CurrentBuild + "_gate_r") || m_Expansion_IsOpened2)
			{
				m_Expansion_IsOpened2 = true;
				SetAnimationPhase( m_CurrentBuild + "_gate_r_rotate", 1 );
			}

			m_Expansion_IsOpened = true;
		}

		SetSynchDirty();
	}
	
	override void Close( string selection ) 
	{	
		if ( GetGame().IsServer() && m_HasWindow )
		{
			if (selection == (m_CurrentBuild + "_window_ll"))
			{
				m_Expansion_IsOpened1 = false;
				SetAnimationPhase( m_CurrentBuild + "_window_ll_rotate", 0 );
			}
			if (selection == (m_CurrentBuild + "_window_lr"))
			{
				m_Expansion_IsOpened2 = false;
				SetAnimationPhase( m_CurrentBuild + "_window_lr_rotate", 0 );
			}
			if (selection == (m_CurrentBuild + "_window_rl"))
			{
				m_Expansion_IsOpened3 = false;
				SetAnimationPhase( m_CurrentBuild+ "_window_rl_rotate", 0 );
			}
			if (selection == (m_CurrentBuild + "_window_rr"))
			{
				m_Expansion_IsOpened4 = false;
				SetAnimationPhase( m_CurrentBuild + "_window_rr_rotate", 0 );
			}

			m_Expansion_IsOpened = false;
			m_Expansion_IsOpened = m_Expansion_IsOpened || GetAnimationPhase( m_CurrentBuild + "_window_ll_rotate" ) < 0.5;
			m_Expansion_IsOpened = m_Expansion_IsOpened || GetAnimationPhase( m_CurrentBuild + "_window_lr_rotate" ) < 0.5;
			m_Expansion_IsOpened = m_Expansion_IsOpened || GetAnimationPhase( m_CurrentBuild + "_window_rl_rotate" ) < 0.5;
			m_Expansion_IsOpened = m_Expansion_IsOpened || GetAnimationPhase( m_CurrentBuild + "_window_rr_rotate" ) < 0.5;
		}

		if ( IsMissionHost() && m_HasDoor )
		{
			if (selection == (m_CurrentBuild + "_door"))
				SetAnimationPhase( m_CurrentBuild + "_door_rotate", 0 );

			m_Expansion_IsOpened = GetAnimationPhase( m_CurrentBuild + "_door_rotate" ) < 0.5;
		}

		if ( IsMissionHost() && m_HasGate )
		{
			if (selection == (m_CurrentBuild + "_gate_l"))
			{
				m_Expansion_IsOpened1 = false;
				SetAnimationPhase( m_CurrentBuild + "_gate_l_rotate", 0 );
			}
			if (selection == (m_CurrentBuild + "_gate_r"))
			{
				m_Expansion_IsOpened2 = false;
				SetAnimationPhase( m_CurrentBuild + "_gate_r_rotate", 0 );
			}

			m_Expansion_IsOpened = GetAnimationPhase( m_CurrentBuild + "_gate_l_rotate" ) < 0.5 && GetAnimationPhase( m_CurrentBuild + "_gate_r_rotate" ) < 0.5;
		}
		
		SetSynchDirty();
	}

	override void UnlockAndOpen( string selection ) 
	{
		super.ExpansionUnlock();

		if ( m_HasDoor )
		{
			Open( m_CurrentBuild + "_door" );
		} else if ( m_HasGate )
		{
			Open( m_CurrentBuild + "_gate_l" );
			Open( m_CurrentBuild + "_gate_r" );
		}
	}

	override void CloseAndLock( string selection )
	{
		if ( m_HasDoor && ExpansionIsOpened() )
		{
			Close( m_CurrentBuild + "_door" );
		} else if ( m_HasGate )
		{
			if ( GetAnimationPhase( m_CurrentBuild + "_gate_l_rotate" ) > 0.5 )
				Close( m_CurrentBuild + "_gate_l" );
			if ( GetAnimationPhase( m_CurrentBuild + "_gate_r_rotate" ) > 0.5 )
				Close( m_CurrentBuild + "_gate_r" );
		}

		ExpansionLock();
	}
}
