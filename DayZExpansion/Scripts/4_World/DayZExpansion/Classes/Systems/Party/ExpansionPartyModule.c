/**
 * ExpansionPartyModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPartyModule: JMModuleBase
{
	//! Server side
	private ref map<int, ref ExpansionPartyData> m_Parties;
	
	//! Client side
	private ref ExpansionPartyData m_Party;
	private ref array<ref ExpansionPartyInviteData> m_PartyInvites;
	private bool m_IsClientInitialized;
	
	int m_NextPartyID = 0;
	
	private const float UPDATE_TICK_TIME = 1.0; // refreshes up to 100 players every ten seconds
	private const int UPDATE_PLAYERS_PER_TICK = 10;
	private float m_UpdateQueueTimer;
	private int m_CurrentPlayerTick;
	private int m_CurrentPartyTick;
	
	static ref ScriptInvoker m_PartyHUDInvoker = new ScriptInvoker();
	
	void ExpansionPartyModule()
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXPrint("ExpansionPartyModule::Constructor - Start");
		#endif
		
		//Server
		m_Parties = new map<int, ref ExpansionPartyData>();
		
		//Client
		m_Party = NULL;
		m_PartyInvites = new array<ref ExpansionPartyInviteData>();
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXPrint("ExpansionPartyModule::Constructor - End");
		#endif
	}
	
	void ~ExpansionPartyModule()
	{
		//Server
		delete m_Parties;
		
		//Client
		m_Party = NULL;
		m_PartyInvites.Clear();
	}
	
	override void OnInit()
	{
		if (IsMissionHost())
		{
			m_UpdateQueueTimer = 0;
		}
	}
	
	override bool IsEnabled()
	{
		return (super.IsEnabled() && GetExpansionSettings().GetParty().EnableParties);
	}

	override void RegisterKeyMouseBindings() 
	{
		super.RegisterKeyMouseBindings();
		
		RegisterBinding(new JMModuleBinding("Input_UpdateQuickMarker", "UAExpansionQuickMarker", true));
		RegisterBinding( new JMModuleBinding("Input_RemoveQuickMarker",	 "UAExpansionMapDeleteMarker",	 true));
	}
	
	override void OnMissionStart() 
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXPrint("ExpansionPartyModule::OnMissionStart - Start");
		#endif

		super.OnMissionStart();

		if (!IsMissionHost())
		{
			return;
		}

		if (!FileExist(EXPANSION_GROUPS_FOLDER))
		{
			MakeDirectory(EXPANSION_GROUPS_FOLDER);
		}
				
		array<string> group_files = ExpansionFindFilesInLocation(EXPANSION_GROUPS_FOLDER, ".bin");
		foreach (string fileName : group_files)
		{
			ExpansionPartyData party;
			if (FileExist(EXPANSION_GROUPS_FOLDER + fileName))
			{
				party = new ExpansionPartyData;
				FileSerializer file = new FileSerializer;
				
				if (file.Open(EXPANSION_GROUPS_FOLDER + fileName, FileMode.READ))
				{
					int version;
					if (Expansion_Assert_False(file.Read(version), "[" + this + ":" + fileName + "] Failed reading version"))
					{
						file.Close();
						continue;
					}
					
					if (party.OnStoreLoad(file, version))
					{
						party.InitMaps();
						m_Parties.Insert(party.GetPartyID(), party);

						if (m_NextPartyID <= party.GetPartyID())
						{
							m_NextPartyID = party.GetPartyID() + 1;
						}
					} else
					{
						Print("Failed loading group " + fileName);
					}
					
					file.Close();
				}
			}
		}

		Print("ExpansionPartyModule::OnMissionStart - Start");
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXPrint("ExpansionPartyModule::OnMissionStart - m_Parties.Count(): " + m_Parties.Count());
		#endif
	}
	
	// ------------------------------------------------------------
	// Override GetRPCMin
	// ------------------------------------------------------------
	override int GetRPCMin()
	{
		return ExpansionPartyModuleRPC.INVALID;
	}
	
	// ------------------------------------------------------------
	// Override GetRPCMax
	// ------------------------------------------------------------
	override int GetRPCMax()
	{
		return ExpansionPartyModuleRPC.COUNT;
	}
	
	// ------------------------------------------------------------
	// Override OnRPC
	// ------------------------------------------------------------
	#ifdef CF_BUGFIX_REF
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	#else
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx)
	#endif
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::OnRPC - Start");
		#endif
		
		switch (rpc_type)
		{
		case ExpansionPartyModuleRPC.Create:
			RPC_Create(ctx, sender);
			break;
		case ExpansionPartyModuleRPC.Dissolve:
			RPC_Dissolve(ctx, sender);
			break;
		case ExpansionPartyModuleRPC.Leave:
			RPC_Leave(ctx, sender);
			break;
		case ExpansionPartyModuleRPC.AcceptInvite:
			RPC_AcceptInvite(ctx, sender);
			break;
		case ExpansionPartyModuleRPC.RemovePlayer:
			RPC_RemovePlayer(ctx, sender);
			break;
		case ExpansionPartyModuleRPC.UpdateClient:
			RPC_UpdateClient(ctx, sender);
			break;
		case ExpansionPartyModuleRPC.CreateMarker:
			RPC_CreateMarker(ctx, sender);
			break;
		case ExpansionPartyModuleRPC.UpdateMarker:
			RPC_UpdateMarker(ctx, sender);
			break;
		case ExpansionPartyModuleRPC.UpdatePositionMarker:
			RPC_UpdatePositionMarker(ctx, sender);
			break;
		case ExpansionPartyModuleRPC.DeleteMarker:
			RPC_DeleteMarker(ctx, sender);
			break;
		case ExpansionPartyModuleRPC.InvitePlayer:
			RPC_InvitePlayer(ctx, sender);
			break;
		case ExpansionPartyModuleRPC.DeclineInvite:
			RPC_DeclineInvite(ctx, sender);
			break;
		case ExpansionPartyModuleRPC.SyncPlayersInvites:
			RPC_SyncPlayersInvites(ctx, sender);
			break;
		case ExpansionPartyModuleRPC.UpdateQuickMarker:
			RPC_UpdateQuickMarker(ctx, sender);
			break;
		case ExpansionPartyModuleRPC.UpdatePermissions:	
			RPC_UpdatePermissions(ctx, sender);
			break;
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::OnRPC - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionPartyModule FindAndSyncPlayerInvites
	// Called on server
	// ------------------------------------------------------------
	private void FindAndSyncPlayerInvites(PlayerBase player, notnull PlayerIdentity sender)
	{	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::FindAndSyncPlayerInvites - Start");
		#endif
		
		if (!player)
		{
			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint("ExpansionPartyModule::FindAndSyncPlayerInvites - [ERROR] Player object is empty or NULL!");
			#endif
			return;
		}
		
		SyncPlayersInvites(player);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::FindAndSyncPlayerInvites - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// RequestSyncPlayersInvites
	// Called on server
	// ------------------------------------------------------------
	void SyncPlayersInvites(notnull PlayerBase sender)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::SyncPlayersInvites - Start");
		#endif
		
		if (!IsMissionHost() || !sender.GetIdentity())
			return;
		
		array<ref ExpansionPartyInviteData> invites = new array<ref ExpansionPartyInviteData>();
		
		foreach (int i, ExpansionPartyData data : m_Parties)
		{
			ExpansionPartyInviteData invite = data.GetPlayerInvite(sender.GetIdentityUID());
			if (invite)
				invites.Insert(invite);
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::SyncPlayersInvites - Found " + invites.Count() + " invites");
		#endif
		
		ScriptRPC rpcServer = new ScriptRPC();
		rpcServer.Write(invites);
		rpcServer.Send(NULL, ExpansionPartyModuleRPC.SyncPlayersInvites, true, sender.GetIdentity());
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::SyncPlayersInvites - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionPartyModule RPC_SyncPlayersInvites
	// Called on client
	// ------------------------------------------------------------
	private void RPC_SyncPlayersInvites(ParamsReadContext ctx, PlayerIdentity senderRPC)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_SyncPlayersInvites - Start");
		#endif
		
		if (!IsMissionClient())
			return;
		
		array<ref ExpansionPartyInviteData> invites;
		if (!ctx.Read(invites))
			return;
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_SyncPlayersInvites invites : " + invites + " invites.Count() : " + invites.Count());
		#endif

		SetPartyInvites(invites);
		
		ExpansionScriptViewMenuBase menu;
		if (Class.CastTo(menu, GetDayZGame().GetExpansionGame().GetExpansionUIManager().GetMenu()))
		{
			menu.Refresh();
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_SyncPlayersInvites - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Create
	// Called on client
	// ------------------------------------------------------------
	void Create(string partyName)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Create - Start");
		#endif
		
		if (!IsMissionClient())
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(partyName);
		rpc.Send(NULL, ExpansionPartyModuleRPC.Create, true, NULL);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Create - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion RPC_Create
	// Called on server
	// ------------------------------------------------------------
	private void RPC_Create(ParamsReadContext ctx, PlayerIdentity senderRPC)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_Create - Start");
		#endif
		
		string partyName;
		if (!ctx.Read(partyName))
			return;

		Exec_Create(partyName, senderRPC);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_Create - Start");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_Create
	// Called on server
	// ------------------------------------------------------------
	private void Exec_Create(string partyName, PlayerIdentity sender)
	{	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_Create - Start");
		#endif
		
		PlayerBase player = PlayerBase.GetPlayerByUID(sender.GetId());
		if (!player)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_PLAYERBASE").Error(sender);
			return;
		}

		if (HasParty(player))
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_IN_PARTY").Error(sender);
			return;
		}
	
		ExpansionPartyData newParty = new ExpansionPartyData(m_NextPartyID);
		newParty.SetupExpansionPartyData(player, partyName);
		newParty.Save();
	
		m_Parties.Insert(m_NextPartyID, newParty);

		UpdateClient(m_NextPartyID++);

		if (GetExpansionSettings().GetLog().Party)
			GetExpansionSettings().GetLog().PrintLog("[Party] Player \"" + sender.GetName() + "\" (id=" + sender.GetId() + ")" + " created a party named " + partyName);
		
		ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_SYSTEM_PARTY_CREATED").Success(sender);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_Create - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion Leave
	// Called on client
	// ------------------------------------------------------------
	void Leave()
	{	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Leave - Start");
		#endif
		
		if (!IsMissionClient())
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Send(NULL, ExpansionPartyModuleRPC.Leave, true, NULL);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Leave - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_Leave
	// ------------------------------------------------------------
	private void RPC_Leave(ParamsReadContext ctx, PlayerIdentity senderRPC)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_Leave - Start");
		#endif
		
		Exec_Leave(senderRPC);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_Leave - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_Leave
	// ------------------------------------------------------------
	private void Exec_Leave(PlayerIdentity sender)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_Leave - Start");
		#endif
		
		if (!sender)
			return;
		
		PlayerBase player = PlayerBase.GetPlayerByUID(sender.GetId());
		if (!player)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_PLAYERBASE").Error(sender);	
			return;
		}

		int partyId = GetPartyID(player);

		ExpansionPartyData party = m_Parties.Get(partyId);
		if (!party || partyId == -1)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_NOT_EXIST").Error(sender);	
			return;
		}
		
		if (party.GetOwnerUID() == sender.GetId())
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_UNKNOWN").Error(sender);	
			return;
		}

		if (!party.RemovePlayer(sender.GetId()))
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_SELF_NOT_IN").Error(sender);	
			return;
		}
		
		UpdateClient(partyId);
		party.Save();
		
		Send_UpdateClient(NULL, player);

		ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_LEFT").Success(sender);
		
		SendNotificationToMembers(new StringLocaliser("STR_EXPANSION_PARTY_LEFT_MEMBERS", sender.GetName()), party, sender);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_Leave - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Dissolve
	// Called on client
	// ------------------------------------------------------------
	void Dissolve()
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Dissolve - Start");
		#endif
		
		if (!IsMissionClient())
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Send(NULL, ExpansionPartyModuleRPC.Dissolve, true);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Dissolve - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_Dissolve
	// Called on server
	// ------------------------------------------------------------
	private void RPC_Dissolve(ParamsReadContext ctx, PlayerIdentity senderRPC)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_Dissolve - Start");
		#endif
		
		Exec_Dissolve(senderRPC);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_Dissolve - Start");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_Dissolve
	// Called on server
	// ------------------------------------------------------------
	private void Exec_Dissolve(PlayerIdentity sender)
	{	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_Dissolve - Start");
		#endif
		
		if (!sender)
			return;
		
		PlayerBase player = PlayerBase.GetPlayerByUID(sender.GetId());
		if (!player)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_PLAYERBASE").Error(sender);
			return;
		}

		int partyId = GetPartyID(player);

		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_Dissolve partyId : " + partyId);
		#endif

		ExpansionPartyData party = m_Parties.Get(partyId);
		if (!party || partyId == -1)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_NOT_EXIST").Error(sender);
			return;
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_Dissolve party : " + party);
		#endif
		
		string senderID = sender.GetId();
		ExpansionPartyPlayerData senderPlayer = party.GetPlayer(senderID);
		if (!senderPlayer)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_NOT_EXIST").Error(sender);
			return;
		}
		
		if (!senderPlayer.CanDelete())
		{
			return;
		}
				
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_Dissolve before delete party : " + party);
		#endif
		
	#ifdef EXPANSIONMODMARKET
		//! Add all the money in the party deposit to the party owners deposit
		if (party.GetMoneyDeposited() > 0)
		{
			ExpansionMarketModule module = ExpansionMarketModule.Cast(GetModuleManager().GetModule(ExpansionMarketModule));
			if (module)
			{
				ExpansionMarketATM_Data data = module.GetPlayerATMData(party.GetOwnerUID());
				if (!data)
				{
					Error("ExpansionPartyModule::Exec_Dissolve - Could not get party owners ATM data!");			
					return;
				}
				
				data.AddMoney(party.GetMoneyDeposited());
				data.Save();
			}
		}
	#endif
		
		if (!RemoveParty(partyId))
		{
			Error("ExpansionPartyModule::Exec_Dissolve - Could not get delete party from party module parties array!");
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_Dissolve after delete party : " + party);
		#endif
				
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_Dissolve - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RemoveParty
	// Called on server
	// ------------------------------------------------------------	
	bool RemoveParty(int partyID)
	{
		for (int i = 0; i < m_Parties.Count(); i++)
		{
			int currentID = m_Parties.GetKey(i);
			ExpansionPartyData currentData = m_Parties.Get(currentID);
			if (!currentData)
				return false;
			
			if (currentID == partyID)
			{
				array<ref ExpansionPartyPlayerData> players = currentData.GetPlayers();
				for (int j = 0; j < players.Count(); j++)
				{
					ExpansionPartyPlayerData currPlayer = players[j];
					if (!currPlayer) 
					{
						Error("ExpansionPartyModule::RemoveParty - Could not get player's party data!");
						continue;
					}
					
					PlayerBase partyPlayer = PlayerBase.GetPlayerByUID(currPlayer.UID);
					if (!partyPlayer)
					{
						#ifdef EXPANSION_PARTY_MODULE_DEBUG
						EXLogPrint("ExpansionPartyModule::RemoveParty - Player with id "+ currPlayer.UID + " is not online! Skiping party client update for this member.");
						#endif
						continue;
					}
					
					#ifdef EXPANSION_PARTY_MODULE_DEBUG
					EXLogPrint("ExpansionPartyModule::RemoveParty removed player : " + currPlayer.UID);
					#endif
									
					if (partyPlayer && partyPlayer.GetIdentity())
					{
						Send_UpdateClient(NULL, partyPlayer);
						ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_PARTY_DISSOLVED").Success(partyPlayer.GetIdentity());
					}
				}
				
				//currentData.Delete();
				//delete currentData;
				m_Parties.Remove(currentData.GetPartyID());				
				return true;
			}
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion InvitePlayer
	// Called on client
	// ------------------------------------------------------------
	void InvitePlayer(string sendID)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::InvitePlayer - Start");
		#endif
		
		if (!IsMissionClient())
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(sendID);
		rpc.Send(NULL, ExpansionPartyModuleRPC.InvitePlayer, true, NULL);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::InvitePlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_InvitePlayer
	// Called on server
	// ------------------------------------------------------------
	private void RPC_InvitePlayer(ParamsReadContext ctx, PlayerIdentity senderRPC)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_InvitePlayer - Start");
		#endif
		
		string sendID;
		if ( !ctx.Read( sendID ) )
			return;

		Exec_InvitePlayer( sendID, senderRPC );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_InvitePlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_InvitePlayer
	// Called on server
	// ------------------------------------------------------------
	private void Exec_InvitePlayer(string targetID, notnull PlayerIdentity sender)
	{		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_InvitePlayer - Start");
		#endif
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID(sender.GetId());
		PlayerBase targetPlayer = PlayerBase.GetPlayerByUID(targetID);
		
		if (!senderPlayer || !targetPlayer)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_PLAYERBASE").Error(sender);
			return;
		}

		int partyId = GetPartyID(senderPlayer);
		ExpansionPartyData party = m_Parties.Get(partyId);
		if (!party || partyId == -1)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_NOT_EXIST").Error(sender);
			return;
		}
		
		ExpansionPartyPlayerData senderPlayerParty = party.GetPlayer(sender.GetId());
		if (!senderPlayerParty || !senderPlayerParty.CanInvite())
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_NOT_OWNER").Error(sender);
			return;
		}

		if (GetExpansionSettings().GetParty().MaxMembersInParty >= 1 && party.GetPlayers().Count() >= GetExpansionSettings().GetParty().MaxMembersInParty)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", new StringLocaliser("STR_EXPANSION_PARTY_ERROR_MAX_PARTY", GetExpansionSettings().GetParty().MaxMembersInParty.ToString())).Error(sender);
			return;
		}
		
		if (party.HasPlayerInvite(targetID))
		{
			SyncPlayersInvites(targetPlayer);
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_INVITED").Error(sender);
			return;
		}
	
		ExpansionPartyInviteData invite = party.AddInvite(targetID);
		if (invite == NULL)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_INVITE").Error(sender);
			return;		
		}
		
		party.Save();
		
		UpdateClient(partyId);
		
		SyncPlayersInvites(targetPlayer);

		if (GetExpansionSettings().GetLog().Party)
			GetExpansionSettings().GetLog().PrintLog("[Party] Player \"" + sender.GetName() + "\" (id=" + sender.GetId() + ")" + " invited the player \"" + targetPlayer.GetIdentity().GetName() + "\" (id=" + targetPlayer.GetIdentity().GetId() + ")" + "to the party named " + party.GetPartyName());

		ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_INVITED", party.GetPartyName()).Success(targetPlayer.GetIdentity());
		ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_INVITED_SENDER", targetPlayer.GetIdentity().GetName()).Success(sender);
	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_InvitePlayer - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion DeclineInvite
	// Called on client
	// ------------------------------------------------------------
	void DeclineInvite(int partyID)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::DeclineInvite - Start");
		#endif
		
		if (!IsMissionClient())
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(partyID);
		rpc.Send(NULL, ExpansionPartyModuleRPC.DeclineInvite, true, NULL);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::DeclineInvite - End");
		#endif
	}
	
		
	// ------------------------------------------------------------
	// Expansion RPC_DeclineInvite
	// Called on server
	// ------------------------------------------------------------
	private void RPC_DeclineInvite(ParamsReadContext ctx, PlayerIdentity senderRPC)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_DeclineInvite - Start");
		#endif
		
		int partyID;
		if (!ctx.Read(partyID))
			return;

		Exec_DeclineInvite(partyID, senderRPC);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_DeclineInvite - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_DeclineInvite
	// ------------------------------------------------------------
	private void Exec_DeclineInvite(int partyID, notnull PlayerIdentity sender)
	{	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_DeclineInvite - Start");
		#endif
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID(sender.GetId());
		if (!senderPlayer)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_PLAYERBASE").Error(sender);
			return;
		}
	
		ExpansionPartyData party = m_Parties.Get(partyID);
		if (!party)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_NOT_EXIST").Error(sender);
			return;
		}

		if (!party.DeclineInvite(sender.GetId()))
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_PLAYER_NOT_IN").Error(sender);
			return;
		}
		
		party.Save();
		UpdateClient(partyID); 
		
		SyncPlayersInvites(senderPlayer);
		
		ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", new StringLocaliser("STR_EXPANSION_PARTY_INVITE_DECLINED", party.GetPartyName())).Success(sender);
	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_DeclineInvite - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion AcceptInvite
	// Called on client
	// ------------------------------------------------------------
	void AcceptInvite(int partyID)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::AcceptInvite - Start");
		#endif
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(partyID);
		rpc.Send(NULL, ExpansionPartyModuleRPC.AcceptInvite, true, NULL);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::AcceptInvite - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_AcceptInvite
	// Called on server
	// ------------------------------------------------------------
	private void RPC_AcceptInvite(ParamsReadContext ctx, PlayerIdentity senderRPC)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_AcceptInvite - Start");
		#endif
		
		if (!IsMissionHost())
			return;
		
		int partyID;
		if (!ctx.Read(partyID))
			return;
		
		Exec_AcceptInvite(senderRPC, partyID);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_AcceptInvite - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_AcceptInvite
	// Called on server
	// ------------------------------------------------------------
	private void Exec_AcceptInvite(notnull PlayerIdentity sender, int partyId)
	{	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_AcceptInvite - Start");
		#endif
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID(sender.GetId());
		if (!senderPlayer)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_PLAYERBASE").Error(sender);
			return;
		}

		ExpansionPartyData party = m_Parties.Get(partyId);
		if (!party)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_NOT_EXIST").Error(sender);
			return;
		}
		
		if (party.IsMember(sender.GetId()))
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_NOT_EXIST").Error(sender);
			return;
		}
		
		if (HasParty(senderPlayer))
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_IN_PARTY").Error(sender);
			return;
		}

		if (GetExpansionSettings().GetParty().MaxMembersInParty >= 1 && party.GetPlayers().Count() >= GetExpansionSettings().GetParty().MaxMembersInParty)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", new StringLocaliser("STR_EXPANSION_PARTY_ERROR_MAX_PARTY", GetExpansionSettings().GetParty().MaxMembersInParty.ToString())).Error(sender);
			return;
		}
	
		if (!party.AcceptInvite(senderPlayer))
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", new StringLocaliser("Something went wrong! Could not accept party invite!")).Error(sender);
			return;
		}
		
		//! Remove the invite and save the party changes
		party.RemoveInvite(sender.GetId());
		party.Save();
		
		//! Update the senders party data
		Send_UpdateClient(party, senderPlayer);
		ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_MEMBER_ADDED").Success(sender);
		
		//! Inform all party players that a new player joined
		UpdateClient(partyId);
		SyncPlayersInvites(senderPlayer);
				
		SendNotificationToMembers(new StringLocaliser("STR_EXPANSION_PARTY_MEMBER_ADDED_SENDER", sender.GetName()), party, sender);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_AcceptInvite - End");
		#endif
	}
		
	// ------------------------------------------------------------
	// Expansion RemovePlayer
	// Called on client
	// ------------------------------------------------------------
	void RemovePlayer(string uid)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RemovePlayer - Start");
		#endif
		
		if (!IsMissionClient()) return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(uid);
		rpc.Send(NULL, ExpansionPartyModuleRPC.RemovePlayer, true, NULL);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RemovePlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_RemovePlayer
	// Called on server
	// ------------------------------------------------------------
	private void RPC_RemovePlayer(ParamsReadContext ctx, PlayerIdentity senderRPC)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_RemovePlayer - Start");
		#endif
		
		string uid;
		if (!ctx.Read(uid))
			return;

		Exec_RemovePlayer(uid, senderRPC);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_RemovePlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_RemovePlayer
	// Called on server
	// ------------------------------------------------------------
	private void Exec_RemovePlayer(string uid, notnull PlayerIdentity sender)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_RemovePlayer - Start");
		#endif
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID( sender.GetId() );
		if (!senderPlayer)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_PLAYERBASE").Error(sender);
			return;
		}

		int partyId = GetPartyID(senderPlayer);
		ExpansionPartyData party = m_Parties.Get(partyId);
		if (!party || partyId == -1)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_NOT_EXIST").Error(sender);
			return;
		}

		ExpansionPartyPlayerData senderPlayerParty = party.GetPlayer(sender.GetId());
		if (!senderPlayerParty.CanKick())
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_NOT_OWNER").Error(sender);
			return;
		}

		if (party.GetOwnerUID() == uid)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_REMOVE_OWNER").Error(sender);
			return;
		}
		
		if (!party.RemovePlayer(uid))
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_PLAYER_NOT_IN").Error(sender);
			return;
		}
		
		UpdateClient(partyId);
		party.Save();
		
		PlayerBase targetPlayer = PlayerBase.GetPlayerByUID(uid);
		
		if (targetPlayer)
		{
			Send_UpdateClient(NULL, targetPlayer);
			
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_MEMBER_REMOVED").Success(targetPlayer.GetIdentity());
		}
		
		ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_MEMBER_REMOVED_SENDER").Success(sender);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_RemovePlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateClient
	// Called on server
	// ------------------------------------------------------------
	void UpdateClient(ExpansionPartyData party)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdateClient - Start");
		#endif
		
		if (!IsMissionHost())
			return;

		array<ref ExpansionPartyPlayerData> players = party.GetPlayers();
		if (!players)
			return;
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdateClient - Start for party.GetPartyName() : " + party.GetPartyName());
		#endif
		
		for (int i = 0; i < players.Count(); i++)
		{
			ExpansionPartyPlayerData playerData = players[i];
			if (!playerData)
				continue;
			
			PlayerBase player = PlayerBase.GetPlayerByUID(playerData.UID);

			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint("ExpansionPartyModule::UpdateClient doing " + playerData.UID + " player:" + player);
			#endif

			if (!player)
				continue;

			Send_UpdateClient(party, player);

			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint("ExpansionPartyModule::UpdateClient finished " + playerData.UID + " player:" + player);
			#endif
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdateClient - End party.GetPartyName() : " + party.GetPartyName());
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateClient
	// Called on server
	// ------------------------------------------------------------
	void UpdateClient(int partyId)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdateClient - Start");
		#endif
		
		if (!IsMissionHost())
			return;

		ExpansionPartyData party = m_Parties.Get(partyId);
		if (!party)
		{
			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint("ExpansionPartyModule::UpdateClient - ExpansionPartyData not found for party with id: " + partyId);
			#endif
		}

		UpdateClient(party);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdateClient - End party.GetPartyName() : " + party.GetPartyName());
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion OnRecieveParty
	// ------------------------------------------------------------
	bool OnRecieveParty(ParamsReadContext ctx, out ExpansionPartyData party)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::OnRecieveParty - Start");
		#endif
		
		int id;
		if (Expansion_Assert_False(ctx.Read(id), "Failed to read id"))
			return false;

		party = m_Parties.Get(id);
		if (!party)
		{
			party = new ExpansionPartyData( id );
			m_Parties.Insert( id, party );
		}

		if (Expansion_Assert_False(party.OnRecieve(ctx), "Failed to read the party"))
			return false;

		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::OnRecieveParty - End");
		#endif

		return true;
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateClient
	// Called on server, when someone connect, because PlayerBase.GetPlayerByUID can't be used in the same frame as the connection of the player
	// ------------------------------------------------------------
	void UpdateClient(int partyId, PlayerBase player)
	{	
		if (!IsMissionHost() || !player)
			return;

		ExpansionPartyData party = m_Parties.Get(partyId);
		if (!party)
		{
			Send_UpdateClient(NULL, player);
		}
		else
		{
			Send_UpdateClient(party, player);
		}
	}

	// ------------------------------------------------------------
	// Expansion Send_UpdateClient
	// Called on server
	// ------------------------------------------------------------
	private void Send_UpdateClient(ExpansionPartyData party, notnull PlayerIdentity toSendToParty)
	{
		if (!IsMissionHost())
			return;
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Send_UpdateClient - Start for party.GetPartyName() : " + party.GetPartyName() + " identity : " + toSendToParty);
		#endif
		
		string senderID = toSendToParty.GetId();
		ScriptRPC rpc = new ScriptRPC();
		if (party && party != NULL && party.IsMember(senderID))
		{
			rpc.Write(true);
			party.OnSend(rpc);
		} 
		else if (!party || !party.IsMember(senderID) || party == NULL)
		{
			rpc.Write(false);
		}
		
		rpc.Send(null, ExpansionPartyModuleRPC.UpdateClient, true, toSendToParty);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Send_UpdateClient - End party.GetPartyName() : " + party.GetPartyName() + " identity : " + toSendToParty);
		#endif
	}

	// ------------------------------------------------------------
	// Expansion Send_UpdateClient
	// Called on server
	// ------------------------------------------------------------
	private void Send_UpdateClient(ExpansionPartyData party, PlayerBase player)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Send_UpdateClient - Start");
		#endif
		
		if (!player || !player.GetIdentity())
			return;
		
		string senderID = player.GetIdentity().GetId();
		ScriptRPC rpc = new ScriptRPC();
		if (party && party.IsMember(senderID))
		{
			rpc.Write(true);
			party.OnSend(rpc);
		} 
		else if (!party || !party.IsMember(senderID))
		{
			rpc.Write(false);
		}
		
		rpc.Send(null, ExpansionPartyModuleRPC.UpdateClient, true, player.GetIdentity());
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Send_UpdateClient - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_UpdateClient
	// Called on client
	// ------------------------------------------------------------
	private void RPC_UpdateClient(ParamsReadContext ctx, PlayerIdentity senderRPC)
	{
		if (!IsMissionClient())
			return;
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_UpdateClient - Start");
		#endif

		bool hasParty = false;
		if (!ctx.Read(hasParty))
		{
			Error("ExpansionPartyModule::RPC_UpdateClient can't read hasParty");
			return;
		}

		if (hasParty)
		{
			if (!OnRecieveParty(ctx, m_Party))
			{
				Error("ExpansionPartyModule::RPC_UpdateClient can't read party");
				return;
			}
		} 
		else
		{
			if (m_Party)
				delete m_Party;
				
			m_Party = NULL;
		}

		m_IsClientInitialized = true;

		ExpansionMarkerModule module;
		if (Class.CastTo(module, GetModuleManager().GetModule(ExpansionMarkerModule)))
		{
			module.Refresh();
		}

		ExpansionScriptViewMenuBase menu;
		if (Class.CastTo(menu, GetDayZGame().GetExpansionGame().GetExpansionUIManager().GetMenu()))
		{
			menu.Refresh();
		}
		
		m_PartyHUDInvoker.Invoke(this, m_Party);

		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_UpdateClient - End party : " + m_Party);
		#endif
	}
		
	// ------------------------------------------------------------
	// Expansion CreateMarker
	// ------------------------------------------------------------
	void CreateMarker(ExpansionMarkerData marker)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::CreateMarker - Start");
		#endif
		
		if (IsMissionClient())
		{
			ScriptRPC rpc = new ScriptRPC();
			marker.OnSend(rpc);
			rpc.Send(NULL, ExpansionPartyModuleRPC.CreateMarker, true, NULL);
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::CreateMarker - End");
		#endif
	}
		
	// -----------------------------------------------------------
	// Expansion Exec_CreateMarker
	// -----------------------------------------------------------
	private void Exec_CreateMarker(ExpansionMarkerData marker, PlayerIdentity sender)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_CreateMarker - Start");
		#endif
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID(sender.GetId());
		if (!senderPlayer)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_PLAYERBASE").Error(sender);
			return;
		}

		int partyId = GetPartyID(senderPlayer);
		ExpansionPartyData party = m_Parties.Get(partyId);
		if (!party || partyId == -1)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_NOT_EXIST").Error(sender);
			return;
		}

		party.AddMarker(marker);
		party.Save();

		ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_MARKER_ADDED").Success(sender);

		UpdateClient(partyId);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_CreateMarker - End");
		#endif
	}

	// -----------------------------------------------------------
	// Expansion RPC_CreateMarker
	// -----------------------------------------------------------
	private void RPC_CreateMarker(ParamsReadContext ctx, PlayerIdentity senderRPC)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_CreateMarker - Start");
		#endif
		
		ExpansionMarkerData marker = ExpansionMarkerData.Create(ExpansionMapMarkerType.PARTY);
		if (!marker.OnRecieve(ctx))
			return;

		Exec_CreateMarker(marker, senderRPC);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_CreateMarker - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion UpdateMarker
	// ------------------------------------------------------------
	void UpdateMarker(ExpansionMarkerData marker)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdateMarker - Start");
		#endif
		
		if (IsMissionClient())
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(marker.GetUID());
			marker.OnSend(rpc);
			rpc.Send(NULL, ExpansionPartyModuleRPC.UpdateMarker, true, NULL);
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdateMarker - End");
		#endif
	}

	// -----------------------------------------------------------
	// Expansion RPC_UpdateMarker
	// -----------------------------------------------------------
	private void RPC_UpdateMarker(ParamsReadContext ctx, PlayerIdentity senderRPC)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_UpdateMarker - Start");
		#endif
		
		string uid;
		if (!ctx.Read(uid))
			return;

		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID(senderRPC.GetId());
		if (!senderPlayer)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_PLAYERBASE").Error(senderRPC);
			return;
		}

		int partyId = GetPartyID(senderPlayer);
		ExpansionPartyData party = m_Parties.Get(partyId);
		if (!party || partyId == -1)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_NOT_EXIST").Error(senderRPC);
			return;
		}

		ExpansionMarkerData marker = party.GetMarker(uid);
		if (!marker.OnRecieve(ctx))
			return;

		party.UpdateMarker(marker);
		party.Save();

		ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_MARKER_ADDED").Success(senderRPC);

		UpdateClient(partyId);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_UpdateMarker - End");
		#endif
	}
		
	// -----------------------------------------------------------
	// Expansion DeleteMarker
	// -----------------------------------------------------------
	void DeleteMarker(string uid)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::DeleteMarker - Start");
		#endif
		
		if (IsMissionClient())
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(uid);
			rpc.Send(NULL, ExpansionPartyModuleRPC.DeleteMarker, true, NULL);
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::DeleteMarker - End");
		#endif
	}
	
	// -----------------------------------------------------------
	// Expansion RPC_DeleteMarker
	// -----------------------------------------------------------
	private void RPC_DeleteMarker(ParamsReadContext ctx, PlayerIdentity senderRPC)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_DeleteMarker - Start");
		#endif
		
		string uid;
		if (!ctx.Read(uid))
			return;

		Exec_DeleteMarker(uid, senderRPC);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_DeleteMarker - End");
		#endif
	}

	// -----------------------------------------------------------
	// Expansion Exec_DeleteMarker
	// -----------------------------------------------------------
	private void Exec_DeleteMarker(string uid, PlayerIdentity sender)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_DeleteMarker - Start");
		#endif
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID(sender.GetId());
		if (!senderPlayer)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_PLAYERBASE").Error(sender);
			return;
		}

		int partyId = GetPartyID(senderPlayer);
		ExpansionPartyData party = m_Parties.Get(partyId);
		if (!party || partyId == -1)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_NOT_EXIST").Error(sender);
			return;
		}

		if (party.RemoveMarker(uid))
		{
			party.Save();

			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_MARKER_REMOVED").Success(sender);
			
			UpdateClient(partyId);
		} else
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "Could not remove party marker!").Error(sender);
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_DeleteMarker - End");
		#endif
	}
	
	// -----------------------------------------------------------
	// Expansion UpdatePositionMarker
	// Called on client
	// -----------------------------------------------------------
	void UpdatePositionMarker(string uid, vector position)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdatePositionMarker - Start");
		#endif
		
		if (!IsMissionClient())
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(uid);
		rpc.Write(position);
		rpc.Send(NULL, ExpansionPartyModuleRPC.UpdatePositionMarker, true, NULL);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdatePositionMarker - End");
		#endif
	}
	
	// -----------------------------------------------------------
	// Expansion RPC_DeleteMarker
	// -----------------------------------------------------------
	private void RPC_UpdatePositionMarker(ParamsReadContext ctx, PlayerIdentity senderRPC)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_UpdatePositionMarker - Start");
		#endif
		
		string uid;
		if (!ctx.Read(uid))
			return;
		
		vector position;
		if (!ctx.Read(position))
			return;

		Exec_UpdatePositionMarker(uid, position, senderRPC);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_UpdatePositionMarker - End");
		#endif
	}
		
	// -----------------------------------------------------------
	// Expansion Exec_DeleteMarker
	// -----------------------------------------------------------
	private void Exec_UpdatePositionMarker(string uid, vector position, PlayerIdentity sender)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_UpdatePositionMarker - Start");
		#endif
		
		if (!sender) return;
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID(sender.GetId());
		if (!senderPlayer)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_PLAYERBASE").Error(sender);
			return;
		}

		int partyId = GetPartyID(senderPlayer);
		ExpansionPartyData party = m_Parties.Get(partyId);
		if (!party || partyId == -1)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_NOT_EXIST").Error(sender);
			return;
		}

		if (party.UpdateMarkerPosition(uid, position))
		{
			party.Save();

			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_MARKER_CHANGED").Success(sender);
			UpdateClient(partyId);
			SendNotificationToMembers(new StringLocaliser("STR_EXPANSION_PARTY_MARKER_CHANGED"), party, sender);
		} else
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "Could not update party marker!").Error(sender);
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_UpdatePositionMarker - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion UpdateQuickMarker
	// Called on client
	// ------------------------------------------------------------
	void UpdateQuickMarker(vector pos)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdateQuickMarker - Start");
		#endif
		
		if (!IsMissionClient())
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(pos);
		rpc.Send(NULL, ExpansionPartyModuleRPC.UpdateQuickMarker, true);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdateQuickMarker - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_UpdateQuickMarker
	// Called on server
	// ------------------------------------------------------------
	private void RPC_UpdateQuickMarker(ParamsReadContext ctx, PlayerIdentity sender)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_UpdateQuickMarker - Start");
		#endif
		
		vector position;
		if (!ctx.Read(position))
			return;
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID(sender.GetId());
		if (!senderPlayer)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_PLAYERBASE").Error(sender);
			return;
		}

		int partyId = GetPartyID(senderPlayer);
		ExpansionPartyData party = m_Parties.Get(partyId);
		if (!party || partyId == -1)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_NOT_EXIST").Error(sender);
			return;
		}

		ExpansionPartyPlayerData senderPlayerParty = party.GetPlayer(sender.GetId());
		if (!senderPlayerParty)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_NOT_EXIST").Error(sender);
			return;
		}

		senderPlayerParty.SetQuickMarker(position);
		UpdateClient(party);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_UpdateQuickMarker - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion UpdatePermissions
	// Called on client
	// ------------------------------------------------------------
	void UpdatePermissions(string memberID, ExpansionPartyPlayerPermissions perm)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdatePermissions - Start");
		#endif
		
		if (!IsMissionClient())
			return;
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdatePermissions - Member ID: " + memberID);
		EXLogPrint("ExpansionPartyModule::UpdatePermissions - Member Permissions: " + perm.ToString());
		#endif
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(memberID);
		rpc.Write(perm);
		rpc.Send(NULL, ExpansionPartyModuleRPC.UpdatePermissions, true);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdatePermissions - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_UpdatePermissions
	// Called on server
	// ------------------------------------------------------------
	private void RPC_UpdatePermissions(ParamsReadContext ctx, PlayerIdentity sender)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_UpdatePermissions - Start");
		#endif
		
		string memberID;
		if (!ctx.Read(memberID))
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "Could not get player's ID!").Error(sender);
			return;
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_UpdatePermissions - Member ID: " + memberID);
		#endif
		
		ExpansionPartyPlayerPermissions perm;
		if (!ctx.Read(perm))
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "Could not get player's party player permissions data!").Error(sender);
			return;
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdatePermissions - Member Permissions: " + perm.ToString());
		#endif
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID(sender.GetId());
		if (!senderPlayer)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_ERROR_PLAYERBASE").Error(sender);
			return;
		}

		int partyId = GetPartyID(senderPlayer);
		ExpansionPartyData senderParty = m_Parties.Get(partyId);
		if (!senderParty || partyId == -1)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "Could not get player's party data!").Error(sender);
			return;
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdatePermissions - Member Party ID: " + partyId.ToString());
		#endif
		
		Exec_UpdatePermissions(memberID, perm, senderParty, sender);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_UpdateQuickMarker - End");
		#endif
	}
	
	// -----------------------------------------------------------
	// Expansion Exec_UpdatePermissions
	// -----------------------------------------------------------
	private void Exec_UpdatePermissions(string memberID, ExpansionPartyPlayerPermissions perm, ExpansionPartyData senderParty,  PlayerIdentity sender)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_UpdatePermissions - Start");
		#endif
		
		ExpansionPartyPlayerData memberPartyPlayer = senderParty.GetPlayer(memberID);
		if (!memberPartyPlayer)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "Could not get member's party player data!").Error(sender);
			return;
		}
				
		if (!perm)
			return;
		
		if (!senderParty)
			return;
		
		if (!sender)
			return;
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_UpdatePermissions - Member Permissions: " + perm.ToString());
		#endif
		
		memberPartyPlayer.ApplyPermissions(perm);
		senderParty.Save();
		
		UpdateClient(senderParty);
	
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID(sender.GetId());
		if (!senderPlayer)
			return;
		
		Send_UpdateClient(senderParty, senderPlayer);
		
		PlayerBase memberPlayer = PlayerBase.GetPlayerByUID(memberID);
		if (!memberPlayer)
			return;
	
		ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "Changed party permissions for player %1!", memberPlayer.GetIdentity().GetName()).Success(sender);
		ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "Your party permissions have been changed!").Success(memberPlayer.GetIdentity());
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_UpdatePermissions - End");
		#endif
	}
	
	override void OnMissionFinish()
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::OnMissionFinish - Start");
		#endif
		
		m_Party = NULL;
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::OnMissionFinish - End");
		#endif
	}
		
	// -----------------------------------------------------------
	// Expansion OnClientRespawn
	// -----------------------------------------------------------
	override void OnClientRespawn(PlayerBase player, PlayerIdentity identity)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::OnClientRespawn - Start");
		#endif
				
		UpdatePlayerPartyData(player, identity);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::OnClientRespawn - End");
		#endif
	}
		
	// -----------------------------------------------------------
	// Expansion OnInvokeConnect
	// -----------------------------------------------------------
	override void OnInvokeConnect(PlayerBase player, PlayerIdentity identity)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::OnInvokeConnect - Start");
		#endif
		
		UpdatePlayerPartyData(player, identity);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::OnPlayerConnect - End - No Party Found");
		#endif
	}
	
	// -----------------------------------------------------------
	// Expansion OnInvokeConnect
	// -----------------------------------------------------------	
	override void OnClientReady( PlayerBase player, PlayerIdentity identity )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::OnClientReady - Start");
		#endif
		
		UpdatePlayerPartyData(player, identity);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::OnClientReady - End - No Party Found");
		#endif
	}
	
	// -----------------------------------------------------------
	// Expansion OnClientReconnect
	// -----------------------------------------------------------
	override void OnClientReconnect(PlayerBase player, PlayerIdentity identity)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::OnClientReconnect - Start");
		#endif
		
		UpdatePlayerPartyData(player, identity);
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::OnClientReconnect - End - No Party Found");
		#endif
	}
	
	// -----------------------------------------------------------
	// Expansion OnClientLogout
	// -----------------------------------------------------------
	override void OnClientLogout( PlayerBase player, PlayerIdentity identity, int logoutTime, bool authFailed )
	{
		if (!IsMissionHost())
			return;
		
		string uid = identity.GetId();
		foreach (int i, ExpansionPartyData data : m_Parties)
		{
			ExpansionPartyPlayerData party_player = data.GetPlayer(uid);
			if (!party_player)
				continue;

			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint("ExpansionPartyModule::OnClientLogout found a party : " + data.GetPartyName());
			#endif
			
			party_player.OnLeave();

			//Update all others clients
			UpdateClient(data);

			//Update our current client
			Send_UpdateClient(data, player);
			
			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint("ExpansionPartyModule::OnClientLogout - End");
			#endif
		}
	}
	
	// -----------------------------------------------------------
	// Expansion OnClientDisconnect
	// -----------------------------------------------------------
	override void OnClientDisconnect(PlayerBase player, PlayerIdentity identity, string uid)
	{
		if (!IsMissionHost())
			return;

		foreach (int i, ExpansionPartyData data : m_Parties)
		{
			ExpansionPartyPlayerData party_player = data.GetPlayer(uid);
			if (!party_player)
				continue;

			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint("ExpansionPartyModule::OnClientDisconnect found a party : " + data.GetPartyName());
			#endif
			
			party_player.OnLeave();

			//Update all others clients
			UpdateClient(data);

			//Update our current client
			Send_UpdateClient(data, player);
			
			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint("ExpansionPartyModule::OnClientDisconnect - End");
			#endif
		}
	}

	// -----------------------------------------------------------
	// Expansion UpdatePlayerPartyData
	// -----------------------------------------------------------
	private void UpdatePlayerPartyData(PlayerBase player, PlayerIdentity identity)
	{
		if (!IsMissionHost())
			return;

		if (!identity || !player)
			return;
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdatePlayerPartyData - Start for " + identity.GetId() + " player : " + player);
		#endif
		
		FindAndSyncPlayerInvites(player, identity);

		foreach (int i, ExpansionPartyData data : m_Parties)
		{
			ExpansionPartyPlayerData party_player = data.GetPlayer(identity.GetId());
			if (!party_player)
				continue;

			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint("ExpansionPartyModule::UpdatePlayerPartyData found a party : " + data.GetPartyName());
			#endif

			party_player.OnJoin(player);
			
			//Update all others clients
			UpdateClient(data);
			
			//Update our current client
			Send_UpdateClient(data, player);

			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint("ExpansionPartyModule::UpdatePlayerPartyData - End");
			#endif
		}
	}
	
	// -----------------------------------------------------------
	// Expansion Input_UpdateQuickMarker
	// -----------------------------------------------------------
	void Input_UpdateQuickMarker(UAInput input)
	{
		if (!(input.LocalPress()))
			return;

		if (!GetExpansionSettings().GetParty().EnableQuickMarker)
			return;
		
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player || !m_Party)
			return;

		if (!GetExpansionSettings().GetParty() || !GetExpansionSettings().GetParty().EnableQuickMarker)
			return;
		
		vector pos = vector.Zero;
		
		vector rayStart = GetGame().GetCurrentCameraPosition();
		vector rayEnd = rayStart + GetGame().GetCurrentCameraDirection() * 10000;
		vector hitPos;
		vector hitNormal;
		int hitComponentIndex;
		
		if (DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, null, null, player, false, false))
		{
			pos = hitPos;
		}

		UpdateQuickMarker(pos);
	}
	
	// -----------------------------------------------------------
	// Expansion Input_RemoveQuickMarker
	// -----------------------------------------------------------
	void Input_RemoveQuickMarker(UAInput input)
	{
		if (!(input.LocalPress()))
			return;

		if (!GetExpansionSettings().GetParty().EnableQuickMarker)
			return;
		
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player || !m_Party)
			return;

		if (!GetExpansionSettings().GetParty() || !GetExpansionSettings().GetParty().EnableQuickMarker)
			return;

		if (GetGame().GetUIManager().FindMenu(MENU_EXPANSION_MAP))
			return;

		UpdateQuickMarker(vector.Zero);
	}
	
	// -----------------------------------------------------------
	// Expansion SendNotificationToMembers
	// Send notification with given StringLocaliser data to all 
	// active members of the given party
	// -----------------------------------------------------------
	void SendNotificationToMembers(StringLocaliser text, ExpansionPartyData party, PlayerIdentity sender)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::SendNotificationToMembers - Start");
		#endif
		
		array<ref ExpansionPartyPlayerData> players = party.GetPlayers();	
	   	if ( !players ) return;
		
		for (int i = 0; i < players.Count(); i++)
		{
			ExpansionPartyPlayerData playerData = players[i];
			if (!playerData) continue;
			
			PlayerBase partyPlayer = PlayerBase.GetPlayerByUID(playerData.UID);			
			if (partyPlayer && partyPlayer.GetIdentity())
			{					
				if (partyPlayer.GetIdentityUID() != sender.GetId())
				{
					ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", text).Success(partyPlayer.GetIdentity());
				}
			}
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::SendNotificationToMembers - End");
		#endif
	}
	
	// -----------------------------------------------------------
	// Expansion HasParty
	// Called on client
	// -----------------------------------------------------------
	bool HasParty()
	{
		//#ifdef EXPANSION_PARTY_MODULE_DEBUG
		//EXLogPrint("ExpansionPartyModule::HasParty - Start");
		//#endif
		
		if (!IsMissionClient())
		{
			//#ifdef EXPANSION_PARTY_MODULE_DEBUG
			//EXLogPrint("ExpansionPartyModule::HasParty - return false");
			//#endif
			
			return false;
		}
			
		//#ifdef EXPANSION_PARTY_MODULE_DEBUG
		//EXLogPrint("ExpansionPartyModule::HasParty - return " + (m_Party != NULL).ToString());
		//#endif
		
		return (m_Party != NULL);
	}
	
	// -----------------------------------------------------------
	// Expansion HasParty
	// Called on server
	// -----------------------------------------------------------
	bool HasParty(PlayerBase player)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::HasParty - Start");
		#endif
		
		if (IsMissionHost())
		{				
			for (int i = 0; i < m_Parties.Count(); ++i)
			{
				ExpansionPartyData party = m_Parties.GetElement(i);
				if (party && party.GetPlayer(player.GetIdentityUID()))
				{
					#ifdef EXPANSION_PARTY_MODULE_DEBUG
					EXLogPrint("ExpansionPartyModule::HasParty - return true");
					#endif
					
					return true;
				}
			}
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::HasParty - return false");
		#endif
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion IsClientInitialized
	// ------------------------------------------------------------
	bool IsClientInitialized()
	{
		return m_IsClientInitialized;
	}
	
	// ------------------------------------------------------------
	// Expansion GetPartyID
	// Called on client
	// ------------------------------------------------------------
	int GetPartyID()
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::GetPartyID - Start");
		#endif
		
		if (!IsMissionClient() || m_Party == NULL)
		{
			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint("ExpansionPartyModule::GetPartyID - m_Party.GetPartyID(): -1");
			#endif
			
			return -1;
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::GetPartyID - m_Party.GetPartyID(): " + m_Party.GetPartyID());
		#endif
		
		return m_Party.GetPartyID();
	}
	
	// -----------------------------------------------------------
	// Expansion HasParty
	// Called on server
	// -----------------------------------------------------------
	int GetPartyID(PlayerBase player)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::GetPartyID - Start");
		#endif
		
		if (!IsMissionHost() || player.GetIdentityUID() == "") return -1;
		
		string id = player.GetIdentityUID();
		
		for (int i = 0; i < m_Parties.Count(); ++i)
		{
			ExpansionPartyData party = m_Parties.GetElement(i);
			if (party)
			{
				if (party.GetPlayer(id))
				{
					#ifdef EXPANSION_PARTY_MODULE_DEBUG
					EXLogPrint("ExpansionPartyModule::GetPartyID - party.GetPartyID(): " + party.GetPartyID());
					#endif
					
					return party.GetPartyID();
				}
			}
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::GetPartyID - party.GetPartyID(): -1");
		#endif
		
		return -1;
	}

	// ------------------------------------------------------------
	// Expansion GetParty
	// Called on client
	// ------------------------------------------------------------
	ExpansionPartyData GetParty()
	{
		//#ifdef EXPANSION_PARTY_MODULE_DEBUG
		//EXLogPrint("ExpansionPartyModule::GetParty - Start");
		//#endif
		
		if (!IsMissionClient())
		{
			//#ifdef EXPANSION_PARTY_MODULE_DEBUG
			//EXLogPrint("ExpansionPartyModule::GetParty - return m_Party: NULL");
			//#endif
			
			return NULL;
		}
		
		//#ifdef EXPANSION_PARTY_MODULE_DEBUG
		//EXLogPrint("ExpansionPartyModule::GetParty - return m_Party: " + m_Party.ToString());
		//#endif
		
		return m_Party;
	}
	
	// ------------------------------------------------------------
	// Expansion GetPartyInvites
	// ------------------------------------------------------------
	array<ref ExpansionPartyInviteData> GetPartyInvites()
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::GetPartyInvites - Start");
		#endif
		
		if (!IsMissionClient())
		{
			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint("ExpansionPartyModule::GetPartyInvites - return m_PartyInvites: NULL");
			#endif
			
			return NULL;
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::GetPartyInvites - m_PartyInvites: " + m_PartyInvites.ToString());
		#endif
		
		return m_PartyInvites;
	}
	
	// ------------------------------------------------------------
	// Expansion HasInvites
	// ------------------------------------------------------------
	bool HasInvites()
	{
		if (!IsMissionClient())
		{
			return false;
		}
		
		return (m_PartyInvites && m_PartyInvites.Count() > 0);
	}
	
	// ------------------------------------------------------------
	// Expansion SetPartyInvites
	// ------------------------------------------------------------
	void SetPartyInvites(array<ref ExpansionPartyInviteData> invites)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::SetPartyInvites - Start");
		#endif
		
		if (!IsMissionClient())
		{
			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint("ExpansionPartyModule::SetPartyInvites - return");
			#endif
			
			return;
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::SetPartyInvites - m_PartyInvites set to: " + invites.ToString());
		#endif

		m_PartyInvites = invites;
	}
	
	// ------------------------------------------------------------
	// Expansion OnUpdate
	// ------------------------------------------------------------
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		if (!IsMissionHost())
			return;
		
		m_UpdateQueueTimer += timeslice;
		if (m_UpdateQueueTimer >= UPDATE_TICK_TIME)
		{
			//! TODO: Ultimately we should really redo this whole thing in MVC,
			//! and then only send the info that changed, not all of it.

			if (m_Parties.Count() > 0)
			{
				int updatedParties;
				int updatedPlayers;

				while (updatedParties < m_Parties.Count())
				{
					if (m_CurrentPartyTick >= m_Parties.Count())
					{
						m_CurrentPartyTick = 0;
					}
					
					ExpansionPartyData party = m_Parties.GetElement(m_CurrentPartyTick);
					if (party.GetPlayers().Count() > 0)
					{
						if (m_CurrentPlayerTick >= party.GetPlayers().Count())
						{
							m_CurrentPlayerTick = 0;
						}

						if (m_CurrentPlayerTick == 0)
						{
							//! This is correct even if we do not update all party members this cycle
							updatedParties++;
						}

						while (updatedPlayers < UPDATE_PLAYERS_PER_TICK)
						{

							ExpansionPartyPlayerData playerData = party.GetPlayers()[m_CurrentPlayerTick];
							PlayerBase active_player = PlayerBase.GetPlayerByUID(playerData.UID);
							if (active_player && active_player.GetIdentity())
							{
								Send_UpdateClient( party, active_player );
								updatedPlayers++;
							}

							m_CurrentPlayerTick++;

							if (m_CurrentPlayerTick == party.GetPlayers().Count())
							{
								break;
							}
						}
					} else
					{
						updatedParties++;
					}
					
					if (m_CurrentPlayerTick == party.GetPlayers().Count())
					{
						m_CurrentPartyTick++;
						m_CurrentPlayerTick = 0;
					}

					if (updatedPlayers == UPDATE_PLAYERS_PER_TICK)
					{
						break;
					}
				}
			}
			else
			{
				m_CurrentPartyTick = 0;
			}
			
			m_UpdateQueueTimer = 0.0;
		}
	}
	
	// ------------------------------------------------------------
	// Expansion GetPartyByID
	// Called on client
	// ------------------------------------------------------------
	ExpansionPartyData GetPartyByID(int partyID)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::GetPartyByID - Start");
		#endif
		
		if ( !IsMissionHost() )
		{
			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint("ExpansionPartyModule::GetPartyByID - return m_Party: NULL");
			#endif
			
			return NULL;
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::GetPartyByID - return m_Party: " + m_Parties.Get(partyID).ToString());
		#endif
		
		return m_Parties.Get(partyID);
	}
}