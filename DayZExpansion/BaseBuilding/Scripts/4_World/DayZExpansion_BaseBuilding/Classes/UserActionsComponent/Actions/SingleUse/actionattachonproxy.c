modded class ActionAttachOnProxy
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		ActionInteractCodeLockItem.Expansion_SetCodeLockTargetForBB(target);
		return super.ActionCondition(player, target, item);
	}
}
