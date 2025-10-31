modded class ActionWorldCraft
{
	override bool Post_SetupAction(ActionData action_data)
	{
		auto actionDataWorldCraft = WorldCraftActionData.Cast(action_data);

		if (actionDataWorldCraft && action_data.m_Player.Expansion_IsAI())
			actionDataWorldCraft.m_RecipeID = action_data.m_Player.GetCraftingRecipeID();

		return super.Post_SetupAction(action_data);
	}
}
