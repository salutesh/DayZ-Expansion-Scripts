/**
 * ActionStopEngine.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionStopEngine
{
	private CarScript m_Car;

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		if (m_Car.IsPlane())
		{
			return "#STR_EXPANSION_UA_STOP_PLANE";
		}
		else if (m_Car.IsHelicopter())
		{
			return "#STR_EXPANSION_UA_STOP_HELICOPTER";
		}
		else if (m_Car.IsBoat())
		{
			return "#STR_EXPANSION_UA_STOP_CAR";
		}

		return "#STR_EXPANSION_UA_STOP_CAR";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!Class.CastTo(m_Car, player.GetParent()))
			return false;

		if (m_Car.IsPlane())
		{
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ITEM_OFF;

			return super.ActionCondition(player, target, item);
		}
		else if (m_Car.IsHelicopter())
		{
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ITEM_OFF;

			return super.ActionCondition(player, target, item);
		}
		else if (m_Car.IsBoat())
		{
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ITEM_OFF;

			HumanCommandVehicle vehCmd = player.GetCommand_Vehicle();

			if (vehCmd && vehCmd.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER)
			{
				if (m_Car.EngineIsOn())
				{
					return true;
				}
			}

			return false;
		}

		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_STOPENGINE;

		return super.ActionCondition(player, target, item);
	}
};