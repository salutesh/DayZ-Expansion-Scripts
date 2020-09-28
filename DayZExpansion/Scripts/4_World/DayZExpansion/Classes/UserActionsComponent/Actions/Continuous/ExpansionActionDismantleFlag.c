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
};

class ExpansionActionDismantleFlagActionData : ActionData
{
	bool m_IsDismantle;
};

class ExpansionActionDismantleFlag: ActionContinuousBase
{
	private bool m_IsDismantle;

	void ExpansionActionDismantleFlag()
	{
		m_CallbackClass = ExpansionActionDismantleFlagCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
	}
	
	override ActionData CreateActionData()
	{
		ActionData action_data = new ExpansionActionDismantleFlagActionData;
		return action_data;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.DEFAULT );
		m_ConditionItem = new CCINotPresent;
	}
	
	override string GetText()
	{
		if ( m_IsDismantle )
			return "#fold";
		
		return "#destroy";
	}
	
	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !GetExpansionSettings().GetBaseBuilding().SimpleTerritory )
			return false;

		m_IsDismantle = false;

		//! This has to be a valid expansion territory flag
		TerritoryFlag flag;
		if ( !Class.CastTo( flag, target.GetObject() ) )
			return false;

		m_IsDismantle = !flag.HasExpansionTerritoryInformation();
		if ( m_IsDismantle )
			return true;

		//! If we aren't in our own territory, we are raiding.
		//! We don't need to check if we are in a territory.
		m_IsDismantle = player.IsInsideOwnTerritory();

		//! Force the player to dismantle with tools
		if ( GetExpansionSettings().GetBaseBuilding().DismantleFlagRequireTools == 1 )
				return false;

		//! The Raider can't dismantle at all but the owner can
		if ( GetExpansionSettings().GetBaseBuilding().DismantleFlagRequireTools == -1 && !m_IsDismantle )
				return false;

		return true;
	}
	
	override void OnStart( ActionData action_data )
	{
		super.OnStart( action_data );

		ExpansionActionDismantleFlagActionData ead_action_data = ExpansionActionDismantleFlagActionData.Cast( action_data );

		ead_action_data.m_IsDismantle = false;

		if ( !TerritoryFlag.Cast( action_data.m_Target.GetObject() ).HasExpansionTerritoryInformation() )
			ead_action_data.m_IsDismantle = true;

		if ( action_data.m_Player.IsInsideOwnTerritory() )
			ead_action_data.m_IsDismantle = true;

		if ( ead_action_data.m_IsDismantle )
			action_data.m_ActionComponent = new CAContinuousTime( 5 );
		else
			action_data.m_ActionComponent = new CAContinuousTime( 30 );

		if ( action_data.m_ActionComponent ) 
		{
			action_data.m_ActionComponent.Init( action_data );		
		}
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
		return " destroyed " + action_data.m_Target.GetObject().GetDisplayName();
	}
};