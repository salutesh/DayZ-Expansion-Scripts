modded class WeaponFireAndChamberNext
{
	override void OnUpdate(float dt)
	{
		eAIBase p;
		if (Class.CastTo(p, m_weapon.GetHierarchyParent()))
		{
			m_dtAccumulator += dt;

			int muzzleIndex = m_weapon.GetCurrentMuzzle();
			float reloadTime = m_weapon.GetReloadTime(muzzleIndex);
			
			if (m_dtAccumulator >= reloadTime && (m_weapon.GetCurrentModeAutoFire(m_weapon.GetCurrentMuzzle()) || (m_weapon.GetBurstCount() < m_weapon.GetCurrentModeBurstSize(muzzleIndex))))
			{
				if (m_weapon.CanProcessWeaponEvents())
				{
					m_weapon.ProcessWeaponEvent(new WeaponEventReloadTimeout(p));
				}
			}
		}
		else
		{
			super.OnUpdate(dt);
		}
	}
}
