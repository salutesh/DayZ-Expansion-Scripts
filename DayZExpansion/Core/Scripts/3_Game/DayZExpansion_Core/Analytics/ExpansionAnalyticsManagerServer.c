/**
 * ExpansionAnalyticsManagerServer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "OnPlayerConnect");
#endif

		super.OnPlayerConnect( player );
		
		player.StatRegister( EXP_STAT_ANIMALS_KILLED );
		//player.StatSyncToClient();
	}
	
	// ------------------------------------------------------------
	// Override OnEntityKilled
	// ------------------------------------------------------------
	override void OnEntityKilled( Object killer, EntityAI target )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "OnEntityKilled");
#endif

		super.OnEntityKilled( killer, target );
		
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
	}
	
	// ------------------------------------------------------------
	// Expansion OnAnimalKilled
	// ------------------------------------------------------------
	protected void OnAnimalKilled( Man killer, EntityAI target )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "OnAnimalKilled");
#endif

		killer.StatUpdate( EXP_STAT_ANIMALS_KILLED, 1 );
		//killer.StatSyncToClient();
	}
};
