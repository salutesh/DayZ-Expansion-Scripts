// very ugly, big mess
// could be worse :-) -lava76
// indeed :) - jacob
class ExpansionClimb
{
	//!!! Output of default variables !!!
	// float m_fCharHeight = 1.7
	// float m_fCharWidth = 0.3
	// float m_fFwMinHeight = 0.7
	// float m_fFwMaxHeight = 3.5
	// float m_fFwMaxDistance = 1.2
	// float m_fStepFreeSpace = 0.6
	// float m_fStepPositionAfterEdge = 0.3
	// float m_fStepDownRadius = 0.02
	// float m_fStepMinNormal = 0.866
	// float m_fStepVariation = 0.2
	// float m_fStepMinWidth = 0.4
	// float m_fClimbOverMaxWidth = 0.5
	// float m_fClimbOverMinHeight = 0.6

	static bool DoClimbTest(PlayerBase pPlayer, SHumanCommandClimbResult pResult, bool alwaysAllowClimb = false)
	{
		CopyClimbResult(new SHumanCommandClimbResult(), pResult);

		SHumanCommandClimbSettings hcls = pPlayer.GetDayZPlayerType().CommandClimbSettingsW();

		vector pPos = pPlayer.GetPosition();
		vector pDir = pPlayer.Expansion_GetHeadingVector();

		vector p0;
		vector p1;
		float radius = hcls.m_fCharWidth;
		bool hit;
		vector hitPosition;
		Object hitObject;
		vector hitNormal;
		float hitFraction;

		vector direction;
		vector position;

		float distance;
		float minDistance;
		int processed;

		vector distanceCheck = pPos + Vector(0, hcls.m_fFwMinHeight + radius, 0);

		int layers = PhxInteractionLayers.DEFAULT | PhxInteractionLayers.BUILDING | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.FENCE;

		p0 = distanceCheck + (pDir * radius);
		p1 = distanceCheck + (pDir * (hcls.m_fFwMaxDistance - radius));

		RaycastRVParams params(p0, p1, pPlayer, radius);
		params.flags = CollisionFlags.ALLOBJECTS;
		params.type = ObjIntersectGeom;
		params.radius = radius;
		params.ignore = pPlayer;
		params.with = pPlayer;

		// Perform a forward pass

		array<ref RaycastRVResult> results();

		array<Object> excluded();
		excluded.Insert(pPlayer);

		/*
		hit = DayZPhysics.RaycastRVProxy(params, results, excluded);
		if (!hit)
		{
			return false;
		}

		minDistance = float.MAX;
		foreach (auto resultDistance : results)
		{
			vector resultGrabDir = resultDistance.dir;
			float size = resultGrabDir.Normalize();

			position = resultDistance.pos + (resultGrabDir * radius);
			distance = vector.DistanceSq(position, distanceCheck);

			if (distance < minDistance)
			{
				minDistance = distance;
				hitPosition = position;

				// May have to be flipped
				pResult.m_ClimbGrabPointNormal = resultGrabDir;

				processed++;
			}
		}

		if (processed == 0)
		{
			return false;
		}

		processed = 0;
		results.Clear();
		*/

		if (radius != 0)
		{
			hit = DayZPhysics.SphereCastBullet(p0, p1, radius, layers, pPlayer, hitObject, hitPosition, hitNormal, hitFraction);
		}
		else
		{
			hit = DayZPhysics.RayCastBullet(p0, p1, layers, pPlayer, hitObject, hitPosition, hitNormal, hitFraction);
		}

		if (!hit)
		{
			return false;
		}

		hitNormal.Normalize();

		vector normalXZ = Vector(-hitNormal[0], 0, -hitNormal[2]).Normalized();

		float minDotNormalDirection = 0.595;
		float normalDirectionDot = vector.Dot(normalXZ, pDir);
		if (normalDirectionDot < 0.595)
		{
			return false;
		}

		float distanceScaled = ((1.0 - hitFraction) * (hcls.m_fFwMaxDistance - (radius * 2.0))) / normalDirectionDot;

		if (distanceScaled > hcls.m_fFwMaxDistance)
		{
			return false;
		}

		pResult.m_ClimbGrabPointNormal = hitNormal;

#ifndef SERVER
		//pPlayer.AddShape(Shape.CreateSphere(0xFFFFFF00, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, hitPosition, radius));
#endif

		// Checking to see if the object in front of us has a spot to grab

		p0 = hitPosition;
		p1 = hitPosition;
		p0[1] = pPos[1] + (hcls.m_fClimbOverMinHeight + radius);
		p1[1] = pPos[1] + (hcls.m_fFwMaxHeight - radius);

#ifndef SERVER
		//pPlayer.AddShape(Shape.CreateSphere(0xFF0000FF, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, p0, radius));
		//pPlayer.AddShape(Shape.CreateSphere(0xFF0000FF, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, p1, radius));
#endif

		params.begPos = p1;
		params.endPos = p0;

		hit = DayZPhysics.RaycastRVProxy(params, results, excluded);
		if (!hit)
		{
			return false;
		}

		minDistance = float.MAX;
		foreach (auto rayInfo : results)
		{
			SHumanCommandClimbResult climbResult();
			climbResult.m_ClimbGrabPointNormal = pResult.m_ClimbGrabPointNormal;
			if (ValidateHeight(rayInfo, pPlayer, climbResult, alwaysAllowClimb))
			{
				position = climbResult.m_ClimbGrabPoint;
				distance = vector.DistanceSq(position, pPos);

				if (distance < minDistance)
				{
					minDistance = distance;
					hitPosition = position;

					CopyClimbResult(climbResult, pResult);

					processed++;
				}
			}
		}

		return processed != 0;
	}

	static bool ValidateHeight(RaycastRVResult rayInfo, PlayerBase pPlayer, SHumanCommandClimbResult pResult, bool alwaysAllowClimb = false)
	{
		// The below checks should no longer be needed as we don't cast a ray for those layers
		Object rayHitObject = rayInfo.obj;
		if (!rayHitObject || rayHitObject.IsTree() || rayHitObject.IsBush() || rayHitObject.IsMan())
		{
			return false;
		}

		EntityAI rayHitEntity;
		if (Class.CastTo(rayHitEntity, rayHitObject) && (rayHitEntity.IsAnimal() || rayHitEntity.IsZombie()))
		{
			return false;
		}

		if (!alwaysAllowClimb)
		{
			ExpansionModelInfo modelInfo = ExpansionModelInfo.Get(rayHitObject);
			if (!modelInfo || !modelInfo.CanClimb)
			{
				EXTrace.Print(EXTrace.AI, pPlayer, "Cannot climb " + Debug.GetDebugName(rayHitObject));
				return false;
			}
		}

		SHumanCommandClimbSettings hcls = pPlayer.GetDayZPlayerType().CommandClimbSettingsW();

		vector pPos = pPlayer.GetPosition();
		vector pDir = pPlayer.Expansion_GetHeadingVector();

		vector p0;
		vector p1;
		float radius = hcls.m_fCharWidth;
		bool hit;
		vector hitPosition;
		Object hitObject;
		vector hitNormal;
		float hitFraction;

		vector direction = pResult.m_ClimbGrabPointNormal;
		vector position;

		float distance;
		float minDistance;
		int processed;

		RaycastRVParams params(p0, p1, pPlayer, radius);
		params.flags = CollisionFlags.ALLOBJECTS;
		params.type = ObjIntersectGeom;
		radius = 0.05;
		params.radius = radius;
		params.ignore = pPlayer;
		params.with = pPlayer;

		array<ref RaycastRVResult> results();
		array<Object> excluded();
		excluded.Insert(pPlayer);

		hitPosition = rayInfo.pos + (rayInfo.dir.Normalized() * hcls.m_fCharWidth);

		pResult.m_fClimbHeight = hitPosition[1] - pPlayer.GetPosition()[1];

		if (pResult.m_fClimbHeight < hcls.m_fFwMinHeight)
		{
#ifndef SERVER
			//pPlayer.AddShape(Shape.CreateSphere(0xFFFF0000, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, hitPosition, radius));
#endif

			return false;
		}

		if (pResult.m_fClimbHeight > hcls.m_fFwMaxHeight)
		{
#ifndef SERVER
			//pPlayer.AddShape(Shape.CreateSphere(0xFFFF0000, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, hitPosition, radius));
#endif

			return false;
		}

#ifndef SERVER
		//pPlayer.AddShape(Shape.CreateSphere(0xFF00FF00, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, hitPosition, radius));
#endif

		float maxDistanceForValid = (radius * radius) + 0.2;

		position = hitPosition;
		params.begPos = position + Vector(0, hcls.m_fCharHeight - radius, 0);
		params.endPos = position - Vector(0, 0.1, 0);
		hit = DayZPhysics.RaycastRVProxy(params, results, excluded);

		foreach (auto heightResult : results)
		{
			direction = heightResult.dir.Normalized();
			position = heightResult.pos + (direction * radius * 2.0);
			distance = vector.DistanceSq(position, params.endPos);

			if (distance > maxDistanceForValid)
			{
#ifndef SERVER
				//pPlayer.AddShape(Shape.CreateSphere(0xFFAA0022, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, params.begPos, radius));
				//pPlayer.AddShape(Shape.CreateSphere(0xFFAA0022, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, params.endPos, radius));
#endif

				return false;
			}
		}

		pResult.m_bIsClimb = true;
		pResult.m_bFinishWithFall = true;
		pResult.m_bHasParent = true;

		vector transform[4];
		rayHitObject.GetTransform(transform);

		//Print(direction);
		//Print(pResult.m_ClimbGrabPointNormal);
		vector normal = pResult.m_ClimbGrabPointNormal - direction;
		normal.Normalize();
		//Print(normal);

		pResult.m_GrabPointParent = rayHitObject;
		pResult.m_ClimbGrabPointNormal = normal.InvMultiply3(transform);
		pResult.m_ClimbGrabPoint = (position + (normal * 0.05)).InvMultiply4(transform);

		// Check to see if we should be climbing over or stepping

		radius = hcls.m_fCharWidth;
		params.radius = radius;

		position = hitPosition + (pDir * (hcls.m_fClimbOverMaxWidth + hcls.m_fStepMinWidth));
		float freeStepMinHeight = position[1] - hcls.m_fClimbOverMinHeight;
		float freeStepMaxHeight = position[1] + hcls.m_fStepVariation;

		params.begPos = position + Vector(0, hcls.m_fCharHeight - radius, 0);
		params.endPos = position - Vector(0, 10.0, 0);
		hit = DayZPhysics.RaycastRVProxy(params, results, excluded);

#ifndef SERVER
		//pPlayer.AddShape(Shape.CreateSphere(0xFFAAAAFF, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, params.begPos, radius));
		//pPlayer.AddShape(Shape.CreateSphere(0xFFAAAAFF, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, params.endPos, radius));
#endif

		Object freeStepParent;
		vector freeStepPoint = Vector(0, float.MIN, 0);

		foreach (auto freeStepResult : results)
		{
#ifndef SERVER
			//pPlayer.AddShape(Shape.CreateSphere(0xFFAAAAFF, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, freeStepResult.pos, 0.1));
#endif

			if (freeStepPoint[1] > freeStepResult.pos[1])
			{
				continue;
			}

			freeStepParent = freeStepResult.obj;
			freeStepPoint = freeStepResult.pos + (freeStepResult.dir.Normalized() * radius);

			processed++;
		}

		if (processed == 0)
		{
			if (results.Count() != 0)
			{
				return false;
			}
		}

		if (freeStepPoint[1] >= freeStepMaxHeight)
		{
			return false;
		}
		else if (freeStepPoint[1] >= freeStepMinHeight)
		{
			pResult.m_bIsClimbOver = false;
			pResult.m_ClimbStandPointParent = freeStepParent;
			pResult.m_ClimbStandPoint = freeStepParent.WorldToModel(freeStepPoint);
		}
		else
		{
			//! Assigning climb over standpoint parent sometimes seemingly deletes the AI. Just always assign null
			//! This one in world coordinates! Comment in vanilla human.c is wrong!

			pResult.m_bIsClimbOver = true;
			pResult.m_ClimbOverStandPointParent = null;
			pResult.m_ClimbOverStandPoint = freeStepPoint;
		}

		int layers = PhxInteractionLayers.DEFAULT | PhxInteractionLayers.BUILDING | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.FENCE;

		p0 = pPos + Vector(0, hcls.m_fCharHeight * 0.5, 0);
		p1 = pPos + Vector(0, freeStepPoint[1] + hcls.m_fCharHeight, 0);

		hit = DayZPhysics.SphereCastBullet(p0, p1, radius, layers, pPlayer, hitObject, hitPosition, hitNormal, hitFraction);
		//hit = DayZPhysics.RayCastBullet(p0, p1, layers, pPlayer, hitObject, hitPosition, hitNormal, hitFraction);
		if (hit)
		{
			return false;
		}

		return true;
	}

	static void CopyClimbResult(SHumanCommandClimbResult a, out SHumanCommandClimbResult b)
	{
		b.m_bIsClimb = a.m_bIsClimb;
		b.m_bIsClimbOver = a.m_bIsClimbOver;
		b.m_bFinishWithFall = a.m_bFinishWithFall;
		b.m_bHasParent = a.m_bHasParent;
		b.m_fClimbHeight = a.m_fClimbHeight;
		b.m_ClimbGrabPoint = a.m_ClimbGrabPoint;
		b.m_ClimbGrabPointNormal = a.m_ClimbGrabPointNormal;
		b.m_ClimbStandPoint = a.m_ClimbStandPoint;
		b.m_ClimbOverStandPoint = a.m_ClimbOverStandPoint;
		b.m_GrabPointParent = a.m_GrabPointParent;
		b.m_ClimbStandPointParent = a.m_ClimbStandPointParent;
		b.m_ClimbOverStandPointParent = a.m_ClimbOverStandPointParent;
	}

	static void DebugClimb(PlayerBase pPlayer, SHumanCommandClimbResult pResult, int pColorGrab, int pColorStand)
	{
#ifdef DIAG
#ifndef SERVER
		if (!pResult.m_bIsClimb && !pResult.m_bIsClimbOver)
		{
			return;
		}

		vector eTransform[4];

		vector p0;
		vector p1;
		vector points[2];
		float radius = 0.05;
		float length = radius * 10.0;

		p0 = pResult.m_ClimbGrabPoint;
		p1 = p0 + pResult.m_ClimbGrabPointNormal;
		if (pResult.m_GrabPointParent)
		{
			pResult.m_GrabPointParent.GetTransform(eTransform);
			p0 = p0.Multiply4(eTransform);
			p1 = p1.Multiply4(eTransform);
		}

		pPlayer.AddShape(Shape.CreateSphere(pColorGrab, ShapeFlags.NOOUTLINE | ShapeFlags.NOZBUFFER | ShapeFlags.TRANSP, p0, radius));

		points[0] = p0;
		points[1] = p1;
		pPlayer.AddShape(Shape.CreateLines(pColorGrab, ShapeFlags.NOOUTLINE | ShapeFlags.NOZBUFFER | ShapeFlags.TRANSP, points, 2));

		if (pResult.m_bIsClimbOver)
		{
			p0 = pResult.m_ClimbOverStandPoint;

			if (pResult.m_ClimbOverStandPointParent)
			{
				pResult.m_ClimbOverStandPointParent.GetTransform(eTransform);
				p0 = p0.Multiply4(eTransform);
			}

		}
		else if (pResult.m_bIsClimb)
		{
			p0 = pResult.m_ClimbStandPoint;

			if (pResult.m_ClimbStandPointParent)
			{
				pResult.m_ClimbStandPointParent.GetTransform(eTransform);
				p0 = p0.Multiply4(eTransform);
			}
		}

		pPlayer.AddShape(Shape.CreateSphere(pColorStand, ShapeFlags.NOOUTLINE | ShapeFlags.NOZBUFFER | ShapeFlags.TRANSP, p0, radius));
#endif
#endif
	}
};
