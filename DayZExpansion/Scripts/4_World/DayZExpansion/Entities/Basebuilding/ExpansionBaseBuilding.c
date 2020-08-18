/**
 * ExpansionBaseBuilding.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionBaseBuilding
 * @brief		This class handle base class of expansion basebuilding	
 **/
class ExpansionBaseBuilding extends BaseBuildingBase
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
	//! Sounds instances
	// ------------------------------------------------------------
	protected EffectSound m_SoundGate_Start;
	protected EffectSound m_SoundGate_End;
	
	// ------------------------------------------------------------
	//! Local values
	// ------------------------------------------------------------
	protected bool m_Locked;
	protected string m_Code;
	protected bool m_HasCode;

	// ------------------------------------------------------------
	//! Constructor
	// ------------------------------------------------------------
	void ExpansionBaseBuilding()
	{
		RegisterNetSyncVariableBool( "m_Locked" );
		RegisterNetSyncVariableBool( "m_HasCode" );

		m_HasCode = false;
		m_Locked = false;
		m_Code = "";
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
		AddAction( ExpansionActionClose );
	}

	override void EEInit()
	{
		super.EEInit();

		UpdatePhysics();
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
	\brief Set code of wall
		\param 	
	*/
	override void SetCode( string code )
	{
		if (code == "")
		{
			m_HasCode = false;
		} 
		else
		{
			m_HasCode = true;
		}

		m_Code = code;
		m_Locked = false;

		SetSynchDirty();
	}
	
	/**
	\brief Returning code of wall
		\param 	
	*/
	override string GetCode()
	{
		return m_Code;
	}

	/**
	\brief Returning if the wall has a code
		\param 	
	*/
	override bool HasCode()
	{
		return m_HasCode;
	}
	
	/**
	\brief Returning if the wall has a code
		\param "selection" selection for codelock
	*/
	override bool HasCodeLock( string selection )
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
	override ExpansionCodeLock GetCodeLock()
	{
		return ExpansionCodeLock.Cast( FindAttachmentBySlotName( "Att_ExpansionCodeLock" ) );
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
		
		ref array<string> attachment_slots = new ref array<string>;
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

	/**
	\brief Opening gate on defined selection
		\param 	
	*/
	override void Open( string selection ) 
	{	
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			SoundGateOpenStart();
		}
	}
	
	/**
	\brief Closing gate on defined selection
		\param 	
	*/
	override void Close( string selection ) 
	{	
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			SoundGateCloseStart();
		}
	}
	
	/**
	\brief Locking base build
		\param 	
	*/
	override void Lock()
	{
		if ( m_HasCode )
		{
			m_Locked = true;
			SoundCodeLockLocked();
		}

		SetSynchDirty();
	}
	
	/**
	\brief Unlocking base build
		\param 	
	*/
	override void Unlock()
	{
		m_Locked = false;
		SoundCodeLockUnlocked();

		SetSynchDirty();
	}
	
	/**
	\brief Failed attempt to unlock base build
		\param 	
	*/
	override void FailedUnlock()
	{
		SoundCodeLockFailedUnlock();
	}

	/**
	\brief Can base building item be damaged
		\param 	
	*/
	bool CanBeDamaged()
	{
		return true;
	}

	/**
	\brief Saving class to storage
		\param 	
	*/
	override void OnStoreSave(ParamsWriteContext ctx)
	{
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
		if ( !super.OnStoreLoad( ctx, version ) )
			return false;
		
		if ( Expansion_Assert_False( ctx.Read( m_Locked ), "[" + this + "] Failed reading m_Locked" ) )
			return false;
		if ( Expansion_Assert_False( ctx.Read( m_Code ), "[" + this + "] Failed reading m_Code" ) )
			return false;
		if ( Expansion_Assert_False( ctx.Read( m_HasCode ), "[" + this + "] Failed reading m_HasCode" ) )
			return false;
		
		SetSynchDirty();

		return true;
	}

	// ------------------------------------------------------------
	//! Sounds
	// ------------------------------------------------------------
	protected void SoundGateOpenStart()
	{
		PlaySoundSet( m_SoundGate_Start, SOUND_GATE_OPEN_START, 0.1, 0.1 );
	}

	protected void SoundGateCloseStart()
	{
		PlaySoundSet( m_SoundGate_Start, SOUND_GATE_CLOSE_START, 0.1, 0.1 );
	}

	protected void SoundGateCloseEnd()
	{
		PlaySoundSet( m_SoundGate_End, SOUND_GATE_CLOSE_END, 0.1, 0.1 );
	}
	protected void SoundCodeLockLocked()
	{
		//PlaySoundSet( m_Sound, SOUND_GATE_LOCK, 0.1, 0.1 );
		//if ( GetGame().IsMultiplayer() && GetGame().IsClient() || !GetGame().IsMultiplayer() )
		//{
			//EffectSound sound = SEffectManager.PlaySound("Expansion_CodeLock_Locks_SoundSet", GetPosition());
			//sound.SetSoundAutodestroy( true );
		//}
		//m_Sound = SEffectManager.PlaySound("Expansion_CodeLock_Locks_SoundSet", GetPosition());
		//m_Sound.SetSoundAutodestroy( true );
	}
	
	protected void SoundCodeLockUnlocked()
	{
		//m_Sound = SEffectManager.PlaySound("Expansion_CodeLock_Unlock1_SoundSet", GetPosition());
		//m_Sound.SetSoundAutodestroy( true );
	}
	
	protected void SoundCodeLockFailedUnlock()
	{
		//m_Sound = SEffectManager.PlaySound("Expansion_Shocks_SoundSet", GetPosition());
		//m_Sound.SetSoundAutodestroy( true );
	}
	
	/**
	 * @param damageResult 
	 * @param source 
	 * @param component 
	 * @param dmgZone 
	 * @param ammo 
	 * @param modelPos 
	 * @param speedCoef 
	 *  
	 * TODO: Better linking to the vanilla damage system but still keep damage multipliers
	 */
	override void EEHitBy( TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef )
	{
		float explosionDamageMultiplier = GetExpansionSettings().GetRaid().ExplosionDamageMultiplier;
		float projectileDamageMultiplier = GetExpansionSettings().GetRaid().ProjectileDamageMultiplier;

		super.EEHitBy( damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef );

		PlayerBase player;
		string playerId;
		string playerName;

		string damageZone = dmgZone;
		if ( damageZone == "" )
			damageZone = "GlobalHealth";

		//if damage breaks, uncomment this line below, it will fix it.
		//damageZone = "";

		GetGame().AdminLog( "------------------------- Expansion BaseRaiding Damage Report -------------------------" );

		if ( damageType == DT_EXPLOSION ) 
		{
			float exposionBonusDamage;
			if ( explosionDamageMultiplier > 1)
			{
				exposionBonusDamage = ( damageResult.GetDamage( dmgZone, "Health" ) * ( explosionDamageMultiplier + 1 ) );

				if ( source && ( Class.CastTo( player, source ) || Class.CastTo( player, source.GetHierarchyRootPlayer() ) ) )
				{
					playerId = player.GetIdentityUID();
					playerName = player.GetIdentityName();

					GetGame().AdminLog( "BaseRaiding: Player \"" + playerName + "(ID = \"" + playerId + ")" + " damaged a base part (" + this.GetType() + ")" + "( " + (this.GetHealth() + damageResult.GetDamage( dmgZone, "Health" ) ) + "current health) " );
					GetGame().AdminLog( "BaseRaiding: They dealt "  + damageResult.GetDamage( dmgZone, "Health" ) + " * " + explosionDamageMultiplier + " = " + ( exposionBonusDamage + damageResult.GetDamage( dmgZone, "Health" ) ) + " damage with a " + source + " at " + this.GetPosition() );
				}

				AddHealth( damageZone, "Health", -exposionBonusDamage );
			} else if ( explosionDamageMultiplier < 1)
			{
				exposionBonusDamage = ( damageResult.GetDamage( dmgZone, "Health" ) * explosionDamageMultiplier );

				if ( source && ( Class.CastTo( player, source ) || Class.CastTo( player, source.GetHierarchyRootPlayer() ) ) )
				{
					playerId = player.GetIdentityUID();

					GetGame().AdminLog( "BaseRaiding: Player \"" + playerName + "\"" + "(ID = \"" + playerId + ")" + " damaged a base part (" + this.GetType() + ")" + "( " + (this.GetHealth() + damageResult.GetDamage( dmgZone, "Health" )) + " current health) " );
					GetGame().AdminLog( "BaseRaiding: They dealt "  + damageResult.GetDamage( dmgZone, "Health" ) + " * " + explosionDamageMultiplier + " = " + ( damageResult.GetDamage( dmgZone, "Health" ) - exposionBonusDamage ) + " damage with a " + source + " at " + this.GetPosition() );
				}
				if ( this.GetHealth() > 1)
					AddHealth( damageZone, "Health", ( damageResult.GetDamage( dmgZone, "Health" ) -exposionBonusDamage ) ); 
			} else 
			{
				if ( source && ( Class.CastTo( player, source ) || Class.CastTo( player, source.GetHierarchyRootPlayer() ) ) )
				{
					playerId = player.GetIdentityUID();

					GetGame().AdminLog( "BaseRaiding: Player \"" + playerName + "\"" + "(ID = \"" + playerId + ")" + " damaged a base part (" + this.GetType() + ")" + "( " + (this.GetHealth() + damageResult.GetDamage( dmgZone, "Health" ) ) + "current health) " );
					GetGame().AdminLog( "BaseRaiding: They dealt "  + damageResult.GetDamage( dmgZone, "Health" ) + " * " + explosionDamageMultiplier + " = " + ( damageResult.GetDamage( dmgZone, "Health" ) ) + " damage with a " + source + " at " + this.GetPosition() );
				}
			}
		} else if ( damageType == DT_FIRE_ARM ) 
		{
			float projectileBonusDamage;
			if ( projectileDamageMultiplier > 1 )
			{
				projectileBonusDamage = ( damageResult.GetDamage( dmgZone, "Health" ) * ( projectileDamageMultiplier + 1 ) );

				if ( source && ( Class.CastTo( player, source ) || Class.CastTo( player, source.GetHierarchyRootPlayer() ) ) )
				{
					playerId = player.GetIdentityUID();
					playerName = player.GetIdentityName();

					GetGame().AdminLog( "BaseRaiding: Player \"" + playerName + "(ID = \"" + playerId + ")" + " damaged a base part (" + this.GetType() + ")" + "( " + (this.GetHealth() + damageResult.GetDamage( dmgZone, "Health" ) ) + "current health) " );
					GetGame().AdminLog( "BaseRaiding: They dealt "  + damageResult.GetDamage( dmgZone, "Health" ) + " * " + projectileDamageMultiplier + " = " + ( exposionBonusDamage + damageResult.GetDamage( dmgZone, "Health" ) ) + " damage with a " + source+ " at " + this.GetPosition() );
				}
				
				AddHealth( damageZone, "Health", -projectileBonusDamage);
			} else if ( projectileDamageMultiplier < 1)
			{
				projectileBonusDamage = ( damageResult.GetDamage( dmgZone,"Health" ) * projectileDamageMultiplier );
				
				if ( source && ( Class.CastTo( player, source ) || Class.CastTo( player, source.GetHierarchyRootPlayer() ) ) )
				{
					playerId = player.GetIdentityUID();
					playerName = player.GetIdentityName();

					GetGame().AdminLog( "BaseRaiding: Player \"" + playerName + "(ID = \"" + playerId + ")" + " damaged a base part (" + this.GetType() + ")" + "( " + (this.GetHealth() + damageResult.GetDamage( dmgZone, "Health" ) ) + "current health) " );
					GetGame().AdminLog( "BaseRaiding: They dealt "  + damageResult.GetDamage( dmgZone, "Health" ) + " * " + projectileDamageMultiplier + " = " + ( exposionBonusDamage + damageResult.GetDamage( dmgZone, "Health" ) ) + " damage with a " + source+ " at " + this.GetPosition() );
				}
				if ( this.GetHealth() > 1)
					AddHealth( damageZone, "Health", ( damageResult.GetDamage( dmgZone,"Health" ) -projectileBonusDamage ) ); 
			} else
			{
				if ( source && ( Class.CastTo( player, source ) || Class.CastTo( player, source.GetHierarchyRootPlayer() ) ) )
				{
					playerId = player.GetIdentityUID();
					playerName = player.GetIdentityName();
					GetGame().AdminLog( "BaseRaiding: Player \"" + playerName + "(ID = \"" + playerId + ")" + " damaged a base part (" + this.GetType() + ")" + "( " + (this.GetHealth() + damageResult.GetDamage( dmgZone, "Health" ) ) + "current health) " );
					GetGame().AdminLog( "BaseRaiding: They dealt "  + damageResult.GetDamage( dmgZone, "Health" ) + " * " + projectileDamageMultiplier + " = " + ( damageResult.GetDamage( dmgZone, "Health" ) ) + " damage with a " + source + " at " + this.GetPosition() );
				}
			}
		}

		GetGame().AdminLog( "Expansion BaseRaiding: Health after damage applied: " + this.GetHealth() );
		GetGame().AdminLog( "--------------------------------------------------------------------------------------" );
	}
}