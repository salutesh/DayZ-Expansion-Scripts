modded class ActionSkinning
{
	override bool Can(PlayerBase player, ActionTarget target, ItemBase item, int condition_mask)
	{
		if (super.Can(player, target, item, condition_mask) || player.Expansion_IsAI())
			return true;

		return false;
	}

	override protected bool CanContinue(ActionData action_data)
	{
		if (super.CanContinue(action_data) || action_data.m_Player.Expansion_IsAI())
			return true;

		return false;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (super.ActionCondition(player, target, item) || player.Expansion_IsAI())
			return true;

		return false;
	}
}
