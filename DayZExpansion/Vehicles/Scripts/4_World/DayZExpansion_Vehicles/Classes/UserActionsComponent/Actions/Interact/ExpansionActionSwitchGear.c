/**
 * ExpansionActionSwitchGear.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

[RegisterAction(ExpansionActionSwitchGear)]
class ExpansionActionSwitchGear: ActionInteractBase
{
	void ExpansionActionSwitchGear()
	{
		m_CommandUID		= DayZPlayerConstants.CMD_ACTIONMOD_OPENLID;
		m_StanceMask		= DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon	 = CursorIcons.LootCorpse;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}	

	override string GetText()
	{
		return "Toggle Gear";
	}

	override bool Can( PlayerBase player, ActionTarget target, ItemBase item )
	{
		return super.Can( player, target, item );
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();
		ExpansionHumanCommandVehicle expVehCommand = player.GetCommand_ExpansionVehicle();
		if (!vehCommand && !expVehCommand)
			return false;

		CarScript car;
		if (vehCommand && (!Class.CastTo(car, vehCommand.GetTransport()) || vehCommand.GetVehicleSeat() != DayZPlayerConstants.VEHICLESEAT_DRIVER) && !car.HasGear())
			return false;

		ExpansionVehicleBase vehicle;
		if (expVehCommand && (!Class.CastTo(vehicle, expVehCommand.GetExpansionVehicle()) || expVehCommand.GetVehicleSeat() != DayZPlayerConstants.VEHICLESEAT_DRIVER) && !vehicle.HasGear())
			return false;

		return false;
	}

	override void OnExecuteServer( ActionData action_data )
	{
		CarScript car;
		if ( Class.CastTo( car, action_data.m_Player.GetParent() ) )
			car.SwitchGear();

		ExpansionVehicleBase vehicle;
		if ( Class.CastTo( vehicle, action_data.m_Player.GetParent() ) )
			vehicle.SwitchGear();
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
}
