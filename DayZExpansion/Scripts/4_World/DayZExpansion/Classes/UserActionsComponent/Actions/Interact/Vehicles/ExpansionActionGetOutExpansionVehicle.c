/**
 * ExpansionActionGetOutExpansionVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionActionGetOutExpansionVehicle
 * @brief		
 **/
class ExpansionActionGetOutExpansionVehicle: ActionInteractBase
{
	private ExpansionVehicleScript m_Transport;
	private int m_CrewIdx;

	void ExpansionActionGetOutExpansionVehicle()
	{
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_HUDCursorIcon = "GetInDriver";
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#leave_vehicle";
	}

	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}
	
	override bool HasProgress()
	{
		return false;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		m_Transport = NULL;
		m_CrewIdx = -1;

		if ( !player.GetCommand_ExpansionVehicle() )
			return false;

        m_Transport = player.GetCommand_ExpansionVehicle().m_Vehicle;

		return true;
	}

	override bool CanBeUsedInRestrain()
	{
		return true;
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
	
	override void OnUpdate( ActionData action_data )
	{
		if ( action_data.m_State == UA_START )
		{
			if ( !action_data.m_Player.GetCommand_ExpansionVehicle().IsGettingOut() )
			{
				End( action_data );
			}
		}
	}

	override void Start( ActionData action_data )
	{
		super.Start( action_data );
		
		action_data.m_Player.GetCommand_ExpansionVehicle().GetOut();
	}
}