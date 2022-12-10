/**
 * ExpansionVehicleTwoWheelAxle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class ExpansionVehicleTwoWheelAxle : ExpansionVehicleAxle
{
	float m_AntiRollForce;

	ExpansionVehicleWheel m_Left;
	ExpansionVehicleWheel m_Right;

	bool m_RawSteering = true;

	float m_TurnRadius;
	float m_WheelBase;
	float m_WheelOffset;

	override void Init()
	{
		super.Init();

		auto vehicle = ExpansionVehicleBase.Cast(m_Vehicle);

		int lastIndex = vehicle.GetAxleCount() - 1;

		if (m_Index != 0 && m_Index != lastIndex)
			return;

		m_TurnRadius = Math.AbsFloat(Math.DEG2RAD * m_MaxSteeringAngle) / (Math.PI2);
		if (m_TurnRadius == 0.0)
		{
			return;
		}

		vector frontWheel;
		vector backWheel;

		if (m_Index == 0)
		{
			frontWheel = m_Wheels[0].m_InitialTransform[3];
			backWheel = vehicle.GetAxle(lastIndex).m_Wheels[0].m_InitialTransform[3];
		}
		else
		{
			frontWheel = vehicle.GetAxle(lastIndex).m_Wheels[0].m_InitialTransform[3];
			backWheel = m_Wheels[0].m_InitialTransform[3];
		}

		m_WheelBase = Math.AbsFloat(backWheel[2] - frontWheel[2]);
		m_WheelOffset = Math.AbsFloat(backWheel[0]);

		m_RawSteering = false;
	}

	override ExpansionVehicleWheel AddWheel(string pName)
	{
		if (pName == "Left")
		{
			m_Left = super.AddWheel(pName);
			return m_Left;
		}
		else if (pName == "Right")
		{
			m_Right = super.AddWheel(pName);
			return m_Right;
		}

		return super.AddWheel(pName);
	}

	override void PreSimulate(ExpansionPhysicsState pState)
	{
		super.PreSimulate(pState);

		float sp = pState.GetModelVelocityAt("0 0 0")[2];

		m_Brake = 0.0;
		if (m_ControlIndex != -1)
		{
			if (m_MaxBrakeTorque)
				m_Brake = m_Controller.m_Brake[m_ControlIndex] * m_MaxBrakeTorque;  //! 1.19
			else
				m_Brake = m_Controller.m_Brake[m_ControlIndex] * m_BrakeForce * m_BrakeBias;
		}

		float steering = m_Controller.m_Yaw;

		m_AntiRollForce = Math.Clamp(sp * steering * 0.01 * pState.m_DeltaTime, -1, 1) * m_SwayBar;

		float ft1 = m_Left.m_FeedbackTorque;
		float ft2 = m_Right.m_FeedbackTorque;

		float bt1 = m_Left.m_BrakeTorque;
		float bt2 = m_Right.m_BrakeTorque;

		float j0 = GetInertia();
		float j1 = 1;
		float j2 = 1;

		float m0 = (m_Torque * m_Ratio);
		float m1 = (ft1 * m_Ratio) + bt1;
		float m2 = (ft2 * m_Ratio) + bt2;
		
		float mt = m1 + m2 + m0;
		float md = (m2 - m1) + m0;
		float jw = j1 + j2;
		float jt = jw + j0;

		float accSym = md / jw;

		// axles must be running at the same rpm
		//m_Torque = mt / jt;

		// directly set here instead of retrieving in the wheel
		//m_Right.m_Torque = m_Torque * m_Ratio;// + accSym;
		//m_Left.m_Torque = m_Torque * m_Ratio;// - accSym;

		m_Left.m_Steering = steering * m_MaxSteeringAngle;
		m_Right.m_Steering = steering * m_MaxSteeringAngle;

		if (!m_RawSteering)
		{
			float leftBackWheel = m_WheelOffset * -Sign(steering);
			float rightBackWheel = m_WheelOffset * Sign(steering);

			float leftFrontWheelSteer = Math.Atan2(m_WheelBase / (m_TurnRadius + leftBackWheel), 1);
			float rightFrontWheelSteer = Math.Atan2(m_WheelBase / (m_TurnRadius + rightBackWheel), 1);
			m_Left.m_Steering = Math.AbsFloat(Math.RAD2DEG * leftFrontWheelSteer * 2.0 / Math.PI) * steering * Sign(m_MaxSteeringAngle);
			m_Right.m_Steering = Math.AbsFloat(Math.RAD2DEG * rightFrontWheelSteer * 2.0 / Math.PI) * steering * Sign(m_MaxSteeringAngle);
		}
	}
};
