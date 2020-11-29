/**
 * ExpansionHumanCommandVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionHumanCommandVehicle extends ExpansionHumanCommandScript
{
	const int STATE_UNKNOWN = -1;
	const int STATE_AWAIT = 1;
	const int STATE_GETTING_IN = 2;
	const int STATE_GETTING_OUT = 3;
	const int STATE_JUMPING_OUT = 4;
	const int STATE_JUMPED_OUT = 5;
	const int STATE_SWITCHING_SEAT = 6;
	const int STATE_FINISH = 7;

	const float TIME_GET_IN = 1.0;
	const float TIME_GET_OUT = 1.5;
	const float TIME_JUMP_OUT = 2.0;
	const float TIME_SWITCH_SEAT = 1.0;

	private DayZPlayerImplement m_Player;
	private ExpansionHumanST m_Table;
	private HumanInputController m_Input;

	private ExpansionVehicleBase m_Vehicle;

	private int m_SeatIndex;
	private int m_SeatAnim;
	private int m_VehicleType;

	private int m_State;
	private int m_PreviousState;

	private float m_Time;
	private float m_TimeMax;

	private vector m_StartTransform[4];
	private vector m_TargetTransform[4];

	private bool m_GearChanged;
	private bool m_ClutchState;
	private bool m_LeftVehicle;

	private bool m_KeepInVehicleSpaceAfterLeave;

	void ExpansionHumanCommandVehicle(DayZPlayerImplement player, ExpansionVehicleBase vehicle, int seatIdx, int seat_anim, ExpansionHumanST table)
	{
		m_Player = player;
		m_Table = table;

		m_Vehicle = vehicle;

		m_SeatIndex = seatIdx;
		m_SeatAnim = seat_anim;

		m_Input = player.GetInputController();
		
		m_State = STATE_UNKNOWN;
		m_PreviousState = STATE_UNKNOWN;
	}

	private void ChangeState(int pState, vector pNewTransform[4])
	{
		m_Time = 0;
		m_State = pState;
		m_Player.GetTransformWS(m_StartTransform);

		m_TargetTransform[0] = pNewTransform[0];
		m_TargetTransform[1] = pNewTransform[1];
		m_TargetTransform[2] = pNewTransform[2];
		m_TargetTransform[3] = pNewTransform[3];
	}

	private void LeaveVehicle()
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

		m_Vehicle.CrewTransform(m_SeatIndex, m_TargetTransform);
		m_Vehicle.CrewGetIn(m_Player, m_SeatIndex);

		m_Table.SetVehicleType(this, m_VehicleType);

		dBodyEnableGravity(m_Player, false);

		dBodyActive(m_Player, ActiveState.INACTIVE);

		m_Player.OnCommandExpansionVehicleStart();

		ChangeState(STATE_GETTING_IN, m_TargetTransform);
	}

	override void OnDeactivate()
	{
		LeaveVehicle();

		m_Player.OnCommandExpansionVehicleFinish();
	}

	override void PreAnimUpdate(float pDt)
	{
		float heading = m_Input.GetHeadingAngle();
		PreAnim_SetFilteredHeading(0, 0.3, 180);

		m_Table.SetLook(this, true);

		HumanCommandWeapons hcw = m_Player.GetCommandModifier_Weapons();
		m_Table.SetLookDirX(this, hcw.GetBaseAimingAngleLR());
		m_Table.SetLookDirY(this, hcw.GetBaseAimingAngleUD());

		m_Table.SetVehicleType(this, m_VehicleType);
		m_Table.SetVehicleSteering(this, m_Vehicle.GetSteering() + 0);
		m_Table.SetVehicleThrottle(this, m_Vehicle.GetThrottle() + 0);
		m_Table.SetVehicleClutch(this, m_Vehicle.GetClutch() + 0);
		m_Table.SetVehicleBrake(this, m_Vehicle.GetBrake() != 0.0);
		
		switch (m_State)
		{
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
		if (m_State == STATE_JUMPING_OUT && m_Table.IsLeaveVehicle(this))
		{
			m_State = STATE_JUMPED_OUT;
			
			LeaveVehicle();
		}

		vector tmPlayer[4];
		vector rotationMat[3];
		
		Math3D.MatrixMultiply3(m_StartTransform, m_TargetTransform, rotationMat);
		
		float speedT = vector.Distance(m_StartTransform[3], m_TargetTransform[3]) / m_TimeMax;
		float speedR = Math3D.MatrixToAngles(rotationMat).Length() / m_TimeMax;
		
		m_Player.GetTransformWS(tmPlayer);
		Math3D.MatrixMultiply3(tmPlayer, m_TargetTransform, rotationMat);
		
		vector rotation = Math3D.MatrixToAngles(rotationMat);
		vector translation = vector.Direction(tmPlayer[3], m_TargetTransform[3]);
		
		ExpansionDebugger.Display(EXPANSION_DEBUG_PLAYER_VEHICLE_COMMAND, "rotation: " + rotation);
		ExpansionDebugger.Display(EXPANSION_DEBUG_PLAYER_VEHICLE_COMMAND, "translation: " + translation);
		
		float lenT = translation.Normalize();
		if (lenT < pDt)
			speedT *= lenT;
		
		float lenR = rotation.Normalize();
		if (lenR < pDt)
			speedR *= lenR;
		
		translation = translation * speedT * pDt;
		rotation = rotation * speedR * pDt;
		
		ExpansionDebugger.Display(EXPANSION_DEBUG_PLAYER_VEHICLE_COMMAND, "rotation: " + rotation);
		ExpansionDebugger.Display(EXPANSION_DEBUG_PLAYER_VEHICLE_COMMAND, "translation: " + translation);
		ExpansionDebugger.Display(EXPANSION_DEBUG_PLAYER_VEHICLE_COMMAND, "lenT: " + lenT);
		ExpansionDebugger.Display(EXPANSION_DEBUG_PLAYER_VEHICLE_COMMAND, "lenR: " + lenR);

		float q[4];
		rotation.RotationMatrixFromAngles(rotationMat);
		Math3D.MatrixToQuat(rotationMat, q);
		
		ExpansionDebugger.Display(EXPANSION_DEBUG_PLAYER_VEHICLE_COMMAND, "rotation: " + q[0] + ", " + q[1] + ", " + q[2] + ", " + q[3]);
		
		PrePhys_SetRotation(q);
		PrePhys_SetTranslation(translation);
	}

	override bool PostPhysUpdate(float pDt)
	{
		m_Time += pDt;

		switch (m_State)
		{
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

		ExpansionDebugger.Push(EXPANSION_DEBUG_PLAYER_VEHICLE_COMMAND);
		
		m_PreviousState = m_State;

		return m_State != STATE_FINISH;
	}

	void GetOutVehicle()
	{
		vector pos;
		vector dir;
		m_Vehicle.CrewEntry(m_SeatIndex, pos, dir);
		Math3D.DirectionAndUpMatrix(dir, vector.Up, m_TargetTransform);
		m_TargetTransform[3] = pos;

		ChangeState(STATE_GETTING_OUT, m_TargetTransform);
	}

	void JumpOutVehicle()
	{		
		vector pos;
		vector dir;
		m_Vehicle.CrewEntry(m_SeatIndex, pos, dir);
		Math3D.DirectionAndUpMatrix(dir, vector.Up, m_TargetTransform);
		m_TargetTransform[3] = pos;

		ChangeState(STATE_JUMPING_OUT, m_TargetTransform);
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

		m_Vehicle.SetClutchState(m_ClutchState);
	}
};