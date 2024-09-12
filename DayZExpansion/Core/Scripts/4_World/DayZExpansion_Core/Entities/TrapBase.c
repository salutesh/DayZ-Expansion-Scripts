modded class TrapBase
{
	override bool Expansion_IsDanger()
	{
		return IsActive();
	}
};
