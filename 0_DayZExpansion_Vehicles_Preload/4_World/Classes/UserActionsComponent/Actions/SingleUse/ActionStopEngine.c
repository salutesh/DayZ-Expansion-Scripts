/**
 * ActionStopEngine.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionStopEngine
{
	string m_Expansion_EngineName;

	override string GetText()
	{
		if (m_Expansion_EngineName != string.Empty)
			return "#STR_EXPANSION_VEHICLE_ENGINE_STOP" + " " + m_Expansion_EngineName + " " + "#STR_EXPANSION_VEHICLE_ENGINE";

		return m_Text;
	}

	//! Can NOT call super here
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		auto vehicle = ExpansionVehicle.Get(player);
		if (!vehicle)
			return false;

	#ifndef DAYZ_1_25
		//! 1.26
		if (vehicle.GetBoat())
			return false;  //! Vanilla boat has own stop engine action
	#endif

		if (vehicle.IsPlayerInSeat(player, DayZPlayerConstants.VEHICLESEAT_DRIVER))
		{
			if (vehicle.EngineIsOn())
			{
				m_CommandUID = vehicle.GetEngineStopAnimation();

			#ifndef SERVER
				m_Expansion_EngineName = vehicle.EngineGetName();
			#endif

				if (vehicle.GetSpeedometerAbsolute() <= 8)
					return true;
			}
		}

		return false;
	}

#ifdef DAYZ_1_27
	override void OnExecuteClient(ActionData action_data)
	{
		ExpansionVehicle vehicle;
		if (ExpansionVehicle.Get(vehicle, action_data.m_Player))
		{
			if (vehicle.GetExpansionVehicleBase())
			{
				vehicle.EngineStop();
				return;
			}
		}

		super.OnExecuteClient(action_data);
	}

	override void OnExecuteServer(ActionData action_data)
	{
		auto vehicle = ExpansionVehicle.Get(action_data.m_Player);

		if (vehicle)
		{
			auto exVehicle = vehicle.GetExpansionVehicleBase();

			if (vehicle.EngineGetCurrent() > 0 || exVehicle)
			{
				vehicle.EngineStop();
				return;
			}
		}

		super.OnExecuteServer(action_data);
	}
#else
	//! 1.28+
	override void OnExecute(ActionData action_data)
	{
		auto vehicle = ExpansionVehicle.Get(action_data.m_Player);

		if (vehicle)
		{
			auto exVehicle = vehicle.GetExpansionVehicleBase();

			if (vehicle.EngineGetCurrent() > 0 || exVehicle)
			{
				if (GetGame().IsServer() || exVehicle)
					vehicle.EngineStop();

				return;
			}
		}

		super.OnExecute(action_data);
	}
#endif

	override bool Expansion_CheckSuccess(ActionData action_data)
	{
		ExpansionVehicle vehicle;
		if (ExpansionVehicle.Get(vehicle, action_data.m_Player))
		{
			return !vehicle.EngineIsOn();
		}

		return false;
	}
};
