class ExpansionUnloadHideBullet extends WeaponEjectBullet
{
    override void OnEntry (WeaponEventBase e)
    {
        super.OnEntry(e);
        m_weapon.ExpansionHideWeaponPart("ammo", true);
    }

    override void OnAbort (WeaponEventBase e)
    {
        super.OnAbort(e);
        m_weapon.ExpansionHideWeaponPart("ammo", true);
    }

    override void OnExit (WeaponEventBase e)
    {
        super.OnAbort(e);
        m_weapon.ExpansionHideWeaponPart("ammo", true);
    }
}