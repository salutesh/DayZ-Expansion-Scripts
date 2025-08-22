modded class TLRLight
{
	override float eAI_GetNightVisibility(bool checkWorking = true)
	{
		if (!checkWorking || GetCompEM().IsWorking())
			return 0.0185;

		return 0.0;
	}
}
