/**
 * ExpansionQuestObjectiveCraftingConfig.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveCraftingConfig: ExpansionQuestObjectiveConfig
{
	ref array<string> ItemNames = new array<string>;

	void AddItemName(string name)
	{
		ItemNames.Insert(name);
	}

	override array<string> GetItemNames()
	{
		return ItemNames;
	}

	override void Save(string fileName)
	{
		if (!ExpansionString.EndsWithIgnoreCase(fileName, ".json"))
			fileName += ".json";
	
		ExpansionJsonFileParser<ExpansionQuestObjectiveCraftingConfig>.Save(EXPANSION_QUESTS_OBJECTIVES_CRAFTING_FOLDER + fileName, this);
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

	override bool Validate()
	{
		if (!super.Validate())
			return false;

		if (!ItemNames || ItemNames && ItemNames.Count() == 0)
			return false;

		return true;
	}
};