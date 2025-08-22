#ifdef AdmiralsDivingMod
class eAIImplementSwimming: DayZPlayerImplementSwimming
{
	override bool CheckSwimmingStart(out vector waterLevel)
	{
		return DayZPlayerUtils.CheckWaterLevel(m_pPlayer,waterLevel) == EWaterLevels.LEVEL_SWIM_START;
	}
}
#endif
