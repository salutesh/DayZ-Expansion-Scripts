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
	private vector m_PrevPosition;
	private float m_PositionTime;

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

	private int m_Stance = -1;
	private int m_StancePrev = -1;
	private float m_StanceChangeTimeout;

	private bool m_LastBlockedForward;
	private bool m_LastBlockedLeft;
	private float m_BlockedLeftDist;
	private bool m_LastBlockedRight;
	private float m_BlockedRightDist;
	private bool m_LastBlockedBackward;
	private bool m_LastBlocked;
	private float m_TurnDirection = 45.0;
	private bool m_TurnOverride;

	private float m_OverrideMovementTimeout;
	private float m_OverrideTargetMovementDirection;
	private vector m_OverrideWaypoint;
	private float m_DebugTime;

	bool m_IsTagWeaponFire;
	bool m_WeaponFire;

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

		//! https://feedback.bistudio.com/T173348
		if (Math.AbsFloat(m_LookLR) > 0.01 || Math.AbsFloat(m_LookUD) > 0.01)
			m_Look = true;
		else
			m_Look = false;
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

		vector debug_points[2];

		vector orientation = Math3D.MatrixToAngles(m_Transform);
		vector position = m_Transform[3];

		vector wayPoint = position;
		bool isFinal = true;
		vector pathDir2D;
		vector fb = m_Transform[2];
		m_Direction = fb;
		if (m_SpeedLimit)
		{
#ifndef EAI_USE_LEGACY_PATHFINDING
			//! https://feedback.bistudio.com/T173348
			if (m_PathFinding.GetNext(wayPoint) <= 2 && m_Unit.m_eAI_TargetPositionIsFinal)
				isFinal = true;
			else
				isFinal = false;
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

				isFinal = wayPointIndex == m_PathFinding.Count() - 1;
			}
#endif

			pathDir2D = vector.Direction(position, Vector(wayPoint[0], position[1], wayPoint[2]));

			if (isFinal && m_PathFinding.m_IsUnreachable)
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
					if (GetGame().GetWaterDepth(position + fb - "0 1 0") > 0.5)
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

		bool blockedForward;
		bool blockedLeft;
		bool blockedRight;
		bool blockedBackward;

		bool moveRight;
		bool backPedal;
		bool turnOverride;

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
			m_LastBlockedForward = false;
			m_LastBlockedBackward = false;
			m_LastBlockedLeft = false;
			m_LastBlockedRight = false;
			m_TurnOverride = false;
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
#ifdef DIAG
			string msg;
#endif

			if (vector.DistanceSq(m_PrevPosition, position) < 0.0016)
				m_PositionTime += pDt;  //! We don't seem to be actually moving
			else
				m_PositionTime = 0;

			vector checkDir;

			//! Only check bwd if we are moving bwd, else check fwd
			if (Math.AbsFloat(m_MovementDirection) >= 135)
			{
				checkDir = position - 0.5 * fb;
				blockedBackward = Raycast(position + CHECK_MIN_HEIGHT, checkDir + CHECK_MIN_HEIGHT, backwardPos, outNormal, hitFraction, checkDir + CHECK_MIN_HEIGHT, 0.5, true);

				if (!blockedBackward && m_PositionTime > 0.3)
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
				checkDir = position + 0.5 * fb;
				blockedForward = Raycast(position + CHECK_MIN_HEIGHT, checkDir + CHECK_MIN_HEIGHT, forwardPos, outNormal, hitFraction, position + fb * m_MovementSpeed + CHECK_MIN_HEIGHT, 1.0, true);

				if (!blockedForward && m_PositionTime > 0.3)
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
				blockedFwdOrBwd = true;

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
				blockedLeft = Raycast(position + CHECK_MIN_HEIGHT, checkLeft + CHECK_MIN_HEIGHT, leftPos, outNormal, hitFraction, checkDir + lr + CHECK_MIN_HEIGHT, 0.5);
				if (blockedLeft)
					m_Unit.Expansion_DebugObject_Deferred(BLOCKED_LEFT_HITPOSITION, leftPos, "ExpansionDebugBox_Purple", outNormal);
				blockedRight = Raycast(position + CHECK_MIN_HEIGHT, checkRight + CHECK_MIN_HEIGHT, rightPos, outNormal, hitFraction, checkDir - lr + CHECK_MIN_HEIGHT, 0.5);
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
							m_OverrideMovementTimeout = 1.0;
						m_OverrideWaypoint = backwardPos;
					#ifdef EAI_DEBUG_MOVE
						EXTrace.Print(EXTrace.AI, this, m_Unit.ToString() + " blocked L+R, move bwd");
					#endif
					}
					else if (turnOverride)
					{
						m_OverrideTargetMovementDirection = 0.0;
						if (m_OverrideMovementTimeout <= 0)
						{
							m_OverrideMovementTimeout = 3.0;
							if (Math.RandomIntInclusive(0, 1))
								m_TurnDirection = -m_TurnDirection;
						}
						m_OverrideWaypoint = forwardPos;
						m_TurnOverride = true;
					#ifdef EAI_DEBUG_MOVE
						EXTrace.Print(EXTrace.AI, this, m_Unit.ToString() + " blocked L+R+B, turn " + m_TurnDirection);
					#endif
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
							m_OverrideMovementTimeout = 1 / m_MovementSpeed;
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

		float minFinal = 0.3;

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

		if (m_MovementSpeed != 0)
		{
			m_WayPointDistance2D = pathDir2D.LengthSq();
			if (m_WayPointDistance2D >= 0.04)  //! 0.2 m
			{
				m_PrevWaypoint = wayPoint;
				if (!m_TurnOverride && (m_OverrideMovementTimeout > 0 || !m_ForceTurnTarget))
					m_TurnTarget = pathDir2D.Normalized().VectorToAngles()[0];
			}

			if (m_TurnOverride)
			{
				speedLimit = 1;
				m_TurnTarget = orientation[0] + m_TurnDirection;
				if (m_TurnTarget >= 360.0)
					m_TurnTarget -= 360.0;
				else if (m_TurnTarget < 0)
					m_TurnTarget += 360.0;
			}
		}

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

		if (m_OverrideMovementTimeout > 0)
		{
			m_TargetMovementDirection = m_OverrideTargetMovementDirection;
			m_ForceMovementDirection = true;
		}

		m_Turn = orientation[0];

		//! Clockwise: Positive, counter-clockwise: Negative
		m_TurnDifference = ExpansionMath.AngleDiff2(m_Turn, m_TurnTarget);

		bool matchLeaderSpeed;

		DayZPlayerImplement leader;
		if (m_Unit.GetFSM().IsInState("FollowFormation"))
		{
			leader = m_Unit.GetGroup().GetLeader();
			if (leader && !leader.eAI_IsSideSteppingObstacles() && Math.AbsFloat(leader.Expansion_GetMovementAngle()) <= 90.0)
			{
				float leaderSpeed = GetVelocity(leader).LengthSq();
				//if (EXTrace.AI && leaderSpeed > 0.0)
					//EXPrint(leader, "velocity " + leaderSpeed);
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

		//! https://feedback.bistudio.com/T173348
		if (isFinal && m_WayPointDistance < minFinal && !matchLeaderSpeed)
			m_Unit.m_eAI_PositionIsFinal = true;
		else
			m_Unit.m_eAI_PositionIsFinal = false;
		if (m_Unit.m_eAI_PositionIsFinal)
		{
			SetTargetSpeed(0.0);
		}
		else if (Math.AbsFloat(m_TurnDifference) > 30.0)
		{
			SetTargetSpeed(Math.Lerp(m_MovementSpeed, 1.0, pDt * 2.0));
		}
		else if (!m_SpeedOverrider)
		{
			float distanceFactor = 4.0;
			if (matchLeaderSpeed || (m_Unit.GetThreatToSelf() > 0.4 && m_Unit.eAI_HasLOS()))
				distanceFactor = 0.5;
			float targetSpeed;
			if (isFinal && m_WayPointDistance < 2.0 * distanceFactor)
			{
				if (matchLeaderSpeed)
					targetSpeed = leader.Expansion_GetMovementSpeed();
				else
					targetSpeed = 1.0;
			}
			else if (isFinal && m_WayPointDistance < 5.0 * distanceFactor)
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
			dirChangeSpeed *= 4;

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

			bool isAboveGround;
			if (position[1] - GetGame().SurfaceY(position[0], position[2]) > 0.5)
				isAboveGround = true;

			//! Enable sharper turns if above ground or waypoint is close and not avoiding obstacles
			if (isAboveGround || (m_WayPointDistance < 8.0 && m_OverrideMovementTimeout <= 0))
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

	private bool Raycast(vector start, vector end, out vector hitPosition, out vector hitNormal, out float hitFraction, vector endRV = vector.Zero, float radiusRV = 0.25, bool includeAI = false)
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
			hit = DayZPhysics.SphereCastBullet(start + dir * 0.125, end, 0.25, hit_mask, m_Unit, hitObject, hitPosition, hitNormal, hitFraction);
			hitFraction = 1.0 - hitFraction;
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

	bool CheckStuck()
	{
		if (!m_MovementSpeed || m_Unit.IsClimbing() || m_Unit.IsFalling() || m_Unit.IsFighting())
			return false;

		if (!m_LastBlocked || !CheckBlockedLeft() || !CheckBlockedRight())
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
			isBlocked = Raycast(position + CHECK_MIN_HEIGHT, checkLeft + CHECK_MIN_HEIGHT, hitPosition, hitNormal, hitFraction, checkDir + lr + CHECK_MIN_HEIGHT, 0.5);
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
			isBlocked = Raycast(position + CHECK_MIN_HEIGHT, checkRight + CHECK_MIN_HEIGHT, hitPosition, hitNormal, hitFraction, checkDir - lr + CHECK_MIN_HEIGHT, 0.5);
			if (isBlocked)
				blockDistSq = vector.DistanceSq(position, hitPosition);
		}

		if (isBlocked && blockDistSq > 0.16)
			return false;

		return isBlocked;
	}
};
