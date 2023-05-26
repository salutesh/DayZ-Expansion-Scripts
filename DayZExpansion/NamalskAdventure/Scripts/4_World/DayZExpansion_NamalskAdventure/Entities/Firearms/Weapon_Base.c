modded class Weapon_Base
{
	override float GetChanceToJam()
	{
		int muzzleCount = GetMuzzleCount();
		float highesAmmoDmg;
		for (int muzzleIndex = 0; muzzleIndex < muzzleCount; muzzleIndex++)
		{
			float ammoDamage;
			string ammoTypeName;
			if (!GetCartridgeInfo(muzzleIndex, ammoDamage, ammoTypeName))
				continue;

			if (ammoDamage > highesAmmoDmg)
				highesAmmoDmg = ammoDamage;
		}

		return super.GetChanceToJam() * highesAmmoDmg;
	}
	
	void ExDebugPrint(string text)
	{
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	}
};