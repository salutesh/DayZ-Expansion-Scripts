/**
 * ExpansionGPS.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionGPS extends ItemBase
{	
	// ------------------------------------------------------------
	// ExpansionGPS Constructor
	// ------------------------------------------------------------
	void ExpansionGPS()
	{
		
	}
	
	// ------------------------------------------------------------
	// ExpansionGPS PlayerInventoryCheckLocal
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
					player.SetHasItemGPS( true );
					player.SetHasItemMap( true );
				}
			}
		}
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
		
		if ( newParentMan && newParentMan.IsInherited( SurvivorBase ) )
		{
			if ( IsMissionClient() )
			{
				PlayerBase player = PlayerBase.Cast( newParentMan);
				
				if ( !player )
				{
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("ExpansionGPS::EEInventoryIn - player: " + player.ToString());
					#endif

					return;
				}
				if ( HasEnergyManager() && GetCompEM().CanWork() )
				{
					player.SetHasItemGPS( true );
					player.SetHasItemMap( true );
				}
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
		
		if ( oldParentMan && oldParentMan.IsInherited( SurvivorBase ) )
		{
			if ( IsMissionClient() )
			{
				PlayerBase player = PlayerBase.Cast( oldParentMan );
				
				if ( !player )
				{
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("ExpansionGPS::EEInventoryOut - player: " + player.ToString());
					#endif
					
					return;
				}
				
				player.SetHasItemGPS( false );
				player.SetHasItemMap( false );
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::EEInventoryOut - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionGPS EEInit
	// ------------------------------------------------------------
	override void EEInit()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::EEInit - Start");
		#endif
		
		super.EEInit();
		
		PlayerInventoryCheckLocal();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionGPS::EEInit - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionGPS SetActions
	// ------------------------------------------------------------
	/*override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionTurnOnWhileInHands);
		AddAction(ActionTurnOffWhileInHands);
	}*/
}