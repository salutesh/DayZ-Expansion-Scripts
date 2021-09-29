/**
 * Expansion_MP5_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_MP5_Base: RifleBoltLock_Base
{
	void Expansion_MP5_Base()
	{
	}
	
	override RecoilBase SpawnRecoilObject()
	{
		return new Mp5kRecoil(this);
	}
		
	//Debug menu Spawn Ground Special
	override void OnDebugSpawn()
	{
		EntityAI entity;
		if ( Class.CastTo(entity, this) )
		{
			entity.SpawnEntityOnGroundPos("Mag_MP5_30Rnd", entity.GetPosition());
		}
	}
	
	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		if  ( attachment.IsKindOf("ItemOptics") )
		{
			PlayerBase player = PlayerBase.Cast( GetHierarchyRootPlayer() );

			if ( player && !player.IsPlayerLoaded() )
			{
				// Blanket allow the scope attachment when a player is currently being loaded from db. This is to prevent players loosing attached scopes during player load when the scope has a prerequisite attachment like a rail that we normally check for. We cannot assume the rail and scope are loaded in order, thus we cannot reliably check for the presence of the rail when the scope is loaded.
				return true;
			}

			if ( FindAttachmentBySlotName("Expansion_MP5ScopeRail") == NULL )
			{
				return false;
			}
		}

		return super.CanReceiveAttachment(attachment, slotId);
	}

	override bool CanReleaseAttachment( EntityAI attachment )
	{
		if ( attachment.IsKindOf("Expansion_MP5_RailAtt") && FindAttachmentBySlotName("WeaponOptics") )
			return false;
			
		return super.CanReleaseAttachment(attachment);
	}

	override bool CanDisplayAttachmentSlot( string slot_name )
	{	
		if (!super.CanDisplayAttachmentSlot(slot_name))
			return false;   
		
		if ( slot_name == "weaponOptics" )
		{
			return this.FindAttachmentBySlotName("Expansion_MP5ScopeRail") != NULL;	
		}

		return true;
	}	
};

class Expansion_MP5: Expansion_MP5_Base
{
	override bool CanDisplayAttachmentSlot( string slot_name )
	{	
		if (!super.CanDisplayAttachmentSlot(slot_name))
			return false;
		
		if ( slot_name == "pistolMuzzle" )
		{
			return this.FindAttachmentBySlotName("weaponMuzzleMP5") == NULL;
		}
		if ( slot_name == "weaponMuzzleMP5" )
		{
			return this.FindAttachmentBySlotName("pistolMuzzle") == NULL;	
		}
		
		return true;
	}	
};