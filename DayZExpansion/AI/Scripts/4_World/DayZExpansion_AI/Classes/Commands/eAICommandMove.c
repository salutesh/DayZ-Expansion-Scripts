class eAICommandMove extends ExpansionHumanCommandScript
{
	static const int TURN_STATE_NONE = 0;
	static const int TURN_STATE_TURNING = 1;
	
	private int m_PreviousInteractionLayer;

	private eAIBase m_Unit;
	private ExpansionPathHandler m_PathFinding;
	private vector m_PrevWaypoint;
	private float m_WayPointDistance;

	private float m_Turn;
	private float m_TurnTarget;
	private float m_TurnDifference;
	private float m_TurnTime;
	private float m_TurnPrevious;
	private float m_TurnVelocity;
	private int m_TurnState;

	private vector m_Direction;
	private float m_MovementDirection;
	private bool m_ForceMovementDirection;
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
	private int m_Stance;
			
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
		SetLookAnglesRel(ExpansionMath.RelAngle(angles[0]), ExpansionMath.RelAngle(angles[1]));
	}

	override void SetLookAnglesRel(float lookLR, float lookUD)
	{
		m_LookLR = lookLR;
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

	void SetTargetDirection(float pTarget, bool force = false)
	{
		m_TargetMovementDirection = pTarget;
		m_ForceMovementDirection = force;
	}

	void GetUp()
	{
		m_GetUp = true;
	}

	void OverrideStance(int stance)
	{
		m_Stance = stance;
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
		m_WayPointDistance = vector.DistanceSq(position, wayPoint);

		float minFinal = 0.3;
		if (isFinal && m_WayPointDistance >= minFinal)
		{
			//! If distance squared to waypoint is more than minFinal, make sure unit can actually reach waypoint.
			//! Prevents unit rotating in spot in case it cannot reach a waypoint due to height difference.

			float y = GetGame().SurfaceY(wayPoint[0], wayPoint[2]);
			if (y > wayPoint[1]) wayPoint[1] = y;
				
			vector orig_WayPoint = wayPoint;
			if (DayZPhysics.SphereCastBullet(wayPoint + Vector(0.0, 1.5, 0.0), wayPoint - Vector(0.0, 10.0, 0.0), 0.3, m_CollisionLayerMask|PhxInteractionLayers.TERRAIN, m_Player, m_HitObject, m_HitPosition, m_HitNormal, m_HitFraction))
				wayPoint = m_HitPosition;

			m_WayPointDistance = vector.DistanceSq(position, wayPoint);
		}

		if (!isFinal && wayPoint == m_PrevWaypoint && m_WayPointDistance < 2.25)
		{
			//! Waypoint is identical to previous waypoint, but not final.
			//! Don't update turn target to prevent AI spinning in place.
			//! All we can do now is hope for better pathfinding next update.
			m_PathFinding.m_Recalculate = true;
			//! Setting speed to jog seems to help against AI not vaulting when it should.
			//! This will be reset in eAIBase::CommandHandler next update
			SetSpeedOverrider(true);
			SetTargetSpeed(2.0);
		}
		else
		{
			m_PrevWaypoint = wayPoint;
			m_PathFinding.m_Recalculate = false;
			if (m_MovementSpeed != 0)
			{
				vector pathDir = vector.Direction(position, wayPoint).Normalized();
				
				m_TurnTarget = pathDir.VectorToAngles()[0];
			}
		}

		m_Turn = orientation[0];

		//! Clockwise: Positive, counter-clockwise: Negative
		m_TurnDifference = ExpansionMath.AngleDiff2(m_Turn, m_TurnTarget);
		
		if (isFinal && m_WayPointDistance < minFinal)
		{
			SetTargetSpeed(0.0);
		}
		else if (Math.AbsFloat(m_TurnDifference) > 30.0)
		{
			SetTargetSpeed(1.0);
		}
		else if (!m_SpeedOverrider)
		{
			if (isFinal && m_WayPointDistance < 8.0)
			{
				SetTargetSpeed(1.0);
			}
			else if (isFinal && m_WayPointDistance < 20.0)
			{
				SetTargetSpeed(2.0);
			}
			else
			{
				SetTargetSpeed(Math.Lerp(m_MovementSpeed, 3.0, pDt * 2.0));
			}
		}
		
		DBGDrawSphere(wayPoint, 0.05, 0xFF00FF00);
		
		float dirChangeSpeed = pDt;
		if (m_ForceMovementDirection)
			dirChangeSpeed *= 4;

		m_MovementDirection = Math.Lerp(m_MovementDirection, m_TargetMovementDirection, dirChangeSpeed);

		m_MovementSpeed = m_TargetSpeed;
		if (m_MovementSpeed > m_SpeedLimit && m_SpeedLimit != -1) m_MovementSpeed = m_SpeedLimit;	

		m_Table.SetMovementDirection(this, m_MovementDirection);
		m_Table.SetMovementSpeed(this, m_MovementSpeed);

		m_Table.SetLook(this, m_Look);
		m_Table.SetLookDirX(this, m_LookLR);
		m_Table.SetLookDirY(this, m_LookUD);
		
		m_TurnVelocity = ExpansionMath.AngleDiff2(m_Turn, m_TurnPrevious);
		m_TurnPrevious = m_Turn;
		
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

			float turnTargetActual = m_TurnTarget;

			auto parent = Object.Cast(m_Player.GetParent());
			if (parent) turnTargetActual -= parent.GetOrientation()[0];

			if (turnTargetActual > 180.0) turnTargetActual = turnTargetActual - 360.0;

			PreAnim_SetFilteredHeading(-turnTargetActual * Math.DEG2RAD, 0.1, 30.0);
		}

		if (m_GetUp)
		{
			m_GetUp = false;
			m_Table.SetStance(this, DayZPlayerConstants.STANCEIDX_ERECT);
		}
		else if (m_Stance)
		{
			m_Table.SetStance(this, m_Stance);
		}
	}
	
	override void PrePhysUpdate(float pDt)
	{
	}

	override bool PostPhysUpdate(float pDt)
	{
		return true;
	}

	float GetWaypointDistance()
	{
		return m_WayPointDistance;
	}
};
