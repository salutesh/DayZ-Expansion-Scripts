//! Fix for vanilla bug of being able to access barrels etc through tent windows from certain angles
modded class ActionTargets
{
	override private float ComputeUtility(Object pTarget, vector pRayStart, vector pRayEnd, Object cursorTarget, vector hitPos, SurfaceInfo surfaceInfo)
	{
		float utility = super.ComputeUtility(pTarget, pRayStart, pRayEnd, cursorTarget, hitPos, surfaceInfo);

		ItemBase targetItem;
		if (pTarget == cursorTarget && utility > 0 && Class.CastTo(targetItem, pTarget))
		{
			auto settings = GetExpansionSettings().GetBaseBuilding(false);
			if (settings.PreventItemAccessThroughObstructingItems)
			{
				PhxInteractionLayers layerMask;
				layerMask |= PhxInteractionLayers.BUILDING;
				layerMask |= PhxInteractionLayers.DOOR;
				layerMask |= PhxInteractionLayers.VEHICLE;
				layerMask |= PhxInteractionLayers.ROADWAY;
				layerMask |= PhxInteractionLayers.TERRAIN;
				layerMask |= PhxInteractionLayers.ITEM_LARGE;
				layerMask |= PhxInteractionLayers.ITEM_SMALL;
				layerMask |= PhxInteractionLayers.FENCE;

				Object hitObject;
				vector hitPosition;
				vector hitNormal;
				float hitFraction;

				if (DayZPhysics.RayCastBullet(pRayStart, pRayEnd, layerMask, m_Player, hitObject, hitPosition, hitNormal, hitFraction))
				{
					//! If hit object is item that can obstruct and is not attached to target item (or vice versa),
					//! prevent interaction by setting utility to -1
					ItemBase hitItem;
					if (Class.CastTo(hitItem, hitObject) && hitItem.Expansion_CanObstruct() && hitItem.GetHierarchyRoot() != targetItem.GetHierarchyRoot())
						return -1;
				}
			}
		}

		return utility;
	}
}
