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
	const string EXP_STAT_AI_PLAYERS_KILLED = "ai_players_killed";

	// ------------------------------------------------------------
	// Override OnPlayerConnect
	// ------------------------------------------------------------
	override void OnPlayerConnect(Man player)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "OnPlayerConnect");
#endif

		super.OnPlayerConnect(player);
		
		player.StatRegister(EXP_STAT_ANIMALS_KILLED);
	#ifdef ENFUSION_AI_PROJECT
		player.StatRegister(EXP_STAT_AI_PLAYERS_KILLED);
	#endif
	}
	
	// ------------------------------------------------------------
	// Override OnEntityKilled
	// ------------------------------------------------------------
	override void OnEntityKilled(Object killer, EntityAI target)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "OnEntityKilled");
#endif

		super.OnEntityKilled(killer, target);
		
		EntityAI killer_entity = EntityAI.Cast( killer );
		if (killer_entity)
		{
			Man killer_survivor = killer_entity.GetHierarchyRootPlayer();
			
			if (killer_survivor)
			{
				if (target.IsAnimal())
				{
					OnAnimalKilled(killer_survivor, target);
				}
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion OnAnimalKilled
	// ------------------------------------------------------------
	protected void OnAnimalKilled(Man killer, EntityAI target)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "OnAnimalKilled");
#endif

		killer.StatUpdate(EXP_STAT_ANIMALS_KILLED, 1);
	}

	override protected void OnPlayerKilled(Man killer, EntityAI target)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "OnPlayerKilled");
#endif

	#ifdef ENFUSION_AI_PROJECT
		//! Since eAIBase and DZPlayerImpl are not available in 3_Game,
		//! we just assume AI if target is man and has no identity
		Man victim;
		if (Class.CastTo(victim, target) && !victim.GetIdentity())
		{
			OnAIPlayerKilled(killer, target);
			return;
		}
	#endif

		super.OnPlayerKilled(killer, target);
	}

	protected void OnAIPlayerKilled(Man killer, EntityAI target)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "OnAIPlayerKilled");
#endif

	#ifdef ENFUSION_AI_PROJECT
		killer.StatUpdate(EXP_STAT_AI_PLAYERS_KILLED, 1);
	#endif
	}
}
