/**
 * ExpansionPartyData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/


#ifdef EXPANSIONMODGROUPS
modded class ExpansionPartyData
{
	//! We send all the group quests to the new group member
	//! ToDo: Might want to check if the joned player has already
	//! completed the quests that are active for this group so he cant
	//! redo it again?!
	override void OnJoin(ExpansionPartyPlayerData player)
	{
		super.OnJoin(player);

		if (!GetExpansionSettings().GetQuest().EnableQuests)
			return;

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OnJoin_Deferred, 1000, false, player);
	}

	protected void OnJoin_Deferred(ExpansionPartyPlayerData player)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		string playerUID = player.GetID();

		map<int, ref ExpansionQuest> activeQuests = ExpansionQuestModule.GetModuleInstance().GetActiveQuests(GetPartyID().ToString());
		foreach (ExpansionQuest activeQuestInstance: activeQuests)
		{
			activeQuestInstance.OnGroupMemberJoined(playerUID);
		}
	}

	//! We send all the group quests to the leaving member
	//! ToDo: Might want to check if the leaving player had already
	//! quest states for the quests that are active for this group so we
	//! can recover these sates.
	override void OnLeave(ExpansionPartyPlayerData player)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		super.OnLeave(player);

		if (!GetExpansionSettings().GetQuest().EnableQuests)
			return;

		string playerUID = player.GetID();

		map<int, ref ExpansionQuest> activeQuests = ExpansionQuestModule.GetModuleInstance().GetActiveQuests(GetPartyID().ToString());
		foreach (ExpansionQuest activeQuestInstance: activeQuests)
		{
			activeQuestInstance.OnGroupMemberLeave(playerUID);
		}
	}
};
#endif
