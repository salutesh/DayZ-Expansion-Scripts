modded class UniversalLight
{
	override float eAI_GetNightVisibility(bool checkWorking = true)
	{
		if (!checkWorking || GetCompEM().IsWorking())
		{
			//! If attached to a weapon, the universal light has significantly more illumination power
			//! than held in hands or attached to helmet (see vanilla UniversallightLight.c, UpdateLightMode)
			EntityAI parent = GetHierarchyParent();
			if (parent && parent.IsWeapon())
				return 0.045;

			return 0.0255;
		}

		return 0.0;
	}
}
