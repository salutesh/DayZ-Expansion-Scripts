//! @note vehicles are only targeted by AI if a targeted player jumps out a moving vehicle (ActionGetOutTransport) or gets hit by it,
//! otherwise they are dealt with by player target info

class eAIVehicleTargetInformation: eAIEntityTargetInformation
{
	Transport m_Transport;

	void eAIVehicleTargetInformation(EntityAI target)
	{
		Class.CastTo(m_Transport, target);
	}

	override bool IsInanimate()
	{
		return true;
	}

	override float CalculateThreat(eAIBase ai = null)
	{
		if (m_Transport.IsDamageDestroyed())
			return 0.0;

		float levelFactor = 0.15;

		if (ai)
		{
			if (m_Transport == ai.Expansion_GetParent())
				return 0.0;

			//! Any AI, even passive, will react if vehicle is speeding towards them
			//! Vehicles WITH drivers are handled by player target info
			if (m_Transport.CrewMember(DayZPlayerConstants.VEHICLESEAT_DRIVER))
				return 0.100001;  //! Just above threshold so it doesn't get removed prematurely

			float distance = GetDistance(ai, true) + 0.1;

			float speed, fromTargetDot;
			levelFactor = ProcessVehicleThreat(m_Transport, ai, distance, speed, fromTargetDot);
			//PrintFormat("eAIVehicleTargetInformation dist %1 spd %2 dot %3 lvl %4", distance, speed, fromTargetDot, levelFactor);

			levelFactor *= ai.Expansion_GetVisibility(distance);
		}

		return Math.Clamp(levelFactor, 0.0, 1000000.0);
	}

	static float ProcessVehicleThreat(Transport transport, eAIBase ai, float distance, out float speed = 0.0, out float fromTargetDot = 0.0)
	{
		vector velocity = GetVelocity(transport);
		
		speed = velocity.Length();  //! m/s

		float speedAbs = Math.AbsFloat(speed);

		if (speedAbs < 0.555555)  //! RegisterTransportHit tolerance 2 km/h = 0.5555 m/s
			return 0.0;

		if (!ai.Expansion_CanBeDamaged())
			return ExpansionMath.LinearConversion(0.5, 100, distance, 0.199999, 0.15);

		vector minMax[2];
		float radius = transport.ClippingInfo(minMax);

		if (distance > radius + 0.1)
		{
			distance -= radius;

			if (distance > 100.0)
				return 15.0 / distance;  //! 0.1 at 200 m
			else if (distance > Math.Max(speedAbs, 1.111111) * 9)  //! 10 m at 4 km/h
				return ExpansionMath.LinearConversion(0.5, 100, distance, 0.199999, 0.15);
		}
		else
		{
			distance = 0.1;
		}

		if (ai.GetMovementSpeedLimitUnderThreat() > 0)
		{
			vector angularVelocity = dBodyGetAngularVelocity(transport);
			vector fullVelocity = (velocity + angularVelocity).Normalized();

			vector pos = ai.GetPosition();
			vector tpos = transport.GetPosition();

			vector fromTargetDirection = vector.Direction(tpos, pos).Normalized();

			fromTargetDot = vector.Dot(fullVelocity, fromTargetDirection);

			float minDist = radius * 2.0;

			if (fromTargetDot < 0.97 && (distance > minDist || fromTargetDot < 0.25))
				return ExpansionMath.LinearConversion(0.5, 100, distance, 0.199999, 0.15);

			float travelTime = 2.0;

			//! If AI is within distance that vehicle can travel in travelTime seconds, sidestep
			if (distance < Math.Max(speedAbs * travelTime, minDist))
			{
				bool vehicleCanHit = true;

				vector begPos = pos + "0 0.9 0";
				vector endPos = tpos + "0 0.9 0";

				vector contactPos;
				vector contactDir;
				int contactComponent;

				set<Object> results = new set<Object>;

				if (DayZPhysics.RaycastRV(begPos, endPos, contactPos, contactDir, contactComponent, results, null, ai, false, false, ObjIntersectGeom, 0.2, CollisionFlags.ALLOBJECTS))
				{
					foreach (Object result: results)
					{
						if (result.IsBuilding() || (result.IsPlainObject() && !result.IsScenery()) || result.IsInherited(BaseBuildingBase))
						{
							vehicleCanHit = false;
							break;
						}
					}
				}

				if (!vehicleCanHit)
					return ExpansionMath.LinearConversion(0.5, 100, distance, 0.199999, 0.15);

				vector lr = fromTargetDirection.Perpend();

				#ifdef DIAG_DEVELOPER
				ai.Expansion_DebugObject(98, pos, "ExpansionDebugBox_Red", fromTargetDirection);
				ai.Expansion_DebugObject(100, tpos + fullVelocity, "ExpansionDebugNoticeMe_Orange", fullVelocity);
				#endif

				float duration = ExpansionMath.LinearConversion(0, speedAbs * travelTime, distance, 1.0, travelTime);
				float sideStepAngle;
				float d = Math.Max(distance, 30);
				float dn = d * -0.5;

				//vector dir = transport.GetDirection();
				//vector ori = transport.GetOrientation();
				//vector futureOrientation = (dir + angularVelocity * travelTime).VectorToAngles();
				//float angleDiff = Math.Round(ExpansionMath.AngleDiff2(ori[0], futureOrientation[0]));
				//EXTrace.Print(EXTrace.AI, ai, "cur ori " + ori.AnglesToVector().VectorToAngles() + " future " + futureOrientation + " yaw diff " + angleDiff);
				//bool evadeR;

				////! If AI is on the left of vehicle movement axis (vehicle viewpoint), go right (AI viewpoint)
				////! except if AI is directly in front of vehicle and vehicle turns to the left
				//if (ExpansionMath.Side(tpos - velocity * 1000, tpos + velocity * 1000, pos) > 0 && (fromTargetDot < 0.97 || angleDiff >= 0))
					//evadeR = true;

				if (ExpansionMath.Side(tpos - fullVelocity * 1000, tpos + fullVelocity * 1000, pos) > 0 && ExpansionMath.Side(tpos - velocity * 1000, tpos + velocity * 1000, pos) > 0)
				//if (evadeR)
				{
					//sideStepAngle = 90;  //! If AI is on the left of vehicle movement axis (vehicle viewpoint), go right (AI viewpoint)
					sideStepAngle = 0.0001;  //! Non-zero so ForceSideStep doesn't override it

					#ifdef DIAG_DEVELOPER
					ai.Expansion_DebugObject(99, pos + lr * 2, "ExpansionDebugArrow", lr);
					#endif

					ai.GetPathFinding().OverridePosition(pos + fromTargetDirection * dn + lr * d, true);
				}
				else
				{
					//sideStepAngle = -90;  //! If AI is on the right of vehicle movement axis (vehicle viewpoint), go left (AI viewpoint)
					sideStepAngle = -0.0001;  //! Non-zero so ForceSideStep doesn't override it

					#ifdef DIAG_DEVELOPER
					ai.Expansion_DebugObject(99, pos - lr * 2, "ExpansionDebugArrow", -lr);
					#endif
	
					ai.GetPathFinding().OverridePosition(pos + fromTargetDirection * dn - lr * d, true);
				}

				ai.RaiseWeapon(false);
				ai.eAI_ForceSideStep(duration, transport, sideStepAngle, false);
			}
			else if (!ai.GetHumanInventory().GetEntityInHands() || !ai.GetHumanInventory().GetEntityInHands().IsWeapon())
			{
				return ExpansionMath.LinearConversion(0.5, 100, distance, 0.199999, 0.15);
			}
		}

		float levelFactor = speedAbs / 5.555555;  //! 0.4 at 8 km/h
		levelFactor *= 100.0 / distance;

		return levelFactor;
	}

	override bool ShouldRemove(eAIBase ai = null)
	{
		return GetThreat(ai) <= 0.1;
	}

	override vector GetAimOffset(eAIBase ai = null)
	{
		vector minMax[2];
		if (m_Transport.GetCollisionBox(minMax))
		{
			vector offset = Vector(0, (minMax[1][1] - minMax[0][1]) / 2.0, 0);
			return offset;
		}

		return "0 0 0";
	}
};
