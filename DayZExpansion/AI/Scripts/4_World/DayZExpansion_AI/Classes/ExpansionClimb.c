// very ugly, big mess
// could be worse :-) -lava76
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

	static bool DoClimbTest(eAIBase pPlayer, SHumanCommandClimbResult pResult)
	{
		pResult.m_fClimbHeight = 0;
		pResult.m_bIsClimb = false;
		pResult.m_bIsClimbOver = false;
		pResult.m_bFinishWithFall = false;
		pResult.m_bHasParent = false;
		pResult.m_GrabPointParent = null;
		
		
		SHumanCommandClimbSettings hcls = pPlayer.GetDayZPlayerType().CommandClimbSettingsW();

		if (pPlayer.m_MovementState.m_iMovement != DayZPlayerConstants.MOVEMENTIDX_IDLE)
			hcls.m_fFwMaxDistance = 2.5;
		else
			hcls.m_fFwMaxDistance = 1.2;

		vector p0;
		vector p1;
		float radius = hcls.m_fCharWidth;
		bool hit;
		vector hitPosition;
		
		vector direction = pPlayer.GetDirection();

		p0 = pPlayer.GetPosition() + Vector(0, hcls.m_fFwMinHeight + radius, 0);
		p1 = p0 + (direction * hcls.m_fFwMaxDistance);

		RaycastRVParams params(p0, p1, pPlayer, radius);
		params.flags = CollisionFlags.NEARESTCONTACT;
		params.type = ObjIntersectGeom;
		//params.sorted = true;
		params.radius = radius;
		params.ignore = pPlayer;
		params.with = pPlayer;
		
		array<ref RaycastRVResult> results();
		array<Object> excluded();
		excluded.Insert(pPlayer);

		hit = DayZPhysics.RaycastRVProxy(params, results, excluded);
		if (!hit)
		{
			return false;
		}
		
		foreach (auto resultDistance : results)
		{			
			hitPosition = resultDistance.pos + (resultDistance.dir * radius);
#ifndef SERVER
			pPlayer.AddShape(Shape.CreateSphere(0xFFFFFF00, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, hitPosition, radius));
#endif
			break;
		}
		
		results.Clear();
		
		p0 = hitPosition - Vector(0, hcls.m_fFwMinHeight + radius, 0);
		p1 = p0 + Vector(0, hcls.m_fFwMaxHeight + radius, 0);
		
		params.begPos = p1;
		params.endPos = p0;
		
		hit = DayZPhysics.RaycastRVProxy(params, results, excluded);
		if (!hit)
		{
			return false;
		}
				
		foreach (auto result : results)
		{
			if (!result.obj || result.obj.IsTree() || result.obj.IsBush() || result.obj.IsInherited(Man))
				continue;

			hitPosition = result.pos + (result.dir * radius);
						
			pResult.m_fClimbHeight = hitPosition[1] - pPlayer.GetPosition()[1];
			
			if (pResult.m_fClimbHeight < hcls.m_fFwMinHeight)
			{
#ifndef SERVER
				pPlayer.AddShape(Shape.CreateSphere(0xFFFF0000, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, hitPosition, radius));
#endif
				continue;
			}
			
			if (pResult.m_fClimbHeight > hcls.m_fFwMaxHeight)
			{
#ifndef SERVER
				pPlayer.AddShape(Shape.CreateSphere(0xFFFF0000, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, hitPosition, radius));
#endif
				continue;
			}
			
			vector grabPoint = hitPosition;
			vector standPoint = hitPosition + (direction * (hcls.m_fStepPositionAfterEdge + radius));
			vector grabPointMS = result.obj.WorldToModel(grabPoint);
			vector standPointMS = result.obj.WorldToModel(standPoint);

			//! Prevent bogus vaulting/climbing on flat ground or through roofs
			if (grabPointMS[1] < 0 || standPointMS[1] < 0)
				continue;

			//! Determine if we want to climb, climb over, or both (yes that's a thing)
			bool climbOver = false;

			array<ref RaycastRVResult> results2();

			//! Do another raycast from slightly above target standpoint in direction the AI is facing to ground,
			//! if hit position is below standpoint, set climb result accordingly to enable vaulting
			vector begPos = standPoint + "0 1.5 0";
			vector offset = begPos + direction;
			float surfaceY = GetGame().SurfaceY(offset[0], offset[2]);
			vector endPos = Vector(offset[0], surfaceY - 1, offset[2]);

			RaycastRVParams params2(begPos, endPos, pPlayer);
			params2.flags = CollisionFlags.NEARESTCONTACT;
			params2.type = ObjIntersectGeom;
			//params2.sorted = true;
			//params2.radius = radius;
			params2.ignore = pPlayer;
			params2.with = pPlayer;

			if (DayZPhysics.RaycastRVProxy(params2, results2, excluded))
			{
				if (standPoint[1] - results2[0].pos[1] > 0.1)
					climbOver = true;
			}

			pResult.m_bIsClimb = pResult.m_fClimbHeight > 1.1 || !climbOver;
			pResult.m_bIsClimbOver = climbOver;
			pResult.m_bFinishWithFall = !climbOver;
			pResult.m_bHasParent = true;

			pResult.m_ClimbGrabPoint = grabPointMS;

			if (pResult.m_bIsClimb)
				pResult.m_ClimbGrabPointNormal = vector.Zero;
			else
				pResult.m_ClimbGrabPointNormal = direction;

			if (pResult.m_bIsClimb)
				pResult.m_ClimbStandPoint = standPointMS;
			else
				pResult.m_ClimbStandPoint = vector.Zero;

			if (climbOver)
				pResult.m_ClimbOverStandPoint = results2[0].pos;  //! This one in world coordinates! Comment in vanilla human.c is wrong!
			else
				pResult.m_ClimbOverStandPoint = vector.Zero;

			pResult.m_GrabPointParent = result.obj;

			if (pResult.m_bIsClimb)
				pResult.m_ClimbStandPointParent = result.obj;
			else
				pResult.m_ClimbStandPointParent = null;

			//! XXX: Assigning climb over standpoint parent sometimes seemingly deletes the AI. Just always assign null
			//if (climbOver && results2[0].obj != result.obj)
				//pResult.m_ClimbOverStandPointParent = results2[0].obj;
			//else
				pResult.m_ClimbOverStandPointParent = null;
			
#ifndef SERVER
			pPlayer.AddShape(Shape.CreateSphere(0xFF00FF00, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, hitPosition, radius));
#endif
			break;
		}
		
		if (!pResult.m_bIsClimb && !pResult.m_bIsClimbOver)
		{
			return false;
		}

		return true;
	}
};
