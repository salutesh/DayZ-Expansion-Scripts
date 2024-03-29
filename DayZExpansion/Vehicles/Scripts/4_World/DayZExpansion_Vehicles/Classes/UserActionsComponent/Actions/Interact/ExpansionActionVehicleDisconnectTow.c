/**
 * ExpansionActionVehicleDisconnectTow.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionVehicleDisconnectTow : ActionInteractBase
{
	bool m_IsWinch;

	void ExpansionActionVehicleDisconnectTow()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_HUDCursorIcon = CursorIcons.OpenDoors;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		if (m_IsWinch)
			return "#STR_EXPANSION_DISCONNECT_WINCH";

		return "#STR_EXPANSION_DISCONNECT_TOW";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		auto vehCommand = player.GetCommand_ExpansionVehicle();

		ExpansionVehicleBase car;
		if (vehCommand && Class.CastTo(car, vehCommand.GetExpansionVehicle()))
		{
			if (car.CrewMemberIndex(player) == DayZPlayerConstants.VEHICLESEAT_DRIVER)
			{
				m_IsWinch = car.IsHelicopter();

				return car.Expansion_IsTowing();
			}
		}
		return false;
	}

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);

		auto vehCommand = action_data.m_Player.GetCommand_ExpansionVehicle();

		ExpansionVehicleBase car;
		if (vehCommand && Class.CastTo(car, vehCommand.GetExpansionVehicle()))
		{
			if (car.CrewMemberIndex(action_data.m_Player) == DayZPlayerConstants.VEHICLESEAT_DRIVER)
			{
				car.Expansion_DestroyTow();

				if (GetGame().IsMultiplayer() && GetExpansionSettings().GetLog().VehicleTowing)
				{
					string msg = "[VehicleTowing] Player \"" + action_data.m_Player.GetIdentity().GetName() + "\" (id=" + action_data.m_Player.GetIdentity().GetId() + " pos=" + action_data.m_Player.GetPosition() + ")" + " untowed " + car.Expansion_GetTowedEntity().GetType() + " (id=" + ExpansionStatic.GetPersistentIDString(car.Expansion_GetTowedEntity()) + " pos=" + car.GetPosition();
					msg += " with " + car.GetType() + " (id=" + car.GetVehiclePersistentIDString() + " pos=" + car.GetPosition() + ")";
					GetExpansionSettings().GetLog().PrintLog(msg);
				}
			}
		}
	}

	override bool CanBeUsedInVehicle()
	{
		return true;
	}
};
