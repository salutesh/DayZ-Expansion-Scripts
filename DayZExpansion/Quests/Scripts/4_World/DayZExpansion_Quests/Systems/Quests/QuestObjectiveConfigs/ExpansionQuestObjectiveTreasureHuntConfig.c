/**
 * ExpansionQuestObjectiveTreasureHuntConfig.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveTreasureHuntConfig: ExpansionQuestObjectiveConfigBase
{
	private ref ExpansionQuestObjectiveTreasureHunt TreasureHunt;

	void SetTreasureHunt(ExpansionQuestObjectiveTreasureHunt hunt)
	{
		TreasureHunt = hunt;
	}

	override ExpansionQuestObjectiveTreasureHunt GetTreasureHunt()
	{
		return TreasureHunt;
	}

	override void Save(string fileName)
	{
		JsonFileLoader<ExpansionQuestObjectiveTreasureHuntConfig>.JsonSaveFile(EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER + fileName + ".JSON", this);
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
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		if (TreasureHunt)
			TreasureHunt.QuestDebug();
	#endif
	}
};