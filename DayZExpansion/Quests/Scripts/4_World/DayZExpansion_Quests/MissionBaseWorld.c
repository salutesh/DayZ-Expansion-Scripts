/**
 * MissionBaseWorld.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class MissionBaseWorld
{
	void Expansion_OnQuestPlayerInit(ExpansionQuestPersistentData playerQuestData, PlayerIdentity identity);
	
	//! This being here allows overriding in init.c
	bool Expansion_CanStartQuest(ExpansionQuestConfig questConfig, PlayerIdentity identity)
	{
		return true;
	}
	
	void Expansion_OnQuestStart(ExpansionQuest quest);

	void Expansion_OnQuestContinue(ExpansionQuest quest);
	
	void Expansion_OnObjectiveTimeLimitReached(ExpansionQuestObjectiveEventBase objectiveEventBase);

	void Expansion_OnQuestCancel(ExpansionQuest quest);

	bool Expansion_CanCompleteQuest(ExpansionQuest quest, PlayerIdentity identity)
	{
		return quest.CanCompleteQuest();
	}
	
	void Expansion_OnQuestObjectivesComplete(ExpansionQuest quest);
	
	void Expansion_OnQuestObjectivesIncomplete(ExpansionQuest quest);

	void Expansion_OnQuestCompletion(ExpansionQuest quest);
}
