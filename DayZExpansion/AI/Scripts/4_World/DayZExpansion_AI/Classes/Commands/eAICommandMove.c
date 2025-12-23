class eAICommandMove: ExpansionHumanCommand
{
	static const int TURN_STATE_NONE = 0;
	static const int TURN_STATE_TURNING = 1;
	static const float MAX_NONRAISED_STRAFE_TIME = 5.0;
	static const float STRAFE_TIME_RESET_THRESHOLD = 3.5;

	static const vector CHECK_MIN_HEIGHT = "0 1.25 0";
	static const vector CHECK_MIN_HEIGHT_BULLET = "0 0.75 0";

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

	//! model name w/o .p3d ext, can be partial
	static ref TStringArray s_NonBlocking = {
		"misc_walkover"
	};

	private int m_PreviousInteractionLayer;

	private eAIBase m_Unit;
#ifndef EAI_USE_LEGACY_PATHFINDING
	private ExpansionPathHandler m_PathFinding;
#else
	private eAIPathFinding m_PathFinding;
#endif
	private vector m_Waypoint;
	private vector m_PathDir;
	private vector m_PathDirNormalized;
	private vector m_PathAngles;
	private vector m_PathDir2D;
	private vector m_PathDir2DNormalized;
	private float m_WaypointDistance;
	private float m_WaypointDistance2DSq;
	private float m_MinFinal = 0.3;
	private bool m_UpdatePath;

	private float m_DT;
	private float m_SmoothVel[1];
	private float m_Turn;
	private float m_TurnTarget;
	private bool m_ForceTurnTarget;
	private float m_TurnDifference;
	private float m_TurnTime;
	private float m_TurnPrevious;
	private float m_TurnVelocity;
	private int m_TurnState;
	private float m_StrafeTime;

	private vector m_Direction;
	private vector m_Velocity;
	private float m_MovementDirection;
	private bool m_ForceMovementDirection;
	private float m_TargetMovementDirection;

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
	private float m_StanceChangeTimeout;

	Object m_BlockingObject;
	Object m_PrevBlockingObject;
	private bool m_LastBlockedForward;
	private bool m_LastBlockedLeft;
	private float m_BlockedLeftDist;
	private bool m_LastBlockedRight;
	private float m_BlockedRightDist;
	private bool m_LastBlockedBackward;
	private bool m_LastBlocked;
	private bool m_IsBlockedByBuildingWithDoor;
	private bool m_IsBlockedByBuildingWithLadder;
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

	bool m_WasOnLadder;

	void eAICommandMove(DayZPlayerImplement player, ExpansionHumanST table, int stance = -1)
	{
		Class.CastTo(m_Unit, player);

		m_PathFinding = m_Unit.GetPathFinding();

		m_InstanceNum = s_InstanceCount++;

		SetStance(stance);

		m_SmoothVel[0] = 0.0;

		//m_Unit.GetInputController().SetDisabled(true);
	}

	void SetStance(int stance)
	{
		m_Stance = stance;
		SetCurrentStance(stance);
	}

	void ~eAICommandMove()
	{
	}

	override void OnActivate()
	{
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
		if (m_WaypointDistance2DSq >= m_MinFinal && !m_TurnOverride && (m_OverrideMovementTimeout > 0 || !m_ForceTurnTarget))
			return true;

		return false;
	}

	bool HasReachedWaypoint(bool isPathPointFinal)
	{
		if (m_WaypointDistance2DSq < m_MinFinal)
		{
			if (isPathPointFinal || !IsBlocked() || !m_BlockingObject || !m_BlockingObject.IsBuilding())
				return true;

			//! Have we passed the waypoint?
			if (m_Velocity.LengthSq() < 0.0001 || vector.Dot(m_Velocity.Normalized(), m_PathDir2DNormalized) < 0.7071)
				return true;
		}

		return false;
	}

	void StartTurnOverride(int speedLimit = 1)
	{
		if (m_OverrideMovementTimeout <= 0 || m_TurnOverride < speedLimit)
		{
			m_OverrideMovementTimeout = 1.0 / Math.Min(Math.Max(m_MovementSpeed, 1.0), speedLimit);
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
		//if (m_TargetSpeed > pTarget && pTarget < 2.0 && m_SpeedUpdateTime < 0.1) return;

		m_SpeedUpdateTime = 0;
		m_TargetSpeed = pTarget;
	}

	float GetCurrentMovementSpeed()
	{
		return m_MovementSpeed;
	}

	void SetCurrentMovementAngle(float angle)
	{
		m_MovementDirection = angle;
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

	float GetTargetDirection()
	{
		return m_TargetMovementDirection;
	}

	//! TODO: Remove force param, it's an artefact of the old HumanCommandScript code
	bool OverrideStance(int stance, bool force = false)
	{
		if (m_Unit.m_eAI_StancePreference != stance)
		{
			m_Unit.m_eAI_StancePreference = stance;

		//#ifdef DIAG_DEVELOPER
			//ExpansionStatic.MessageNearPlayers(m_Unit.GetPosition(), 100.0, m_Unit.ToString() + " override stance " + m_Unit.eAI_GetStance() + " -> " + m_Stance + " " + force);
		//#endif

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
		if (m_Unit.eAI_GetStance() != m_Stance)
			return true;

		return m_StanceChangeTimeout > 0;
	}

	override void PreAnimUpdate(float pDt)
	{
		if (!g_Game)
			return;

#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Profile(EXTrace.AI_PROFILE, m_Unit, "CommandHandler(15) -> eAICommandMove::PreAnimUpdate ");
#endif

		super.PreAnimUpdate(pDt);

		if (m_Unit.m_MovementState.m_CommandTypeId == DayZPlayerConstants.COMMANDID_VEHICLE)
			return;

		m_DT = pDt;

		if (m_StanceChangeTimeout > 0)
			m_StanceChangeTimeout -= pDt;

#ifdef DIAG_DEVELOPER
		auto hitch = new EXHitch(m_Unit.ToString() + " eAICommandMove::PreAnimUpdate ", 20000);
#endif

		m_SpeedUpdateTime += pDt;

		vector orientation = Math3D.MatrixToAngles(m_Transform);
		vector position = m_Transform[3];
		vector surfacePosition;

		vector waypoint = position;
		bool isPathPointFinal;  //! Is waypoint the final point of the current path? (not necessarily final target position!)
		bool isTargetPositionFinal = true;  //! Is waypoint the final target position?
		bool isDangerousAltitude;
		vector fb = m_Transform[2];
		m_Direction = fb;
		m_Velocity = m_Unit.Expansion_GetActualVelocity();
		if (m_SpeedLimit)
		{
#ifndef EAI_USE_LEGACY_PATHFINDING
			//! https://feedback.bistudio.com/T173348
			if (m_PathFinding.GetNext(waypoint) <= 2)
				isPathPointFinal = true;
			if (isPathPointFinal && (m_Unit.m_eAI_TargetPositionIsFinal || m_PathFinding.m_Count == 0))
				isTargetPositionFinal = true;
			else
				isTargetPositionFinal = false;
			//m_WaypointDistance = vector.DistanceSq(position, waypoint);
			DBGDrawSphere(waypoint, 0.05, 0xFFFF0000);
			//DBGDrawLine(position, waypoint, 0xFFFF0000);
		#ifdef DIAG_DEVELOPER
			if (m_PathFinding.m_IsUnreachable)
			{
				vector unreachableTarget = m_PathFinding.GetTarget();
				vector unreachableDir = m_PathFinding.m_PathSegmentDirection;
				unreachableDir[1] = 0.0;
				m_Unit.Expansion_DebugObject_Deferred(ORIGINAL_WAYPOINT, unreachableTarget, "ExpansionDebugNoticeMe_Red", unreachableDir);
			}
			else
			{
				m_Unit.Expansion_DebugObject_Deferred(ORIGINAL_WAYPOINT, "0 0 0", "ExpansionDebugNoticeMe_Red");
			}
		#endif
#else
			int waypointIndex;

			if (m_PathFinding.Count() >= 2)
			{
				waypointIndex = m_PathFinding.Next(position);
				waypoint = m_PathFinding[waypointIndex];

				isTargetPositionFinal = waypointIndex == m_PathFinding.Count() - 1;
			}
#endif

			m_PathDir = vector.Direction(position, waypoint);
			m_PathDirNormalized = m_PathDir.Normalized();
			m_PathAngles = m_PathDirNormalized.VectorToAngles();
			m_PathDir2D = Vector(m_PathDir[0], 0.0, m_PathDir[2]);
			m_PathDir2DNormalized = m_PathDir2D.Normalized();

			isDangerousAltitude = m_Unit.eAI_IsDangerousAltitude();

			//if (isTargetPositionFinal && m_PathFinding.m_IsUnreachable)
			if ((isPathPointFinal || Math.AbsFloat(m_MovementDirection) > 4.0) && (!m_PathFinding.m_IsUnreachable || m_Unit.m_eAI_BuildingWithLadder))
			{
				if (isDangerousAltitude)
				{
					//! Prevent fall from a large height (e.g. building top) - movement direction check
					vector checkDirection = m_Velocity;
					checkDirection[1] = 0;

					if (checkDirection.LengthSq() < 0.0001)
					{
						checkDirection = m_Direction;

						if (Math.AbsFloat(m_MovementDirection) >= 135)
							checkDirection = checkDirection * -1.0;
					}
					else
					{
						checkDirection.Normalize();
					}

					if ((!m_Unit.m_eAI_Ladder || (!m_Unit.m_eAI_IsOnLadder && !m_Unit.eAI_IsCloseToLadderEntryPoint())) && !m_Unit.eAI_IsFallSafe(checkDirection * 2.0))
					{
					#ifdef DIAG_DEVELOPER
						if (!m_PathFinding.m_IsUnreachable)
						{
							EXTrace.Print(EXTrace.AI, this, m_Unit.ToString() + " unreachable (not fall safe)");
							ExpansionStatic.MessageNearPlayers(m_Unit.GetPosition(), 100.0, m_Unit.ToString() + " unreachable (not fall safe)");
						}
					#endif
						m_PathFinding.m_IsUnreachable = true;
						m_PathFinding.m_IsTargetUnreachable = true;
					}
					else if ((m_PathFinding.m_IsUnreachable || m_PathFinding.m_IsTargetUnreachable) && m_Unit.m_eAI_BuildingWithLadder && !m_Unit.m_eAI_IsOnLadder && m_PathFinding.m_Count > 0 && !m_PathFinding.m_IsBlocked)
					{
						m_PathFinding.ForceRecalculate();
					}
				}

				if (!m_PathFinding.m_IsUnreachable && (m_PathFinding.m_IsTargetUnreachable || (!m_Unit.IsSwimming() && !m_PathFinding.m_IsSwimmingEnabled && !m_Unit.GetGroup().IsFormationLeaderSwimming() && m_Unit.GetCurrentWaterLevel() < -0.5)))
				{
					//! Don't go into deep water
					surfacePosition = ExpansionStatic.GetSurfaceRoadPosition(position + fb);
					//! Swim start water level = 1.5 m, see DayZPlayerUtils::CheckWaterLevel
					if (g_Game.GetWaterDepth(surfacePosition) > 1.5)
					{
					#ifdef DIAG_DEVELOPER
						if (!m_PathFinding.m_IsUnreachable)
							EXTrace.Print(EXTrace.AI, this, m_Unit.ToString() + " unreachable (deep water)");
					#endif
						m_PathFinding.m_IsUnreachable = true;
						m_PathFinding.m_IsTargetUnreachable = true;
				#ifdef DIAG_DEVELOPER
						m_Unit.Expansion_DebugObject(1343, position + fb, "ExpansionDebugRodBig_Blue", fb, surfacePosition);
					}
					else
					{
						m_Unit.Expansion_DebugObject(1343, "0 0 0", "ExpansionDebugRodBig_Blue");
				#endif
					}
				}
			}

			if (m_PathFinding.m_IsUnreachable)
			{
				waypoint = position;
				m_MovementSpeed = 0;
				m_OverrideMovementTimeout = 0.0;
				isTargetPositionFinal = true;
			}
		}

		m_Waypoint = waypoint;

		eAIGroup group = m_Unit.GetGroup();
		DayZPlayerImplement leader = group.GetFormationLeader();

		float characterDepth = m_Unit.GetCurrentWaterLevel();

		if (!m_Unit.IsSwimming())
		{
			//! Disable swimming once we are 3.5m above sea level
			if (characterDepth < -3.5 && m_PathFinding.m_IsSwimmingEnabled && !m_Unit.m_eAI_EffectArea && m_Unit.m_eAI_SurfaceY >= g_Game.SurfaceGetSeaLevelMin())
			{
				if (leader == m_Unit || !leader.IsSwimming())
					m_PathFinding.EnableSwimming(false, false);
			}
		}

		bool blockedForward;
		bool blockedLeft;
		bool blockedRight;
		bool blockedBackward;

		bool moveLeft;
		bool moveRight;
		bool backPedal;
		bool turnOverride;
	
#ifdef DIAG_DEVELOPER
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
				#ifdef DIAG_DEVELOPER
					msg = " WARNING: CW to CCW turn time diff " + turnDirectionDiff + " exceeds 5 seconds! Changing direction";
					EXTrace.Print(EXTrace.AI, this, "eAICommandMove::StartTurnOverride -" + msg);
					ExpansionStatic.MessageNearPlayers(m_Unit.GetPosition(), 100, m_Unit.GetType() + msg);
				#endif
					if (m_Stance == DayZPlayerConstants.STANCEIDX_PRONE)
						m_TurnOverride = 0;
					else
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

		bool isBusy;

		if (m_Unit.IsClimbing() || m_Unit.IsFalling() || m_Unit.IsFighting())
			isBusy = true;

		//! Try and avoid obstacles if we are moving and not busy with other actions
		float cdt = m_Unit.m_eAI_CommandHandlerDT;
		if (m_MovementSpeed && !isBusy && cdt >= 0.12)
		{
			vector leftPos;
			vector rightPos;
			vector forwardPos;
			vector backwardPos;
			vector outNormal;
			float hitFraction;

			bool chg;

			vector velocity = m_Velocity;

			if (m_Unit.IsSwimming())
				velocity[1] = 0.0;  //! Set vertical velocity to zero to nullify effect of tide

			/**
			 * Measured movement speeds (avg)
			 * 
			 * ================================================================
			 * Full health
			 * ================================================================
			 * Empty handed     m/s               squared
			 * ----------------------------------------------------------------
			 * Prone            ~0.42459290       ~0.180279
			 * Crouch walk      ~1.31864166       ~1.738816
			 * Crouch run       ~3.5893068        ~12.883123
			 * Walk             ~1.52753647       ~2.333367
			 * Jog              ~4.08504166       ~16.687565
			 * Sprint           ~6.75593681       ~45.642682
			 * ----------------------------------------------------------------
			 * Rifle in hands   m/s
			 * ----------------------------------------------------------------
			 * Prone            ~0.48079174       ~0.231160
			 * Crouch walk      ~1.31665192       ~1.733572
			 * Crouch run       ~3.19667952       ~10.218759
			 * Walk             1.362             1.855044
			 * Jog              ~3.64278882       ~13.269910
			 * Sprint           ~6.70817238       ~44.999576
			 * 
			 * ================================================================
			 * Injured (5% health)
			 * ================================================================
			 * Rifle in hands   m/s               squared
			 * ----------------------------------------------------------------
			 * Prone            (same as full health)
			 * Crouch walk      ~1.075639         ~1.156999
			 * Crouch run       ~1.07554892       ~1.156805
			 * Walk             ~1.22531954       ~1.501407
			 * Jog              ~2.02479724       ~4.099803
			 * Sprint           ~2.03242218       ~4.130739
			 */
			float speedThreshold;
			if (m_Stance == DayZPlayerConstants.STANCEIDX_PRONE)  //! Prone
			{
				speedThreshold = 0.1;
			}
			else
			{
				if (m_Stance == DayZPlayerConstants.STANCEIDX_CROUCH || m_MovementSpeed < 2.0 || m_Unit.IsSwimming() || m_Unit.IsRaised())  //! Crouch, walk, swim or raised
					speedThreshold = 1.0;
				else
					speedThreshold = 2.0 * m_MovementSpeed;  //! Jog/sprint
				speedThreshold *= (1.0 - m_Unit.m_InjuryHandler.GetInjuryAnimValue() * 0.9);
			}
			if (velocity.LengthSq() < speedThreshold && !m_Unit.GetActionManager().GetRunningAction() && !m_Unit.IsRaised() && !m_Unit.m_eAI_IsOnLadder)
				m_Unit.m_eAI_PositionTime += cdt;  //! We don't seem to be actually moving
			else
				m_Unit.m_eAI_PositionTime = 0;

			vector checkDir;

			m_PrevBlockingObject = m_BlockingObject;
			m_BlockingObject = null;

			//! Only check bwd if we are moving bwd, else check fwd
			if (Math.AbsFloat(m_MovementDirection) >= 135)
			{
				if (m_Unit.m_eAI_IsOnLadder)
					fb = "0 -1 0";

				if (!m_Unit.IsRaised())
				{
					checkDir = position - 0.5 * fb;
					blockedBackward = this.Raycast(position, checkDir, backwardPos, outNormal, hitFraction, checkDir, 0.5, true, m_BlockingObject);
				}

				if (!blockedBackward && m_Unit.m_eAI_PositionTime > 4.0)
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
				if (m_Unit.m_eAI_IsOnLadder)
				{
					fb = "0 1 0";
					checkDir = position + 1.5 * fb;
				}
				else
				{
					fb = m_PathDirNormalized;
					checkDir = position + 0.5 * fb;
				}

				blockedForward = this.Raycast(position, checkDir, forwardPos, outNormal, hitFraction, position + fb * m_MovementSpeed * 1.7, 0.3, true, m_BlockingObject, true);

				if (!blockedForward && m_Unit.m_eAI_PositionTime > 4.0)
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
					m_Unit.m_eAI_BlockedTime += cdt;
			}
			else
			{
				m_Unit.m_eAI_BlockedTime = 0.0;
			}

			m_LastBlocked = blockedFwdOrBwd;

			if (!m_BlockingObject)
			{
				m_IsBlockedByBuildingWithDoor = false;
				m_IsBlockedByBuildingWithLadder = false;
			}
			else if (m_BlockingObject != m_PrevBlockingObject)
			{
				m_IsBlockedByBuildingWithDoor = false;
				m_IsBlockedByBuildingWithLadder = false;

				BuildingBase building;
				if (Class.CastTo(building, m_BlockingObject))
				{
					if (building.GetDoorCount() > 0)
						m_IsBlockedByBuildingWithDoor = true;

					if (blockedFwdOrBwd && !m_Unit.m_eAI_Ladder && building.Expansion_GetLaddersCount() > 0 && !m_Unit.eAI_IsExcludedBuildingWithLadder(building))
					{
						if (m_Unit.m_eAI_BuildingWithLadder != building)
						{
						#ifdef DIAG_DEVELOPER
							msg = "Bumped into building with ladder " + building.GetType();
							EXTrace.Print(EXTrace.AI, this, msg);
							ExpansionStatic.MessageNearPlayers(m_Unit.m_ExTransformPlayer[3], 100, m_Unit.ToString() + " " + msg);
						#endif

							m_Unit.m_eAI_BuildingWithLadder = building;
							m_Unit.m_eAI_LadderLoops = 0;
						}

						m_IsBlockedByBuildingWithLadder = true;
					}
				}
			}

			float overrideTargetMovementDirection = m_OverrideTargetMovementDirection;

			//! If during movement overriding we are no longer blocked in any direction, stop overriding imediately
			if (!blockedFwdOrBwd && m_OverrideMovementTimeout > 0)
			{
				m_OverrideMovementTimeout = 0;
				m_OverrideTargetMovementDirection = 0;
				m_ForceMovementDirection = true;
			}
			else if (m_Unit.m_eAI_IsOnLadder)
			{
				if (m_Unit.m_eAI_BlockedTime > 2.0)
				{
					m_Unit.m_eAI_LadderClimbDirection *= -1;
					m_Unit.m_eAI_BlockedTime = 0;
				}
			}
			//! Check left/right/bwd if blocked forward or already overriding movement
			else if (blockedFwdOrBwd || m_OverrideMovementTimeout > 0)
			{
				vector lr = fb.Perpend();
				vector checkLeft = position + 0.25 * lr;
				vector checkRight = position - 0.25 * lr;
				blockedLeft = this.Raycast(position, checkLeft, leftPos, outNormal, hitFraction, checkDir + lr, 0.5);
				if (!blockedLeft && m_Unit.m_eAI_PositionTime > 4.0)
					blockedLeft = true;
				if (blockedLeft)
					m_Unit.Expansion_DebugObject_Deferred(BLOCKED_LEFT_HITPOSITION, leftPos, "ExpansionDebugBox_Purple", outNormal);
				blockedRight = this.Raycast(position, checkRight, rightPos, outNormal, hitFraction, checkDir - lr, 0.5);
				if (!blockedRight && m_Unit.m_eAI_PositionTime > 4.0)
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

					//! Check which direction we want to move
					if (m_TurnOverride)
					{
						turnOverride = true;
					}
					else if (blockedLeft && blockedRight)
					{
						switch (m_Stance)
						{
							case DayZPlayerConstants.STANCEIDX_PRONE:
								if (m_Unit.m_eAI_BlockedTime > 10.0)
									turnOverride = true;
								break;

							default:
								//! This helps when on the catwalk of (e.g.) Land_Factory_Small so don't instantly start turning around
								//! our own axis at the corners
								if (m_Unit.m_eAI_BlockedTime > 0.2)
									turnOverride = true;
								break;
						}
					}

					if (turnOverride)
					{
						//! Don't set movement direction
					}
					else if (blockedLeft && !blockedRight)
					{
						//! Go right if blocked left
						moveRight = true;
					#ifdef EAI_DEBUG_MOVE
						EXTrace.Print(EXTrace.AI, this, m_Unit.ToString() + " blocked left, move right");
					#endif
					}
					else if (!blockedLeft && blockedRight)
					{
						//! Go left if blocked right
						moveLeft = true;
					#ifdef EAI_DEBUG_MOVE
						EXTrace.Print(EXTrace.AI, this, m_Unit.ToString() + " blocked right, move left");
					#endif
					}
					else
					{
						//! No direction blocked or both directions blocked

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
							else
							{
								moveLeft = true;
							#ifdef EAI_DEBUG_MOVE
								EXTrace.Print(EXTrace.AI, this, m_Unit.ToString() + " not blocked L+R, already moving left, keep moving left");
							#endif
							}
						}
						//! 50% chance move right
						else if (Math.RandomIntInclusive(0, 1))
						{
							moveRight = true;
						#ifdef EAI_DEBUG_MOVE
							EXTrace.Print(EXTrace.AI, this, m_Unit.ToString() + " not blocked L+R, move right");
						#endif
						}
						else
						{
							moveLeft = true;
						#ifdef EAI_DEBUG_MOVE
							EXTrace.Print(EXTrace.AI, this, m_Unit.ToString() + " not blocked L+R, move left");
						#endif
						}
					}

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
						StartTurnOverride(2);
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
					else if (moveLeft)
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
					//m_Unit.Expansion_DebugObject_Deferred(OVERRIDDEN_WAYPOINT, m_OverrideWaypoint, "ExpansionDebugBox_Orange");
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

#ifdef DIAG_DEVELOPER
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

#ifdef DIAG_DEVELOPER
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
				case DayZPlayerConstants.STANCEIDX_CROUCH:
					if (characterDepth >= 0.3 && !m_Unit.GetActionManager().GetRunningAction())  //! Neck position when crouched: 0.8 m above ground, +- 0.5 m tide tolerance
						OverrideStance(DayZPlayerConstants.STANCEIDX_ERECT);
					speedLimit = m_SpeedLimit;
					break;
				case DayZPlayerConstants.STANCEIDX_PRONE:
					if (m_Unit.GetActionManager().GetRunningAction())
					{
						speedLimit = 0;  //! Have to stop moving else it breaks animation state
					}
					else if (characterDepth >= 0.3)  //! Neck position when crouched: 0.8 m above ground, +- 0.5 m tide tolerance
					{
						OverrideStance(DayZPlayerConstants.STANCEIDX_ERECT);
						speedLimit = m_SpeedLimit;
					}
					else if (characterDepth >= -0.5)
					{
						OverrideStance(DayZPlayerConstants.STANCEIDX_CROUCH);
						speedLimit = m_SpeedLimit;
					}
					else if (m_Unit.IsRaised() || m_Unit.GetWeaponManager().IsRunning())
					{
						speedLimit = 0;  //! Have to stop moving else it breaks animation state
					}
					else
					{
						speedLimit = Math.Min(1, m_SpeedLimit);
					}
					break;
				default:
					speedLimit = m_SpeedLimit;
					break;
			}

			//! Limit speed to jog when avoiding obstacles, going downhill, altitude is dangerous or surface is deep enough under water for swimming
			if (speedLimit > 2)
			{
				if (m_OverrideMovementTimeout > 0 || (!m_Unit.IsSwimming() && (ExpansionMath.RelAngle(m_PathAngles[1]) < -25.0 || isDangerousAltitude || (!m_PathFinding.m_IsSwimmingEnabled && m_Unit.m_eAI_SurfaceY < g_Game.SurfaceGetSeaLevel() - 1.5))))
					speedLimit = 2;
			}
		}

		if (waypoint != position)
			m_WaypointDistance2DSq = m_PathDir2D.LengthSq();
		else
			m_WaypointDistance2DSq = 0.0;

		HumanCommandMove move = m_Unit.GetCommand_Move();

		if (m_MovementSpeed != 0)
		{
			if (move)
				m_MinFinal = Math.Max(move.GetCurrentMovementSpeed(), 1.0) * 0.1;

			if (ShouldRecalculateTurnTarget())
			{
				//! Turn to waypoint while moving
				m_TurnTarget = m_PathAngles[0];
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
		if (waypoint != position)
		{
			float y = g_Game.SurfaceY(waypoint[0], waypoint[2]);
			if (y > waypoint[1]) waypoint[1] = y;

			if (DayZPhysics.RayCastBullet(waypoint + Vector(0.0, 1.5, 0.0), waypoint - Vector(0.0, 10.0, 0.0), m_CollisionLayerMask|PhxInteractionLayers.TERRAIN, m_Player, m_HitObject, m_HitPosition, m_HitNormal, m_HitFraction))
				waypoint = m_HitPosition;

			m_WaypointDistance = vector.DistanceSq(position, waypoint);
		}
		else
		{
			m_WaypointDistance = 0.0;
		}
		*/

		m_Turn = orientation[0];

		eAITarget target = m_Unit.GetTarget();
		PlayerBase targetPlayer;
		eAIGroup targetGroup;

		if (target && Class.CastTo(targetPlayer, target.GetEntity()))
			targetGroup = targetPlayer.GetGroup();

		if (m_OverrideMovementTimeout > 0)
		{
			m_TargetMovementDirection = m_OverrideTargetMovementDirection;
			m_ForceMovementDirection = true;
		}
		//! Turn towards aim direction when raised or no look target but keep moving in waypoint direction (strafe/backpedal if necessary)
		//! @note on target handling:
		//! We only look at targets if threat level > 0.1, so consequently we can be sure we don't turn towards a target
		//! if we check it's threat level is below that.
		//! We must never turn towards targets if only looking briefly since that can make the AI spin around its axis repeatedly
		//! which looks very awkward.
		//! We ignore targets with a lifetime less than three seconds for that reason.
		//! @note on ladder handling:
		//! If we have a ladder target, we don't strafe/backpedal as it would interfere with looking towards ladder
		//! (handled in eAIBase::eAI_UpdateLookDirectionTarget)
		else if (m_MovementSpeed > 0 && m_WaypointDistance2DSq > 0.0001 && !m_Unit.IsSwimming() && !m_Unit.m_eAI_Ladder)
		{
			bool allowStrafing;

			if (m_Unit.IsRaised())
			{
				allowStrafing = true;
			}
			else if (target)
			{
				if (target.m_ThreatLevelActive <= 0.1)
				{
					if (m_PathFinding.m_PointIdx > 0 || group.GetWaypointBehaviour() == eAIWaypointBehavior.ROAMING)
						allowStrafing = true;
				}
				else if (target.GetRemainingTime() > 3.0)
				{
					if (target.IsNoise())
					{
						if (target.m_ThreatLevelActive > 0.2)
							allowStrafing = true;
					}
					else
					{
						allowStrafing = true;
					}
				}
			}
			else if (m_PathFinding.m_PointIdx > 0 || group.GetWaypointBehaviour() == eAIWaypointBehavior.ROAMING)
			{
				allowStrafing = true;
			}

			if (allowStrafing)
			{
				vector aimDir;

				if (m_Unit.IsRaised())
				{
					aimDir = m_Unit.GetAimDirection();
				}
				else if (!target || target.m_ThreatLevelActive <= 0.1 || target.IsItem())
				{
					aimDir = m_Unit.GetLookDirection();
				}
				else
				{
					aimDir = target.GetDirection().Normalized();
				}

				aimDir[1] = 0;
				float aimAngle = aimDir.VectorToAngles()[0];
				//float aimToMovementAngle = ExpansionMath.AngleDiff2(aimAngle, m_TurnTarget);
				//float aimToMovementAngleAbs = Math.AbsFloat(aimToMovementAngle);

				//if (aimToMovementAngleAbs >= 90)
				if (vector.Dot(aimDir, m_PathDir2DNormalized) < 0.0)
				{
					if (m_Unit.IsRaised() || m_StrafeTime < MAX_NONRAISED_STRAFE_TIME)
					{
						m_TargetMovementDirection = ExpansionMath.AngleDiff2(m_Turn, m_TurnTarget);
						m_ForceMovementDirection = true;
						m_TurnTarget = aimAngle;
						m_StrafeTime += pDt;
					}
				}
				else if (m_StrafeTime >= STRAFE_TIME_RESET_THRESHOLD)
				{
					m_StrafeTime = 0;
				}
			}
		}

		//! Clockwise: Positive, counter-clockwise: Negative
		m_TurnDifference = ExpansionMath.AngleDiff2(m_Turn, m_TurnTarget);

		bool matchLeaderSpeed;

		if (m_Unit.GetFSM().IsInState("FollowFormation") && !m_PathFinding.m_IsUnreachable)
		{
			if (leader && !leader.eAI_IsSideSteppingObstacles() && Math.AbsFloat(leader.Expansion_GetMovementAngle()) <= 90.0)
			{
				float leaderSpeed = leader.Expansion_GetActualVelocity().LengthSq();
				//if (EXTrace.AI && leaderSpeed > 0.0)
					//EXPrint(leader.ToString() + " velocity " + leaderSpeed);
				if (leaderSpeed > 1.7)
					matchLeaderSpeed = true;
			}
		}
		else if (m_Unit == leader && group.Count() > 1)
		{
			group.GetFormation().Update(pDt);
		}

		bool hasReachedWaypoint = HasReachedWaypoint(isPathPointFinal);

		if (m_MovementSpeed > 0.0 && m_Waypoint != position && m_Unit.m_eAI_PositionTime > 3.0 && !m_Unit.IsClimbing() && !m_Unit.IsFalling())
		{
			if (m_BlockingObject && !m_BlockingObject.IsPlainObject() && !m_BlockingObject.IsRock())
			{
				if (m_BlockingObject.IsBuilding())
				{
					if (m_PathFinding.m_IsJumpClimb)
					{
						if (m_PathFinding.m_AllowJumpClimb)
							m_PathFinding.SetAllowJumpClimb(false, 15.0);
					}
					else if (!m_PathFinding.m_IsUnreachable)
					{
					#ifdef DIAG_DEVELOPER
						EXTrace.Print(EXTrace.AI, this, m_Unit.ToString() + " unreachable (blocked by building for more than 3 s)");
						ExpansionStatic.MessageNearPlayers(m_Unit.GetPosition(), 100.0, m_Unit.ToString() + " unreachable (blocked by building for more than 3 s)");
					#endif
						m_PathFinding.m_IsUnreachable = true;
						m_PathFinding.m_IsTargetUnreachable = true;
					}
				}
			}
			else if (m_PathFinding.m_PathGlueIdx == -1)  //! Don't use inverse path for teleport
			{
				//surfacePosition = ExpansionStatic.GetSurfaceRoadPosition(waypoint, RoadSurfaceDetection.CLOSEST);
				//if (waypoint[1] < surfacePosition[1])
				//	waypoint = surfacePosition;

				float distSq = vector.DistanceSq(position, waypoint);
				if (distSq < 16.0 && Math.AbsFloat(waypoint[1] - position[1]) < 3.0)
				{
					m_Unit.SetPosition(waypoint);  //! OH GOD THE HACKS
					hasReachedWaypoint = true;
					m_Unit.m_eAI_PositionTime = 0.0;

				#ifdef DIAG_DEVELOPER
					msg = "Teleported from " + position + " to " + waypoint + " (" + Math.Sqrt(distSq) + " m)";
					EXTrace.Print(EXTrace.AI, m_Unit, msg);
					ExpansionStatic.MessageNearPlayers(m_Unit.GetPosition(), 100, m_Unit.ToString() + " " + msg);
				#endif
				}
			}
		}

		//! https://feedback.bistudio.com/T173348
		if (isTargetPositionFinal && hasReachedWaypoint && !matchLeaderSpeed)
			m_Unit.m_eAI_PositionIsFinal = true;
		else
			m_Unit.m_eAI_PositionIsFinal = false;

		if (!m_Unit.IsClimbing() && !m_Unit.IsFalling() && m_Waypoint != position && hasReachedWaypoint)
		{
			m_UpdatePath = true;
		}

		if (m_UpdatePath)
		{
			if (isPathPointFinal)
			{
				if (matchLeaderSpeed || vector.DistanceSq(m_Waypoint, m_PathFinding.GetTarget()) >= m_MinFinal)
				{
				//#ifdef DIAG_DEVELOPER
					//ExpansionStatic.MessageNearPlayers(m_Unit.GetPosition(), 100.0, m_Unit.ToString() + " final point - recalc path");
				//#endif

					if (m_Unit.eAI_CanEnableSwimming())
						m_PathFinding.EnableSwimming(true);
					else
						m_PathFinding.ForceRecalculate(true);
				}
			#ifdef DIAG_DEVELOPER
				else
				{
					m_Unit.Expansion_DebugObject_Deferred(3141, m_PathFinding.GetTarget() - "0 4.5 0", "ExpansionDebugNoticeMe_Black", m_Direction);
				}
			#endif
			//#ifdef DIAG_DEVELOPER
				//m_Unit.Expansion_DeleteDebugObject(11111 + m_PathFinding.m_PointIdx);
			//#endif
			}
			else
			{
				m_PathFinding.UpdateNext(true);
			}

			m_UpdatePath = false;
		}

		/**
		 * If AI has a ladder it tries to reach, is not on ladder, has a building with ladder and current raw (not navmesh sampled)
		 * target position is ladder entrypoint, we check if the ladder is reachable, and discard it if not
		 */
		if (m_Unit.m_eAI_Ladder && !m_Unit.m_eAI_IsOnLadder && m_Unit.m_eAI_BuildingWithLadder && m_PathFinding.m_CurrentTargetPosition == m_Unit.m_eAI_LadderEntryPoint)
		{
			vector end = m_PathFinding.GetEnd();

			/**
			 * This comically complex check deals with discarding unreachable ladders if any of the following conditions is true:
			 * 
			 * - Navmesh sampled target position is not close enough to ladder entrypoint
			 * OR
			 * - Path endpoint is reasonably close to navmesh sampled target (so we know we are at the end),
			 *   but path endpoint is not close enough to ladder entrypoint
			 * OR
			 * - AI has no more path points on way to target position and is close to path endpoint, but...
			 *   ...path endpoint is too far from navmesh sampled target
			 *   OR
			 *   ...AI is too far from ladder entrypoint
			 *   OR
			 *   ...AI cannot reach ladder entrypoint (e.g. physically blocked)
			 */

			bool isLadderEntryPointNearTarget;
			if (Math.IsPointInCircle(m_Unit.m_eAI_LadderEntryPoint, UAMaxDistances.LADDERS, m_PathFinding.GetTarget()))
				isLadderEntryPointNearTarget = true;

			bool isEndNearTarget = vector.DistanceSq(end, m_PathFinding.GetTarget()) < 4.0;
			bool isLadderEntryPointNearEnd = Math.IsPointInCircle(m_Unit.m_eAI_LadderEntryPoint, UAMaxDistances.LADDERS, end);

			bool isLastSegment = m_PathFinding.GetRemainingCount() <= 2;
			bool isUnitNearEnd = Math.IsPointInCircle(end, 0.55, position);

			if (!isLadderEntryPointNearTarget || (isEndNearTarget && !isLadderEntryPointNearEnd) || (isLastSegment && (isUnitNearEnd || m_PathFinding.m_IsUnreachable) && (!isEndNearTarget || !m_Unit.eAI_IsCloseToLadderEntryPoint() || !m_Unit.eAI_CanReachLadderEntryPoint())))
			{
				//! Remove unreachable ladder from pool
				if (m_Unit.m_eAI_BuildingWithLadder.Expansion_GetLaddersCount() > 1)
					m_Unit.m_eAI_Ladders[m_Unit.m_eAI_BuildingWithLadder].Remove(m_Unit.m_eAI_Ladder.m_Index);

				m_Unit.m_eAI_Ladder = null;

				if (target)
				{
					EntityAI targetEntity = target.GetEntity();
					if (targetEntity && !targetEntity.IsMan())
					{
						if (!m_Unit.eAI_GetThreatOverride(targetEntity) && vector.DistanceSq(end, targetEntity.GetPosition()) > 4.0)
						{
							m_Unit.eAI_ThreatOverride(targetEntity, true);
							m_Unit.m_eAI_PreferLadder = false;
							m_PathFinding.m_IsUnreachable = false;
							m_PathFinding.m_IsTargetUnreachable = false;
						}
					}
				}
			}
		}

	//#ifdef DIAG_DEVELOPER
		//Object dbgObj;
		//vector dbgOri;
	//#endif

		if (m_Unit.eAI_IsSideSteppingVehicle())
		{
			//SetTargetSpeed(Math.Lerp(m_MovementSpeed, 3.0, pDt * 4.0));
			m_TargetSpeed = 3.0;
		}
		else if (m_Unit.m_eAI_PositionIsFinal || m_PathFinding.m_IsUnreachable || m_Unit.m_eAI_IsAttachedToMovingParent || m_Unit.GetFSM().IsInState("Idle"))
		{
		//#ifdef DIAG_DEVELOPER
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
		//#ifdef DIAG_DEVELOPER
			//dbgObj = m_Unit.m_Expansion_DebugObjects[11107];
			//if (dbgObj)
				//dbgOri = dbgObj.GetOrientation();
			//else
				//dbgOri = m_Unit.GetOrientation();
			//dbgOri[0] = dbgOri[0] + 1;
			//m_Unit.Expansion_DebugObject_Deferred(11107, m_Unit.GetPosition(), "ExpansionDebugNoticeMe_Black", dbgOri.AnglesToVector());
		//#endif
			if (ExpansionMath.Distance2DSq(position, m_PathFinding.GetEnd()) >= m_MinFinal)
				m_TargetSpeed = Math.Min(1.0, speedLimit);
				//SetTargetSpeed(Math.Lerp(m_MovementSpeed, Math.Min(1.0, speedLimit), pDt * 2.0));
		}
		else if (m_Unit.IsRaised())
		{
		//#ifdef DIAG_DEVELOPER
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
		//#ifdef DIAG_DEVELOPER
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
			if (isTargetPositionFinal && m_WaypointDistance2DSq < 2.0 * distanceFactor)
			{
				if (matchLeaderSpeed)
					targetSpeed = leader.Expansion_GetMovementSpeed();
				else
					targetSpeed = 1.0;
			}
			else if (isTargetPositionFinal && m_WaypointDistance2DSq < 5.0 * distanceFactor)
			{
				if (matchLeaderSpeed)
					targetSpeed = leader.Expansion_GetMovementSpeed();
				else
					targetSpeed = 2.0;
			}
			else
			{
				targetSpeed = 3.0;
			}

			if (m_MovementSpeed > speedLimit && speedLimit >= 0)
				targetSpeed = speedLimit;

			if (targetSpeed > 1 && targetSpeed > m_MovementSpeed)
				SetTargetSpeed(Math.Lerp(m_MovementSpeed, targetSpeed, pDt * 2.0));
			else
				m_TargetSpeed = targetSpeed;
		}
		else
		{
			if (m_MovementSpeed > speedLimit && speedLimit >= 0)
				m_TargetSpeed = speedLimit;

			if (m_TargetSpeed > 1 && m_TargetSpeed > m_MovementSpeed)
				SetTargetSpeed(Math.Lerp(m_MovementSpeed, m_TargetSpeed, pDt * 2.0));
		}

		DBGDrawSphere(waypoint, 0.05, 0xFF00FF00);
		//DBGDrawLine(position, waypoint, 0xFF00FF00);
		//m_Unit.Expansion_DebugObject_Deferred(CORRECTED_WAYPOINT, waypoint, "ExpansionDebugBox");

		if (m_Unit.m_eAI_IsOnLadder)
		{
			m_WasOnLadder = true;

			if (m_Unit.m_eAI_LadderClimbDirection < 0)
			{
				m_MovementDirection = -180.0;

				EntityAI gloves = m_Unit.GetInventory().FindAttachment(InventorySlots.GLOVES);
				if (gloves && !gloves.IsDamageDestroyed())
					m_TargetSpeed = 3.0;
				else
					m_TargetSpeed = 2.0;
			}
			else
			{
				m_MovementDirection = 0;
				m_TargetSpeed = 3.0;
			}
		}
		else if (m_WasOnLadder)
		{
			m_WasOnLadder = false;
			m_MovementDirection = 0;
		}
		else
		{
			if (Math.AbsFloat(m_MovementDirection) == 180 && m_TargetMovementDirection == 0)
			{
				m_MovementDirection = 0;
			}
			else if (m_MovementDirection == 0 && Math.AbsFloat(m_TargetMovementDirection) == 180)
			{
				m_MovementDirection = m_TargetMovementDirection;
			}
			else
			{
				m_MovementDirection += ExpansionMath.AngleDiff2(m_MovementDirection, m_TargetMovementDirection);
				m_MovementDirection = Math.Clamp(m_MovementDirection, -180.0, 180.0);
			}
		}

	#ifdef DIAG_DEVELOPER
		if (m_MovementSpeed > 0.0)
		{
			vector moveDir = Vector(m_MovementDirection, 0.0, 0.0).AnglesToVector().Multiply3(m_Transform);
			m_Unit.Expansion_DebugObject(2024, position + "0 0.1 0" + moveDir * 0.4, "ExpansionDebugArrow_Black", moveDir);
			m_Unit.Expansion_DebugObject(2025, position + "0 0.3 0" + m_Direction * 0.4, "ExpansionDebugArrow_White", m_Direction);
		}
		else
		{
			m_Unit.Expansion_DebugObject(2024, "0 0 0", "ExpansionDebugArrow_Black");
			m_Unit.Expansion_DebugObject(2025, "0 0 0", "ExpansionDebugArrow_White");
		}
	#endif

		if (m_Stance != -1 && m_Stance != m_Unit.eAI_GetStance() && m_StanceChangeTimeout <= 0.0 && !isBusy && !m_Unit.IsSwimming() && !m_Unit.GetEmoteManager().IsEmotePlaying() && !m_Unit.GetActionManager().GetRunningAction() && !m_Unit.m_eAI_IsOnLadder)
		{
			//! Can't go from erect to prone or prone to erect directly, need to crouch first
			//! else it breaks character to surface alignment and hitbox
			if (m_Stance != m_Unit.eAI_GetStance())
			{
				switch (m_Unit.eAI_GetStance())
				{
					case DayZPlayerConstants.STANCEIDX_ERECT:
					case DayZPlayerConstants.STANCEIDX_PRONE:
						if (m_Stance != DayZPlayerConstants.STANCEIDX_CROUCH)
							m_Stance = DayZPlayerConstants.STANCEIDX_CROUCH;
						break;
				}

				if (m_Stance == DayZPlayerConstants.STANCEIDX_PRONE || m_Unit.eAI_GetStance() == DayZPlayerConstants.STANCEIDX_PRONE)
					m_StanceChangeTimeout = 0.75;  //! Crouch to prone and prone to crouch
				else
					m_StanceChangeTimeout = 0.3;  //! Erect to crouch and crouch to erect
			}
			else
			{
				m_StanceChangeTimeout = 0.75;  //! Safety
			}

		#ifdef EXPANSION_AI_STANCEDBG_CHATTY
			ExpansionStatic.MessageNearPlayers(m_Unit.GetPosition(), 100.0, m_Unit.ToString() + " set stance " + m_Unit.eAI_GetStance() + " -> " + m_Stance);
		#endif

			if (move)
				move.ForceStance(m_Stance);

			SetCurrentStance(m_Stance);
		}

		if (!IsChangingStance() && !isBusy && !m_Unit.IsSwimming() && !m_Unit.GetEmoteManager().IsEmotePlaying() && !m_Unit.m_eAI_IsOnLadder && cdt >= 0.12)
		{
			//! Head position standing 1.6m, crouched 1m, prone 0.3m
			//! + 0.2m = full character height (not including gear)
			//! @note make sure that the 2nd raycast (the one checking if there is enough space to stand up)
			//! has a higher endpoint Y than the first one, else we could get stuck in a stance change loop!
			vector head = m_Unit.GetBonePositionWS(m_Unit.GetBoneIndexByName("head"));
			vector hitPosition;
			vector hitNormal;
			int contactComponent;
			set<Object> results = new set<Object>;
			if (DayZPhysics.RaycastRV(head, head + "0 0.2 0", hitPosition, hitNormal, contactComponent, results, null, m_Unit, false, false, ObjIntersectView) && results.Count() && ObjectCanLimitStance(results[0]))
			{
				if (m_Unit.m_eAI_StancePreference == -1)
				{
					m_Unit.m_eAI_StancePreference = DayZPlayerConstants.STANCEIDX_ERECT;

				#ifdef DIAG_DEVELOPER
					ExpansionStatic.MessageNearPlayers(m_Unit.GetPosition(), 100.0, m_Unit.ToString() + " stance pref " + m_Unit.m_eAI_StancePreference);
				#endif
				}

				if (m_Stance == DayZPlayerConstants.STANCEIDX_ERECT || m_Stance == -1)
				{
					m_Stance = DayZPlayerConstants.STANCEIDX_CROUCH;
				}
				else if (m_Stance == DayZPlayerConstants.STANCEIDX_CROUCH)
				{
					m_Stance = DayZPlayerConstants.STANCEIDX_PRONE;
				}

			#ifdef DIAG_DEVELOPER
				if (m_Stance != m_Unit.eAI_GetStance() && m_StanceChangeTimeout <= 0.0)
					ExpansionStatic.MessageNearPlayers(m_Unit.GetPosition(), 100.0, m_Unit.ToString() + " adjust stance " + m_Stance);

				m_Unit.Expansion_DebugObject_Deferred(1122, hitPosition, "ExpansionDebugSphereSmall_Red", vector.Zero, hitPosition - hitNormal);
			#endif
			}
			else if (m_Unit.m_eAI_StancePreference != -1 && m_Stance != m_Unit.m_eAI_StancePreference && !DayZPhysics.RaycastRV(position + "0 0.3 0", position + Vector(0, 0.5 + (2.0 - m_Unit.m_eAI_StancePreference) * 0.75, 0), hitPosition, hitNormal, contactComponent, results, null, m_Unit, false, false, ObjIntersectView, 0.1))
			{
				m_Stance = m_Unit.m_eAI_StancePreference;

			#ifdef DIAG_DEVELOPER
				ExpansionStatic.MessageNearPlayers(m_Unit.GetPosition(), 100.0, m_Unit.ToString() + " restoring stance pref " + m_Unit.m_eAI_StancePreference);
			#endif
			}
		}

		if (speedLimit == 0)
			m_MovementSpeed = 0.0;
		else
			m_MovementSpeed = m_TargetSpeed;

		HumanInputController hic = m_Unit.GetInputController();

		hic.OverrideMovementAngle(HumanInputControllerOverrideType.ONE_FRAME, m_MovementDirection);

		float movementSpeed;

		if (m_Unit.IsSwimming() && m_MovementSpeed > 0)
			movementSpeed = Math.Round(ExpansionMath.LinearConversion(0.0, 2.99, m_MovementSpeed, 1.99, 3.0));
		else
			movementSpeed = m_MovementSpeed;

		hic.OverrideMovementSpeed(HumanInputControllerOverrideType.ONE_FRAME, movementSpeed);

		if (movementSpeed > 2)
			SetCurrentMovement(DayZPlayerConstants.MOVEMENT_SPRINT);
		else if (movementSpeed > 1)
			SetCurrentMovement(DayZPlayerConstants.MOVEMENT_RUN);
		else if (movementSpeed > 0)
			SetCurrentMovement(DayZPlayerConstants.MOVEMENT_WALK);
		else
			SetCurrentMovement(DayZPlayerConstants.MOVEMENT_IDLE);

		if (m_Unit.m_eAI_IsOnLadder)
			return;

		//m_TurnVelocity = ExpansionMath.AngleDiff2(m_Turn, m_TurnPrevious);
		//m_TurnPrevious = m_Turn;

		bool tacticalLean;
		if (Math.AbsFloat(m_Unit.m_eAI_LeanTarget) > 0 && !m_Unit.IsFighting())
			tacticalLean = true;

		if ((tacticalLean || Math.AbsFloat(m_Unit.m_eAI_Lean) > 0.0 || m_Unit.m_eAI_MemeLevel) && !m_Unit.IsClimbing() && !m_Unit.IsFalling() && !m_Unit.IsSwimming())
		{
			bool meme;

			if (!tacticalLean && m_Unit.m_eAI_MemeLevel && m_Unit.GetThreatToSelf() > 0.15 && m_Unit.GetThreatToSelf() < 0.4)
			{
				//! If a friendly player leans, we lean
				if (targetPlayer && (m_Unit.m_eAI_Meme || Math.AbsFloat(targetPlayer.m_MovementState.m_fLeaning) > 0.5))
				{
					eAIFaction faction = group.GetFaction();
					if (faction.IsObserver())
					{
						meme = true;
					}
					else
					{
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
				case 0:
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

			if ((!m_Unit.m_eAI_HasProjectileWeaponInHands || ((!tacticalLean || !m_Unit.IsRaised()) && !meme)) && Math.AbsFloat(m_Unit.m_eAI_Lean) > 0.0)
			{
				//! Always return to neutral state
				returnToNeutral = true;
				EXTrace.Print(EXTrace.AI, m_Unit, "Target lean neutral");
			}

			if (tacticalLean && !returnToNeutral)
			{
				m_LeanDirection = m_Unit.m_eAI_LeanTarget;
				EXTrace.Print(EXTrace.AI, m_Unit, "Target lean tactical " + m_LeanDirection);
			}
			else if (meme || returnToNeutral)
			{
				float leanReverseThresh = Math.RandomFloat(0.5, 2.0);
				if (Math.AbsFloat(m_Unit.m_eAI_Lean) > leanReverseThresh)
				{
					if (m_Unit.m_eAI_Lean > 0.0)
						m_Unit.m_eAI_Lean = leanReverseThresh;
					else
						m_Unit.m_eAI_Lean = -leanReverseThresh;

					m_LeanDirection = -m_LeanDirection;
					EXTrace.Print(EXTrace.AI, m_Unit, "Target lean " + m_LeanDirection);

					if (m_Unit.m_eAI_Meme)
						m_Unit.m_eAI_Meme--;
				}
			}

			if (tacticalLean || meme || returnToNeutral)
			{
				float prevLean = m_Unit.m_eAI_Lean;

				m_Unit.m_eAI_Lean = m_Unit.m_eAI_Lean + pDt * 4.0 * m_LeanDirection;

				if (returnToNeutral && ((m_Unit.m_eAI_Lean < 0.0 && prevLean > 0.0) || (m_Unit.m_eAI_Lean > 0.0 && prevLean < 0.0)))
				{
					m_Unit.m_eAI_Lean = 0.0;
					m_Unit.m_eAI_LeanTarget = 0.0;
				}

				float lean = ExpansionMath.SmoothStep(Math.Clamp(m_Unit.m_eAI_Lean, -1.0, 1.0), 1, -1.0, 1.0);
				m_Table.SetLean(m_Unit, lean);
				SetCurrentLeaning(lean);
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

						m_Table.CallTurn(m_Unit);
						m_Table.SetTurnAmount(m_Unit, m_TurnDifference / 90.0);

						m_TurnState = TURN_STATE_TURNING;
					}
					break;
				case TURN_STATE_TURNING:
					m_TurnTime += pDt;

					if (m_TurnTime > 2.0 || Math.AbsFloat(m_TurnDifference) < 1)
					{
						m_Table.CallStopTurn(m_Unit);

						m_TurnState = TURN_STATE_NONE;
					}

					break;
			}

		}
		else
		{
			if (m_TurnState == TURN_STATE_TURNING)
				m_Table.CallStopTurn(m_Unit);

			m_TurnState = TURN_STATE_NONE;

			float turnTargetActual = m_TurnTarget;

			//auto parent = Object.Cast(m_Player.Expansion_GetParent());
			//if (parent)
				//turnTargetActual -= parent.GetOrientation()[0];

			if (turnTargetActual > 180.0)
				turnTargetActual -= 360.0;

			bool isAboveGround;
			if (position[1] - m_Unit.m_eAI_SurfaceY > 0.5)
				isAboveGround = true;

			if (m_Unit.IsSwimming())
				Anim_SetFilteredHeading(-turnTargetActual * Math.DEG2RAD, 0.3, 30.0);
			//! Enable sharpest turns if turning around while running or sprinting or dangerous altitude
			else if (isDangerousAltitude || (!m_Unit.IsRaised() && Math.AbsFloat(m_TurnDifference) > 135.0 && m_MovementSpeed >= 2.0))
				Anim_SetFilteredHeading(-turnTargetActual * Math.DEG2RAD, 0.15, 30.0);
			//! Enable sharper turns if above ground or waypoint is not final but close and not avoiding obstacles
			else if (!m_Unit.IsRaised() && (isAboveGround || (!isPathPointFinal && m_WaypointDistance2DSq < 8.0 * Math.Max(m_MovementSpeed, 1.0) && m_OverrideMovementTimeout <= 0)))
				Anim_SetFilteredHeading(-turnTargetActual * Math.DEG2RAD, 0.2, 30.0);
			else
				Anim_SetFilteredHeading(-turnTargetActual * Math.DEG2RAD, 0.3, 30.0);
		}

		if (m_WeaponFire && !m_IsTagWeaponFire)
		{
			m_Table.CallWeaponFire(m_Unit);
			m_WeaponFire = false;
		}
	}

	void Anim_SetFilteredHeading(float pYawAngle, float pFilterDt, float pMaxYawSpeed)
	{
		float angle = m_Unit.GetOrientation()[0];
		//m_Unit.SetOrientation(Vector(angle + m_TurnDifference * m_DT / pFilterDt, 0, 0));
		m_Unit.SetOrientation(Vector(Math.SmoothCD(angle, angle + m_TurnDifference, m_SmoothVel, pFilterDt, 1000, m_DT), 0, 0));
		//if (Math.AbsFloat(m_TurnDifference) < 0.01)
			//m_SmoothVel[0] = 0.0;
	}

	/**
	 * @brief Returns total water depth at position
	 * 
	 * @note returns zero value when on land
	 */
	float GetWaterDepth(vector position)
	{
		vector  wl = HumanCommandSwim.WaterLevelCheck(m_Unit, position);

		return wl[0];
	}

	/**
	 * @brief Returns how deep character is in/under water at position
	 * 
	 * @note returns negative value when on land
	 */
	float GetCharacterDepth(vector position)
	{
		vector  wl = HumanCommandSwim.WaterLevelCheck(m_Unit, position);

		return wl[1];
	}

	private bool Raycast(vector start, vector end, out vector hitPosition, out vector hitNormal, out float hitFraction, vector endRV = vector.Zero, float radiusRV = 0.25, bool includeAI = false, out Object blockingObject = null, bool updatePathIfTreeClose = false)
	{
	#ifdef EXTRACE_DIAG
		EXTrace trace;
	#endif

		if (endRV == vector.Zero)
			endRV = end;

		vector dir = vector.Direction(start, end).Normalized();
		vector begPos = start + dir * radiusRV * 0.5 + CHECK_MIN_HEIGHT;
		vector endPos = endRV + CHECK_MIN_HEIGHT;
		bool hit;

		//! 1st raycast specifically for trees
		int contactComponent;
		set<Object> results();
		//m_Unit.Expansion_DebugObject(2099, endPos, "ExpansionDebugSphereSmall_Purple", dir, begPos);
		if (DayZPhysics.RaycastRV(begPos, endPos, hitPosition, hitNormal, contactComponent, results, null, m_Unit, false, false, ObjIntersectGeom, radiusRV))
		{
			float distSq;
			float farDistSqThresh;

			if (updatePathIfTreeClose)
			{
				distSq = vector.DistanceSq(start, hitPosition);
				float farDistThresh = m_MovementSpeed + radiusRV;
				farDistSqThresh = farDistThresh * farDistThresh;
			}

			foreach (Object obj: results)
			{
				if (obj.IsTree())
				{
					hit = true;

					if (obj != m_PrevBlockingObject)
					{
						if (updatePathIfTreeClose && vector.DistanceSq(m_Waypoint, hitPosition) < 16.0)
						{
							m_UpdatePath = true;
							m_PathFinding.m_SuppressRecalculate = false;
						}

						if (distSq > farDistSqThresh)
						{
							if (!m_UpdatePath && obj != m_Unit.m_eAI_CurrentCoverObject && m_PathFinding.m_Count > 1 + m_PathFinding.m_PointIdx)
							{
								//! In case we move towards a tree, move path point to left or right side of tree

								blockingObject = obj;

								hitPosition[1] = hitPosition[1] - CHECK_MIN_HEIGHT[1];
								vector position = obj.GetPosition();
								position[1] = hitPosition[1];
								position = start + vector.Direction(start, position).Normalized() * Math.Sqrt(distSq);

								vector pathDir = m_PathFinding.m_PathSegmentDirection.Normalized();
								vector side = pathDir.Perpend();

								vector waypoint;

								if (ExpansionMath.Side(start, position, hitPosition) > 0)
									waypoint = hitPosition + pathDir * 0.5 + side * 1.25;  //! left
								else
									waypoint = hitPosition + pathDir * 0.5 - side * 1.25;  //! right
							
								if (m_PathFinding.m_AIWorld.SampleNavmeshPosition(waypoint, 0.5, m_PathFinding.m_AllFilter, waypoint))
								{
									m_PathFinding.m_Points.InsertAt(waypoint, 1 + m_PathFinding.m_PointIdx);
									++m_PathFinding.m_Count;
									m_PathFinding.m_Next0.Position = waypoint;

									if (m_Unit.GetFSM().IsInState("FollowFormation"))
										m_PathFinding.m_SuppressRecalculate = true;
								}

							#ifdef DIAG_DEVELOPER
								m_Unit.Expansion_DebugObject(99999, position, "ExpansionDebugCone_Red");
								m_Unit.Expansion_DebugObject(999991, waypoint, "ExpansionDebugCone");
							#endif
							}

							return false;
						}

					#ifdef DIAG_DEVELOPER
						m_Unit.Expansion_DebugObject(2100, begPos - dir * radiusRV, "ExpansionDebugSphereSmall_Cyan", dir);
						m_Unit.Expansion_DebugObject(2101, begPos + dir.Perpend() * radiusRV, "ExpansionDebugSphereSmall_Cyan", dir);
						m_Unit.Expansion_DebugObject(2102, begPos - dir.Perpend() * radiusRV, "ExpansionDebugSphereSmall_Cyan", dir);
						m_Unit.Expansion_DebugObject(2103, endPos + dir * radiusRV, "ExpansionDebugSphereSmall_Yellow", dir, begPos - dir * radiusRV);
						m_Unit.Expansion_DebugObject(2104, endPos + dir.Perpend() * radiusRV, "ExpansionDebugSphereSmall_Yellow", dir, begPos + dir.Perpend() * radiusRV);
						m_Unit.Expansion_DebugObject(2105, endPos - dir.Perpend() * radiusRV, "ExpansionDebugSphereSmall_Yellow", dir, begPos - dir.Perpend() * radiusRV);
						m_Unit.Expansion_DebugObject(2106, hitPosition, "ExpansionDebugSphereSmall_Red", dir, begPos - dir * radiusRV);
						m_Unit.Expansion_DebugObject(2107, hitPosition + hitNormal, "ExpansionDebugSphereSmall_Purple", hitNormal, hitPosition);
					#endif

						//ExpansionStatic.MessageNearPlayers(m_Unit.GetPosition(), 100.0, m_Unit.ToString() + " mov block " + obj);
					}
					else if (distSq > farDistSqThresh)
					{
						return false;
					}

					blockingObject = obj;
				}
				else if (obj.IsBuilding())
				{
					blockingObject = obj;

					if (distSq > farDistSqThresh)
						return false;
				}
				else if (distSq > farDistSqThresh)
				{
					return false;
				}
				else if (obj.IsDayZCreature())
				{
					if (obj.IsDamageDestroyed() || m_Unit.IsRaised() || m_Unit.eAI_GetTargetEntity() == obj)
						continue;

					hit = true;

					blockingObject = obj;
				}
				else if (obj.IsPlainObject())
				{
					string debugName = obj.GetDebugName();
					debugName.ToLower();

					if (ExpansionString.ContainsAny(debugName, s_NonBlocking))
						return false;
				}
			}
		}

		//! Everything else
		if (!hit)
		{
			IEntity hitEntity;
			Object hitObject;
		//#ifdef EXTRACE_DIAG
			//trace = EXTrace.Profile(EXTrace.AI_PROFILE, this, "Raycast CollisionMoveTest");
		//#endif
			//! @note CollisionMoveTest is around 1.5x slower than SphereCastBullet (below), it deals better with more corner cases (ymmv)
			//! although has a few issues of its own (steep stairs tend to be detected as colliding even though AI could run up no problem).
			//! Still a bit undecided on this one, for now it's the way to go
			hitFraction = m_Unit.CollisionMoveTest(dir * 0.8, vector.Zero, 1.0, null, hitEntity, hitPosition, hitNormal);
		//#ifdef EXTRACE_DIAG
			//delete trace;
		//#endif
			if (hitFraction < 0.5)
			{
				//ExpansionStatic.MessageNearPlayers(m_Unit.GetPosition(), 100.0, m_Unit.ToString() + " IEnt " + hitEntity + " frac " + hitFraction);
				hitObject = Object.Cast(hitEntity);

				if (!m_PathFinding.m_IsJumpClimb || !m_PathFinding.m_AllowJumpClimb || m_PathFinding.m_IsBlockedPhysically)
				{
				//#ifdef DIAG_DEVELOPER
					//if (hitObject != m_PrevBlockingObject)
						//ExpansionStatic.MessageNearPlayers(m_Unit.GetPosition(), 100.0, m_Unit.ToString() + " mov block " + hitObject + " - recalc path");
				//#endif

					if (IsBlockedUnreachable(hitObject))
					{
					#ifdef DIAG_DEVELOPER
						if (!m_PathFinding.m_IsUnreachable)
							EXTrace.Print(EXTrace.AI, this, m_Unit.ToString() + " unreachable (blocked by buggy object)");
					#endif
						m_PathFinding.m_IsTargetUnreachable = true;
						m_PathFinding.m_IsUnreachable = true;
					}
					else if ((!m_Unit.IsSwimming() && m_Unit.m_eAI_BlockedTime > 0.1) || m_Unit.m_eAI_BlockedTime > 0.8)
					{
						//! @note the blocked time treshold of 0.8 has been very carefully fine-tuned to avoid recalculation
						//! at critical points while swimming.
						//! To test this on Chernarus, spawn AI at <14297.8, -0.36239, 13245.3> (in water) and set a waypoint
						//! at <14297.7, 3.31738, 13250.3> (on pier)
						m_PathFinding.ForceRecalculate();
					}
				}

				if (hitObject)
				{
					if (hitObject.IsMan())
					{
						if (hitObject.IsDamageDestroyed() || m_Unit.IsRaised() || m_Unit.eAI_GetTargetEntity() == hitObject)
							return false;

						blockingObject = hitObject;
					}
					else if (!hitObject.IsBush())
					{
						blockingObject = hitObject;
					}

					return true;
				}
			}

			return false;

			PhxInteractionLayers hit_mask = PhxInteractionLayers.BUILDING | PhxInteractionLayers.DOOR | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.ITEM_LARGE | PhxInteractionLayers.FENCE;
			if (includeAI)
				hit_mask |= PhxInteractionLayers.CHARACTER | PhxInteractionLayers.AI;
		#ifdef EXTRACE_DIAG
			trace = EXTrace.Profile(EXTrace.AI_PROFILE, this, "Raycast SphereCastBullet");
		#endif
			hit = DayZPhysics.SphereCastBullet(start + dir * 0.125 + CHECK_MIN_HEIGHT_BULLET, end + CHECK_MIN_HEIGHT_BULLET, 0.25, hit_mask, m_Unit, hitObject, hitPosition, hitNormal, hitFraction);
		#ifdef EXTRACE_DIAG
			delete trace;
		#endif
			hitFraction = 1.0 - hitFraction;
			if (hit && (!m_PathFinding.m_IsJumpClimb || !m_PathFinding.m_AllowJumpClimb || m_PathFinding.m_IsBlockedPhysically))
			{
			//#ifdef DIAG_DEVELOPER
				//if (hitObject != m_PrevBlockingObject)
					//ExpansionStatic.MessageNearPlayers(m_Unit.GetPosition(), 100.0, m_Unit.ToString() + " mov block " + hitObject + " - recalc path");
			//#endif

				if (IsBlockedUnreachable(hitObject))
				{
				#ifdef DIAG_DEVELOPER
					if (!m_PathFinding.m_IsUnreachable)
						EXTrace.Print(EXTrace.AI, this, m_Unit.ToString() + " unreachable (blocked by buggy object)");
				#endif
					m_PathFinding.m_IsTargetUnreachable = true;
					m_PathFinding.m_IsUnreachable = true;
				}
				else if ((!m_Unit.IsSwimming() && m_Unit.m_eAI_BlockedTime > 0.1) || m_Unit.m_eAI_BlockedTime > 0.8)
				{
					m_PathFinding.ForceRecalculate();
				}
			}
			if (hitObject && !hitObject.IsBush())
				blockingObject = hitObject;
		}

#ifdef DIAG_DEVELOPER
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

	bool IsBlockedUnreachable(Object obj)
	{
		if (obj)
		{
			switch (obj.GetType())
			{
				//! Objects with no navmesh but blocking physically
				//! https://feedback.bistudio.com/T181817
				case "Land_Castle_Wall1_End1_nolc":
				case "Land_Castle_Wall1_End2_nolc":
					return true;
			}
		}

		return false;
	}

	vector GetWaypoint()
	{
		return m_Waypoint;
	}

	//! @note currently not set/unused
	float GetWaypointDistance()
	{
		return m_WaypointDistance;
	}

	float GetWaypointDistance2D()
	{
		return Math.Sqrt(m_WaypointDistance2DSq);
	}

	float GetWaypointDistance2DSq()
	{
		return m_WaypointDistance2DSq;
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

	bool IsBlockedByBuildingWithDoor()
	{
		return m_IsBlockedByBuildingWithDoor;
	}

	void ResetIsBlockedByBuildingWithLadder()
	{
		m_IsBlockedByBuildingWithLadder = false;
	}

	bool IsBlockedByBuildingWithLadder()
	{
		return m_IsBlockedByBuildingWithLadder;
	}

	bool CheckBlocked()
	{
		if (!m_MovementSpeed || m_Unit.IsClimbing() || m_Unit.IsFalling() || m_Unit.IsFighting())
			return false;

		if (!m_LastBlocked || !m_LastBlockedLeft || !m_LastBlockedRight)
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
			isBlocked = this.Raycast(position, checkLeft, hitPosition, hitNormal, hitFraction, checkDir + lr, 0.5);
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
			isBlocked = this.Raycast(position, checkRight, hitPosition, hitNormal, hitFraction, checkDir - lr, 0.5);
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

	bool ObjectCanLimitStance(Object obj)
	{
		if (obj.IsTree())
		{
			if (m_Unit.m_eAI_BlockedTime < 1.0)
				return false;
		}
		else if (obj.IsBush())
		{
			return false;
		}
		else if (!ExpansionStatic.CanObstruct(obj) || obj.GetType() == "Land_Office_Municipal1")
		{
			return false;
		}

		return true;
	}
};
