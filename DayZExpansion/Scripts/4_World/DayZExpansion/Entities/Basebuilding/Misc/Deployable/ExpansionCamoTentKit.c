/**
 * ExpansionCamoTentKit.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionCamoTentKit
 * @brief		This class handle base class of expansion basebuilding	
 **/
class ExpansionCamoTentKit: ExpansionKitLarge
{
	// ------------------------------------------------------------
	// OnPlacementComplete
	// ------------------------------------------------------------
	override void OnPlacementComplete( Man player )
	{
		if ( IsMissionHost() )
		{
			//! Create tent
			PlayerBase player_base = PlayerBase.Cast( player );
			vector position = player_base.GetLocalProjectionPosition();
			vector orientation = player_base.GetLocalProjectionOrientation();
			
			ExpansionCamoTent tent = ExpansionCamoTent.Cast( GetGame().CreateObject( "ExpansionCamoTent", GetPosition() ) );
			tent.SetPosition( position );
			tent.SetOrientation( orientation );
			
			//! Make the kit invisible, so it can be destroyed from deploy UA when action ends
			HideAllSelections();

			GetGame().ObjectDelete(this);
		}	
		
		SetIsDeploySound( true );
	}

	override string GetDeploySoundset()
	{
		return "placeLargeTent_SoundSet";
	}
	
	override string GetLoopDeploySoundset()
	{
		return "largetent_deploy_SoundSet";
	}
}