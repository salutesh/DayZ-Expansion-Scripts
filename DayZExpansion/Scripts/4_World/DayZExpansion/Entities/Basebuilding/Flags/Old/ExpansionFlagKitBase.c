/**
 * ExpansionFlagKitBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionFlagKitBase
 * @brief		
 **/

class ExpansionFlagKitBase extends ExpansionKitSmall
{
	// ------------------------------------------------------------
	// CanBePlaced
	// ------------------------------------------------------------
	override bool CanBePlaced( Man player, vector position )
	{
		PlayerBase player_base = PlayerBase.Cast( player );

		if ( super.CanBePlaced( player, position ) && player_base )
		{
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// OnPlacementComplete
	// ------------------------------------------------------------
	override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" )
	{
		//! ON SERVER ONLY
		if ( IsMissionHost() )
		{
			//! Create ExpansionFlagBase flag
			PlayerBase player_base = PlayerBase.Cast( player );
			
			ExpansionFlagBase flag = ExpansionFlagBase.Cast( GetGame().CreateObject( GetFlagObject(), GetPosition() ) );
			flag.SetPosition( position );
			flag.SetOrientation( orientation );
			flag.SetFlagTexture( GetTexturePath() );
			flag.SetOwnerID( player_base.GetIdentityUID() );
			
			//player_base.AddFlag( flag );

			//! Make the kit invisible, so it can be destroyed from deploy UA when action ends
			HideAllSelections();
			SetIsDeploySound( true );
			GetGame().ObjectDelete(this);
		}
	}

	string GetFlagObject()
	{
		return "ExpansionFlagBase";
	}
		
	// ------------------------------------------------------------
	// Expansion GetTexturePath
	// ------------------------------------------------------------
	string GetTexturePath()
	{
		return EXPANSION_FLAG_LOGO_EXPANSION;
	}

	// ------------------------------------------------------------
	// Override AfterStoreLoad
	// ------------------------------------------------------------	
	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();
		
		PlayerBase player = PlayerBase.Cast( GetHierarchyRootPlayer() );
		InventoryLocation il = new InventoryLocation;
		if ( player && GetInventory().GetCurrentInventoryLocation( il ) )
		{
			if ( il.GetType() == InventoryLocationType.HANDS )
			{
				MiscGameplayFunctions.TurnItemIntoItem( this, "TerritoryFlagKit", player );
			} else
			{
				MiscGameplayFunctions.TurnItemInHandsIntoItem( this, "TerritoryFlagKit", player );
			}
		} else
		{
			vector position = GetPosition();
			vector orientation = GetOrientation();

			GetGame().ObjectDelete( this );

			Object obj = GetGame().CreateObjectEx( "TerritoryFlagKit", position, ECE_PLACE_ON_SURFACE );
			obj.SetPosition( position );
			obj.SetOrientation( orientation );
		}
	}
}