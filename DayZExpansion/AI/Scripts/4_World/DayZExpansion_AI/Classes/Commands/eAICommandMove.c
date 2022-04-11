class eAICommandMove extends ExpansionHumanCommandScript
{
	static const int TURN_STATE_NONE = 0;
	static const int TURN_STATE_TURNING = 1;
	
	private int m_PreviousInteractionLayer;

	private eAIBase m_Unit;
	private eAIPathFinding m_PathFinding;

	private float m_Turn;
	private float m_TurnTarget;
	private float m_TurnDifference;
	private float m_TurnTime;
	private float m_TurnPrevious;
	private float m_TurnVelocity;
	private int m_TurnState;

	private vector m_Direction;
	private float m_MovementDirection;
	private float m_TargetMovementDirection;

	private bool m_Look;
	private float m_LookLR;
	private float m_LookUD;
	
	private float m_SpeedUpdateTime;
	private float m_MovementSpeed;
	private float m_TargetSpeed;
	private float m_SpeedLimit;
	private bool m_SpeedOverrider;
	
	private vector m_MovementCorrection;

	private PhxInteractionLayers m_CollisionLayerMask = PhxInteractionLayers.ROADWAY|PhxInteractionLayers.BUILDING|PhxInteractionLayers.FENCE|PhxInteractionLayers.VEHICLE;
	private Object m_HitObject; //! always null and low priority fix at BI
	private vector m_HitPosition;
	private vector m_HitNormal;
	private float m_HitFraction;

	private bool m_GetUp;
			
	void eAICommandMove(DayZPlayerImplement player, ExpansionHumanST st)
	{
		Class.CastTo(m_Unit, player);
		
		m_PathFinding = m_Unit.GetPathFinding();
	}
	
	void ~eAICommandMove()
	{
	}

	override void OnActivate()
	{
		SetSpeedLimit(-1);
		
		dBodySetInteractionLayer(m_Player, PhxInteractionLayers.CHARACTER | PhxInteractionLayers.BUILDING | PhxInteractionLayers.DOOR | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.ITEM_LARGE | PhxInteractionLayers.FENCE | PhxInteractionLayers.AI);
	}

	override void OnDeactivate()
	{
	}

	override void SetLookDirection(vector pDirection)
	{
		vector angles = pDirection.VectorToAngles();
		m_LookLR = angles[0];
		m_LookUD = angles[1];
		if (m_LookLR > 180) m_LookLR = m_LookLR - 360;
		if (m_LookUD > 180) m_LookUD = m_LookUD - 360;
		m_Look = (Math.AbsFloat(m_LookLR) > 0.01) || (Math.AbsFloat(m_LookUD) > 0.01);
	}

	void SetTurnTarget(float pTarget)
	{
		m_TurnTarget = pTarget;
	}

	void SetSpeedLimit(float pSpeedIdx)
	{
		m_SpeedLimit = pSpeedIdx;

		if (m_SpeedLimit < 0 || m_SpeedLimit > 3) m_SpeedLimit = 3;
	}
	
	void SetTargetSpeed(float pTarget)
	{
		if (m_TargetSpeed > pTarget && pTarget < 2.0 && m_SpeedUpdateTime < 0.1) return;

		m_SpeedUpdateTime = 0;
		m_TargetSpeed = pTarget;
	}

	float GetCurrentMovementSpeed()
	{
		return m_MovementSpeed;
	}

	void SetSpeedOverrider(bool pActive)
	{
		m_SpeedOverrider = pActive;
	}

	void SetTargetDirection(float pTarget)
	{
		m_TargetMovementDirection = pTarget;
	}

	override void PreAnimUpdate(float pDt)
	{
		super.PreAnimUpdate(pDt);

		m_SpeedUpdateTime += pDt;

		vector debug_points[2];
		
		vector transform[4];
		m_Player.GetTransform(transform);

		vector orientation = Math3D.MatrixToAngles(transform);
		vector position = transform[3];

		float wayPointDistance = 0.0;
		int wayPointIndex;
		vector wayPoint = position;

		bool isFinal = true;
		if (m_PathFinding.Count() >= 2)
		{
			wayPointIndex = m_PathFinding.Next(position);
			wayPoint = m_PathFinding[wayPointIndex];

			float y = GetGame().SurfaceY(wayPoint[0], wayPoint[2]);			
			if (y > wayPoint[1]) wayPoint[1] = y;

			DBGDrawSphere(wayPoint, 0.3, 0xFFFFFFFF);
				
			vector orig_WayPoint = wayPoint;
			if (DayZPhysics.SphereCastBullet(wayPoint + Vector(0.0, 1.5, 0.0), wayPoint - Vector(0.0, 10.0, 0.0), 0.3, m_CollisionLayerMask|PhxInteractionLayers.TERRAIN, m_Player, m_HitObject, m_HitPosition, m_HitNormal, m_HitFraction)) wayPoint = m_HitPosition;
			
			DBGDrawSphere(wayPoint, 0.05, 0xFF0000FF);
			
			wayPointDistance = vector.DistanceSq(wayPoint, position);

			isFinal = wayPointIndex == m_PathFinding.Count() - 1;
		}

		float minFinal = 0.3;

		if (m_MovementSpeed != 0)
		{
			vector pathDir = vector.Direction(position, wayPoint).Normalized();
			m_TurnTarget = pathDir.VectorToAngles()[0];
		}

		m_Turn = orientation[0];

		if (m_Turn > 180.0) m_Turn = m_Turn - 360.0;
		if (m_TurnTarget > 180.0) m_TurnTarget = m_TurnTarget - 360.0;

		m_TurnDifference = m_TurnTarget - m_Turn;
		if (m_TurnDifference > 180.0) m_TurnDifference = m_TurnDifference - 360.0;
		if (m_TurnDifference < -180.0) m_TurnDifference = m_TurnDifference + 360.0;

		if (isFinal && wayPointDistance < minFinal)
		{
			SetTargetSpeed(0.0);
		}
		else if (Math.AbsFloat(m_TurnDifference) > 30.0)
		{
			SetTargetSpeed(1.0);
		}
		else if (!m_SpeedOverrider)
		{
			if (isFinal && wayPointDistance < 8.0)
			{
				SetTargetSpeed(1.0);
			}
			else if (isFinal && wayPointDistance < 20.0)
			{
				SetTargetSpeed(2.0);
			}
			else
			{
				SetTargetSpeed(Math.Lerp(m_TargetSpeed, 3.0, pDt * 2.0));
			}
		}

		m_MovementDirection += Math.Clamp((m_TargetMovementDirection - m_MovementDirection) * pDt, -180.0, 180.0);

		m_MovementSpeed = m_TargetSpeed;
		if (m_MovementSpeed > m_SpeedLimit && m_SpeedLimit != -1) m_MovementSpeed = m_SpeedLimit;	

		m_Table.SetMovementDirection(this, m_MovementDirection);
		m_Table.SetMovementSpeed(this, m_MovementSpeed);

		m_Table.SetLook(this, m_Look);
		m_Table.SetLookDirX(this, m_LookLR);
		m_Table.SetLookDirY(this, m_LookUD);
		
		m_TurnVelocity = m_TurnPrevious - m_Turn;
		m_TurnPrevious = m_Turn;
		if (m_TurnVelocity > 180.0) m_TurnVelocity = m_TurnVelocity - 360.0;
		else if (m_TurnVelocity < -180.0) m_TurnVelocity = m_TurnVelocity + 360.0;
		
		if (m_MovementSpeed == 0)
		{
			switch (m_TurnState)
			{
				case TURN_STATE_NONE:
					if (Math.AbsFloat(m_TurnDifference) > 1)
					{
						m_TurnTime = 0;

						m_Table.CallTurn(this);
						m_Table.SetTurnAmount(this, m_TurnDifference / 90.0);

						m_TurnState = TURN_STATE_TURNING;
					}
					break;
				case TURN_STATE_TURNING:
					m_TurnTime += pDt;
				
					if (m_TurnTime > 2.0)
					{
						m_Table.CallStopTurn(this);

						m_TurnState = TURN_STATE_NONE;
					}			
					else if (Math.AbsFloat(m_TurnVelocity) < 1 * pDt)
					{
						m_Table.CallStopTurn(this);
						
						m_TurnState = TURN_STATE_NONE;
					}

					break;
			}

		}
		else
		{
			m_TurnState = TURN_STATE_NONE;

			auto parent = Object.Cast(m_Player.GetParent());
			if (parent) m_TurnTarget -= parent.GetOrientation()[0];

			PreAnim_SetFilteredHeading(-m_TurnTarget * Math.DEG2RAD, 0.1, 30.0);
		}

		if (m_GetUp)
		{
			m_GetUp = false;
			m_Table.SetStance(this, 0);
		}
	}

	override void PrePhysUpdate(float pDt)
	{
		vector translation;
		PrePhys_GetTranslation(translation);
		vector position = m_Player.ModelToWorld(translation);

		vector transform[4];
		m_Player.GetTransform(transform);

		// TODO: this is only temporary code and a better solution has to be found later on
		// This fix is for when the AI is meant to be moving faster but height elevation is blocking us
		// Reason why this is temporary; it effictively is telling the player to jump.
		m_MovementCorrection = vector.Zero;
		
		//! make AI go fast for debugging navigation between large distances
		//translation = translation * Math.Max(2.0 * m_MovementSpeed, 1);
		
		float minSpeed = 1.0 * pDt;
		float minSpeedSq = minSpeed * minSpeed;
		
		float speed = translation.LengthSq();

		dBodyEnableGravity(m_Player, true);
		if (m_MovementSpeed != 0 && speed < minSpeedSq)
		{
			vector checkPosition = position + (transform[2] * 0.25);

			int doPseudoJump = 0xFFFF0000;
				
			if (DayZPhysics.SphereCastBullet(checkPosition + Vector(0.0, 0.4, 0.0), checkPosition, 0.5, m_CollisionLayerMask, m_Player, m_HitObject, m_HitPosition, m_HitNormal, m_HitFraction)) 
			{
				checkPosition = m_HitPosition;

				doPseudoJump = 0xFF00FF00;

				float yDiff = checkPosition[1] - position[1];
				
				if (yDiff > 0.01 && yDiff <= 0.4)
				{
					m_MovementCorrection = Vector(0, yDiff / pDt, 0);

					translation[1] = translation[1] + (yDiff * pDt * 1.1);
					
					dBodyEnableGravity(m_Player, false);
				}
			}

			DBGDrawSphere(checkPosition + Vector(0.0, 0.4, 0.0), 0.5, 0xFFFFFF00);
			DBGDrawSphere(checkPosition, 0.5, 0xFFFFFF00);
			DBGDrawSphere(checkPosition, 0.05, doPseudoJump);
		}

		PrePhys_SetTranslation(translation);
	}

	override bool PostPhysUpdate(float pDt)
	{
		return true;
	}

	void GetUp()
	{
		m_GetUp = true;
	}
};
