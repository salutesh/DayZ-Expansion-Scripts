modded class ActionAttachOnProxy
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		ActionInteractCodeLockItem.Expansion_SetCodeLockTargetForBB(target);
		return super.ActionCondition(player, target, item);
	}

#ifndef SERVER
	override void AttachItem(AttachActionData action_data)
	{
		super.AttachItem(action_data);

		if (action_data.m_MainItem.Expansion_IsCodeLock())
		{
			int component = action_data.m_Target.GetComponentIndex();
			string selection = action_data.m_Target.GetObject().GetActionComponentName(component);
			g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ExpansionCodeLockUI.Open, 250, false, action_data.m_Target.GetParentOrObject(), component, selection, false, true);
		}
	}
#endif
}
