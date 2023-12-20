//! Fix vanilla unnecessarily creating network overhead for what should be a simple move operation
modded class DigOutStashLambda
{
	override void Execute(HumanInventoryWithFSM fsm_to_notify = null)
	{
		array<EntityAI> children = new array<EntityAI>;
		m_OldItem.GetInventory().EnumerateInventory(InventoryTraversalType.LEVELORDER, children);
		foreach (EntityAI child : children)
		{
			if (child)
			{
				if (!m_OldItem.GetInventory().DropEntity(InventoryMode.SERVER, m_OldItem, child))
				{
					OnAbort();
					return;
				}
			}
		}

		DeleteOldEntity();

		OnSuccess(null);
	}
}
