modded class Chemlight_ColorBase
{
	override float eAI_GetNightVisibility(bool checkWorking = true)
	{
		if (!checkWorking || GetCompEM().IsWorking())
			return 0.0075;

		return 0.0;
	}
}
