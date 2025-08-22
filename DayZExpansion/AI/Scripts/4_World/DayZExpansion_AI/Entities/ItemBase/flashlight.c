modded class Flashlight
{
	override float eAI_GetNightVisibility(bool checkWorking = true)
	{
		if (!checkWorking || GetCompEM().IsWorking())
			return 0.0255;

		return 0.0;
	}
}
