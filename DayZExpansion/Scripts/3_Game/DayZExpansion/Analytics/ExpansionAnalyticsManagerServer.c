/**
 * ExpansionAnalyticsManagerServer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class AnalyticsManagerServer
{
	const string EXP_STAT_ANIMALS_KILLED = "animals_killed";

	// ------------------------------------------------------------
	// Override OnPlayerConnect
	// ------------------------------------------------------------
	override void OnPlayerConnect( Man player )
	{
		super.OnPlayerConnect( player );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("AnalyticsManagerServer::OnPlayerConnect Start");
		#endif
		
		player.StatRegister( EXP_STAT_ANIMALS_KILLED );
		player.StatSyncToClient();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("AnalyticsManagerServer::OnPlayerConnect End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override OnEntityKilled
	// ------------------------------------------------------------
	override void OnEntityKilled( Object killer, EntityAI target )
	{
		super.OnEntityKilled( killer, target );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("AnalyticsManagerServer::OnEntityKilled Start");
		#endif
		
		EntityAI killer_entity = EntityAI.Cast( killer );
		if ( killer_entity )
		{
			Man killer_survivor = killer_entity.GetHierarchyRootPlayer();
			
			if ( killer_survivor )
			{
				if ( target.IsAnimal() )
				{
					OnAnimalKilled( killer_survivor, target );
				}
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("AnalyticsManagerServer::OnEntityKilled End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion OnAnimalKilled
	// ------------------------------------------------------------
	protected void OnAnimalKilled( Man killer, EntityAI target )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("AnalyticsManagerServer::OnAnimalKilled - Start");
		#endif
		
		killer.StatUpdate( EXP_STAT_ANIMALS_KILLED, 1 );
		killer.StatSyncToClient();	
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("AnalyticsManagerServer::OnAnimalKilled - End");
		#endif
	}
}