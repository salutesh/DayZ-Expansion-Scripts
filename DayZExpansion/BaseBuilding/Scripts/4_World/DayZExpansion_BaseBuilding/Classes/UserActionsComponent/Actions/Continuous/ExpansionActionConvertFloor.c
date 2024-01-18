/**
 * ExpansionActionConvertFloor.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionConvertFloorCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime( 3 );
	}
};

class ExpansionActionConvertFloor : ActionContinuousBase
{
	//! Set and use this on client only!
	ExpansionFloorBase m_ExpansionFloor;

	void ExpansionActionConvertFloor()
	{
		m_CallbackClass = ExpansionActionConvertFloorCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_ASSEMBLE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;

		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		string text;

		if (m_ExpansionFloor.IsRoof())
			text = "#STR_USRACT_CONVERT_TO_FLOOR";
		else
			text = "#STR_USRACT_CONVERT_TO_ROOF";

		if (GetPermissionsManager().IsAdminToolsToggledOn())
			text = "[ADMIN] " + text;

		return text;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !item || !target.GetObject() )
			return false;

		if ( player.IsInTerritory() && !player.IsInsideOwnTerritory() )
			return false;

		ExpansionFloorBase floor = ExpansionFloorBase.Cast( target.GetObject() );

		if (!GetGame().IsDedicatedServer())
			m_ExpansionFloor = floor;

		if ( !floor || !floor.IsLastStage() || !floor.IsPlayerInside( player, "" ) )
			return false;

		return true;
	}

	override void OnFinishProgressServer( ActionData action_data )
	{
		ExpansionFloorBase floor = ExpansionFloorBase.Cast( action_data.m_Target.GetObject() );
		if ( !floor )
			return;

		floor.SetRoof( !floor.IsRoof() );

		action_data.m_MainItem.DecreaseHealth( 1 / 100 * action_data.m_MainItem.GetMaxHealth( "", "Health" ), false );  //! 1%
		action_data.m_Player.GetSoftSkillsManager().AddSpecialty( m_SpecialtyWeight );
	}
}
