modded class TrapBase
{
	override protected eAIItemTargetInformation CreateTargetInformation()
	{
		auto info = new eAIItemHazardTargetInformation(this);
		info.UpdateItemInfo();
		return info;
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		if (damageType == DamageType.CLOSE_COMBAT && source && Expansion_IsMechanicalTrap() && !Expansion_IsExplosive() && IsActive())
		{
			//eAIBase ai;
			//if (Class.CastTo(ai, source.GetHierarchyRootPlayer()))
			//{
				//! Only AI can disarm mechanical traps by melee hit
				Disarm();

				//! Damage source (tool/weapon or hands)
				if (source.IsMan())
				{
					TStringArray dmgZones = {"LeftHand", "RightHand", "LeftArm", "RightArm"};
					dmgZone = dmgZones.GetRandomElement();
				}
					
				source.ProcessDirectDamage(DamageType.CLOSE_COMBAT, this, dmgZone, Expansion_GetAmmoTypes().GetRandomElement(), "0 0 0", 1);
			//}
		}
	}
};
