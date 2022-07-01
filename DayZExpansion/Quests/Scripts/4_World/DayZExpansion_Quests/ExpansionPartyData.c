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
	private bool m_ManualLeave;

	// ------------------------------------------------------------
	// Expansion RemoveMember
	// ------------------------------------------------------------
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

		ExpansionQuestModule questModule;
		if (!Class.CastTo(questModule,CF_ModuleCoreManager.Get(ExpansionQuestModule)))
		{
			Error(ToString() + "::OnJoin - Could not get quest module!");
			return;
		}

		string playerUID = player.GetID();
		ExpansionQuestPersistentData playerQuestData = questModule.GetPlayerQuestDataByUID(playerUID);
		if (!playerQuestData)
		{
			EXPrint(ToString() + "::OnJoin - No player quest data!");
			return;
		}
		
		questModule.AddPlayerGroupID(playerUID, player.GetParty().GetPartyID());

		for (int a = 0; a < questModule.GetActiveQuests().Count(); a++)
		{
			ExpansionQuest activeQuestInstance = questModule.GetActiveQuests()[a];
		#ifdef EXPANSIONMODQUESTSMODULEDEBUG
			Print(ToString() + "::OnJoin - Quest: " + activeQuestInstance);
		#endif
			if (activeQuestInstance.IsGroupQuest() && GetPartyID() == activeQuestInstance.GetGroupID())
			{
			#ifdef EXPANSIONMODQUESTSMODULEDEBUG
				Print(ToString() + "::OnJoin - There is a active group quest instance for this player! Add quest.");
			#endif
				//! Make sure player has the correct quest state for this quest in his quest data.
				if (playerQuestData.HasDataForQuest(activeQuestInstance.GetQuestConfig().GetID()))
					return;
				
				playerQuestData.AddQuestData(activeQuestInstance.GetQuestConfig());
				playerQuestData.UpdateQuestState(activeQuestInstance.GetQuestConfig().GetID(), activeQuestInstance.GetQuestState());
				questModule.UpdatePlayerQuestObjectiveData(activeQuestInstance, playerUID);
				activeQuestInstance.OnGroupMemberJoined(playerUID);
			}
		}

		PlayerBase playerBase = PlayerBase.GetPlayerByUID(playerUID);
		if (!playerBase)
		{
		#ifdef EXPANSIONMODQUESTSMODULEDEBUG
			Print(ToString() + "::OnJoin - Could not get player base. Player is offline!");
		#endif
			return;
		}

		questModule.SendPlayerQuestData(playerBase.GetIdentity());
	}

	//! We send all the group quests to the leaving member
	//! ToDo: Might want to check if the leaving player had already
	//! quest states for the quests that are active for this group so Well
	//! can recover these sates.
	override void OnLeave(ExpansionPartyPlayerData player)
	{
		super.OnLeave(player);

		if (!GetExpansionSettings().GetQuest().EnableQuests)
			return;

		ExpansionQuestModule questModule;
		if (!Class.CastTo(questModule,CF_ModuleCoreManager.Get(ExpansionQuestModule)))
		{
			Error(ToString() + "::OnLeave - Could not get quest module!");
			return;
		}

		string playerUID = player.GetID();
		ExpansionQuestPersistentData playerQuestData = questModule.GetPlayerQuestDataByUID(playerUID);
		if (!playerQuestData)
		{
			EXPrint(ToString() + "::OnLeave - No player quest data!");
			return;
		}

		for (int a = 0; a < questModule.GetActiveQuests().Count(); a++)
		{
			ExpansionQuest activeQuestInstance = questModule.GetActiveQuests()[a];
		#ifdef EXPANSIONMODQUESTSMODULEDEBUG
			Print(ToString() + "::OnLeave - Quest: " + activeQuestInstance);
		#endif
			if (activeQuestInstance.IsGroupQuest() && GetPartyID() == activeQuestInstance.GetGroupID())
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

		questModule.SendPlayerQuestData(playerBase.GetIdentity());
	}
};
#endif