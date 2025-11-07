/**
 * ExpansionHumanCommandVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionHumanCommandVehicle : ExpansionHumanCommandScript
{
	int STATE_UNKNOWN = -1;
	int STATE_AWAIT = 1;
	int STATE_PRE_GETTING_IN = STATE_AWAIT + 1;
	int STATE_GETTING_IN = STATE_PRE_GETTING_IN + 1;
	int STATE_GETTING_OUT = STATE_GETTING_IN + 1;
	int STATE_JUMPING_OUT = STATE_GETTING_OUT + 1;
	int STATE_JUMPED_OUT = STATE_JUMPING_OUT + 1;
	int STATE_SWITCHING_SEAT = STATE_JUMPED_OUT + 1;
	int STATE_FINISH = STATE_SWITCHING_SEAT + 1;

	float TIME_PRE_GET_IN = 0.1;
	float TIME_GET_IN = 1.73;
	float TIME_GET_OUT = 1.5;
	float TIME_JUMP_OUT = 2.0;
	float TIME_SWITCH_SEAT = 1.0;

	Object m_Object;
#ifdef EXPANSIONMODVEHICLE
	ExpansionVehicleBase m_Vehicle;
#endif
	Transport m_Transport;
	Car m_Car;
	Boat m_Boat;

	int m_SeatIndex;
	int m_SeatAnim;
	int m_VehicleType;
	bool m_FromUnconscious;

	int m_State;
	int m_PreviousState;

	float m_Time;
	float m_TimeMax;

	float m_LinearSpeed;
	float m_AngularSpeed;

	vector m_StartTransform[4];
	vector m_TargetTransform[4];

	bool m_GearChanged;
	bool m_ClutchState;
	bool m_LeftVehicle;

	bool m_KeepInVehicleSpaceAfterLeave;

	void ExpansionHumanCommandVehicle(DayZPlayerImplement player, ExpansionHumanST table, Object vehicle, int seatIdx, int seat_anim, bool fromUnconscious)
	{
		m_Object = vehicle;

#ifdef EXPANSIONMODVEHICLE
		Class.CastTo(m_Vehicle, vehicle);
#endif
		Class.CastTo(m_Transport, vehicle);
		Class.CastTo(m_Car, vehicle);
		Class.CastTo(m_Boat, vehicle);

		m_SeatIndex = seatIdx;
		m_SeatAnim = seat_anim;

		m_FromUnconscious = fromUnconscious;

		m_State = STATE_UNKNOWN;
		m_PreviousState = STATE_UNKNOWN;
	}

	void ChangeState(int state)
	{
		m_PreviousState = m_State;
		m_State = state;

		UpdateSpeed();
	}

	void ChangeState(int state, vector newTransform[4])
	{
		m_PreviousState = m_State;
		m_State = state;
		
		m_StartTransform[0] = m_TargetTransform[0];
		m_StartTransform[1] = m_TargetTransform[1];
		m_StartTransform[2] = m_TargetTransform[2];
		m_StartTransform[3] = m_TargetTransform[3];

		m_TargetTransform[0] = newTransform[0];
		m_TargetTransform[1] = newTransform[1];
		m_TargetTransform[2] = newTransform[2];
		m_TargetTransform[3] = newTransform[3];

		UpdateSpeed();
	}

	void UpdateSpeed()
	{		
		switch (m_State)
		{
		case STATE_PRE_GETTING_IN:
			m_TimeMax = TIME_PRE_GET_IN;
			break;
		case STATE_GETTING_IN:
			m_TimeMax = TIME_GET_IN;
			break;
		case STATE_GETTING_OUT:
			m_TimeMax = TIME_GET_OUT;
			break;
		case STATE_JUMPED_OUT:
		case STATE_JUMPING_OUT:
			m_TimeMax = TIME_JUMP_OUT;
			break;
		case STATE_SWITCHING_SEAT:
			m_TimeMax = TIME_SWITCH_SEAT;
			break;
		default:
			m_TimeMax = 1.0;
			break;
		}
		
		vector deltaTransformForTime[4];
		Math3D.MatrixInvMultiply3(m_StartTransform, m_TargetTransform, deltaTransformForTime);
		
		m_LinearSpeed = (m_TargetTransform[3] - m_StartTransform[3]).Normalize() / m_TimeMax;
		m_AngularSpeed = Math3D.MatrixToAngles(deltaTransformForTime).Normalize() * Math.DEG2RAD / m_TimeMax;
	}

	void LeaveVehicle()
	{
		if (m_LeftVehicle)
			return;

		m_LeftVehicle = true;

		m_Table.SetVehicleType(this, -1);

		if (m_Transport) m_Transport.CrewGetOut(m_SeatIndex);
#ifdef EXPANSIONMODVEHICLE
		else if (m_Vehicle) m_Vehicle.CrewGetOut(m_SeatIndex);
#endif

		dBodyActive(m_Player, ActiveState.ALWAYS_ACTIVE);
		dBodyEnableGravity(m_Player, true);

		if (!m_KeepInVehicleSpaceAfterLeave)
		{
			vector tmPlayer[4];
			vector tmVehicle[4];
			vector tmTarget[4];
			m_Player.GetTransformWS(tmPlayer);
			m_Object.GetTransform(tmVehicle);
			Math3D.MatrixMultiply4(tmVehicle, tmPlayer, tmTarget);

			m_Player.UnlinkFromLocalSpace();
			m_Player.SetTransform(tmTarget);
		}
	}

	override void OnActivate()
	{
		super.OnActivate();

		m_LeftVehicle = false;
		
		if (!m_FromUnconscious && m_Player.GetParent() != m_Object)
		{
			vector tmPlayer[4];
			vector tmTarget[4];
			vector tmLocal[4];
			m_Player.GetTransformWS(tmPlayer);
			m_Object.GetTransform(tmTarget);
			Math3D.MatrixInvMultiply4(tmTarget, tmPlayer, tmLocal);

			m_Player.LinkToLocalSpaceOf(m_Object, tmLocal);
		}
		
		m_Player.GetTransformWS(m_StartTransform);

		vector crewPos;
		vector crewDir;

		if (m_Transport) m_Transport.CrewEntry(m_SeatIndex, crewPos, crewDir);
#ifdef EXPANSIONMODVEHICLE
		else if (m_Vehicle) m_Vehicle.CrewEntry(m_SeatIndex, crewPos, crewDir);
#endif

		Math3D.DirectionAndUpMatrix(crewDir, "0 1 0", m_TargetTransform);
		m_TargetTransform[3] = crewPos;

		if (m_Transport)
		{
			m_Transport.CrewGetIn(m_Player, m_SeatIndex);
		}
#ifdef EXPANSIONMODVEHICLE
		else if (m_Vehicle)
		{
			m_Vehicle.CrewGetIn(m_Player, m_SeatIndex);
		}
#endif

		m_Table.SetVehicleType(this, m_VehicleType);

		dBodyEnableGravity(m_Player, false);

		dBodyActive(m_Player, ActiveState.INACTIVE);

#ifdef EXPANSIONMODVEHICLE
		m_Player.OnCommandExpansionVehicleStart();
#endif
		
		ChangeState(STATE_PRE_GETTING_IN);
	}

	override void OnDeactivate()
	{
		super.OnDeactivate();

		LeaveVehicle();

#ifdef EXPANSIONMODVEHICLE
		m_Player.OnCommandExpansionVehicleFinish();
#endif
	}

#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		return true;
	}
#endif

	override void PreAnimUpdate(float pDt)
	{
		super.PreAnimUpdate(pDt);

		m_Table.SetVehicleType(this, m_VehicleType);

		if (m_SeatIndex == DayZPlayerConstants.VEHICLESEAT_DRIVER)
		{
			if (m_Car)
			{
				m_Table.SetVehicleSteering(this, m_Car.GetSteering());
				m_Table.SetVehicleThrottle(this, m_Car.GetThrottle());
				m_Table.SetVehicleBrake(this, m_Car.GetBrake() != 0.0);
				m_Table.SetVehicleClutch(this, m_ClutchState);
			}
			else if (m_Boat)
			{
				m_Table.SetVehicleSteering(this, m_Boat.GetSteering());
				m_Table.SetVehicleThrottle(this, m_Boat.GetThrottle());
				m_Table.SetVehicleClutch(this, m_ClutchState);
			}
#ifdef EXPANSIONMODVEHICLE
			else if (m_Vehicle)
			{
				/*
				ExpansionVehicleSimulation simulation = m_Vehicle.m_Simulation;

				m_Table.SetVehicleSteering(this, simulation.GetSteering());
				m_Table.SetVehicleThrottle(this, simulation.GetThrottle());
				m_Table.SetVehicleBrake(this, simulation.GetBrake() != 0.0);
				m_Table.SetVehicleClutch(this, simulation.GetClutch());
				*/
			}
#endif
		}

		m_Time += pDt;
		if (m_State != m_PreviousState)
		{
			m_PreviousState = m_State;
			m_Time = 0;
			
			switch (m_State)
			{
			case STATE_GETTING_IN:
				m_Table.CallVehicleGetIn(this, m_SeatAnim);
				break;
			case STATE_GETTING_OUT:
				m_Table.CallVehicleGetOut(this);
				break;
			case STATE_JUMPING_OUT:
				m_Table.CallVehicleJumpOut(this);
				break;
			case STATE_SWITCHING_SEAT:
				m_Table.CallVehicleSwitchSeat(this, m_SeatAnim);
				break;
			}
		}
	}

	override void PrePhysUpdate(float pDt)
	{
		if (m_State == STATE_JUMPING_OUT)
		{
			if (m_Table.IsLeaveVehicle(this))
			{
				m_PreviousState = m_State;
				m_State = STATE_JUMPED_OUT;

				LeaveVehicle();
			}
		}
		
		vector transform[4];
		vector vehicleTransform[4];
		m_Object.GetTransform(vehicleTransform);
		
		//Math3D.MatrixMultiply4(vehicleTransform, m_StartTransform, transform);
		//Shape.Create(ShapeType.LINE, 0xFFFF0000, ShapeFlags.NOZBUFFER | ShapeFlags.ONCE, transform[3], transform[3] + transform[0]);
		//Shape.Create(ShapeType.LINE, 0xFF00FF00, ShapeFlags.NOZBUFFER | ShapeFlags.ONCE, transform[3], transform[3] + transform[1]);
		//Shape.Create(ShapeType.LINE, 0xFF0000FF, ShapeFlags.NOZBUFFER | ShapeFlags.ONCE, transform[3], transform[3] + transform[2]);
		//
		//Math3D.MatrixMultiply4(vehicleTransform, m_TargetTransform, transform);
		//Shape.Create(ShapeType.LINE, 0xFFFF0000, ShapeFlags.NOZBUFFER | ShapeFlags.ONCE, transform[3], transform[3] + transform[0]);
		//Shape.Create(ShapeType.LINE, 0xFF00FF00, ShapeFlags.NOZBUFFER | ShapeFlags.ONCE, transform[3], transform[3] + transform[1]);
		//Shape.Create(ShapeType.LINE, 0xFF0000FF, ShapeFlags.NOZBUFFER | ShapeFlags.ONCE, transform[3], transform[3] + transform[2]);

		float linearSpeed = 1.0;
		float angularSpeed = Math.PI;
		
		if (m_State != STATE_AWAIT)
		{
			linearSpeed = Math.Max(m_LinearSpeed, linearSpeed);
			angularSpeed = Math.Max(m_AngularSpeed, angularSpeed);
		}
		
		linearSpeed *= pDt;
		angularSpeed *= pDt;
		
		float rotation[4];
		vector translation;
		
		PrePhys_GetRotation(rotation);
		PrePhys_GetTranslation(translation);
		
		vector currentTransform[4];
		m_Player.GetTransformWS(currentTransform);
		
		vector deltaTransform[4];
		Math3D.MatrixInvMultiply3(currentTransform, m_TargetTransform, deltaTransform);
		
		vector linearVelocity = m_TargetTransform[3] - currentTransform[3];
		vector angularVelocity = Math3D.MatrixToAngles(deltaTransform) * Math.DEG2RAD;
						
		float linearMagnitude = linearVelocity.Normalize();
		float angularMagnitude = angularVelocity.Normalize();
		
		vector currentLinearAcceleration = translation.Multiply3(currentTransform);
		vector currentAngularAcceleration = Math3D.QuatToAngles(rotation) * Math.DEG2RAD;
		
		vector targetLinearAcceleration = linearVelocity * Math.Min(linearSpeed, linearMagnitude);
		vector targetAngularAcceleration = angularVelocity * Math.Min(angularSpeed, angularMagnitude);
		
		vector linearAcceleration = vector.Lerp(currentLinearAcceleration, targetLinearAcceleration, 0.5);
		vector angularAcceleration = vector.Lerp(currentAngularAcceleration, targetAngularAcceleration, 0.5);
		
		Math3D.YawPitchRollMatrix(angularAcceleration * Math.RAD2DEG, deltaTransform);
		deltaTransform[3] = linearAcceleration;
		
		Math3D.MatrixToQuat(deltaTransform, rotation);
		
		//Math3D.MatrixMultiply3(currentTransform, deltaTransform, currentTransform);
		translation = deltaTransform[3].InvMultiply3(currentTransform);
			
		PrePhys_SetRotation(rotation);
		PrePhys_SetTranslation(translation);
		
		//Print(linearMagnitude);
		//Print(linearSpeed);
		//Print(angularMagnitude);
		//Print(angularSpeed);
		
		bool linearCheck;
		//! https://feedback.bistudio.com/T173348
		if (linearMagnitude <= linearSpeed || linearSpeed == 0.0)
			linearCheck = true;
		bool angularCheck;
		if (angularMagnitude <= angularSpeed || angularSpeed == 0.0)
			angularCheck = true;
		
		if (linearCheck && angularCheck)
		{
			if (m_State == STATE_PRE_GETTING_IN)
			{
				m_PreviousState = m_State;
				
				if (m_Transport) m_Transport.CrewTransform(m_SeatIndex, transform);
#ifdef EXPANSIONMODVEHICLE
				else if (m_Vehicle) m_Vehicle.CrewTransform(m_SeatIndex, transform);
#endif
	
				ChangeState(STATE_GETTING_IN, transform);
			}
		}
	}

	override bool PostPhysUpdate(float pDt)
	{		
		switch (m_State)
		{
		case STATE_GETTING_IN:
			if (m_Time > m_TimeMax)
				ChangeState(STATE_AWAIT);
			break;
		case STATE_GETTING_OUT:
			if (m_Time > m_TimeMax)
				ChangeState(STATE_FINISH);
			break;
		case STATE_JUMPED_OUT:
		case STATE_JUMPING_OUT:
			if (m_Time > m_TimeMax)
				ChangeState(STATE_FINISH);
			break;
		case STATE_SWITCHING_SEAT:
			if (m_Time > m_TimeMax)
				ChangeState(STATE_AWAIT);
			break;
		}

		return m_State != STATE_FINISH;
	}

	void GetOutVehicle()
	{
		vector transform[4];
		
		vector crewPos;
		vector crewDir;

		if (m_Transport) m_Transport.CrewEntry(m_SeatIndex, crewPos, crewDir);
#ifdef EXPANSIONMODVEHICLE
		else if (m_Vehicle) m_Vehicle.CrewEntry(m_SeatIndex, crewPos, crewDir);
#endif
		
		crewDir[1] = 0.0;
		crewDir.Normalize();

		Math3D.DirectionAndUpMatrix(-crewDir, "0 1 0", transform);
		transform[3] = crewPos;

		ChangeState(STATE_GETTING_OUT, transform);
	}

	void JumpOutVehicle()
	{
		vector transform[4];
		
		vector crewPos;
		vector crewDir;

		if (m_Transport) m_Transport.CrewEntry(m_SeatIndex, crewPos, crewDir);
#ifdef EXPANSIONMODVEHICLE
		else if (m_Vehicle) m_Vehicle.CrewEntry(m_SeatIndex, crewPos, crewDir);
#endif
		
		crewDir[1] = 0.0;
		crewDir.Normalize();

		Math3D.DirectionAndUpMatrix(-crewDir, "0 1 0", transform);
		transform[3] = crewPos;

		ChangeState(STATE_JUMPING_OUT, transform);
	}

	void SwitchSeat(int seatIdx, int seatAnimType)
	{
		m_SeatAnim = seatAnimType;
		
		vector transform[4];

		if (m_Transport)
		{
			m_Transport.CrewGetOut(m_SeatIndex);

			m_SeatIndex = seatIdx;

			m_Transport.CrewTransform(m_SeatIndex, transform);
			m_Transport.CrewGetIn(m_Player, m_SeatIndex);
		}
#ifdef EXPANSIONMODVEHICLE
		else if (m_Vehicle)
		{
			m_Vehicle.CrewGetOut(m_SeatIndex);

			m_SeatIndex = seatIdx;

			m_Vehicle.CrewTransform(m_SeatIndex, transform);
			m_Vehicle.CrewGetIn(m_Player, m_SeatIndex);
		}
#endif

		ChangeState(STATE_SWITCHING_SEAT, transform);
	}

	bool IsGettingOut()
	{
		return m_State == STATE_GETTING_OUT || m_State == STATE_JUMPING_OUT;
	}

	bool IsGettingIn()
	{
		return m_State == STATE_GETTING_IN;
	}

	bool IsSwitchSeat()
	{
		return m_State == STATE_SWITCHING_SEAT;
	}

	Object GetObject()
	{
		return m_Object;
	}

	Transport GetTransport()
	{
		return m_Transport;
	}

#ifdef EXPANSIONMODVEHICLE
	ExpansionVehicleBase GetExpansionVehicle()
	{
		return m_Vehicle;
	}
#endif

	int GetVehicleSeat()
	{
		return m_SeatIndex;
	}

	void SetVehicleType(int vehicleType)
	{
		m_VehicleType = vehicleType;
	}

	void KeepInVehicleSpaceAfterLeave(bool state)
	{
		m_KeepInVehicleSpaceAfterLeave = state;
	}

	void SignalGearChange()
	{
		m_GearChanged = true;
	}

	bool WasGearChange()
	{
		bool changed = m_GearChanged & !m_ClutchState;
		m_GearChanged = false;
		return changed;
	}

	void SetClutchState(bool state)
	{
		m_ClutchState = state;
	}
};
