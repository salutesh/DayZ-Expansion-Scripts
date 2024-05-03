modded class ExplosivesBase
{
	override TStringArray Expansion_GetAmmoTypes()
	{
		return m_AmmoTypes;
	}

	override bool Expansion_IsDanger()
	{
		return Expansion_IsLive();
	}

	bool Expansion_IsLive()
	{
		if (GetArmed() && !GetDefused())
			return true;

		return false;
	}
};
