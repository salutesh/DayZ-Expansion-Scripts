/*
 * ExpansionVehicleModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleModule
{
	EntityAI m_Vehicle;
	ExpansionController m_Controller;

	vector m_Position;
	bool m_SelfDebugWindow;

	bool m_TEMP_DeferredInit;
	bool m_SettingsChanged;
	bool m_Control;
	bool m_PreSimulate;
	bool m_Simulate;
	bool m_Animate;
	bool m_Network;

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

	void ~ExpansionVehicleModule()
	{
#ifdef CF_DebugUI
		CF_Debug.Destroy(this);
#endif
	}

	void Init()
	{
#ifdef CF_DebugUI
		if (m_SelfDebugWindow)
		{
			CF_Debug.Create(this);
		}
#endif
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

	void NetworkSend(ParamsWriteContext ctx)
	{
	}

	void NetworkRecieve(ParamsReadContext ctx)
	{
	}

#ifdef CF_DebugUI
	bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		instance.SetPosition(m_Vehicle.ModelToWorld(m_Position * 2.0));

		return true;
	}
#endif
};
