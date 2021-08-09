/**
 * ExpansionActionSwitchSeats.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionSwitchSeats: ActionBase
{
	void ExpansionActionSwitchSeats()
	{
		//m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_STARTENGINE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_SpecialtyWeight = 1.0;
		m_LockTargetOnUse = false;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#change_seat";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		
		ExpansionVehicleBase trans = null;
		int nextSeatIdx = -1;
		
		ExpansionHumanCommandVehicle vehCommand = player.GetCommand_ExpansionVehicle();

		if ( !vehCommand  )
			return false;
		
		int componentIndex = target.GetComponentIndex();

		if ( !target )
			return false;

		if ( !Class.CastTo(trans, target.GetObject()) )
			return false;

		nextSeatIdx = trans.CrewPositionIndex( componentIndex );

		if ( nextSeatIdx < 0 )
			return false;

		Human crew = trans.CrewMember( nextSeatIdx );
		if ( crew )
			return false;

		if ( !trans.CanReachSeatFromSeat( trans.CrewMemberIndex( player ), nextSeatIdx ) )
			return false;

		return true;
	}

	override void Start( ActionData action_data )
	{
		super.Start( action_data );
		ExpansionHumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_ExpansionVehicle();
		if ( vehCommand )
		{
			ExpansionVehicleBase trans;
			if ( Class.CastTo(trans, action_data.m_Target.GetObject()) )
			{
				int nextSeat = trans.CrewPositionIndex( action_data.m_Target.GetComponentIndex() );
				int seat = trans.GetSeatAnimationType( nextSeat );
				if ( seat >= 0 )
				{
					//pTransportPositionIndex, int pVehicleSeat
					vehCommand.SwitchSeat( nextSeat, seat );
					if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
					{

						action_data.m_Player.OnVehicleSwitchSeat( nextSeat );
					}
				}
			}
		}
	}
	
	override void OnUpdate(ActionData action_data)
	{
		if(action_data.m_State == UA_START)
		{
			ExpansionHumanCommandVehicle hcv = action_data.m_Player.GetCommand_ExpansionVehicle();
			if( !hcv || !action_data.m_Player.GetCommand_ExpansionVehicle().IsSwitchSeat() )
			{
				End(action_data);
			}
		}
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
};