class Expansion_M14_Base : RifleBoltLock_Base
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
			if ( FindAttachmentBySlotName("Expansion_M1AScopeRail") == NULL )
			{
				return false;
			}	
		}		  
		if  ( attachment.IsKindOf("ItemOptics") )
		{			
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
	override bool CanDisplayAttachmentSlot( string slot_name )
	{	
		if (!super.CanDisplayAttachmentSlot(slot_name))
			return false;   
		
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
};