class ExpansionVehicleModule
{
	EntityAI m_Vehicle;
	ExpansionController m_Controller;

	void ExpansionVehicleModule(EntityAI pVehicle)
	{
		m_Vehicle = pVehicle;

		auto vehicle = ExpansionVehicleBase.Cast(m_Vehicle);
		if (vehicle)
			m_Controller = vehicle.m_Controller;

		auto car = CarScript.Cast(m_Vehicle);
		if (car)
			m_Controller = car.m_Controller;
	}

	void Init()
	{
	}

	void TEMP_DeferredInit()
	{
	}

	void SettingsChanged()
	{
	}

	/**
	 * @note pDriver could be an AI, fuck would I know to do in this case though :pepelul:
	 */
	void Control(ExpansionPhysicsState pState, DayZPlayerImplement pDriver)
	{
	}

	void PreSimulate(ExpansionPhysicsState pState)
	{
	}

	void Simulate(ExpansionPhysicsState pState)
	{
	}

	void Animate(ExpansionPhysicsState pState)
	{
	}

	void OnNetworkSend(ParamsWriteContext ctx)
	{
	}

	void OnNetworkRecieve(ParamsReadContext ctx)
	{
	}

#ifdef CF_DebugUI
	bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		return false;
	}
#endif
};
