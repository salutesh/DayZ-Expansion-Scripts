/**
 * ExpansionActionRepairVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionRepairVehicleBase: ExpansionActionToolBase
{
	void ExpansionActionRepairVehicleBase()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
	}

	override void Setup(PlayerBase player, ActionTarget target, ItemBase item)
	{
		m_Time = 10;
		m_Cycles = 1;
		m_ToolDamagePercent = 8.75;  //! Matches effective vanilla ActionRepairCarEngine value for PipeWrench
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#repair";
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		EntityAI vehicle = EntityAI.Cast(action_data.m_Target.GetParentOrObject());

		if (vehicle)
		{
			float factor = Math.RandomFloatInclusive( 0.05, 0.15);  //! Matches vanilla ActionRepairCarEngine value
			vehicle.AddHealth("", "Health", vehicle.GetMaxHealth("", "Health") * factor);
			vehicle.AddHealth("Engine", "Health", vehicle.GetMaxHealth("Engine", "Health") * factor);
			for (int i = 0; i < vehicle.GetInventory().AttachmentCount(); i++)
			{
				auto att = vehicle.GetInventory().GetAttachmentFromIndex(i);
				if (att && !att.IsDamageDestroyed() && att.GetHealth01() < 1.0)
					att.AddHealth("", "Health", att.GetMaxHealth("", "Health") * factor);
			}
			super.OnFinishProgressServer( action_data );
		}
	}
}

class ExpansionActionRepairVehicle: ExpansionActionRepairVehicleBase
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!super.ActionCondition(player, target, item))
			return false;

		auto vehicle = ExpansionVehicle.Get( target.GetParentOrObject() );

		if (!vehicle || (!vehicle.IsHelicopter() && !vehicle.IsBoat()) || vehicle.IsDestroyed())
			return false;

		if (vehicle.EngineIsOn())
			return false;

		if (vehicle.GetHealthLevel() != GameConstants.STATE_PRISTINE)
			return true;

		if (vehicle.GetHealthLevel("Engine") != GameConstants.STATE_PRISTINE)
			return true;

		if (vehicle.IsHelicopter() && vehicle.IsRotorDamaged())
			return true;

		return false;
	}
}

class ExpansionActionRepairHelicopter: ExpansionActionRepairVehicle
{
	void ExpansionActionRepairHelicopter()
	{
		Error("DEPRECATED, please use ExpansionActionRepairVehicle");
	}
}
