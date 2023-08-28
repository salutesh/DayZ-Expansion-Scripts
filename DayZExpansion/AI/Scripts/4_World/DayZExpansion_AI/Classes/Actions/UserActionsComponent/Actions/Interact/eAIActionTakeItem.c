class eAIActionTakeItem: ActionInteractBase
{
	string m_ItemName = "";

	void eAIActionTakeItem()
	{
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_INVENTORY;
		m_CommandUIDProne	= DayZPlayerConstants.CMD_ACTIONFB_PICKUP_INVENTORY;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTObject(UAMaxDistances.DEFAULT);
	}
	
	override bool HasProneException()
	{
		return true;
	}

	override bool ActionConditionContinue( ActionData action_data )
	{
		return true;
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		return false;
	}
	
	override bool CanBeUsedOnBack()
	{
		return true;
	}
	
	override bool InventoryReservation(ActionData action_data)
	{
		bool success = true;
		
		InventoryLocation il = new InventoryLocation;
		
		ItemBase targetItem;
		if ( ItemBase.CastTo(targetItem, action_data.m_Target.GetObject()) )
		{
			action_data.m_Player.GetInventory().FindFreeLocationFor( targetItem , FindInventoryLocationType.ANY, il );
			if ( action_data.m_Player.GetInventory().HasInventoryReservation( targetItem, il) )
			{
				success = false;
			}
			else
			{
				action_data.m_Player.GetInventory().AddInventoryReservationEx( targetItem, il, GameInventory.c_InventoryReservationTimeoutMS);
			}
		}	
		
		if ( success )
		{
			if( il )
				action_data.m_ReservedInventoryLocations.Insert(il);
		}
		
		return success;
	}
	
	override void OnExecuteServer( ActionData action_data )
	{
		ItemBase ntarget = ItemBase.Cast(action_data.m_Target.GetObject());

		if (!ntarget)
			return;
		
		float stackable = ntarget.GetTargetQuantityMax(-1);
		
		if( stackable == 0 || stackable >= ntarget.GetQuantity() )
		{
			eAIBase ai = eAIBase.Cast(action_data.m_Player);
			ai.eAI_TakeItemToInventoryImpl(ntarget);
		}
		else if (InventoryReservation(action_data))
		{
			InventoryLocation il = action_data.m_ReservedInventoryLocations.Get(0);
			ClearInventoryReservationEx(action_data);
			ntarget.SplitIntoStackMaxToInventoryLocationClient( il );
		}
	}
}
