/**
 * ActionStartEngine.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionStartEngine
{
	string m_Expansion_EngineName;

	override string GetText()
	{
		return "#STR_EXPANSION_VEHICLE_ENGINE_START" + " " + m_Expansion_EngineName + " " + "#STR_EXPANSION_VEHICLE_ENGINE";
	}

	//! Can NOT call super here
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		ExpansionVehicle vehicle;
		if (Expansion_ActionConditionStatic(player, target, item, vehicle))
		{
		#ifndef DAYZ_1_25
			//! 1.26
			if (vehicle.GetBoat())
				return false;  //! Vanilla boat has own start engine action
		#endif

		#ifndef DAYZ_1_29
			//! 1.30
			if (vehicle.GetMotorbike())
				return false;  //! Vanilla motorbike has own start engine action
		#endif

			m_CommandUID = vehicle.GetEngineStartAnimation();

		#ifndef SERVER
			m_Expansion_EngineName = vehicle.EngineGetName();
		#endif

			return true;
		}

		return false;
	}

	static bool Expansion_ActionConditionStatic(PlayerBase player, ActionTarget target, ItemBase item, out ExpansionVehicle vehicle)
	{
		if (!ExpansionVehicle.Get(vehicle, player))
			return false;

		if (!vehicle.EngineIsOn())
		{
			if (vehicle.GetHealthLevel("Engine") >= GameConstants.STATE_RUINED)
				return false;

			if (vehicle.CrewMemberIndex(player) == DayZPlayerConstants.VEHICLESEAT_DRIVER)
			{
				switch (vehicle.GetLockState())
				{
					case ExpansionVehicleLockState.NOLOCK:
					case ExpansionVehicleLockState.FORCEDUNLOCKED:
						return true;
					case ExpansionVehicleLockState.FORCEDLOCKED:
						return false;
				}

				if (!GetExpansionSettings().GetVehicle(false).IsLoaded())
					return false;

				ExpansionCarKey key;
				if (GetExpansionSettings().GetVehicle().VehicleRequireKeyToStart == 1)
				{
					return player.HasKeyForCar(vehicle);
				}
				else if (GetExpansionSettings().GetVehicle().VehicleRequireKeyToStart == 2)
				{
					if (!Class.CastTo(key, player.GetItemInHands()))
						return false;

					if (!vehicle.IsPairedTo(key))
						return false;

					return true;
				}

				//! GetExpansionSettings().GetVehicle().VehicleRequireKeyToStart == 0, or invalid number but this still saves checking
				return true;
			}
		}

		return false;
	}

	//! 1.28+
	override void OnFinishProgress(ActionData action_data)
	{
		ExpansionVehicle vehicle;
		if (ExpansionVehicle.Get(vehicle, action_data.m_Player))
		{
			if (vehicle.EngineGetCurrent() > 0)
			{
				if (g_Game.IsServer())
					vehicle.EngineStart();

				return;
			}
		}

		super.OnFinishProgress(action_data);
	}

	override bool Expansion_CheckSuccess(ActionData action_data)
	{
		ExpansionVehicle vehicle;
		if (ExpansionVehicle.Get(vehicle, action_data.m_Player))
		{
			return vehicle.EngineIsOn();
		}

		return false;
	}
};
