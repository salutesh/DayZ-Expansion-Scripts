class bldr_fire_barrel: BarrelHoles_Red
{
	protected bool m_bldr_Opened;

	override protected void SpendFireConsumable( float amount ) {};
	override void StopFire( FireplaceFireState fire_state = FireplaceFireState.END_FIRE ) {};
	override void AddWetnessToFireplace(float amount) {};
	override protected void AddWetnessToItem(ItemBase item, float amount) {};

	void bldr_fire_barrel()
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

	override void SoundBarrelOpenPlay()
	{
		if (!m_bldr_Opened)
		{
			m_bldr_Opened = true;
			super.SoundBarrelOpenPlay();
		}
	}

	override void SoundBarrelClosePlay()
	{
		if (m_bldr_Opened)
		{
			m_bldr_Opened = false;
			super.SoundBarrelClosePlay();
		}
	}
};