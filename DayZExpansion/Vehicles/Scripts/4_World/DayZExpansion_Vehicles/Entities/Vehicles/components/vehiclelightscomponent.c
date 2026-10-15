#ifndef DAYZ_1_29
//! 1.30+
modded class VehicleLightsComponent
{
	void Expansion_UnregisterSelection(string name)
	{
		m_SelectionMap.Remove(name);
	}

	void Expansion_UnregisterLight(string name)
	{
		m_LightMap.Remove(name);
	}

	override void SetSelectionMaterial(string name)
	{
		if (m_Parent.IsInherited(ExpansionHelicopterScript) || m_Parent.IsInherited(ExpansionBoatScript))
		{
			if (!m_SelectionMap.Contains(name))
				return;
		}

		super.SetSelectionMaterial(name);
	}

	override void EnableLight(string name)
	{
		if (m_Parent.IsInherited(ExpansionHelicopterScript) || m_Parent.IsInherited(ExpansionBoatScript))
		{
			if (!m_LightMap.Contains(name))
				return;
		}

		super.EnableLight(name);
	}
}
#endif
