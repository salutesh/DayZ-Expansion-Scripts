/**
 * ActionGetInTransport.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionGetInTransport
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( player.GetCommand_ExpansionVehicle() )
			return false;

		if ( !super.ActionCondition( player, target, item ) )
			return false;

		CarScript car = CarScript.Cast( target.GetObject() );
		
		if ( car.DoorCount() <= 0 )
		{
			return !car.IsLocked();
		} else
		{
			return true;
		}
	}

	override void Start( ActionData action_data )
	{
		IEntity parent = action_data.m_Player.GetParent();

		if ( parent )
		{
			action_data.m_Player.UnlinkFromLocalSpace();

			parent.Update();
		}

		super.Start( action_data );

		bool isCar = true;
		bool isBoat = false;
		bool isPlane = false;

		if ( IsMissionClient() )
		{
			CarScript car;
			if ( Class.CastTo( car, action_data.m_Target.GetObject() ) )
			{
				isCar = car.IsCar();
				isBoat = car.IsBoat();
				isPlane = car.IsPlane();
			}
		}
			
		if ( IsMissionClient() )
		{
			GetUApi().GetInputByName( "UACarLeft" ).ForceDisable( !isCar );
			GetUApi().GetInputByName( "UACarRight" ).ForceDisable( !isCar );

			if ( isPlane )
			{
				GetUApi().GetInputByName( "UACarShiftGearUp" ).ForceDisable( isPlane );
			} else
			{
				GetUApi().GetInputByName( "UACarForward" ).ForceDisable( !isCar );
			}
			
			GetUApi().GetInputByName( "UACarBack" ).ForceDisable( !isCar );

			if ( isBoat )
			{
				GetUApi().GetInputByName( "UACarShiftGearUp" ).ForceDisable( false );
				GetUApi().GetInputByName( "UACarShiftGearDown" ).ForceDisable( false );
			} else
			{
				GetUApi().GetInputByName( "UACarShiftGearUp" ).ForceDisable( !isCar );
				GetUApi().GetInputByName( "UACarShiftGearDown" ).ForceDisable( !isCar );
			}
		}
	}
};