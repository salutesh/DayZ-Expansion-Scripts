modded class ActionInteractCodeLockItem
{
	override void CreateConditionComponents()
	{
		super.CreateConditionComponents();

		m_ConditionTarget = new CCTCursorInherited({ItemBase}, UAMaxDistances.DEFAULT);
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!Expansion_SetCodeLockTargetForBB(target))
			return false;

		return super.ActionCondition(player, target, item);
	}

	static DigitalCodeLock Expansion_GetCodeLockForBB(Object tgtObj, int component)
	{
		ExpansionBaseBuilding exbb;
		if (Class.CastTo(exbb, tgtObj))
		{
			DigitalCodeLock codeLock;
			if (Class.CastTo(codeLock, exbb.Expansion_GetCodeLock(component)))
				return codeLock;
		}

		return null;
	}

	static bool Expansion_SetCodeLockTargetForBB(ActionTarget target)
	{
		Object tgtObj = target.GetObject();
		if (tgtObj.IsInherited(DigitalCodeLock))
			return true;

		DigitalCodeLock codeLock = Expansion_GetCodeLockForBB(tgtObj, target.GetComponentIndex());
		if (codeLock)
		{
			target.Expansion_SetObject(codeLock);
			return true;
		}

		return false;
	}
}
