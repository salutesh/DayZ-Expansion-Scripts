/**
 * ExpansionQuestObjectiveActionConfig.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveActionConfig: ExpansionQuestObjectiveConfigBase
{
	ref array<string> ActionNames = new array<string>;

	void AddActionName(string name)
	{
		ActionNames.Insert(name);
	}

	override array<string> GetActionNames()
	{
		return ActionNames;
	}

	override void Save(string fileName)
	{
		JsonFileLoader<ExpansionQuestObjectiveActionConfig>.JsonSaveFile(EXPANSION_QUESTS_OBJECTIVES_ACTION_FOLDER + fileName + ".JSON", this);
	}

	override void OnSend(ParamsWriteContext ctx)
	{
		super.OnSend(ctx);
	}

	override bool OnRecieve(ParamsReadContext ctx)
	{
		if (!super.OnRecieve(ctx))
			return false;

		return true;
	}
	
	override void QuestDebug()
	{

	}
};