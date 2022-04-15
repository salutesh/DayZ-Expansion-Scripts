/**
 * OrienteeringCompass.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class OrienteeringCompass
{	
	private bool m_Added = false;
	
	// ------------------------------------------------------------
	// OrienteeringCompass EEInventoryIn
	// ------------------------------------------------------------	
	override void EEInventoryIn(Man newParentMan, EntityAI diz, EntityAI newParent)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GENERAL_ITEMS, this, "EEInventoryIn");
#endif

		super.EEInventoryIn( newParentMan, diz, newParent );
		
		if ( IsMissionClient() && newParentMan && newParentMan.IsInherited( SurvivorBase ) )
		{
			PlayerBase newParentBase;
			if (Class.CastTo(newParentBase,newParentMan) && !m_Added)
			{
				m_Added = true;
				
				newParentBase.SetHasItemCompass( true );
			}
		}
	}
	
	// ------------------------------------------------------------
	// OrienteeringCompass EEInventoryOut
	// ------------------------------------------------------------	
	override void EEInventoryOut(Man oldParentMan, EntityAI diz, EntityAI newParent)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GENERAL_ITEMS, this, "EEInventoryOut");
#endif

		super.EEInventoryOut( oldParentMan, diz, newParent );
		
		if ( IsMissionClient() && oldParentMan && oldParentMan.IsInherited( SurvivorBase ) )
		{
			PlayerBase oldParentBase;
			if (Class.CastTo(oldParentBase, oldParentMan) && m_Added)
			{
				m_Added = false;
				
				oldParentBase.SetHasItemCompass( false );
			}
		}
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
				PlayerBase playerBase;
				if (Class.CastTo(item, GetHierarchyParent()))
				{
					PlayerBase parentPlayer;
					if (!Class.CastTo(parentPlayer, item.GetHierarchyRootPlayer()) && m_Added)
					{
						PlayerBase.Cast(player).SetHasItemCompass( false );
						m_Added = false;
					}
					else if (Class.CastTo(parentPlayer, item.GetHierarchyRootPlayer()) && m_Added)
					{
						if (parentPlayer != player)
						{
							PlayerBase parentPlayerBase;
							if (!Class.CastTo(parentPlayerBase, player))
							{
								m_Added = false;
								
								parentPlayerBase.SetHasItemCompass( false );
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
						
						parentPlayer.SetHasItemCompass( true );
					}
				}
			}
		}
	}
};
