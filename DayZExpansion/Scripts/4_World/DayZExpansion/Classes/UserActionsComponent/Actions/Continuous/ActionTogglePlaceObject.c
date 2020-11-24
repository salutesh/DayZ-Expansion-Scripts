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

		PlayerBase player = action_data.m_Player;
		ItemBase item = action_data.m_MainItem;
		bool territoryReqNotif = false;
		bool enemyTerritoryNotif = false;
		int i;
 		if ( GetExpansionSettings().GetBaseBuilding() )
		{
			if (GetExpansionSettings().GetBaseBuilding().AllowBuildingWithoutATerritory)
			{		
				if ( player.IsInTerritory() )
				{
					if (!player.IsInsideOwnTerritory())
					{
						enemyTerritoryNotif = true;
						for (i = 0; i < GetExpansionSettings().GetBaseBuilding().DeployableInsideAEnemyTerritory.Count(); ++i)
						{
							if (item.IsKindOf(GetExpansionSettings().GetBaseBuilding().DeployableInsideAEnemyTerritory[i]))
							{
								enemyTerritoryNotif = false;
							}
						}
					}
				}
			} else
			{
				territoryReqNotif = true;
				//Place stuff other than flag should be possible inside your territory
				if ( player.IsInTerritory() )
				{
					territoryReqNotif = false;
					if (!player.IsInsideOwnTerritory())
					{
						enemyTerritoryNotif = true;
						for (i = 0; i < GetExpansionSettings().GetBaseBuilding().DeployableInsideAEnemyTerritory.Count(); ++i)
						{
							if (item.IsKindOf(GetExpansionSettings().GetBaseBuilding().DeployableInsideAEnemyTerritory[i]))
							{
								enemyTerritoryNotif = false;
							}
						}
					
					}
				} else
				{
					for (i = 0; i < GetExpansionSettings().GetBaseBuilding().DeployableOutsideATerritory.Count(); ++i)
					{
						if (item.IsKindOf(GetExpansionSettings().GetBaseBuilding().DeployableOutsideATerritory[i]))
						{
							territoryReqNotif = false;
						}
					}
				}
			}
		}
		if (territoryReqNotif)
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_TERRITORY_REQUIRED" ), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, action_data.m_Player.GetIdentity() );
		
		if (enemyTerritoryNotif)
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ENEMY_TERRITORY" ), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, action_data.m_Player.GetIdentity() );	
	}

}