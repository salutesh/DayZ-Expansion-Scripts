#ifndef DAYZ_1_18
//! 1.19
modded class ActionCarHornBase
{
	override static bool ActionCondition(PlayerBase player)
	{
		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();

		if (vehCommand)
		{
			Transport trans = vehCommand.GetTransport();
			if (trans)
			{
				if (!PlayerIsDriver(trans, player))
				{
					return false;
				}

				CarScript car;
				if (Class.CastTo(car, trans))
				{
					return BatteryIsVital(car.GetBattery()) && car.Expansion_HasVehicleHorn();
				}
			}
		}
		
		return false;
	}
};

modded class ActionCarHornShort
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!super.ActionCondition(player, target, item))
			return false;

		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();

		if (vehCommand)
		{
			CarScript car;
			if (Class.CastTo(car, vehCommand.GetTransport()))
			{
				return car.Expansion_HasVehicleHornShort();
			}
		}

		return true;
	}
}
#endif