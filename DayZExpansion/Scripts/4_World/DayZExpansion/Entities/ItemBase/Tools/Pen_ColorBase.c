/**
 * Pen_ColorBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Pen_ColorBase
{	
	// ------------------------------------------------------------
	// Pen_ColorBase EEInventoryIn
	// ------------------------------------------------------------	
	override void EEInventoryIn(Man newParentMan, EntityAI diz, EntityAI newParent)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Pen_ColorBase::EEInventoryIn - Start");
		#endif
		
		super.EEInventoryIn( newParentMan, diz, newParent );
		
		if ( IsMissionClient() )
		{
			if ( newParentMan && newParentMan.IsInherited( SurvivorBase ) )
			{
				PlayerBase player = PlayerBase.Cast( newParentMan);
			
				if (!player)
					player = PlayerBase.Cast(GetHierarchyRootPlayer());
				
				if ( !player )
				{
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("Pen_ColorBase::EEInventoryIn - player: " + player.ToString());
					#endif
	
					return;
				}
				
				player.SetHasItemPen( true );
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Pen_ColorBase::EEInventoryIn - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Pen_ColorBase EEInventoryOut
	// ------------------------------------------------------------	
	override void EEInventoryOut(Man oldParentMan, EntityAI diz, EntityAI newParent)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Pen_ColorBase::EEInventoryOut - Start");
		#endif
		
		super.EEInventoryOut( oldParentMan, diz, newParent );
		
		if ( IsMissionClient() )
		{
			if ( oldParentMan && oldParentMan.IsInherited( SurvivorBase ) )
			{
				PlayerBase player = PlayerBase.Cast( oldParentMan );
				
				if (!player)
					player = PlayerBase.Cast(GetHierarchyRootPlayer());
				
				if ( !player )
				{
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("Pen_ColorBase::EEInventoryOut - player: " + player.ToString());
					#endif
					
					return;
				}
				
				player.SetHasItemPen( false );
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Pen_ColorBase::EEInventoryOut - End");
		#endif
	}
}