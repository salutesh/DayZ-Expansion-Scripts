modded class ActionOpenCarDoorsOutside
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (!super.ActionCondition(player, target, item))
			return false;

		CarScript car;
		if (Class.CastTo(car, target.GetParent()) && car.IsLocked())
			return false;

		return true;
	}
};
