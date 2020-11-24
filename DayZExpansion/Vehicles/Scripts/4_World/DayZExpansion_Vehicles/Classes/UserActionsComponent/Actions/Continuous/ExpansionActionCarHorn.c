/**
 * ActionCarHorn.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionActionCarHornCB
 * @brief		
 **/
class ExpansionActionCarHornCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousRepeat(0);
	}
}

/**@class		ExpansionActionCarHorn
 * @brief		
 **/
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
		if (!player)
			return false;

		ItemBase battery;

		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();
		CarScript car;
		if (vehCommand && Class.CastTo(car, vehCommand.GetTransport()))
		{
			if (vehCommand.GetVehicleSeat() != DayZPlayerConstants.VEHICLESEAT_DRIVER)
				return false;

			if (car.IsVitalCarBattery())
			{
				battery = ItemBase.Cast(car.FindAttachmentBySlotName("CarBattery"));

				if (!battery || battery.IsRuined())
				{
					return false;
				}
			}
			else if (car.IsVitalTruckBattery())
			{
				battery = ItemBase.Cast(car.FindAttachmentBySlotName("TruckBattery"));

				if (!battery || battery.IsRuined())
				{
					return false;
				}
			}
			else
			{
				return false;
			}

			return true;
		}

		ExpansionHumanCommandVehicle expVehCommand = player.GetCommand_ExpansionVehicle();
		ExpansionVehicleBase vehicle;
		if (expVehCommand && Class.CastTo(vehicle, expVehCommand.GetTransport()))
		{
			if (expVehCommand.GetVehicleSeat() != DayZPlayerConstants.VEHICLESEAT_DRIVER)
				return false;

			if (vehicle.IsVitalCarBattery())
			{
				battery = ItemBase.Cast(vehicle.FindAttachmentBySlotName("CarBattery"));

				if (!battery || battery.IsRuined())
				{
					return false;
				}
			}
			else if (vehicle.IsVitalTruckBattery())
			{
				battery = ItemBase.Cast(vehicle.FindAttachmentBySlotName("TruckBattery"));

				if (!battery || battery.IsRuined())
				{
					return false;
				}
			}
			else
			{
				return false;
			}

			return true;
		}

		return false;
	}

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);

		CarScript car;
		if (Class.CastTo(car, action_data.m_Player.GetParent()))
		{
			car.PlayHonk();
			return;
		}

		ExpansionVehicleBase vehicle;
		if (Class.CastTo(vehicle, action_data.m_Player.GetParent()))
		{
			vehicle.PlayHonk();
			return;
		}
	}

	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);

		CarScript car;
		if (Class.CastTo(car, action_data.m_Player.GetParent()))
		{
			car.StopHonk();
			return;
		}

		ExpansionVehicleBase vehicle;
		if (Class.CastTo(vehicle, action_data.m_Player.GetParent()))
		{
			vehicle.StopHonk();
			return;
		}
	}
}