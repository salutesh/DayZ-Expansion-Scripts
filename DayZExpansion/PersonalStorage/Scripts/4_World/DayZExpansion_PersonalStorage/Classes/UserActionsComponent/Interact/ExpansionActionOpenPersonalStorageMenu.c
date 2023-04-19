/**
 * ExpansionActionOpenPersonalStorageMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionOpenPersonalStorageMenu: ActionInteractBase
{
	void ExpansionActionOpenPersonalStorageMenu()
	{
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
		m_Text = "Open Storage";
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		ExpansionPersonalStorageBase storage = ExpansionPersonalStorageBase.Cast(target.GetObject());
		if (!storage)
			return false;
		
		auto settings = GetExpansionSettings().GetPersonalStorage(false);
		if (!settings.IsLoaded())
			return false;

		if (!settings.Enabled)
			return false;

		return true;
	}
	
	override void OnExecuteServer(ActionData action_data)
	{
		super.OnExecuteServer(action_data);

		ExpansionPersonalStorageBase storage = ExpansionPersonalStorageBase.Cast(action_data.m_Target.GetObject());
		if (!storage)
			return;

		if (!ExpansionPersonalStorageModule.GetModuleInstance())
			return;
		
		int storageID = storage.GetStorageID();
		Print(ToString() + "::OnExecuteServer - Storage ID: " + storageID);
		ExpansionPersonalStorageConfig storageConfig = ExpansionPersonalStorageModule.GetModuleInstance().GetPersonalStorageConfigByID(storageID);
		if (!storageConfig)
		{
			Error(ToString() + "::OnExecuteServer - Could not get storage data!");
			return;
		}

		PlayerBase player = action_data.m_Player;
		if (!player || !player.GetIdentity())
		{
			Error(ToString() + "::OnExecuteServer - Could not get player!");
			return;
		}
		
	#ifdef EXPANSIONMODQUESTS
		if (GetExpansionSettings().GetQuest().EnableQuests)
		{
			int questID = storageConfig.GetQuestID();
			Print(ToString() + "::OnExecuteServer - Need to complete quest with ID: " + questID);
			if (questID > -1)
			{
				//! Check if player has completed required quest
				if (!ExpansionQuestModule.GetModuleInstance().HasCompletedQuest(questID, player.GetIdentity().GetId()))
				{
					ExpansionQuestConfig questConig = ExpansionQuestModule.GetModuleInstance().GetQuestConfigByID(questID);
					if (!questConig)
					{
						Error(ToString() + "::OnExecuteServer - Could not get quest config for quest ID: " + questID);
						return;
					}
		
					ExpansionNotification(new StringLocaliser("Storage is locked!"), new StringLocaliser("You have no access to this storage yet. You need to compelete the quest " + questConig.GetTitle() + " first to use this storage."), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_AMETHYST, 10, ExpansionNotificationType.TOAST).Create(player.GetIdentity());
					return;
				}
			}
		}
	#endif
		
	#ifdef EXPANSIONMODHARDLINE
		if (GetExpansionSettings().GetHardline().UseReputation)
		{
			int reputationRequirement = storageConfig.GetReputation();
			Print(ToString() + "::OnExecuteServer - Need to have reputation: " + reputationRequirement);
			if (reputationRequirement > 0)
			{
				int reputation = player.Expansion_GetReputation();
				Print(ToString() + "::OnExecuteServer - Player reputation: " + reputation);
				if (reputation < reputationRequirement)
				{
					ExpansionNotification(new StringLocaliser("Storage is locked!"), new StringLocaliser("You have no access to this storage yet. You need at least " + reputationRequirement + " reputation points first to use this storage."), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_AMETHYST, 10, ExpansionNotificationType.TOAST).Create(player.GetIdentity());
					return;
				}
			}
		}
	#endif
		
	#ifdef EXPANSIONMODAI
		string factionName = storageConfig.GetFaction();
		bool isInFaction;
		bool isInInOtherFaction;
		Print(ToString() + "::OnExecuteServer - Need to be in faction: " + factionName);
		if (factionName != string.Empty)
		{
			eAIGroup group = player.GetGroup();
			if (!group)
				group = eAIGroup.GetGroupByLeader(player);
			
			Print(ToString() + "::OnExecuteServer - Player group: " + group.ToString());
			if (group)
			{
				eAIFaction playerFaction = group.GetFaction();
				Print(ToString() + "::OnExecuteServer - Player faction: " + playerFaction.ToString());
				if (playerFaction)
				{
					string playerFactionName = playerFaction.GetName();
					Print(ToString() + "::OnExecuteServer - Player faction name: " + playerFactionName);
					if (playerFactionName == factionName)
					{
						isInFaction = true;
					}
					else
					{
						if (playerFactionName != string.Empty)
							isInInOtherFaction = true;
					}
				}
			}
			
			if (!isInFaction)
			{
				string message;
				if (isInInOtherFaction)
					message = "You have no access to this storage. You need to be a member of the " + factionName + " faction. You are a member of the " + playerFactionName + " faction.";
				else
					message = "You have no access to this storage. You need to be a member of the " + factionName + " faction.";
				
				ExpansionNotification(new StringLocaliser("Storage is locked!"), new StringLocaliser(message), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_AMETHYST, 10, ExpansionNotificationType.TOAST).Create(player.GetIdentity());
				return;
			}
		}
	#endif
		
		string displayName;
		string displayIcon;

		storage.m_Expansion_NetsyncData.Get(0, displayName);
		storage.m_Expansion_NetsyncData.Get(1, displayIcon);

		ExpansionPersonalStorageModule.GetModuleInstance().SendItemData(player.GetIdentity(), storageID, displayName, displayIcon);
	}
};
