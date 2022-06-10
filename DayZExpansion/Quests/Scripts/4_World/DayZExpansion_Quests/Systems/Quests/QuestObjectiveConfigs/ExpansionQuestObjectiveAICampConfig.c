/**
 * ExpansionQuestObjectiveAICampConfig.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODAI
class ExpansionQuestObjectiveAICampConfig: ExpansionQuestObjectiveConfigBase
{
	private ref ExpansionQuestObjectiveAICamp AICamp;

	void SetAICamp(ExpansionQuestObjectiveAICamp camp)
	{
		AICamp = camp;
	}

	override ExpansionQuestObjectiveAICamp GetAICamp()
	{
		return AICamp;
	}

	override void Save(string fileName)
	{
		JsonFileLoader<ExpansionQuestObjectiveAICampConfig>.JsonSaveFile(EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER + fileName + ".JSON", this);
	}

	override void OnSend(ParamsWriteContext ctx)
	{
		super.OnSend(ctx);

		AICamp.OnSend(ctx);
	}

	override bool OnRecieve(ParamsReadContext ctx)
	{
		if (!super.OnRecieve(ctx))
			return false;

		if (!AICamp)
			AICamp = new ExpansionQuestObjectiveAICamp();

		if (!AICamp.OnRecieve(ctx))
			return false;

		return true;
	}

	override void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		if (AICamp)
			AICamp.QuestDebug();
	#endif
	}
};
#endif