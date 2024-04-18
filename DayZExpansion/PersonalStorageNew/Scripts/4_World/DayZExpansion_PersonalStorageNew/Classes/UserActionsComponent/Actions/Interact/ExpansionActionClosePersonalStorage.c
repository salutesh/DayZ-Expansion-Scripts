/**
 * ExpansionActionClosePersonalStorage.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionClosePersonalStorage: ActionInteractBase
{
	void ExpansionActionClosePersonalStorage()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.CloseDoors;
		m_Text = "#close #STR_EXPANSION_PERSONALSTORAGE";
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
			m_Text = "#close " + hub.Expansion_GetContainerDisplayName(player);
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		ExpansionPersonalStorageHub hub;
		if (!Class.CastTo(hub, target.GetObject()))
			return false;

		return ExpansionPersonalStorageHub.s_Expansion_PersonalStorageHubs[player.GetIdentityUID()] == hub;
	}

	override void OnStart(ActionData action_data)
	{
		super.OnStart(action_data);

		ExpansionPersonalStorageHub hub;
		if (!Class.CastTo(hub, action_data.m_Target.GetObject()))
			return;

		if (!hub.Expansion_ClosePersonalStorage(action_data.m_Player))
			ExpansionNotification("STR_EXPANSION_ERROR_TITLE", "STR_EXPANSION_PERSONALSTORAGE_CLOSE_ERROR").Error(action_data.m_Player.GetIdentity());
	}
}
