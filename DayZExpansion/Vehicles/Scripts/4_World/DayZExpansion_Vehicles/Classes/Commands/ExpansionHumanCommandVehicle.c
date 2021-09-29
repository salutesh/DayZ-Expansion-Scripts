/**
 * ExpansionHumanCommandVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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

	float TIME_PRE_GET_IN = 0.2;
	float TIME_GET_IN = 1.3;
	float TIME_GET_OUT = 1.5;
	float TIME_JUMP_OUT = 2.0;
	float TIME_SWITCH_SEAT = 1.0;

	ExpansionVehicleBase m_Vehicle;

	int m_SeatIndex;
	int m_SeatAnim;
	int m_VehicleType;

	int m_State;
	int m_PreviousState;

	float m_Time;
	float m_TimeMax;

	float m_TranslationSpeed;

	vector m_StartTransform[4];
	vector m_TargetTransform[4];

	bool m_GearChanged;
	bool m_ClutchState;
	bool m_LeftVehicle;

	bool m_KeepInVehicleSpaceAfterLeave;

	void ExpansionHumanCommandVehicle(DayZPlayerImplement player, ExpansionHumanST table, ExpansionVehicleBase vehicle, int seatIdx, int seat_anim)
	{
		m_Vehicle = vehicle;

		m_SeatIndex = seatIdx;
		m_SeatAnim = seat_anim;

		m_State = STATE_UNKNOWN;
		m_PreviousState = STATE_UNKNOWN;
	}

	void ChangeState(int pState, vector pNewTransform[4])
	{
		m_Time = 0;
		m_State = pState;
		
		m_StartTransform[0] = m_TargetTransform[0];
		m_StartTransform[1] = m_TargetTransform[1];
		m_StartTransform[2] = m_TargetTransform[2];
		m_StartTransform[3] = m_TargetTransform[3];

		m_TargetTransform[0] = pNewTransform[0];
		m_TargetTransform[1] = pNewTransform[1];
		m_TargetTransform[2] = pNewTransform[2];
		m_TargetTransform[3] = pNewTransform[3];
	}

	void LeaveVehicle()
	{
		if (m_LeftVehicle)
			return;

		m_LeftVehicle = true;

		m_Table.SetVehicleType(this, -1);

		m_Vehicle.CrewGetOut(m_SeatIndex);

		dBodyActive(m_Player, ActiveState.ALWAYS_ACTIVE);
		dBodyEnableGravity(m_Player, true);

		if (!m_KeepInVehicleSpaceAfterLeave)
		{
			vector tmPlayer[4];
			vector tmVehicle[4];
			vector tmTarget[4];
			m_Player.GetTransformWS(tmPlayer);
			m_Vehicle.GetTransform(tmVehicle);
			Math3D.MatrixMultiply4(tmVehicle, tmPlayer, tmTarget);

			m_Player.UnlinkFromLocalSpace();
			m_Player.SetTransform(tmTarget);
		}
	}

	override void OnActivate()
	{
		super.OnActivate();

		m_LeftVehicle = false;

		if (m_Player.GetParent() != m_Vehicle)
		{
			vector tmPlayer[4];
			vector tmTarget[4];
			vector tmLocal[4];
			m_Player.GetTransformWS(tmPlayer);
			m_Vehicle.GetTransform(tmTarget);
			Math3D.MatrixInvMultiply4(tmTarget, tmPlayer, tmLocal);

			m_Player.LinkToLocalSpaceOf(m_Vehicle, tmLocal);
		}

		vector crewPos;
		vector crewDir;

		m_Vehicle.CrewEntry(m_SeatIndex, crewPos, crewDir);

		Math3D.DirectionAndUpMatrix(crewDir, "0 1 0", m_TargetTransform);
		m_TargetTransform[3] = crewPos;

		m_Vehicle.CrewGetIn(m_Player, m_SeatIndex);

		m_Table.SetVehicleType(this, m_VehicleType);

		dBodyEnableGravity(m_Player, false);

		dBodyActive(m_Player, ActiveState.INACTIVE);

		m_Player.OnCommandExpansionVehicleStart();

		ChangeState(STATE_PRE_GETTING_IN, m_TargetTransform);
		m_Player.GetTransformWS(m_StartTransform);
	}

	override void OnDeactivate()
	{
		super.OnDeactivate();

		LeaveVehicle();

		m_Player.OnCommandExpansionVehicleFinish();
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

		PreAnim_SetFilteredHeading(0, 0.3, 180);

		m_Table.SetLook(this, true);

		HumanCommandWeapons hcw = m_Player.GetCommandModifier_Weapons();
		m_Table.SetLookDirX(this, hcw.GetBaseAimingAngleLR());
		m_Table.SetLookDirY(this, hcw.GetBaseAimingAngleUD());

		ExpansionController controller = m_Vehicle.GetExpansionController();

		m_Table.SetVehicleType(this, m_VehicleType);
		m_Table.SetVehicleSteering(this, controller.GetSteering());
		m_Table.SetVehicleThrottle(this, controller.GetThrottle());
		m_Table.SetVehicleClutch(this, controller.GetClutch());
		m_Table.SetVehicleBrake(this, controller.GetBrake() != 0.0);
		
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
		}

		if (m_State != m_PreviousState)
		{
			m_TranslationSpeed = vector.Distance(m_StartTransform[3], m_TargetTransform[3]) / m_TimeMax;

			switch (m_State)
			{
			case STATE_PRE_GETTING_IN:
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
		if ((m_PreviousState == STATE_AWAIT && m_State == STATE_AWAIT) || m_State == STATE_JUMPED_OUT)
			return;
		
		if (m_State == STATE_JUMPING_OUT)
		{
			if (m_Table.IsLeaveVehicle(this))
			{
				m_State = STATE_JUMPED_OUT;

				LeaveVehicle();
			}
		}
		
		vector transform[4];
		m_Player.GetTransformWS(transform);

		float rotation[4];
		float currentRotation[4];
		float targetRotation[4];
		PrePhys_GetRotation(currentRotation);
		Math3D.MatrixToQuat(m_TargetTransform, targetRotation);

		vector translation;
		PrePhys_GetTranslation(translation);		
		DBGDrawLineDirectionMS(translation * (1.0 / pDt), 0x4400FF00);

		translation = translation + vector.Direction(transform[3], m_TargetTransform[3]);

		if (m_State != m_PreviousState)
		{
			if (m_State == STATE_AWAIT)
			{
				translation = translation.InvMultiply3(transform);
				Math3D.QuatToMatrix(targetRotation, transform);
				translation = translation.InvMultiply3(transform);
				
				PrePhys_SetRotation(targetRotation);
				PrePhys_SetTranslation(translation);
				return;
			}
		}

		float dt = m_Time / m_TimeMax;
		if (dt > 1.0)
			dt = 1.0;
		
		float speed = m_TranslationSpeed;
		if (m_State != STATE_PRE_GETTING_IN)
			speed = translation.Length() / pDt;

		float len = translation.Normalize();
		if (len > pDt)
			len = pDt;

		translation = translation * speed * len;

		Math3D.QuatLerp(rotation, currentRotation, targetRotation, dt);
		
		translation = translation.InvMultiply3(transform);
		Math3D.QuatToMatrix(rotation, transform);
		translation = translation.InvMultiply3(transform);
		
		PrePhys_SetRotation(rotation);
		PrePhys_SetTranslation(translation);
		
		DBGDrawLineDirectionMS(translation * (1.0 / pDt), 0x44FF00FF);
	}

	override bool PostPhysUpdate(float pDt)
	{
		m_Time += pDt;

		m_PreviousState = m_State;

		switch (m_State)
		{
		case STATE_PRE_GETTING_IN:
			if (m_Time > TIME_PRE_GET_IN)
			{
				vector transform[4];
				m_Vehicle.CrewTransform(m_SeatIndex, m_TargetTransform);
				ChangeState(STATE_GETTING_IN, m_TargetTransform);
			}
			break;
		case STATE_GETTING_IN:
			if (m_Time > TIME_GET_IN)
				m_State = STATE_AWAIT;
			break;
		case STATE_GETTING_OUT:
			if (m_Time > TIME_GET_OUT)
				m_State = STATE_FINISH;
			break;
		case STATE_JUMPED_OUT:
		case STATE_JUMPING_OUT:
			if (m_Time > TIME_JUMP_OUT)
				m_State = STATE_FINISH;
			break;
		case STATE_SWITCHING_SEAT:
			if (m_Time > TIME_SWITCH_SEAT)
				m_State = STATE_AWAIT;
			break;
		}

		return m_State != STATE_FINISH;
	}

	void GetOutVehicle()
	{
		vector transform[4];
		
		vector pos;
		vector dir;
		m_Vehicle.CrewEntry(m_SeatIndex, pos, dir);
		Math3D.DirectionAndUpMatrix(-dir, vector.Up, transform);
		transform[3] = pos - (dir * 0.5);

		ChangeState(STATE_GETTING_OUT, transform);
	}

	void JumpOutVehicle()
	{
		vector transform[4];
		
		vector pos;
		vector dir;
		m_Vehicle.CrewEntry(m_SeatIndex, pos, dir);
		Math3D.DirectionAndUpMatrix(-dir, vector.Up, transform);
		transform[3] = pos - (dir * 0.5);

		ChangeState(STATE_JUMPING_OUT, transform);
	}

	void SwitchSeat(int seatIdx, int seatAnimType)
	{
		m_SeatAnim = seatAnimType;

		m_Vehicle.CrewGetOut(m_SeatIndex);

		m_SeatIndex = seatIdx;

		m_Vehicle.CrewTransform(m_SeatIndex, m_TargetTransform);
		m_Vehicle.CrewGetIn(m_Player, m_SeatIndex);

		ChangeState(STATE_SWITCHING_SEAT, m_TargetTransform);
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

	ExpansionVehicleBase GetTransport()
	{
		return m_Vehicle;
	}

	int GetVehicleSeat()
	{
		return m_SeatIndex;
	}

	void SetVehicleType(int vehicleType)
	{
		m_VehicleType = vehicleType;
	}

	void KeepInVehicleSpaceAfterLeave(bool pState)
	{
		m_KeepInVehicleSpaceAfterLeave = pState;
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

	void SetClutchState(bool pState)
	{
		m_ClutchState = pState;
	}
};
