class ExpansionInterpolatedInput
{
	static bool s_Debug;

#ifdef COMPONENT_SYSTEM
	string m_Action;
#else
	int m_Action;
#endif
	private UAIDWrapper m_InputWrapper;
	float m_Speed;
	float m_ValueRaw;
	float m_Value;

#ifdef COMPONENT_SYSTEM
	void ExpansionInterpolatedInput(string action, float speed = 50.0)
#else
	void ExpansionInterpolatedInput(int action, float speed = 50.0)
#endif
	{
		m_Action = action;
	#ifdef COMPONENT_SYSTEM
		m_InputWrapper = GetUApi().GetInputByName(m_Action).GetPersistentWrapper();
	#else
		m_InputWrapper = GetUApi().GetInputByID(m_Action).GetPersistentWrapper();
	#endif
		m_Speed = speed;
	}

	float GetValueRaw(UAInterface inputInterface = null)
	{
		if (inputInterface)
		{
		#ifdef COMPONENT_SYSTEM
			m_ValueRaw = inputInterface.SyncedValue(m_Action);
		#else
			m_ValueRaw = inputInterface.SyncedValue_ID(m_Action);
		#endif
		}
		else
		{
			m_ValueRaw = m_InputWrapper.InputP().LocalValue();
		}

		return m_ValueRaw;
	}

	float GetValue(float dt, float sensitivity = 1.0, UAInterface inputInterface = null)
	{
		float prev = m_ValueRaw;

		GetValueRaw(inputInterface);

		//! Hold key = interpolate to tgt value, release key = instantly go to tgt value
		if (m_ValueRaw != 0.0)
		{
			float t = 1.0 - Math.Exp(-m_Speed * sensitivity * dt);
			m_Value = Math.Lerp(m_Value, m_ValueRaw, t);
		}
		else
		{
			m_Value = m_ValueRaw;
		}

	#ifdef DIAG_DEVELOPER
		if (s_Debug && Math.Round(m_Value * 10000) != Math.Round(m_ValueRaw * 10000))
			EXPrint(this, "m_Action = " + m_Action + " m_ValueRaw = " + m_ValueRaw.ToString(false) + " m_Value = " + m_Value.ToString(false) + " dt " + dt);
	#endif

		return m_Value;
	}
}
