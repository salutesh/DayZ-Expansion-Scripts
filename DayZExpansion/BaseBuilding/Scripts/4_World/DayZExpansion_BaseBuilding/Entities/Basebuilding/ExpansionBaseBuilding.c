/**
 * ExpansionBaseBuilding.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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

	protected string m_CurrentBuild;

	// ------------------------------------------------------------
	//! SetActions
	// ------------------------------------------------------------
	override void SetActions()
	{
		super.SetActions();

		AddAction( ActionFoldBaseBuildingObject );

		AddAction( ExpansionActionDamageBaseBuilding );
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BASEBUILDING, this, "ExpansionIsFacing");
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
			return true;
		}

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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BASEBUILDING, this, "IsFacingPlayer");
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
				return true;
			}
		}
		
		return false;
		// return ExpansionIsFacing( player.GetDirection() );
	}
		
	override bool IsFacingCamera( string selection )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BASEBUILDING, this, "IsFacingCamera");
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
				return true;
			}
		}

		return false;
		//return ExpansionIsFacing( GetGame().GetCurrentCameraDirection() );
	}
	
	override bool HasProperDistance( string selection, PlayerBase player )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BASEBUILDING, this, "HasProperDistance");
#endif

		if ( MemoryPointExists( selection ) )
		{
			vector selection_pos = ModelToWorld( GetMemoryPointPos( selection ) );
			float distance = vector.Distance( selection_pos, player.GetPosition() );
			if ( distance >= MAX_ACTION_DETECTION_DISTANCE )
			{
				return false;
			}
		}

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

	override void OnPartBuiltServer( notnull Man player, string part_name, int action_id )
	{
		super.OnPartBuiltServer(player, part_name, action_id );

		if (GetEconomyProfile())
		{
			//! Set max lifetime according to CE
			SetLifetimeMax(GetEconomyProfile().GetLifetime());
			MaxLifetimeRefreshCalc();
		}
	}

	//! For some reasons doing this fixed the null pointer. It's 3AM I need to sleep, good luck guys <3
	//! -LieutenantMaster


	// avoid calling this function on frequent occasions, it's a massive performance hit
	override void UpdatePhysics()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BASEBUILDING, this, "UpdatePhysics");
#endif

		
		array<string> attachment_slots = new array<string>;
		GetAttachmentSlots( this, attachment_slots );

		for ( int i = 0; i < attachment_slots.Count(); i++ )
		{
			string slot_name = attachment_slots.Get( i );

			UpdateAttachmentPhysics( slot_name, IsAttachmentSlotLocked( slot_name ) );
		}
		
		//check base
		if ( !HasBase() )
		{
			AddProxyPhysics( ANIMATION_DEPLOYED );
		}
		else
		{
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
		
		RemoveProxyPhysics( slot_name_mounted );
		RemoveProxyPhysics( slot_name );
		
		if ( attachment )
		{
			if ( is_locked )
			{
				AddProxyPhysics( slot_name_mounted );
			}
			else
			{
				AddProxyPhysics( slot_name );
			}
		}
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

	//! @note this overrides BaseBuildingBase::IsOpened and restores ItemBase::IsOpened
	override bool IsOpened()
	{
		return ExpansionIsOpened();
	}

	override bool CanBeDamaged()
	{
		auto settings = GetExpansionSettings().GetRaid();
		if (settings.BaseBuildingRaidMode == ExpansionBaseBuildingRaidMode.All)
			return settings.IsRaidableNow();

		return false;
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

		m_WasSynced = true;
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
		
		if ( ctx.GetVersion() >= 38 )
			return true;

		if (!ctx.Read(m_Locked))
			return false;
		if (!ctx.Read(m_Code))
			return false;

		m_CodeLength = m_Code.Length();

		bool hasCode;
		if (!ctx.Read(hasCode))
			return false;

		return true;
	}
	#endif

	override void AfterStoreLoad()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CE, this, "AfterStoreLoad");
#endif

		super.AfterStoreLoad();

		SetSynchDirty();
	}

	// ------------------------------------------------------------
	//! Sounds
	// ------------------------------------------------------------
	protected void SoundGateOpenStart()
	{
		if ( !GetGame().IsDedicatedServer() )
		{
			EffectSound sound = SEffectManager.PlaySound(SOUND_GATE_OPEN_START, GetPosition());
			sound.SetSoundAutodestroy( true );
		}
	}

	protected void SoundGateCloseStart()
	{
		if ( !GetGame().IsDedicatedServer() )
		{
			EffectSound sound = SEffectManager.PlaySound(SOUND_GATE_CLOSE_START, GetPosition());
			sound.SetSoundAutodestroy( true );
		}
	}

	protected void SoundGateCloseEnd()
	{
		if ( !GetGame().IsDedicatedServer() )
		{
			EffectSound sound = SEffectManager.PlaySound(SOUND_GATE_CLOSE_END, GetPosition());
			sound.SetSoundAutodestroy( true );
		}
	}
}