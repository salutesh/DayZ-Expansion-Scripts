/**
 * Compass.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Compass
{	
	// ------------------------------------------------------------
	// Compass EEInventoryIn
	// ------------------------------------------------------------	
	override void EEInventoryIn(Man newParentMan, EntityAI diz, EntityAI newParent)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Compass::EEInventoryIn - Start");
		#endif
		
		super.EEInventoryIn( newParentMan, diz, newParent );
		
		if ( IsMissionClient() )
		{
			if ( newParentMan && newParentMan.IsInherited( SurvivorBase ) )
			{
				PlayerBase player = PlayerBase.Cast( newParentMan);
				
				if ( !player )
				{
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("Compass::EEInventoryIn - player: " + player.ToString());
					#endif

					return;
				}
				
				player.SetHasItemCompass( true );
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Compass::EEInventoryIn - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Compass EEInventoryOut
	// ------------------------------------------------------------	
	override void EEInventoryOut(Man oldParentMan, EntityAI diz, EntityAI newParent)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Compass::EEInventoryOut - Start");
		#endif
		
		super.EEInventoryOut( oldParentMan, diz, newParent );
		
		if ( IsMissionClient() )
		{
			if ( oldParentMan && oldParentMan.IsInherited( SurvivorBase ) )
			{
				PlayerBase player = PlayerBase.Cast( oldParentMan );
				
				if ( !player )
				{
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("Compass::EEInventoryOut - player: " + player.ToString());
					#endif
					
					return;
				}
				
				player.SetHasItemCompass( false );
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Compass::EEInventoryOut - End");
		#endif
	}
}