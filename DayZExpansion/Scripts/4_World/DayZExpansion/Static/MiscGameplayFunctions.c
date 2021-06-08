/**
 * MiscGameplayFunctions.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class MiscGameplayFunctions
{
	static bool ObstructingObjectsContainExpansionBaseBuildingOrTent;
	
	override static void FilterObstructingObjects(array<Object> potentiallyObstructingObjects, out array<Object> obstructingObjects)
	{
		if (!obstructingObjects)
			obstructingObjects = new array<Object>;

		ObstructingObjectsContainExpansionBaseBuildingOrTent = false;
		for ( int i = 0; i < potentiallyObstructingObjects.Count(); ++i )
		{
			Object obj = potentiallyObstructingObjects[i];
			//! Prevent accessing vicinity through Expansion basebuilding and vanilla tents
			TentBase tent = TentBase.Cast( obj );
			if ( obj && ( obj.CanObstruct() || ( tent && tent.GetState() == TentBase.PITCHED ) ) )
			{
				if ( obj.IsInherited( ExpansionBaseBuilding ) || tent )
					ObstructingObjectsContainExpansionBaseBuildingOrTent = true;

				obstructingObjects.Insert(obj);
			}
		}
	}

	override static void FilterObstructedObjectsByGrouping(vector origin, float range, float distanceDelta, array<Object> objects, array<Object> obstructingObjects, out array<Object> filteredObjects, bool doDistanceCheck = false, bool checkIfDistanceCanBeIgnored = false, float maxDist = 0)
	{
		//! Mostly vanilla
		//! Had to duplicate this so our versions of CanIgnoreDistanceCheck and IsObjectObstructedEx gets used and not the vanilla one

		array<Object> vicinityObjects= new array<Object>;
		vicinityObjects.Copy(objects);
		
		int i = 0;
		int j = 0;
		int k = 0;			
		int mCount = vicinityObjects.Count();
		
		if (!filteredObjects)
			filteredObjects = new array<Object>;
		
		// Remove objects that are too far from the player anyways
		if ( doDistanceCheck )
		{
			for ( i = vicinityObjects.Count() - 1; i >= 0; --i )
			{
				Object obj = vicinityObjects[i];
				if ( obj && !CanIgnoreDistanceCheck( obj ) && vector.DistanceSq(origin, obj.GetPosition()) > maxDist * maxDist )
					vicinityObjects.Remove(i);
			}
		}

		// Sort obstructingObjects to have the furthest one first
		array<Object> sortedObstructingObjects = new array<Object>;
		array<float> distanceHelper = new array<float>;
		array<float> distanceHelperUnsorted = new array<float>;
		float distance, dist1, dist2;

		for ( i = 0; i < obstructingObjects.Count(); ++i )
		{
			distance = vector.DistanceSq(obstructingObjects[i].GetWorldPosition(), origin);
			distanceHelper.Insert(distance);
		}

		distanceHelperUnsorted.Copy(distanceHelper);
		distanceHelper.Sort();

		for ( i = distanceHelper.Count() - 1; i >= 0; --i )
			sortedObstructingObjects.Insert(obstructingObjects[distanceHelperUnsorted.Find(distanceHelper[i])]);

		array<ref array<Object>> tempGroups = new array<ref array<Object>>;
		array<ref array<Object>> objectGroups = new array<ref array<Object>>;
		ref array<Object> group;
		
		float cos = Math.Cos(90);
		float sin = Math.Sin(90);

		// Iterate through sorted obstructingObjects
		for ( i = 0; i < sortedObstructingObjects.Count(); ++i )
		{
			Object obstrObj = sortedObstructingObjects[i];
			vector worldPos = obstrObj.GetWorldPosition();
			vector min, max;
			vector minMax[2];
			if ( obstrObj.GetCollisionBox(minMax) || ( obstrObj.IsInherited( ExpansionBaseBuilding ) && ExpansionBaseBuilding.Cast( obstrObj ).ExpansionGetCollisionBox( minMax ) ) )
			{
				min = minMax[0];
				max = minMax[1];
				max = max * (obstrObj.GetOrientation() * range);

				vector center, dx, dy, dz, half;
				center = (min + max) * 0.5;
				dz = obstrObj.GetOrientation();
				dx = vector.RotateAroundZero(dz, vector.Up, cos, sin);
				dy = vector.RotateAroundZero(dz, vector.Aside, cos, sin);
				half = (max - min) * 0.5;
				half = Vector(Math.AbsFloat(half[0]), Math.AbsFloat(half[1]), Math.AbsFloat(half[2]));

				group = new array<Object>;

				// Group objects within the above box
				for ( j = vicinityObjects.Count() - 1; j >= 0; --j )
				{
					Object vicObj = vicinityObjects[j];
					if ( vicObj )
					{
						vector d = vicObj.GetWorldPosition() - worldPos + center;
						if ( Math.AbsFloat(vector.Dot(d, dx)) <= half[0] && Math.AbsFloat(vector.Dot(d, dy)) <= half[1] && Math.AbsFloat(vector.Dot(d, dz)) <= half[2] )
						{
							group.Insert(vicObj);
							vicinityObjects.Remove(j);
						}
					}
				}

				if ( group.Count() > 0 )
					tempGroups.Insert(group);
			}
		}

		// Go through the objects grouped by obstruction to split them by distance too
		for ( i = 0; i < tempGroups.Count(); ++i )
			SplitArrayIntoGroupsByDistance(tempGroups[i], objectGroups, distanceDelta);

		// Split initial groups by distance
		SplitArrayIntoGroupsByDistance(vicinityObjects, objectGroups, distanceDelta);

		// Raycast accordingly to groups		
		IsObjectObstructedCache cache = new IsObjectObstructedCache(origin, mCount);
		for ( i = 0; i < objectGroups.Count(); ++i )
		{
			array<Object> objectGroup = objectGroups[i];
			Object sampleObject = objectGroup[0];

			if ( !IsObjectObstructedEx(sampleObject, cache) )
				filteredObjects.InsertAll(objectGroup);

			cache.ClearCache();
		}
	}

	override static bool IsObjectObstructed(Object object, bool doDistanceCheck = false, vector distanceCheckPos = "0 0 0", float maxDist = 0)
	{
		//! Vanilla
		//! Had to duplicate this so our version of IsObjectObstructedEx gets used and not the vanilla one

		vector rayStart;
		MiscGameplayFunctions.GetHeadBonePos( PlayerBase.Cast( GetGame().GetPlayer() ), rayStart);
		IsObjectObstructedCache cache = new IsObjectObstructedCache(rayStart, 1);
		return IsObjectObstructedEx(object, cache, doDistanceCheck, distanceCheckPos, maxDist);
	}

	override static bool IsObjectObstructedEx(Object object, IsObjectObstructedCache cache, bool doDistanceCheck = false, vector distanceCheckPos = "0 0 0", float maxDist = 0)
	{
		//! Vanilla
		//! Had to duplicate this so our version of IsObjectObstructedFilter gets used and not the vanilla one

		if (!object)
			return true;
		
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		if (doDistanceCheck && vector.DistanceSq(player.GetPosition(), distanceCheckPos) > maxDist * maxDist)
			return true;

		cache.ObjectCenterPos = object.GetCenter();
			
		if (IsObjectObstructedProxy(object, cache, player))
			return true;

		DayZPhysics.RaycastRV( cache.RaycastStart, cache.ObjectCenterPos, cache.ObjectContactPos, cache.ObjectContactDir, cache.ContactComponent, cache.HitObjects, object, GetGame().GetPlayer(), false, false, ObjIntersectFire, 0.0, CollisionFlags.ALLOBJECTS );

		return IsObjectObstructedFilter(object, cache, player);
	}

	override static bool IsObjectObstructedFilter(Object object, IsObjectObstructedCache cache, PlayerBase player)
	{
		#ifdef EXPANSION_VICINITY_DEBUG
		string tmp;
		int count;
		foreach ( Object potObstrObj : cache.HitObjects )
		{
			if ( tmp )
				tmp += ", ";
			tmp += potObstrObj.ToString() + "(" + potObstrObj.CanObstruct() + ")";
			count++;
			if ( count == 10 || ( count > 0 && cache.HitObjects.Count() < 10 ) )
			{
				EXPrint(object.ToString() + " potentially obstructed by {" + tmp + "}");
				tmp = "";
				count = 0;
			}
		}
		#endif

		//! If target object is item or transport, do additional obstruction check to prevent accessing vicinity through Expansion basebuilding
		bool isTargetItemOrTransport = object.IsInherited( ItemBase ) || object.IsTransport();
		bool doAdditionalCheck = ObstructingObjectsContainExpansionBaseBuildingOrTent && isTargetItemOrTransport;

		foreach ( Object potentiallyObstructingObject : cache.HitObjects )
		{
			if ( potentiallyObstructingObject.CanObstruct() )
			{
				//! If potentially obstructing object is not Expansion basebuilding, don't do additional check and return true (obstructed)
				if ( !potentiallyObstructingObject.IsInherited( ExpansionBaseBuilding ) )
					return true;

				//! Potentially obstructing object is Expansion basebuilding

				doAdditionalCheck = isTargetItemOrTransport;

				if ( doAdditionalCheck )
					break;

				//! If not doing additional check, return true (obstructed)
				return true;
			} else
			{
				//! Fix accessing vicinity through vanilla tents as well for good measure
				TentBase tent = TentBase.Cast( potentiallyObstructingObject );
				if ( tent && tent.GetState() == TentBase.PITCHED )
				{
					doAdditionalCheck = isTargetItemOrTransport;

					if ( doAdditionalCheck )
						break;
				}
			}
		}

		if ( doAdditionalCheck )
		{
			//! We are only interested in large items
			PhxInteractionLayers layerMask = PhxInteractionLayers.ITEM_LARGE;

			vector hitPosition;

			if ( DayZPhysics.SphereCastBullet( cache.RaycastStart, cache.ObjectCenterPos, 0.1, layerMask, player, NULL, hitPosition, NULL, NULL ) )
			{
				//! Check if the object itself was hit or something in front.
				//! As hitObject from RayCastBullet/SphereCastBullet is always NULL (no point in even passing the variable in,
				//! it will never get set), we have to do some jumping though hoops to determine what was actually hit.

				bool hasCollisionBox;
				vector minMax[2];
				//! Ignore vanilla territoryflag, tents and fireplaces as their collision box info isn't accurate
				if ( object.IsInherited( ExpansionBaseBuilding ) )
					hasCollisionBox = ExpansionBaseBuilding.Cast( object ).ExpansionGetCollisionBox( minMax );
				else if ( !object.IsInherited( TerritoryFlag ) && !object.IsInherited( TentBase ) && !object.IsInherited( Fireplace ) )
					hasCollisionBox = object.GetCollisionBox( minMax );
				if ( hasCollisionBox )
				{
					//! We have collision box info and going to assume it's accurate. Fire a short ray from hit position
					//! in the direction the object center would be if object was hit and check if it intersects with collision box
					//! to determine if we have hit the object (= not obstructed) or not (= hit something in front, obstructed).
					vector min = minMax[0];
					vector max = minMax[1];
					vector start = object.WorldToModel( hitPosition );
					//! ObjectCenterPos may not be actual object center (usually closer to the bottom), so a ray following that
					//! direction could come in at a steep angle if the player is very close and thus fail to intersect anything
					//! ("scraping the surface"). Get actual collision box center and use that instead.
					//! This will only point to object center if original hit was on object surface (this is intended).
					vector center = ( min + max ) * 0.5;
					vector dir = vector.Direction( start, center );
					dir.Normalize();
					vector end = start + dir * 0.1;  //! Short offset to make sure the ray intersects if original hit was on object
					float intersect = Math3D.IntersectRayBox( start, end, min, max );
					#ifdef EXPANSION_VICINITY_DEBUG
					EXPrint(object.ToString() + " hit " + hitPosition + " min " + object.ModelToWorld( min ) + " max " + object.ModelToWorld( max ) + " intersect " + intersect);
					#endif
					return intersect < 0;
				}

				//! Fall back to radius-based check. If distance from hit position to ObjectCenterPos is smaller than radius,
				//! we assume to have hit the object (= not obstructed), else we assume to have hit something in front (= obstructed)
				float radius;
				if ( object.IsInherited( TerritoryFlag ) )
					radius = 1.5;
				else
					radius = 0.5;
				float dist = vector.Distance( hitPosition, cache.ObjectCenterPos );
				#ifdef EXPANSION_VICINITY_DEBUG
				EXPrint(object.ToString() + " hit " + hitPosition + " radius " + radius + " dist " + dist);
				#endif
				return dist > radius;
			}
			#ifdef EXPANSION_VICINITY_DEBUG
			else
			{
				EXPrint(object.ToString() + " no hit");
			}
			#endif
		}

		#ifdef EXPANSION_VICINITY_DEBUG
		EXPrint(object.ToString() + " not obstructed");
		#endif
		
		return false;
	}
}
