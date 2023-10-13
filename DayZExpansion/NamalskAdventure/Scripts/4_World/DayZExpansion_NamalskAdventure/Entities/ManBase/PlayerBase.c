
//! Modifications to allow inventory menu while in a vehicle.
modded class PlayerBase
{
	override void OnCommandVehicleStart()
	{
		super.OnCommandVehicleStart();

		GameInventory gameInv = GetInventory();
		if (gameInv)
			gameInv.UnlockInventory(LOCK_FROM_SCRIPT);
	}

	override void OnCommandVehicleFinish()
	{
		GameInventory gameInv = GetInventory();
		if (gameInv)
			gameInv.LockInventory(LOCK_FROM_SCRIPT);

		super.OnCommandVehicleFinish();
	}

	override bool CanReceiveItemIntoHands(EntityAI item_to_hands)
	{
		if (IsInVehicle() && CanPickupHeavyItem(item_to_hands))
			return true;

		return super.CanReceiveItemIntoHands(item_to_hands);
	}
};