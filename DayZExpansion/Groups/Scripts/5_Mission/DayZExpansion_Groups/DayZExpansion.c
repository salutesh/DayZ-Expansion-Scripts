modded class DayZExpansion
{
	override void OnLoaded()
	{
		if (IsMissionHost())
		{
			ExpansionSettings.SI_Party.Invoke();
		}

		super.OnLoaded();
	}
};
