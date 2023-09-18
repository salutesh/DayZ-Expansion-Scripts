/**
 * ExpansionHardlineModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

[CF_RegisterModule(ExpansionHardlineModule)]
class ExpansionHardlineModule: CF_ModuleWorld
{
	protected static ExpansionHardlineModule s_Instance;
	
	protected ref map<string, ref ExpansionHardlinePlayerData> m_HardlinePlayerData = new map<string, ref ExpansionHardlinePlayerData>;
	
	void ExpansionHardlineModule()
	{
		s_Instance = this;
	}
	
	override void OnInit()
	{
		super.OnInit();

/**
 * Order of player-related mission events
 * 
 * On connect (new character):
 * 1) OnClientPrepare
 * 3) OnClientNew (calls CreateCharacter/EquipCharacter in vanilla)
 * 3) OnInvokeConnect
 * 
 * On connect (existing character):
 * 1) OnClientPrepare
 * 2) OnClientReady
 * 3) OnInvokeConnect
 * 
 * On respawn:
 * 1) OnClientRespawn
 * 2) OnClientPrepare
 * 3) OnClientNew (calls CreateCharacter/EquipCharacter in vanilla)
 * 4) OnInvokeConnect
 * 
 * On logout:
 * 1) OnClientLogout
 * 2) OnClientLogoutCancelled (if player cancels logout countdown)
 * 
 * On disconnect (if logout countdown runs out or player early disconnects):
 * 1) OnClientDisconnect
 * 2) OnInvokeDisconnect (only if player character not yet deleted)
 */

		EnableClientPrepare();
		EnableInvokeConnect();
		EnableClientDisconnect();
	}

	override void OnClientPrepare(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(EXTrace.HARDLINE, this);

		auto cArgs = CF_EventPlayerPrepareArgs.Cast(args);

		if (GetExpansionSettings().GetHardline().UseReputation)
			LoadPlayerData(cArgs.Identity);
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

		if (GetExpansionSettings().GetHardline().UseReputation)
			SetupClientData(cArgs.Player, cArgs.Identity);
	}

	override void OnClientDisconnect(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(EXTrace.HARDLINE, this);

		auto cArgs = CF_EventPlayerDisconnectedArgs.Cast(args);

		//! Just to conserve some memory, not needed otherwise
		m_HardlinePlayerData.Remove(cArgs.UID);
	}

	//! Load player data early so it's available in StartingEquipSetup
	protected void LoadPlayerData(PlayerIdentity identity)
	{
		auto trace = EXTrace.Start(EXTrace.HARDLINE, this);

		//! Check if hardline player data file exists and load it
		string playerUID = identity.GetId();
		auto data = m_HardlinePlayerData[playerUID];

		if (!data)
		{
			data = new ExpansionHardlinePlayerData;

			if (data.Load(playerUID))
			{
				EXPrint(this, "::LoadPlayerData - Loaded player hardline data for player " + identity.GetName() + "[" + playerUID + "]");
			#ifdef EXPANSIONMODAI
				if (data.FactionID != -1 && !GetExpansionSettings().GetHardline().EnableFactionPersistence)
				{
					data.FactionID = -1;
					data.Save(playerUID);
				}
			#endif
			}
	
			m_HardlinePlayerData[playerUID] = data;
		}
	}

	//! For use in init.c StartingEquipSetup (e.g. for reputation or faction specific starting gear) which runs before OnInvokeConnect
	ExpansionHardlinePlayerData GetPlayerData(PlayerBase player)
	{
		PlayerIdentity identity = player.GetIdentity();
		if (!identity)
			return null;

		string playerUID = identity.GetId();
		return m_HardlinePlayerData[playerUID];
	}

	protected void SetupClientData(PlayerBase player, PlayerIdentity identity)
	{
	#ifdef DIAG
		auto trace = EXTrace.Start(ExpansionTracing.HARDLINE, this);
		EXTrace.Add(trace, player);
		EXTrace.Add(trace, identity);
	#endif

		string playerUID = identity.GetId();
		player.Expansion_SetHardlineData(m_HardlinePlayerData[playerUID]);
	}
	
#ifdef EXPANSIONMODAI
	//! Server
	//! @note oldFactionID will be -1 if player didn't have a group/faction before (e.g. initial assignment),
	//! newFactionID will be -1 if removing group/faction
	void OnFactionChange(DayZPlayerImplement playerImp, int oldFactionID, int newFactionID)
	{
	#ifdef DIAG
		auto trace = EXTrace.Start(ExpansionTracing.HARDLINE, this);
		EXTrace.Add(trace, playerImp);
		EXTrace.Add(trace, oldFactionID);
		EXTrace.Add(trace, newFactionID);
	#endif
		
		PlayerBase player;
		if (Class.CastTo(player, playerImp))
		{
			bool persistFaction = GetExpansionSettings().GetHardline().EnableFactionPersistence;

			if (persistFaction)
				oldFactionID = player.m_Expansion_HardlineData.FactionID;

			if (newFactionID != oldFactionID)
			{
				if (oldFactionID != -1)
				{
					//! Store current reputation for old faction
					player.m_Expansion_HardlineData.FactionReputation[oldFactionID] = player.Expansion_GetReputation();
	
					if (newFactionID != -1)
					{
						//! Replace the player's current reputation with the one from the hashmap
						//! for the given faction if found, else zero
						player.Expansion_SetReputation(player.m_Expansion_HardlineData.FactionReputation[newFactionID]);
					}
				}
				else
				{
					//! Player didn't have a faction before, carry over current reputation
					player.m_Expansion_HardlineData.FactionReputation[newFactionID] = player.Expansion_GetReputation();
				}

				player.m_Expansion_HardlineData.FactionID = newFactionID;
				player.Expansion_SaveHardlineData();
				MissionBaseWorld.Cast(GetGame().GetMission()).Expansion_OnPlayerFactionChange(player, oldFactionID, newFactionID);
			}
		}
	}
#endif
	
	void OnEntityKilled(EntityAI victim, Object killer)
	{
	#ifdef DIAG
		auto trace = EXTrace.Start(ExpansionTracing.HARDLINE, this);
		EXTrace.Add(trace, victim.GetType());
		EXTrace.Add(trace, killer.GetType());
	#endif

		int reputation;
		string matchingClass;
		foreach (string cls, int rep: GetExpansionSettings().GetHardline().EntityReputation)
		{
		    if (ExpansionStatic.Is(victim, cls))
		    {
				matchingClass = cls;
		        reputation = rep;
		        break;
		    }
		}
		
	#ifdef EXPANSIONMODAI
		if ((matchingClass == "PlayerBase" || matchingClass == "SurvivorBase" || matchingClass == "ManBase") && victim.GetType().IndexOf("eAI") > -1)
		{
			if (!GetExpansionSettings().GetHardline().EntityReputation.Find("eAIBase", reputation))
				Error("[Hardline] No invalid reputation configuration in Hardline settings for class eAIBase!");
		}
	#endif
	
	#ifdef DIAG
		if (matchingClass != string.Empty)
			ModulePrint(ToString() + "::OnEntityKilled - Match <Class name: " + matchingClass + " | Reputation:" + reputation + " | Victim entity type:" + victim.GetType() + ">");
	#endif

		if (reputation == 0)
		{
			GetExpansionSettings().GetLog().PrintLog("[Hardline] Entity %1 has no invalid reputation configuration in Hardline settings!", victim.GetType());
			return;
		}
		
		EntityAI killerEntity;
		if (!Class.CastTo(killerEntity, killer))
			return;

		PlayerBase killerPlayer;
		PlayerBase victimPlayer;
		
		bool killerIsPlayer = false;
		bool killerIsAI = false;
		bool victimIsPlayer = false;
		bool victimIsAI = false;
		
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

		//! Killer was a normal player
		if (killerIsPlayer && victimPlayer != killerPlayer)
		{
			//! Victim was a normal player.
			if (victimIsPlayer || victimIsAI)
			{
				HandlePlayerKilledPlayer(killerPlayer, victimPlayer, reputation, victimIsAI);
			}
			else
			{
				HandlePlayerKilledEntity(killerPlayer, victim, reputation);
			}
		}
	#ifdef EXPANSIONMODAI
		//! Killer was an AI and victim was player
		else if (victimIsPlayer && killerIsAI) 
		{
			HandlePlayerKilledByAI(victimPlayer, killerPlayer, reputation);
		}
	#endif
		//! Victim was a normal player
		else if (victimIsPlayer)
		{
			HandlePlayerDeath(victimPlayer);
		}
	}
	
	protected void HandlePlayerKilledEntity(PlayerBase killer, EntityAI victim, int reputation)
	{		
		auto trace = EXTrace.Start(ExpansionTracing.HARDLINE, this);

		killer.Expansion_AddReputation(reputation);
		MissionBaseWorld.Cast(GetGame().GetMission()).Expansion_OnPlayerKilledEntity(killer, victim, reputation);
	}
	
	protected void HandlePlayerKilledPlayer(PlayerBase killer, PlayerBase victim, int reputation, bool victimIsAI = false)
	{
		auto trace = EXTrace.Start(ExpansionTracing.HARDLINE, this);
		
		bool isFriendly = false;
		
		//! Calculate and add the percantage amount based on the victims reputation and settings param "ReputationPercent" to the given reputation value.
		if (killer.GetIdentity())
		{
			int victimRep = victim.Expansion_GetReputation();
			int bonusRep = ((victimRep / 100) * GetExpansionSettings().GetHardline().ReputationBonusPercent);
			if (reputation >= 0)
			{
				reputation += bonusRep;
			}
			else
			{
				reputation -= bonusRep;	
			}
			
		#ifdef EXPANSIONMODGROUPS
			//! Check if killer and victim are in the same party/group.
			if (victim.GetIdentity())
			{
				if (killer.Expansion_GetPartyID() == victim.Expansion_GetPartyID())
					isFriendly = true;
			}
		#endif
		}

	#ifdef EXPANSIONMODAI
		//! Check if killer player or AI enties are in a faction.
		if (victim.GetGroup() && !isFriendly)
		{
			if (victim.GetGroup() == killer.GetGroup())
			{
				isFriendly = true;
			}
			else if (killer.GetGroup())
			{
				isFriendly = victim.GetGroup().GetFaction().IsFriendly(killer.GetGroup().GetFaction());
			}
			else
			{
				isFriendly = victim.GetGroup().GetFaction().IsFriendly(killer);
			}
		}
	#endif
		
	#ifdef DIAG
		ModulePrint(ToString() + "::HandlePlayerKilledPlayer - Killer and victim are friendly to each other: " + isFriendly);
	#endif
		
		//! If player killed a friendly player because they share the same faction or same party then we remove instead of add the reputation.
		if (isFriendly)
			reputation = -reputation;
		
		killer.Expansion_AddReputation(reputation);
		MissionBaseWorld.Cast(GetGame().GetMission()).Expansion_OnPlayerKilledPlayer(killer, victim, reputation, victimIsAI);
	}
		
	protected void HandlePlayerDeath(PlayerBase victim)
	{
		auto trace = EXTrace.Start(ExpansionTracing.HARDLINE, this);
		
		int victimRep = victim.Expansion_GetReputation();
		int repToRemove = ((victimRep / 100) * GetExpansionSettings().GetHardline().ReputationLossPercent);
		
		victim.Expansion_DecreaseReputation(repToRemove);
		MissionBaseWorld.Cast(GetGame().GetMission()).Expansion_OnPlayerDeath(victim, repToRemove);
	}
	
#ifdef EXPANSIONMODAI
	protected void HandlePlayerKilledByAI(PlayerBase victim, PlayerBase killer, int reputation)
	{
		auto trace = EXTrace.Start(ExpansionTracing.HARDLINE, this);

		HandlePlayerDeath(victim);
		MissionBaseWorld.Cast(GetGame().GetMission()).Expansion_OnPlayerKilledAI(killer, victim, reputation);
	}
#endif

	void OnPlayerAction(PlayerBase player, int reputation)
	{
		auto trace = EXTrace.Start(ExpansionTracing.HARDLINE, this);
		
		if (reputation == 0)
			return;
		
	#ifdef EXPANSIONMODAI
		if (player.IsAI())
			return;
	#endif

		player.Expansion_AddReputation(reputation);
	}
	
	static ExpansionHardlineModule GetModuleInstance()
	{
		return s_Instance;
	}
	
	static void ModulePrint(string text)
	{
	#ifdef DIAG
		EXTrace.Print(EXTrace.HARDLINE, s_Instance, text);
	#endif
	}
	
 };