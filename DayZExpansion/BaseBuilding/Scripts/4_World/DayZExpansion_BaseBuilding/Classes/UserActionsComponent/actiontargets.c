//! Fix for vanilla bug of being able to access barrels etc through tent windows from certain angles
modded class ActionTargets
{
	override private float ComputeUtility(Object pTarget, vector pRayStart, vector pRayEnd, Object cursorTarget, vector hitPos)
	{
		float utility = super.ComputeUtility(pTarget, pRayStart, pRayEnd, cursorTarget, hitPos);

		if (pTarget == cursorTarget && utility > 0 && pTarget.IsItemBase())
		{
			PhxInteractionLayers layerMask = PhxInteractionLayers.ITEM_LARGE | PhxInteractionLayers.ITEM_SMALL | PhxInteractionLayers.BUILDING;

			Object hitObject;
			vector hitPosition;
			vector hitNormal;
			float hitFraction;

			if (DayZPhysics.RayCastBullet(pRayStart, pRayEnd, layerMask, m_Player, hitObject, hitPosition, hitNormal, hitFraction))
			{
				if (hitObject != pTarget)
					return -1;
			}
		}

		return utility;
	}
}
