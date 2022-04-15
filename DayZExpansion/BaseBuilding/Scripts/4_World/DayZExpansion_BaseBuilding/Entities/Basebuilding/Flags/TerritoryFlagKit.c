/**
 * TerritoryFlagKit.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class TerritoryFlagKit
{	
	ref Timer m_BuildFlagTimer;
	
	void TerritoryFlagKit()
	{
		
	}
	
	void ~TerritoryFlagKit()
	{

	}
	
	override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" )
	{
		if ( GetGame().IsServer() )
		{
			//Create TerritoryFlag
			PlayerBase player_base = PlayerBase.Cast( player );
			
			TerritoryFlag totem = TerritoryFlag.Cast( GetGame().CreateObjectEx( "TerritoryFlag", GetPosition(), ECE_PLACE_ON_SURFACE ) );
			totem.SetPosition( position );
			totem.SetOrientation( orientation );
			totem.SetOwnerID( player_base.GetIdentityUID() );
			totem.SetIsExpansionTerritoryFlag( false );
			
			//make the kit invisible, so it can be destroyed from deploy UA when action ends
			HideAllSelections();
			
			SetIsDeploySound( true );
			
			//! If the server hoster doesn't want to build with the vanilla way the entire flag pole
			if (GetExpansionSettings().GetBaseBuilding().SimpleTerritory)
			{
				totem.GetConstruction().AdminBuildPartServer( player_base, "base", AT_BUILD_PART );
				totem.GetConstruction().AdminBuildPartServer( player_base, "support", AT_BUILD_PART );
				totem.GetConstruction().AdminBuildPartServer( player_base, "pole", AT_BUILD_PART );
			}
		}
	}
};