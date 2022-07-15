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

class ExpansionScriptedtQuestTemplate extends ExpansionQuest
{
	//! Event called when a quest is stated
	override void OnQuestStart()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnQuestStart").Add(sender).Add(ctx);
	#endif

		QuestPrint(ToString() + "::OnQuestStart - Start");

		super.OnQuestStart();
		
		QuestPrint(ToString() + "::OnQuestStart - End");
	}

	//! Event called when ever a quest is completed and turned-in
	override void OnQuestTurnIn(ExpansionQuestRewardConfig reward = null)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnQuestTurnIn").Add(sender).Add(ctx);
	#endif

		QuestPrint(ToString() + "::OnQuestTurnIn - Start");

		super.OnQuestTurnIn(reward);

		QuestPrint(ToString() + "::OnQuestTurnIn - End");
	}

	//! Event called when a quest is manualy canceled by the player
	override void OnQuestCancel()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnQuestCancel").Add(sender).Add(ctx);
	#endif

		QuestPrint(ToString() + "::OnQuestCancel - Start");

		super.OnQuestCancel();

		QuestPrint(ToString() + "::OnQuestCancel - End");
	}

	//! Event called when a quest gets recreated (reloaded because the player reloged/disconnected)
	override void OnQuestContinue()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnQuestContinue").Add(sender).Add(ctx);
	#endif

		QuestPrint(ToString() + "::OnQuestContinue - Start");

		super.OnQuestContinue();

		QuestPrint(ToString() + "::OnQuestContinue - End");
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
	}
};