/**
 * ExpansionHumanityModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionHumanityModule: JMModuleBase
{
	// ------------------------------------------------------------
	// ExpansionHumanityModule Constructor
	// Gets called on server and client
	// ------------------------------------------------------------
	void ExpansionHumanityModule()
    {
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanityModule::ExpansionHumanityModule - Start");
		#endif
				
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanityModule::ExpansionHumanityModule - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionHumanityModule Destructor
	// Gets called on server and client
	// ------------------------------------------------------------
	void ~ExpansionHumanityModule()
    {
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanityModule::~ExpansionHumanityModule - Start");
		#endif
		
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanityModule::~ExpansionHumanityModule - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionHumanityModule IsEnabled
	// ------------------------------------------------------------
	override bool IsEnabled()
	{
		return (super.IsEnabled() && GetExpansionSettings().GetGeneral().EnableHumanity);
	}
	
	// ------------------------------------------------------------
	// ExpansionHumanityModule Destructor
	// Gets called from client of killed player
	// ------------------------------------------------------------
	void OnPlayerKilledEvent(PlayerBase player, Object killer)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanityModule::OnPlayerKilledEvent - Start");
		#endif
		
		PlayerBase killerPlayer = NULL;

		if (!player)
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionHumanityModule::OnPlayerKilledEvent - ERROR - PlayerBase is NULL");
			#endif

			return;
		}
			
		if (!killer)
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionHumanityModule::OnPlayerKilledEvent - NO KILLER - Killer is NULL");
			#endif

			return;
		}
		
		if ( killer && killer.IsInherited(SurvivorBase) )
		{
			killerPlayer = PlayerBase.Cast( EntityAI.Cast( killer ).GetHierarchyRootPlayer() );
			if ( killerPlayer )
			{
				killerPlayer.DecreaseHumanity( 200 );					
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanityModule::OnPlayerKilledEvent - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionHumanityModule OnBandageTargetActionEvent
	// Gets called from client that executed bandage action on target
	// ------------------------------------------------------------
	void OnBandageTargetActionEvent(PlayerBase player, Object target)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanityModule::OnBandageTargetActionEvent - Start");
		#endif
		
		if (!player)
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionHumanityModule::OnPlayerKilledEvent - ERROR - PlayerBase is NULL");
			#endif

			return;
		}
			
		if (!target)
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionHumanityModule::OnPlayerKilledEvent - NO KILLER - Killer is NULL");
			#endif

			return;
		}
		
		if ( player )
		{
			player.AddHumanity( 20 );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanityModule::OnBandageTargetActionEvent - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionHumanityModule OnCPRActionEvent
	// Gets called from client that executed CPR action on target
	// ------------------------------------------------------------
	void OnCPRActionEvent(PlayerBase player, Object target)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanityModule::OnCPRActionEvent - Start");
		#endif
		
		if (!player)
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionHumanityModule::OnCPRActionEvent - ERROR - PlayerBase is NULL");
			#endif

			return;
		}
			
		if (!target)
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionHumanityModule::OnCPRActionEvent - NO KILLER - Killer is NULL");
			#endif

			return;
		}
		
		if ( player )
		{
			player.AddHumanity( 200 );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanityModule::OnCPRActionEvent - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionHumanityModule OnFeedTetracyclineAntibioticsActionEvent
	// Gets called from client that executed feed antibiotics action on target
	// ------------------------------------------------------------
	void OnFeedTetracyclineAntibioticsActionEvent(PlayerBase player, Object target)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanityModule::OnFeedTetracyclineAntibioticsActionEvent - Start");
		#endif

		if (!player)
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionHumanityModule::OnFeedTetracyclineAntibioticsActionEvent - ERROR - PlayerBase is NULL");
			#endif

			return;
		}
			
		if (!target)
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionHumanityModule::OnFeedTetracyclineAntibioticsActionEvent - NO KILLER - Killer is NULL");
			#endif

			return;
		}
		
		if ( player )
		{
			player.AddHumanity( 20 );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanityModule::OnFeedTetracyclineAntibioticsActionEvent - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionHumanityModule OnFeedPainkillerTabletsActionEvent
	// Gets called from client that executed feed painkillers action on target
	// ------------------------------------------------------------
	void OnFeedPainkillerTabletsActionEvent(PlayerBase player, Object target)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanityModule::OnFeedPainkillerTabletsActionEvent - Start");
		#endif

		if (!player)
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionHumanityModule::OnFeedPainkillerTabletsActionEvent - ERROR - PlayerBase is NULL");
			#endif

			return;
		}
			
		if (!target)
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionHumanityModule::OnFeedPainkillerTabletsActionEvent - NO KILLER - Killer is NULL");
			#endif

			return;
		}
		
		if ( player )
		{
			player.AddHumanity( 20 );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanityModule::OnFeedPainkillerTabletsActionEvent - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionHumanityModule OnGiveBloodToTargetActionEvent
	// Gets called from client that executed give blood action on target
	// ------------------------------------------------------------
	void OnGiveBloodToTargetActionEvent(PlayerBase player, Object target)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanityModule::OnGiveBloodToTargetActionEvent - Start");
		#endif

		if (!player)
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionHumanityModule::OnGiveBloodToTargetActionEvent - ERROR - PlayerBase is NULL");
			#endif

			return;
		}
			
		if (!target)
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionHumanityModule::OnGiveBloodToTargetActionEvent - NO KILLER - Killer is NULL");
			#endif

			return;
		}
		
		if ( player )
		{
			player.AddHumanity( 250 );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanityModule::OnGiveBloodToTargetActionEvent - End");
		#endif
	}
		
	// ------------------------------------------------------------
	// ExpansionHumanityModule OnGiveSalineToTargetActionEvent
	// Gets called from client that executed give saline action on target
	// ------------------------------------------------------------
	void OnGiveSalineToTargetActionEvent(PlayerBase player, Object target)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanityModule::OnGiveSalineToTargetActionEvent - Start");
		#endif

		if (!player)
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionHumanityModule::OnGiveSalineToTargetActionEvent - ERROR - PlayerBase is NULL");
			#endif

			return;
		}
			
		if (!target)
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionHumanityModule::OnGiveSalineToTargetActionEvent - NO KILLER - Killer is NULL");
			#endif

			return;
		}
		
		if ( player )
		{
			player.AddHumanity( 200 );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanityModule::OnGiveSalineToTargetActionEvent - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionHumanityModule OnInjectMorphineToTargetActionEvent
	// Gets called from client that executed injection of morphine action on target
	// ------------------------------------------------------------
	void OnInjectMorphineToTargetActionEvent(PlayerBase player, Object target)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanityModule::OnInjectMorphineToTargetActionEvent - Start");
		#endif

		if (!player)
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionHumanityModule::OnInjectMorphineToTargetActionEvent - ERROR - PlayerBase is NULL");
			#endif

			return;
		}
			
		if (!target)
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionHumanityModule::OnInjectMorphineToTargetActionEvent - NO KILLER - Killer is NULL");
			#endif

			return;
		}
		
		if ( player )
		{
			player.AddHumanity( 50 );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanityModule::OnInjectMorphineToTargetActionEvent - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionHumanityModule OnInfectedKilledEvent
	// ------------------------------------------------------------
	void OnInfectedKilledEvent(Object killer)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanityModule::OnInfectedKilledEvent - Start");
		#endif
				
		if (!killer)
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionHumanityModule::OnInfectedKilledEvent - ERROR - Killer object is NULL");
			#endif
			
			return;
		}
		
		if ( killer.IsInherited( SurvivorBase ) )
		{
			PlayerBase player = PlayerBase.Cast( killer );
			if ( player )
			{
				player.AddHumanity( 10 );
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanityModule::OnInjectMorphineToTargetActionEvent - End");
		#endif
	}
}