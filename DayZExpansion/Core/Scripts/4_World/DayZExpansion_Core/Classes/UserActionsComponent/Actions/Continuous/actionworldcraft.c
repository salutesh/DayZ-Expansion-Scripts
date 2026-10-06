modded class ActionWorldCraft
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!super.ActionCondition(player, target, item))
			return false;

		//! Do not allow any recipes on items in owned containers that are set to not receive items
		ItemBase targetItem;
		if (Class.CastTo(targetItem, target.GetObject()))
		{
			EntityAI hierarchyParent = targetItem.GetHierarchyParent();
			if (hierarchyParent && hierarchyParent.IsInherited(ExpansionOwnedContainer))
			{
				InventoryLocation lcn = new InventoryLocation();
				targetItem.GetInventory().GetCurrentInventoryLocation(lcn);
				if (lcn.GetType() == InventoryLocationType.CARGO)
				{
					if (!hierarchyParent.CanReceiveItemIntoCargo(targetItem))
						return false;
				}
				else
				{
					if (!hierarchyParent.CanReceiveAttachment(targetItem, lcn.GetSlot()))
						return false;
				}
			}
		}

		return true;
	}
}
