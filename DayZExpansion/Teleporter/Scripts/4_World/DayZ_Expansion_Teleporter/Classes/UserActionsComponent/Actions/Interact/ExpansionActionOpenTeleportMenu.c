/**
 * ExpansionActionOpenTeleportMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionOpenTeleportMenu: ActionInteractBase
{
	protected ExpansionTeleporterModule m_Expansion_TeleporterModule;

	void ExpansionActionOpenTeleportMenu()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}

	override typename GetInputType()
	{
		return InteractActionInput;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		Object targetObject;
		if (!Class.CastTo(targetObject, target.GetParentOrObject()))
			return false;

		Expansion_Teleporter_Base teleportObj = Expansion_Teleporter_Base.Cast(targetObject);
		if (!teleportObj || teleportObj && !teleportObj.IsActive())
			return false;

		if (!GetGame().IsDedicatedServer())
		{
			//! Client
			//! Dont show action if menu is already opened
			if (GetDayZGame().GetExpansionGame().GetExpansionUIManager().GetMenu())
				return false;

			string actionText = "#use";
			m_Text = actionText + " " + targetObject.GetDisplayName();
		}

		return true;
	}

	/**
	 * Client/server handshake
	 *
	 * Server: ExpansionActionOpenTeleportMenu::OnExecuteServer
	 * Server: ExpansionTeleporterModule::RequestOpenTeleporterMenu
	 * Client: ExpansionTeleporterModule::RPC_RequestOpenTeleporterMenu
	 * Client: Invoke ExpansionTeleporterMenu::SetTeleportLocations
	 **/
	override void OnExecuteServer(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		super.OnExecuteServer(action_data);

		PlayerBase player = action_data.m_Player;
		if (!player || !player.GetIdentity())
		{
			Error(ToString() + "::OnExecuteServer - Could not get player!");
			return;
		}

		if (!CF_Modules<ExpansionTeleporterModule>.Get(m_Expansion_TeleporterModule))
			return;

		Object targetObject;
		if (!Class.CastTo(targetObject, action_data.m_Target.GetParentOrObject()))
			return;
		
		Expansion_Teleporter_Base teleportObj = Expansion_Teleporter_Base.Cast(targetObject);
		if (!teleportObj || teleportObj && !teleportObj.IsActive())
			return;
		
		int teleporterID = teleportObj.GetTeleporterID();
		ExpansionTeleportData teleporterData = m_Expansion_TeleporterModule.GetTeleporterDataByID(teleporterID);
		if (!teleporterData)
			return;

	#ifdef EXPANSIONMODQUESTS
		if (GetExpansionSettings().GetQuest().EnableQuests)
		{
			int questID = teleporterData.GetQuestID();
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
		
					ExpansionNotification(new StringLocaliser("Teleporter is locked!"), new StringLocaliser("You have no access to this teleporter yet. You need to compelete the quest " + questConig.GetTitle() + " first to use this teleporter."), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_AMETHYST, 10, ExpansionNotificationType.TOAST).Create(player.GetIdentity());
					return;
				}
			}
		}
	#endif
		
	#ifdef EXPANSIONMODHARDLINE
		if (GetExpansionSettings().GetHardline().UseReputation)
		{
			int reputationRequirement = teleporterData.GetReputation();
			Print(ToString() + "::OnExecuteServer - Need to have reputation: " + reputationRequirement);
			if (reputationRequirement > 0)
			{
				int reputation = player.Expansion_GetReputation();
				Print(ToString() + "::OnExecuteServer - Player reputation: " + reputation);
				if (reputation < reputationRequirement)
				{
					ExpansionNotification(new StringLocaliser("Teleporter is locked!"), new StringLocaliser("You have no access to this teleporter yet. You need at least " + reputationRequirement + " reputation points first to use this teleporter."), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_AMETHYST, 10, ExpansionNotificationType.TOAST).Create(player.GetIdentity());
					return;
				}
			}
		}
	#endif
		
	#ifdef EXPANSIONMODAI
		string factionName = teleporterData.GetFaction();
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
					message = "You have no access to this teleporter. You need to be a member of the " + factionName + " faction. You are a member of the " + playerFactionName + " faction.";
				else
					message = "You have no access to this teleporter. You need to be a member of the " + factionName + " faction.";
				
				ExpansionNotification(new StringLocaliser("Teleporter is locked!"), new StringLocaliser(message), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_AMETHYST, 10, ExpansionNotificationType.TOAST).Create(player.GetIdentity());
				return;
			}
		}
	#endif
		
		if (!teleporterData.IsExit())
		{
			m_Expansion_TeleporterModule.RequestOpenTeleporterMenu(teleportObj, player.GetIdentity(), teleporterData);
		}
		else
		{
			m_Expansion_TeleporterModule.ExitTeleport(player, teleporterData);
		}
	}
};
