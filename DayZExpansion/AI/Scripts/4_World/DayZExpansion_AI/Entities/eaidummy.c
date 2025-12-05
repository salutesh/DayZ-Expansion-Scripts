class eAIDummy: PlayerBase
{
	override bool CanBeTargetedByAI(EntityAI ai)
	{
		return false;
	}
}

class eAIDummy_SurvivorM_Mirek: eAIDummy {};
