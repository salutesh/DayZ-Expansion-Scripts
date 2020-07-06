/**
 * ActionHeliRefill.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionActionHelicopterHoverRefillCB
 * @brief		
 **/
class ExpansionActionHelicopterHoverRefillCB: ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousFillFuel( UAQuantityConsumed.FUEL, 0.5 );
	}
}

/**@class		ExpansionActionHelicopterHoverRefill
 * @brief		
 **/
class ExpansionActionHelicopterHoverRefill: ActionContinuousBase
{
	protected ExpansionHelicopterScript m_Helicopter;

	void ExpansionActionHelicopterHoverRefill()
	{
		m_CallbackClass = ExpansionActionHelicopterHoverRefillCB;

		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENITEM;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_LOW;
	}

	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override typename GetInputType()
	{
		return ExpansionInputCarRefill;
	}
	
	override string GetText()
	{
		return "#refill";
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !player )
			return false;
		
		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();

		ExpansionHelicopterScript car;
		if ( vehCommand && Class.CastTo( car, vehCommand.GetTransport() ) )
		{
			m_Helicopter = car;

			if ( vehCommand.GetVehicleSeat() != DayZPlayerConstants.VEHICLESEAT_DRIVER )
				return false;

			if ( car.GetFluidFraction( CarFluid.FUEL ) >= 1.0 )
				return false;

			vector boundingBox[2];
			m_Helicopter.GetCollisionBox( boundingBox );
			float height = Math.AbsFloat( boundingBox[1][1] ) + Math.AbsFloat( boundingBox[0][1] );
				
			vector start = m_Helicopter.GetPosition() + Vector( 0, height, 0 );
			vector end = m_Helicopter.GetPosition() + Vector( 0, -10.0, 0 );
			float radius = 5.0;

			RaycastRVParams params = new RaycastRVParams( start, end, m_Helicopter, radius );
			params.sorted = true;
			params.type = ObjIntersectGeom;
			params.flags = CollisionFlags.ALLOBJECTS;

			// Initiate the raycast
			array< ref RaycastRVResult > results = new array< ref RaycastRVResult >();
			if ( DayZPhysics.RaycastRVProxy( params, results ))
			{
				for ( int i = 0; i < results.Count(); ++i )
				{						
					if ( results[i].obj == NULL )
						return false;

					if ( results[i].obj.GetType() == "Land_FuelStation_Feed" )
						return true;

					if ( results[i].obj.GetType() == "Land_FuelStation_Feed" )
						return true;
						
					if ( results[i].obj.GetType() == "Static_FuelStation_Shed_Enoch" )
						return true;
						
					return false;
				}
			}
		}
		
		return false;
	}

	override void OnFinishProgressServer( ActionData action_data )
	{
		super.OnFinishProgressServer( action_data );

		if ( m_Helicopter.GetFluidFraction( CarFluid.FUEL ) >= 0.98 )
			return;

		float capacity = m_Helicopter.GetFluidCapacity( CarFluid.FUEL );
		float add = capacity / 50;
		float fuel;

		fuel += add; 

		m_Helicopter.Fill( CarFluid.FUEL, fuel );
	}
}