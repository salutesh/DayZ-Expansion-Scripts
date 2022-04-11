/**
 * ItemMap.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GENERAL_ITEMS, this, "EEInventoryIn");
#endif

		super.EEInventoryIn( newParentMan, diz, newParent );
		
		if ( IsMissionClient() && newParentMan && newParentMan.IsInherited( SurvivorBase ) && !m_Added )
		{
			m_Added = true;
			
			UpdatePlayerHasItemMap(PlayerBase.Cast( newParentMan ), true);
		}
	}
	
	// ------------------------------------------------------------
	// ChernarusMap EEInventoryOut
	// ------------------------------------------------------------	
	override void EEInventoryOut(Man oldParentMan, EntityAI diz, EntityAI newParent)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GENERAL_ITEMS, this, "EEInventoryOut");
#endif

		super.EEInventoryOut( oldParentMan, diz, newParent );
		
		if ( IsMissionClient() && oldParentMan && oldParentMan.IsInherited( SurvivorBase ) && m_Added )
		{
			m_Added = true;
			
			UpdatePlayerHasItemMap(PlayerBase.Cast( oldParentMan ), false);
		}
	}
	
	// ------------------------------------------------------------
	// ChernarusMap UpdatePlayerHasItemMap
	// ------------------------------------------------------------	
	static void UpdatePlayerHasItemMap(PlayerBase player, bool state)
	{
		if ( !player )
		{
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GENERAL_ITEMS, this, "OnInventoryExit");
#endif

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
	}
	
	// ------------------------------------------------------------
	// Compass OnInventoryExit
	// ------------------------------------------------------------
	override void OnInventoryEnter(Man player)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GENERAL_ITEMS, this, "OnInventoryEnter");
#endif

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
	}
};
