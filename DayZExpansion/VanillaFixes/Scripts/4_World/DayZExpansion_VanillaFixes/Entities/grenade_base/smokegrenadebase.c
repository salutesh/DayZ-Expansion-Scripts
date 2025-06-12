modded class SmokeGrenadeBase
{
	//! SmokeGrenadeBase inherits from Grenade_Base which inherits from ExplosivesBase
	//! SmokeGrenadeBase calls DestroySmokeGrenade in OnWorkStop, which sets grenade health to zero,
	//! which in turn causes ExplosivesBase::EEKilled to fire, which calls InitiateExplosion -> OnExplode,
	//! which starts a timer to call DeleteSafe, which deletes the grenade and makes the smoke particle stop abruptly.
	//! Prevent this by turning OnExplode into a no-op for chemical and nonlethal smoke grenades w/o defined ammo.
	//! https://feedback.bistudio.com/T189673
	override protected void OnExplode()
	{
		switch (GetGrenadeType())
		{
			case EGrenadeType.CHEMICAL:
			case EGrenadeType.NON_LETHAL:
				bool hasAmmo;

				foreach (string ammoType: m_AmmoTypes)
				{
					if (ammoType)
					{
						hasAmmo = true;
						break;
					}
				}

				if (!hasAmmo)
					return;
		}

		super.OnExplode();
	}
}
