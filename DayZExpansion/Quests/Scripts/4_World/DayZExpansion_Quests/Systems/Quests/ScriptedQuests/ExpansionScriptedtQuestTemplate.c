/**
 * ExpansionScriptedtQuestTemplate.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionScriptedtQuestTemplate: ExpansionQuest
{
	//! Event called when a quest is stated
	override bool OnQuestStart()
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		return super.OnQuestStart();
	}

	//! Event called when ever a quest is completed and turned-in
	override bool OnQuestTurnIn(string playerUID, ExpansionQuestRewardConfig reward = null, int selectedObjItemIndex = -1)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		return super.OnQuestTurnIn(playerUID, reward);
	}

	//! Event called when a quest is manualy canceled by the player
	override bool OnQuestCancel()
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		return super.OnQuestCancel();
	}

	//! Event called when a quest gets recreated (reloaded because the player reloged/disconnected)
	override bool OnQuestContinue()
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		return super.OnQuestContinue();
	}
};