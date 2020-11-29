/**
 * ExpansionVehicleActionStopMotor.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleActionStopMotor : ActionSingleUseBase
{
	void ExpansionVehicleActionStopMotor()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ITEM_OFF;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#STR_EXPANSION_UA_STOP_BOAT";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		ExpansionHumanCommandVehicle vehCmd = player.GetCommand_ExpansionVehicle();
		ExpansionVehicleBase car;

		if (vehCmd && Class.CastTo(car, vehCmd.GetTransport()))
		{
			if (vehCmd.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER)
			{
				if (car.MotorIsOn())
				{
					return true;
				}
			}
		}

		return false;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		ExpansionHumanCommandVehicle vehCmd = action_data.m_Player.GetCommand_ExpansionVehicle();
		ExpansionVehicleBase car;

		if (vehCmd && Class.CastTo(car, vehCmd.GetTransport()))
		{
			car.MotorStop();

			if (!GetGame().IsMultiplayer())
				SEffectManager.PlaySound(car.m_EngineStopFuel, car.GetPosition());
		}
	}

	override void OnExecuteClient(ActionData action_data)
	{
		ExpansionHumanCommandVehicle vehCmd = action_data.m_Player.GetCommand_ExpansionVehicle();
		ExpansionVehicleBase car;

		if (vehCmd && Class.CastTo(car, vehCmd.GetTransport()))
		{
			car.MotorStop();
			
			SEffectManager.PlaySound(car.m_EngineStopFuel, car.GetPosition());
		}
	}

	override bool CanBeUsedInVehicle()
	{
		return true;
	}
};