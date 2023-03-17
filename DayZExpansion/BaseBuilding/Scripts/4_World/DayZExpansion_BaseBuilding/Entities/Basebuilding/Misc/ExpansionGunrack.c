/**
 * ExpansionGunrack.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionGunrack
 * @brief		
 **/
class ExpansionGunrack: DeployableContainer_Base 
{
	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionGunrack()
	{
		m_HalfExtents = Vector(0.2,0.5,0.4); //! TODO: Need adjustment
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
	override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" )
	{
		if ( IsMissionHost() )
		{			
			SetPosition( position );
			SetOrientation( orientation );

			SetSynchDirty();
		}	
			
		SetIsPlaceSound( true );
	}
	
	// ------------------------------------------------------------
	// CanPutInCargo
	// ------------------------------------------------------------
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo( parent ) )
		{
			return false;
		}
		
		if( GetInventory().AttachmentCount() == 0 )
		{
			return true;
		}
		return false;
	}
	
	// ------------------------------------------------------------
	// CanPutIntoHands
	// ------------------------------------------------------------
	override bool CanPutIntoHands( EntityAI parent )
	{
		if( !super.CanPutIntoHands( parent ) )
		{
			return false;
		}
		
		if( GetInventory().AttachmentCount() == 0 )
		{
			return true;
		}
		return false;
	}
	
	// ------------------------------------------------------------
	// CanReceiveAttachment
	// ------------------------------------------------------------
	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		
		if ( GetHierarchyRootPlayer() || GetInventory().IsInCargo() )
		{
			return false;
		}
		
		return super.CanReceiveAttachment(attachment, slotId);
	}

} 