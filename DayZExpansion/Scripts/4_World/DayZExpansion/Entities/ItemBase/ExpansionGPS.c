/**
 * ExpansionGPS.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnWorkStart - Start");
		//#endif
		
		if (GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()))
		{
			m_IsWorking = true;
			
			m_HasEnergy = true;
			
			UpdatePlayerHasItemGPS(PlayerBase.Cast(GetHierarchyRootPlayer()), true);
		}
		
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnWorkStart - End");
		//#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionGPS OnWork
	// ------------------------------------------------------------
	override void OnWork( float consumed_energy )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnWork - Start");
		#endif
		
		if ( GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()) )
		{			
			Battery9V battery = Battery9V.Cast(GetCompEM().GetEnergySource());
			if (battery)
			{
				float energy = battery.GetEnergy();
				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint("ExpansionGPS::OnWork - energy: " + energy);
				#endif
									
				if ( energy > 0 )
				{
					m_HasEnergy = true;
					
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("ExpansionGPS::OnWork - m_HasEnergy: " + m_HasEnergy);
					#endif
				}
				else if ( energy <= 0 )
				{
					m_HasEnergy = false;
					
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("ExpansionGPS::OnWork - m_HasEnergy: " + m_HasEnergy);
					#endif
				}
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnWork - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionGPS OnWork
	// ------------------------------------------------------------
	override void OnWorkStop()
	{
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnWorkStop - Start");
		//#endif
		
		if ( GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()))
		{			
			m_IsWorking = false;
			
			m_HasEnergy = false;
			
			UpdatePlayerHasItemGPS(PlayerBase.Cast(GetHierarchyRootPlayer()), false);
		}
		
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnWorkStop - End");
		//#endif
	} 
	
	// ------------------------------------------------------------
	// ExpansionGPS OnInventoryExit
	// ------------------------------------------------------------
	//! Inventory manipulation
	override void OnInventoryExit(Man player)
	{
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnInventoryExit - Start");
		//#endif
			
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
		
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnInventoryExit - End");
		//#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionGPS OnInventoryExit
	// On login this method is called second after EEInventoryIn
	// ------------------------------------------------------------
	override void OnInventoryEnter(Man player)
	{
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnInventoryEnter - Start");
		//#endif
		
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
		
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnInventoryEnter - End");
		//#endif
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
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::EEInventoryIn - Start");
		//#endif
		
		super.EEInventoryIn( newParentMan, diz, newParent );
	
		if ( GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()) )
		{
			//#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionGPS::EEInventoryIn - Before - m_HasEnergy: " + m_HasEnergy);
			EXLogPrint("ExpansionGPS::EEInventoryIn - Before - m_IsWorking: " + m_IsWorking);
			EXLogPrint("ExpansionGPS::EEInventoryIn - Before - m_Dropped: " + m_Dropped);
			EXLogPrint("ExpansionGPS::EEInventoryIn - newParentMan: " + newParentMan.GetType());
			EXLogPrint("ExpansionGPS::EEInventoryIn - newParent: " + newParent.GetType());
			if (newParent && newParent.GetHierarchyRootPlayer())
				EXLogPrint("ExpansionGPS::EEInventoryIn - newParent.GetHierarchyRootPlayer(): " + newParent.GetHierarchyRootPlayer());
			//#endif
			
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
			
			EXLogPrint("ExpansionGPS::EEInventoryIn - After - m_HasEnergy: " + m_HasEnergy);
			EXLogPrint("ExpansionGPS::EEInventoryIn - After - m_IsWorking: " + m_IsWorking);
			EXLogPrint("ExpansionGPS::EEInventoryIn - After - m_Dropped: " + m_Dropped);
		}
			
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::EEInventoryIn - End");
		//#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionGPS EEInventoryOut
	// ------------------------------------------------------------	
	override void EEInventoryOut(Man oldParentMan, EntityAI diz, EntityAI newParent)
	{
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::EEInventoryOut - Start");
		//#endif

		super.EEInventoryOut( oldParentMan, diz, newParent );

		if ( GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()) )
		{
			//#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionGPS::EEInventoryOut - m_HasEnergy: " + m_HasEnergy);
			EXLogPrint("ExpansionGPS::EEInventoryOut - m_IsWorking: " + m_IsWorking);
			EXLogPrint("ExpansionGPS::EEInventoryOut - m_Dropped: " + m_Dropped);
			//#endif
			
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
		
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::EEInventoryOut - End");
		//#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionGPS OnMovedWithinCargo
	// ------------------------------------------------------------	
	override void OnMovedWithinCargo(EntityAI container)
	{	
		EXLogPrint("ExpansionGPS::OnMovedWithinCargo - Start");
		
		super.OnMovedWithinCargo(container);
		
		EXLogPrint("ExpansionGPS::OnMovedWithinCargo - container: " + container.GetType());
		
		EXLogPrint("ExpansionGPS::OnMovedWithinCargo - End");
	}
	
	// ------------------------------------------------------------
	// ExpansionGPS OnBatteryAttached
	// ------------------------------------------------------------	
	void OnBatteryAttached(EntityAI item, string slot, EntityAI parent)
	{
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnBatteryAttached - Start");
		//#endif
		
		if ( GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()) )
		{
			if (item.IsKindOf("Battery9V"))
			{
				m_HasBattery = true;
	
				UpdatePlayerHasItemGPS(PlayerBase.Cast(GetHierarchyRootPlayer()), true);
			}	
		}
		
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnBatteryAttached - End");
		//#endif
	}
		
	// ------------------------------------------------------------
	// ExpansionGPS OnBatteryDetached
	// ------------------------------------------------------------		
	void OnBatteryDetached(EntityAI item, string slot, EntityAI parent)
	{
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnBatteryDetached - Start");
		//#endif
		
		if ( GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()) )
		{
			if (item.IsKindOf("Battery9V") && m_Added)
			{
				m_HasBattery = false;
				
				UpdatePlayerHasItemGPS(PlayerBase.Cast(GetHierarchyRootPlayer()), false);
			}
		}
		
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnBatteryDetached - End");
		//#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionGPS UpdatePlayerHasItemGPS
	// ------------------------------------------------------------		
	protected void UpdatePlayerHasItemGPS(PlayerBase player, bool state)
	{
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::UpdatePlayerHasItemGPS - Start");
		//#endif
		
		if ( !player )
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionGPS::UpdatePlayerHasItemGPS - player: " + player.ToString());
			#endif
			
			return;
		}
		
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::UpdatePlayerHasItemGPS - state: " + state);
		EXLogPrint("ExpansionGPS::UpdatePlayerHasItemGPS - m_HasBattery: " + m_HasBattery);
		EXLogPrint("ExpansionGPS::UpdatePlayerHasItemGPS - m_IsWorking: " + m_IsWorking);
		EXLogPrint("ExpansionGPS::UpdatePlayerHasItemGPS - m_Dropped: " + m_Dropped);
		//#endif
		
		if ( m_HasBattery && m_IsWorking && !m_Dropped && state && !m_Added)
		{
			//#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionGPS::UpdatePlayerHasItemGPS - SetHasItemGPS: " + state);
			EXLogPrint("ExpansionGPS::UpdatePlayerHasItemGPS - SetHasItemMap: " + state);
			//#endif
			
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
		
		//#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::UpdatePlayerHasItemGPS - End");
		//#endif
	}
}
