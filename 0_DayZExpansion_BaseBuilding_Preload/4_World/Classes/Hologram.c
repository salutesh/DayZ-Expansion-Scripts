/**
 * ExpansionHologram.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		Hologram
 * @brief		
 **/
modded class Hologram
{
	//! Hack fix to prevent the issue of placeable items snapping to center of Expansion base building.
	//! Almost same as vanilla GetProjectionEntityPosition, except we do an additional RayCastBullet if the hit object is inherited from ExpansionBaseBuilding.
	override protected vector GetProjectionEntityPosition(PlayerBase player)
	{
		float minProjectionDistance;
		float maxProjectionDistance; 
		m_ContactDir = vector.Zero;
		vector minMax[2];
		float projectionRadius = GetProjectionRadius();
		float cameraToPlayerDistance = vector.Distance(GetGame().GetCurrentCameraPosition(), player.GetPosition());

		if (projectionRadius < SMALL_PROJECTION_RADIUS) // objects with radius smaller than 1m
		{
			minProjectionDistance = SMALL_PROJECTION_RADIUS;
			maxProjectionDistance = SMALL_PROJECTION_RADIUS * 2;
		}
		else
		{
			minProjectionDistance = projectionRadius;
			maxProjectionDistance = projectionRadius * 2;
			maxProjectionDistance = Math.Clamp(maxProjectionDistance, SMALL_PROJECTION_RADIUS, LARGE_PROJECTION_DISTANCE_LIMIT);
		}
		
		vector from = GetGame().GetCurrentCameraPosition();
		//adjusts raycast origin to player head approx. level (limits results behind the character)
		if (DayZPlayerCamera3rdPerson.Cast(player.GetCurrentCamera()))
		{
			vector head_pos;
			MiscGameplayFunctions.GetHeadBonePos(player,head_pos);
			float dist = vector.Distance(head_pos,from);
			from = from + GetGame().GetCurrentCameraDirection() * dist;
		}
		
		vector to = from + (GetGame().GetCurrentCameraDirection() * (maxProjectionDistance + cameraToPlayerDistance));
		vector contactPosition;
		set<Object> hitObjects = new set<Object>;

		//! @note vanilla changed raycast from ObjIntersectView to ObjIntersectFire in 1.16, which breaks some 3rd party mods including our basebuilding.
		//! Override it back to ObjIntersectView which works just fine for vanilla and 3rd party mods.
		DayZPhysics.RaycastRV( from, to, contactPosition, m_ContactDir, m_ContactComponent, hitObjects, player, m_Projection, false, false, ObjIntersectView );

		//! will not push hologram up when there is direct hit of an item
		if (!CfgGameplayHandler.GetDisableIsCollidingBBoxCheck())
		{
			if (hitObjects.Count() > 0 && hitObjects[0].IsInherited(InventoryItem))
				contactPosition = hitObjects[0].GetPosition();
		}

		static const float raycastOriginOffsetOnFail = 0.25;
		static const float minDistFromStart = 0.01;
		// Camera isn't correctly positioned in some cases, leading to raycasts hitting the object directly behind the camera
		if ((hitObjects.Count() > 0) && (vector.DistanceSq(from, contactPosition) < minDistFromStart))
		{
			from = contactPosition + GetGame().GetCurrentCameraDirection() * raycastOriginOffsetOnFail;
			//! @note vanilla changed raycast from ObjIntersectView to ObjIntersectFire in 1.16, which breaks some 3rd party mods including our basebuilding.
			//! Override it back to ObjIntersectView which works just fine for vanilla and 3rd party mods.
			DayZPhysics.RaycastRV( from, to, contactPosition, m_ContactDir, m_ContactComponent, hitObjects, player, m_Projection, false, false, ObjIntersectView );
		}
		
		if ((hitObjects.Count() > 0) && hitObjects[0].IsInherited(Watchtower))
			contactPosition = CorrectForWatchtower( m_ContactComponent, contactPosition, player, hitObjects[0] );

		//! START part that is different from vanilla GetProjectionEntityPosition
		if ( hitObjects.Count() > 0 && hitObjects[0].IsInherited( ExpansionBaseBuilding ) )
		{
			PhxInteractionLayers layerMask;
			layerMask |= PhxInteractionLayers.BUILDING;
			layerMask |= PhxInteractionLayers.DOOR;
			layerMask |= PhxInteractionLayers.VEHICLE;
			layerMask |= PhxInteractionLayers.ROADWAY;
			layerMask |= PhxInteractionLayers.TERRAIN;
			layerMask |= PhxInteractionLayers.ITEM_SMALL;
			layerMask |= PhxInteractionLayers.ITEM_LARGE;
			layerMask |= PhxInteractionLayers.FENCE;

			vector hitNormal;
			if ( !DayZPhysics.RayCastBullet( from, to, layerMask, m_Projection, null, contactPosition, hitNormal, null ) )
				contactPosition = to;
			else if ( hitObjects[0].IsInherited( ExpansionFloorBase ) || hitObjects[0].IsInherited( ExpansionRampBase ) )
				m_ContactDir = hitNormal;
		}
		//! END part that is different from vanilla GetProjectionEntityPosition

		bool isFloating = SetHologramPosition(player.GetPosition(), minProjectionDistance, maxProjectionDistance, contactPosition);
		SetIsFloating(isFloating);
		
		#ifdef DEVELOPER
		DrawDebugArrow(minProjectionDistance, maxProjectionDistance);
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_HOLOGRAM) )
		{
			Debug.DrawSphere(GetProjectionPosition(), 0.1, 0x99FF0000, ShapeFlags.ONCE|ShapeFlags.TRANSP|ShapeFlags.NOOUTLINE);
		}
		#endif
		
		m_FromAdjusted = from;
		
		return contactPosition;
	}

	override vector SetOnGround( vector position )
	{
		return HideWhenClose( super.SetOnGround( position ) );  //! Fix for vanilla not assigning the result of calling HideWhenClose
	}
}
