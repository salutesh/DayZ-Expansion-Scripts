modded class ActionOpenCarDoors
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (!super.ActionCondition(player, target, item))
			return false;

		auto hcv = player.GetCommand_Vehicle();
		CarScript car;
		if (hcv && Class.CastTo(car, hcv.GetTransport()) && car.IsLocked())
			return false;

		return true;
	}
};