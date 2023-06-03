modded class ExpansionStatic
{
	static string GetImpactSurfaceTypeEx(Object directHit, vector hitPosition, vector relativeVelocityBefore)
	{
		string surfaceImpact;

		if (directHit)
		{
			if (directHit.IsMan() || directHit.IsInherited(DayZCreature))
				surfaceImpact = "Hit_MeatBones";
			else if (directHit.IsBuilding())
				surfaceImpact = "Hit_Concrete";
			else if (directHit.IsBush())
				surfaceImpact = "Hit_Foliage";
			else if (directHit.IsTree())
				surfaceImpact = "Hit_Wood";
			else if (directHit.IsRock())
				surfaceImpact = "Hit_Gravel";
			else if (directHit.IsWeapon() || directHit.IsMagazine() || directHit.IsTransport())
				surfaceImpact = "Hit_Metal";
			else if (directHit.IsClothing())
				surfaceImpact = "Hit_Textile";
			else if (SurfaceIsWater(hitPosition))
				surfaceImpact = "Hit_Water";
			else
				surfaceImpact = "Hit_Dirt";
		}
		else
		{
			if (SurfaceIsWater(hitPosition))
				surfaceImpact = "Hit_Water";
			else
				surfaceImpact = "Hit_Dirt";
		}

		return surfaceImpact;
	}
}
