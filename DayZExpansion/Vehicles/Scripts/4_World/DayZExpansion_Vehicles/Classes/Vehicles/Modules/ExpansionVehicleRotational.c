class ExpansionVehicleRotational : ExpansionVehicleModule
{
	float m_Rotation;
	float m_PositionPrev;

	float m_Velocity;
	float m_Acceleration;

	float m_Torque;

	float m_RPM;

	float m_Inertia;
	float m_Ratio;
	float m_Radius;

	ExpansionVehicleRotational m_Parent;
	ExpansionVehicleRotational m_Component;

	void ExpansionVehicleRotational(EntityAI vehicle)
	{
		m_Inertia = 1.0;
		m_Ratio = 1.0;
		m_Radius = 1.0;

		// event flags (but as bools cause lazy)
		m_TEMP_DeferredInit = false;
		m_SettingsChanged = false;
		m_Control = false;
		m_PreSimulate = true;
		m_Simulate = true;
		m_Animate = false;
		m_Network = false;
		
		m_SelfDebugWindow = true;
	}

	override void PreSimulate(ExpansionPhysicsState pState)
	{
		m_Acceleration = m_Torque;

		if (m_Component)
		{
			m_Component.m_Torque = m_Acceleration * m_Ratio * m_Radius;

			float accel = m_Velocity - (m_Component.m_Velocity / m_Ratio);
			m_Acceleration -= accel;// * pState.m_DeltaTime;
		}
	}

	override void Simulate(ExpansionPhysicsState pState)
	{
		//m_Velocity = (m_PositionPrev - m_Rotation) / pState.m_DeltaTime;
		//m_PositionPrev = m_Rotation;

		m_Velocity += m_Acceleration * pState.m_DeltaTime;
		m_Rotation += m_Velocity * pState.m_DeltaTime;

		m_Rotation = Math.WrapFloatInclusive(m_Rotation, 0, Math.PI2);
		m_RPM = Math.AbsFloat(m_Velocity) * 30.0 / Math.PI;
	}

	static float FromRPM(float rpm)
	{
		return rpm * Math.PI / 30.0;
	}

	static float ToRPM(float rot)
	{
		return Math.AbsFloat(rot) * 30.0 / Math.PI;
	}

#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		super.CF_OnDebugUpdate(instance, type);

		instance.Add("Parent", m_Parent);
		instance.Add("Component", m_Component);

		instance.Add("Ratio", m_Ratio);
		instance.Add("Radius", m_Radius);
		instance.Add("Inertia", m_Inertia);

		instance.Add("Torque", m_Torque);
		instance.Add("Acceleration", m_Acceleration);
		instance.Add("Velocity", m_Velocity);

		instance.Add("Position", m_Rotation);

		instance.Add("RPM", m_RPM);

		return true;
	}
#endif
};
