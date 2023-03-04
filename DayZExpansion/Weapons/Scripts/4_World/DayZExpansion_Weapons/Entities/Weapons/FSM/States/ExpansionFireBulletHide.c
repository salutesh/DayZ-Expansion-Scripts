class ExpansionFireBulletHide: WeaponFire
{
	override void OnEntry (WeaponEventBase e)
	{
		m_dtAccumulator = 0;

		wpnPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " WeaponFire bang!");
		//m_weapon.Fire();
		int mi = m_weapon.GetCurrentMuzzle();
		if (TryFireWeapon(m_weapon, mi))
		{
			DayZPlayerImplement p;
			if (Class.CastTo(p, e.m_player))
				p.GetAimingModel().SetRecoil(m_weapon);
			m_weapon.EjectCasing(mi);
			m_weapon.ExpansionHideWeaponPart("ammo", true);
			m_weapon.OnFire(mi);
		}
		super.OnEntry(e);
	}
}