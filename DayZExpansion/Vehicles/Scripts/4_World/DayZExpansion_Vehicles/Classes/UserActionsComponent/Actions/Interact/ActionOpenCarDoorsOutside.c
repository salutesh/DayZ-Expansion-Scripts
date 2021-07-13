modded class ActionOpenCarDoorsOutside
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		CarScript car = null;
        //! player is outside of vehicle
        if ( Class.CastTo(car, target.GetParent()) )
        {
            array<string> selections = new array<string>();

            if( car.IsLocked() )
                return false;
        }

		return super.ActionCondition(player, target, item);
	}
};
