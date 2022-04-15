/**
 * ExpansionGPS.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionGPS extends ToolBase
{
	private bool m_HasBattery = false;
	private bool m_IsWorking = false;
	private bool m_HasEnergy = false;
	private bool m_Dropped = false;
	private bool m_Added = false;
	
	// ------------------------------------------------------------
	// ExpansionGPS Constructor
	// ------------------------------------------------------------		
	void ExpansionGPS()
	{
		if (GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()) )
		{
			GetOnItemAttached().Insert(OnBatteryAttached);
			GetOnItemDetached().Insert(OnBatteryDetached);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionGPS Destructor
	// ------------------------------------------------------------
	void ~ExpansionGPS()
	{
		if (GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()) )
		{
			GetOnItemAttached().Remove(OnBatteryAttached);
			GetOnItemDetached().Remove(OnBatteryDetached);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionGPS OnWorkStart
	// ------------------------------------------------------------	
	override void OnWorkStart()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GENERAL_ITEMS, this, "OnWorkStart");
#endif

		if (GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()))
		{
			m_IsWorking = true;
			
			m_HasEnergy = true;
			
			UpdatePlayerHasItemGPS(PlayerBase.Cast(GetHierarchyRootPlayer()), true);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionGPS OnWork
	// ------------------------------------------------------------
	override void OnWork( float consumed_energy )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GENERAL_ITEMS, this, "OnWork");
#endif

		if ( GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()) )
		{			
			Battery9V battery = Battery9V.Cast(GetCompEM().GetEnergySource());
			if (battery)
			{
				float energy = battery.GetEnergy();
									
				if ( energy > 0 )
				{
					m_HasEnergy = true;
				}
				else if ( energy <= 0 )
				{
					m_HasEnergy = false;
				}
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionGPS OnWork
	// ------------------------------------------------------------
	override void OnWorkStop()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GENERAL_ITEMS, this, "OnWorkStop");
#endif

		if ( GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()))
		{			
			m_IsWorking = false;
			
			m_HasEnergy = false;
			
			UpdatePlayerHasItemGPS(PlayerBase.Cast(GetHierarchyRootPlayer()), false);
		}
	} 
	
	// ------------------------------------------------------------
	// ExpansionGPS OnInventoryExit
	// ------------------------------------------------------------
	//! Inventory manipulation
	override void OnInventoryExit(Man player)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GENERAL_ITEMS, this, "OnInventoryExit");
#endif

		super.OnInventoryExit(player);
		
		EXLogPrint("ExpansionGPS::OnInventoryExit - Player: " + player.GetType());
		EXLogPrint("ExpansionGPS::OnInventoryExit - Parent: " + GetParent());
		EXLogPrint("ExpansionGPS::OnInventoryExit - Hirachy Parent: " + GetHierarchyParent());
		EXLogPrint("ExpansionGPS::OnInventoryExit - Hirachy Root Player: " + GetHierarchyRootPlayer());
		
		if ( GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()) )
		{
			m_Dropped = true;
			
			UpdatePlayerHasItemGPS(PlayerBase.Cast(player), false);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionGPS OnInventoryExit
	// On login this method is called second after EEInventoryIn
	// ------------------------------------------------------------
	override void OnInventoryEnter(Man player)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GENERAL_ITEMS, this, "OnInventoryEnter");
#endif
		
		super.OnInventoryEnter(player);
		
		if ( GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()) )
		{
			m_Dropped = false;
			
			if (m_HasEnergy)
			{
				m_IsWorking = true;
				
				UpdatePlayerHasItemGPS(PlayerBase.Cast(player), true);
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionGPS SetActions
	// ------------------------------------------------------------
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionTurnOnWhileInHands);
		AddAction(ActionTurnOffWhileInHands);
	}
	
	// ------------------------------------------------------------
	// ExpansionGPS EEInventoryIn
	// ------------------------------------------------------------	
	override void EEInventoryIn(Man newParentMan, EntityAI diz, EntityAI newParent)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GENERAL_ITEMS, this, "EEInventoryIn");
#endif

		super.EEInventoryIn( newParentMan, diz, newParent );
	
		if ( GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()) )
		{
			CF_Log.Debug("ExpansionGPS::EEInventoryIn - Before - m_HasEnergy: " + m_HasEnergy);
			CF_Log.Debug("ExpansionGPS::EEInventoryIn - Before - m_IsWorking: " + m_IsWorking);
			CF_Log.Debug("ExpansionGPS::EEInventoryIn - Before - m_Dropped: " + m_Dropped);
			CF_Log.Debug("ExpansionGPS::EEInventoryIn - newParentMan: " + newParentMan.GetType());
			CF_Log.Debug("ExpansionGPS::EEInventoryIn - newParent: " + newParent.GetType());
			if (newParent && newParent.GetHierarchyRootPlayer())
				CF_Log.Debug("ExpansionGPS::EEInventoryIn - newParent.GetHierarchyRootPlayer(): " + newParent.GetHierarchyRootPlayer());
			
			m_Dropped = false;
			
			PlayerBase parentPlayer;
			if (newParentMan)
			{				
				if (m_HasEnergy)
				{
					m_IsWorking = true;
					
					parentPlayer = PlayerBase.Cast(newParentMan);
					UpdatePlayerHasItemGPS(parentPlayer, true);
				}
			}
			
			if (newParent)
			{
				
				if (m_HasEnergy)
				{
					m_IsWorking = true;
										
					if (newParent.GetHierarchyRootPlayer())
					{
						parentPlayer = PlayerBase.Cast(newParent.GetHierarchyRootPlayer());
						UpdatePlayerHasItemGPS(parentPlayer, true);
					}
				}
			}
			
			CF_Log.Debug("ExpansionGPS::EEInventoryIn - After - m_HasEnergy: " + m_HasEnergy);
			CF_Log.Debug("ExpansionGPS::EEInventoryIn - After - m_IsWorking: " + m_IsWorking);
			CF_Log.Debug("ExpansionGPS::EEInventoryIn - After - m_Dropped: " + m_Dropped);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionGPS EEInventoryOut
	// ------------------------------------------------------------	
	override void EEInventoryOut(Man oldParentMan, EntityAI diz, EntityAI newParent)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GENERAL_ITEMS, this, "EEInventoryOut");
#endif

		super.EEInventoryOut( oldParentMan, diz, newParent );

		if ( GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()) )
		{
			CF_Log.Debug("ExpansionGPS::EEInventoryOut - m_HasEnergy: " + m_HasEnergy);
			CF_Log.Debug("ExpansionGPS::EEInventoryOut - m_IsWorking: " + m_IsWorking);
			CF_Log.Debug("ExpansionGPS::EEInventoryOut - m_Dropped: " + m_Dropped);
			
			ItemBase item;
			PlayerBase parentPlayer;
			if (Class.CastTo(parentPlayer, oldParentMan))
			{
				m_Dropped = true;
					
				UpdatePlayerHasItemGPS(parentPlayer, false);
			}
			
			if (GetHierarchyParent())
			{
				if (Class.CastTo(item, GetHierarchyParent()))
				{
					if (Class.CastTo(parentPlayer, GetHierarchyParent().GetHierarchyRootPlayer()))
					{
						if (m_HasEnergy)
						{
							m_Dropped = true;
							
							UpdatePlayerHasItemGPS(parentPlayer, false);
						}
					}
				}
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionGPS OnMovedWithinCargo
	// ------------------------------------------------------------	
	override void OnMovedWithinCargo(EntityAI container)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GENERAL_ITEMS, this, "OnMovedWithinCargo");
#endif

		super.OnMovedWithinCargo(container);
		
		CF_Log.Debug("ExpansionGPS::OnMovedWithinCargo - container: " + container.GetType());
	}
	
	// ------------------------------------------------------------
	// ExpansionGPS OnBatteryAttached
	// ------------------------------------------------------------	
	void OnBatteryAttached(EntityAI item, string slot, EntityAI parent)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GENERAL_ITEMS, this, "OnBatteryAttached");
#endif

		if ( GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()) )
		{
			if (item.IsKindOf("Battery9V"))
			{
				m_HasBattery = true;
	
				UpdatePlayerHasItemGPS(PlayerBase.Cast(GetHierarchyRootPlayer()), true);
			}	
		}
	}
		
	// ------------------------------------------------------------
	// ExpansionGPS OnBatteryDetached
	// ------------------------------------------------------------		
	void OnBatteryDetached(EntityAI item, string slot, EntityAI parent)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GENERAL_ITEMS, this, "OnBatteryDetached");
#endif

		if ( GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()) )
		{
			if (item.IsKindOf("Battery9V") && m_Added)
			{
				m_HasBattery = false;
				
				UpdatePlayerHasItemGPS(PlayerBase.Cast(GetHierarchyRootPlayer()), false);
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionGPS UpdatePlayerHasItemGPS
	// ------------------------------------------------------------		
	protected void UpdatePlayerHasItemGPS(PlayerBase player, bool state)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GENERAL_ITEMS, this, "UpdatePlayerHasItemGPS");
#endif

		if ( !player )
		{			
			return;
		}
		
		CF_Log.Debug("ExpansionGPS::UpdatePlayerHasItemGPS - state: " + state);
		CF_Log.Debug("ExpansionGPS::UpdatePlayerHasItemGPS - m_HasBattery: " + m_HasBattery);
		CF_Log.Debug("ExpansionGPS::UpdatePlayerHasItemGPS - m_IsWorking: " + m_IsWorking);
		CF_Log.Debug("ExpansionGPS::UpdatePlayerHasItemGPS - m_Dropped: " + m_Dropped);
		
		if ( m_HasBattery && m_IsWorking && !m_Dropped && state && !m_Added)
		{
			CF_Log.Debug("ExpansionGPS::UpdatePlayerHasItemGPS - SetHasItemGPS: " + state);
			CF_Log.Debug("ExpansionGPS::UpdatePlayerHasItemGPS - SetHasItemMap: " + state);
			
			m_Added = true;
			
			player.SetHasItemGPS( true );
			
			player.SetHasItemMap( true );
		}
		
		if ( (!m_HasBattery || !m_IsWorking || m_Dropped) && !state && m_Added )
		{
			m_Added = false;
			
			player.SetHasItemGPS( false );
			
			player.SetHasItemMap( false );
		}
	}
};
