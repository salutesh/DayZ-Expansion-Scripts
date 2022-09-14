class eAICommandMove extends ExpansionHumanCommandScript
{
	static const int TURN_STATE_NONE = 0;
	static const int TURN_STATE_TURNING = 1;
	
	static const vector CHECK_MIN_HEIGHT = "0 1.25 0";

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
	static const int BLOCKED_FORWARD_HITPOSITION = 12;
	static const int BLOCKED_LEFT_HITPOSITION = 13;
	static const int BLOCKED_RIGHT_HITPOSITION = 14;
	static const int BLOCKED_BACKWARD_HITPOSITION = 15;
	
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
	private float m_AimLR;
	private float m_AimUD;
	
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
	private float m_BlockedLeftDist;
	private bool m_LastBlockedRight;
	private float m_BlockedRightDist;
	private bool m_LastBlockedBackward;

	private float m_OverrideMovementTimeout;
	private float m_OverrideTargetMovementDirection;
	private vector m_OverrideWaypoint;
	private float m_DebugTime;

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

/*
	override void SetAimAnglesRel(float aimLR, float aimUD)
	{
		m_AimLR = aimLR;
		m_AimUD = aimUD;
	}
*/

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

	float GetCurrentMovementAngle()
	{
		return m_MovementDirection;
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
			m_DebugTime = m_OverrideMovementTimeout;
		}
		else
		{
			m_PathFinding.StopOverride();
			if (m_DebugTime > 0)
				m_DebugTime -= pDt;
		}

		//! Try and avoid obstacles if we are moving and not climbing
		if (m_MovementSpeed && !m_PathFinding.GetOverride() && /*!m_Unit.eAI_HasLOS() &&*/ !m_Unit.eAI_IsClimb())
		{
			bool chg;
#ifdef DIAG
			string msg;
#endif

			//! Always check fwd
			vector fb = vector.Direction(position, Vector(wayPoint[0], position[1], wayPoint[2])).Normalized();
			vector checkFwd = position + 0.25 * fb;
			vector checkBwd = position - 0.25 * fb;
			blockedForward = Raycast(position + CHECK_MIN_HEIGHT, checkFwd + CHECK_MIN_HEIGHT, forwardPos, outNormal, hitFraction, position + fb * m_MovementSpeed + CHECK_MIN_HEIGHT, 0.5, true);
			if (blockedForward)
			{
				m_Unit.Expansion_DebugObject_Deferred(BLOCKED_FORWARD_HITPOSITION, forwardPos, "ExpansionDebugBox_Purple");
				m_Unit.Expansion_DebugObject_Deferred(CHECK_ORIGIN, position + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugBox_Blue", fb);
				m_Unit.Expansion_DebugObject_Deferred(CHECK_FORWARD_BLOCKED, position + 0.5 * fb + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow_Red", fb);
			}

			if (blockedForward != m_LastBlockedForward)
			{
				m_LastBlockedForward = blockedForward;
				chg = true;
			}

			//! If during movement overriding we are no longer blocked in the fwd direction, stop overriding imediately
			if (!blockedForward && m_OverrideMovementTimeout > 0)
			{
				m_OverrideMovementTimeout = 0;
				m_OverrideTargetMovementDirection = 0;
				m_ForceMovementDirection = true;
			}
			//! Check left/right/bwd if blocked forward or already overriding movement
			else if (blockedForward || m_OverrideMovementTimeout > 0)
			{
				//! Ready to play pinball

				vector lr = fb.Perpend();
				vector checkLeft = position + 0.25 * lr;
				vector checkRight = position - 0.25 * lr;
				blockedLeft = Raycast(position + CHECK_MIN_HEIGHT, checkLeft + CHECK_MIN_HEIGHT, leftPos, outNormal, hitFraction, position + lr + CHECK_MIN_HEIGHT, 0.5);
				if (blockedLeft)
					m_Unit.Expansion_DebugObject_Deferred(BLOCKED_LEFT_HITPOSITION, leftPos, "ExpansionDebugBox_Purple", outNormal);
				blockedRight = Raycast(position + CHECK_MIN_HEIGHT, checkRight + CHECK_MIN_HEIGHT, rightPos, outNormal, hitFraction, position - lr + CHECK_MIN_HEIGHT, 0.5);
				if (blockedRight)
					m_Unit.Expansion_DebugObject_Deferred(BLOCKED_RIGHT_HITPOSITION, rightPos, "ExpansionDebugBox_Purple", outNormal);

				//! Only check bwd if we were backpedaling
				if (m_OverrideTargetMovementDirection == -180 && m_OverrideMovementTimeout <= 0)
				{
					blockedBackward = Raycast(position + CHECK_MIN_HEIGHT, checkBwd + CHECK_MIN_HEIGHT, backwardPos, outNormal, hitFraction);
					if (blockedBackward)
						m_Unit.Expansion_DebugObject_Deferred(BLOCKED_BACKWARD_HITPOSITION, backwardPos, "ExpansionDebugBox_Purple", outNormal);

					backwardPos = position - 0.5 * fb;
				}

				if (blockedLeft && blockedRight)
				{
					float leftDist;
					float rightDist;
					if (blockedLeft)
						m_BlockedLeftDist = vector.DistanceSq(position, leftPos);
					if (blockedRight)
						m_BlockedRightDist = vector.DistanceSq(position, rightPos);
					//if (Math.Max(leftDist, rightDist) - Math.Min(leftDist, rightDist) > 0.25)
					//{
						//blockedLeft = leftDist < rightDist;
						//blockedRight = leftDist > rightDist;
					//}
				}

				//if (blockedLeft && !m_PathFinding.IsBlocked(position, position - 0.5 * lr, rightPos, outNormal))
					rightPos = position - 0.5 * lr;
				//if (blockedRight && !m_PathFinding.IsBlocked(position, position + 0.5 * lr, leftPos, outNormal))
					leftPos = position + 0.5 * lr;

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

				//! If change in blocked directions or currently not overriding movement, adjust movement direction
				if (blockedForward || chg)
				{
					if (m_MovementSpeed > 2)
					{
						//! Limiting speed to jog helps obstacle avoidance while strafing
						//! This will be reset in eAIBase::CommandHandler next update
						SetSpeedOverrider(true);
						SetTargetSpeed(2.0);
						m_MovementSpeed = 2;
					}

					//! Check which direction we want to move. Moving left is always the fall-through case
					bool moveRight;
					bool backPedal;
					if (blockedLeft && blockedRight)
					{
						if (blockedBackward)
						{
							if (m_OverrideMovementTimeout > 0)
							{
								//! If already moving right, keep moving right
								if (m_OverrideTargetMovementDirection == 90)
									moveRight = true;
								//! If already backpedaling, keep backpedaling
								else if (m_OverrideTargetMovementDirection == -180)
									backPedal = true;
							}
							else
							{
								//! 33% chance move right or backpedal if all directions blocked (because what choice do we have)
								int roll = Math.RandomIntInclusive(0, 2);
								if (roll == 1)
									moveRight = true;
								else if (roll == 2)
									backPedal = true;
							}
						}
						else
						{
							backPedal = true;
						}
					}
					else if (blockedLeft && !blockedRight)
					{
						//! Go right if blocked left
						moveRight = true;
					}
					else if (!blockedLeft && !blockedRight)
					{
						if (m_OverrideMovementTimeout > 0)
						{
							//! If already moving right, keep moving right
							if (m_OverrideTargetMovementDirection == 90)
								moveRight = true;
						}
						//! 50% chance move right if neither blocked left/right
						else if (Math.RandomIntInclusive(0, 1))
						{
							moveRight = true;
						}
					}

					if (backPedal)
					{
						//! Backpedal
						//if (Math.RandomIntInclusive(0, 1))
							//m_OverrideTargetMovementDirection = Math.RandomFloat(-135.0, -180.0);
						//else
							//m_OverrideTargetMovementDirection = Math.RandomFloat(135.0, 180.0);
						m_OverrideTargetMovementDirection = -180;
						if (m_OverrideMovementTimeout <= 0)
							m_OverrideMovementTimeout = 1 / m_MovementSpeed;
						m_OverrideWaypoint = backwardPos;
						m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_BLOCKED, position + 0.5 * lr + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow_Orange", lr);
						m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_BLOCKED, position - 0.5 * lr + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow_Orange", -lr);
						m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_OK, "0 0 0", "ExpansionDebugArrow");
						m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_OK, "0 0 0", "ExpansionDebugArrow");
						m_Unit.Expansion_DebugObject_Deferred(CHECK_BACKWARD_BLOCKED, "0 0 0", "ExpansionDebugArrow_Orange");
						m_Unit.Expansion_DebugObject_Deferred(CHECK_BACKWARD_OK, position - 0.5 * fb + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow", -fb);
					}
					else if (moveRight)
					{
						m_OverrideTargetMovementDirection = 90.0;
						if (m_OverrideMovementTimeout <= 0)
							m_OverrideMovementTimeout = 1 / m_MovementSpeed;
						m_OverrideWaypoint = rightPos;
						if (blockedLeft)
						{
							m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_BLOCKED, position + 0.5 * lr + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow_Orange", lr);
							m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_OK, "0 0 0", "ExpansionDebugArrow");
						}
						else
						{
							m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_BLOCKED, "0 0 0", "ExpansionDebugArrow_Orange");
							m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_OK, position + 0.5 * lr + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow", lr);
						}
						m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_BLOCKED, "0 0 0", "ExpansionDebugArrow_Orange");
						m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_OK, position - 0.5 * lr + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow", -lr);
					}
					else
					{
						//! Go left
						m_OverrideTargetMovementDirection = -90.0;
						if (m_OverrideMovementTimeout <= 0)
							m_OverrideMovementTimeout = 1 / m_MovementSpeed;
						m_OverrideWaypoint = leftPos;
						if (blockedRight)
						{
							m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_BLOCKED, position - 0.5 * lr + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow_Orange", -lr);
							m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_OK, "0 0 0", "ExpansionDebugArrow");
						}
						else
						{
							m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_BLOCKED, "0 0 0", "ExpansionDebugArrow_Orange");
							m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_OK, position - 0.5 * lr + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow", -lr);
						}
						m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_BLOCKED, "0 0 0", "ExpansionDebugArrow_Orange");
						m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_OK, position + 0.5 * lr + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow", lr);
					}

					//m_PathFinding.OverridePosition(m_OverrideWaypoint);
					m_Unit.Expansion_DebugObject_Deferred(OVERRIDDEN_WAYPOINT, m_OverrideWaypoint, "ExpansionDebugBox_Orange");
				}

				if (blockedBackward)
				{
					m_Unit.Expansion_DebugObject_Deferred(CHECK_BACKWARD_BLOCKED, position - 0.5 * fb + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow_Orange", -fb);
					m_Unit.Expansion_DebugObject_Deferred(CHECK_BACKWARD_OK, "0 0 0", "ExpansionDebugArrow");
				}
				else if (!blockedLeft || !blockedRight)
				{
					m_Unit.Expansion_DebugObject_Deferred(CHECK_BACKWARD_BLOCKED, "0 0 0", "ExpansionDebugArrow_Orange");
					m_Unit.Expansion_DebugObject_Deferred(CHECK_BACKWARD_OK, "0 0 0", "ExpansionDebugArrow");
				}
			}

#ifdef DIAG
			if (chg)
			{
				if (blockedForward || blockedLeft || blockedRight || blockedBackward)
				{
					msg = "blocked";
					if (blockedForward)
						msg += " fwd";
					if (blockedLeft)
						msg += " left";
					if (blockedRight)
						msg += " right";
					if (blockedBackward)
						msg += " bwd";
					msg += " move " + m_OverrideTargetMovementDirection;
				}
				else
				{
					msg = "not blocked";
				}
				EXTrace.Print(EXTrace.AI, m_Player, msg);
				//ExpansionStatic.MessageNearPlayers(m_Unit.GetPosition(), 30, m_Unit.ToString() + " " + m_InstanceNum.ToString() + " " + msg);
			}
#endif
		}

#ifdef DIAG
		if (m_OverrideMovementTimeout <= 0 && !blockedForward && !blockedLeft && !blockedRight && !blockedBackward && m_DebugTime <= 0)
		{
			//! No longer blocked and no longer overriding movement
			if (m_Unit.m_Expansion_DebugObjects[CHECK_ORIGIN])
				m_Unit.Expansion_DebugObject_Deferred(CHECK_ORIGIN, "0 0 0", "ExpansionDebugBox_Blue");
			if (m_Unit.m_Expansion_DebugObjects[CHECK_FORWARD_BLOCKED])
				m_Unit.Expansion_DebugObject_Deferred(CHECK_FORWARD_BLOCKED, "0 0 0", "ExpansionDebugArrow_Red");
			if (m_Unit.m_Expansion_DebugObjects[CHECK_LEFT_BLOCKED])
				m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_BLOCKED, "0 0 0", "ExpansionDebugArrow_Orange");
			if (m_Unit.m_Expansion_DebugObjects[CHECK_RIGHT_BLOCKED])
				m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_BLOCKED, "0 0 0", "ExpansionDebugArrow_Orange");
			if (m_Unit.m_Expansion_DebugObjects[CHECK_LEFT_OK])
				m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_OK, "0 0 0", "ExpansionDebugArrow");
			if (m_Unit.m_Expansion_DebugObjects[CHECK_RIGHT_OK])
				m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_OK, "0 0 0", "ExpansionDebugArrow");
			if (m_Unit.m_Expansion_DebugObjects[OVERRIDDEN_WAYPOINT])
				m_Unit.Expansion_DebugObject_Deferred(OVERRIDDEN_WAYPOINT, "0 0 0", "ExpansionDebugBox_Orange");
			if (m_Unit.m_Expansion_DebugObjects[CHECK_BACKWARD_BLOCKED])
				m_Unit.Expansion_DebugObject_Deferred(CHECK_BACKWARD_BLOCKED, "0 0 0", "ExpansionDebugArrow_Orange");
			if (m_Unit.m_Expansion_DebugObjects[CHECK_BACKWARD_OK])
				m_Unit.Expansion_DebugObject_Deferred(CHECK_BACKWARD_OK, "0 0 0", "ExpansionDebugArrow");
			if (m_Unit.m_Expansion_DebugObjects[BLOCKED_FORWARD_HITPOSITION])
				m_Unit.Expansion_DebugObject_Deferred(BLOCKED_FORWARD_HITPOSITION, "0 0 0", "ExpansionDebugBox_Purple");
			if (m_Unit.m_Expansion_DebugObjects[BLOCKED_LEFT_HITPOSITION])
				m_Unit.Expansion_DebugObject_Deferred(BLOCKED_LEFT_HITPOSITION, "0 0 0", "ExpansionDebugBox_Purple");
			if (m_Unit.m_Expansion_DebugObjects[BLOCKED_RIGHT_HITPOSITION])
				m_Unit.Expansion_DebugObject_Deferred(BLOCKED_RIGHT_HITPOSITION, "0 0 0", "ExpansionDebugBox_Purple");
			if (m_Unit.m_Expansion_DebugObjects[BLOCKED_BACKWARD_HITPOSITION])
				m_Unit.Expansion_DebugObject_Deferred(BLOCKED_BACKWARD_HITPOSITION, "0 0 0", "ExpansionDebugBox_Purple");
		}
#endif

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
		
		//m_Table.SetAimX(this, m_AimLR);
		//m_Table.SetAimY(this, m_AimUD);

		//m_TurnVelocity = ExpansionMath.AngleDiff2(m_Turn, m_TurnPrevious);
		//m_TurnPrevious = m_Turn;
		
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
				
					if (m_TurnTime > 2.0 || Math.AbsFloat(m_TurnDifference) < 1)
					{
						m_Table.CallStopTurn(this);

						m_TurnState = TURN_STATE_NONE;
					}

					break;
			}

		}
		else
		{
			if (m_TurnState == TURN_STATE_TURNING)
				m_Table.CallStopTurn(this);

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

	private bool Raycast(vector start, vector end, out vector hitPosition, out vector hitNormal, out float hitFraction, vector endRV = vector.Zero, float radiusRV = 0.25, bool includeAI = false)
	{
		if (endRV == vector.Zero)
			endRV = end;

		bool hit;

		//! 1st raycast specifically for trees
		int contactComponent;
		set<Object> results();
		if (DayZPhysics.RaycastRV(start, endRV, hitPosition, hitNormal, contactComponent, results, null, m_Unit, false, false, ObjIntersectGeom, radiusRV))
		{
			foreach (Object obj: results)
			{
				if (obj.IsTree())
					hit = true;
			}
		}

		//! Everything else
		if (!hit)
		{
			Object hitObject;
			PhxInteractionLayers hit_mask = PhxInteractionLayers.BUILDING | PhxInteractionLayers.DOOR | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.ITEM_LARGE | PhxInteractionLayers.FENCE;
			if (includeAI)
				hit_mask |= PhxInteractionLayers.CHARACTER | PhxInteractionLayers.AI;
			hit = DayZPhysics.SphereCastBullet(start, end, 0.25, hit_mask, m_Unit, hitObject, hitPosition, hitNormal, hitFraction);
			hitFraction = 1.0 - hitFraction;
		}
			
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

	bool IsStuck()
	{
		if (!m_MovementSpeed)
			return false;

		if (!m_LastBlockedForward /*&& !m_LastBlockedLeft && !m_LastBlockedRight*/)
			return false;

		//if (m_BlockedLeftDist > 0.16 && m_BlockedRightDist > 0.16)
			//return false;

		return !m_Unit.eAI_IsClimb();
	}
};
