class ExpansionEXPS3HoloOptic : ItemOptics {};
class ExpansionHAMROptic : ItemOptics {};

class ExpansionKar98ScopeOptic : ItemOptics
{
	override bool CanPutAsAttachment(EntityAI parent)
	{
		if (!super.CanPutAsAttachment(parent))
			return false;

		if (parent)
		{
			//! Allow scope attachment if parent is Expansion Kar98 or has Expansion Kar98 optics slot
			//! (latter is deprecated but kept for compat)
			int slotIdKar98 = InventorySlots.GetSlotIdFromString("ExpansionKar98Optics");
			if (!parent.IsInherited(Expansion_Kar98_Base) && !parent.GetInventory().HasAttachmentSlot(slotIdKar98))
				return false;
		}
		
		return true;
	}
}

class ExpansionDeltapointOptic : ItemOptics {};
