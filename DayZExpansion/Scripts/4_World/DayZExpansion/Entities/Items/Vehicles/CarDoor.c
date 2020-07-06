modded class CarDoor
{
	override bool CanDetachAttachment( EntityAI parent )
	{
		// parent casts to CarScript, this causes an error filling up crash log.
		ExpansionVehicleScript veh;
		if ( Class.CastTo( veh, parent ) )
		{
			InventoryLocation loc = new InventoryLocation();
			if ( !GetInventory().GetCurrentInventoryLocation( loc ) )
				return false;

			string slotName = InventorySlots.GetSlotName( loc.GetSlot() );
			if ( slotName && veh.GetCarDoorsState( slotName ) == CarDoorState.DOORS_OPEN )
				return true;

			return false;
		}

		return super.CanDetachAttachment( parent );
	}

	string ExpansionMainType()
	{
		return "";
	}
}