modded class ExplosivesBase
{
	TStringArray Expansion_GetAmmoTypes()
	{
		return m_AmmoTypes;
	}

	bool Expansion_IsLive()
	{
		if (GetArmed() && !GetDefused())
			return true;

		return false;
	}
};
