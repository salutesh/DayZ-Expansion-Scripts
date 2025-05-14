/*
 * ExpansionVehicleModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleModule
{
	EntityAI m_Vehicle;
	Transport m_Transport;
	ExpansionController m_Controller;

	vector m_Position;
	bool m_SelfDebugWindow;

	bool m_TEMP_DeferredInit;
	bool m_SettingsChanged;
	bool m_Control;
	bool m_PreSimulate;
	bool m_Simulate;
	bool m_PostSimulate;
	bool m_Animate;
	bool m_Network;

	void ExpansionVehicleModule(EntityAI vehicle)
	{
		m_Vehicle = vehicle;
		Class.CastTo(m_Transport, vehicle);

		auto vehicleBase = ExpansionVehicleBase.Cast(m_Vehicle);
		if (vehicleBase)
			m_Controller = vehicleBase.m_Controller;

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

	event void ObtainMove(PawnMove pMove)
	{
	}

	event void ConsumeMove(PawnMove pMove)
	{
	}

	event bool ReplayMove(PawnMove pMove)
	{
		return true;
	}

	event void ObtainState(/*inout*/ PawnOwnerState pState)
	{
	}

	event void RewindState(PawnOwnerState pState, /*inout*/ PawnMove pMove, inout NetworkRewindType pRewindType)
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

	void PostSimulate(ExpansionPhysicsState pState)
	{
	}

	void Animate(ExpansionPhysicsState pState, float deltaTime)
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
