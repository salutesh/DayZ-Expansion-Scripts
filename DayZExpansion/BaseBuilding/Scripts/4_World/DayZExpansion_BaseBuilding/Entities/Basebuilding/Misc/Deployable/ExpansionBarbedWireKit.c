/**
 * ExpansionBarbedWireKit.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionBarbedWireKit
 * @brief		This class handle base class of expansion basebuilding	
 **/
class ExpansionBarbedWireKit: ExpansionKitLarge
{
	// ------------------------------------------------------------
	// OnPlacementComplete
	// ------------------------------------------------------------
	override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" )
	{
		if ( IsMissionHost() )
		{			
			ExpansionBarbedWire wire = ExpansionBarbedWire.Cast( GetGame().CreateObject( "ExpansionBarbedWire", GetPosition() ) );
			wire.SetPosition( position );
			wire.SetOrientation( orientation );
			// hesco.PlaceOnSurface();
			
			//! Make the kit invisible, so it can be destroyed from deploy UA when action ends
			HideAllSelections();

			GetGame().ObjectDelete(this);
		}	
		
		SetIsDeploySound( true );
	}
}