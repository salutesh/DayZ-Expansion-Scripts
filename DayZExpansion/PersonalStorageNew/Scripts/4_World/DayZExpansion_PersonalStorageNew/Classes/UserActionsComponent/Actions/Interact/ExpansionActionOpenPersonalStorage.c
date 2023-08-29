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
	static const string EXPANSION_ACTION_DEFAULT_TEXT = "Open Personal Storage";

	void ExpansionActionOpenPersonalStorage()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.OpenDoors;
		m_Text = EXPANSION_ACTION_DEFAULT_TEXT;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursorNoObject(UAMaxDistances.DEFAULT);
	}

	#ifdef EXPANSIONMODHARDLINE
	override void OnActionInfoUpdate(PlayerBase player, ActionTarget target, ItemBase item)
	{
		m_Text = EXPANSION_ACTION_DEFAULT_TEXT + " (lv. " + player.Expansion_GetPersonalStorageLevel() + ")";
	}
	#endif

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		ExpansionPersonalStorageHub hub;
		if (!Class.CastTo(hub, target.GetObject()))
			return false;

		return ExpansionPersonalStorageHub.s_Expansion_PersonalStorageHubs[player.GetIdentityUID()] != hub;
	}

	override void OnStart(ActionData action_data)
	{
		super.OnStart(action_data);

		ExpansionPersonalStorageHub hub;
		if (!Class.CastTo(hub, action_data.m_Target.GetObject()))
			return;

		if (!hub.Expansion_OpenPersonalStorage(action_data.m_Player))
			ExpansionNotification("STR_EXPANSION_ERROR_TITLE", "Could not open personal storage").Error(action_data.m_Player.GetIdentity());
	}
}
