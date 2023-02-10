/**
 * ExpansionHardlineModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

[CF_RegisterModule(ExpansionHardlineModule)]
class ExpansionHardlineModule: CF_ModuleWorld
{
	override void OnInit()
	{
		super.OnInit();

		EnableInvokeConnect();
	}

	override void OnInvokeConnect(Class sender, CF_EventArgs args)
	{
	#ifdef DIAG
		auto trace = EXTrace.Start(ExpansionTracing.HARDLINE, this);
	#endif

		super.OnInvokeConnect(sender, args);

		auto cArgs = CF_EventPlayerArgs.Cast(args);
		
	#ifdef DIAG
		EXTrace.Add(trace, cArgs.Player);
		EXTrace.Add(trace, cArgs.Identity);
	#endif

		if (GetGame().IsServer() && GetGame().IsMultiplayer() && GetExpansionSettings().GetHardline().UseReputation)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetupClientData, 1000, false, cArgs.Player, cArgs.Identity);
		}
	}

	protected void SetupClientData(PlayerBase player, PlayerIdentity identity)
	{
	#ifdef DIAG
		auto trace = EXTrace.Start(ExpansionTracing.HARDLINE, this);
		EXTrace.Add(trace, player);
		EXTrace.Add(trace, identity);
	#endif

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;

		if (!player || !identity)
			return;

		if (player.Expansion_GetReputation() < 0)
		{
			//! If reputation is negative, this is a legacy player DB storage format where reputation was saved externally.
			//! Check if hardline player data file exists and load it
			string playerUID = identity.GetId();
			auto hardlinePlayerData = new ExpansionHardlinePlayerData();
			if (hardlinePlayerData.Load(playerUID))
				EXPrint("ExpansionHardlineModule::SetupClientData - Loaded legacy player hardline data for player " + identity.GetName() + "[" + playerUID + "]");
			//! If data was succesfully loaded, player rep will be set to value from file, else zero
			player.Expansion_SetReputation(hardlinePlayerData.GetReputation());
		}
	}
	
	void OnEntityKilled(EntityAI victim, Object killer)
	{
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;
		
		EntityAI killerEntity;
		if (!Class.CastTo(killerEntity, killer))
			return;

		PlayerBase killerPlayer;
		ZombieBase killerInfected;
		AnimalBase killerAnimal;
		PlayerBase victimPlayer;
		ZombieBase victimInfected;
		AnimalBase victimAnimal;
		
		bool killerIsPlayer = false;
		bool killerIsAI = false;
		bool killerIsInfected = false;
		bool killerIsAnimal = false;
		bool victimIsPlayer = false;
		bool victimIsAI = false;
		bool victimIsInfected = false;
		bool victimIsAnimal = false;
		
		//! Set killer entity
		if (Class.CastTo(killerPlayer, killerEntity.GetHierarchyRootPlayer()))
		{
		#ifdef EXPANSIONMODAI
			if (killerPlayer.IsAI())
				killerIsAI = true;
			else
		#endif
			killerIsPlayer = true;
		}
		else if (Class.CastTo(killerInfected, killerEntity))
		{
			killerIsInfected = true;
		}
		else if (Class.CastTo(killerAnimal, killerEntity))
		{
			killerIsAnimal = true;
		}
		
		//! Set victim entity
		if (Class.CastTo(victimPlayer, victim.GetHierarchyRootPlayer()))
		{
		#ifdef EXPANSIONMODAI
			if (victimPlayer.IsAI())
				victimIsAI = true;
			else
		#endif
			victimIsPlayer = true;
		}
		else if (Class.CastTo(victimInfected, victim))
		{
			victimIsInfected = true;
		}
		else if (Class.CastTo(victimAnimal, victim))
		{
			victimIsAnimal = true;
		}
		
		if (!victimPlayer && !killerPlayer)
			return;
		
		//! Killer was a normal player
		if (killerIsPlayer && killerPlayer != victimPlayer)
		{
			//! Victim was a normal player.
			if (victimIsPlayer)
			{
				HandlePlayerKilledPlayer(killerPlayer, victimPlayer);
			}
			//! Victim was a infected.
			else if (victimIsInfected)	
			{
				HandlePlayerKilledInfected(killerPlayer, victimInfected);
			}
			//! Victim was an animal.
			else if (victimIsAnimal)	
			{
				HandlePlayerKilledAnimal(killerPlayer, victimAnimal);
			}
		#ifdef EXPANSIONMODAI
			 //! Victim was an AI.
			else if (victimIsAI)
			{
				HandlePlayerKilledAI(killerPlayer, victimPlayer);
			}
		#endif
		}
	#ifdef EXPANSIONMODAI
		//! Killer was an AI and victim was player
		else if (victimIsPlayer && killerIsAI) 
		{
			HandlePlayerKilledByAI(victimPlayer, killerPlayer);
		}
	#endif
		//! Victim was a normal player
		else if (victimIsPlayer)
		{
			HandlePlayerDeath(victimPlayer);
		}
	}
	
	protected void HandlePlayerKilledAnimal(PlayerBase killer, AnimalBase animal)
	{
		int killerReputation = GetExpansionSettings().GetHardline().ReputationOnKillAnimal;
		
		if (!killerReputation)
			return;
		
		bool isFriendly;
	#ifdef EXPANSIONMODAI
		  //! If the animal was friendly to the killer the killer will lose reputation
        isFriendly = killer.GetGroup() && killer.GetGroup().GetFaction().IsFriendly(animal);
	#endif
		if (isFriendly)
			killerReputation = -killerReputation;
		
		killer.Expansion_AddReputation(killerReputation);
	}
	
	protected void HandlePlayerKilledInfected(PlayerBase killer, ZombieBase infected)
	{
		int killerReputation = GetExpansionSettings().GetHardline().ReputationOnKillInfected;
		
		if (!killerReputation)
			return;
		
		bool isFriendly;
	#ifdef EXPANSIONMODAI
		  //! If the animal was friendly to the killer the killer will lose reputation.
        isFriendly = killer.GetGroup() && killer.GetGroup().GetFaction().IsFriendly(infected);
	#endif
		if (isFriendly)
			killerReputation = -killerReputation;
		
		killer.Expansion_AddReputation(killerReputation);
	}
	
#ifdef EXPANSIONMODAI
	protected void HandlePlayerKilledAI(PlayerBase killer, PlayerBase victim)
	{
		int killerReputation = GetExpansionSettings().GetHardline().ReputationOnKillAI;
		
		HandlePlayerKill(killer, victim, killerReputation);
	}
#endif
	
	protected void HandlePlayerKilledPlayer(PlayerBase killer, PlayerBase victim)
	{
		if (!killer.GetIdentity() || !victim.GetIdentity())
			return;
					
		int killerReputation = GetExpansionSettings().GetHardline().ReputationOnKillPlayer;	
		
		HandlePlayerKill(killer, victim, killerReputation);
	}
	
	protected void HandlePlayerKill(PlayerBase killer, PlayerBase victim, int killerReputation)
	{
		if (killerReputation == 0)
			return;

	#ifdef EXPANSIONMODAI
		//! If the other player was friendly to the killer the killer will lose reputation.
		bool isFriendly;
		if (victim.GetGroup())
		{
			if (killer.GetGroup())
				isFriendly = victim.GetGroup().GetFaction().IsFriendly(killer.GetGroup().GetFaction());
			else
				isFriendly = victim.GetGroup().GetFaction().IsFriendly(killer);
		}
		
		if (isFriendly)
			killerReputation = -killerReputation;
	#endif
		
		killer.Expansion_AddReputation(killerReputation);
	}
		
	protected void HandlePlayerDeath(PlayerBase victim)
	{
		int reputationLoss = GetExpansionSettings().GetHardline().ReputationLossOnDeath;
		
		if (!reputationLoss)
			return;

		victim.Expansion_DecreaseReputation(reputationLoss);
	}
	
#ifdef EXPANSIONMODAI
	protected void HandlePlayerKilledByAI(PlayerBase victim, PlayerBase killer)
	{
		if (!victim.GetIdentity())
			return;
		
		HandlePlayerDeath(victim);
	}
#endif

	void OnPlayerAction(PlayerBase player, int reputation)
	{
		if (reputation == 0)
			return;
		
	#ifdef EXPANSIONMODAI
		if (player.IsAI())
			return;
	#endif

		player.Expansion_AddReputation(reputation);
	}
 };