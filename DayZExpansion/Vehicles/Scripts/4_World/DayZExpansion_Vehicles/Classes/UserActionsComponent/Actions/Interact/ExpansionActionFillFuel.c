/**
 * ExpansionActionFillFuel.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

[RegisterAction(ExpansionActionFillFuel)]
class ExpansionActionFillFuel: ActionContinuousBase
{
	const string FUEL_SELECTION_NAME = "refill";
	
	void ExpansionActionFillFuel()
	{
		m_CallbackClass = ActionFillFuelCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_EMPTY_VESSEL;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_FullBody = true;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
		m_LockTargetOnUse = false;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#refuel";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !target || !IsTransport(target) )
			return false;

		if ( item.GetQuantity() <= 0 )
			return false;

		if ( item.GetLiquidType() != LIQUID_GASOLINE )
			return false;

		ExpansionVehicleBase car = ExpansionVehicleBase.Cast( target.GetObject() );
		if ( !car )
			return false;
		
		if ( car.GetFluidFraction( CarFluid.FUEL ) >= 0.98 )
			return false;

		array<string> selections = new array<string>;
		target.GetObject().GetActionComponentNameList(target.GetComponentIndex(), selections);

		for (int s = 0; s < selections.Count(); s++)
		{
			if ( selections[s] == car.GetActionCompNameFuel() )
			{
				float dist = vector.DistanceSq( car.GetRefillPointPosWS(), player.GetPosition() );

				if ( dist < car.GetActionDistanceFuel() * car.GetActionDistanceFuel() )
					return true;
			}
		}

		return false;
	}
};
