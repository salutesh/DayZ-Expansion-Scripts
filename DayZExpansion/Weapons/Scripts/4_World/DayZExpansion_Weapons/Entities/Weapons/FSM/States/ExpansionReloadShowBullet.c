class ExpansionReloadShowBullet: WeaponChambering
{
	override void OnAbort(WeaponEventBase e)
	{
		super.OnAbort(e);
		m_weapon.ExpansionHideWeaponPart("ammo", true);
	}

	override void OnExit (WeaponEventBase e)
	{
		super.OnExit(e);
		m_weapon.ExpansionHideWeaponPart("ammo", false);
	}
	
};