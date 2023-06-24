modded class DayZGame
{
	void eAI_HandleFirearmEffects(Object source, Object hitObject, int contactComponent, string surface, vector hitPosition, vector surfNormal, vector inSpeed, string ammoType)
	{
		bool isWater;
		if (surface == "Hit_Water")
			isWater = true;
		bool deflected;
		if (surface != "Hit_MeatBones" && Math.RandomIntInclusive(0, 1))
			deflected = true;
		GetExpansionGame().m_eAI_FirearmEffectsCallCount++;
		FirearmEffects(source, hitObject, contactComponent, surface, hitPosition, surfNormal, vector.Zero, inSpeed, vector.Zero, isWater, deflected, ammoType);
	#ifndef SERVER
		EffectSound sound = SEffectManager.Expansion_PlaySound("bulletFlyBy_SoundSet", hitPosition);
		//! FIXME: No impact sound :-(
	#endif
	}
}
