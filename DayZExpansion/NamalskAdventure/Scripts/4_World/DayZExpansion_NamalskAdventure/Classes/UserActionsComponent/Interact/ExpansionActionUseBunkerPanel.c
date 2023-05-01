/**
 * ExpansionActionUseBunkerPanel.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionUseBunkerPanel: ActionSingleUseBase
{
	void ExpansionActionUseBunkerPanel()
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

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (item.IsDamageDestroyed())
			return false;
		
		Land_Underground_Panel trgt = Land_Underground_Panel.Cast(target.GetObject());
		if (!trgt)
			return false;

		if (trgt.ClassName() != "Land_Underground_Panel")
			return false;

		return (trgt.CanInteract());
	}

	override void OnStartServer( ActionData action_data )
	{
		Land_Underground_Panel target =  Land_Underground_Panel.Cast(action_data.m_Target.GetObject());

		if (!target)
			return;

		target.Interact();
		Expansion_KeyCard_A1_B1.Cast(action_data.m_MainItem).OnCardUsed();
	}

};