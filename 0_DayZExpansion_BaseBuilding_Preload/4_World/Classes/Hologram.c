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
	override protected vector GetProjectionEntityPosition( PlayerBase player )
	{
		float min_projection_dist;
		float max_projection_dist; 
		m_ContactDir = vector.Zero;
		vector min_max[2];
		float projection_radius = GetProjectionRadius();
		float camera_to_player_distance = vector.Distance( GetGame().GetCurrentCameraPosition(), player.GetPosition() );

		if( projection_radius < SMALL_PROJECTION_RADIUS )	// objects with radius smaller than 1m
		{
			min_projection_dist = SMALL_PROJECTION_RADIUS;
			max_projection_dist = SMALL_PROJECTION_RADIUS * 2;
		}
		else
		{
			min_projection_dist = projection_radius;
			max_projection_dist = projection_radius * 2;
			max_projection_dist = Math.Clamp( max_projection_dist, SMALL_PROJECTION_RADIUS, LARGE_PROJECTION_DISTANCE_LIMIT );
		}
		
		vector from = GetGame().GetCurrentCameraPosition();
		//adjusts raycast origin to player head approx. level (limits results behind the character)
		if ( DayZPlayerCamera3rdPerson.Cast(player.GetCurrentCamera()) )
		{
			vector head_pos;
			MiscGameplayFunctions.GetHeadBonePos(player,head_pos);
			float dist = vector.Distance(head_pos,from);
			from = from + GetGame().GetCurrentCameraDirection() * dist;
		}
		
		vector to = from + ( GetGame().GetCurrentCameraDirection() * ( max_projection_dist + camera_to_player_distance ) );
		vector contact_pos;
		set<Object> hit_object = new set<Object>;

		DayZPhysics.RaycastRV( from, to, contact_pos, m_ContactDir, m_ContactComponent, hit_object, player, m_Projection, false, false, ObjIntersectFire );

		static const float raycastOriginOffsetOnFail = 0.25;
		static const float minDistFromStart = 0.01;
		// Camera isn't correctly positioned in some cases, leading to raycasts hitting the object directly behind the camera
		if ((hit_object.Count() > 0) && (vector.DistanceSq(from, contact_pos) < minDistFromStart))
		{
			from = contact_pos + GetGame().GetCurrentCameraDirection() * raycastOriginOffsetOnFail;
			DayZPhysics.RaycastRV( from, to, contact_pos, m_ContactDir, m_ContactComponent, hit_object, player, m_Projection, false, false, ObjIntersectFire );
		}
		
		if ((hit_object.Count() > 0) && hit_object[0].IsInherited(Watchtower))
			contact_pos = CorrectForWatchtower( m_ContactComponent, contact_pos, player, hit_object[0] );

		//! START part that is different from vanilla GetProjectionEntityPosition
		if ( hit_object.Count() > 0 && hit_object[0].IsInherited( ExpansionBaseBuilding ) )
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
			if ( !DayZPhysics.RayCastBullet( from, to, layerMask, m_Projection, null, contact_pos, hitNormal, null ) )
				contact_pos = to;
			else if ( hit_object[0].IsInherited( ExpansionFloorBase ) || hit_object[0].IsInherited( ExpansionRampBase ) )
				m_ContactDir = hitNormal;
		}
		//! END part that is different from vanilla GetProjectionEntityPosition

		float player_to_projection_distance = vector.Distance( player.GetPosition(), contact_pos );
		vector player_to_projection_vector;

		//hologram is at min distance from player
		if ( player_to_projection_distance <= min_projection_dist )
		{
			player_to_projection_vector = contact_pos - player.GetPosition();		  
			player_to_projection_vector.Normalize();
			//prevents the hologram to go underground/floor while hologram exceeds min_projection_dist
			player_to_projection_vector[1] = player_to_projection_vector[1] + PROJECTION_TRANSITION_MIN;
			
			contact_pos = player.GetPosition() + (player_to_projection_vector * min_projection_dist);
			SetIsFloating( true );
		}
		//hologram is at max distance from player
		else if ( player_to_projection_distance >= max_projection_dist )
		{
			player_to_projection_vector = contact_pos - player.GetPosition();
			player_to_projection_vector.Normalize();
			//prevents the hologram to go underground/floor while hologram exceeds max_projection_dist
			player_to_projection_vector[1] = player_to_projection_vector[1] + PROJECTION_TRANSITION_MAX;
			
			contact_pos = player.GetPosition() + (player_to_projection_vector * max_projection_dist);
			SetIsFloating( true );
		}
		//hologram is between min and max distance from player
		else
		{
			SetIsFloating( false );
		}
		m_FromAdjusted = from;
		
		#ifdef DEVELOPER
		DrawDebugArrow(min_projection_dist, max_projection_dist);
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_HOLOGRAM) )
			Debug.DrawSphere(GetProjectionPosition(), 0.1, 0x99FF0000, ShapeFlags.ONCE|ShapeFlags.TRANSP|ShapeFlags.NOOUTLINE);
		#endif
		
		return contact_pos;
	}

	override vector SetOnGround( vector position )
	{
		return HideWhenClose( super.SetOnGround( position ) );  //! Fix for vanilla not assigning the result of calling HideWhenClose
	}
}
