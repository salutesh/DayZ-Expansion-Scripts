/**
 * ExpansionActionOpenGarageMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionOpenGarageMenu: ActionInteractBase
{
	void ExpansionActionOpenGarageMenu()
	{
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
		m_Text = "#STR_EXPANSION_GARAGE_ACTION";
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		Object targetObject = target.GetObject();
		if (!targetObject)
			return false;

		if (!ExpansionGarageModule.s_Instance)
			return false;

		if (!ExpansionGarageModule.s_Instance.CanOpenMenu())
			return false;

		auto settings = GetExpansionSettings().GetGarage(false);
		if (!settings.IsLoaded())
			return false;

		if (!settings.Enabled)
			return false;

		if (!ExpansionStatic.IsAnyOf(targetObject, settings.EntityWhitelist))
			return false;

	#ifdef EXPANSIONMODBASEBUILDING
		//! Can only interact with parking meter when it has energy and is in a territory when territories are enabled
		auto territorySettings = GetExpansionSettings().GetTerritory();
		auto garageSettings = GetExpansionSettings().GetGarage();
		ExpansionParkingMeter parkingMeter;
		if (Class.CastTo(parkingMeter, targetObject))
		{
			if (territorySettings.EnableTerritories && garageSettings.GarageMode == ExpansionGarageMode.Territory && !player.IsInTerritory())
				return false;

			if (!parkingMeter.GetCompEM().CanWork())
				return false;
		}
	#endif

		return true;
	}

	override void OnExecuteClient(ActionData action_data)
	{
		super.OnExecuteClient(action_data);
		GetDayZGame().GetExpansionGame().GetExpansionUIManager().CreateSVMenu("ExpansionGarageMenu");
	}
};
