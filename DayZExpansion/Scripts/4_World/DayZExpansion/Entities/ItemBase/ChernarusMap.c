/**
 * ChernarusMap.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ChernarusMap
{
	// ------------------------------------------------------------
	// ChernarusMap PlayerInventoryCheckLocal
	// ------------------------------------------------------------
	private void PlayerInventoryCheckLocal()
	{
		if ( IsMissionClient() )
		{
			//! Get player who has this item
			if ( GetHierarchyRootPlayer() && GetHierarchyRootPlayer().IsKindOf("SurvivorBase") )
			{
				PlayerBase player = PlayerBase.Cast( GetHierarchyRootPlayer() );
				if ( player )
				{
					player.SetHasItemMap( true );
				}
			}
		}
	}
	
	// ------------------------------------------------------------
	// ChernarusMap EEInventoryIn
	// ------------------------------------------------------------	
	override void EEInventoryIn(Man newParentMan, EntityAI diz, EntityAI newParent)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::EEInventoryIn - Start");
		#endif
		
		super.EEInventoryIn( newParentMan, diz, newParent );
		
		if ( newParentMan && newParentMan.IsInherited( SurvivorBase ) )
		{
			if ( IsMissionClient() )
			{
				PlayerBase player = PlayerBase.Cast( newParentMan);
				
				if ( !player )
				{
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("ChernarusMap::EEInventoryIn - player: " + player.ToString());
					#endif

					return;
				}
				
				player.SetHasItemMap( true );
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::EEInventoryIn - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ChernarusMap EEInventoryOut
	// ------------------------------------------------------------	
	override void EEInventoryOut(Man oldParentMan, EntityAI diz, EntityAI newParent)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::EEInventoryOut - Start");
		#endif
		
		super.EEInventoryOut( oldParentMan, diz, newParent );
		
		if ( oldParentMan && oldParentMan.IsInherited( SurvivorBase ) )
		{
			if ( IsMissionClient() )
			{
				PlayerBase player = PlayerBase.Cast( oldParentMan );
				
				if ( !player )
				{
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("ChernarusMap::EEInventoryOut - player: " + player.ToString());
					#endif
					
					return;
				}
				
				player.SetHasItemMap( false );
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::EEInventoryOut - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ChernarusMap EEInit
	// ------------------------------------------------------------
	override void EEInit()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::EEInit - Start");
		#endif
		
		super.EEInit();
		
		PlayerInventoryCheckLocal();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::EEInit - End");
		#endif
	}
};
#ifdef NAMALSK_SURVIVAL
modded class dzn_map_namalsk
{
	// ------------------------------------------------------------
	// dzn_map_namalsk PlayerInventoryCheckLocal
	// ------------------------------------------------------------
	private void PlayerInventoryCheckLocal()
	{
		if ( IsMissionClient() )
		{
			//! Get player who has this item
			if ( GetHierarchyRootPlayer() && GetHierarchyRootPlayer().IsKindOf("SurvivorBase") )
			{
				PlayerBase player = PlayerBase.Cast( GetHierarchyRootPlayer() );
				if ( player )
				{
					player.SetHasItemMap( true );
				}
			}
		}
	}
	
	// ------------------------------------------------------------
	// dzn_map_namalsk EEInventoryIn
	// ------------------------------------------------------------	
	override void EEInventoryIn(Man newParentMan, EntityAI diz, EntityAI newParent)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("dzn_map_namalsk::EEInventoryIn - Start");
		#endif
		
		super.EEInventoryIn( newParentMan, diz, newParent );
		
		if ( newParentMan && newParentMan.IsInherited( SurvivorBase ) )
		{
			if ( IsMissionClient() )
			{
				PlayerBase player = PlayerBase.Cast( newParentMan);
				
				if ( !player )
				{
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("dzn_map_namalsk::EEInventoryIn - player: " + player.ToString());
					#endif

					return;
				}
				
				player.SetHasItemMap( true );
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("dzn_map_namalsk::EEInventoryIn - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// dzn_map_namalsk EEInventoryOut
	// ------------------------------------------------------------	
	override void EEInventoryOut(Man oldParentMan, EntityAI diz, EntityAI newParent)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("dzn_map_namalsk::EEInventoryOut - Start");
		#endif
		
		super.EEInventoryOut( oldParentMan, diz, newParent );
		
		if ( oldParentMan && oldParentMan.IsInherited( SurvivorBase ) )
		{
			if ( IsMissionClient() )
			{
				PlayerBase player = PlayerBase.Cast( oldParentMan );
				
				if ( !player )
				{
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("dzn_map_namalsk::EEInventoryOut - player: " + player.ToString());
					#endif
					
					return;
				}
				
				player.SetHasItemMap( false );
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("dzn_map_namalsk::EEInventoryOut - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// dzn_map_namalsk EEInit
	// ------------------------------------------------------------
	override void EEInit()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("dzn_map_namalsk::EEInit - Start");
		#endif
		
		super.EEInit();
		
		PlayerInventoryCheckLocal();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("dzn_map_namalsk::EEInit - End");
		#endif
	}
};
#endif