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
	private ref map< int, ref ExpansionPartyData > m_Parties;
	
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
	
	void ExpansionPartyModule()
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXPrint("ExpansionPartyModule::Constructor - Start");
		#endif
		
		//Server
		m_Parties = new map< int, ref ExpansionPartyData >();
		
		//Client
		m_Party = NULL;
		m_PartyInvites = new array< ref ExpansionPartyInviteData >();
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXPrint("ExpansionPartyModule::Constructor - End");
		#endif
	}
	
	void ~ExpansionPartyModule()
	{
		delete m_Parties;
	}
	
	override void OnInit()
	{
		if ( IsMissionHost() )
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
		
		RegisterBinding( new JMModuleBinding( "Input_UpdateQuickMarker",			"UAExpansionQuickMarker",		true 	) );
		RegisterBinding( new JMModuleBinding( "Input_RemoveQuickMarker",			"UAExpansionMapDeleteMarker",	true 	) );
	}
	
	override void OnMissionStart() 
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXPrint( "ExpansionPartyModule::OnMissionStart - Start" );
		#endif

		super.OnMissionStart();

		if ( !IsMissionHost() )
		{
			return;
		}

		if ( !FileExist( EXPANSION_GROUPS_FOLDER ) )
		{
			MakeDirectory( EXPANSION_GROUPS_FOLDER );
		}
				
		array< string > group_files = ExpansionFindFilesInLocation( EXPANSION_GROUPS_FOLDER, ".bin" );

		foreach ( string fileName : group_files )
		{
			ExpansionPartyData party;
			if (FileExist(EXPANSION_GROUPS_FOLDER + fileName))
			{
				party = new ExpansionPartyData;
				
				FileSerializer file = new FileSerializer;
				
				if (file.Open(EXPANSION_GROUPS_FOLDER + fileName, FileMode.READ))
				{
					int version;
					if ( Expansion_Assert_False( file.Read( version ), "[" + this + ":" + fileName + "] Failed reading version" ) )
					{
						file.Close();
						continue;
					}
					
					if (party.OnStoreLoad(file, version))
					{
						party.InitMaps();
						
						m_Parties.Insert( party.GetPartyID(), party );

						if ( m_NextPartyID <= party.GetPartyID() )
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

		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXPrint("ExpansionPartyModule::OnMissionStart - End");
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
	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	{ 	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::OnRPC - Start");
		#endif
		
		switch ( rpc_type )
		{
		case ExpansionPartyModuleRPC.Create:
			RPC_Create( ctx, sender, target );
			break;
		case ExpansionPartyModuleRPC.Dissolve:
			RPC_Dissolve( ctx, sender, target );
			break;
		case ExpansionPartyModuleRPC.Leave:
			RPC_Leave( ctx, sender, target );
			break;
		case ExpansionPartyModuleRPC.AcceptInvite:
			RPC_AcceptInvite( ctx, sender, target );
			break;
		case ExpansionPartyModuleRPC.RemovePlayer:
			RPC_RemovePlayer( ctx, sender, target );
			break;
		case ExpansionPartyModuleRPC.UpdateClient:
			RPC_UpdateClient( ctx, sender, target );
			break;
		case ExpansionPartyModuleRPC.CreateMarker:
			RPC_CreateMarker( ctx, sender, target );
			break;
		case ExpansionPartyModuleRPC.UpdateMarker:
			RPC_UpdateMarker( ctx, sender, target );
			break;
		case ExpansionPartyModuleRPC.UpdatePositionMarker:
			RPC_UpdatePositionMarker( ctx, sender, target );
			break;
		case ExpansionPartyModuleRPC.DeleteMarker:
			RPC_DeleteMarker( ctx, sender, target );
			break;
		/*case ExpansionPartyModuleRPC.PromotePlayer:
			RPC_PromotePlayer( ctx, sender, target );
			break;
		case ExpansionPartyModuleRPC.DemotePlayer:
			RPC_DemotePlayer( ctx, sender, target );
			break;*/
		case ExpansionPartyModuleRPC.InvitePlayer:
			RPC_InvitePlayer( ctx, sender, target );
			break;
		case ExpansionPartyModuleRPC.DeclineInvite:
			RPC_DeclineInvite( ctx, sender, target );
			break;
		case ExpansionPartyModuleRPC.SyncPlayersInvites:
			RPC_SyncPlayersInvites( ctx, sender, target );
			break;
		case ExpansionPartyModuleRPC.UpdateQuickMarker:
			RPC_UpdateQuickMarker( ctx, sender, target );
			break;
		case ExpansionPartyModuleRPC.UpdatePermissions:	
			RPC_UpdatePermissions( ctx, sender, target );
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
	private void FindAndSyncPlayerInvites( PlayerBase player, notnull PlayerIdentity sender )
	{	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::FindAndSyncPlayerInvites - Start");
		#endif
		
		if ( !player )
		{
			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint( "ExpansionPartyModule::FindAndSyncPlayerInvites - [ERROR] Player object is empty or NULL!" );
			#endif

			return;
		}
		
		SyncPlayersInvites( player );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::FindAndSyncPlayerInvites - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// RequestSyncPlayersInvites
	// Called on server
	// ------------------------------------------------------------
	void SyncPlayersInvites( notnull PlayerBase sender )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::SyncPlayersInvites - Start");
		#endif
		
		if ( !IsMissionHost() || !sender.GetIdentity() )
			return;
		
		array< ref ExpansionPartyInviteData > invites = new array< ref ExpansionPartyInviteData >();
		
		foreach ( int i, ref ExpansionPartyData data : m_Parties )
		{
			ExpansionPartyInviteData invite = data.GetPlayerInvite( sender.GetIdentityUID() );
			if ( invite )
				invites.Insert( invite );
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::SyncPlayersInvites - Found " + invites.Count() + " invites");
		#endif
		
		ScriptRPC rpcServer = new ScriptRPC();
		rpcServer.Write( invites );
		rpcServer.Send( NULL, ExpansionPartyModuleRPC.SyncPlayersInvites, true, sender.GetIdentity() );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::SyncPlayersInvites - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionPartyModule RPC_SyncPlayersInvites
	// Called on client
	// ------------------------------------------------------------
	private void RPC_SyncPlayersInvites( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_SyncPlayersInvites - Start");
		#endif
		
		if ( !IsMissionClient() )
			return;
		
		array<ref ExpansionPartyInviteData> invites;
		if ( !ctx.Read( invites ) )
			return;
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_SyncPlayersInvites invites : " + invites + " invites.Count() : " + invites.Count());
		#endif

		SetPartyInvites( invites );
		
		ExpansionUIScriptedMenu menu;
		if ( Class.CastTo( menu, GetGame().GetUIManager().FindMenu( MENU_EXPANSION_BOOK_MENU ) ) )
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
	void Create( string partyName )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Create - Start");
		#endif
		
		if ( !IsMissionClient() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( partyName );
		rpc.Send( NULL, ExpansionPartyModuleRPC.Create, true, NULL );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Create - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion RPC_Create
	// Called on server
	// ------------------------------------------------------------
	private void RPC_Create( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_Create - Start");
		#endif
		
		string partyName;
		if ( !ctx.Read( partyName ) )
			return;

		Exec_Create( partyName, senderRPC );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_Create - Start");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_Create
	// Called on server
	// ------------------------------------------------------------
	private void Exec_Create( string partyName, PlayerIdentity sender )
	{	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_Create - Start");
		#endif
		
		PlayerBase player = PlayerBase.GetPlayerByUID(sender.GetId());
		if ( !player )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		if ( HasParty(player) )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_IN_PARTY" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
	
		ExpansionPartyData newParty = new ExpansionPartyData( m_NextPartyID );
		newParty.SetupExpansionPartyData( player, partyName );
		newParty.Save();
	
		m_Parties.Insert( m_NextPartyID, newParty );

		UpdateClient( m_NextPartyID++ );

		if ( GetExpansionSettings().GetLog().Party )
			GetExpansionSettings().GetLog().PrintLog( "[Party] Player \"" + sender.GetName() + "\" (id=" + sender.GetId() + ")" + " created a party named " + partyName );
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_SYSTEM_PARTY_CREATED" ), EXPANSION_NOTIFICATION_ICON_GROUP, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, sender );
	
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
		
		if ( !IsMissionClient() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Send( NULL, ExpansionPartyModuleRPC.Leave, true, NULL );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Leave - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_Leave
	// ------------------------------------------------------------
	private void RPC_Leave( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_Leave - Start");
		#endif
		
		Exec_Leave( senderRPC );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_Leave - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_Leave
	// ------------------------------------------------------------
	private void Exec_Leave( PlayerIdentity sender )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_Leave - Start");
		#endif
		
		if ( !sender )
			return;
		
		PlayerBase player = PlayerBase.GetPlayerByUID(sender.GetId());
		if ( !player )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		int partyId = GetPartyID(player);

		ref ExpansionPartyData party = m_Parties.Get( partyId );
		if ( !party || partyId == -1 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
		
		if ( party.GetOwnerUID() == sender.GetId() )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		if ( !party.RemovePlayer( sender.GetId() ) )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_SELF_NOT_IN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
		
		UpdateClient( partyId );
		party.Save();
		
		Send_UpdateClient( null, player );

		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_LEFT" ), EXPANSION_NOTIFICATION_ICON_GROUP, COLOR_EXPANSION_NOTIFICATION_INFO, 7, sender );
		
		SendNotificationToMembers( new StringLocaliser( "STR_EXPANSION_PARTY_LEFT_MEMBERS", sender.GetName() ), party, sender );
		
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
		
		if ( !IsMissionClient() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Send( NULL, ExpansionPartyModuleRPC.Dissolve, true );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Dissolve - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_Dissolve
	// Called on server
	// ------------------------------------------------------------
	private void RPC_Dissolve( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_Dissolve - Start");
		#endif
		
		Exec_Dissolve( senderRPC );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_Dissolve - Start");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_Dissolve
	// Called on server
	// ------------------------------------------------------------
	private void Exec_Dissolve( PlayerIdentity sender )
	{	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_Dissolve - Start");
		#endif
		
		if ( !sender )
			return;
		
		PlayerBase player = PlayerBase.GetPlayerByUID(sender.GetId());
		if ( !player )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		int partyId = GetPartyID(player);

		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_Dissolve partyId : " + partyId);
		#endif

		ref ExpansionPartyData party = m_Parties.Get( partyId );
		if ( !party || partyId == -1 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_Dissolve party : " + party);
		#endif

		ref array< ref ExpansionPartyPlayerData > players = party.GetPlayers();

		if ( sender.GetId() != party.GetOwnerUID() && players.Count() > 1 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_OWNER" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_Dissolve sender.GetId() : " + sender.GetId());
		#endif

		for ( int i = 0; i < players.Count(); i++ )
		{
			ref ExpansionPartyPlayerData currPlayer = players[i];
			if (!currPlayer) continue;
			
			PlayerBase partyPlayer = PlayerBase.GetPlayerByUID(currPlayer.UID);
			
			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint("ExpansionPartyModule::Exec_Dissolve removed player : " + currPlayer.UID);
			#endif
							
			if ( partyPlayer && partyPlayer.GetIdentity() )
			{
				Send_UpdateClient( NULL, partyPlayer );
				
				GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_PARTY_DISSOLVED" ), EXPANSION_NOTIFICATION_ICON_GROUP, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, partyPlayer.GetIdentity() );
			}
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_Dissolve beforeDelete party : " + party);
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
		
		party.Delete();

		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_Dissolve afterDelete party : " + party);
		#endif

		m_Parties.Remove( partyId );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_Dissolve - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion InvitePlayer
	// Called on client
	// ------------------------------------------------------------
	void InvitePlayer( string sendID )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::InvitePlayer - Start");
		#endif
		
		if ( !IsMissionClient() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( sendID );
		rpc.Send( NULL, ExpansionPartyModuleRPC.InvitePlayer, true, NULL );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::InvitePlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_InvitePlayer
	// Called on server
	// ------------------------------------------------------------
	private void RPC_InvitePlayer( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
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
	private void Exec_InvitePlayer( string targetID, notnull PlayerIdentity sender )
	{		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_InvitePlayer - Start");
		#endif
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID(sender.GetId());
		
		PlayerBase targetPlayer = PlayerBase.GetPlayerByUID(targetID);
		
		if ( !senderPlayer || !targetPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		int partyId = GetPartyID(senderPlayer);
		ref ExpansionPartyData party = m_Parties.Get( partyId );
		if ( !party || partyId == -1 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
		
		ref ExpansionPartyPlayerData senderPlayerParty = party.GetPlayer( sender.GetId() );
		if ( !senderPlayerParty || !senderPlayerParty.CanInvite() )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_OWNER" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		if ( GetExpansionSettings().GetParty().MaxMembersInParty >= 1 && party.GetPlayers().Count() >= GetExpansionSettings().GetParty().MaxMembersInParty )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_MAX_PARTY", GetExpansionSettings().GetParty().MaxMembersInParty.ToString() ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
		
		if ( party.HasPlayerInvite( targetID ) )
		{
			SyncPlayersInvites( targetPlayer );

			//TODO: Change message
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_IN_PARTY" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
	
		ExpansionPartyInviteData invite = party.AddInvite( targetID );
		
		if (invite == NULL)
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_INVITED", GetExpansionSettings().GetParty().MaxMembersInParty.ToString() ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;		
		}
		
		party.Save();
		
		UpdateClient( partyId );
		
		SyncPlayersInvites( targetPlayer );

		if ( GetExpansionSettings().GetLog().Party )
			GetExpansionSettings().GetLog().PrintLog( "[Party] Player \"" + sender.GetName() + "\" (id=" + sender.GetId() + ")" + " invited the player \"" + targetPlayer.GetIdentity().GetName() + "\" (id=" + targetPlayer.GetIdentity().GetId() + ")" + "to the party named " + party.GetPartyName() );

		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_MEMBER_ADDED" ), EXPANSION_NOTIFICATION_ICON_GROUP, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, targetPlayer.GetIdentity() );
		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_MEMBER_ADDED_SENDER" ), EXPANSION_NOTIFICATION_ICON_GROUP, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, sender );
	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_InvitePlayer - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion DeclineInvite
	// Called on client
	// ------------------------------------------------------------
	void DeclineInvite( int partyID )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::DeclineInvite - Start");
		#endif
		
		if ( !IsMissionClient() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( partyID );
		rpc.Send( NULL, ExpansionPartyModuleRPC.DeclineInvite, true, NULL );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::DeclineInvite - End");
		#endif
	}
	
		
	// ------------------------------------------------------------
	// Expansion RPC_DeclineInvite
	// Called on server
	// ------------------------------------------------------------
	private void RPC_DeclineInvite( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_DeclineInvite - Start");
		#endif
		
		int partyID;
		if ( !ctx.Read( partyID ) )
			return;

		Exec_DeclineInvite( partyID, senderRPC );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_DeclineInvite - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_DeclineInvite
	// ------------------------------------------------------------
	private void Exec_DeclineInvite( int partyID, notnull PlayerIdentity sender )
	{	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_DeclineInvite - Start");
		#endif
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID( sender.GetId() );
		if ( !senderPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
	
		ref ExpansionPartyData party = m_Parties.Get( partyID );
		if ( !party )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		if ( !party.DeclineInvite( sender.GetId() ) )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_PLAYER_NOT_IN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
		
		party.Save();
		UpdateClient( partyID ); 
		
		SyncPlayersInvites( senderPlayer );
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_MEMBER_REMOVED" ), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_INFO, 7, sender );
	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_DeclineInvite - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion AcceptInvite
	// Called on client
	// ------------------------------------------------------------
	void AcceptInvite( int partyID )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::AcceptInvite - Start");
		#endif
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( partyID );
		rpc.Send( NULL, ExpansionPartyModuleRPC.AcceptInvite, true, NULL );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::AcceptInvite - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_AcceptInvite
	// Called on server
	// ------------------------------------------------------------
	private void RPC_AcceptInvite( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_AcceptInvite - Start");
		#endif
		
		if ( !IsMissionHost() )
			return;
		
		int partyID;
		if ( !ctx.Read( partyID ) )
			return;
		
		Exec_AcceptInvite( senderRPC, partyID );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_AcceptInvite - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_AcceptInvite
	// Called on server
	// ------------------------------------------------------------
	private void Exec_AcceptInvite( notnull PlayerIdentity sender, int partyId )
	{	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_AcceptInvite - Start");
		#endif
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID( sender.GetId() );
		if ( !senderPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		ref ExpansionPartyData party = m_Parties.Get( partyId );
		if ( !party )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		if ( GetExpansionSettings().GetParty().MaxMembersInParty >= 1 && party.GetPlayers().Count() >= GetExpansionSettings().GetParty().MaxMembersInParty )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_MAX_PARTY", GetExpansionSettings().GetParty().MaxMembersInParty.ToString() ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
	
		if ( !party.AcceptInvite( senderPlayer ) )
		{
			//TODO: Notification
			return;
		}

		party.Save();
	
		SyncPlayersInvites( senderPlayer );
		
		UpdateClient( partyId );
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_MEMBER_ADDED" ), EXPANSION_NOTIFICATION_ICON_GROUP, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, sender );
	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_AcceptInvite - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion PromotePlayer
	// Called on client
	// ------------------------------------------------------------
	/*void PromotePlayer( string uid )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::PromotePlayer - Start");
		#endif
		
		if ( !IsMissionClient() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(uid);
		rpc.Send( NULL, ExpansionPartyModuleRPC.PromotePlayer, true, NULL );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::PromotePlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_PromotePlayer
	// Called on server
	// ------------------------------------------------------------
	private void RPC_PromotePlayer( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_PromotePlayer - Start");
		#endif
		
		string targetID;
		if ( !ctx.Read( targetID ) )
			return;

		Exec_PromotePlayer( targetID, senderRPC );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_PromotePlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_PromotePlayer
	// ------------------------------------------------------------
	private void Exec_PromotePlayer( string targetID, notnull PlayerIdentity sender )
	{	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_PromotePlayer - Start");
		#endif
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID( sender.GetId() );
		if ( !senderPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		int partyId = GetPartyID(senderPlayer);
		ref ExpansionPartyData party = m_Parties.Get( partyId );
		if ( !party || partyId == -1 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		if ( sender.GetId() != party.GetOwnerUID() )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_OWNER" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
		
		if ( targetID == party.GetOwnerUID() )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
		
		ref ExpansionPartyPlayerData targetPlayer = party.GetPlayer( targetID );
		if (!targetPlayer)
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
		
		targetPlayer.Promoted = true;

		UpdateClient( partyId );
		
		PlayerBase targetPlayerPB = PlayerBase.GetPlayerByUID( targetID );
		if (targetPlayerPB)
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_MEMBER_PROMOTED", "STR_EXPANSION_PARTY_RANK_MOD" ), EXPANSION_NOTIFICATION_ICON_GROUP, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, targetPlayerPB.GetIdentity() );
		}
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_MEMBER_PROMOTED_SENDER", targetPlayer.Name ), EXPANSION_NOTIFICATION_ICON_GROUP, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, sender );
	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_PromotePlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion DemotePlayer
	// Called on client
	// ------------------------------------------------------------
	void DemotePlayer( string uid )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::DemotePlayer - Start");
		#endif
		
		if ( !IsMissionClient() ) return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(uid);
		rpc.Send( NULL, ExpansionPartyModuleRPC.DemotePlayer, true, NULL );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::DemotePlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_PromotePlayer
	// Called on server
	// ------------------------------------------------------------
	private void RPC_DemotePlayer( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_DemotePlayer - Start");
		#endif
		
		string targetID;
		if ( !ctx.Read( targetID ) )
			return;

		Exec_DemotePlayer( targetID, senderRPC );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_DemotePlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_PromotePlayer
	// ------------------------------------------------------------
	private void Exec_DemotePlayer( string targetID, notnull PlayerIdentity sender )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_DemotePlayer - Start");
		#endif
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID( sender.GetId() );
		if ( !senderPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		int partyId = GetPartyID(senderPlayer);
		ref ExpansionPartyData party = m_Parties.Get( partyId );
		if ( !party || partyId == -1 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		if ( sender.GetId() != party.GetOwnerUID() )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_OWNER" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
		
		if ( targetID == party.GetOwnerUID() )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_CANT_PROMOTE_OWNER" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
		
		ref ExpansionPartyPlayerData targetPlayer = party.GetPlayer( targetID );
		if (!targetPlayer)
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
		
		targetPlayer.Promoted = false;

		UpdateClient( partyId );
		
		PlayerBase targetPlayerPB = PlayerBase.GetPlayerByUID( targetID );
		if (targetPlayerPB)
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_MEMBER_DEMOTED", "STR_EXPANSION_PARTY_RANK_MEMBER" ), EXPANSION_NOTIFICATION_ICON_GROUP, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, targetPlayerPB.GetIdentity() );
		}
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_MEMBER_DEMOTED_SENDER", targetPlayer.Name ), EXPANSION_NOTIFICATION_ICON_GROUP, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, sender );
	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_DemotePlayer - End");
		#endif
	}*/
	
	// ------------------------------------------------------------
	// Expansion RemovePlayer
	// Called on client
	// ------------------------------------------------------------
	void RemovePlayer( string uid )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RemovePlayer - Start");
		#endif
		
		if (!IsMissionClient()) return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( uid );
		rpc.Send( NULL, ExpansionPartyModuleRPC.RemovePlayer, true, NULL );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RemovePlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_RemovePlayer
	// Called on server
	// ------------------------------------------------------------
	private void RPC_RemovePlayer( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_RemovePlayer - Start");
		#endif
		
		string uid;
		if ( !ctx.Read( uid ) )
			return;

		Exec_RemovePlayer( uid, senderRPC );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_RemovePlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_RemovePlayer
	// Called on server
	// ------------------------------------------------------------
	private void Exec_RemovePlayer( string uid, notnull PlayerIdentity sender )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_RemovePlayer - Start");
		#endif
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID( sender.GetId() );
		if ( !senderPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		int partyId = GetPartyID(senderPlayer);
		ref ExpansionPartyData party = m_Parties.Get( partyId );
		if ( !party || partyId == -1 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		ref ExpansionPartyPlayerData senderPlayerParty = party.GetPlayer( sender.GetId() );
		if ( !senderPlayerParty.CanKick() )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_OWNER" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		if ( party.GetOwnerUID() == uid )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_REMOVE_OWNER" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
		
		if ( !party.RemovePlayer( uid ) )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_PLAYER_NOT_IN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
		
		UpdateClient( partyId );
		party.Save();
		
		PlayerBase targetPlayer = PlayerBase.GetPlayerByUID(uid);
		
		if ( targetPlayer )
		{
			Send_UpdateClient( NULL, targetPlayer );
			
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_MEMBER_REMOVED" ), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_INFO, 7, targetPlayer.GetIdentity() );
		}
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_MEMBER_REMOVED_SENDER" ), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, sender );
	
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_RemovePlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateClient
	// Called on server
	// ------------------------------------------------------------
	void UpdateClient( ref ExpansionPartyData party )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdateClient - Start");
		#endif
		
		if ( !IsMissionHost() )
			return;

		array< ref ExpansionPartyPlayerData > players = party.GetPlayers();
		if ( !players )
			return;
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdateClient - Start for party.GetPartyName() : " + party.GetPartyName());
		#endif
		
		for ( int i = 0; i < players.Count(); i++ )
		{
			ExpansionPartyPlayerData playerData = players[i];
			if ( !playerData )
				continue;
			
			PlayerBase player = PlayerBase.GetPlayerByUID(playerData.UID);

			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint("ExpansionPartyModule::UpdateClient doing " + playerData.UID + " player:" + player);
			#endif

			if ( !player )
				continue;

			Send_UpdateClient( party, player );

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
	void UpdateClient( int partyId )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdateClient - Start");
		#endif
		
		if ( !IsMissionHost() )
			return;

		ref ExpansionPartyData party = m_Parties.Get( partyId );
		if ( !party )
		{
			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint("ExpansionPartyModule::UpdateClient - ERROR: ExpansionPartyData not found for party with id: " + partyId);
			#endif
			return;
		}

		UpdateClient( party );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdateClient - End party.GetPartyName() : " + party.GetPartyName());
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion OnRecieveParty
	// ------------------------------------------------------------
	bool OnRecieveParty( ParamsReadContext ctx, out ExpansionPartyData party )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::OnRecieveParty - Start");
		#endif
		
		int id;
		if ( Expansion_Assert_False( ctx.Read( id ), "Failed to read id" ) )
			return false;

		party = m_Parties.Get( id );
		if ( !party )
		{
			party = new ExpansionPartyData( id );
			m_Parties.Insert( id, party );
		}

		if ( Expansion_Assert_False( party.OnRecieve( ctx ), "Failed to read the party" ) )
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
	void UpdateClient( int partyId, PlayerBase player )
	{		
		if ( !IsMissionHost() || !player )
			return;

		ref ExpansionPartyData party = m_Parties.Get( partyId );
		if ( !party )
			return;
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdateClient - Start for party.GetPartyName() : " + party.GetPartyName() + " player : " + player);
		#endif

		Send_UpdateClient( party, player );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdateClient - End party.GetPartyName() : " + party.GetPartyName() + " player : " + player);
		#endif
	}

	// ------------------------------------------------------------
	// Expansion Send_UpdateClient
	// Called on server
	// ------------------------------------------------------------
	private void Send_UpdateClient( ref ExpansionPartyData party, notnull PlayerIdentity toSendToParty )
	{		
		if ( !IsMissionHost() )
			return;
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Send_UpdateClient - Start for party.GetPartyName() : " + party.GetPartyName() + " identity : " + toSendToParty);
		#endif
		
		ScriptRPC rpc = new ScriptRPC();
		if ( party )
		{
			rpc.Write( true );
			party.OnSend( rpc );
		} else
		{
			rpc.Write( false );
		}
		
		rpc.Send( null, ExpansionPartyModuleRPC.UpdateClient, true, toSendToParty );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Send_UpdateClient - End party.GetPartyName() : " + party.GetPartyName() + " identity : " + toSendToParty);
		#endif
	}

	// ------------------------------------------------------------
	// Expansion Send_UpdateClient
	// Called on server
	// ------------------------------------------------------------
	private void Send_UpdateClient( ref ExpansionPartyData party, PlayerBase player )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Send_UpdateClient - Start");
		#endif
		
		if ( !player || !player.GetIdentity() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		if ( party )
		{
			rpc.Write( true );
			party.OnSend( rpc );
		} else
		{
			rpc.Write( false );
		}
		
		rpc.Send( null, ExpansionPartyModuleRPC.UpdateClient, true, player.GetIdentity() );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Send_UpdateClient - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_UpdateClient
	// Called on client
	// ------------------------------------------------------------
	private void RPC_UpdateClient( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{		
		if ( !IsMissionClient() )
			return;
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_UpdateClient - Start");
		#endif

		bool hasParty = false;
		if ( !ctx.Read( hasParty ) )
		{
			Error("ExpansionPartyModule::RPC_UpdateClient can't read hasParty");
			return;
		}

		if ( hasParty )
		{
			if ( !OnRecieveParty( ctx, m_Party ) )
			{
				Error("ExpansionPartyModule::RPC_UpdateClient can't read party");
				return;
			}
		} else
		{
			if ( m_Party )
				delete m_Party;
				
			m_Party = NULL;
		}

		m_IsClientInitialized = true;

		ExpansionMarkerModule module;
		if ( Class.CastTo( module, GetModuleManager().GetModule( ExpansionMarkerModule ) ) )
		{
			module.Refresh();
		}

		ExpansionUIScriptedMenu menu;
		if ( Class.CastTo( menu, GetGame().GetUIManager().FindMenu( MENU_EXPANSION_BOOK_MENU ) ) )
		{
			menu.Refresh();
		}

		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_UpdateClient - End party : " + m_Party);
		#endif
	}
		
	// ------------------------------------------------------------
	// Expansion CreateMarker
	// ------------------------------------------------------------
	void CreateMarker( ExpansionMarkerData marker )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::CreateMarker - Start");
		#endif
		
		if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			marker.OnSend( rpc );
			rpc.Send( NULL, ExpansionPartyModuleRPC.CreateMarker, true, NULL );
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::CreateMarker - End");
		#endif
	}
		
	// -----------------------------------------------------------
	// Expansion Exec_CreateMarker
	// -----------------------------------------------------------
	private void Exec_CreateMarker( ExpansionMarkerData marker, PlayerIdentity sender )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_CreateMarker - Start");
		#endif
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID(sender.GetId());
		if ( !senderPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		int partyId = GetPartyID( senderPlayer) ;
		ref ExpansionPartyData party = m_Parties.Get( partyId );
		if ( !party || partyId == -1 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		party.AddMarker( marker );
		party.Save();

		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_MARKER_ADDED", marker.GetName() ), EXPANSION_NOTIFICATION_ICON_MARKER, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, sender );

		UpdateClient( partyId );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_CreateMarker - End");
		#endif
	}

	// -----------------------------------------------------------
	// Expansion RPC_CreateMarker
	// -----------------------------------------------------------
	private void RPC_CreateMarker( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_CreateMarker - Start");
		#endif
		
		ExpansionMarkerData marker = ExpansionMarkerData.Create( ExpansionMapMarkerType.PARTY );
		if ( !marker.OnRecieve( ctx ) )
			return;

		Exec_CreateMarker( marker, senderRPC );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_CreateMarker - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion UpdateMarker
	// ------------------------------------------------------------
	void UpdateMarker( ExpansionMarkerData marker )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdateMarker - Start");
		#endif
		
		if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( marker.GetUID() );
			marker.OnSend( rpc );
			rpc.Send( NULL, ExpansionPartyModuleRPC.UpdateMarker, true, NULL );
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdateMarker - End");
		#endif
	}

	// -----------------------------------------------------------
	// Expansion RPC_UpdateMarker
	// -----------------------------------------------------------
	private void RPC_UpdateMarker( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_UpdateMarker - Start");
		#endif
		
		string uid;
		if ( !ctx.Read( uid ) )
			return;

		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID( senderRPC.GetId() );
		if ( !senderPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, senderRPC );
			return;
		}

		int partyId = GetPartyID( senderPlayer );
		ref ExpansionPartyData party = m_Parties.Get( partyId );
		if ( !party || partyId == -1 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, senderRPC );
			return;
		}

		ExpansionMarkerData marker = party.GetMarker( uid );
		if ( !marker.OnRecieve( ctx ) )
			return;

		party.UpdateMarker( marker );
		party.Save();

		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_MARKER_ADDED", marker.GetName() ), EXPANSION_NOTIFICATION_ICON_MARKER, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, senderRPC );

		UpdateClient( partyId );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_UpdateMarker - End");
		#endif
	}
		
	// -----------------------------------------------------------
	// Expansion DeleteMarker
	// -----------------------------------------------------------
	void DeleteMarker( string uid )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::DeleteMarker - Start");
		#endif
		
		if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( uid );
			rpc.Send( NULL, ExpansionPartyModuleRPC.DeleteMarker, true, NULL );
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::DeleteMarker - End");
		#endif
	}
	
	// -----------------------------------------------------------
	// Expansion RPC_DeleteMarker
	// -----------------------------------------------------------
	private void RPC_DeleteMarker( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_DeleteMarker - Start");
		#endif
		
		string uid;
		if ( !ctx.Read( uid ) )
			return;

		Exec_DeleteMarker( uid, senderRPC );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_DeleteMarker - End");
		#endif
	}

	// -----------------------------------------------------------
	// Expansion Exec_DeleteMarker
	// -----------------------------------------------------------
	private void Exec_DeleteMarker( string uid, PlayerIdentity sender )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_DeleteMarker - Start");
		#endif
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID(sender.GetId());
		if ( !senderPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		int partyId = GetPartyID(senderPlayer);
		ref ExpansionPartyData party = m_Parties.Get( partyId );
		if ( !party || partyId == -1 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		if ( party.RemoveMarker( uid ) )
		{
			party.Save();

			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_MARKER_REMOVED" ), EXPANSION_NOTIFICATION_ICON_MARKER, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, sender );
			
			UpdateClient( partyId );
		} else
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_DeleteMarker - End");
		#endif
	}
	
	// -----------------------------------------------------------
	// Expansion UpdatePositionMarker
	// Called on client
	// -----------------------------------------------------------
	void UpdatePositionMarker( string uid, vector position )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdatePositionMarker - Start");
		#endif
		
		if ( !IsMissionClient() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( uid );
		rpc.Write( position );
		rpc.Send( NULL, ExpansionPartyModuleRPC.UpdatePositionMarker, true, NULL );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdatePositionMarker - End");
		#endif
	}
	
	// -----------------------------------------------------------
	// Expansion RPC_DeleteMarker
	// -----------------------------------------------------------
	private void RPC_UpdatePositionMarker( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_UpdatePositionMarker - Start");
		#endif
		
		string uid;
		if ( !ctx.Read( uid ) )
			return;
		
		vector position;
		if ( !ctx.Read( position ) )
			return;

		Exec_UpdatePositionMarker( uid, position, senderRPC );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_UpdatePositionMarker - End");
		#endif
	}
		
	// -----------------------------------------------------------
	// Expansion Exec_DeleteMarker
	// -----------------------------------------------------------
	private void Exec_UpdatePositionMarker( string uid, vector position, PlayerIdentity sender )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_UpdatePositionMarker - Start");
		#endif
		
		if (!sender) return;
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID(sender.GetId());
		if ( !senderPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		int partyId = GetPartyID(senderPlayer);
		ref ExpansionPartyData party = m_Parties.Get( partyId );
		if ( !party || partyId == -1 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		if ( party.UpdateMarkerPosition( uid, position ) )
		{
			party.Save();

			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_MARKER_CHANGED" ), EXPANSION_NOTIFICATION_ICON_MARKER, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, sender );
			UpdateClient( partyId );
			SendNotificationToMembers( new StringLocaliser( "STR_EXPANSION_PARTY_MARKER_CHANGED" ), party, sender );
		} else
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::Exec_UpdatePositionMarker - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion UpdateQuickMarker
	// Called on client
	// ------------------------------------------------------------
	void UpdateQuickMarker( vector pos )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdateQuickMarker - Start");
		#endif
		
		if ( !IsMissionClient() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( pos );
		rpc.Send( NULL, ExpansionPartyModuleRPC.UpdateQuickMarker, true );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdateQuickMarker - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_UpdateQuickMarker
	// Called on server
	// ------------------------------------------------------------
	private void RPC_UpdateQuickMarker( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_UpdateQuickMarker - Start");
		#endif
		
		vector position;
		if ( !ctx.Read( position ) )
			return;
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID( senderRPC.GetId() );
		if ( !senderPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, senderRPC );
			return;
		}

		int partyId = GetPartyID( senderPlayer );
		ref ExpansionPartyData party = m_Parties.Get( partyId );
		if ( !party || partyId == -1 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, senderRPC );
			return;
		}

		ref ExpansionPartyPlayerData senderPlayerParty = party.GetPlayer( senderRPC.GetId() );
		if ( !senderPlayerParty )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, senderRPC );
			return;
		}
		

		senderPlayerParty.SetQuickMarker( position );
		UpdateClient( party );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_UpdateQuickMarker - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion UpdatePermissions
	// Called on client
	// ------------------------------------------------------------
	void UpdatePermissions(ExpansionPartyPlayerData player, ExpansionPartyPlayerPermissions perm)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::UpdatePermissions - Start");
		#endif
		
		if ( !IsMissionClient() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(player);
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
	private void RPC_UpdatePermissions(ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_UpdatePermissions - Start");
		#endif
		
		ref ExpansionPartyPlayerData player;
		if (!ctx.Read(player))
			return;
		
		ExpansionPartyPlayerPermissions perm;
		if (!ctx.Read(perm))
			return;
		
		Print("ExpansionPartyModule::RPC_UpdatePermissions - perm: " + perm);
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID( senderRPC.GetId() );
		if ( !senderPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, senderRPC );
			return;
		}

		int partyId = GetPartyID( senderPlayer );
		ref ExpansionPartyData senderParty = m_Parties.Get( partyId );
		if ( !senderParty || partyId == -1 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, senderRPC );
			return;
		}

		ref ExpansionPartyPlayerData memberPartyPlayer = senderParty.GetPlayer( player.GetID() );
		if ( !memberPartyPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, senderRPC );
			return;
		}
		
		memberPartyPlayer.ApplyPermissions(perm);
		senderParty.Save();
		
		UpdateClient( senderParty );
		PlayerBase playerB = PlayerBase.Cast(GetPlayerObjectByIdentity(senderRPC));
		Send_UpdateClient( senderParty, playerB );		
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::RPC_UpdateQuickMarker - End");
		#endif
	}
	
	// -----------------------------------------------------------
	// Expansion OnClientRespawn
	// -----------------------------------------------------------
	override void OnClientRespawn( PlayerBase player, PlayerIdentity identity )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::OnClientRespawn - Start");
		#endif
		
		OnInvokeConnect( player, identity );
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::OnClientRespawn - End");
		#endif
	}
		
	// -----------------------------------------------------------
	// Expansion OnInvokeConnect
	// -----------------------------------------------------------
	override void OnInvokeConnect( PlayerBase player, PlayerIdentity identity )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::OnInvokeConnect - Start");
		#endif
		
		if ( !identity || !player )
			return;
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint( "ExpansionPartyModule::OnPlayerConnect - Start for " + identity.GetId() + " player : " + player );
		#endif
		
		FindAndSyncPlayerInvites( player, identity );

		foreach ( int i, ref ExpansionPartyData data : m_Parties )
		{
			ExpansionPartyPlayerData party_player = data.GetPlayer( identity.GetId() );
			if ( !party_player )
				continue;

			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint("ExpansionPartyModule::OnPlayerConnect found a party : " + data.GetPartyName());
			#endif

			party_player.OnJoin( player );
			
			//Update all others clients
			UpdateClient( data );
			
			//Update our current client
			Send_UpdateClient( data, player );

			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint("ExpansionPartyModule::OnPlayerConnect - End");
			#endif

			return;
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::OnPlayerConnect - End - No Party Found");
		#endif
	}
	
	// -----------------------------------------------------------
	// Expansion OnClientDisconnect
	// -----------------------------------------------------------
	override void OnClientDisconnect( PlayerBase player, PlayerIdentity identity, string uid )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::OnClientDisconnect - Start");
		#endif
		
		foreach ( int i, ref ExpansionPartyData data : m_Parties )
		{
			ExpansionPartyPlayerData party_player = data.GetPlayer( uid );
			if ( !party_player )
				continue;

			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint("ExpansionPartyModule::OnClientDisconnect found a party : " + data.GetPartyName());
			#endif
			
			party_player.OnLeave();

			//Update all others clients
			UpdateClient( data );

			//Update our current client
			Send_UpdateClient( data, player );
			
			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint("ExpansionPartyModule::OnClientDisconnect - End");
			#endif

			return;
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::OnClientDisconnect - End - No Party Found");
		#endif
	}
	
	// -----------------------------------------------------------
	// Expansion Input_UpdateQuickMarker
	// -----------------------------------------------------------
	void Input_UpdateQuickMarker( UAInput input )
	{
		if ( !(input.LocalPress()) )
			return;

		if ( !GetExpansionSettings().GetParty().EnableQuickMarker )
			return;
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( !player || !m_Party )
			return;

		if ( !GetExpansionSettings().GetParty() || !GetExpansionSettings().GetParty().EnableQuickMarker )
			return;
		
		vector pos = vector.Zero;
		
		vector rayStart = GetGame().GetCurrentCameraPosition();
		vector rayEnd = rayStart + GetGame().GetCurrentCameraDirection() * 10000;
		vector hitPos;
		vector hitNormal;
		int hitComponentIndex;
		
		if ( DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, null, null, player, false, false) )
		{
			pos = hitPos;
		}

		UpdateQuickMarker( pos );
	}
	
	// -----------------------------------------------------------
	// Expansion Input_RemoveQuickMarker
	// -----------------------------------------------------------
	void Input_RemoveQuickMarker( UAInput input )
	{
		if ( !(input.LocalPress()) )
			return;

		if ( !GetExpansionSettings().GetParty().EnableQuickMarker )
			return;
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( !player || !m_Party )
			return;

		if ( !GetExpansionSettings().GetParty() || !GetExpansionSettings().GetParty().EnableQuickMarker )
			return;

		if ( GetGame().GetUIManager().FindMenu( MENU_EXPANSION_MAP ) )
			return;

		UpdateQuickMarker( vector.Zero );
	}
	
	// -----------------------------------------------------------
	// Expansion SendNotificationToMembers
	// Send notification with given StringLocaliser data to all 
	// active members of the given party
	// -----------------------------------------------------------
	void SendNotificationToMembers( StringLocaliser text, ExpansionPartyData party, PlayerIdentity sender )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::SendNotificationToMembers - Start");
		#endif
		
		ref array< ref ExpansionPartyPlayerData > players = party.GetPlayers();	
	   	if ( !players ) return;
		
		for ( int i = 0; i < players.Count(); i++ )
		{
			ref ExpansionPartyPlayerData playerData = players[i];
			if (!playerData) continue;
			
			PlayerBase partyPlayer = PlayerBase.GetPlayerByUID( playerData.UID );			
			if ( partyPlayer && partyPlayer.GetIdentity() )
			{					
				if ( partyPlayer.GetIdentityUID() != sender.GetId() )
				{
					GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), text, EXPANSION_NOTIFICATION_ICON_GROUP, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, partyPlayer.GetIdentity() );
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
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::HasParty - Start");
		#endif
		
		if ( !IsMissionClient() )
		{
			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint("ExpansionPartyModule::HasParty - return false");
			#endif
			
			return false;
		}
			
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::HasParty - return " + ( m_Party != NULL ).ToString());
		#endif
		
		return ( m_Party != NULL );
	}
	
	// -----------------------------------------------------------
	// Expansion HasParty
	// Called on server
	// -----------------------------------------------------------
	bool HasParty( PlayerBase player )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::HasParty - Start");
		#endif
		
		if ( IsMissionHost() )
		{				
			for( int i = 0; i < m_Parties.Count(); ++i )
			{
				ref ExpansionPartyData party = m_Parties.GetElement(i);
				if ( party && party.GetPlayer( player.GetIdentityUID() ) )
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
		
		if ( !IsMissionClient() || m_Party == NULL )
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
		
		if ( !IsMissionHost() || player.GetIdentityUID() == "" ) return -1;
		
		string id = player.GetIdentityUID();
		
		for (int i = 0; i < m_Parties.Count(); ++i)
		{
			ref ExpansionPartyData party = m_Parties.GetElement(i);
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
	ref ExpansionPartyData GetParty()
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::GetParty - Start");
		#endif
		
		if ( !IsMissionClient() )
		{
			#ifdef EXPANSION_PARTY_MODULE_DEBUG
			EXLogPrint("ExpansionPartyModule::GetParty - return m_Party: NULL");
			#endif
			
			return NULL;
		}
		
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::GetParty - return m_Party: " + m_Party.ToString());
		#endif
		
		return m_Party;
	}
	
	// ------------------------------------------------------------
	// Expansion GetPartyInvites
	// ------------------------------------------------------------
	ref array<ref ExpansionPartyInviteData> GetPartyInvites()
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::GetPartyInvites - Start");
		#endif
		
		if ( !IsMissionClient() )
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
	// Expansion SetPartyInvites
	// ------------------------------------------------------------
	void SetPartyInvites( ref array< ref ExpansionPartyInviteData > invites )
	{
		#ifdef EXPANSION_PARTY_MODULE_DEBUG
		EXLogPrint("ExpansionPartyModule::SetPartyInvites - Start");
		#endif
		
		if ( !IsMissionClient() )
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
	override void OnUpdate( float timeslice )
	{
		super.OnUpdate( timeslice );

		if ( !IsMissionHost() )
			return;
		
		m_UpdateQueueTimer += timeslice;
		if ( m_UpdateQueueTimer >= UPDATE_TICK_TIME )
		{
			//! TODO: Ultimately we should really redo this whole thing in MVC,
			//! and then only send the info that changed, not all of it.

			if ( m_Parties.Count() > 0 )
			{
				int updatedParties;
				int updatedPlayers;

				while ( updatedParties < m_Parties.Count() )
				{
					if ( m_CurrentPartyTick >= m_Parties.Count() )
					{
						m_CurrentPartyTick = 0;
					}
					
					ref ExpansionPartyData party = m_Parties.GetElement( m_CurrentPartyTick );
					if ( party.GetPlayers().Count() > 0 )
					{
						if ( m_CurrentPlayerTick >= party.GetPlayers().Count() )
						{
							m_CurrentPlayerTick = 0;
						}

						if ( m_CurrentPlayerTick == 0 )
						{
							//! This is correct even if we do not update all party members this cycle
							updatedParties++;
						}

						while ( updatedPlayers < UPDATE_PLAYERS_PER_TICK )
						{

							ref ExpansionPartyPlayerData playerData = party.GetPlayers()[m_CurrentPlayerTick];
							PlayerBase active_player = PlayerBase.GetPlayerByUID( playerData.UID );
							if ( active_player && active_player.GetIdentity() )
							{
								Send_UpdateClient( party, active_player );
								updatedPlayers++;
							}

							m_CurrentPlayerTick++;

							if ( m_CurrentPlayerTick == party.GetPlayers().Count() )
							{
								break;
							}
						}
					} else
					{
						updatedParties++;
					}
					
					if ( m_CurrentPlayerTick == party.GetPlayers().Count() )
					{
						m_CurrentPartyTick++;
						m_CurrentPlayerTick = 0;
					}

					if ( updatedPlayers == UPDATE_PLAYERS_PER_TICK )
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
	ref ExpansionPartyData GetPartyByID(int partyID)
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