modded class Grenade_Base
{
	override bool Expansion_IsLive()
	{
		if (IsPinned())
			return false;

		return true;
	}
}
