modded class BearTrap
{
	override TStringArray Expansion_GetAmmoTypes()
	{
		return {"BearTrapHit"};
	}

	override bool Expansion_IsMechanicalTrap()
	{
		return true;
	}
}
