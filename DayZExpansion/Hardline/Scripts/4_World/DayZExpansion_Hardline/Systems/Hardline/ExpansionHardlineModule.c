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

enum ExpansionHardlineModuleRPC
{
	INVALID = 70000,
	SendPlayerHardlineData,
	RequestHardlineItemData,
	SendHardlineItemData
	COUNT
};

[CF_RegisterModule(ExpansionHardlineModule)]
class ExpansionHardlineModule: CF_ModuleWorld
{
	//! Server only
	private autoptr map<string, ref ExpansionHardlinePlayerData> m_PlayerDatas; //! Server
	
	//! Client only
	private ref ExpansionHardlinePlayerData m_HardlinePlayerData; //! Client

	override void OnInit()
	{
		super.OnInit();

		EnableMissionStart();
		EnableMissionLoaded();
		EnableMissionFinish();
		EnableInvokeConnect();
		EnableInvokeDisconnect();
		EnableClientLogout();
		EnableClientReady();
		EnableClientDisconnect();
		EnableUpdate();
		EnableRPC();
	}

	override void OnMissionStart(Class sender, CF_EventArgs args)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.HARDLINE, this, "OnMissionStart");
	#endif

		super.OnMissionStart(sender, args);
	}

	override void OnMissionLoaded(Class sender, CF_EventArgs args)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.HARDLINE, this, "OnMissionLoaded");
	#endif

		super.OnMissionLoaded(sender, args);
		
		m_PlayerDatas = new map<string, ref ExpansionHardlinePlayerData>;

		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			if (!FileExist(EXPANSION_HARDLINE_FOLDER))
			{
				ExpansionStatic.MakeDirectoryRecursive(EXPANSION_HARDLINE_FOLDER);
				ExpansionStatic.MakeDirectoryRecursive(EXPANSION_HARDLINE_PLAYERDATA_FOLDER);
			}
		}
	}

	override void OnInvokeConnect(Class sender, CF_EventArgs args)
	{
	#ifdef DIAG
		auto trace = EXTrace.Start(ExpansionTracing.HARDLINE, this);
	#endif

		super.OnInvokeConnect(sender, args);

		auto cArgs = CF_EventPlayerArgs.Cast(args);
		
	#ifdef DIAG
		EXTrace.Add(trace, cArgs.Identity);
	#endif

		if (GetGame().IsServer() && GetGame().IsMultiplayer() && GetExpansionSettings().GetHardline().UseReputation)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetupClientData, 1000, false, cArgs.Identity);
		}
	}

	override void OnClientDisconnect(Class sender, CF_EventArgs args)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.HARDLINE, this, "OnClientDisconnect");
	#endif

		super.OnClientDisconnect(sender, args);

		auto cArgs = CF_EventPlayerDisconnectedArgs.Cast(args);
		
		//! Maybe move this to the OnClientLogout method
		if (GetGame().IsServer() && GetGame().IsMultiplayer() && GetExpansionSettings().GetHardline().UseReputation)
		{
			SavePlayerHardlineData(cArgs.UID);
		}
	}

	protected void SavePlayerHardlineData(string playerUID)
	{
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;

		ExpansionHardlinePlayerData playerData = GetPlayerHardlineDataByUID(playerUID);
		if (playerData)
			playerData.Save(playerUID);
	}
	
	//! This method is only called once on player connection
	// and to send the needed data from the hardline module to the client
	protected void SetupClientData(PlayerIdentity identity)
	{
	#ifdef DIAG
		auto trace = EXTrace.Start(ExpansionTracing.HARDLINE, this);
		EXTrace.Add(trace, identity);
	#endif

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;

		if (!identity)
			return;

		string playerUID = identity.GetId();
		ExpansionHardlinePlayerData hardlinePlayerData = GetPlayerHardlineDataByUID(playerUID);
		if (!hardlinePlayerData)
		{
			//! If we don't have cached player hardline data, check if file exists and load it, else use fresh instance as-is
			hardlinePlayerData = new ExpansionHardlinePlayerData();
			m_PlayerDatas.Insert(playerUID, hardlinePlayerData);
			if (hardlinePlayerData.Load(playerUID))
				HardlineModulePrint("ExpansionHardlineModule::SetupClientData - Loaded player hardline data for player " + identity.GetName() + "[" + playerUID + "]");
			else
				HardlineModulePrint("ExpansionHardlineModule::SetupClientData - Created new player hardline data for player " + identity.GetName() + "[" + playerUID + "]");
		}
		else if (hardlinePlayerData)
		{
			HardlineModulePrint("ExpansionHardlineModule::SetupClientData - Got cached player hardline data for player " + identity.GetName() + "[" + playerUID + "]");
		}

		if (!hardlinePlayerData)
			return;

		SendPlayerHardlineData(hardlinePlayerData, identity);
	}
	
	ExpansionHardlinePlayerData GetPlayerHardlineDataByUID(string playerUID)
	{
		return m_PlayerDatas.Get(playerUID);
	}
	
	void OnEntityKilled(EntityAI victim, Object killer)
	{
		HardlineModulePrint(ToString() + "::OnEntityKilled - Start");
		
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;
		
		EntityAI killerEntity;
		if (!Class.CastTo(killerEntity, killer))
			return;
			
		HardlineModulePrint(ToString() + "::OnEntityKilled - Victim type: " + victim.GetType());
		HardlineModulePrint(ToString() + "::OnEntityKilled - Killer type: " + killerEntity.GetType());

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
		
		ExpansionHardlinePlayerData killerPlayerData;
		ExpansionHardlinePlayerData victimPlayerData;
		
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
		
		HardlineModulePrint(ToString() + "::OnEntityKilled - Killer is Player: " + killerIsPlayer);
		HardlineModulePrint(ToString() + "::OnEntityKilled - Killer is AI: " + killerIsAI);
		HardlineModulePrint(ToString() + "::OnEntityKilled - Killer is Infected: " + killerIsInfected);
		HardlineModulePrint(ToString() + "::OnEntityKilled - Killer is Animal: " + killerIsAnimal);
		HardlineModulePrint(ToString() + "::OnEntityKilled - Victim is Player: " + victimIsPlayer);
		HardlineModulePrint(ToString() + "::OnEntityKilled - Victim is AI: " + victimIsAI);
		HardlineModulePrint(ToString() + "::OnEntityKilled - Victim is Infected: " + victimIsInfected);
		HardlineModulePrint(ToString() + "::OnEntityKilled - Victim is Animal: " + victimIsAnimal);
		
		StringLocaliser title;
		StringLocaliser text;
			
		//! Killer was a normal player
		if (killerIsPlayer)
		{
			//! Victim was a normal player.
			if (victimIsPlayer && !victimIsAI)
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
		
		HardlineModulePrint(ToString() + "::OnEntityKilled - End");
	}
	
	protected void HandlePlayerKilledAnimal(PlayerBase killer, AnimalBase animal)
	{
		HardlineModulePrint(ToString() + "::HandlePlayerKilledAnimal - Start");
		
		string killerUID = killer.GetIdentity().GetId();
        ExpansionHardlinePlayerData killerPlayerData = GetPlayerHardlineDataByUID(killerUID);
        
        if (!killerPlayerData)
            return;
		
		int killerReputation = GetExpansionSettings().GetHardline().ReputationOnKillAnimal;
		
		bool isFriendly;
	#ifdef EXPANSIONMODAI
		  //! If the animal was friendly to the killer the killer will lose humanity.
        isFriendly = killer.GetGroup() && killer.GetGroup().GetFaction().IsFriendly(animal);
	#endif
		if (isFriendly)
        {
        	killerReputation = -killerReputation;
		}     
		
		HardlineModulePrint(ToString() + "::HandlePlayerKilledAnimal - Killer reputation change: " + killerReputation);
		
		killerPlayerData.AddReputation(killerReputation);
        killerPlayerData.Save(killerUID);
        SendPlayerHardlineData(killerPlayerData, killer.GetIdentity());
		
		HardlineModulePrint(ToString() + "::HandlePlayerKilledAnimal - End");
	}
	
	protected void HandlePlayerKilledInfected(PlayerBase killer, ZombieBase infected)
	{
		HardlineModulePrint(ToString() + "::HandlePlayerKilledInfected - Start");
		
		string killerUID = killer.GetIdentity().GetId();
        ExpansionHardlinePlayerData killerPlayerData = GetPlayerHardlineDataByUID(killerUID);
        
        if (!killerPlayerData)
            return;
		
		int killerReputation = GetExpansionSettings().GetHardline().ReputationOnKillInfected;
		
		bool isFriendly;
	#ifdef EXPANSIONMODAI
		  //! If the animal was friendly to the killer the killer will lose reputation.
        isFriendly = killer.GetGroup() && killer.GetGroup().GetFaction().IsFriendly(infected);
	#endif
		if (isFriendly)
        {
			killerReputation = -killerReputation;
        }
		
		HardlineModulePrint(ToString() + "::HandlePlayerKilledInfected - Killer reputation change: " + killerReputation);
		
		killerPlayerData.AddReputation(killerReputation);
        killerPlayerData.Save(killerUID);
        SendPlayerHardlineData(killerPlayerData, killer.GetIdentity());
		
		HardlineModulePrint(ToString() + "::HandlePlayerKilledInfected - End");
	}
	
#ifdef EXPANSIONMODAI
	protected void HandlePlayerKilledAI(PlayerBase killer, PlayerBase victim)
    {
		HardlineModulePrint(ToString() + "::HandlePlayerKilledAI - Start");
		
        string killerUID = killer.GetIdentity().GetId();
        ExpansionHardlinePlayerData killerPlayerData = GetPlayerHardlineDataByUID(killerUID);
        
        if (!killerPlayerData)
            return;
        
        eAIBase victimAI = eAIBase.Cast(victim);
        if (!victimAI)
            return;
        
        int killerReputation = GetExpansionSettings().GetHardline().ReputationOnKillAI;
        
        //! If the ai was friendly to the killer the killer will lose humanity.
        bool isFriendly;
        if (killer.GetGroup())
            isFriendly = victimAI.GetGroup().GetFaction().IsFriendly(killer.GetGroup().GetFaction());
        else
            isFriendly = victimAI.GetGroup().GetFaction().IsFriendly(killer);
        
		if (isFriendly)
        {
			killerReputation = -killerReputation;  //! Hero/Bambi looses humanity when negative
        }    
        
		HardlineModulePrint(ToString() + "::HandlePlayerKilledAI - Killer humanity change: " + killerReputation);
		
        killerPlayerData.AddReputation(killerReputation);
        killerPlayerData.Save(killerUID);
        SendPlayerHardlineData(killerPlayerData, killer.GetIdentity());
		
		HardlineModulePrint(ToString() + "::HandlePlayerKilledAI - End");
    }
#endif
	
	protected void HandlePlayerKilledPlayer(PlayerBase killer, PlayerBase victim)
	{
		HardlineModulePrint(ToString() + "::HandlePlayerKilledPlayer - Start");
		
		if (!killer.GetIdentity() || !victim.GetIdentity())
			return;
					
		string killerUID = killer.GetIdentity().GetId();
        ExpansionHardlinePlayerData killerPlayerData = GetPlayerHardlineDataByUID(killerUID);
		int killerReputation = GetExpansionSettings().GetHardline().ReputationOnKillPlayer;	
		
		HandlePlayerDeath(victim);
		
		HardlineModulePrint(ToString() + "::HandlePlayerKilledPlayer - End");
	}
		
	protected void HandlePlayerDeath(PlayerBase victim)
	{
		HardlineModulePrint(ToString() + "::HandlePlayerDeath - Start");
		
		string victimUID = victim.GetIdentity().GetId();
		ExpansionHardlinePlayerData victimPlayerData = GetPlayerHardlineDataByUID(victimUID);
		
		if (!victimPlayerData)
			return;

		int victimReputation = GetExpansionSettings().GetHardline().ReputationLossOnDeath;
		victimReputation = -victimReputation;
		
		int currentReputation = victimPlayerData.GetReputation();			
		int reputationDifference = currentReputation - GetExpansionSettings().GetHardline().ReputationLossOnDeath;
		if (reputationDifference <= 0)
		{
			victimReputation = 0;
		}

		HardlineModulePrint(ToString() + "::HandlePlayerKilledPlayer - Victim repuation change: " + victimReputation);
		
		victimPlayerData.AddReputation(victimReputation);
		victimPlayerData.Save(victimUID);
		SendPlayerHardlineData(victimPlayerData, victim.GetIdentity());
		
		HardlineModulePrint(ToString() + "::HandlePlayerDeath - End");
	}
	
#ifdef EXPANSIONMODAI
	protected void HandlePlayerKilledByAI(PlayerBase victim, PlayerBase killer)
	{
		HardlineModulePrint(ToString() + "::HandlePlayerKilledByAI - Start");
		
		if (!victim.GetIdentity())
			return;
		
		HandlePlayerDeath(victim);
		
		HardlineModulePrint(ToString() + "::HandlePlayerKilledByAI - End");
	}
#endif

	void OnPlayerAction(PlayerBase player, int reputation)
	{
	#ifdef EXPANSIONMODAI
		if (player.IsAI())
			return;
	#endif
		
		string playerID = player.GetIdentity().GetId();		
		ExpansionHardlinePlayerData hardlinePlayerData = GetPlayerHardlineDataByUID(playerID);
		if (!hardlinePlayerData)
			return;

		if (reputation == 0)
			return;
		
		hardlinePlayerData.AddReputation(reputation);	
		hardlinePlayerData.Save(player.GetIdentity().GetId());		
		SendPlayerHardlineData(hardlinePlayerData, player.GetIdentity());
	}

	void HardlineModulePrint(string text)
	{
	#ifdef DIAG
		EXTrace.Print(EXTrace.HARDLINE, this, text);
	#endif
	}

	override int GetRPCMin()
	{
		return ExpansionHardlineModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionHardlineModuleRPC.COUNT;
	}

	override void OnRPC(Class sender, CF_EventArgs args)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.HARDLINE, this, "OnRPC");
	#endif

		super.OnRPC(sender, args);

		auto rpc = CF_EventRPCArgs.Cast(args);

		switch ( rpc.ID )
		{
			case ExpansionHardlineModuleRPC.SendPlayerHardlineData:
			{
				RPC_SendPlayerHardlineData(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
		}
	}

	void SendPlayerHardlineData(PlayerIdentity identity)
	{
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;

		//! Get existing player hardline data if there is a exiting one in m_PlayerDatas
		ExpansionHardlinePlayerData hardlinePlayerData = GetPlayerHardlineDataByUID(identity.GetId());

		SendPlayerHardlineData(hardlinePlayerData, identity);
	}

	void SendPlayerHardlineData(ExpansionHardlinePlayerData hardlinePlayerData, PlayerIdentity identity)
	{
		if (!hardlinePlayerData)
			return;
		
		PlayerBase player = PlayerBase.GetPlayerByUID(identity.GetId());
		if (player)
		{
			player.SetReputation(hardlinePlayerData.GetReputation());
		}
		
		auto rpc = ExpansionScriptRPC.Create();
		hardlinePlayerData.OnSend(rpc);
		rpc.Send(NULL, ExpansionHardlineModuleRPC.SendPlayerHardlineData, true, identity);
	}

	protected void RPC_SendPlayerHardlineData(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
			return;
		
		if (GetGame().IsDedicatedServer())
			return;

		if (!m_HardlinePlayerData)
			m_HardlinePlayerData = new ExpansionHardlinePlayerData();
		
		if (!m_HardlinePlayerData.OnRecieve(ctx))
		{
			Error("ExpansionHardlineModule::RPC_SendPlayerHardlineData - Error on recieving hardline player data!");
			return;
		}

		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player)
		{
			player.SetReputation(m_HardlinePlayerData.GetReputation());
		}
	}

	ExpansionHardlinePlayerData GetHardlineClientData()
	{
		return m_HardlinePlayerData;
	}
 };