modded class DayZGame
{
	override void ExplosionEffectsEx(Object source, Object directHit, int componentIndex, float energyFactor, float explosionFactor, HitInfo hitInfo)
	{
		super.ExplosionEffectsEx(source, directHit, componentIndex, energyFactor, explosionFactor, hitInfo);
		
		if (IsServer())
		{
			eAINoiseSystem.AddNoise(hitInfo.GetPosition(), 21.0, string.Format("cfgAmmo %1 NoiseExplosion", hitInfo.GetAmmoType()), hitInfo.GetSurfaceNoiseMultiplier());
		}
	}

	/* Probably not necessary for AI to react to bullet impact, enough if they can hear the shot (source)
	override void FirearmEffects(Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal,
		 vector exitPos, vector inSpeed, vector outSpeed, bool isWater, bool deflected, string ammoType) 
	 {
		 super.FirearmEffects(source, directHit, componentIndex, surface, pos, surfNormal,
		 exitPos, inSpeed, outSpeed, isWater, deflected, ammoType);

		if (IsServer())
		{
			float surfaceCoef = SurfaceGetNoiseMultiplier(directHit, pos, componentIndex);
			float coefAdjusted = surfaceCoef * inSpeed.Length() / ConfigGetFloat("cfgAmmo " + ammoType + " initSpeed");
			eAINoiseSystem.AddNoise(pos, 10.0, string.Format("cfgAmmo %1 NoiseHit", ammoType), coefAdjusted);
		}
	 }
	 */

	override void CloseCombatEffects(Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal, bool isWater, string ammoType) 
	{
		super.CloseCombatEffects(source, directHit, componentIndex, surface, pos, surfNormal,
		 isWater, ammoType);

		if (IsServer())
		{
			float surfaceCoef = SurfaceGetNoiseMultiplier(directHit, pos, componentIndex);
			eAINoiseSystem.AddNoise(EntityAI.Cast(source), pos, string.Format("cfgAmmo %1 NoiseHit", ammoType), surfaceCoef);
		}
	}
}
