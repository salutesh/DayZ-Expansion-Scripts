modded class FlammableBase
{
	//! Vanilla doesn't call super...
	override void OnSwitchOn()
	{
		super.OnSwitchOn();

		eAI_OnSwitchOn();
	}

	//! Vanilla doesn't call super...
	override void OnSwitchOff()
	{
		super.OnSwitchOff();

		eAI_OnSwitchOff();
	}

	override float eAI_GetNightVisibility(bool checkWorking = true)
	{
		if (!checkWorking || GetCompEM().IsWorking())
			return 0.03;

		return 0.0;
	}
}
