modded class WeaponStableState
{
	override void SyncAnimState()
	{
		//! DayZ 1.24 changed p from DayZPlayer to PlayerBase
		ExpansionNotPlayerBase p;
		if (Class.CastTo(p, m_weapon.GetHierarchyParent()))
		{
			//! Almost verbatim copy of vanilla except we use GetHumanInventory().GetEntityInHands() instead of GetItemInHands()
			int curr = m_weapon.GetWeaponAnimState();
			if (curr != m_animState)
			{
				if (p.GetHumanInventory().GetEntityInHands() == m_weapon)
				{
					HumanCommandWeapons hcw = p.GetCommandModifier_Weapons();
					if (hcw)
					{
						hcw.SetInitState(m_animState);
						if (LogManager.IsWeaponLogEnable()) fsmDebugSpam("[wpnfsm] " + Object.GetDebugName(m_weapon) + " state=" + m_weapon.GetCurrentState().Type() + " synchronized anim state: " + typename.EnumToString(PistolAnimState, curr) + " --> " + typename.EnumToString(PistolAnimState, m_animState));
					}
					else
					{
						Human wpnOwner = Human.Cast(m_weapon.GetHierarchyRootPlayer());
						HumanCommandWeapons.StaticSetInitState(wpnOwner, m_animState);
						if (LogManager.IsWeaponLogEnable()) fsmDebugSpam("[wpnfsm] " + Object.GetDebugName(m_weapon) + " state=" + m_weapon.GetCurrentState().Type() + " synchronized remote anim state: " + typename.EnumToString(PistolAnimState, curr) + " --> " + typename.EnumToString(PistolAnimState, m_animState));
					}
				}
				m_weapon.SetWeaponAnimState(m_animState);
			}

			return;
		}

		super.SyncAnimState();
	}
}
