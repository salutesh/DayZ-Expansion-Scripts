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

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		auto vehicle = ExpansionVehicle.Get(player);
		if (!vehicle)
			return false;

		if (!vehicle.IsPlayerInSeat(player, DayZPlayerConstants.VEHICLESEAT_DRIVER))
			return false;

		if (!vehicle.HasGear())
			return false;

		return false;
	}

	override void OnExecuteServer( ActionData action_data )
	{
		auto vehicle = ExpansionVehicle.Get(action_data.m_Player);
		vehicle.SwitchGear();
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
}