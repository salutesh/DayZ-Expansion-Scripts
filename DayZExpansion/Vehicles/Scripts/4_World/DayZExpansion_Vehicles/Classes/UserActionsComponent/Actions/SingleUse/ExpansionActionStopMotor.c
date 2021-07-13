/**
 * ExpansionActionStopMotor.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionStopMotor : ActionSingleUseBase
{
	void ExpansionActionStopMotor()
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
		HumanCommandVehicle vehCmd = player.GetCommand_Vehicle();
		ExpansionBoatScript m_Boat;

		if (vehCmd && Class.CastTo(m_Boat, vehCmd.GetTransport()))
		{
			if (vehCmd.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER)
			{
				if (m_Boat.MotorIsOn())
				{
					return true;
				}
			}
		}

		return false;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		HumanCommandVehicle vehCmd = action_data.m_Player.GetCommand_Vehicle();
		ExpansionBoatScript m_Boat;

		if (vehCmd && Class.CastTo(m_Boat, vehCmd.GetTransport()))
		{
			m_Boat.MotorStop();
		}
	}

	override void OnExecuteClient(ActionData action_data)
	{
		if (GetGame().IsMultiplayer())
			return;

		HumanCommandVehicle vehCmd = action_data.m_Player.GetCommand_Vehicle();
		ExpansionBoatScript m_Boat;

		if (vehCmd && Class.CastTo(m_Boat, vehCmd.GetTransport()))
		{
			m_Boat.MotorStop();
		}
	}

	override bool CanBeUsedInVehicle()
	{
		return true;
	}
	
	override bool UseMainItem()
	{
		return false;
	}
};