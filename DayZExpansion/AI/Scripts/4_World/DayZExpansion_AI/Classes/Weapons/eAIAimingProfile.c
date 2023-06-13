class eAIAimingProfile
{
	eAIBase m_Unit;

	void eAIAimingProfile(eAIBase ai)
	{
		m_Unit = ai;
	}

	bool Get(out vector position, out vector direction)
	{
		vector transform[4];
		m_Unit.GetTransform(transform);

		//! TODO: Accuracy could be influenced by stamina and status effects
		// m_Unit.GetStatStamina().Get();
		// m_Unit.GetHealth("","");
		// m_Unit.GetStatHeatComfort().Get(); <= freezing ?
		// m_Unit.GetAimingModel().GetSwayWeight();

		position = m_Unit.GetBonePositionWS(m_Unit.GetBoneIndexByName("neck"));

		//! 100% guarantee to hit target
		direction = vector.Direction(position, m_Unit.GetAimPosition());

		PlayerBase targetPlayer;
		if (Class.CastTo(targetPlayer, m_Unit.GetTarget().GetEntity()))
		{
			//! More complex accuracy if target is a player/AI

			float distSq = direction.LengthSq();

			float accuracyMin = m_Unit.m_eAI_AccuracyMin;
			float accuracyMax = m_Unit.m_eAI_AccuracyMax;

			Weapon_Base weapon;
			ItemOptics optics;
			if (accuracyMin < 1 && Class.CastTo(weapon, m_Unit.GetHumanInventory().GetEntityInHands()) && Class.CastTo(optics, weapon.GetAttachedOptics()))
			{
				float zoomMin = optics.GetZeroingDistanceZoomMin();
				float zoomMax = optics.GetZeroingDistanceZoomMax();
				float zoomMinSq = zoomMin * zoomMin;
				float zoomMaxSq = zoomMax * zoomMax;

				//! If target distance is within zeroing range, give accuracy bonus.
				//! Gradual dropoff due to squared values.
				if (zoomMaxSq > 0 && distSq >= zoomMinSq)
				{
					accuracyMin = ExpansionMath.LinearConversion(0, zoomMaxSq + zoomMinSq, distSq, accuracyMax, accuracyMin);
					if (distSq <= zoomMaxSq)
						accuracyMax = ExpansionMath.LinearConversion(zoomMinSq, zoomMaxSq, distSq, 1.0, accuracyMax);
				}
			}

			float inaccuracyLR = 1.0 - Math.RandomFloat(accuracyMin, accuracyMax);
			float inaccuracyUD = 1.0 - Math.RandomFloat(accuracyMin, accuracyMax);

			//! Accuracy influenced by target movement speed and angle
			//vector aimOrientation = direction.InvMultiply3(transform).VectorToAngles();
			vector aimOrientation = m_Unit.GetAimRelAngles();
			float distClamped = Math.Clamp(Math.Sqrt(distSq), 100, 1000);
			float targetSpeedMult = targetPlayer.Expansion_GetMovementSpeed() / 3.0;
			float targetMovementAngle = targetPlayer.Expansion_GetMovementAngle();
			float targetMovementAngleMult;
			if (Math.AbsFloat(targetMovementAngle) < 180)
			{
				targetMovementAngleMult = targetMovementAngle / 90.0;
				if (targetMovementAngleMult < -1)
					targetMovementAngleMult += 2;
				else if (targetMovementAngleMult > 1)
					targetMovementAngleMult -= 2;
			}
			if (Math.RandomIntInclusive(0, 1))
				inaccuracyLR = -inaccuracyLR;
			if (Math.RandomIntInclusive(0, 10))
				inaccuracyUD = -inaccuracyUD * 1.5;
			aimOrientation[0] = aimOrientation[0] + inaccuracyLR * (281.25 / distClamped) * (1 + targetSpeedMult * targetMovementAngleMult);
			aimOrientation[1] = aimOrientation[1] + inaccuracyUD * (281.25 / distClamped) * (0.5 + targetSpeedMult);
			direction = aimOrientation.AnglesToVector().Multiply3(transform);
		}

		direction.Normalize();

		return true;
	}
};