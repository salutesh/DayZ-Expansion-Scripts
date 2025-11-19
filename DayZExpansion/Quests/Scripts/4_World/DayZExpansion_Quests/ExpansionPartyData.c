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
	//! With the ExpansionQuestModule::QuestDisplayConditions method we check if the joining player can participate on each active group quest for this group. 
	override void OnJoin(ExpansionPartyPlayerData player)
	{
		super.OnJoin(player);

		if (!GetExpansionSettings().GetQuest().EnableQuests)
			return;

		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OnJoin_Deferred, 1000, false, player);
	}

	protected void OnJoin_Deferred(ExpansionPartyPlayerData player)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif
		
		ExpansionQuestModule.GetModuleInstance().LoadGroupQuestData(PartyID.ToString());
		
		string playerUID = player.GetID();
		ExpansionQuestPersistentData playerQuestData = ExpansionQuestModule.GetModuleInstance().GetPlayerQuestDataByUID(playerUID);
		if (!playerQuestData)
		{
			Error(ToString() + "::OnJoin_Deferred - Could not get persistent quest data for player with UID: " + playerUID);
			return;
		}
		
		PlayerBase playerPB = PlayerBase.GetPlayerByUID(playerUID);
		if (!playerPB)
		{
			Error(ToString() + "::OnJoin_Deferred - Could not get player instance with UID: " + playerUID);
			return;
		}
		
		map<int, ref ExpansionQuest> activeQuests = ExpansionQuestModule.GetModuleInstance().GetActiveQuests(GetPartyID().ToString());
		foreach (ExpansionQuest activeQuestInstance: activeQuests)
		{
			if (ExpansionQuestModule.GetModuleInstance().QuestDisplayConditions(activeQuestInstance.GetQuestConfig(), playerPB, playerQuestData, -1, true))
				activeQuestInstance.OnGroupMemberJoined(playerUID);
		}

		//! Track group membership during this session to prevent players circumventing group restrictions by temporarily leaving their group
		//! (e.g. target objectives that require player kills)
		ExpansionQuestModule.GetModuleInstance().AddPlayerGroupID(playerUID, PartyID);
	}

	//! We send all the group quests to the leaving member
	//! ToDo: Might want to check if the leaving player had already
	//! quest states for the quests that are active for this group so we
	//! can recover these sates.
	override void OnLeave(ExpansionPartyPlayerData player)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		super.OnLeave(player);

		if (!GetExpansionSettings().GetQuest().EnableQuests)
			return;

		string playerUID = player.GetID();

		map<int, ref ExpansionQuest> activeQuests = ExpansionQuestModule.GetModuleInstance().GetActiveQuests(GetPartyID().ToString());
		foreach (ExpansionQuest activeQuestInstance: activeQuests)
		{
			if (activeQuestInstance.IsQuestPlayer(playerUID))
				activeQuestInstance.OnGroupMemberLeave(playerUID);
		}
	}
};
#endif
