/**
 * ExpansionActionFillGeneratorTankOnGround.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ActionFillGeneratorTankOnGroundCB : ActionContinuousBaseCB
{
	private const float QUANTITY_FILLED_PER_SEC = 400;
	
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousFillPowerGenerator(QUANTITY_FILLED_PER_SEC, LIQUID_GASOLINE);
	}
}

class ExpansionActionFillGeneratorTankOnGround: ActionContinuousBase
{
	// ------------------------------------------------------------
	void ExpansionActionFillGeneratorTankOnGround()
	{
		m_CallbackClass = ActionFillGeneratorTankOnGroundCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_EMPTY_VESSEL;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_FullBody = true;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
	}

	// ------------------------------------------------------------
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
	}

	// ------------------------------------------------------------
	override string GetText()
	{
		return "#refuel";
	}
		
	// ------------------------------------------------------------
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		PowerGenerator pg;
		if ( Class.CastTo(pg,target.GetObject()) )
		{
			if ( pg.CanAddFuel( item ) )
			{
				return true;
			}
		}
		
		return false;
	}
}