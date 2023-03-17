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
	protected bool m_ManualLeave;

	override bool RemoveMember(string uid)
	{
		m_ManualLeave = true;

		return super.RemoveMember(uid);
	}

	//! We send all the group quests to the new group member
	//! ToDo: Might want to check if the joned player has already
	//! completed the quests that are active for this group so he cant
	//! redo it again?!
	override void OnJoin(ExpansionPartyPlayerData player)
	{
		super.OnJoin(player);

		if (!GetExpansionSettings().GetQuest().EnableQuests)
			return;

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OnGroupMemberJoined, 1000, false, player);
	}

	protected void OnGroupMemberJoined(ExpansionPartyPlayerData player)
	{
		string playerUID = player.GetID();
		ExpansionQuestPersistentData playerQuestData = ExpansionQuestModule.GetModuleInstance().GetPlayerQuestDataByUID(playerUID);
		if (!playerQuestData)
		{
			Error(ToString() + "::OnGroupMemberJoined - No player quest data!");
			return;
		}

		array<ref ExpansionQuest> activeQuests = ExpansionQuestModule.GetModuleInstance().GetActiveQuests();
		foreach (ExpansionQuest activeQuestInstance: activeQuests)
		{
		#ifdef EXPANSIONMODQUESTSMODULEDEBUG
			Print(ToString() + "::OnGroupMemberJoined - Quest: " + activeQuestInstance);
		#endif
			if (activeQuestInstance.GetQuestConfig().IsGroupQuest() && GetPartyID() == activeQuestInstance.GetGroupID())
			{
			#ifdef EXPANSIONMODQUESTSMODULEDEBUG
				Print(ToString() + "::OnGroupMemberJoined - There is a active group quest instance for this player! Add quest.");
			#endif
				//! Make sure player has the correct quest state for this quest in his quest data.
				if (!playerQuestData.HasDataForQuest(activeQuestInstance.GetQuestConfig().GetID()))
					playerQuestData.AddQuestData(activeQuestInstance.GetQuestConfig().GetID(), activeQuestInstance.GetQuestState());

				activeQuestInstance.UpdateQuest(true);
				activeQuestInstance.OnGroupMemberJoined(playerUID);
			}
		}

		PlayerBase playerBase = PlayerBase.GetPlayerByUID(playerUID);
		if (!playerBase)
		{
		#ifdef EXPANSIONMODQUESTSMODULEDEBUG
			Print(ToString() + "::OnGroupMemberJoined - Could not get player base. Player is offline!");
		#endif
			return;
		}

		//! Add the group ID to the player in to our group list in the quest module.
		ExpansionQuestModule.GetModuleInstance().AddPlayerGroupID(playerUID, player.GetParty().GetPartyID());

		//! Send the updated persistent quest data to the client.
		ExpansionQuestModule.GetModuleInstance().SendClientQuestData(playerBase.GetIdentity());
	}

	//! We send all the group quests to the leaving member
	//! ToDo: Might want to check if the leaving player had already
	//! quest states for the quests that are active for this group so we
	//! can recover these sates.
	override void OnLeave(ExpansionPartyPlayerData player)
	{
		super.OnLeave(player);

		if (!GetExpansionSettings().GetQuest().EnableQuests)
			return;

		string playerUID = player.GetID();
		ExpansionQuestPersistentData playerQuestData = ExpansionQuestModule.GetModuleInstance().GetPlayerQuestDataByUID(playerUID);
		if (!playerQuestData)
		{
			Error(ToString() + "::OnLeave - No player quest data!");
			return;
		}

		array<ref ExpansionQuest> activeQuests = ExpansionQuestModule.GetModuleInstance().GetActiveQuests();
		foreach (ExpansionQuest activeQuestInstance: activeQuests)
		{
		#ifdef EXPANSIONMODQUESTSMODULEDEBUG
			Print(ToString() + "::OnLeave - Quest: " + activeQuestInstance);
		#endif
			if (activeQuestInstance.GetQuestConfig().IsGroupQuest() && GetPartyID() == activeQuestInstance.GetGroupID())
			{
			#ifdef EXPANSIONMODQUESTSMODULEDEBUG
				Print(ToString() + "::OnLeave - There is a active group quest instance for this player! Remove quest.");
			#endif

				if (m_ManualLeave)
				{
					playerQuestData.UpdateQuestState(activeQuestInstance.GetQuestConfig().GetID(), ExpansionQuestState.NONE);
					m_ManualLeave = false;
				}

				playerQuestData.Save(playerUID);
				activeQuestInstance.OnGroupMemberLeave(playerUID);
			}
		}

		PlayerBase playerBase = PlayerBase.GetPlayerByUID(playerUID);
		if (!playerBase)
		{
		#ifdef EXPANSIONMODQUESTSMODULEDEBUG
			Print(ToString() + "::OnLeave - Could not get player base. Player is offline!");
		#endif
			return;
		}

		//! Send the updated persistent quest data to the client.
		ExpansionQuestModule.GetModuleInstance().SendClientQuestData(playerBase.GetIdentity());
	}
};
#endif
