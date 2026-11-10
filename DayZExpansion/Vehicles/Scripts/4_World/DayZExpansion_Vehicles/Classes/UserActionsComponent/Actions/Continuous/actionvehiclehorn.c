modded class ActionVehicleHornLong
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!super.ActionCondition(player, target, item))
			return false;

		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();

		if (vehCommand)
		{
			Transport transport = vehCommand.GetTransport();
			if (transport)
			{
				return VehicleHornComponent.Cast(transport.GetVehicleHornComponent()).Expansion_HasSound(VehicleHornMode.LONG);
			}
		}

		return true;
	}
};

modded class ActionVehicleHornShort
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!super.ActionCondition(player, target, item))
			return false;

		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();

		if (vehCommand)
		{
			Transport transport = vehCommand.GetTransport();
			if (transport)
			{
				return VehicleHornComponent.Cast(transport.GetVehicleHornComponent()).Expansion_HasSound(VehicleHornMode.SHORT);
			}
		}

		return true;
	}
}
