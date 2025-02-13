//! Only needed for hand animation
class eAIActionOpenDoorsDummy: ActionOpenDoors
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		return false;
	}
	
	override void OnStartServer(ActionData action_data)
	{
	}
}

//! Only needed for hand animation
class eAIActionCloseDoorsDummy: ActionCloseDoors
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		return false;
	}
	
	override void OnStartServer(ActionData action_data)
	{
	}
}