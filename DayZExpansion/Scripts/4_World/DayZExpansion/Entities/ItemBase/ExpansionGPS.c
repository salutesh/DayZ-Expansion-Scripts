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
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnWorkStart - Start");
		#endif
		
		if (GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()) )
		{
			m_IsWorking = true;
			m_HasEnergy = true;
			
			UpdatePlayerHasItemGPS(PlayerBase.Cast(GetHierarchyRootPlayer()), true);
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnWorkStart - End");
		#endif
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
				EXLogPrint("ExpansionGPS::OnWork - energy: " + energy);
									
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
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnWorkStop - Start");
		#endif
		
		if ( GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()) )
		{
			UpdatePlayerHasItemGPS(PlayerBase.Cast(GetHierarchyRootPlayer()), false);
					
			m_IsWorking = false;
			m_HasEnergy = false;
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnWorkStop - End");
		#endif
	} 
	
	// ------------------------------------------------------------
	// ExpansionGPS OnInventoryExit
	// ------------------------------------------------------------
	//! Inventory manipulation
	override void OnInventoryExit(Man player)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnInventoryExit - Start");
		#endif
		
		super.OnInventoryExit(player);
		
		if ( GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()) )
		{
			if ( GetCompEM().IsWorking() )
			{
				if (player)
				{
					vector ori_rotate = player.GetOrientation();
					ori_rotate = ori_rotate + Vector(270,0,0);
					SetOrientation(ori_rotate);
				}
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnInventoryExit - End");
		#endif
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
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::EEInventoryIn - Start");
		#endif
		
		super.EEInventoryIn( newParentMan, diz, newParent );
				
		if ( GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()) )
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionGPS::EEInventoryIn - m_HasEnergy: " + m_HasEnergy);
			EXLogPrint("ExpansionGPS::EEInventoryIn - m_IsWorking: " + m_IsWorking);
			#endif
			
			if (m_HasEnergy)
			{
				m_IsWorking = true;
				
				UpdatePlayerHasItemGPS(PlayerBase.Cast(GetHierarchyRootPlayer()), true);
				
				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint("ExpansionGPS::EEInventoryIn - m_IsWorking: " + m_IsWorking);
				#endif
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::EEInventoryIn - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionGPS EEInventoryOut
	// ------------------------------------------------------------	
	override void EEInventoryOut(Man oldParentMan, EntityAI diz, EntityAI newParent)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::EEInventoryOut - Start");
		#endif
		
		super.EEInventoryOut( oldParentMan, diz, newParent );
		
		if ( GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()) && oldParentMan && oldParentMan.IsInherited( SurvivorBase ) )
			UpdatePlayerHasItemGPS(PlayerBase.Cast(oldParentMan), false);
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::EEInventoryOut - End");
		#endif
	}
		
	// ------------------------------------------------------------
	// ExpansionGPS OnBatteryAttached
	// ------------------------------------------------------------	
	void OnBatteryAttached(EntityAI item, string slot, EntityAI parent)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnBatteryAttached - Start");
		#endif
		
		if ( GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()) )
		{
			if (item.IsKindOf("Battery9V"))
			{
				m_HasBattery = true;
	
				UpdatePlayerHasItemGPS(PlayerBase.Cast(GetHierarchyRootPlayer()), true);
			}	
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnBatteryAttached - End");
		#endif
	}
		
	// ------------------------------------------------------------
	// ExpansionGPS OnBatteryDetached
	// ------------------------------------------------------------		
	void OnBatteryDetached(EntityAI item, string slot, EntityAI parent)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnBatteryDetached - Start");
		#endif
		
		if ( GetGame() && (!GetGame().IsServer() || !GetGame().IsMultiplayer()) )
		{
			if (item.IsKindOf("Battery9V"))
			{				
				UpdatePlayerHasItemGPS(PlayerBase.Cast(GetHierarchyRootPlayer()), false);
				
				m_HasBattery = false;
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::OnBatteryDetached - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionGPS UpdatePlayerHasItemGPS
	// ------------------------------------------------------------		
	protected void UpdatePlayerHasItemGPS(PlayerBase player, bool state)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::UpdatePlayerHasItemGPS - Start");
		#endif
		
		if ( !player )
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionGPS::UpdatePlayerHasItemGPS - player: " + player.ToString());
			#endif
			
			return;
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::UpdatePlayerHasItemGPS - state: " + state);
		EXLogPrint("ExpansionGPS::UpdatePlayerHasItemGPS - m_HasBattery: " + m_HasBattery);
		EXLogPrint("ExpansionGPS::UpdatePlayerHasItemGPS - m_IsWorking: " + m_IsWorking);
		#endif
		
		if ( m_HasBattery && m_IsWorking )
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionGPS::UpdatePlayerHasItemGPS - SetHasItemGPS: " + state);
			EXLogPrint("ExpansionGPS::UpdatePlayerHasItemGPS - SetHasItemMap: " + state);
			#endif
			
			player.SetHasItemGPS( state );
			player.SetHasItemMap( state );
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::UpdatePlayerHasItemGPS - End");
		#endif
	}
}
