modded class ActionConstructor
{
	override void RegisterActions(TTypenameArray actions)
	{
		super.RegisterActions(actions);

		actions.Insert(eAIActionWeaponControl);
		actions.Insert(eAIActionWeaponUnjam);
		actions.Insert(eAIActionTakeItemToHands);
		actions.Insert(eAIActionTakeItem);
		actions.Insert(eAIActionDropItem);

		actions.Insert(ExpansionActionRecruitAI);
		actions.Insert(ExpansionActionDismissAI);
	}
};
