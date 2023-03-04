/**
 * ActionBuildPart.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionBuildPartCB
{
	override float SetCallbackDuration( ItemBase item )
	{
		if ( item && item.IsKindOf("ExpansionAdminHammer") )
			return 0.1;
		
		return super.SetCallbackDuration(item);
	}
};

modded class ActionBuildPart
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !super.ActionCondition( player, target, item ) )
			return false;

		if ( player.IsInTerritory() )
		{
			//! If it was deployable, it's also buildable
			ItemBase targetItem = ItemBase.Cast( target.GetObject() );
			if (ActionDeployObject.CanDeployInTerritory(player, targetItem))
				return true;

			if (GetGame().IsServer() && player.GetIdentity())
				ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ENEMY_TERRITORY").Error(player.GetIdentity());

			return false;
		}

		return true;
	}

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
				if ( !construction.IsColliding( part_name ) && construction.CanBuildPart( part_name, action_data.m_MainItem, true ) )
				{
					construction.Expansion_AdminBuildPartServer( action_data.m_Player, part_name, AT_BUILD_PART );

					if ( GetExpansionSettings().GetLog().AdminTools )
						GetExpansionSettings().GetLog().PrintLog("[AdminTools] Player \"" + action_data.m_Player.GetIdentity().GetName() + "\" (id=" + action_data.m_Player.GetIdentity().GetId() + " pos=" + action_data.m_Player.GetPosition() + ")" + " used the Admin Hammer to build " + part_name);
				}

				action_data.m_Player.GetSoftSkillsManager().AddSpecialty( m_SpecialtyWeight );
			}
			else
			{
				super.OnFinishProgressServer( action_data );
			}
		}
	}
};
