/**
 * ExpansionSafeBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSafeBase extends Container_Base
{
	protected EffectSound m_Sound;
	
	protected bool m_IsOpen;
	protected bool m_Locked;
	protected string m_Code;
	protected bool m_HasCode;

	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionSafeBase()
	{
		RegisterNetSyncVariableBool( "m_IsOpen" );
		RegisterNetSyncVariableBool( "m_Locked" );
		RegisterNetSyncVariableBool( "m_HasCode" );

		m_IsOpen = false;
		m_HasCode = false;
		m_Locked = false;
		m_Code = "";

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

		AddAction( ExpansionActionCloseSafeLock );
		AddAction( ExpansionActionCloseSafeUnlock );
		
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
		if ( IsLocked() )
			return false;
			
		if ( selection == "safe_door" && !IsOpened() )
			return true;
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion CanClose
	// ------------------------------------------------------------
	override bool CanClose( string selection )
	{
		if ( selection == "safe_door" && IsOpened() && !IsRuined())
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
	// Expansion ExpansionGetCodeLock
	// ------------------------------------------------------------
	override ExpansionCodeLock ExpansionGetCodeLock()
	{
		return ExpansionCodeLock.Cast(FindAttachmentBySlotName("Att_ExpansionCodeLock"));
	}

	// ------------------------------------------------------------
	// Expansion Open
	// ------------------------------------------------------------
	override void Open( string selection ) 
	{	
		m_IsOpen = true;

		//! Door open animation
		if ( selection == "safe_door" || selection == "codelock" )
			SetAnimationPhase( "safe_door", 1 );
		
		GetInventory().UnlockInventory(HIDE_INV_FROM_SCRIPT);
		SetSynchDirty();
		super.Open( selection );
	}
	
	// ------------------------------------------------------------
	// Expansion Close (And Lock)
	// ------------------------------------------------------------
	override void Close( string selection ) 
	{	
		m_IsOpen = false;

		//! Door open animation
		if (selection == "safe_door" || selection == "codelock")
			SetAnimationPhase( "safe_door", 0 );
		
		GetInventory().LockInventory(HIDE_INV_FROM_SCRIPT);
		SetSynchDirty();
		
		super.Close( selection );
		if ( m_HasCode )
			Lock();
	}

	// ------------------------------------------------------------
	// Expansion CloseUnlock
	// ------------------------------------------------------------
	void CloseUnlock( string selection ) 
	{	
		Print("ExpansionSafeBase::CloseUnlock: Selection: " + selection);

		m_IsOpen = false;

		//! Door open animation
		if (selection == "safe_door" || selection == "codelock")
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
	
	/**
	\brief Set code of safe
		\param 	
	*/
	override void SetCode( string code )
	{
		if ( code == "" )
		{
			m_HasCode = false;
		} else
		{
			m_HasCode = true;
		}

		m_Code = code;
		m_Locked = false;

		SetSynchDirty();
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
			
			GetInventory().LockInventory(HIDE_INV_FROM_SCRIPT);
			
			if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
			{
				m_Sound = SEffectManager.PlaySound("Expansion_CodeLock_Locks_SoundSet", GetPosition());
				m_Sound.SetSoundAutodestroy( true );
			}
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
		GetInventory().UnlockInventory(HIDE_INV_FROM_SCRIPT);
		Open("safe_door");

		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) // client side
		{
			m_Sound = SEffectManager.PlaySound("Expansion_CodeLock_Unlock_SoundSet", GetPosition());
			m_Sound.SetSoundAutodestroy( true );
		}
		
		SetSynchDirty();
	}
	
	/**
	\brief Failed attempt to unlock base build
		\param 	
	*/
	override void FailedUnlock()
	{
		string SOUND_CODE_DENIED = "";		

		if ( GetExpansionSettings().GetBaseBuilding().DoDamageWhenEnterWrongCodeLock )
		{
			SOUND_CODE_DENIED = "Expansion_Shocks_SoundSet";
		} else {
			SOUND_CODE_DENIED = "Expansion_Denied_SoundSet";
		}

		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) // client side
		{
			m_Sound = SEffectManager.PlaySound(SOUND_CODE_DENIED, GetPosition());
			m_Sound.SetSoundAutodestroy( true );
		}
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
	\brief Returning if the safe has a code
		\param 	
	*/
	override bool HasCode()
	{
		return m_HasCode;
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
		#ifdef CF_MOD_STORAGE
		if ( GetGame().SaveVersion() >= 116 )
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
	}

	/**
	\brief Loading class to storage
		\param 	
	*/
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		#ifdef CF_MOD_STORAGE
		if ( version >= 116 )
			return super.OnStoreLoad( ctx, version );
		#endif

		if ( !super.OnStoreLoad( ctx, version ) )
			return false;
		
		if ( Expansion_Assert_False( ctx.Read( m_IsOpen ), "[" + this + "] Failed reading m_IsOpen" ) )
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

	#ifdef CF_MOD_STORAGE
	override void OnModStoreSave( ModStorage storage, string modName )
	{
		super.OnModStoreSave( storage, modName );

		if ( modName != "DZ_Expansion" )
			return;
		
		storage.WriteBool( m_IsOpen );
		storage.WriteBool( m_Locked );
		storage.WriteString( m_Code );
		storage.WriteBool( m_HasCode );
	}
	
	override bool OnModStoreLoad( ModStorage storage, string modName )
	{
		if ( !super.OnModStoreLoad( storage, modName ) )
			return false;

		if ( modName != "DZ_Expansion" )
			return true;

		if ( Expansion_Assert_False( storage.ReadBool( m_IsOpen ), "[" + this + "] Failed reading m_IsOpen" ) )
			return false;
		if ( Expansion_Assert_False( storage.ReadBool( m_Locked ), "[" + this + "] Failed reading m_Locked" ) )
			return false;
		if ( Expansion_Assert_False( storage.ReadString( m_Code ), "[" + this + "] Failed reading m_Code" ) )
			return false;
		if ( Expansion_Assert_False( storage.ReadBool( m_HasCode ), "[" + this + "] Failed reading m_HasCode" ) )
			return false;

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
			SetAnimationPhase( "safe_door", 1 );
		} else
		{
			SetAnimationPhase( "safe_door", 0 );
		}
	}
	
	// ------------------------------------------------------------
	// IsContainer
	// ------------------------------------------------------------
	override bool IsContainer()
	{
		return true;
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
	#ifdef DAYZ_1_10
	override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" )
	#else
	override void OnPlacementComplete( Man player )
	#endif
	{
		if ( IsMissionHost() )
		{
			#ifndef DAYZ_1_10
			PlayerBase player_base = PlayerBase.Cast( player );
			vector position = player_base.GetLocalProjectionPosition();
			vector orientation = player_base.GetLocalProjectionOrientation();
			#endif
			
			SetPosition( position );
			SetOrientation( orientation );

			if( HasCode() && !IsLocked() )
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
	override bool CanReleaseCargo(EntityAI attachment)
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

		Unlock();

		super.EEKilled( killer );
	}

	// ------------------------------------------------------------
	// EEHitBy
	// ------------------------------------------------------------
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		float explosionDamageMultiplier = GetExpansionSettings().GetRaid().SafeExplosionDamageMultiplier; 
		float projectileDamageMultiplier = GetExpansionSettings().GetRaid().SafeProjectileDamageMultiplier; 
		
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);	
		PlayerBase player;
		GetGame().AdminLog("------------------------- Expansion BaseRaiding Damage Report -------------------------");
		if (damageType == 2) 
		{
			float exposionBonusDamage;
			if ( explosionDamageMultiplier > 1)
			{
				exposionBonusDamage = ( damageResult.GetDamage( dmgZone, "Health" ) * ( explosionDamageMultiplier + 1 ) );
				if ( source != NULL)
				{
					player = PlayerBase.Cast( source.GetHierarchyRootPlayer() );
					GetGame().AdminLog("Expansion BaseRaiding: Player \"" + player.GetIdentity().GetName() + "(ID = \"" + player.GetIdentity().GetId() + ")" + " damaged a base part (" + this.GetType() + ")" + "( " + (this.GetHealth() + damageResult.GetDamage( dmgZone, "Health" ) ) + "current health) " );
					GetGame().AdminLog("Expansion BaseRaiding: They dealt "  + damageResult.GetDamage( dmgZone, "Health" ) + " * " + explosionDamageMultiplier + " = " + ( exposionBonusDamage + damageResult.GetDamage( dmgZone, "Health" ) ) + " damage with a " + source + " at " + this.GetPosition() );
				}
				AddHealth("", "Health", -exposionBonusDamage);

			}

			else if ( explosionDamageMultiplier < 1)
			{
				exposionBonusDamage = ( damageResult.GetDamage( dmgZone, "Health" ) * explosionDamageMultiplier );

				player = PlayerBase.Cast(source.GetHierarchyRootPlayer() );
				GetGame().AdminLog("Expansion BaseRaiding: Player \"" + player.GetIdentity().GetName() + "\"" + "(ID = \"" + player.GetIdentity().GetId() + ")" + " damaged a base part (" + this.GetType() + ")" + "( " + (this.GetHealth() + damageResult.GetDamage( dmgZone, "Health" )) + " current health) "  );
				GetGame().AdminLog("Expansion BaseRaiding: They dealt "  + damageResult.GetDamage( dmgZone, "Health" ) + " * " + explosionDamageMultiplier + " = " + ( damageResult.GetDamage( dmgZone, "Health" ) - exposionBonusDamage ) + " damage with a " + source + " at " + this.GetPosition() );
				AddHealth("", "Health", ( damageResult.GetDamage( dmgZone, "Health" ) -exposionBonusDamage ) ); 

			}
			else 
			{
				player = PlayerBase.Cast( source.GetHierarchyRootPlayer() );
				GetGame().AdminLog("Expansion BaseRaiding: Player \"" + player.GetIdentity().GetName() + "\"" + "(ID = \"" + player.GetIdentity().GetId() + ")" + " damaged a base part (" + this.GetType() + ")" + "( " + (this.GetHealth() + damageResult.GetDamage( dmgZone, "Health" ) ) + "current health) " );
				GetGame().AdminLog("Expansion BaseRaiding: They dealt "  + damageResult.GetDamage( dmgZone, "Health" ) + " * " + explosionDamageMultiplier + " = " + ( damageResult.GetDamage( dmgZone, "Health" ) ) + " damage with a " + source + " at " + this.GetPosition());
			}
		}

		if (damageType == 1) 
		{
			float projectileBonusDamage;
			if ( projectileDamageMultiplier > 1)
			{
				projectileBonusDamage = ( damageResult.GetDamage( dmgZone, "Health" ) * ( projectileDamageMultiplier + 1 ) );
				player = PlayerBase.Cast( source.GetHierarchyRootPlayer() );
				GetGame().AdminLog("Expansion BaseRaiding: Player \"" + player.GetIdentity().GetName() + "(ID = \"" + player.GetIdentity().GetId() + ")" + " damaged a base part (" + this.GetType() + ")" + "( " + (this.GetHealth() + damageResult.GetDamage( dmgZone, "Health" ) ) + "current health) "  );
				GetGame().AdminLog("Expansion BaseRaiding: They dealt "  + damageResult.GetDamage( dmgZone, "Health" ) + " * " + projectileDamageMultiplier + " = " + ( exposionBonusDamage + damageResult.GetDamage( dmgZone, "Health" ) ) + " damage with a " + source+ " at " + this.GetPosition() );
				AddHealth("", "Health", -projectileBonusDamage);
				
			}
			else if ( projectileDamageMultiplier < 1)
			{
				projectileBonusDamage = ( damageResult.GetDamage( dmgZone,"Health" ) * projectileDamageMultiplier );
				player = PlayerBase.Cast( source.GetHierarchyRootPlayer() );
				GetGame().AdminLog("Expansion BaseRaiding: Player \"" + player.GetIdentity().GetName() + "(ID = \"" + player.GetIdentity().GetId() + ")" + " damaged a base part (" + this.GetType() + ")" + "( " + (this.GetHealth() + damageResult.GetDamage( dmgZone, "Health" ) ) + "current health) "  );
				GetGame().AdminLog("Expansion BaseRaiding: They dealt "  + damageResult.GetDamage( dmgZone, "Health" ) + " * " + projectileDamageMultiplier + " = " + ( exposionBonusDamage + damageResult.GetDamage( dmgZone, "Health" ) ) + " damage with a " + source+ " at " + this.GetPosition() );
				AddHealth("", "Health", ( damageResult.GetDamage( dmgZone,"Health" ) -projectileBonusDamage ) ); 
			}
			else
			{
				player = PlayerBase.Cast( source.GetHierarchyRootPlayer() );
				GetGame().AdminLog("Expansion BaseRaiding: Player \"" + player.GetIdentity().GetName() + "(ID = \"" + player.GetIdentity().GetId() + ")" + " damaged a base part (" + this.GetType() + ")" + "( " + (this.GetHealth() + damageResult.GetDamage( dmgZone, "Health" ) ) + "current health) "  );
				GetGame().AdminLog("Expansion BaseRaiding: They dealt "  + damageResult.GetDamage( dmgZone, "Health" ) + " * " + projectileDamageMultiplier + " = " + ( damageResult.GetDamage( dmgZone, "Health" ) ) + " damage with a " + source+ " at " + this.GetPosition() );
			}
			
		}
		GetGame().AdminLog("Expansion BaseRaiding: Health after damage applied: " + this.GetHealth());
		GetGame().AdminLog("--------------------------------------------------------------------------------------");
	}	
}