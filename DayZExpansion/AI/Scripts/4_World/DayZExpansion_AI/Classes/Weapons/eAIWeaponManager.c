/**
 * eAIWeaponManager.c
 * 
 * Partly based on Enfusion AI Project Copyright 2021 William Bowers
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class eAIWeaponManager: WeaponManager
{
	override bool StartAction(int action, Magazine mag, InventoryLocation il, ActionBase control_action = NULL)
	{
		//if it is controled by action inventory reservation and synchronization provide action itself
		if(control_action)
		{
			m_ControlAction = ActionBase.Cast(control_action);
			m_PendingWeaponAction = action;
			m_InProgress = true;
			m_IsEventSended = false;
			m_PendingTargetMagazine = mag;
			m_PendingInventoryLocation = il;
			StartPendingAction();
			
			return true;
		}
		
		if ( !InventoryReservation(mag, il) )
			return false;

		m_PendingWeaponAction = action;
		m_InProgress = true;
		m_IsEventSended = false;

		m_readyToStart = true;
		
		return true;
	}

	override void OnWeaponActionEnd()
	{
		if ( !m_InProgress )
			return;
		
		if (!m_ControlAction)
		{
			InventoryLocation il = new InventoryLocation;
			il.SetHands(m_player,m_player.GetItemInHands());
			m_player.GetInventory().ClearInventoryReservation(m_player.GetItemInHands(),il);
					
			if( m_PendingTargetMagazine )
			{
				m_player.GetInventory().ClearInventoryReservation(m_PendingTargetMagazine, m_TargetInventoryLocation );
			}
					
			if( m_PendingInventoryLocation )
			{
				m_player.GetInventory().ClearInventoryReservation( m_PendingInventoryLocation.GetItem(), m_PendingInventoryLocation );
			}
		}
		
		if(m_WeaponInHand && !m_justStart && m_player.IsRaised())
		{
			if(m_InIronSight)
				m_player.SetIronsights(m_InIronSight);
			if(m_InOptic)
			{
				ItemOptics optic = m_WeaponInHand.GetAttachedOptics();
				if(optic)
					m_player.SwitchOptics(optic,true);
			}
		}
		m_ControlAction = NULL;
		m_PendingWeaponAction = -1;
		m_PendingTargetMagazine = NULL;
		m_PendingInventoryLocation = NULL;
		m_TargetInventoryLocation = NULL;
		m_PendingWeaponActionAcknowledgmentID = -1;
		//m_WeaponInHand = NULL;
		m_InProgress = false;
		m_readyToStart = false;
		m_WantContinue = true;
		
	}

	override void Update( float deltaT )
	{

		if (m_WeaponInHand != m_player.GetItemInHands())
		{
			if( m_WeaponInHand )
			{
				m_SuitableMagazines.Clear();
				OnWeaponActionEnd();
			}
			m_WeaponInHand = Weapon_Base.Cast(m_player.GetItemInHands());
			if ( m_WeaponInHand )
			{
				m_MagazineInHand = null;
				//SET new magazine
				SetSutableMagazines();
				m_WeaponInHand.SetSyncJammingChance(0);
			}
			m_AnimationRefreshCooldown = 0;
		}
		
		if (m_WeaponInHand)
		{
			if(m_AnimationRefreshCooldown)
			{
				m_AnimationRefreshCooldown--;
			
				if( m_AnimationRefreshCooldown == 0)
				{
					RefreshAnimationState();
				}
			}
		
			m_WeaponInHand.SetSyncJammingChance(m_WeaponInHand.GetChanceToJam());
			
			if(m_readyToStart)
			{
				StartPendingAction();
				m_readyToStart = false;
				return;
			}
		
			if( !m_InProgress || !m_IsEventSended )
				return;
		
			if(m_canEnd)
			{
				if(m_WeaponInHand.IsIdle())
				{
					OnWeaponActionEnd();
				}
				else if(m_justStart)
				{
					m_InIronSight = m_player.IsInIronsights();
					m_InOptic = m_player.IsInOptics();
		
					if(m_InIronSight || m_InOptic)
					{
						m_player.GetInputController().ResetADS();
						m_player.ExitSights();
						//CF_Log.Debug("exitsights");
					}
				
					m_justStart = false;
				}
			
			}
			else
			{
				m_canEnd = true;
				m_justStart = true;
			}
		}
		else
		{
			if ( m_MagazineInHand != m_player.GetItemInHands() )
			{
				m_MagazineInHand = MagazineStorage.Cast(m_player.GetItemInHands());
				if ( m_MagazineInHand )
				{
					SetSutableMagazines();
				}
			}
		
		
		}
	}

	bool CanAttachMagazine_NoHandsCheck(Weapon_Base wpn, Magazine mag, bool reservationCheck = true )
	{	
		if ( !wpn || !mag || mag.IsAmmoPile() )
			return false;
		
		if ( wpn.IsDamageDestroyed())
			return false;
		
		if( m_player.IsItemsToDelete())
			return false;
		
		if ( reservationCheck && (m_player.GetInventory().HasInventoryReservation(wpn, null) || m_player.GetInventory().HasInventoryReservation(mag, null)))
			return false;
		
		InventoryLocation invLoc;
		invLoc = new InventoryLocation;
		
		mag.GetInventory().GetCurrentInventoryLocation(invLoc);
		Weapon_Base wnp2;
		

		//magazine is already in weapon
		if ( Class.CastTo(wnp2,  invLoc.GetParent()) )
			return false;
			
		int muzzleIndex = wpn.GetCurrentMuzzle();
		
		if (wpn.CanAttachMagazine(muzzleIndex, mag))
			return true;

		return false;
		
	}

	bool CanSwapMagazine_NoHandsCheck(Weapon_Base wpn, Magazine mag, bool reservationCheck = true)
	{
		if ( !wpn || !mag || mag.IsAmmoPile() )
			return false;
		
		if ( mag.IsDamageDestroyed() || wpn.IsDamageDestroyed())
			return false;

		if( m_player.IsItemsToDelete())
			return false;
		
		if ( reservationCheck && (m_player.GetInventory().HasInventoryReservation(wpn, null) || m_player.GetInventory().HasInventoryReservation(mag, null)))
			return false;
		

		InventoryLocation invLoc;
		invLoc = new InventoryLocation;
		
		mag.GetInventory().GetCurrentInventoryLocation(invLoc);
		Weapon_Base wnp2;
		
		//second magazine is already in weapon
		if( Class.CastTo(wnp2,  invLoc.GetParent()) )
			return false;		
		
		int muzzleIndex = wpn.GetCurrentMuzzle();	
					
		Magazine mag2;
		if( !Class.CastTo(mag2, wpn.GetMagazine(muzzleIndex)) ) 
			return false;
		
		if (wpn.TestAttachMagazine(muzzleIndex, mag, false, true))
			return true;
		
		return false;
	}

	bool CanLoadBullet_NoHandsCheck(Weapon_Base wpn, Magazine mag, bool reservationCheck = true)
	{
		if ( !wpn || !mag )
			return false;
		
		if( mag.IsDamageDestroyed() || wpn.IsDamageDestroyed())
			return false;
		
		if( wpn.IsJammed(/*wpn.GetCurrentMuzzle()*/) )
			return false;
	
		if( m_player.IsItemsToDelete())
			return false;
		
		if ( reservationCheck && (m_player.GetInventory().HasInventoryReservation(wpn,null) || m_player.GetInventory().HasInventoryReservation(mag,null)))
			return false;
		
		for( int i = 0; i < wpn.GetMuzzleCount(); i++)
		{
			if( wpn.CanChamberBullet( i, mag ) )
				return true;
		}
		
		return false;
	}
};