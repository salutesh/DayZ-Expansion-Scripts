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
			return;

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
