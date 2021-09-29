/**
 * ExpansionActionConnectTow.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/
#ifdef EXPANSION_VEHICLE_TOWING
class ExpansionActionConnectTowReciveData : ActionReciveData
{
	CarScript m_Car;
}

class ExpansionActionConnectTowData : ActionData
{
	CarScript m_Car;
};

class ExpansionActionConnectTow: ActionInteractBase
{
	private bool m_IsWinch;

	void ExpansionActionConnectTow()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_HUDCursorIcon = CursorIcons.OpenDoors;
	}

	override ActionData CreateActionData()
	{
		ExpansionActionConnectTowData action_data = new ExpansionActionConnectTowData;
		return action_data;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		if ( m_IsWinch )
			return "#STR_EXPANSION_CONNECT_WINCH";

		return "#STR_EXPANSION_CONNECT_TOW";
	}

	override void WriteToContext( ParamsWriteContext ctx, ActionData action_data )
	{
		super.WriteToContext( ctx, action_data );
		
		ExpansionActionConnectTowData poActionData;
		poActionData = ExpansionActionConnectTowData.Cast(action_data);

		ctx.Write( poActionData.m_Car );
	}
	
	override bool ReadFromContext( ParamsReadContext ctx, out ActionReciveData action_recive_data )
	{
		if ( !action_recive_data )
		{
			action_recive_data = new ExpansionActionConnectTowReciveData;
		}

		super.ReadFromContext(ctx, action_recive_data );

		ExpansionActionConnectTowReciveData action_data_po = ExpansionActionConnectTowReciveData.Cast(action_recive_data);
		
		if ( !ctx.Read( action_data_po.m_Car ) )
			return false;
		
		return true;
	}

	override void HandleReciveData( ActionReciveData action_recive_data, ActionData action_data )
	{
		super.HandleReciveData( action_recive_data, action_data );
		
		ExpansionActionConnectTowReciveData recive_data_po = ExpansionActionConnectTowReciveData.Cast(action_recive_data);
		ExpansionActionConnectTowData action_data_po = ExpansionActionConnectTowData.Cast(action_data);
		
		action_data_po.m_Car = recive_data_po.m_Car;
	}

	override bool SetupAction( PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extra_data = NULL )
	{
		if ( super.SetupAction( player, target, item, action_data, extra_data ) )
		{
			ExpansionActionConnectTowData action_data_b = ExpansionActionConnectTowData.Cast( action_data );
			action_data_b.m_Car = GetCarToTow( player );
			return true;
		}

		return false;
	}

	private CarScript GetCarToTow( PlayerBase player )
	{
		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();

		CarScript car;
		if ( vehCommand && Class.CastTo( car, vehCommand.GetTransport() ) )
		{
			if ( car.IsTowing() )
				return NULL;
				
			if ( car.CrewMemberIndex( player ) == DayZPlayerConstants.VEHICLESEAT_DRIVER )
			{
				array<Object> excluded = new array<Object>;
				array<Object> collided = new array<Object>;
				excluded.Insert( car );
				excluded.Insert( player );

				GetGame().IsBoxColliding( car.GetTowPosition(), car.GetTowDirection(), car.GetTowExtents(), excluded, collided );

				foreach ( Object o : collided )
				{
					CarScript other_car;
					if ( Class.CastTo( other_car, o ) )
					{
						if ( !other_car.IsBeingTowed() && !other_car.IsTowing() && car.CanConnectTow( other_car ) )
						{
							m_IsWinch = car.IsHelicopter();

							//! Check if someone is inside the Vehicle the Helicopter is trying to winch
							//! Goal is to prevent a exploit allowing to get inside people bases
							if ( m_IsWinch )
							{								
								for( int i = 0; i < other_car.CrewSize(); i++ )
								{
									Human crew = other_car.CrewMember( i );
									if ( crew )
										return NULL;
								}
							}

							if (dBodyGetMass(other_car) * 0.9 < dBodyGetMass(car)  )
								return other_car;
						}
					}
				}
			}
		}

		return NULL;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( GetExpansionSettings() && !GetExpansionSettings().GetVehicle().Towing )
			return false;

		if ( !super.ActionCondition( player, target, item ) )
			return false;

		return GetCarToTow( player ) != NULL;
	}
	
	override void OnStartServer( ActionData action_data )
	{
		super.OnStartServer( action_data );

		ExpansionActionConnectTowData action_data_b = ExpansionActionConnectTowData.Cast( action_data );

		HumanCommandVehicle vehCommand = action_data_b.m_Player.GetCommand_Vehicle();

		CarScript car;
		if ( vehCommand && Class.CastTo( car, vehCommand.GetTransport() ) )
		{				
			if ( car.CrewMemberIndex( action_data_b.m_Player ) == DayZPlayerConstants.VEHICLESEAT_DRIVER )
			{
				#ifdef EXPANSION_VEHICLE_TOWING
				car.CreateTow( action_data_b.m_Car );
				#endif
				
#ifdef EXPANSIONMODVEHICLE
				if ( GetExpansionSettings().GetLog().VehicleTowing )
					GetExpansionSettings().GetLog().PrintLog("[VehicleTowing] Player \"" + action_data.m_Player.GetIdentity().GetName() + "\" (id=" + action_data.m_Player.GetIdentity().GetId() + " pos=" + action_data.m_Player.GetPosition() + ")" + " Towed " + action_data_b.m_Car.GetType() + " with " + car.GetType() );
#endif
			}
		}
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
}
#endif