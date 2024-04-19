/**
 * ExpansionActionOpenPersonalStorage.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionOpenPersonalStorage: ActionInteractBase
{
	void ExpansionActionOpenPersonalStorage()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.OpenDoors;
		m_Text = "#open #STR_EXPANSION_PERSONALSTORAGE";
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursorNoObject(UAMaxDistances.DEFAULT);
	}

	override void OnActionInfoUpdate(PlayerBase player, ActionTarget target, ItemBase item)
	{
		ExpansionPersonalStorageHub hub;
		if (Class.CastTo(hub, target.GetObject()))
			m_Text = "#open " + hub.Expansion_GetContainerDisplayName(player);
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		ExpansionPersonalStorageHub hub;
		if (!Class.CastTo(hub, target.GetObject()))
			return false;

	#ifdef EXPANSIONMODHARDLINE
		if (GetGame().IsServer())
		{
			int nextLevel, repReq, questID;
			bool completed;
			int lvl = ExpansionPersonalStorageHub.Expansion_GetPersonalStorageLevelEx(player, nextLevel, repReq, questID, completed);
			if (lvl < 1)
			{
				if (repReq > player.Expansion_GetReputation())
					ExpansionNotification("STR_EXPANSION_PERSONALSTORAGE_ACCESS_DENIED", new StringLocaliser("STR_EXPANSION_PERSONALSTORAGE_ACCESS_DENIED_REPUTATION", repReq.ToString()), "Exclamationmark", COLOR_EXPANSION_NOTIFICATION_AMETHYST).Create();

			#ifdef EXPANSIONMODQUESTS
				if (questID && !completed)
				{
					ExpansionQuestConfig questConfig = ExpansionQuestModule.GetModuleInstance().GetQuestConfigByID(questID);
					if (questConfig)
						ExpansionNotification("STR_EXPANSION_PERSONALSTORAGE_ACCESS_DENIED", new StringLocaliser("STR_EXPANSION_PERSONALSTORAGE_ACCESS_DENIED_QUEST", questConfig.GetTitle()), "Exclamationmark", COLOR_EXPANSION_NOTIFICATION_AMETHYST).Create();
					else
						ExpansionNotification("Internal Error", "Could not get config for quest ID: " + questID).Error();
				}
			#endif

				return false;
			}
		}
	#endif

		return ExpansionPersonalStorageHub.s_Expansion_PersonalStorageHubs[player.GetIdentityUID()] != hub;
	}

	override void OnStart(ActionData action_data)
	{
		super.OnStart(action_data);

		ExpansionPersonalStorageHub hub;
		if (!Class.CastTo(hub, action_data.m_Target.GetObject()))
			return;

		if (!hub.Expansion_OpenPersonalStorage(action_data.m_Player))
			ExpansionNotification("STR_EXPANSION_ERROR_TITLE", "STR_EXPANSION_PERSONALSTORAGE_OPEN_ERROR").Error(action_data.m_Player.GetIdentity());
	}
}
