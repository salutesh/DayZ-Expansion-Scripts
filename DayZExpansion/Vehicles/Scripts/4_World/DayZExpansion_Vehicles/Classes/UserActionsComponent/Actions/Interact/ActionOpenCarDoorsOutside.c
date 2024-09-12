modded class ActionOpenCarDoorsOutside
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (!super.ActionCondition(player, target, item))
			return false;

		auto vehicle = ExpansionVehicle.Get(target.GetParent());
		if (vehicle && vehicle.IsLocked())
			return false;

		return true;
	}
};
