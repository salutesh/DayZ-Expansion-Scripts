/**
 * ExpansionBaseBuilding.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionBaseBuilding
 * @brief		This class handle base class of expansion basebuilding	
 **/
modded class ExpansionBaseBuilding
{
	// ------------------------------------------------------------
	//! Static values
	// ------------------------------------------------------------
	const string SOUND_GATE_OPEN_START			= "DoorWoodTowerOpen_SoundSet";
	const string SOUND_GATE_CLOSE_START			= "DoorWoodTowerClose_start_SoundSet";
	const string SOUND_GATE_CLOSE_END			= "DoorWoodTowerClose_end_SoundSet";
	const string SOUND_GATE_LOCK 				= "Expansion_CodeLock_Locks_SoundSet";
	
	const float MAX_ACTION_DETECTION_ANGLE_RAD 	= 1.3;		//1.3 RAD = ~75 DEG
	const float MAX_ACTION_DETECTION_DISTANCE 	= 2.0;		//meters
	
	// ------------------------------------------------------------
	//! Local values
	// ------------------------------------------------------------
	protected bool m_WasSynced;
	protected bool m_WasLocked;

	protected string m_CurrentBuild;

	// ------------------------------------------------------------
	//! Constructor
	// ------------------------------------------------------------
	void ExpansionBaseBuilding()
	{
		RegisterNetSyncVariableBool( "m_Locked" );
		RegisterNetSyncVariableBool( "m_HasCode" );
		RegisterNetSyncVariableInt( "m_CodeLength" );
		
		SetAllowDamage(CanBeDamaged());
	}

	// ------------------------------------------------------------
	//! Destructor
	// ------------------------------------------------------------
	void ~ExpansionBaseBuilding()
	{
	}

	// ------------------------------------------------------------
	//! SetActions
	// ------------------------------------------------------------
	override void SetActions()
	{
		super.SetActions();

		AddAction( ActionFoldBaseBuildingObject );

		AddAction( ExpansionActionDamageBaseBuilding );

		AddAction( ExpansionActionOpen );

		if ( IsInherited( ExpansionWallBase ) )
		{
			//! Order matters. Have ExpansionActionEnterCodeLock AFTER ExpansionActionOpen
			//! so that "Open locked" shows on locked doors/gates without having to cycle through actions in the UI.
			AddAction( ExpansionActionEnterCodeLock );
			AddAction( ExpansionActionChangeCodeLock );
		}

		//! Order matters. Have ExpansionActionClose AFTER ExpansionActionEnterCodeLock
		//! so that "Lock" shows on opened doors/gates without having to cycle through actions in the UI.
		AddAction( ExpansionActionClose );
	}

	override void EEInit()
	{
		super.EEInit();
		
		UpdatePhysics();
	}

	override void EEKilled( Object killer )
	{
		if ( IsInherited( ExpansionCamoBox ) || IsInherited( ExpansionCamoTent ) )
		{
			super.EEKilled( killer );
		} else if ( !killer.IsInherited( CarScript ) )
		{
			super.EEKilled( killer );

			SetSynchDirty();
		}
	}

	bool ExpansionIsFacing( vector direction )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuilding " + this + " ExpansionIsFacing Start");
		#endif

		vector ref_dir = GetDirection();
		vector cam_dir = direction;
		
		ref_dir[1] = 0;
		ref_dir.Normalize();
		
		cam_dir[1] = 0;
		cam_dir.Normalize();
		
		float dot = vector.Dot( cam_dir, ref_dir );
		
		if ( dot >= 0 )
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionBaseBuilding " + this + " ExpansionIsFacing End True");
			#endif
			return true;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuilding " + this + " ExpansionIsFacing End False");
		#endif

		return false;
	}
	
	override bool CanBeRepairedToPristine()
	{
		return true;
	}
	
	override protected string GetDestroySound()
	{
		switch ( m_CurrentBuild )
		{
			case "wood": 	return SOUND_DISMANTLE_WOOD_LOG;  //! Vanilla soundset
			case "metal": 	return SOUND_DISMANTLE_METAL;  //! Vanilla soundset
			case "concrete":	return "";  //! TODO
		}
		
		return "";
	}
	
	override vector GetKitSpawnPosition()
	{
		if ( MemoryPointExists( "kit_spawn_position" ) )
		{
			vector position;
			position = GetMemoryPointPos( "kit_spawn_position" );

			return ModelToWorld( position );
		}

		return GetPosition();
	}	

	override bool IsFacingPlayer( PlayerBase player, string selection )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuilding " + this + " IsFacingPlayer Start");
		#endif

		vector fence_pos = GetPosition();
		vector player_pos = player.GetPosition();
		vector ref_dir = GetDirection();
		
		//vector fence_player_dir = player_pos - fence_pos;
		vector fence_player_dir = player.GetDirection();
		fence_player_dir.Normalize();
		fence_player_dir[1] = 0; //! Ignore height
		
		ref_dir.Normalize();
		ref_dir[1] = 0;	//! Ignore height
		
		if ( ref_dir.Length() != 0 )
		{
			float angle = Math.Acos( fence_player_dir * ref_dir );
			
			if ( angle >= MAX_ACTION_DETECTION_ANGLE_RAD )
			{
				#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuilding " + this + " IsFacingPlayer End True");
		#endif
				return true;
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuilding " + this + " IsFacingPlayer End False");
		#endif

		return false;
		// return ExpansionIsFacing( player.GetDirection() );
	}
		
	override bool IsFacingCamera( string selection )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuilding " + this + " IsFacingCamera Start");
		#endif

		vector ref_dir = GetDirection();
		vector cam_dir = GetGame().GetCurrentCameraDirection();
		
		//ref_dir = GetGame().GetCurrentCameraPosition() - GetPosition();
		ref_dir.Normalize();
		ref_dir[1] = 0;	//! Ignore height
		
		cam_dir.Normalize();
		cam_dir[1] = 0;	//! Ignore height
		
		if ( ref_dir.Length() != 0 )
		{
			float angle = Math.Acos( cam_dir * ref_dir );
			
			if ( angle >= MAX_ACTION_DETECTION_ANGLE_RAD )
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionBaseBuilding " + this + " IsFacingCamera End True");
				#endif
				return true;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuilding " + this + " IsFacingCamera End False");
		#endif

		return false;
		//return ExpansionIsFacing( GetGame().GetCurrentCameraDirection() );
	}
	
	override bool HasProperDistance( string selection, PlayerBase player )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuilding " + this + " HasProperDistance Start");
		#endif

		if ( MemoryPointExists( selection ) )
		{
			vector selection_pos = ModelToWorld( GetMemoryPointPos( selection ) );
			float distance = vector.Distance( selection_pos, player.GetPosition() );
			if ( distance >= MAX_ACTION_DETECTION_DISTANCE )
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionBaseBuilding " + this + " HasProperDistance End True");
				#endif
				return false;
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBaseBuilding " + this + " HasProperDistance End False");
		#endif
		
		return true;
	}

	protected void ShowSelectionProxy( string selection )
	{
		AddProxyPhysics( selection );
		ShowSelection( selection );
	}

	protected void HideSelectionProxy( string selection )
	{
		RemoveProxyPhysics( selection );
		HideSelection( selection );
	}
	
	/**
	\brief Returning if the wall has a code
		\param "selection" selection for codelock
	*/
	override bool ExpansionHasCodeLock( string selection )
	{
		if ( selection == "codelock" )
		{
			if ( FindAttachmentBySlotName( "Att_ExpansionCodeLock" ) )
			{
				return true;
			}
		}
		return false;
	}

	/**
	\brief Returning codelock entity
		\param
	*/
	override ExpansionCodeLock ExpansionGetCodeLock()
	{
		return ExpansionCodeLock.Cast( FindAttachmentBySlotName( "Att_ExpansionCodeLock" ) );
	}

	bool ExpansionGetCollisionBox( out vector minMax[2] )
	{
		return GetCollisionBox( minMax );
	}


	//! For some reasons doing this fixed the null pointer. It's 3AM I need to sleep, good luck guys <3
	//! -LieutenantMaster


	// avoid calling this function on frequent occasions, it's a massive performance hit
	override void UpdatePhysics()
	{
		//update attachments physics
		bsbDebugPrint("[bsb] " + GetDebugName(this) + " BaseBuildingBase::UpdatePhysics");
		#ifdef EXPANSIONEXPRINT
		EXPrint("[bsb] " + GetDebugName(this) + " BaseBuildingBase::UpdatePhysics");
		#endif
		
		array<string> attachment_slots = new array<string>;
		GetAttachmentSlots( this, attachment_slots );
		bsbDebugPrint("[bsb] " + GetDebugName(this) + " att_cnt=" + attachment_slots.Count());
		#ifdef EXPANSIONEXPRINT
		EXPrint("[bsb] " + GetDebugName(this) + " att_cnt=" + attachment_slots.Count());
		#endif
		for ( int i = 0; i < attachment_slots.Count(); i++ )
		{
			string slot_name = attachment_slots.Get( i );
			#ifdef EXPANSIONEXPRINT
			EXPrint("[bsb] " + slot_name + IsAttachmentSlotLocked( slot_name ) + "  LOOP");
			#endif
			UpdateAttachmentPhysics( slot_name, IsAttachmentSlotLocked( slot_name ) );
		}
		
		//check base
		if ( !HasBase() )
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("[bsb] " + GetDebugName(this) + ANIMATION_DEPLOYED + "  ADD");
			#endif
			bsbDebugPrint("[bsb] " + GetDebugName(this) + ANIMATION_DEPLOYED + "  ADD");
			AddProxyPhysics( ANIMATION_DEPLOYED );
		}
		else
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("[bsb] " + GetDebugName(this) + ANIMATION_DEPLOYED + " RM");
			#endif
			bsbDebugPrint("[bsb] " + GetDebugName(this) + ANIMATION_DEPLOYED + " RM");
			RemoveProxyPhysics( ANIMATION_DEPLOYED );
		}
		
		GetConstruction().UpdatePhysics();
		
		//regenerate navmesh
		UpdateNavmesh();
	}
	
	override void UpdateAttachmentPhysics( string slot_name, bool is_locked )
	{
		string slot_name_mounted = slot_name + "_Mounted";
		EntityAI attachment = FindAttachmentBySlotName( slot_name );
		
		//remove proxy physics
		bsbDebugPrint("[bsb] " + GetDebugName(this) + " Removing ATT SLOT=" + slot_name + " RM / RM");
		RemoveProxyPhysics( slot_name_mounted );
		RemoveProxyPhysics( slot_name );
		
		if ( attachment )
		{
			bsbDebugPrint("[bsb] " + GetDebugName(this) + " Adding ATT=" + Object.GetDebugName(attachment));
			if ( is_locked )
			{
				bsbDebugPrint("[bsb] " + GetDebugName(this) + " RM / RM");
				AddProxyPhysics( slot_name_mounted );
			}
			else
			{
				bsbDebugPrint("[bsb] " + GetDebugName(this) + " ADD");
				AddProxyPhysics( slot_name );
			}
		}
	}

	/**
	\brief Returning if wall is locked
		\param 	
	*/
	override bool IsLocked()
	{
		return m_HasCode && m_Locked;
	}

	bool IsPartBuilt( string part_name )
	{
		ConstructionPart construction_part = GetConstruction().GetConstructionParts().Get( part_name );

		if ( construction_part && construction_part.GetPartName() == part_name && construction_part.IsBuilt() )
			return true;

		return false;
	}

	override bool CanReceiveAttachment( EntityAI attachment, int slotId )
	{
		if ( !super.CanReceiveAttachment( attachment, slotId ) )
			return false;

		if (IsLastStage() && GetHealthLevel() == GameConstants.STATE_PRISTINE)
		{
			//! Do not allow to attach materials if fully built and not damaged

			string slot_name = InventorySlots.GetSlotName( slotId );

			switch (slot_name)
			{
				case "Material_Nails":
				case "Material_WoodenPlanks":
				case "Att_ExpansionLumber":
				case "Att_ExpansionLumberThird":
				case "Att_ExpansionLumberHalf":
					return false;
			}
		}

		return true;
	}

	/**
	\brief Opening gate on defined selection
		\param 	
	*/
	override void Open( string selection ) 
	{
		super.Open( selection );
	}
	
	/**
	\brief Closing gate on defined selection
		\param 	
	*/
	override void Close( string selection ) 
	{
		super.Close( selection );
	}

	override bool CanBeDamaged()
	{
		if (GetExpansionSettings().GetRaid().BaseBuildingRaidMode == 0)
		{
			return true;
		} else {
			return false;
		}
	}

	bool IsLastStage()
	{
		return false;
	}

	bool IsLastStageBuilt()
	{
		return false;
	}
	
	override bool CanObstruct()
	{
		return IsLastStage();
	}

	override void ExpansionOnDestroyed( Object killer )
	{
		if ( GetHealth() == 0 && !ToDelete() )
			Delete();
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

	/**
	\brief Saving class to storage
		\param 	
	*/
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

	/**
	\brief Loading class to storage
		\param 	
	*/
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( Expansion_Assert_False( super.OnStoreLoad( ctx, version ), "[" + this + "] Failed reading OnStoreLoad super" ) )
			return false;

		#ifdef CF_MODULE_MODSTORAGE
		if ( version > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET || m_ExpansionSaveVersion > EXPANSION_VERSION_SAVE_MODSTORAGE_TARGET )
			return true;
		#endif
		
		if ( Expansion_Assert_False( ctx.Read( m_Locked ), "[" + this + "] Failed reading m_Locked" ) )
			return false;
		if ( Expansion_Assert_False( ctx.Read( m_Code ), "[" + this + "] Failed reading m_Code" ) )
			return false;

		m_CodeLength = m_Code.Length();

		if ( Expansion_Assert_False( ctx.Read( m_HasCode ), "[" + this + "] Failed reading m_HasCode" ) )
			return false;

		return true;
	}

	#ifdef CF_MODULE_MODSTORAGE
	override void CF_OnStoreSave(map<string, CF_ModStorage> storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[ModStructure.DZ_Expansion];
		if (!ctx) return;

		ctx.Write(m_Locked);
		ctx.Write(m_Code);
		ctx.Write(m_HasCode);
	}
	
	override bool CF_OnStoreLoad(map<string, CF_ModStorage> storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[ModStructure.DZ_Expansion];
		if (!ctx) return true;

		if (!ctx.Read(m_Locked))
			return false;
		if (!ctx.Read(m_Code))
			return false;
		if (!ctx.Read(m_HasCode))
			return false;

		return true;
	}
	#endif

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();

		SetSynchDirty();
	}

	// ------------------------------------------------------------
	//! Sounds
	// ------------------------------------------------------------
	protected void SoundGateOpenStart()
	{
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			EffectSound sound = SEffectManager.PlaySound(SOUND_GATE_OPEN_START, GetPosition());
			sound.SetSoundAutodestroy( true );
		}
	}

	protected void SoundGateCloseStart()
	{
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			EffectSound sound = SEffectManager.PlaySound(SOUND_GATE_CLOSE_START, GetPosition());
			sound.SetSoundAutodestroy( true );
		}
	}

	protected void SoundGateCloseEnd()
	{
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			EffectSound sound = SEffectManager.PlaySound(SOUND_GATE_CLOSE_END, GetPosition());
			sound.SetSoundAutodestroy( true );
		}
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
}