class bldr_fireplace: Fireplace
{
	override protected void SpendFireConsumable( float amount ) {};
	override void StopFire( FireplaceFireState fire_state = FireplaceFireState.END_FIRE ) {};
	override void AddWetnessToFireplace(float amount) {};
	override protected void AddWetnessToItem(ItemBase item, float amount) {};

	void bldr_fireplace()
	{
		SetAllowDamage(false);
	}

	override bool CanExtinguishFire()
	{
		return false;
	}

	override bool CanHaveWetness()
	{
		return false;
	}

	override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}

	override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}
};