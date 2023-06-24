class Expansion_Kar98_Bayonet: Inventory_Base
{
	override bool IsMeleeFinisher()
	{
		return true;
	}
	
	override array<int> GetValidFinishers()
	{
		return {EMeleeHitType.FINISHER_LIVERSTAB,EMeleeHitType.FINISHER_NECKSTAB};
	}
	
	override bool CanPutAsAttachment( EntityAI parent )
	{
		if(!super.CanPutAsAttachment(parent)) return false;

		if ( parent.FindAttachmentBySlotName("suppressorImpro") == NULL )
			return true;
        
		return false;
	}

	override void OnWasAttached(EntityAI parent, int slot_id)
	{
		super.OnWasAttached(parent, slot_id);
		
		if( parent.IsWeapon() )
			parent.SetBayonetAttached(true, slot_id);
	}
	
	override void OnWasDetached(EntityAI parent, int slot_id)
	{
		super.OnWasDetached(parent, slot_id);
		
		if( parent.IsWeapon() )
			parent.SetBayonetAttached(false);
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionBurnSewTarget);
		AddAction(ActionUnrestrainTarget);
		AddAction(ActionBurnSewSelf);
		AddAction(ActionDigWorms);
	}
};
