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
	private bool m_Added = false;
	
	// ------------------------------------------------------------
	// ChernarusMap EEInventoryIn
	// ------------------------------------------------------------	
	override void EEInventoryIn(Man newParentMan, EntityAI diz, EntityAI newParent)
	{
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::EEInventoryIn - Start");
		//#endif
		
		super.EEInventoryIn( newParentMan, diz, newParent );
		
		if ( IsMissionClient() && newParentMan && newParentMan.IsInherited( SurvivorBase ) && !m_Added )
		{
			m_Added = true;
			
			UpdatePlayerHasItemMap(PlayerBase.Cast( newParentMan ), true);
		}
		
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::EEInventoryIn - End");
		//#endif
	}
	
	// ------------------------------------------------------------
	// ChernarusMap EEInventoryOut
	// ------------------------------------------------------------	
	override void EEInventoryOut(Man oldParentMan, EntityAI diz, EntityAI newParent)
	{
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::EEInventoryOut - Start");
		//#endif
		
		super.EEInventoryOut( oldParentMan, diz, newParent );
		
		if ( IsMissionClient() && oldParentMan && oldParentMan.IsInherited( SurvivorBase ) && m_Added )
		{
			m_Added = true;
			
			UpdatePlayerHasItemMap(PlayerBase.Cast( oldParentMan ), false);
		}
		
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::EEInventoryOut - End");
		//#endif
	}
	
	// ------------------------------------------------------------
	// ChernarusMap UpdatePlayerHasItemMap
	// ------------------------------------------------------------	
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
	
	// ------------------------------------------------------------
	// Compass OnInventoryExit
	// ------------------------------------------------------------
	//! Inventory manipulation
	override void OnInventoryExit(Man player)
	{
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Compass::OnInventoryExit - Start");
		//#endif
			
		super.OnInventoryExit(player);
		
		if ( GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()) )
		{
			if (GetHierarchyParent())
			{
				ItemBase item;
				if (Class.CastTo(item, GetHierarchyParent()))
				{
					PlayerBase parentPlayer;
					if (!Class.CastTo(parentPlayer, item.GetHierarchyRootPlayer()) && m_Added)
					{
						m_Added = false;
						
						UpdatePlayerHasItemMap(PlayerBase.Cast( player ), false);
					}
					else if (Class.CastTo(parentPlayer, item.GetHierarchyRootPlayer()) && m_Added)
					{
						if (parentPlayer != player)
						{
							PlayerBase playerBase;
							if (!Class.CastTo(playerBase, player) && m_Added)
							{
								m_Added = false;
								
								UpdatePlayerHasItemMap(playerBase, false);
							}
						}
					}
				}
			}
		}
	
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Compass::OnInventoryExit - End");
		//#endif
	}
	
	// ------------------------------------------------------------
	// Compass OnInventoryExit
	// ------------------------------------------------------------
	override void OnInventoryEnter(Man player)
	{
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Compass::OnInventoryEnter - Start");
		//#endif
			
		super.OnInventoryEnter(player);
		
		if ( GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()) )
		{
			if (GetHierarchyParent())
			{
				ItemBase item;
				if (Class.CastTo(item, GetHierarchyParent()))
				{
					PlayerBase parentPlayer;
					if (Class.CastTo(parentPlayer, item.GetHierarchyRootPlayer()) && !m_Added)
					{
						m_Added = true;
						
						UpdatePlayerHasItemMap(parentPlayer, true);
					}
				}
			}
		}
		
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Compass::OnInventoryEnter - End");
		//#endif
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
