/**
 * ActionStopEngine.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionStopBoat: ActionSingleUseBase
{
	void ExpansionActionStopBoat()
	{
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_ITEM_OFF;
		m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		//m_HUDCursorIcon     = CursorIcons.LootCorpse;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#STR_EXPANSION_UA_STOP_BOAT";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		HumanCommandVehicle vehCmd = player.GetCommand_Vehicle();
		ExpansionBoatScript car;
		
		if ( vehCmd && vehCmd.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER )
		{
			Transport trans = vehCmd.GetTransport();
			if ( trans )
			{
				if ( Class.CastTo(car, trans) && car.MotorIsOn() )
				{
					//if ( car.CrewMemberIndex( player ) == DayZPlayerConstants.VEHICLESEAT_DRIVER )
					if ( car.GetSpeedometer() <= 8 )
						return true;
				}
			}
		}
		return false;
	}
	
	override void OnExecuteServer( ActionData action_data )
	{
		HumanCommandVehicle vehCmd = action_data.m_Player.GetCommand_Vehicle();
		ExpansionBoatScript car;

		if ( vehCmd )
		{
			Transport trans = vehCmd.GetTransport();
			if ( trans )
			{
				if ( Class.CastTo(car, trans) )
				{
					car.MotorStop();
					if ( !GetGame().IsMultiplayer() )
						SEffectManager.PlaySound("offroad_engine_stop_fuel_SoundSet", car.GetPosition() );
				}
			}
		}	
	}
	
	override void OnExecuteClient( ActionData action_data )
	{
		HumanCommandVehicle vehCmd = action_data.m_Player.GetCommand_Vehicle();
		ExpansionBoatScript car;

		if ( vehCmd )
		{
			Transport trans = vehCmd.GetTransport();
			if ( trans )
			{
				if ( Class.CastTo(car, trans) )
				{
					car.MotorStop();
					SEffectManager.PlaySound("offroad_engine_stop_fuel_SoundSet", car.GetPosition() );
				}
			}
		}
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
}