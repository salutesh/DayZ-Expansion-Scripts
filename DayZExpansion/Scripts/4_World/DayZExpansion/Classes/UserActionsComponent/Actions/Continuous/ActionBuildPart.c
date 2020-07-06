/**
 * ActionBuildPart.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/


modded class ActionBuildPartCB
{
	override float SetCallbackDuration( ItemBase item )
	{
		if ( item.IsKindOf("ExpansionAdminHammer") )
			return 0.1;
		
		return super.SetCallbackDuration(item);
	}
}

modded class ActionBuildPart
{
	/*
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		ItemBase item_in_hands = ItemBase.Cast( player.GetHumanInventory().GetEntityInHands() );

		if ( item_in_hands )
		{
			if ( item_in_hands.GetType() == "ExpansionAdminHammer" )
			{
				return true;
			}
		}

		return super.ActionCondition( player, target, item );
	}

	override bool ActionConditionContinue( ActionData action_data )
	{
		ItemBase item_in_hands = ItemBase.Cast( action_data.m_Player.GetHumanInventory().GetEntityInHands() );

		if ( item_in_hands )
		{
			if ( item_in_hands.GetType() == "ExpansionAdminHammer" )
			{
				return true;
			}
		}

		return super.ActionConditionContinue( action_data );
	}
	*/

	override void OnFinishProgressServer( ActionData action_data )
	{
		BaseBuildingBase base_building = BaseBuildingBase.Cast( action_data.m_Target.GetObject() );
		Construction construction = base_building.GetConstruction();
		
		string part_name = BuildPartActionData.Cast(action_data).m_PartType;

		ItemBase item_in_hands = ItemBase.Cast( action_data.m_Player.GetHumanInventory().GetEntityInHands() );

		if ( item_in_hands )
		{
			if ( item_in_hands.GetType() == "ExpansionAdminHammer" )
			{
				if ( !construction.IsColliding( part_name ) && construction.CanBuildPart( part_name, action_data.m_MainItem ) )
				{
					construction.BuildPartServer( part_name, AT_BUILD_PART );
				}

				action_data.m_Player.GetSoftSkillsManager().AddSpecialty( m_SpecialtyWeight );
			}
			else
			{
				super.OnFinishProgressServer( action_data );
			}
		}
	}
}