/**
 * ExpansionActionUseSatellitePanel.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionUseSatellitePanel: ActionInteractBase
{
	void ExpansionActionUseSatellitePanel()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "#use";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		Expansion_Satellite_Panel_Lever trgt = Expansion_Satellite_Panel_Lever.Cast(target.GetObject());
		if (!trgt) 
			return false;
		
	#ifdef SERVER
		if (!trgt.CanInteract())
			return false;
	#endif
		
		return true;
	}

	override void OnStartServer(ActionData action_data)
	{
		Expansion_Satellite_Panel_Lever target =  Expansion_Satellite_Panel_Lever.Cast(action_data.m_Target.GetObject());
		if (!target)
			return;
		
		target.Interact();
	}
};