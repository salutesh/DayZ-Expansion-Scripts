/**
 * ExpansionRampKit.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionRampKit
 * @brief		This class handle base class of expansion basebuilding	
 **/
class ExpansionRampKit extends ExpansionKitLarge
{
	override void OnPlacementComplete( Man player )
	{
		if ( IsMissionHost() )
		{
			PlayerBase player_base = PlayerBase.Cast( player );
			vector position = player_base.GetLocalProjectionPosition();
			vector orientation = player_base.GetLocalProjectionOrientation();
			
			Object obj = GetGame().CreateObject( GetDeployType(), GetPosition() );
			obj.SetPosition( position );
			obj.SetOrientation( orientation );
			
			//! Make the kit invisible, so it can be destroyed from deploy UA when action ends
			HideAllSelections();
			
			SetIsDeploySound( true );
		}
	}
}