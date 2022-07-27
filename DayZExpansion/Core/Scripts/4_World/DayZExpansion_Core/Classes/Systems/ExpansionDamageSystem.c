/**
 * ExpansionDamageSystem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! Consistent explosion damage
class ExpansionDamageSystem
{
	static bool IsEnabledForExplosionTarget(EntityAI target)
	{
		if (!GetExpansionSettings().GetDamageSystem().Enabled)
			return false;

		return ExpansionStatic.IsAnyOf(target, GetExpansionSettings().GetDamageSystem().ExplosionTargets, true);
	}

	static bool GetExplosionRange(EntityAI source, inout string ammoType, out float explosionRange, out float explosionDropoffRange)
	{
		if (!ammoType)
			ammoType = source.ConfigGetString("ammoType");

		if (ammoType)
		{
			if (GetGame().ConfigIsExisting("CfgAmmo " + ammoType + " indirectHitRange"))
			{
				explosionDropoffRange = GetGame().ConfigGetFloat("CfgAmmo " + ammoType + " indirectHitRange");
			}
			else
			{
				Log("WARNING: Ammo " + ammoType + " has no indirectHitRange! Assuming 1.0");
				explosionDropoffRange = 1.0;
			}

			if (GetGame().ConfigIsExisting("CfgAmmo " + ammoType + " indirectHitRangeMultiplier"))
			{
				explosionRange = explosionDropoffRange * GetGame().ConfigGetFloat("CfgAmmo " + ammoType + " indirectHitRangeMultiplier");
			}
			else
			{
				Log("WARNING: Ammo " + ammoType + " has no indirectHitRangeMultiplier! Assuming 1.0");
				explosionRange = explosionDropoffRange;
			}

			return true;
		}
		else
		{
			Log("ERROR: " + source.GetType() + " has no ammo!");
		}

		return false;
	}

	static float GetExplosionDamage(EntityAI source, EntityAI target, string ammoType = "")
	{
		float explosionRange;
		float explosionDropoffRange;
		GetExplosionRange(source, ammoType, explosionRange, explosionDropoffRange);

		if (ammoType)
		{
			float dmg = GetGame().ConfigGetFloat("CfgAmmo " + ammoType + " DamageApplied Health damage");
			return dmg * GetExplosionDamageNormalized(source, target, explosionRange, explosionDropoffRange);
		}

		return 0.0;
	}

	static float GetExplosionDamageNormalized(EntityAI source, EntityAI target, float explosionRange, float explosionDropoffRange, vector position = vector.Zero)
	{
		if (position == vector.Zero)
			position = source.GetPosition();

		float blastDistance = vector.Distance(position, target.GetPosition());

		if (explosionRange > explosionDropoffRange)
			return ExpansionMath.LinearConversion(explosionRange, explosionDropoffRange, blastDistance);

		if (blastDistance > explosionDropoffRange)
			return 0.0;

		return 1.0;
	}

	static void OnBeforeExplode(EntityAI source, int damageType, string ammoType = "", vector position = vector.Zero)
	{
		if (!GetGame().IsServer() || !GetExpansionSettings().GetDamageSystem().Enabled)
			return;

		if (position == vector.Zero)
			position = source.GetPosition();

		array<Object> nearest_objects = new array<Object>;
		array<CargoBase> proxy_cargos = new array<CargoBase>;

		float explosionRange;
		float explosionDropoffRange;
		GetExplosionRange(source, ammoType, explosionRange, explosionDropoffRange);

		Log("About to explode " + source + " at " + position + " " + ExpansionStatic.DamageTypeToString(damageType) + " " + ammoType + " range " + explosionRange + " dropoff " + explosionDropoffRange);

		map<EntityAI, ref Param2<float, float>> targets();

		GetGame().GetObjectsAtPosition3D(position, explosionRange, nearest_objects, proxy_cargos);

		foreach (Object nearest_object: nearest_objects)
		{
			if (nearest_object == source)
				continue;

			if (!nearest_object.GetAllowDamage())
				continue;

			ItemBase nearest_item;
			if (!Class.CastTo(nearest_item, nearest_object) || !nearest_item.CanBeDamaged())
				continue;

			if (!IsEnabledForExplosionTarget(nearest_item))
				continue;

			float explosionDamageNormalized = GetExplosionDamageNormalized(source, nearest_item, explosionRange, explosionDropoffRange, position);
			bool targetHit = !GetExpansionSettings().GetDamageSystem().CheckForBlockingObjects;
			Object blockingObject;
			if (!explosionDamageNormalized || (!targetHit && !Raycast(source, nearest_item, position, targetHit, blockingObject)))
			//if (!explosionDamageNormalized)
			{
				Log(nearest_object.ToString() + " at " + nearest_object.GetPosition() + " is NOT a target for " + source + " at " + position + " (too far away)");
				continue;
			}

			if (targetHit)
			{
				Log(nearest_object.ToString() + " at " + nearest_object.GetPosition() + " is a target for " + source + " at " + position);
				targets.Insert(nearest_item, new Param2<float, float>(nearest_item.GetHealth(), explosionDamageNormalized));
			}
			else if (blockingObject)
			{
				Log(nearest_object.ToString() + " at " + nearest_object.GetPosition() + " is NOT a target for " + source + " at " + position + " (blocking " + blockingObject + " is in the way)");
			}
			else
			{
				Log(nearest_object.ToString() + " at " + nearest_object.GetPosition() + " is NOT a target for " + source + " at " + position);
			}
		}

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OnAfterExplode, 34, false, source, source.GetType(), position.ToString(), damageType, ammoType, targets);
	}

	static bool Raycast(EntityAI source, EntityAI target, vector position, out bool targetHit, out Object blockingObject)
	{
		vector hitPosition;
		vector hitNormal;
		int contactComponent;
		set<Object> results();

		if (!DayZPhysics.RaycastRV(position, target.GetPosition(), hitPosition, hitNormal, contactComponent, results, null, source, true, false, ObjIntersectView, 0.1, CollisionFlags.ALLOBJECTS))
			return false;

		targetHit = false;
		blockingObject = null;

		foreach (Object obj: results)
		{
			if (obj == target)
			{
				if (!blockingObject || vector.Distance(blockingObject.GetPosition(), target.GetPosition()) >= vector.Distance(position, target.GetPosition()))
					targetHit = true;
				break;
			}
			else if (obj.IsBuilding() || obj.ConfigGetString("physLayer") == "item_large")
			{
				blockingObject = obj;
			}
		}

		return true;
	}

	static void OnAfterExplode(EntityAI source, string sourceType, string sourcePosition, int damageType, string ammoType, map<EntityAI, ref Param2<float, float>> targets)
	{
		EntityAI directDamageSource = source;
		foreach (EntityAI target, Param2<float, float> health_dmg: targets)
		{
			if (!target)
				continue;

			if (target.GetHealth() == health_dmg.param1)
			{
				Log(target.ToString() + " at " + target.GetPosition() + " was not damaged by " + sourceType + " at " + sourcePosition + " - applying direct " + ExpansionStatic.DamageTypeToString(damageType) + " " + ammoType + " damage");
				if (!source)
					directDamageSource = target;
				target.ProcessDirectDamage(damageType, directDamageSource, "", ammoType, "0 0 0", health_dmg.param2);
			}
		}
	}

	static void Log(string msg)
	{
		if (GetExpansionSettings().GetLog().ExplosionDamageSystem)
			GetExpansionSettings().GetLog().PrintLog("[ExplosionDamageSystem] " + msg);
	}
}
