/**
 * ActionTogglePlaceObject.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionTogglePlaceObject
{
	override void Start( ActionData action_data ) //Setup on start of action
	{
		super.Start( action_data );

		//! NOTE: When making changes below, don't forget that the logic in ActionDeployObject::ActionCondition should be compatible!

		PlayerBase player = action_data.m_Player;
		ItemBase item = action_data.m_MainItem;
		bool territoryReqNotif = false;
		bool enemyTerritoryNotif = false;
		bool enemyTerritoryNearNotif = false;

		if ( !player.IsPlacingLocal() )
			return;
		
		if ( GetExpansionSettings().GetTerritory() && !GetExpansionSettings().GetTerritory().EnableTerritories )
			return;

 		if ( GetExpansionSettings().GetBaseBuilding() )
		{
			if (GetExpansionSettings().GetBaseBuilding().AllowBuildingWithoutATerritory)
			{
				//! Flag can be placed if outside any territory/perimeter or if inside own territory/perimeter
				//! Other items can be placed if not in enemy territory or if item is whitelisted
				//! Special handling for items that can make garden plots

				if ( player.IsInTerritory() )
				{
					enemyTerritoryNotif = !CanDeployInTerritory( player, item );
				} else if ( item.IsInherited( TerritoryFlagKit ) && player.IsInPerimeter() && !player.IsInsideOwnPerimeter() )
				{
					enemyTerritoryNearNotif = true;
				}
			} else
			{
				//! Flag can be placed if outside any territory/perimeter and is whitelisted, if inside enemy territory and is whitelisted, or if inside own territory/perimeter
				//! Other items can only be placed if item is whitelisted
				//! Special handling for items that can make garden plots

				if ( player.IsInTerritory() )
				{
					enemyTerritoryNotif = !CanDeployInTerritory( player, item );
				} else
				{
					territoryReqNotif = true;
					for ( int i = 0; i < GetExpansionSettings().GetBaseBuilding().DeployableOutsideATerritory.Count(); ++i )
					{
						string deployable = GetExpansionSettings().GetBaseBuilding().DeployableOutsideATerritory[i];
						if ( ( item.CanMakeGardenplot() && deployable == "GardenPlot" ) || item.IsKindOf( deployable ) )
						{
							territoryReqNotif = false;
							if ( item.IsInherited( TerritoryFlagKit ) && player.IsInPerimeter() && !player.IsInsideOwnPerimeter() )
							{
								enemyTerritoryNearNotif = true;
							}
							break;
						}
					}
				}
			}
		}

		if (territoryReqNotif)
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_TERRITORY_REQUIRED" ), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, action_data.m_Player.GetIdentity() );
		
		if (enemyTerritoryNotif)
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ENEMY_TERRITORY" ), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, action_data.m_Player.GetIdentity() );	
		
		if (enemyTerritoryNearNotif)
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ENEMY_TERRITORY_NEAR" ), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, action_data.m_Player.GetIdentity() );	
	}

	protected static bool CanDeployInTerritory( PlayerBase player, ItemBase item )
	{
		if ( player.IsInsideOwnTerritory() )
			return true;

		for ( int i = 0; i < GetExpansionSettings().GetBaseBuilding().DeployableInsideAEnemyTerritory.Count(); ++i )
		{
			string deployable = GetExpansionSettings().GetBaseBuilding().DeployableInsideAEnemyTerritory[i];
			if ( ( item.CanMakeGardenplot() && deployable == "GardenPlot" ) || item.IsKindOf( deployable ) )
				return true;
		}

		return false;
	}
}