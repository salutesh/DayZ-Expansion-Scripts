modded class DayZExpansion
{
	override void OnLoaded()
	{
		if (IsMissionHost())
		{
			ExpansionSettings.SI_Map.Invoke();
		}

		super.OnLoaded();
	}
};
