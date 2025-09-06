modded class NVGoggles
{
	override bool Expansion_TryTurningOn()
	{
		if (HasEnergyManager() && (!GetCompEM().CanSwitchOn() || !GetCompEM().CanWork()))
			return false;

		RotateGoggles(false);

		return true;
	}

	override bool Expansion_TryTurningOff()
	{
		if (HasEnergyManager() && !GetCompEM().CanSwitchOff())
			return false;

		RotateGoggles(true);

		return true;
	}

	override float eAI_GetNightVisibility(bool checkWorking = true)
	{
		if (!checkWorking || (m_IsLowered && (!HasEnergyManager() || GetCompEM().IsWorking())))
			return 0.35;

		return 0.0;
	}
}
