#ifdef DAYZ_1_09
modded class TerritoryFlagKit
{	
	ref Timer m_BuildFlagTimer;
	
	void TerritoryFlagKit()
	{
		
	}
	
	void ~TerritoryFlagKit()
	{

	}
	
	override void OnPlacementComplete( Man player )
	{
		if ( GetGame().IsServer() )
		{
			//Create TerritoryFlag
			PlayerBase player_base = PlayerBase.Cast( player );
			vector position = player_base.GetLocalProjectionPosition();
			vector orientation = player_base.GetLocalProjectionOrientation();
			
			TerritoryFlag totem = TerritoryFlag.Cast( GetGame().CreateObjectEx( "TerritoryFlag", GetPosition(), ECE_PLACE_ON_SURFACE ) );
			totem.SetPosition( position );
			totem.SetOrientation( orientation );
			totem.SetOwnerID( player_base.GetIdentityUID() );
			totem.SetIsExpansionTerritoryFlag( false );
			
			//make the kit invisible, so it can be destroyed from deploy UA when action ends
			HideAllSelections();
			
			SetIsDeploySound( true );
			
			//! If the server hoster doesn't want to build with the vanilla way the entire flag pole
			if (GetExpansionSettings().GetBaseBuilding().EnableSimpleFlagBuilding)
			{				
				totem.GetConstruction().COT_BuildPart( "base", false );
				totem.GetConstruction().COT_BuildPart( "support", false );
				totem.GetConstruction().COT_BuildPart( "pole", false );
								
				if ( GetExpansionSettings().GetBaseBuilding().AddFlagItem )
					totem.GetInventory().CreateAttachment("Flag_DayZ");
			}
		}
	}
}
#endif