/**
 * ExpansionActionDismantleFlag.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionDismantleFlagCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime( 30 );
	}
}

class ExpansionActionDismantleFlag: ActionContinuousBase
{
	void ExpansionActionDismantleFlag()
	{
		m_CallbackClass = ExpansionActionDismantleFlagCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.DEFAULT );
		m_ConditionItem = new CCINotPresent;
	}
	
	override string GetText()
	{
		return "#fold";
	}
	
	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		bool canDismantle = (GetExpansionSettings().GetBaseBuilding().CanDismantleFlag && GetExpansionSettings().GetBaseBuilding().EnableSimpleFlagBuilding);

		if ( targetObject )
		{
			//! is this a flag ?
			TerritoryFlag flag;
			if ( !Class.CastTo( flag, targetObject ) )
				return false;

			//! is he inside his own territory ?
			if ( player.IsInsideTerritory() )
				return canDismantle;
		}
		
		return false;
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{
		EntityAI flag = EntityAI.Cast(action_data.m_Target.GetObject() );

		//! Should the flag drop a flag kit after being dismantled ?
		if ( GetExpansionSettings().GetBaseBuilding().DestroyFlagOnDismantle )
			GetGame().CreateObject("TerritoryFlagKit", flag.GetPosition() );

		flag.Delete();
	}
	
	override string GetAdminLogMessage(ActionData action_data)
	{
		return " folded " + action_data.m_Target.GetObject().GetDisplayName();
	}
}