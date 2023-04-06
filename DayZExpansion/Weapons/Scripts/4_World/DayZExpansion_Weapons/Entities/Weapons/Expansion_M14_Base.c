/**
 * Expansion_M14_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_M14_Base: RifleBoltLock_Base
{
	override bool CanEnterIronsights()
	{
		ItemOptics optic = GetAttachedOptics();
		
		if (optic && Expansion_PMII25Optic.Cast(optic))
			return true;

		return super.CanEnterIronsights();
	}

	override RecoilBase SpawnRecoilObject()
	{
		return new SvdRecoil(this);
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

			if ( FindAttachmentBySlotName("Expansion_M1AScopeRail") == NULL )
			{
				return false;
			}
	
			if ( FindAttachmentBySlotName("ExpansionSniperOptics") != NULL || FindAttachmentBySlotName("WeaponOptics"))
			{
				return false;
			}	
		}	 
		return super.CanReceiveAttachment(attachment, slotId);
	}

	override bool CanReleaseAttachment( EntityAI attachment )
	{
		if ( attachment.IsKindOf("Expansion_M1A_RailAtt") && ( FindAttachmentBySlotName("ExpansionSniperOptics") || FindAttachmentBySlotName("WeaponOptics") ) )
			return false;
			
		return super.CanReleaseAttachment(attachment);
	}

	override bool CanDisplayAttachmentSlot(int slot_id)
	{	
		if (!super.CanDisplayAttachmentSlot(slot_id))
			return false;   

		string slot_name = InventorySlots.GetSlotName(slot_id);
		
		if ( slot_name == "ExpansionSniperOptics" )
		{
			return this.FindAttachmentBySlotName("ExpansionSniperOptics") != NULL;
		}
		if ( slot_name == "weaponOptics" )
		{
			return this.FindAttachmentBySlotName("ExpansionSniperOptics") == NULL;	
		}		
		return true;
	}		
		
	//Debug menu Spawn Ground Special
	override void OnDebugSpawn()
	{
		EntityAI entity;
		if ( Class.CastTo(entity, this) )
		{
			entity.SpawnEntityOnGroundPos("Mag_Expansion_M14_10Rnd", entity.GetPosition());
		}
	}	
};