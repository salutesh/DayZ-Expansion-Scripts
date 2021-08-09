/**
 * ItemMap.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ChernarusMap
{	
	// ------------------------------------------------------------
	// ChernarusMap EEInventoryIn
	// ------------------------------------------------------------	
	override void EEInventoryIn(Man newParentMan, EntityAI diz, EntityAI newParent)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::EEInventoryIn - Start");
		#endif
		
		super.EEInventoryIn( newParentMan, diz, newParent );
		
		if ( IsMissionClient() && newParentMan && newParentMan.IsInherited( SurvivorBase ) )
			UpdatePlayerHasItemMap(PlayerBase.Cast( newParentMan ), true);
		
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
		
		if ( IsMissionClient() && oldParentMan && oldParentMan.IsInherited( SurvivorBase ) )
			UpdatePlayerHasItemMap(PlayerBase.Cast( oldParentMan ), false);
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::EEInventoryOut - End");
		#endif
	}

	static void UpdatePlayerHasItemMap(PlayerBase player, bool state)
	{
		if ( !player )
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ChernarusMap::UpdatePlayerHasItemMap - player: " + player.ToString());
			#endif
			
			return;
		}
		
		player.SetHasItemMap( state );
	}
};
#ifdef NAMALSK_SURVIVAL
modded class dzn_map_namalsk
{	
	// ------------------------------------------------------------
	// dzn_map_namalsk EEInventoryIn
	// ------------------------------------------------------------	
	override void EEInventoryIn(Man newParentMan, EntityAI diz, EntityAI newParent)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("dzn_map_namalsk::EEInventoryIn - Start");
		#endif

		super.EEInventoryIn( newParentMan, diz, newParent );
		
		if ( IsMissionClient() && newParentMan && newParentMan.IsInherited( SurvivorBase ) )
			ChernarusMap.UpdatePlayerHasItemMap(PlayerBase.Cast( newParentMan ), true);

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
		
		if ( IsMissionClient() && oldParentMan && oldParentMan.IsInherited( SurvivorBase ) )
			ChernarusMap.UpdatePlayerHasItemMap(PlayerBase.Cast( oldParentMan ), false);

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("dzn_map_namalsk::EEInventoryOut - End");
		#endif
	}
};
#endif
