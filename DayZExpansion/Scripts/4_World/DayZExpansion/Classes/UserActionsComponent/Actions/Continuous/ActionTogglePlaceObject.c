/**
 * ActionTogglePlaceObject.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
		bool buildZoneReqNotif;
		bool territoryReqNotif;
		bool enemyTerritoryNotif;
		bool enemyTerritoryNearNotif;

		if ( !player.IsPlacingLocal() )
			return;

		if ( GetExpansionSettings().GetBaseBuilding().Zones.Count() )
		{
			ExpansionBuildNoBuildZone zone = player.GetBuildNoBuildZone();

			if ( zone )
			{
				//! Player is inside zone

				bool isItemAllowed = !zone.IsWhitelist;

				foreach ( string item_name : zone.Items )
				{
					if ( ( item.CanMakeGardenplot() && item_name == "GardenPlot" ) || item.IsKindOf( item_name ) )
					{
						isItemAllowed = zone.IsWhitelist;
						break;
					}
				}

				if ( !isItemAllowed )
				{
					string notificationTitleStringID;
					string notificationTextStringID = zone.CustomMessage;

					if ( GetExpansionSettings().GetBaseBuilding().ZonesAreNoBuildZones )
					{
						notificationTitleStringID = "STR_EXPANSION_NOBUILD_ZONE_TITLE";
						if ( !notificationTextStringID )
							notificationTextStringID = "STR_EXPANSION_NOBUILD_ZONE_ITEM_DISALLOWED";
					} else
					{
						notificationTitleStringID = "STR_EXPANSION_BUILD_ZONE_TITLE";
						if ( !notificationTextStringID )
							notificationTextStringID = "STR_EXPANSION_BUILD_ZONE_ITEM_DISALLOWED";
					}
						
					ExpansionNotification(new StringLocaliser(notificationTitleStringID, zone.Name), new StringLocaliser(notificationTextStringID)).Error(action_data.m_Player.GetIdentity());
					
					return;
				}
			} else if ( !GetExpansionSettings().GetBaseBuilding().ZonesAreNoBuildZones )
			{
				//! Zones are "build" zones, so if player isn't inside one, disallow placing if not whitelisted

				buildZoneReqNotif = true;
			}
		}
		
		if ( GetExpansionSettings().GetTerritory() && !GetExpansionSettings().GetTerritory().EnableTerritories )
			return;

 		if ( GetExpansionSettings().GetBaseBuilding() )
		{
			if (GetExpansionSettings().GetBaseBuilding().AllowBuildingWithoutATerritory && !buildZoneReqNotif)
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
					territoryReqNotif = !GetExpansionSettings().GetBaseBuilding().AllowBuildingWithoutATerritory;
					for ( int i = 0; i < GetExpansionSettings().GetBaseBuilding().DeployableOutsideATerritory.Count(); ++i )
					{
						string deployable = GetExpansionSettings().GetBaseBuilding().DeployableOutsideATerritory[i];
						if ( ( item.CanMakeGardenplot() && deployable == "GardenPlot" ) || item.IsKindOf( deployable ) )
						{
							buildZoneReqNotif = false;
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
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_TERRITORY_REQUIRED").Error(action_data.m_Player.GetIdentity());
		else if (enemyTerritoryNotif)
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ENEMY_TERRITORY").Error(action_data.m_Player.GetIdentity());
		else if (enemyTerritoryNearNotif)
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ENEMY_TERRITORY_NEAR").Error(action_data.m_Player.GetIdentity());
		else if (buildZoneReqNotif)
		{
			string buildZoneRequiredMsg = GetExpansionSettings().GetBaseBuilding().BuildZoneRequiredCustomMessage;
			if ( !buildZoneRequiredMsg )
				buildZoneRequiredMsg = "STR_EXPANSION_BUILD_ZONE_REQUIRED";
			ExpansionNotification("STR_EXPANSION_BUILD_ZONE_REQUIRED_TITLE", new StringLocaliser(buildZoneRequiredMsg)).Error(action_data.m_Player.GetIdentity());
		}
	}

	static bool CanDeployInTerritory( PlayerBase player, ItemBase item )
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