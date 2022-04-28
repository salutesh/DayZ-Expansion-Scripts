class eAICommandMove extends ExpansionHumanCommandScript
{
	static const int TURN_STATE_NONE = 0;
	static const int TURN_STATE_TURNING = 1;
	
	private int m_PreviousInteractionLayer;

	private eAIBase m_Unit;
	private ExpansionPathHandler m_PathFinding;

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

		float lookLR = angles[0];
		float lookUD = angles[1];

		if (lookLR > 180) lookLR -= 360;
		if (lookUD > 180) lookUD -= 360;

		if (m_LookLR + 1.0 < lookLR)
			m_LookLR += 1.0;
		else if (m_LookLR - 1.0 > lookLR)
			m_LookLR -= 1.0;
		else
			m_LookLR = lookLR;

		if (m_LookUD + 1.0 < lookUD)
			m_LookUD += 1.0;
		else if (m_LookUD - 1.0 > lookUD)
			m_LookUD -= 1.0;
		else
			m_LookUD = lookUD;
	
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

	void GetUp()
	{
		m_GetUp = true;
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

		vector wayPoint = position;
		bool isFinal = m_PathFinding.GetNext(wayPoint) <= 2;
		float wayPointDistance = vector.DistanceSq(position, wayPoint);

		float minFinal = 0.3;
		if (isFinal && wayPointDistance >= minFinal && wayPointDistance < 1.0)
		{
			//! If distance squared to waypoint is more than minFinal but less than 1.0,
			//! subtract height diff squared from distance squared to make it more likely to fall below minFinal.
			//! Prevents unit rotating in spot in case it cannot reach a waypoint due to height difference.
			float yDiff = Math.AbsFloat(wayPoint[1] - position[1]);
			wayPointDistance -= yDiff * yDiff;
		}

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

		if (m_MovementSpeed != 0)
		{
			vector pathDir = vector.Direction(position, wayPoint).Normalized();
			
			m_TurnTarget = pathDir.VectorToAngles()[0];
		}

		m_Turn = orientation[0];

		if (m_Turn > 180.0) m_Turn = m_Turn - 360.0;
		if (m_TurnTarget > 180.0) m_TurnTarget = m_TurnTarget - 360.0;

		m_TurnDifference = Math.AngleDiff(m_TurnTarget, m_Turn);
		if (m_TurnDifference > 180.0) m_TurnDifference = m_TurnDifference - 360.0;
		if (m_TurnDifference < -180.0) m_TurnDifference = m_TurnDifference + 360.0;
		
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
		float rotation[4];
		vector translation;
		
		PrePhys_GetTranslation(translation);
		PrePhys_GetRotation(rotation);
		
		AnglesToQuat("0 0 0", rotation);
		
		vector position = m_Player.ModelToWorld(translation);

		vector transform[4];
		m_Player.GetTransform(transform);
		
		//! make AI go fast for debugging navigation between large distances
		//translation = translation * Math.Max(2.0 * m_MovementSpeed, 1);
		
		float minSpeed = 2.0 * pDt;
		float minSpeedSq = minSpeed * minSpeed;
		
		float speed = translation.LengthSq();

		vector checkPosition = position + (transform[2] * 0.5);
		int doPseudoJump = 0xFFFF0000;
		
		dBodyEnableGravity(m_Player, true);
			
		if (m_MovementSpeed != 0 && speed < minSpeedSq)
		{
			if (DayZPhysics.SphereCastBullet(checkPosition + Vector(0.0, 0.4, 0.0), position, 0.5, m_CollisionLayerMask, m_Player, m_HitObject, m_HitPosition, m_HitNormal, m_HitFraction)) 
			{
				checkPosition = m_HitPosition;

				float yDiff = checkPosition[1] - position[1];
				
				if (yDiff > 0.01 && yDiff <= 0.4)
				{
					doPseudoJump = 0xFF00FF00;
					
					translation[1] = translation[1] + (yDiff * pDt * 1.1);
					
					dBodyEnableGravity(m_Player, false);
				}
			}			

			//DBGDrawSphere(checkPosition + Vector(0.0, 0.4, 0.0), 0.5, 0xFFFFFF00);
			//DBGDrawSphere(checkPosition, 0.1, 0xFFFFFF00);
			//DBGDrawSphere(checkPosition, 0.05, doPseudoJump);
		}
			

		PrePhys_SetRotation(rotation);
		PrePhys_SetTranslation(translation);
	}

	override bool PostPhysUpdate(float pDt)
	{
		return true;
	}
};
