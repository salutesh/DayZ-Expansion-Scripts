/**
 * Pen_ColorBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GENERAL_ITEMS, this, "EEInventoryIn");
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
					return;
				}
				
				player.SetHasItemPen( true );
			}
		}
	}
	
	// ------------------------------------------------------------
	// Pen_ColorBase EEInventoryOut
	// ------------------------------------------------------------	
	override void EEInventoryOut(Man oldParentMan, EntityAI diz, EntityAI newParent)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GENERAL_ITEMS, this, "EEInventoryOut");
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
					return;
				}
				
				player.SetHasItemPen( false );
			}
		}
	}
}