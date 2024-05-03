class eAICommandMove: ExpansionHumanCommandScript
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
	static const int CHECK_LEFT_GO = 7;
	static const int CHECK_RIGHT_OK = 8;
	static const int CHECK_RIGHT_GO = 9;
	static const int OVERRIDDEN_WAYPOINT = 10;
	static const int BLOCKED_HITPOSITION = 11;
	static const int CHECK_BACKWARD_BLOCKED = 12;
	static const int CHECK_BACKWARD_OK = 13;
	static const int BLOCKED_FORWARD_HITPOSITION = 14;
	static const int BLOCKED_LEFT_HITPOSITION = 15;
	static const int BLOCKED_RIGHT_HITPOSITION = 16;
	static const int BLOCKED_BACKWARD_HITPOSITION = 17;

	static int s_InstanceCount;
	private int m_InstanceNum;

	private int m_PreviousInteractionLayer;

	private eAIBase m_Unit;
#ifndef EAI_USE_LEGACY_PATHFINDING
	private ExpansionPathHandler m_PathFinding;
#else
	private eAIPathFinding m_PathFinding;
#endif
	private vector m_Waypoint;
	private float m_WaypointTime;
	private float m_WayPointDistance;
	private float m_WayPointDistance2D;
	private float m_MinFinal = 0.3;
	private bool m_UpdatePath;

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
	private vector m_PrevPosition;
	private float m_PositionTime;

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

	private int m_Stance = -1;
	private int m_StancePrev = -1;
	private float m_StanceChangeTimeout;

	Object m_BlockingObject;
	private bool m_LastBlockedForward;
	private bool m_LastBlockedLeft;
	private float m_BlockedLeftDist;
	private bool m_LastBlockedRight;
	private float m_BlockedRightDist;
	private bool m_LastBlockedBackward;
	private bool m_LastBlocked;
	private float m_BlockedTime;
	private float m_TurnDirection = 45.0;
	private int m_TurnOverride;
	private float m_TurnDirectionCWTime;
	private float m_TurnDirectionCCWTime;

	private float m_OverrideMovementTimeout;
	private float m_OverrideTargetMovementDirection;
	private vector m_OverrideWaypoint;
	private float m_DebugTime;

	bool m_IsTagWeaponFire;
	bool m_WeaponFire;

	float m_LeanDirection = 1.0;

	void eAICommandMove(DayZPlayerImplement player, ExpansionHumanST st, int stance = -1)
	{
		Class.CastTo(m_Unit, player);

		m_PathFinding = m_Unit.GetPathFinding();

		m_InstanceNum = s_InstanceCount++;

		m_StancePrev = stance;
		m_Stance = stance;
	}

	void ~eAICommandMove()
	{
	}

	override void OnActivate()
	{
		//! XXX: This doesn't seem to be needed
		//dBodySetInteractionLayer(m_Player, PhxInteractionLayers.CHARACTER | PhxInteractionLayers.BUILDING | PhxInteractionLayers.DOOR | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.ITEM_LARGE | PhxInteractionLayers.FENCE | PhxInteractionLayers.AI);
	}

	override void OnDeactivate()
	{
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

	float GetTurnTarget()
	{
		return m_TurnTarget;
	}

	float GetTurnDifference()
	{
		return m_TurnDifference;
	}

	bool IsTurning()
	{
		if (m_TurnState == TURN_STATE_TURNING)
			return true;

		return false;
	}

	bool ShouldRecalculateTurnTarget()
	{
		if (m_WayPointDistance2D >= m_MinFinal && !m_TurnOverride && (m_OverrideMovementTimeout > 0 || !m_ForceTurnTarget))
			return true;

		return false;
	}

	void StartTurnOverride(int speedLimit = 1)
	{
		if (m_OverrideMovementTimeout <= 0 || m_TurnOverride < speedLimit)
		{
			m_OverrideMovementTimeout = 3.0;
			m_DebugTime = 3.0;

			if (m_LastBlockedLeft && !m_LastBlockedRight)
			{
				//! Blocked left, turn right
				m_TurnDirection = 45.0;
			}
			else if (!m_LastBlockedLeft && m_LastBlockedRight)
			{
				//! Blocked right, turn left
				m_TurnDirection = -45.0;
			}
			else if (Math.RandomIntInclusive(0, 1))
			{
				//! Both directions blocked, random turn direction
				m_TurnDirection = -m_TurnDirection;
			}

			if (m_TurnOverride < speedLimit)
				m_TurnOverride = speedLimit;
		}
	}

	void SetWeaponFire(bool state)
	{
		m_WeaponFire = state;
	}

	void SetSpeedLimit(float pSpeedIdx)
	{
		m_SpeedLimit = pSpeedIdx;

		if (m_SpeedLimit < 0 || m_SpeedLimit > 3) m_SpeedLimit = 3;
	}

	float GetSpeedLimit()
	{
		return m_SpeedLimit;
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

	bool OverrideStance(int stance)
	{
		if (m_Stance != stance)
		{
			m_Stance = stance;
			return true;
		}

		return false;
	}

	int GetStance()
	{
		if (m_Stance == -1)
			return 0;

		return m_Stance;
	}

	bool IsChangingStance()
	{
		if (m_StancePrev != m_Stance)
			return true;

		return m_StanceChangeTimeout > 0;
	}

	override void PreAnimUpdate(float pDt)
	{
#ifdef DIAG
		auto trace = EXTrace.Profile(EXTrace.AI, this);
#endif

		super.PreAnimUpdate(pDt);

		if (m_StanceChangeTimeout > 0)
			m_StanceChangeTimeout -= pDt;

#ifdef DIAG
		auto hitch = new EXHitch(m_Unit.ToString() + " eAICommandMove::PreAnimUpdate ", 20000);
#endif

		m_SpeedUpdateTime += pDt;

		m_UpdatePath = false;

		vector debug_points[2];

		vector orientation = Math3D.MatrixToAngles(m_Transform);
		vector position = m_Transform[3];

		vector wayPoint = position;
		bool isPathPointFinal;  //! Is waypoint the final point of the current path? (not necessarily final target position!)
		bool isTargetPositionFinal = true;  //! Is waypoint the final target position?
		vector pathDir2D;
		vector fb = m_Transform[2];
		m_Direction = fb;
		if (m_SpeedLimit)
		{
#ifndef EAI_USE_LEGACY_PATHFINDING
			//! https://feedback.bistudio.com/T173348
			if (m_PathFinding.GetNext(wayPoint) <= 2)
				isPathPointFinal = true;
			if (isPathPointFinal && (m_Unit.m_eAI_TargetPositionIsFinal || m_PathFinding.m_Count == 0))
				isTargetPositionFinal = true;
			else
				isTargetPositionFinal = false;
			//m_WayPointDistance = vector.DistanceSq(position, wayPoint);
			DBGDrawSphere(wayPoint, 0.05, 0xFFFF0000);
			//DBGDrawLine(position, wayPoint, 0xFFFF0000);
			m_Unit.Expansion_DebugObject_Deferred(ORIGINAL_WAYPOINT, wayPoint, "ExpansionDebugBox_Red");
#else
			int wayPointIndex;

			if (m_PathFinding.Count() >= 2)
			{
				wayPointIndex = m_PathFinding.Next(position);
				wayPoint = m_PathFinding[wayPointIndex];

				isTargetPositionFinal = wayPointIndex == m_PathFinding.Count() - 1;
			}
#endif

			pathDir2D = vector.Direction(position, Vector(wayPoint[0], position[1], wayPoint[2]));

			if (isTargetPositionFinal && m_PathFinding.m_IsUnreachable)
			{
				bool halt;

				//! Prevent fall from a large height (e.g. building top)
				vector checkDirection = fb * 2;
				if (!m_Unit.eAI_IsFallSafe(checkDirection))
				{
					halt = true;
				}
				else
				{
					//! Don't go into deep water
					vector surfacePosition = ExpansionStatic.GetSurfacePosition(position + fb);
					if (GetGame().GetWaterDepth(surfacePosition) > 0.5)
						halt = true;
				}

				if (halt)
				{
					wayPoint = position;
					m_MovementSpeed = 0;
					m_OverrideMovementTimeout = 0.0;
				}
			}
		}

		m_Waypoint = wayPoint;

		bool blockedForward;
		bool blockedLeft;
		bool blockedRight;
		bool blockedBackward;

		bool moveRight;
		bool backPedal;
		bool turnOverride;
	
#ifdef DIAG
		string msg;
#endif

		if (m_OverrideMovementTimeout > 0)
		{
			m_OverrideMovementTimeout -= pDt;
			if (m_TurnOverride)
			{
				if (m_TurnDirection < 0)
					m_TurnDirectionCCWTime += pDt;
				else
					m_TurnDirectionCWTime += pDt;

				float turnDirectionDiff = m_TurnDirectionCWTime - m_TurnDirectionCCWTime;
				if (Math.AbsFloat(turnDirectionDiff) > 5)
				{
					//! Failsafe so we don't make a turn in same direction indefinitely
				#ifdef DIAG
					msg = " WARNING: CW to CCW turn time diff " + turnDirectionDiff + " exceeds 5 seconds! Changing direction";
					EXTrace.Print(EXTrace.AI, this, "eAICommandMove::StartTurnOverride -" + msg);
					ExpansionStatic.MessageNearPlayers(m_Unit.GetPosition(), 100, m_Unit.GetType() + msg);
				#endif
					m_TurnDirection = -m_TurnDirection;
				}
			}
			else
			{
				m_TurnDirectionCCWTime = 0.0;
				m_TurnDirectionCWTime = 0.0;
			}
		}
		else
		{
			//m_PathFinding.StopOverride();
			if (m_DebugTime > 0)
				m_DebugTime -= pDt;
			m_LastBlockedForward = false;
			m_LastBlockedBackward = false;
			m_LastBlockedLeft = false;
			m_LastBlockedRight = false;
			m_TurnOverride = 0;
		}

		//! Try and avoid obstacles if we are moving and not climbing
		if (m_MovementSpeed && !m_Unit.IsClimbing() && !m_Unit.IsFalling() && !m_Unit.IsFighting())
		{
			vector leftPos;
			vector rightPos;
			vector forwardPos;
			vector backwardPos;
			vector outNormal;
			float hitFraction;

			bool chg;

			if (m_Stance != DayZPlayerConstants.STANCEIDX_PRONE && vector.DistanceSq(m_PrevPosition, position) < 0.0001 && !m_Unit.GetActionManager().GetRunningAction())
				m_PositionTime += pDt;  //! We don't seem to be actually moving
			else
				m_PositionTime = 0;

			vector checkDir;

			m_BlockingObject = null;

			//! Only check bwd if we are moving bwd, else check fwd
			if (Math.AbsFloat(m_MovementDirection) >= 135)
			{
				if (!m_Unit.IsRaised())
				{
					checkDir = position - 0.5 * fb;
					blockedBackward = this.Raycast(position + CHECK_MIN_HEIGHT, checkDir + CHECK_MIN_HEIGHT, backwardPos, outNormal, hitFraction, checkDir + CHECK_MIN_HEIGHT, 0.5, true, m_BlockingObject);
				}

				if (!blockedBackward && m_PositionTime > 3.0)
					blockedBackward = true;

				if (blockedBackward)
					m_Unit.Expansion_DebugObject_Deferred(BLOCKED_BACKWARD_HITPOSITION, backwardPos, "ExpansionDebugBox_Purple", outNormal);

				if (blockedBackward != m_LastBlockedBackward)
				{
					m_LastBlockedBackward = blockedBackward;
					chg = true;
				}
			}
			else
			{
				fb = pathDir2D.Normalized();
				checkDir = position + 0.5 * fb;
				blockedForward = this.Raycast(position + CHECK_MIN_HEIGHT, checkDir + CHECK_MIN_HEIGHT, forwardPos, outNormal, hitFraction, position + fb * m_MovementSpeed + CHECK_MIN_HEIGHT, 1.0, true, m_BlockingObject, true);

				if (!blockedForward && m_PositionTime > 3.0)
					blockedForward = true;

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
			}

			bool blockedFwdOrBwd;
			//! https://feedback.bistudio.com/T173348
			if (blockedForward || blockedBackward)
			{
				blockedFwdOrBwd = true;
				if (m_LastBlocked)
					m_BlockedTime += pDt;
			}
			else
			{
				m_BlockedTime = 0.0;
			}

			m_LastBlocked = blockedFwdOrBwd;

			float overrideTargetMovementDirection = m_OverrideTargetMovementDirection;

			//! If during movement overriding we are no longer blocked in any direction, stop overriding imediately
			if (!blockedFwdOrBwd && m_OverrideMovementTimeout > 0)
			{
				m_OverrideMovementTimeout = 0;
				m_OverrideTargetMovementDirection = 0;
				m_ForceMovementDirection = true;
			}
			//! Check left/right/bwd if blocked forward or already overriding movement
			else if (blockedFwdOrBwd || m_OverrideMovementTimeout > 0)
			{
				vector lr = fb.Perpend();
				vector checkLeft = position + 0.25 * lr;
				vector checkRight = position - 0.25 * lr;
				blockedLeft = this.Raycast(position + CHECK_MIN_HEIGHT, checkLeft + CHECK_MIN_HEIGHT, leftPos, outNormal, hitFraction, checkDir + lr + CHECK_MIN_HEIGHT, 0.5);
				if (!blockedLeft && m_PositionTime > 3.0)
					blockedLeft = true;
				if (blockedLeft)
					m_Unit.Expansion_DebugObject_Deferred(BLOCKED_LEFT_HITPOSITION, leftPos, "ExpansionDebugBox_Purple", outNormal);
				blockedRight = this.Raycast(position + CHECK_MIN_HEIGHT, checkRight + CHECK_MIN_HEIGHT, rightPos, outNormal, hitFraction, checkDir - lr + CHECK_MIN_HEIGHT, 0.5);
				if (!blockedRight && m_PositionTime > 3.0)
					blockedRight = true;
				if (blockedRight)
					m_Unit.Expansion_DebugObject_Deferred(BLOCKED_RIGHT_HITPOSITION, rightPos, "ExpansionDebugBox_Purple", outNormal);

				if (blockedLeft && blockedRight)
				{
					float leftDist;
					float rightDist;
					if (blockedLeft)
						m_BlockedLeftDist = vector.DistanceSq(position, leftPos);
					if (blockedRight)
						m_BlockedRightDist = vector.DistanceSq(position, rightPos);
				}

				rightPos = position - 0.5 * lr;
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

				//! Make sure the normal waypoint distance speed adjustments run
				m_SpeedOverrider = false;

				//! If change in blocked directions or currently not overriding movement, adjust movement direction
				if (blockedFwdOrBwd || (chg && (blockedLeft || blockedRight)))
				{
					//! Ready to play pinball

					//! Check which direction we want to move. Moving left is always the fall-through case
					if ((blockedLeft && blockedRight) || m_TurnOverride)
					{
						turnOverride = true;
					}
					else if (blockedLeft && !blockedRight)
					{
						//! Go right if blocked left
						moveRight = true;
					#ifdef EAI_DEBUG_MOVE
						EXTrace.Print(EXTrace.AI, this, m_Unit.ToString() + " blocked left, move right");
					#endif
					}
					else if (!blockedLeft && !blockedRight)
					{
						if (m_OverrideMovementTimeout > 0)
						{
							//! If already moving right, keep moving right
							if (m_OverrideTargetMovementDirection == 90)
							{
								moveRight = true;
							#ifdef EAI_DEBUG_MOVE
								EXTrace.Print(EXTrace.AI, this, m_Unit.ToString() + " not blocked L+R, already moving right, keep moving right");
							#endif
							}
						#ifdef EAI_DEBUG_MOVE
							else
							{
								EXTrace.Print(EXTrace.AI, this, m_Unit.ToString() + " not blocked L+R, already moving left, keep moving left");
							}
						#endif
						}
						//! 50% chance move right if neither blocked left/right
						else if (Math.RandomIntInclusive(0, 1))
						{
							moveRight = true;
						#ifdef EAI_DEBUG_MOVE
							EXTrace.Print(EXTrace.AI, this, m_Unit.ToString() + " not blocked L+R, move right");
						#endif
						}
					#ifdef EAI_DEBUG_MOVE
						else
						{
							EXTrace.Print(EXTrace.AI, this, m_Unit.ToString() + " not blocked L+R, move left");
						}
					#endif
					}
				#ifdef EAI_DEBUG_MOVE
					else
					{
						EXTrace.Print(EXTrace.AI, this, m_Unit.ToString() + " blocked right, move left");
					}
				#endif

					if (backPedal)
					{
						//! Backpedal
						//if (Math.RandomIntInclusive(0, 1))
							//m_OverrideTargetMovementDirection = Math.RandomFloat(-135.0, -180.0);
						//else
							//m_OverrideTargetMovementDirection = Math.RandomFloat(135.0, 180.0);
						if (m_BlockedRightDist - m_BlockedLeftDist > 0.16)  //! 0.4 m
							m_OverrideTargetMovementDirection = 175;
						else if (m_BlockedLeftDist - m_BlockedRightDist > 0.16)  //! 0.4 m
							m_OverrideTargetMovementDirection = -175;
						else
							m_OverrideTargetMovementDirection = -180;
						if (m_OverrideMovementTimeout <= 0)
						{
							m_OverrideMovementTimeout = 1.0;
							m_DebugTime = 3.0;
						}
						m_OverrideWaypoint = backwardPos;
					#ifdef EAI_DEBUG_MOVE
						EXTrace.Print(EXTrace.AI, this, m_Unit.ToString() + " blocked L+R, move bwd");
					#endif
					}
					else if (turnOverride)
					{
						m_OverrideTargetMovementDirection = 0.0;
						StartTurnOverride();
						m_OverrideWaypoint = forwardPos;
					#ifdef EAI_DEBUG_MOVE
						EXTrace.Print(EXTrace.AI, this, m_Unit.ToString() + " blocked L+R+B, turn " + m_TurnDirection);
					#endif
					}
					else if (moveRight)
					{
						m_OverrideTargetMovementDirection = 90.0;
						if (m_OverrideMovementTimeout <= 0)
						{
							m_OverrideMovementTimeout = 1.5 / m_MovementSpeed;
							m_DebugTime = 3.0;
						}
						m_OverrideWaypoint = rightPos;
						if (blockedLeft)
						{
							m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_BLOCKED, position + 0.5 * lr + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow_Orange", lr);
							m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_OK, "0 0 0", "ExpansionDebugArrow_Blue");
						}
						else
						{
							m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_BLOCKED, "0 0 0", "ExpansionDebugArrow_Orange");
							m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_OK, position + 0.5 * lr + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow_Blue", lr);
						}
						m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_GO, "0 0 0", "ExpansionDebugArrow");
						m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_BLOCKED, "0 0 0", "ExpansionDebugArrow_Orange");
						m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_OK, "0 0 0", "ExpansionDebugArrow_Blue");
						m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_GO, position - 0.5 * lr + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow", -lr);
					}
					else
					{
						//! Go left
						m_OverrideTargetMovementDirection = -90.0;
						if (m_OverrideMovementTimeout <= 0)
						{
							m_OverrideMovementTimeout = 1.5 / m_MovementSpeed;
							m_DebugTime = 3.0;
						}
						m_OverrideWaypoint = leftPos;
						if (blockedRight)
						{
							m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_BLOCKED, position - 0.5 * lr + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow_Orange", -lr);
							m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_OK, "0 0 0", "ExpansionDebugArrow_Blue");
						}
						else
						{
							m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_BLOCKED, "0 0 0", "ExpansionDebugArrow_Orange");
							m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_OK, position - 0.5 * lr + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow_Blue", -lr);
						}
						m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_GO, "0 0 0", "ExpansionDebugArrow");
						m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_BLOCKED, "0 0 0", "ExpansionDebugArrow_Orange");
						m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_OK, "0 0 0", "ExpansionDebugArrow_Blue");
						m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_GO, position + 0.5 * lr + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow", lr);
					}

					//m_PathFinding.OverridePosition(m_OverrideWaypoint);
					m_Unit.Expansion_DebugObject_Deferred(OVERRIDDEN_WAYPOINT, m_OverrideWaypoint, "ExpansionDebugBox_Orange");
				}

				if (backPedal || turnOverride)
				{
					m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_BLOCKED, position + 0.5 * lr + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow_Orange", lr);
					m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_BLOCKED, position - 0.5 * lr + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow_Orange", -lr);
					m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_OK, "0 0 0", "ExpansionDebugArrow_Blue");
					m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_GO, "0 0 0", "ExpansionDebugArrow");
					m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_OK, "0 0 0", "ExpansionDebugArrow_Blue");
					m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_GO, "0 0 0", "ExpansionDebugArrow");
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
				else
				{
					m_Unit.Expansion_DebugObject_Deferred(CHECK_BACKWARD_BLOCKED, "0 0 0", "ExpansionDebugArrow_Orange");
					m_Unit.Expansion_DebugObject_Deferred(CHECK_BACKWARD_OK, position - 0.5 * fb + CHECK_MIN_HEIGHT * 0.5, "ExpansionDebugArrow", -fb);
				}
			}

#ifdef DIAG
			if (m_OverrideTargetMovementDirection != overrideTargetMovementDirection)
				chg = true;

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

		m_PrevPosition = position;

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
				m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_OK, "0 0 0", "ExpansionDebugArrow_Blue");
			if (m_Unit.m_Expansion_DebugObjects[CHECK_LEFT_GO])
				m_Unit.Expansion_DebugObject_Deferred(CHECK_LEFT_GO, "0 0 0", "ExpansionDebugArrow");
			if (m_Unit.m_Expansion_DebugObjects[CHECK_RIGHT_OK])
				m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_OK, "0 0 0", "ExpansionDebugArrow_Blue");
			if (m_Unit.m_Expansion_DebugObjects[CHECK_RIGHT_GO])
				m_Unit.Expansion_DebugObject_Deferred(CHECK_RIGHT_GO, "0 0 0", "ExpansionDebugArrow");
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

		int speedLimit;
		if (m_StanceChangeTimeout > 0)
		{
			speedLimit = 0;  //! Have to stop moving else it breaks animation state
		}
		else
		{
			switch (m_Stance)
			{
				case DayZPlayerConstants.STANCEIDX_PRONE:
					if (m_Unit.IsRaised() || m_Unit.GetWeaponManager().IsRunning() || m_Unit.GetActionManager().GetRunningAction())
						speedLimit = 0;  //! Have to stop moving else it breaks animation state
					else
						speedLimit = Math.Min(1, m_SpeedLimit);
					break;
				case DayZPlayerConstants.STANCEIDX_CROUCH:
					speedLimit = Math.Min(2, m_SpeedLimit);
					break;
				default:
					speedLimit = m_SpeedLimit;
					break;
			}

			//! Limit speed to jog when moving bwd or avoiding obstacles
			if ((Math.AbsFloat(m_MovementDirection) > 90 || m_OverrideMovementTimeout > 0) && speedLimit > 2)
				speedLimit = 2;
		}

		if (wayPoint != position)
			m_WayPointDistance2D = pathDir2D.LengthSq();
		else
			m_WayPointDistance2D = 0.0;

		if (m_MovementSpeed != 0)
		{
			if (ShouldRecalculateTurnTarget())
			{
				//! Turn to waypoint while moving
				m_TurnTarget = pathDir2D.Normalized().VectorToAngles()[0];
			}

			if (m_TurnOverride)
			{
				speedLimit = m_TurnOverride;
				m_TurnTarget = orientation[0] + m_TurnDirection;
				if (m_TurnTarget >= 360.0)
					m_TurnTarget -= 360.0;
				else if (m_TurnTarget < 0)
					m_TurnTarget += 360.0;
			}
		}

		/*
		if (wayPoint != position)
		{
			float y = GetGame().SurfaceY(wayPoint[0], wayPoint[2]);
			if (y > wayPoint[1]) wayPoint[1] = y;

			if (DayZPhysics.RayCastBullet(wayPoint + Vector(0.0, 1.5, 0.0), wayPoint - Vector(0.0, 10.0, 0.0), m_CollisionLayerMask|PhxInteractionLayers.TERRAIN, m_Player, m_HitObject, m_HitPosition, m_HitNormal, m_HitFraction))
				wayPoint = m_HitPosition;

			m_WayPointDistance = vector.DistanceSq(position, wayPoint);
		}
		else
		{
			m_WayPointDistance = 0.0;
		}
		*/

		m_Turn = orientation[0];

		if (m_OverrideMovementTimeout > 0)
		{
			m_TargetMovementDirection = m_OverrideTargetMovementDirection;
			m_ForceMovementDirection = true;
		}
		//! Turn towards aim direction when raised but keep moving in waypoint direction (strafe/backpedal if necessary)
		else if (m_MovementSpeed > 0 && m_Unit.IsRaised())
		{
			float aimAngle = m_Unit.GetAimDirection().VectorToAngles()[0];
			float aimToMovementAngle = ExpansionMath.AngleDiff2(aimAngle, m_TurnTarget);
			float aimToMovementAngleAbs = Math.AbsFloat(aimToMovementAngle);

			if (aimToMovementAngleAbs >= 90)
			{

				m_TargetMovementDirection = ExpansionMath.AngleDiff2(m_Turn, m_TurnTarget);
				m_ForceMovementDirection = true;
				m_TurnTarget = aimAngle;
			}
		}

		//! Clockwise: Positive, counter-clockwise: Negative
		m_TurnDifference = ExpansionMath.AngleDiff2(m_Turn, m_TurnTarget);

		bool matchLeaderSpeed;

		eAIGroup group = m_Unit.GetGroup();
		DayZPlayerImplement leader = group.GetFormationLeader();
		if (m_Unit.GetFSM().IsInState("FollowFormation"))
		{
			if (leader && !leader.eAI_IsSideSteppingObstacles() && Math.AbsFloat(leader.Expansion_GetMovementAngle()) <= 90.0)
			{
				float leaderSpeed = GetVelocity(leader).LengthSq();
				//if (EXTrace.AI && leaderSpeed > 0.0)
					//EXPrint(leader.ToString() + " velocity " + leaderSpeed);
				/**
				 * Movement speeds (approx)
				 * 
				 * Empty handed, walk: 2.3 m/s
				 * Rifle in hand, walk: 1.87733 m/s
				 * Axe in hand, walk: 2.37429 m/s
				 * Pistol in hand, walk: 2.3 m/s
				 * Knife in hand, walk: 2.3 m/s
				 */
				if (leaderSpeed > 1.8)
					matchLeaderSpeed = true;
			}
		}
		else if (m_Unit == leader && group.Count() > 1)
		{
			group.GetFormation().Update(pDt);
		}

		if (m_WayPointDistance2D < m_MinFinal)
			m_UpdatePath = true;

		if (m_UpdatePath)
		{
			if (isPathPointFinal && wayPoint != position)
			{
				if (matchLeaderSpeed || wayPoint != m_PathFinding.GetTarget())
				{
					m_PathFinding.ForceRecalculate(true);
				}
			#ifdef DIAG
				m_Unit.Expansion_DeleteDebugObject(11111 + m_PathFinding.m_PointIdx);
			#endif
			}
			else
			{
				m_PathFinding.UpdateNext(true);
			}
		}

		//! https://feedback.bistudio.com/T173348
		if (isTargetPositionFinal && m_WayPointDistance2D < m_MinFinal && !matchLeaderSpeed)
			m_Unit.m_eAI_PositionIsFinal = true;
		else
			m_Unit.m_eAI_PositionIsFinal = false;

	//#ifdef DIAG
		//Object dbgObj;
		//vector dbgOri;
	//#endif

		if (m_Unit.eAI_IsSideSteppingVehicle())
		{
			SetTargetSpeed(Math.Lerp(m_MovementSpeed, 3.0, pDt * 4.0));
		}
		else if (m_Unit.m_eAI_PositionIsFinal)
		{
		//#ifdef DIAG
			//dbgObj = m_Unit.m_Expansion_DebugObjects[11106];
			//if (dbgObj)
				//dbgOri = dbgObj.GetOrientation();
			//else
				//dbgOri = m_Unit.GetOrientation();
			//dbgOri[0] = dbgOri[0] + 1;
			//m_Unit.Expansion_DebugObject_Deferred(11106, m_Unit.GetPosition(), "ExpansionDebugNoticeMe_White", dbgOri.AnglesToVector());
		//#endif
			SetTargetSpeed(0.0);
		}
		else if (Math.AbsFloat(m_TurnDifference) > 30.0 && !m_TurnOverride)
		{
		//#ifdef DIAG
			//dbgObj = m_Unit.m_Expansion_DebugObjects[11107];
			//if (dbgObj)
				//dbgOri = dbgObj.GetOrientation();
			//else
				//dbgOri = m_Unit.GetOrientation();
			//dbgOri[0] = dbgOri[0] + 1;
			//m_Unit.Expansion_DebugObject_Deferred(11107, m_Unit.GetPosition(), "ExpansionDebugNoticeMe_Black", dbgOri.AnglesToVector());
		//#endif
			if (ExpansionMath.Distance2DSq(position, m_PathFinding.GetEnd()) >= m_MinFinal)
				SetTargetSpeed(Math.Lerp(m_MovementSpeed, 1.0, pDt * 2.0));
		}
		else if (m_Unit.IsRaised())
		{
		//#ifdef DIAG
			//dbgObj = m_Unit.m_Expansion_DebugObjects[11108];
			//if (dbgObj)
				//dbgOri = dbgObj.GetOrientation();
			//else
				//dbgOri = m_Unit.GetOrientation();
			//dbgOri[0] = dbgOri[0] + 1;
			//m_Unit.Expansion_DebugObject_Deferred(11108, m_Unit.GetPosition(), "ExpansionDebugNoticeMe_Orange", dbgOri.AnglesToVector());
		//#endif
			if (ExpansionMath.Distance2DSq(position, m_PathFinding.GetEnd()) >= m_MinFinal)
				SetTargetSpeed(2.0);
		}
		else if (!m_SpeedOverrider)
		{
		//#ifdef DIAG
			//dbgObj = m_Unit.m_Expansion_DebugObjects[11109];
			//if (dbgObj)
				//dbgOri = dbgObj.GetOrientation();
			//else
				//dbgOri = m_Unit.GetOrientation();
			//dbgOri[0] = dbgOri[0] + 1;
			//m_Unit.Expansion_DebugObject_Deferred(11109, m_Unit.GetPosition(), "ExpansionDebugNoticeMe_Red", dbgOri.AnglesToVector());
		//#endif
			float distanceFactor = 4.0;
			if (matchLeaderSpeed || (m_Unit.GetThreatToSelf() >= 0.4 && m_Unit.eAI_HasLOS()))
				distanceFactor = 0.5;
			float targetSpeed;
			if (isTargetPositionFinal && m_WayPointDistance2D < 2.0 * distanceFactor)
			{
				if (matchLeaderSpeed)
					targetSpeed = leader.Expansion_GetMovementSpeed();
				else
					targetSpeed = 1.0;
			}
			else if (isTargetPositionFinal && m_WayPointDistance2D < 5.0 * distanceFactor)
			{
				if (matchLeaderSpeed)
					targetSpeed = leader.Expansion_GetMovementSpeed();
				else
					targetSpeed = 2.0;
			}
			else
			{
				if (Math.AbsFloat(m_TargetMovementDirection) > 90)
					targetSpeed = 2.0;  //! Can't sprint backwards
				else
					targetSpeed = 3.0;
			}

			SetTargetSpeed(Math.Lerp(m_MovementSpeed, targetSpeed, pDt * 2.0));
		}

		DBGDrawSphere(wayPoint, 0.05, 0xFF00FF00);
		//DBGDrawLine(position, wayPoint, 0xFF00FF00);
		m_Unit.Expansion_DebugObject_Deferred(CORRECTED_WAYPOINT, wayPoint, "ExpansionDebugBox");

		float dirChangeSpeed = pDt;
		if (m_ForceMovementDirection)
		{
			if (m_TargetMovementDirection == 0)
				dirChangeSpeed *= 24;
			else
				dirChangeSpeed *= 4;
		}

		m_MovementDirection += ExpansionMath.AngleDiff2(m_MovementDirection, m_TargetMovementDirection) * dirChangeSpeed;
		m_MovementDirection = Math.Clamp(m_MovementDirection, -180.0, 180.0);


		if (m_Stance != m_StancePrev)
		{
			m_Table.SetStance(this, m_Stance);
			m_StanceChangeTimeout = 0.5 * Math.AbsFloat(Math.Max(m_StancePrev, 0) - m_Stance);
			m_StancePrev = m_Stance;
		}

		if (m_TargetSpeed > speedLimit && speedLimit != -1)
			m_MovementSpeed = speedLimit;
		else
			m_MovementSpeed = m_TargetSpeed;

		m_Table.SetMovementDirection(this, m_MovementDirection);
		m_Table.SetMovementSpeed(this, m_MovementSpeed);

		//m_Table.SetAimX(this, m_AimLR);
		//m_Table.SetAimY(this, m_AimUD);

		//m_TurnVelocity = ExpansionMath.AngleDiff2(m_Turn, m_TurnPrevious);
		//m_TurnPrevious = m_Turn;

		if (m_Unit.m_eAI_MemeLevel && !m_Unit.IsClimbing() && !m_Unit.IsFalling())
		{
			bool meme;

			if (m_Unit.GetThreatToSelf() > 0.15)
			{
				//! If a friendly player leans, we lean
				eAITarget target = m_Unit.GetTarget();
				PlayerBase targetPlayer;
				if (target && Class.CastTo(targetPlayer, target.GetEntity()) && (m_Unit.m_eAI_Meme || Math.AbsFloat(targetPlayer.m_MovementState.m_fLeaning) > 0.5))
				{
					eAIFaction faction = group.GetFaction();
					if (faction.IsObserver())
					{
						meme = true;
					}
					else
					{
						eAIGroup targetGroup = targetPlayer.GetGroup();
						if (targetGroup)
						{
							if (group == targetGroup)
							{
								meme = true;
							}
							else
							{
								eAIFaction targetFaction = targetGroup.GetFaction();
								if (targetFaction.IsFriendly(faction) || targetFaction.IsFriendlyEntity(m_Unit, targetPlayer))
									meme = true;
							}
						}
					}
				}

				if (meme && !m_Unit.m_eAI_Meme)
					m_Unit.m_eAI_Meme = Math.RandomInt(2, 4);
			}

			bool returnToNeutral;

			switch (m_Unit.m_eAI_MemeLevel)
			{
				case 1:
					break;
				case 2:
					if (m_MovementSpeed && m_Unit.m_eAI_IsFightingFSM && !m_Unit.IsFighting())
						meme = true;
					break;
				case 3:
					if (m_MovementSpeed)
						meme = true;
					break;
				default:
					if (m_MovementSpeed || m_TurnState == TURN_STATE_TURNING)
						meme = true;
					break;
			}

			if (!meme && Math.AbsFloat(m_Unit.m_eAI_Lean) > 0.0)
			{
				//! Always return to neutral state
				meme = true;
				returnToNeutral = true;
			}

			if (meme)
			{
				float leanReverseThresh = Math.RandomFloat(0.5, 2.0);
				if (Math.AbsFloat(m_Unit.m_eAI_Lean) > leanReverseThresh)
				{
					if (m_Unit.m_eAI_Lean > 0.0)
						m_Unit.m_eAI_Lean = leanReverseThresh;
					else
						m_Unit.m_eAI_Lean = -leanReverseThresh;

					m_LeanDirection = -m_LeanDirection;

					if (m_Unit.m_eAI_Meme)
						m_Unit.m_eAI_Meme--;
				}

				float prevLean = m_Unit.m_eAI_Lean;

				m_Unit.m_eAI_Lean = m_Unit.m_eAI_Lean + pDt * 4.0 * m_LeanDirection;

				if (returnToNeutral && ((m_Unit.m_eAI_Lean < 0.0 && prevLean > 0.0) || (m_Unit.m_eAI_Lean > 0.0 && prevLean < 0.0)))
					m_Unit.m_eAI_Lean = 0.0;

				m_Table.SetLean(this, ExpansionMath.SmoothStep(Math.Clamp(m_Unit.m_eAI_Lean, -1.0, 1.0), 1, -1.0, 1.0));
				//m_Table.SetLean(this, m_Unit.m_eAI_Lean);
			}
		}

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
			if (parent)
				turnTargetActual -= parent.GetOrientation()[0];

			if (turnTargetActual > 180.0)
				turnTargetActual -= 360.0;

			bool isAboveGround;
			if (position[1] - GetGame().SurfaceY(position[0], position[2]) > 0.5)
				isAboveGround = true;

			//! Enable sharper turns if above ground or waypoint is close and not avoiding obstacles
			if (!m_Unit.IsRaised() && (isAboveGround || (m_WayPointDistance2D < 8.0 && m_OverrideMovementTimeout <= 0)))
				PreAnim_SetFilteredHeading(-turnTargetActual * Math.DEG2RAD, 0.15, 30.0);
			else
				PreAnim_SetFilteredHeading(-turnTargetActual * Math.DEG2RAD, 0.3, 30.0);
		}

		if (m_WeaponFire && !m_IsTagWeaponFire)
		{
			m_Table.CallWeaponFire(this);
			m_WeaponFire = false;
		}
	}

	override void PrePhysUpdate(float pDt)
	{
		if (PrePhys_IsTag(m_Table.m_TAG_WeaponFire))
			m_IsTagWeaponFire = true;
		else
			m_IsTagWeaponFire = false;
	}

	override bool PostPhysUpdate(float pDt)
	{
		return true;
	}

	private bool Raycast(vector start, vector end, out vector hitPosition, out vector hitNormal, out float hitFraction, vector endRV = vector.Zero, float radiusRV = 0.25, bool includeAI = false, out Object blockingObject = null, bool updatePathIfTreeClose = false)
	{
		if (endRV == vector.Zero)
			endRV = end;

		vector dir = vector.Direction(start, end).Normalized();
		bool hit;

		//! 1st raycast specifically for trees
		int contactComponent;
		set<Object> results();
		if (DayZPhysics.RaycastRV(start + dir * radiusRV * 0.5, endRV, hitPosition, hitNormal, contactComponent, results, null, m_Unit, false, false, ObjIntersectGeom, radiusRV))
		{
			foreach (Object obj: results)
			{
				if (obj.IsTree())
				{
					hit = true;

					if (updatePathIfTreeClose && vector.DistanceSq(m_Waypoint, hitPosition) < 16.0)
						m_UpdatePath = true;
				}

				blockingObject = obj;
			}
		}

		//! Everything else
		if (!hit)
		{
			IEntity hitEntity;
			hitFraction = m_Unit.CollisionMoveTest(dir * 0.8, vector.Zero, 1.0, null, hitEntity, hitPosition, hitNormal);
			if (hitFraction < 0.5)
			{
				//ExpansionStatic.MessageNearPlayers(m_Unit.GetPosition(), 100.0, "IEnt " + hitEntity + " frac " + hitFraction);
				blockingObject = Object.Cast(hitEntity);
				m_PathFinding.ForceRecalculate();
				return true;
			}

			return false;

			Object hitObject;
			PhxInteractionLayers hit_mask = PhxInteractionLayers.BUILDING | PhxInteractionLayers.DOOR | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.ITEM_LARGE | PhxInteractionLayers.FENCE;
			if (includeAI)
				hit_mask |= PhxInteractionLayers.CHARACTER | PhxInteractionLayers.AI;
			hit = DayZPhysics.SphereCastBullet(start + dir * 0.125, end, 0.25, hit_mask, m_Unit, hitObject, hitPosition, hitNormal, hitFraction);
			hitFraction = 1.0 - hitFraction;
			if (hitObject)
				blockingObject = hitObject;
		}

#ifdef DIAG
#ifndef SERVER
		int debugColour = 0xFF00AAFF;
		if (hit) debugColour = 0xFFAA00FF;
		vector points2[2];
		points2[0] = start;
		points2[1] = end;
		if (hit) points2[1] = hitPosition;
		m_Unit.AddShape(Shape.CreateLines(debugColour, ShapeFlags.NOZBUFFER, points2, 2));
#endif
#endif

		return hit;
	}

	float GetWaypointDistance()
	{
		return m_WayPointDistance;
	}

	bool IsSideSteppingObstacles()
	{
		return m_LastBlocked || m_LastBlockedLeft || m_LastBlockedRight;
	}

	bool IsBlocked()
	{
		if (!m_MovementSpeed || m_Unit.IsClimbing() || m_Unit.IsFalling() || m_Unit.IsFighting())
			return false;

		if (!m_LastBlocked)
			return false;

		return true;
	}

	bool CheckBlocked()
	{
		if (!IsBlocked() || !CheckBlockedLeft() || !CheckBlockedRight())
			return false;

		return true;
	}

	bool CheckBlockedLeft()
	{
		if (!m_MovementSpeed || m_Unit.IsClimbing() || m_Unit.IsFalling() || m_Unit.IsFighting())
			return false;

		bool isBlocked = m_LastBlockedLeft;
		float blockDistSq = m_BlockedRightDist;

		if (!m_LastBlockedLeft)
		{
			vector position = m_Transform[3];
			vector lr = m_Direction.Perpend();
			vector checkDir = position + 0.5 * m_Direction;
			if (Math.AbsFloat(m_MovementDirection) >= 135)
				checkDir = checkDir * -1.0;
			vector checkLeft = position + 0.25 * lr;
			vector hitPosition;
			vector hitNormal;
			float hitFraction;
			isBlocked = this.Raycast(position + CHECK_MIN_HEIGHT, checkLeft + CHECK_MIN_HEIGHT, hitPosition, hitNormal, hitFraction, checkDir + lr + CHECK_MIN_HEIGHT, 0.5);
			if (isBlocked)
				blockDistSq = vector.DistanceSq(position, hitPosition);
		}

		if (isBlocked && blockDistSq > 0.16)
			return false;

		return isBlocked;
	}

	bool CheckBlockedRight()
	{
		if (!m_MovementSpeed || m_Unit.IsClimbing() || m_Unit.IsFalling() || m_Unit.IsFighting())
			return false;

		bool isBlocked = m_LastBlockedRight;
		float blockDistSq = m_BlockedLeftDist;

		if (!m_LastBlockedRight)
		{
			vector position = m_Transform[3];
			vector lr = m_Direction.Perpend();
			vector checkDir = position + 0.5 * m_Direction;
			if (Math.AbsFloat(m_MovementDirection) >= 135)
				checkDir = checkDir * -1.0;
			vector checkRight = position - 0.25 * lr;
			vector hitPosition;
			vector hitNormal;
			float hitFraction;
			isBlocked = this.Raycast(position + CHECK_MIN_HEIGHT, checkRight + CHECK_MIN_HEIGHT, hitPosition, hitNormal, hitFraction, checkDir - lr + CHECK_MIN_HEIGHT, 0.5);
			if (isBlocked)
				blockDistSq = vector.DistanceSq(position, hitPosition);
		}

		if (isBlocked && blockDistSq > 0.16)
			return false;

		return isBlocked;
	}

	Object GetBlockingObject()
	{
		return m_BlockingObject;
	}

	float GetBlockedTime()
	{
		return m_BlockedTime;
	}
};
