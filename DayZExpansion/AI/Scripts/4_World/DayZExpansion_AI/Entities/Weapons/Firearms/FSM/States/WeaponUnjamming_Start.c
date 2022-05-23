modded class WeaponUnjamming_Start
{
	override void OnUpdate(float dt)
	{
		eAIBase p;
		if (Class.CastTo(p, m_weapon.GetHierarchyParent()))
		{
			m_dtAccumulator += dt;

			HumanInputController hic = p.GetInputController();
			HumanCommandWeapons hcw = p.GetCommandModifier_Weapons();

			if (m_dtAccumulator >= m_jamTime)
			{
				wpnDebugPrint("[wpnfsm] " + Object.GetDebugName(m_weapon) + " Unjammed!");
				hcw.StartAction(WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_END);
				m_weapon.ProcessWeaponEvent(new WeaponEventUnjammingTimeout(p));
			}

			return;
		}

		super.OnUpdate(dt);
	}
};
