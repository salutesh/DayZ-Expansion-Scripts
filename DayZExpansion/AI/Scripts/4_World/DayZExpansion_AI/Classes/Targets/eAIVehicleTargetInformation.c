//! @note vehicles are only targeted by AI if a taegeted player jumps out a moving vehicle (ActionGetOutTransport),
//! otherwise they are dealt with by player target info

class eAIVehicleTargetInformation: eAIEntityTargetInformation
{
	Transport m_Transport;

	void eAIVehicleTargetInformation(EntityAI target)
	{
		Class.CastTo(m_Transport, target);
	}

	override float CalculateThreat(eAIBase ai = null)
	{
		if (m_Transport.IsDamageDestroyed())
			return 0.0;

		float levelFactor = 0.15;

		if (ai)
		{
			if (m_Transport == ai.GetParent())
				return 0.0;

			float distance = GetDistance(ai, true) + 0.1;

			//! Any AI, even passive, will react if vehicle is speeding towards them
			//! Vehicles WITH drivers are handled by player target info
			if (distance > 100.0)
			{
				levelFactor = 15.0 / distance;  //! 0.1 at 200 m
			}
			else if (!m_Transport.CrewMember(DayZPlayerConstants.VEHICLESEAT_DRIVER))
			{
				float speed, fromTargetDot;
				levelFactor = ProcessVehicleThreat(m_Transport, ai, distance, speed, fromTargetDot);
				//PrintFormat("eAIVehicleTargetInformation dist %1 spd %2 dot %3 lvl %4", distance, speed, fromTargetDot, levelFactor);
			}
		}

		return Math.Clamp(levelFactor, 0.0, 1000000.0);
	}

	static float ProcessVehicleThreat(Transport transport, eAIBase ai, float distance, out float speed = 0.0, out float fromTargetDot = 0.0)
	{
		vector minMax[2];
		float radius = transport.ClippingInfo(minMax);

		if (distance > radius + 0.1)
			distance -= radius;
		else
			distance = 0.1;

		Car car;
		if (Class.CastTo(car, transport))
			speed = car.GetSpeedometer();
		else
			speed = GetVelocity(transport).Length() * 3.6;

		float speedAbs = Math.AbsFloat(speed);
		if (distance > Math.Max(speedAbs, 4.0) * 0.833333 * 3)
			return 0.15;

		if (!ai.eAI_IsSideStepping())
		{
			vector vehicleDir = transport.GetDirection();
			vector fromTargetDirection = vector.Direction(transport.GetPosition(), ai.GetPosition()).Normalized();

			if (speed < 0)
				vehicleDir = -vehicleDir;

			fromTargetDot = vector.Dot(vehicleDir, fromTargetDirection);

			if (fromTargetDot < 0.97 && distance > 10)
				return 0.15;

			//! If AI is within distance that vehicle can travel in three seconds, sidestep
			if (distance < speedAbs * 0.833333)
			{
				vector transform[4];
				CarScript cs;
				if (Class.CastTo(cs, transport))
					cs.Expansion_EstimateTransform(0.25, transform);  //! Quarter second ahead
				else
					transport.GetTransform(transform);

				if (speed < 0)
					transform[2] = -transform[2];

				#ifdef DIAG
				vector lr = fromTargetDirection.Perpend();
				ai.Expansion_DebugObject(98, ai.GetPosition(), "ExpansionDebugBox_Red", fromTargetDirection);
				#endif

				float duration;
				float angle;

				if (fromTargetDot < 0.5 && distance < 3)
				{
					duration = 1.5;
					angle = -180;
				}
				else if (ExpansionMath.Side(transport.GetPosition() - transform[2] * 1000, transport.GetPosition() + transform[2] * 1000, ai.GetPosition()) > 0)
				{
					duration = 1.5;
					angle = 90;  //! If AI is on the left of vehicle movement axis (vehicle viewpoint), go right (AI viewpoint)

					#ifdef DIAG
					vector posR = ai.GetPosition() + lr * 2;
					ai.Expansion_DebugObject(99, posR, "ExpansionDebugArrow", lr);
					#endif
				}
				else
				{
					duration = 1.5;
					angle = -90;  //! If AI is on the right of vehicle movement axis (vehicle viewpoint), go left (AI viewpoint)
	
					#ifdef DIAG
					vector posL = ai.GetPosition() - lr * 2;
					ai.Expansion_DebugObject(99, posL, "ExpansionDebugArrow", -lr);
					#endif
				}

				ai.eAI_ForceSideStep(duration, transport, angle, angle == -180);
			}
			else if (!ai.GetHumanInventory().GetEntityInHands() || !ai.GetHumanInventory().GetEntityInHands().IsWeapon())
			{
				return 0.15;
			}
		}

		float levelFactor = speedAbs / 20.0;  //! 0.4 at 8 km/h
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
