modded class ActionOpenCarDoors
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		CarScript car;
		//! player inside vehicle
		if ( player && player.GetCommand_ExpansionVehicle() )
		{
			return super.ActionCondition(player, target, item);
		} else
		{
			//! reach check from outside of vehicle
			if ( !IsInReach(player, target, UAMaxDistances.DEFAULT) )
				return false;

			//! player is outside of vehicle
			if ( Class.CastTo(car, target.GetParent()) )
			{
				array<string> selections = new array<string>();

				if( car.IsLocked() )
					return false;
			}
		}

		return super.ActionCondition(player, target, item);
	}
};