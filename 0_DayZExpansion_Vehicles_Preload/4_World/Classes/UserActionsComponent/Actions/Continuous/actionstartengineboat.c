/**
 * ActionStartEngineBoat.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2024 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifndef DAYZ_1_25
modded class ActionStartEngineBoat
{
	//! Can NOT call super here
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		ExpansionVehicle vehicle;
		if (ActionStartEngine.Expansion_ActionConditionStatic(player, target, item, vehicle))
		{
			if (vehicle.GetBoat())
				return true;
		}

		return false;
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
#endif
