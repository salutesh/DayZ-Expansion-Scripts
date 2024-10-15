modded class Grenade_Base
{
	override bool Expansion_IsLive()
	{
		if ((!m_Pinnable && !dBodyIsActive(this)) || IsPinned())
			return false;

		return true;
	}
}
