/**
 * dzn_map_namalsk.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef NAMALSK_SURVIVAL
modded class dzn_map_namalsk
{	
	private bool m_Added;

	// ------------------------------------------------------------
	// dzn_map_namalsk EEInventoryIn
	// ------------------------------------------------------------	
	override void EEInventoryIn(Man newParentMan, EntityAI diz, EntityAI newParent)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("dzn_map_namalsk::EEInventoryIn - Start");
		#endif

		super.EEInventoryIn( newParentMan, diz, newParent );
		
		if ( IsMissionClient() && newParentMan && newParentMan.IsInherited( SurvivorBase ) && !m_Added )
		{
			m_Added = true;
			
			ChernarusMap.UpdatePlayerHasItemMap(PlayerBase.Cast( newParentMan ), true);
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
		
		if ( IsMissionClient() && oldParentMan && oldParentMan.IsInherited( SurvivorBase ) && m_Added)
		{
			m_Added = false;
			ChernarusMap.UpdatePlayerHasItemMap(PlayerBase.Cast( oldParentMan ), false);
		}

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("dzn_map_namalsk::EEInventoryOut - End");
		#endif
	}
};
#endif
