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
		AddAction( ExpansionActionClose );
		
		AddAction( ExpansionActionEnterSafeLock );
	}

	// ------------------------------------------------------------
	// CanReceiveItemIntoCargo
	// ------------------------------------------------------------
	override bool CanReceiveItemIntoCargo(EntityAI cargo)
	{
		return IsOpened();
	}
	
	// ------------------------------------------------------------
	// CanReleaseCargo
	// ------------------------------------------------------------
	override bool CanReleaseCargo(EntityAI attachment)
	{
		return IsOpened();
	}

	// ------------------------------------------------------------
	// Expansion IsOpenable
	// ------------------------------------------------------------
	override bool IsOpenable()
	{
		return true;
	}
	
	// ------------------------------------------------------------
	// Expansion CanOpen
	// ------------------------------------------------------------
	override bool CanOpen( PlayerBase player, string selection )
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
	// Expansion HasCodeLock
	// ------------------------------------------------------------
	override bool HasCodeLock( string selection )
	{
		return (selection == "codelock");
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
	// Expansion Close
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
	}

	// ------------------------------------------------------------
	// Expansion IsOpened
	// ------------------------------------------------------------
	override bool IsOpened()
	{
		return m_IsOpen;
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
	\brief Set code of wall
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
		if ( !super.OnStoreLoad( ctx, version ) )
		{
			return false;
		}
		
		if ( !ctx.Read( m_IsOpen ) )
		{
			return false;
		} else
		{
			if ( m_IsOpen )
			{
				SetAnimationPhase( "safe_door", 1 );
			} else
			{
				SetAnimationPhase( "safe_door", 0 );
			}
		}

		if ( !ctx.Read( m_Locked ) )
		{
			return false;
		}
		
		if ( !ctx.Read( m_Code ) )
		{
			return false;
		}

		if ( !ctx.Read( m_HasCode ) )
		{
			return false;
		}

		return true;
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
	override void OnPlacementComplete( Man player )
	{
		if ( IsMissionHost() )
		{
			PlayerBase player_base = PlayerBase.Cast( player );
			vector position = player_base.GetLocalProjectionPosition();
			vector orientation = player_base.GetLocalProjectionOrientation();
			
			SetPosition( position );
			SetOrientation( orientation );

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

		if ( GetNumberOfItems() == 0 )
		{
			return true;
		}
		
		if ( !IsOpened() )
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

		if ( GetNumberOfItems() != 0 )
		{
			return false;
		}
		
		if ( !IsOpened() )
		{
			return true;
		}

		return false;
	}
	
	override void EEKilled( Object killer )
	{
		bool canRaidSafes = GetExpansionSettings().GetBaseBuilding().CanRaidSafes;

		if ( !canRaidSafes )
			return;

		Unlock();

		super.EEKilled( killer );
	}
}