class ExpansionVehicleRotational : ExpansionVehicleModule
{
	float m_Velocity;
	float m_Acceleration;
	float m_Torque;

	float m_Inertia;
	float m_Ratio;
	float m_InvRatio;
	float m_Radius;

	autoptr array<ExpansionVehicleRotational> m_Components = new array<ExpansionVehicleRotational>();
	ExpansionVehicleRotational m_Parent;

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
		m_PostSimulate = true;
		m_Animate = false;
		m_Network = false;
		
		m_SelfDebugWindow = true;
	}

	void AddComponent(ExpansionVehicleRotational component)
	{
		component.m_Parent = this;
		m_Components.Insert(component);
	}

	override void PreSimulate(ExpansionPhysicsState pState)
	{
		m_Torque = 0;

		ProcessTorque(pState);
	}

	override void Simulate(ExpansionPhysicsState pState)
	{
		m_Acceleration = 0;

		if (m_Ratio == 0.0) m_InvRatio = 0.0;
		else m_InvRatio = 1.0 / m_Ratio;

		ProcessAcceleration(pState);
	}

	float GetInertia()
	{
		float inertia = m_Inertia;
		if (m_Parent) inertia *= m_Parent.GetInertia();
		return inertia;
	}

	void ProcessTorque(ExpansionPhysicsState pState)
	{
		if (!m_Parent)
		{
			return;
		}

		m_Torque = m_Parent.m_Torque * m_Ratio;
	}

	void ProcessAcceleration(ExpansionPhysicsState pState)
	{
		if (m_Components.Count() == 0)
		{
			return;
		}
		/*
		float acceleration = 0;
		foreach (auto component : m_Components)
		{
			acceleration += component.m_Acceleration;
		}

		acceleration /= m_Components.Count();

		m_Velocity += acceleration * pState.m_DeltaTime;
		m_Acceleration = acceleration * m_InvRatio;
		*/

		float acceleration = 0;
		foreach (auto component : m_Components)
		{
			acceleration += component.m_Velocity;
		}

		acceleration /= m_Components.Count();

		m_Velocity = acceleration * m_InvRatio;
	}

	float FromRPM(float rpm)
	{
		return rpm * Math.PI / 30.0;
	}

	float ToRPM(float rot)
	{
		return Math.AbsFloat(rot) * 30.0 / Math.PI;
	}

	float Sign(float number)
	{
		if (number < 0)
			return -1;
		return 1;
	}

#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		super.CF_OnDebugUpdate(instance, type);

		instance.Add("Parent", m_Parent);
		instance.Add("Component Count", m_Components.Count());
		for (int i = 0; i < m_Components.Count(); i++)
		{
			instance.Add("Component[" + i + "]", m_Components[i]);
		}

		instance.Add("Torque", m_Torque);
		instance.Add("Acceleration", m_Acceleration);
		instance.Add("Velocity", m_Velocity);

		return true;
	}
#endif
};
