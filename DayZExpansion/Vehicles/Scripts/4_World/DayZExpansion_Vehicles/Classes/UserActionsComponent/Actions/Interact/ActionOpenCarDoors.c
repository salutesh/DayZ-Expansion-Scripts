modded class ActionOpenCarDoors
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (!super.ActionCondition(player, target, item))
			return false;

		ExpansionVehicle vehicle;
		if (ExpansionVehicle.Get(vehicle, player) && vehicle.IsLocked())
			return false;

		return true;
	}
};