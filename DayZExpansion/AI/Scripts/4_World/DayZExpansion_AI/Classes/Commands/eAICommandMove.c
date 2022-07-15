class eAICommandMove extends ExpansionHumanCommandScript
{
	static const int TURN_STATE_NONE = 0;
	static const int TURN_STATE_TURNING = 1;
	
	static const vector CHECK_MIN_HEIGHT = "0 1.1 0";

	static const int ORIGINAL_WAYPOINT = 0;
	static const int CORRECTED_WAYPOINT = 1;
	static const int CHECK_ORIGIN = 2;
	static const int CHECK_FORWARD_BLOCKED = 3;
	static const int CHECK_LEFT_BLOCKED = 4;
	static const int CHECK_RIGHT_BLOCKED = 5;
	static const int CHECK_LEFT_OK = 6;
	static const int CHECK_RIGHT_OK = 7;
	static const int OVERRIDDEN_WAYPOINT = 8;
	static const int BLOCKED_HITPOSITION = 9;
	static const int CHECK_BACKWARD_BLOCKED = 10;
	static const int CHECK_BACKWARD_OK = 11;
	
	static int s_InstanceCount;
	private int m_InstanceNum;

	private int m_PreviousInteractionLayer;

	private eAIBase m_Unit;
#ifndef EAI_USE_LEGACY_PATHFINDING
	private ExpansionPathHandler m_PathFinding;
#else
	private eAIPathFinding m_PathFinding;
#endif
	private vector m_PrevWaypoint;
	private float m_WaypointTime;
	private float m_WayPointDistance;
	private float m_WayPointDistance2D;

	private float m_Turn;
	private float m_TurnTarget;
	private bool m_ForceTurnTarget;
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
			
	private bool m_LastBlockedForward;
	private bool m_LastBlockedLeft;
	private bool m_LastBlockedRight;
	private bool m_LastBlockedBackward;

	private float m_OverrideMovementTimeout;
	private float m_OverrideTargetMovementDirection;
	private vector m_OverrideWaypoint;

	void eAICommandMove(DayZPlayerImplement player, ExpansionHumanST st)
	{
		Class.CastTo(m_Unit, player);
		
		m_PathFinding = m_Unit.GetPathFinding();

		m_InstanceNum = s_InstanceCount++;
	}
	
	void ~eAICommandMove()
	{
	}

	override void OnActivate()
	{
		SetSpeedLimit(-1);
		
		//! XXX: This doesn't seem to be needed
		//dBodySetInteractionLayer(m_Player, PhxInteractionLayers.CHARACTER | PhxInteractionLayers.BUILDING | PhxInteractionLayers.DOOR | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.ITEM_LARGE | PhxInteractionLayers.FENCE | PhxInteractionLayers.AI);
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

	void SetTurnTarget(float pTarget, bool force = false)
	{
		m_TurnTarget = pTarget;
		m_ForceTurnTarget = force;
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
		if (m_OverrideMovementTimeout <= 0)
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

		vector orientation = Math3D.MatrixToAngles(m_Transform);
		vector position = m_Transform[3];

		vector wayPoint = position;
#ifndef EAI_USE_LEGACY_PATHFINDING
		bool isFinal = m_PathFinding.GetNext(wayPoint) <= 2 && m_Unit.m_eAI_TargetPositionIsFinal;
		//m_WayPointDistance = vector.DistanceSq(position, wayPoint);
		DBGDrawSphere(wayPoint, 0.05, 0xFFFF0000);
		//DBGDrawLine(position, wayPoint, 0xFFFF0000);
		m_Unit.Expansion_DebugObject_Deferred(ORIGINAL_WAYPOINT, wayPoint, "ExpansionDebugBox_Red");
#else
		int wayPointIndex;

		bool isFinal = true;
		if (m_PathFinding.Count() >= 2)
		{
			wayPointIndex = m_PathFinding.Next(position);
			wayPoint = m_PathFinding[wayPointIndex];

			isFinal = wayPointIndex == m_PathFinding.Count() - 1;
		}
#endif

		vector leftPos;
		vector rightPos;
		vector forwardPos;
		vector backwardPos;
		vector outNormal;

		float hitFraction;
		bool blockedForward;
		bool blockedLeft;
		bool blockedRight;
		bool blockedBackward;

		if (m_OverrideMovementTimeout > 0)
		{
			m_OverrideMovementTimeout -= pDt;
		}
		else
		{
			bool chg;
#ifdef DIAG
			string msg;
#endif

			m_PathFinding.StopOverride();

			if (m_MovementSpeed && /*!m_Unit.eAI_HasLOS() &&*/ !m_Unit.eAI_IsClimb())
			{
				vector fb = vector.Direction(position, Vector(wayPoint[0], position[1], wayPoint[2])).Normalized();
				vector checkFwd = position + (0.5 * fb);
				vector checkBwd = position + (-0.5 * fb);
				blockedForward = Raycast(position + CHECK_MIN_HEIGHT, checkFwd + CHECK_MIN_HEIGHT, forwardPos, outNormal, hitFraction, position + fb * m_MovementSpeed);

				if (blockedForward != m_LastBlockedForward)
				{
					m_LastBlockedForward = blockedForward;
					chg = true;
				}

				if (blockedForward)
				{
					vector lr = fb.Perpend();
					vector checkLeft = position + (0.5 * lr);
					vector checkRight = position + (-0.5 * lr);
					blockedLeft = Raycast(position + CHECK_MIN_HEIGHT, checkLeft + CHECK_MIN_HEIGHT, leftPos, outNormal, hitFraction);
					blockedRight = Raycast(position + CHECK_MIN_HEIGHT, checkRight + CHECK_MIN_HEIGHT, rightPos, outNormal, hitFraction);

					if (blockedLeft && blockedRight)
					{
						blockedBackward = Raycast(position + CHECK_MIN_HEIGHT, checkBwd + CHECK_MIN_HEIGHT, rightPos, outNormal, hitFraction);
						if (!blockedBackward)
							backwardPos = checkBwd;
					}

					//if (blockedLeft || blockedRight)
					//{
						//float leftDist;
						//float rightDist;
						//if (blockedLeft)
							//leftDist = vector.DistanceSq(position, leftPos);
						//if (blockedRight)
							//rightDist = vector.DistanceSq(position, rightPos);
						//if (Math.Max(leftDist, rightDist) - Math.Min(leftDist, rightDist) > 0.25)
						//{
							//blockedLeft = leftDist < rightDist;
							//blockedRight = leftDist > rightDist;
						//}
					//}

					//if (blockedLeft && !m_PathFinding.IsBlocked(position, checkRight, rightPos, outNormal))
						rightPos = checkRight;
					//if (blockedRight && !m_PathFinding.IsBlocked(position, checkLeft, leftPos, outNormal))
						leftPos = checkLeft;

					if (blockedLeft != m_LastBlockedLeft)
					{
						m_LastBlockedLeft = blockedLeft;
						chg = true;
					}

					if (blockedRight != m_LastBlockedRight)
					{
						m_LastBlockedRight = blockedRight;
						chg = true;
					}

					if (blockedBackward != m_LastBlockedBackward)
					{
						m_LastBlockedBackward = blockedBackward;
						chg = true;
					}

					if (m_MovementSpeed > 2)
					{
						//! Limiting speed to jog helps obstacle avoidance while strafing
						//! This will be reset in eAIBase::CommandHandler next update
						SetSpeedOverrider(true);
						SetTargetSpeed(2.0);
						m_MovementSpeed = 2;
					}

					m_Unit.Expansion_DebugObject_Deferred(CHECK_ORIGIN, position + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugBox_Blue", fb);
					m_Unit.Expansion_DebugObject_Deferred(CHECK_FORWARD_BLOCKED, checkFwd + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow_Red", fb);
					if (blockedLeft && blockedRight && !blockedBackward)
					{
						//! Backpedal
						//if (Math.RandomIntInclusive(0, 1))
							//m_OverrideTargetMovementDirection = Math.RandomFloat(-135.0, -180.0);
						//else
							//m_OverrideTargetMovementDirection = Math.RandomFloat(135.0, 180.0);
						m_OverrideTargetMovementDirection = -180;
						m_OverrideMovementTimeout = 1.5 / m_MovementSpeed;
						m_OverrideWaypoint = backwardPos;
						m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_BLOCKED, checkLeft + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow_Red", lr);
						m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_BLOCKED, checkRight + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow_Red", -lr);
						m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_OK, "0 0 0", "ExpansionDebugArrow");
						m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_OK, "0 0 0", "ExpansionDebugArrow");
						m_Unit.Expansion_DebugObject_Deferred(CHECK_BACKWARD_BLOCKED, "0 0 0", "ExpansionDebugArrow_Red");
						m_Unit.Expansion_DebugObject_Deferred(CHECK_BACKWARD_OK, checkBwd + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow", -fb);
					}
					else if (blockedLeft || ((!blockedRight || blockedBackward) && Math.RandomIntInclusive(0, 1)))
					{
						//! Go right if only blocked left or 50% chance go right if neither blocked left/right
						m_OverrideTargetMovementDirection = 90.0;
						m_OverrideMovementTimeout = 1.5 / m_MovementSpeed;
						if (blockedLeft)
						{
							m_OverrideWaypoint = rightPos;
							m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_BLOCKED, checkLeft + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow_Red", lr);
							m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_OK, "0 0 0", "ExpansionDebugArrow");
						}
						else
						{
							m_OverrideWaypoint = checkRight;
							m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_BLOCKED, "0 0 0", "ExpansionDebugArrow_Red");
							m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_OK, checkLeft + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow", lr);
						}
						m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_BLOCKED, "0 0 0", "ExpansionDebugArrow_Red");
						m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_OK, checkRight + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow", -lr);
					}
					else
					{
						//! Go left
						m_OverrideTargetMovementDirection = -90.0;
						m_OverrideMovementTimeout = 1.5 / m_MovementSpeed;
						if (blockedRight)
						{
							m_OverrideWaypoint = leftPos;
							m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_BLOCKED, checkRight + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow_Red", -lr);
							m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_OK, "0 0 0", "ExpansionDebugArrow");
						}
						else
						{
							m_OverrideWaypoint = checkLeft;
							m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_BLOCKED, "0 0 0", "ExpansionDebugArrow_Red");
							m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_OK, checkRight + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow", -lr);
						}
						m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_BLOCKED, "0 0 0", "ExpansionDebugArrow_Red");
						m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_OK, checkLeft + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow", lr);
					}

					if (blockedBackward)
					{
						m_Unit.Expansion_DebugObject_Deferred(CHECK_BACKWARD_BLOCKED, checkBwd + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow_Red", -fb);
						m_Unit.Expansion_DebugObject_Deferred(CHECK_BACKWARD_OK, "0 0 0", "ExpansionDebugArrow");
					}
					else if (!blockedLeft || !blockedRight)
					{
						m_Unit.Expansion_DebugObject_Deferred(CHECK_BACKWARD_BLOCKED, "0 0 0", "ExpansionDebugArrow_Red");
						m_Unit.Expansion_DebugObject_Deferred(CHECK_BACKWARD_OK, "0 0 0", "ExpansionDebugArrow");
					}

					//m_PathFinding.OverridePosition(m_OverrideWaypoint);
					m_Unit.Expansion_DebugObject_Deferred(OVERRIDDEN_WAYPOINT, m_OverrideWaypoint, "ExpansionDebugBox_Orange");

					chg = true;
#ifdef DIAG
					msg = "blocked";
#endif
				}
				else if (chg)
				{
					if (m_Unit.m_Expansion_DebugObjects[CHECK_ORIGIN])
						m_Unit.Expansion_DebugObject_Deferred(CHECK_ORIGIN, "0 0 0", "ExpansionDebugBox_Blue");
					if (m_Unit.m_Expansion_DebugObjects[CHECK_FORWARD_BLOCKED])
						m_Unit.Expansion_DebugObject_Deferred(CHECK_FORWARD_BLOCKED, "0 0 0", "ExpansionDebugArrow_Red");
					if (m_Unit.m_Expansion_DebugObjects[CHECK_LEFT_BLOCKED])
						m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_BLOCKED, "0 0 0", "ExpansionDebugArrow_Red");
					if (m_Unit.m_Expansion_DebugObjects[CHECK_RIGHT_BLOCKED])
						m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_BLOCKED, "0 0 0", "ExpansionDebugArrow_Red");
					if (m_Unit.m_Expansion_DebugObjects[CHECK_LEFT_OK])
						m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_OK, "0 0 0", "ExpansionDebugArrow");
					if (m_Unit.m_Expansion_DebugObjects[CHECK_RIGHT_OK])
						m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_OK, "0 0 0", "ExpansionDebugArrow");
					if (m_Unit.m_Expansion_DebugObjects[OVERRIDDEN_WAYPOINT])
						m_Unit.Expansion_DebugObject_Deferred(OVERRIDDEN_WAYPOINT, "0 0 0", "ExpansionDebugBox_Orange");
					if (m_Unit.m_Expansion_DebugObjects[CHECK_BACKWARD_BLOCKED])
						m_Unit.Expansion_DebugObject_Deferred(CHECK_BACKWARD_BLOCKED, "0 0 0", "ExpansionDebugArrow_Red");
					if (m_Unit.m_Expansion_DebugObjects[CHECK_BACKWARD_OK])
						m_Unit.Expansion_DebugObject_Deferred(CHECK_BACKWARD_OK, "0 0 0", "ExpansionDebugArrow");

#ifdef DIAG
					msg = "not blocked";
#endif
				}
			}

#ifdef DIAG
			if (chg)
			{
				if (blockedForward)
					msg += " fwd";
				if (blockedLeft)
					msg += " left";
				if (blockedRight)
					msg += " right";
				if (blockedBackward)
					msg += " bwd";
				if (blockedForward || blockedLeft || blockedRight || blockedBackward)
					msg += " move " + m_OverrideTargetMovementDirection;
				EXTrace.Print(EXTrace.AI, m_Player, msg);
				ExpansionStatic.MessageNearPlayers(m_Unit.GetPosition(), 30, m_Unit.ToString() + " " + m_InstanceNum.ToString() + " " + msg);
			}
#endif
		}

		float minFinal = 0.3;

		if (m_MovementSpeed != 0)
		{
			vector pathDir2D = vector.Direction(position, Vector(wayPoint[0], position[1], wayPoint[2]));
			m_WayPointDistance2D = pathDir2D.LengthSq();
			if (m_WayPointDistance2D >= 0.04)  //! 0.2 m
			{
				m_PrevWaypoint = wayPoint;
				if (m_OverrideMovementTimeout > 0 || !m_ForceTurnTarget)
					m_TurnTarget = pathDir2D.Normalized().VectorToAngles()[0];
			}
			else if (!isFinal && m_OverrideMovementTimeout <= 0 && wayPoint == m_PrevWaypoint)
			{
				//! Don't update turn target to prevent AI spinning in place.
				//! Attempt to move waypoint further towards target.
				if (m_WaypointTime > 1.5 && m_PathFinding.IsBlocked(position, m_PathFinding.m_TargetReference.Position, wayPoint, outNormal))
				{
					m_WaypointTime = 0;
					m_OverrideTargetMovementDirection = 0;
					m_OverrideMovementTimeout = 1.5;
					m_PathFinding.OverridePosition(wayPoint);
					//! Setting speed to jog seems to help against AI not vaulting when it should.
					//! This will be reset in eAIBase::CommandHandler next update
					SetSpeedOverrider(true);
					SetTargetSpeed(2.0);
#ifdef DIAG
					ExpansionStatic.MessageNearPlayers(m_Unit.GetPosition(), 30, m_Unit.ToString() + " " + m_InstanceNum.ToString() + " wp " + wayPoint + " dist " + m_WayPointDistance2D);
#endif
				}
				else
				{
					m_WaypointTime += pDt;
				}
			}
		}

		//if (isFinal && m_WayPointDistance >= minFinal)
		//{
			////! If distance squared to waypoint is more than minFinal, make sure unit can actually reach waypoint.
			////! Prevents unit rotating in spot in case it cannot reach a waypoint due to height difference.

			float y = GetGame().SurfaceY(wayPoint[0], wayPoint[2]);
			if (y > wayPoint[1]) wayPoint[1] = y;
				
			vector orig_WayPoint = wayPoint;
			if (DayZPhysics.RayCastBullet(wayPoint + Vector(0.0, 1.5, 0.0), wayPoint - Vector(0.0, 10.0, 0.0), m_CollisionLayerMask|PhxInteractionLayers.TERRAIN, m_Player, m_HitObject, m_HitPosition, m_HitNormal, m_HitFraction))
				wayPoint = m_HitPosition;

			m_WayPointDistance = vector.DistanceSq(position, wayPoint);
		//}

		if (m_OverrideMovementTimeout > 0)
		{
			m_TargetMovementDirection = m_OverrideTargetMovementDirection;
			m_ForceMovementDirection = true;
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
			SetTargetSpeed(Math.Lerp(m_MovementSpeed, 1.0, pDt * 2.0));
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
				float maxSpeed;
				if (Math.AbsFloat(m_TargetMovementDirection) > 90)
					maxSpeed = 2.0;  //! Can't sprint backwards
				else
					maxSpeed = 3.0;
				SetTargetSpeed(Math.Lerp(m_MovementSpeed, maxSpeed, pDt * 2.0));
			}
		}
		
		DBGDrawSphere(wayPoint, 0.05, 0xFF00FF00);
		//DBGDrawLine(position, wayPoint, 0xFF00FF00);
		m_Unit.Expansion_DebugObject_Deferred(CORRECTED_WAYPOINT, wayPoint, "ExpansionDebugBox");

		float dirChangeSpeed = pDt;
		if (m_ForceMovementDirection)
			dirChangeSpeed *= 4;

		m_MovementDirection += ExpansionMath.AngleDiff2(m_MovementDirection, m_TargetMovementDirection) * dirChangeSpeed;
		m_MovementDirection = Math.Clamp(m_MovementDirection, -180.0, 180.0);

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

	private bool Raycast(vector start, vector end, out vector hitPosition, out vector hitNormal, out float hitFraction, vector endRV = vector.Zero, float radiusRV = 0.7)
	{
		if (endRV == vector.Zero)
			endRV = end;

		bool hit;

		//! 1st raycast specifically for trees and plain objects except scenery
		int contactComponent;
		set<Object> results();
		if (DayZPhysics.RaycastRV(start, endRV, hitPosition, hitNormal, contactComponent, results, null, m_Unit, false, false, ObjIntersectGeom, radiusRV))
		{
			foreach (Object obj: results)
			{
				if (obj.IsTree() || (obj.IsPlainObject() && !obj.IsScenery() && !obj.IsRock()))
					hit = true;
			}
		}

		//! Everything else
		if (!hit)
		{
			Object hitObject;
			PhxInteractionLayers hit_mask = PhxInteractionLayers.CHARACTER | PhxInteractionLayers.BUILDING | PhxInteractionLayers.DOOR | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.ITEM_LARGE | PhxInteractionLayers.FENCE | PhxInteractionLayers.AI;
			hit = DayZPhysics.RayCastBullet(start, end, hit_mask, m_Unit, hitObject, hitPosition, hitNormal, hitFraction);
			hitFraction = 1.0 - hitFraction;
		}

		if (hit)
			m_Unit.Expansion_DebugObject_Deferred(BLOCKED_HITPOSITION, hitPosition, "ExpansionDebugBox_Purple");
		else
			m_Unit.Expansion_DebugObject_Deferred(BLOCKED_HITPOSITION, "0 0 0", "ExpansionDebugBox_Purple");
			
#ifndef SERVER
		int debugColour = 0xFF00AAFF;
		if (hit) debugColour = 0xFFAA00FF;
		vector points2[2];
		points2[0] = start;
		points2[1] = end;
		if (hit) points2[1] = hitPosition;
		m_Unit.AddShape(Shape.CreateLines(debugColour, ShapeFlags.NOZBUFFER, points2, 2));
#endif

		return hit;
	}

	float GetWaypointDistance()
	{
		return m_WayPointDistance;
	}
};
