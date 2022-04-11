/**
 * ActionHeliRefill.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionActionHelicopterHoverRefillCB
 * @brief		
 **/
class ExpansionActionHelicopterHoverRefillCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousFillFuel(UAQuantityConsumed.FUEL, 0.5);
	}
}

/**@class		ExpansionActionHelicopterHoverRefill
 * @brief		
 **/
class ExpansionActionHelicopterHoverRefill : ActionContinuousBase
{
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

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!player)
			return false;

		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();
		ExpansionHumanCommandVehicle expVehCommand = player.GetCommand_ExpansionVehicle();
		if (!vehCommand && !expVehCommand)
			return false;

		ExpansionHelicopterScript d_helicopter;
		if (vehCommand && (!Class.CastTo(d_helicopter, vehCommand.GetTransport()) || vehCommand.GetVehicleSeat() != DayZPlayerConstants.VEHICLESEAT_DRIVER))
			return false;

		ExpansionVehicleHelicopterBase helicopter;
		if (expVehCommand && (!Class.CastTo(helicopter, expVehCommand.GetTransport()) || expVehCommand.GetVehicleSeat() != DayZPlayerConstants.VEHICLESEAT_DRIVER))
			return false;

		EntityAI vehicle = EntityAI.Cast(player.GetParent());

		vector boundingBox[2];
		vehicle.GetCollisionBox(boundingBox);
		float height = Math.AbsFloat(boundingBox[1][1]) + Math.AbsFloat(boundingBox[0][1]);

		vector start = vehicle.GetPosition() + Vector(0, height, 0);
		vector end = vehicle.GetPosition() + Vector(0, -10.0, 0);
		float radius = 5.0;

		RaycastRVParams params = new RaycastRVParams(start, end, vehicle, radius);
		params.sorted = true;
		params.type = ObjIntersectGeom;
		params.flags = CollisionFlags.ALLOBJECTS;

		// Initiate the raycast
		array<ref RaycastRVResult> results = new array<ref RaycastRVResult>();
		if (DayZPhysics.RaycastRVProxy(params, results))
		{
			for (int i = 0; i < results.Count(); ++i)
			{
				if (results[i].obj == NULL)
					return false;

				if (results[i].obj.GetType() == "Land_FuelStation_Feed")
					return true;

				if (results[i].obj.GetType() == "Land_FuelStation_Feed")
					return true;

				if (results[i].obj.GetType() == "Static_FuelStation_Shed_Enoch")
					return true;

				return false;
			}
		}

		return false;
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		super.OnFinishProgressServer(action_data);

		float capacity;
		float add;
		float fuel;

		ExpansionHelicopterScript d_helicopter;
		if ( Class.CastTo( d_helicopter, action_data.m_Player.GetParent() ) )
		{
			if (d_helicopter.GetFluidFraction(CarFluid.FUEL) >= 0.98)
				return;

			d_helicopter.Fill(CarFluid.FUEL, 5.0);
			return;
		}

		ExpansionVehicleHelicopterBase helicopter;
		if ( Class.CastTo( helicopter, action_data.m_Player.GetParent() ) )
		{
			if (helicopter.GetFluidFraction(CarFluid.FUEL) >= 0.98)
				return;

			helicopter.Fill(CarFluid.FUEL, 5.0);
			return;
		}
	}
}