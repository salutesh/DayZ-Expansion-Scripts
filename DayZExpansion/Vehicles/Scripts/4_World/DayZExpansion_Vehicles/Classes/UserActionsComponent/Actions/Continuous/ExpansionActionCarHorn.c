/**
 * ActionCarHorn.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionActionCarHornCB
 * @brief		
 **/
#ifdef DAYZ_1_18
class ExpansionActionCarHornCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousRepeat(0.25);
	}
};

/**@class		ExpansionActionCarHorn
 * @brief		
 **/
[RegisterAction(ExpansionActionCarHorn)]
class ExpansionActionCarHorn : ActionContinuousBase
{
	void ExpansionActionCarHorn()
	{
		m_CallbackClass = ExpansionActionCarHornCB;

		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_TOOTHORN;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_SpecialtyWeight = 0.5;
		m_LockTargetOnUse = false;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override typename GetInputType()
	{
		return ExpansionInputCarHorn;
	}

	override string GetText()
	{
		return "#STR_USRACT_USE_CARHORN";
	}

	override bool CanBeUsedInVehicle()
	{
		return true;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		ItemBase battery;

		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();
		CarScript car;
		if (vehCommand && Class.CastTo(car, vehCommand.GetTransport()))
		{
			if (vehCommand.GetVehicleSeat() != DayZPlayerConstants.VEHICLESEAT_DRIVER)
				return false;

			return car.IsBatteryWorking();
		}

		ExpansionHumanCommandVehicle expVehCommand = player.GetCommand_ExpansionVehicle();
		ExpansionVehicleBase vehicle;
		if (expVehCommand && Class.CastTo(vehicle, expVehCommand.GetExpansionVehicle()))
		{
			if (expVehCommand.GetVehicleSeat() != DayZPlayerConstants.VEHICLESEAT_DRIVER)
				return false;

			return vehicle.IsBatteryWorking();
		}

		return false;
	}

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);

		CarScript car;
		if (Class.CastTo(car, action_data.m_Player.GetParent()))
		{
			car.PlayHorn();
			return;
		}

		ExpansionVehicleBase vehicle;
		if (Class.CastTo(vehicle, action_data.m_Player.GetParent()))
		{
			vehicle.PlayHorn();
			return;
		}
	}

	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);

		CarScript car;
		if (Class.CastTo(car, action_data.m_Player.GetParent()))
		{
			car.StopHorn();
			return;
		}

		ExpansionVehicleBase vehicle;
		if (Class.CastTo(vehicle, action_data.m_Player.GetParent()))
		{
			vehicle.StopHorn();
			return;
		}
	}
};
#endif
