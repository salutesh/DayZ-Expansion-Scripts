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
	//Server side
    private ref map< int, ref ExpansionPartySaveFormat > m_Parties;
	
	//Client side
	private ref ExpansionPartySaveFormat m_Party;
	private ref array<ref ExpansionPartyInvite> m_PartyInvites;
	
	int m_NextPartyID = 0;
	
	// ------------------------------------------------------------
	// Expansion ExpansionPartyModule Constructor
	// ------------------------------------------------------------
	void ExpansionPartyModule()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPartyModule::Constructor - Start");
		#endif
		
		//Server
		m_Parties = new map< int, ref ExpansionPartySaveFormat >;
		
		//Client
		m_Party = null;
		m_PartyInvites = new array<ref ExpansionPartyInvite>;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPartyModule::Constructor - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionPartyModule Destructor
	// ------------------------------------------------------------
	void ~ExpansionPartyModule()
	{
		delete m_Parties;
	}
	
	override bool IsEnabled()
	{
		return (super.IsEnabled() && GetExpansionSettings().GetParty().EnableParties);
	}
	
	// ------------------------------------------------------------
	// Override OnMissionStart
	// ------------------------------------------------------------
    override void OnMissionStart() 
    {
		#ifdef EXPANSIONEXPRINT
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
				
        array< string > group_files = FindFilesInLocation( EXPANSION_GROUPS_FOLDER );

        for ( int i = 0; i < group_files.Count(); i++ )
        {          
            string name = group_files[i];
			int pos = group_files[i].IndexOf( "." );
				
			if ( pos > -1 )
			{
				name = group_files[i].Substring( 0, pos );
			}

            ExpansionPartySaveFormat party;
            JsonFileLoader< ExpansionPartySaveFormat >.JsonLoadFile( EXPANSION_GROUPS_FOLDER + name + ".json", party );
			if ( party )
			{
				party.InitMaps();
				
				m_Parties.Insert( party.GetPartyID(), party );

				if ( m_NextPartyID <= party.GetPartyID() )
				{
					m_NextPartyID = party.GetPartyID() + 1;
				}
			}
        }

		#ifdef EXPANSIONEXPRINT
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
		case ExpansionPartyModuleRPC.UpdatePositionMarker:
			RPC_UpdatePositionMarker( ctx, sender, target );
			break;
		case ExpansionPartyModuleRPC.DeleteMarker:
			RPC_DeleteMarker( ctx, sender, target );
			break;
		case ExpansionPartyModuleRPC.PromotePlayer:
			RPC_PromotePlayer( ctx, sender, target );
			break;
		case ExpansionPartyModuleRPC.DemotePlayer:
			RPC_DemotePlayer( ctx, sender, target );
			break;
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
		case ExpansionPartyModuleRPC.SyncQuickMarkers:
			RPC_SyncQuickMarkers( ctx, sender, target );
			break;
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionPartyModule FindAndSyncPlayerInvites
	// Called on server
	// ------------------------------------------------------------
	private void FindAndSyncPlayerInvites( PlayerBase player, notnull PlayerIdentity sender )
	{	
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyModule::FindAndSyncPlayerInvites - Start");
		#endif
		
		if ( !player )
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "ExpansionPartyModule::FindAndSyncPlayerInvites - [ERROR] Player object is empty or NULL!" );
			#endif

			return;
		}
		
		SyncPlayersInvites( player );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyModule::FindAndSyncPlayerInvites - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// RequestSyncPlayersInvites
	// Called on server
	// ------------------------------------------------------------
	void SyncPlayersInvites( PlayerBase sender )
	{
		if ( !IsMissionHost() || !sender || !sender.GetIdentity() )
			return;
		
		array< ref ExpansionPartyInvite > invites = new array< ref ExpansionPartyInvite >;
		string id = sender.GetIdentityUID();
		
		for (int i = 0; i < m_Parties.Count(); ++i)
		{
			ref ExpansionPartySaveFormat party = m_Parties.GetElement(i);
			if (party)
			{
				ref ExpansionPartyInvite invite = party.GetPlayerInvite(id);
				if (invite) invites.Insert(invite);
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyModule::SyncPlayersInvites - Found " + invites.Count() + " invites");
		#endif
		
		ScriptRPC rpcServer = new ScriptRPC();
		rpcServer.Write( invites );
		rpcServer.Send( NULL, ExpansionPartyModuleRPC.SyncPlayersInvites, true, sender.GetIdentity() );
	}
	
	// ------------------------------------------------------------
	// ExpansionPartyModule RPC_SyncPlayersInvites
	// Called on client
	// ------------------------------------------------------------
	private void RPC_SyncPlayersInvites( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if (!IsMissionClient()) return;
		
		array<ref ExpansionPartyInvite> invites;
		if ( !ctx.Read( invites ) )
			return;
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyModule::RPC_SyncPlayersInvites invites : " + invites + " invites.Count() : " + invites.Count());
		#endif

		SetPartyInvites( invites );
		
		UIScriptedMenu menu;
		if ( Class.CastTo( menu, GetGame().GetUIManager().FindMenu( MENU_EXPANSION_BOOK_MENU ) ) )
		{
            menu.Refresh();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion Create
	// Called on client
	// ------------------------------------------------------------
	void Create( string partyName )
	{
		if ( !IsMissionClient() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( partyName );
		rpc.Send( NULL, ExpansionPartyModuleRPC.Create, true, NULL );
	}

	// ------------------------------------------------------------
	// Expansion RPC_Create
	// Called on server
	// ------------------------------------------------------------
	private void RPC_Create( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		string partyName;
		if ( !ctx.Read( partyName ) )
			return;

		Exec_Create( partyName, senderRPC );
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_Create
	// Called on server
	// ------------------------------------------------------------
	private void Exec_Create( string partyName, PlayerIdentity sender )
	{	
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
	
	    ExpansionPartySaveFormat newParty = new ExpansionPartySaveFormat( m_NextPartyID );
	    newParty.SetupExpansionPartySaveFormat( player, partyName );
	    newParty.Save();
	
	    m_Parties.Insert( m_NextPartyID, newParty );

		UpdateClient( m_NextPartyID++ );
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_SYSTEM_PARTY_CREATED" ), EXPANSION_NOTIFICATION_ICON_GROUP, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, sender );
	}

	// ------------------------------------------------------------
	// Expansion Leave
	// ------------------------------------------------------------
	void Leave()
	{
		if ( !IsMissionClient() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Send( NULL, ExpansionPartyModuleRPC.Leave, true, NULL );
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_Leave
	// ------------------------------------------------------------
	private void RPC_Leave( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		Exec_Leave( senderRPC );
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_Leave
	// ------------------------------------------------------------
	private void Exec_Leave( PlayerIdentity sender )
	{
		if ( !sender )
			return;
		
		PlayerBase player = PlayerBase.GetPlayerByUID(sender.GetId());
		if ( !player )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		int partyId = GetPartyID(player);

		ref ExpansionPartySaveFormat party = m_Parties.Get( partyId );
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
	}
	
	// ------------------------------------------------------------
	// Expansion Dissolve
	// Called on client
	// ------------------------------------------------------------
	void Dissolve()
	{
		if ( !IsMissionClient() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Send( NULL, ExpansionPartyModuleRPC.Dissolve, true );
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_Dissolve
	// Called on server
	// ------------------------------------------------------------
	private void RPC_Dissolve( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		Exec_Dissolve( senderRPC );
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_Dissolve
	// Called on server
	// ------------------------------------------------------------
	private void Exec_Dissolve( PlayerIdentity sender )
	{
		if ( !sender )
			return;
		
		PlayerBase player = PlayerBase.GetPlayerByUID(sender.GetId());
		if ( !player )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		int partyId = GetPartyID(player);

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyModule::Exec_Dissolve partyId : " + partyId);
		#endif

		ref ExpansionPartySaveFormat party = m_Parties.Get( partyId );
		if ( !party || partyId == -1 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyModule::Exec_Dissolve party : " + party);
		#endif

		if ( sender.GetId() != party.GetOwnerUID() )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_OWNER" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyModule::Exec_Dissolve sender.GetId() : " + sender.GetId());
		#endif

	    ref array< ref ExpansionPartySaveFormatPlayer > players = party.GetPlayers();	
	    for ( int i = 0; i < players.Count(); i++ )
	    {
			ref ExpansionPartySaveFormatPlayer currPlayer = players[i];
			if (!currPlayer) continue;
			
			PlayerBase partyPlayer = PlayerBase.GetPlayerByUID(currPlayer.UID);

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionPartyModule::Exec_Dissolve removed player : " + currPlayer.UID);
			#endif
							
	        if ( partyPlayer && partyPlayer.GetIdentity() )
	        {
				Send_UpdateClient( NULL, partyPlayer );
				
				GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_PARTY_DISSOLVED" ), EXPANSION_NOTIFICATION_ICON_GROUP, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, partyPlayer.GetIdentity() );
	        }
	    }
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyModule::Exec_Dissolve beforeDelete party : " + party);
		#endif
		
	    party.Delete();

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyModule::Exec_Dissolve afterDelete party : " + party);
		#endif

		m_Parties.Remove( partyId );
    }
	
	// ------------------------------------------------------------
	// Expansion InvitePlayer
	// Called on client
	// ------------------------------------------------------------
	void InvitePlayer( string sendID )
	{
		if ( !IsMissionClient() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( sendID );
		rpc.Send( NULL, ExpansionPartyModuleRPC.InvitePlayer, true, NULL );
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_InvitePlayer
	// Called on server
	// ------------------------------------------------------------
	private void RPC_InvitePlayer( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		string sendID;
		if ( !ctx.Read( sendID ) )
			return;

		Exec_InvitePlayer( sendID, senderRPC );
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_InvitePlayer
	// Called on server
	// ------------------------------------------------------------
	private void Exec_InvitePlayer( string targetID, notnull PlayerIdentity sender )
	{		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID(sender.GetId());
		
		PlayerBase targetPlayer = PlayerBase.GetPlayerByUID(targetID);
		
		if ( !senderPlayer || !targetPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		int partyId = GetPartyID(senderPlayer);
		ref ExpansionPartySaveFormat party = m_Parties.Get( partyId );
		if ( !party || partyId == -1 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
		
		ref ExpansionPartySaveFormatPlayer senderPlayerParty = party.GetPlayer( sender.GetId() );
		if ( !senderPlayerParty || !senderPlayerParty.Promoted )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_OWNER" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		if ( GetExpansionSettings().GetParty().MaxInParty >= 1 && party.GetPlayers().Count() >= GetExpansionSettings().GetParty().MaxInParty )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_MAX_PARTY", GetExpansionSettings().GetParty().MaxInParty.ToString() ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
		}
		
		if ( party.HasPlayerInvite( targetID ) )
		{
			SyncPlayersInvites( targetPlayer );

			//TODO: Change message
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_IN_PARTY" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
	
		ExpansionPartyInvite invite = party.AddInvite( targetID );
	    party.Save();
		
		UpdateClient( partyId );
		
		SyncPlayersInvites( targetPlayer );

		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_MEMBER_ADDED" ), EXPANSION_NOTIFICATION_ICON_GROUP, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, targetPlayer.GetIdentity() );
		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_MEMBER_ADDED_SENDER" ), EXPANSION_NOTIFICATION_ICON_GROUP, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, sender );
    }

	// ------------------------------------------------------------
	// Expansion DeclineInvite
	// Called on client
	// ------------------------------------------------------------
	void DeclineInvite( int partyID )
	{
		if ( !IsMissionClient() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( partyID );
		rpc.Send( NULL, ExpansionPartyModuleRPC.DeclineInvite, true, NULL );
	}
	
		
	// ------------------------------------------------------------
	// Expansion RPC_DeclineInvite
	// Called on server
	// ------------------------------------------------------------
	private void RPC_DeclineInvite( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{		
		int partyID;
		if ( !ctx.Read( partyID ) )
			return;

		Exec_DeclineInvite( partyID, senderRPC );
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_DeclineInvite
	// ------------------------------------------------------------
	private void Exec_DeclineInvite( int partyID, notnull PlayerIdentity sender )
	{
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID( sender.GetId() );
		if ( !senderPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
	
		ref ExpansionPartySaveFormat party = m_Parties.Get( partyID );
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
    }
	
	// ------------------------------------------------------------
	// Expansion AcceptInvite
	// Called on client
	// ------------------------------------------------------------
	void AcceptInvite( int partyID )
	{
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( partyID );
		rpc.Send( NULL, ExpansionPartyModuleRPC.AcceptInvite, true, NULL );
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_AcceptInvite
	// Called on server
	// ------------------------------------------------------------
	private void RPC_AcceptInvite( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( !IsMissionHost() )
			return;
		
		int partyID;
		if ( !ctx.Read( partyID ) )
			return;
		
		Exec_AcceptInvite( senderRPC, partyID );
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_AcceptInvite
	// Called on server
	// ------------------------------------------------------------
	private void Exec_AcceptInvite( notnull PlayerIdentity sender, int partyId )
	{
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID( sender.GetId() );
		if ( !senderPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		ref ExpansionPartySaveFormat party = m_Parties.Get( partyId );
		if ( !party )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		if ( GetExpansionSettings().GetParty().MaxInParty >= 1 && party.GetPlayers().Count() >= GetExpansionSettings().GetParty().MaxInParty )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_MAX_PARTY", GetExpansionSettings().GetParty().MaxInParty.ToString() ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
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
	}
	
	// ------------------------------------------------------------
	// Expansion PromotePlayer
	// Called on client
	// ------------------------------------------------------------
	void PromotePlayer( string uid )
	{
		if ( !IsMissionClient() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(uid);
		rpc.Send( NULL, ExpansionPartyModuleRPC.PromotePlayer, true, NULL );
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_PromotePlayer
	// Called on server
	// ------------------------------------------------------------
	private void RPC_PromotePlayer( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		string targetID;
		if ( !ctx.Read( targetID ) )
			return;

		Exec_PromotePlayer( targetID, senderRPC );
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_PromotePlayer
	// ------------------------------------------------------------
	private void Exec_PromotePlayer( string targetID, notnull PlayerIdentity sender )
	{
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID( sender.GetId() );
		if ( !senderPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		int partyId = GetPartyID(senderPlayer);
		ref ExpansionPartySaveFormat party = m_Parties.Get( partyId );
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
		
		ref ExpansionPartySaveFormatPlayer targetPlayer = party.GetPlayer( targetID );
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
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_MEMBER_PROMOTED", "#STR_EXPANSION_PARTY_RANK_MOD" ), EXPANSION_NOTIFICATION_ICON_GROUP, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, targetPlayerPB.GetIdentity() );
		}
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_MEMBER_PROMOTED_SENDER", targetPlayer.Name ), EXPANSION_NOTIFICATION_ICON_GROUP, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, sender );
    }
	
	// ------------------------------------------------------------
	// Expansion DemotePlayer
	// Called on client
	// ------------------------------------------------------------
	void DemotePlayer( string uid )
	{
		if ( !IsMissionClient() ) return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(uid);
		rpc.Send( NULL, ExpansionPartyModuleRPC.DemotePlayer, true, NULL );
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_PromotePlayer
	// Called on server
	// ------------------------------------------------------------
	private void RPC_DemotePlayer( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		string targetID;
		if ( !ctx.Read( targetID ) )
			return;

		Exec_DemotePlayer( targetID, senderRPC );
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_PromotePlayer
	// ------------------------------------------------------------
	private void Exec_DemotePlayer( string targetID, notnull PlayerIdentity sender )
	{
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID( sender.GetId() );
		if ( !senderPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		int partyId = GetPartyID(senderPlayer);
		ref ExpansionPartySaveFormat party = m_Parties.Get( partyId );
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
		
		ref ExpansionPartySaveFormatPlayer targetPlayer = party.GetPlayer( targetID );
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
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_MEMBER_DEMOTED", "#STR_EXPANSION_PARTY_RANK_MEMBER" ), EXPANSION_NOTIFICATION_ICON_GROUP, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, targetPlayerPB.GetIdentity() );
		}
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_MEMBER_DEMOTED_SENDER", targetPlayer.Name ), EXPANSION_NOTIFICATION_ICON_GROUP, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, sender );
    }
	
	// ------------------------------------------------------------
	// Expansion RemovePlayer
	// Called on client
	// ------------------------------------------------------------
	void RemovePlayer( string uid )
	{
		if (!IsMissionClient()) return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( uid );
		rpc.Send( NULL, ExpansionPartyModuleRPC.RemovePlayer, true, NULL );
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_RemovePlayer
	// Called on server
	// ------------------------------------------------------------
	private void RPC_RemovePlayer( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		string uid;
		if ( !ctx.Read( uid ) )
			return;

		Exec_RemovePlayer( uid, senderRPC );
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_RemovePlayer
	// Called on server
	// ------------------------------------------------------------
	private void Exec_RemovePlayer( string uid, notnull PlayerIdentity sender )
	{
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID( sender.GetId() );
		if ( !senderPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		int partyId = GetPartyID(senderPlayer);
		ref ExpansionPartySaveFormat party = m_Parties.Get( partyId );
		if ( !party || partyId == -1 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		ref ExpansionPartySaveFormatPlayer senderPlayerParty = party.GetPlayer( sender.GetId() );
		if ( !senderPlayerParty.Promoted )
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
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateClient
	// Called on server
	// ------------------------------------------------------------
	void UpdateClient( int partyId )
	{
		if ( !IsMissionHost() )
			return;

		ref ExpansionPartySaveFormat party = m_Parties.Get( partyId );
		if ( !party )
			return;

		ref array< ref ExpansionPartySaveFormatPlayer > players = party.GetPlayers();
		if ( !players )
			return;
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyModule::UpdateClient - Start for party.GetPartyName() : " + party.GetPartyName());
		#endif
		
		for ( int i = 0; i < players.Count(); i++ )
		{
			ref ExpansionPartySaveFormatPlayer playerData = players[i];
			if (!playerData) continue;
			
			PlayerBase player = PlayerBase.GetPlayerByUID(playerData.UID);

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionPartyModule::UpdateClient doing " + playerData.UID + " player:" + player);
			#endif

			if ( !player )
				continue;

			Send_UpdateClient( party, player );

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionPartyModule::UpdateClient finished " + playerData.UID + " player:" + player);
			#endif
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyModule::UpdateClient - End party.GetPartyName() : " + party.GetPartyName());
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateClient
	// Called on server, when someone connect, because PlayerBase.GetPlayerByUID can't be used in the same frame as the connection of the player
	// ------------------------------------------------------------
	void UpdateClient( int partyId, PlayerBase player )
	{
		if ( !IsMissionHost() || !player )
			return;

		ref ExpansionPartySaveFormat party = m_Parties.Get( partyId );
		if ( !party )
			return;
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyModule::UpdateClient - Start for party.GetPartyName() : " + party.GetPartyName() + " player : " + player);
		#endif

		Send_UpdateClient( party, player );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyModule::UpdateClient - Start end party.GetPartyName() : " + party.GetPartyName() + " player : " + player);
		#endif
	}

	// ------------------------------------------------------------
	// Expansion Send_UpdateClient
	// Called on server
	// ------------------------------------------------------------
	private void Send_UpdateClient( ref ExpansionPartySaveFormat party, notnull PlayerIdentity toSendToParty )
	{		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( party );
		rpc.Send( null, ExpansionPartyModuleRPC.UpdateClient, true, toSendToParty );
	}

	// ------------------------------------------------------------
	// Expansion Send_UpdateClient
	// Called on server
	// ------------------------------------------------------------
	private void Send_UpdateClient( ref ExpansionPartySaveFormat party, PlayerBase player )
	{
		if ( !player || !player.GetIdentity() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( party );
		rpc.Send( null, ExpansionPartyModuleRPC.UpdateClient, true, player.GetIdentity() );
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_UpdateClient
	// Called on client
	// ------------------------------------------------------------
	private void RPC_UpdateClient( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( !IsMissionClient() )
			return;
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyModule::RPC_UpdateClient - Start");
		#endif

		ExpansionPartySaveFormat party;
		if ( !ctx.Read( party ) )
		{
			Error("ExpansionPartyModule::RPC_UpdateClient can't read party");
			return;
		}
			
		Exec_UpdateClient( party );

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyModule::RPC_UpdateClient - End party : " + party);
		#endif
	}
		
	// ------------------------------------------------------------
	// Expansion Exec_UpdateClient
	// Called on client
	// ------------------------------------------------------------
	private void Exec_UpdateClient( ref ExpansionPartySaveFormat party )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyModule::Exec_UpdateClient - Start party : " + party + " " + party.GetPartyName());
		#endif

		PlayerBase player;
        if ( Class.CastTo( player, GetGame().GetPlayer() ) )
        {
			if (party) party.InitMaps();
			
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionPartyModule::Exec_UpdateClient before set m_Party");
			#endif
			
			if (m_Party)
				party.SetQuickMarkers( m_Party.GetQuickMarkers() );

			m_Party = party;

			UIScriptedMenu menu;
			if ( Class.CastTo( menu, GetGame().GetUIManager().FindMenu( MENU_EXPANSION_BOOK_MENU ) ) )
			{
				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint("ExpansionPartyModule::Exec_UpdateClient before refresh MENU_EXPANSION_BOOK_MENU");
				#endif

	            menu.Refresh();

				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint("ExpansionPartyModule::Exec_UpdateClient after refresh MENU_EXPANSION_BOOK_MENU");
				#endif
			}

			if ( Class.CastTo( menu, GetGame().GetUIManager().FindMenu( MENU_EXPANSION_MAP ) ) )
			{
				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint("ExpansionPartyModule::Exec_UpdateClient before refresh MENU_EXPANSION_MAP");
				#endif

	            menu.Refresh();

				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint("ExpansionPartyModule::Exec_UpdateClient after refresh MENU_EXPANSION_MAP");
				#endif
			}

			Expansion3DMarkerModule module;
			if ( Class.CastTo( module, GetModuleManager().GetModule( Expansion3DMarkerModule ) ) )
			{
				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint("ExpansionPartyModule::Exec_UpdateClient before refresh Expansion3DMarkerModule");
				#endif

				module.RefreshMarkers();

				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint("ExpansionPartyModule::Exec_UpdateClient after refresh Expansion3DMarkerModule");
				#endif
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyModule::Exec_UpdateClient - End");
		#endif
	}
		
	// ------------------------------------------------------------
	// Expansion CreateMarker
	// ------------------------------------------------------------
	void CreateMarker( ExpansionMapMarker marker )
	{
		if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( marker );
			rpc.Send( NULL, ExpansionPartyModuleRPC.CreateMarker, true, NULL );
		}
	}
		
	// -----------------------------------------------------------
	// Expansion Exec_CreateMarker
	// -----------------------------------------------------------
	private void Exec_CreateMarker( ExpansionMapMarker marker, PlayerIdentity sender )
	{
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID(sender.GetId());
		if ( !senderPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		int partyId = GetPartyID(senderPlayer);
		ref ExpansionPartySaveFormat party = m_Parties.Get( partyId );
		if ( !party || partyId == -1 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

	    party.AddMarker( marker );
	    party.Save();

		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_MARKER_ADDED", marker.GetMarkerText() ), EXPANSION_NOTIFICATION_ICON_MARKER, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, sender );

	    UpdateClient( partyId );
    }
		
	// -----------------------------------------------------------
	// Expansion RPC_CreateMarker
	// -----------------------------------------------------------
	private void RPC_CreateMarker( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		ExpansionMapMarker marker;
		if ( !ctx.Read( marker ) )
			return;

		Exec_CreateMarker( marker, senderRPC );
	}
		
	// -----------------------------------------------------------
	// Expansion DeleteMarker
	// -----------------------------------------------------------
	void DeleteMarker( string name )
	{
		if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( name );
			rpc.Send( NULL, ExpansionPartyModuleRPC.DeleteMarker, true, NULL );
		}
	}
	
	// -----------------------------------------------------------
	// Expansion RPC_DeleteMarker
	// -----------------------------------------------------------
	private void RPC_DeleteMarker( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		string name;
		if ( !ctx.Read( name ) )
			return;

		Exec_DeleteMarker( name, senderRPC );
	}
		
	// -----------------------------------------------------------
	// Expansion Exec_DeleteMarker
	// -----------------------------------------------------------
	private void Exec_DeleteMarker( string name, PlayerIdentity sender )
	{
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID(sender.GetId());
		if ( !senderPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		int partyId = GetPartyID(senderPlayer);
		ref ExpansionPartySaveFormat party = m_Parties.Get( partyId );
		if ( !party || partyId == -1 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

	    if ( party.RemoveMarker( name ) )
	    {
	        party.Save();

			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_MARKER_REMOVED" ), EXPANSION_NOTIFICATION_ICON_MARKER, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, sender );
	        
	    	UpdateClient( partyId );
	    } 
		else
	    {
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
	    }
    }
	
	// -----------------------------------------------------------
	// Expansion UpdatePositionMarker
	// Called on client
	// -----------------------------------------------------------
	void UpdatePositionMarker( string name, vector position )
	{
		if ( !IsMissionClient() ) return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( name );
		rpc.Write( position );
		rpc.Send( NULL, ExpansionPartyModuleRPC.UpdatePositionMarker, true, NULL );
	}
	
	// -----------------------------------------------------------
	// Expansion RPC_DeleteMarker
	// -----------------------------------------------------------
	private void RPC_UpdatePositionMarker( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		string name;
		if ( !ctx.Read( name ) )
			return;
		
		vector position;
		if ( !ctx.Read( position ) )
			return;

		Exec_UpdatePositionMarker( name, position, senderRPC );
	}
		
	// -----------------------------------------------------------
	// Expansion Exec_DeleteMarker
	// -----------------------------------------------------------
	private void Exec_UpdatePositionMarker( string name, vector position, PlayerIdentity sender )
	{
		if (!sender) return;
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID(sender.GetId());
		if ( !senderPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		int partyId = GetPartyID(senderPlayer);
		ref ExpansionPartySaveFormat party = m_Parties.Get( partyId );
		if ( !party || partyId == -1 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

	    if ( party.UpdateMarkerPosition( name, position ) )
	    {
	        party.Save();

			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_MARKER_CHANGED" ), EXPANSION_NOTIFICATION_ICON_MARKER, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, sender );
	    	UpdateClient( partyId );
			SendNotificationToMembers( new StringLocaliser( "STR_EXPANSION_PARTY_MARKER_CHANGED" ), party, sender );
	    } 
		else
	    {
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
	    }
    }
	
	// ------------------------------------------------------------
	// Expansion UpdateQuickMarker
	// Called on client
	// ------------------------------------------------------------
	void UpdateQuickMarker( vector pos )
	{
		if ( !IsMissionClient() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( pos );
		rpc.Send( null, ExpansionPartyModuleRPC.UpdateQuickMarker, true );
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_UpdateQuickMarker
	// Called on server
	// ------------------------------------------------------------
	private void RPC_UpdateQuickMarker( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		vector position;
		if ( !ctx.Read( position ) )
			return;
		
		Exec_UpdateQuickMarker( position, senderRPC );
	}
	
	// -----------------------------------------------------------
	// Expansion Exec_DeleteMarker
	// -----------------------------------------------------------
	private void Exec_UpdateQuickMarker( vector position, PlayerIdentity sender )
	{
		if (!sender || !GetExpansionSettings().GetParty() || !GetExpansionSettings().GetParty().EnableQuickMarker)
			return;
		
		string senderID = sender.GetId();
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID(senderID);
		if ( !senderPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_UNKNOWN" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}

		int partyId = GetPartyID(senderPlayer);
		ref ExpansionPartySaveFormat party = m_Parties.Get( partyId );
		if ( !party || partyId == -1 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_PARTY_NOTIF_TITLE" ), new StringLocaliser( "STR_EXPANSION_PARTY_ERROR_NOT_EXIST" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, sender );
			return;
		}
		
		if (position == vector.Zero)
		{
			if (party.RemoveQuickMarker(senderID))
				SyncQuickMarkers(party);
		}
		else
		{
			ExpansionQuickMarker quickMarker = new ExpansionQuickMarker;
			quickMarker.UID = senderID;
			quickMarker.Pos = position;
			quickMarker.Color = senderPlayer.GetQuickMarkerColor();
			
			party.UpdateQuickMarker( quickMarker );
			
			SyncQuickMarkers(party);
		}
	}
	
	// -----------------------------------------------------------
	// Expansion SyncQuickMarkers
	// -----------------------------------------------------------
	void SyncQuickMarkers(ref ExpansionPartySaveFormat party)
	{
		if ( !IsMissionHost() || !party)
			return;
		
		ref array< ref ExpansionPartySaveFormatPlayer > players = party.GetPlayers();
		if ( !players )
			return;
		
		ref array<ref ExpansionQuickMarker> quickMarkers = party.GetQuickMarkers();
		
		for ( int i = 0; i < players.Count(); i++ )
		{
			ref ExpansionPartySaveFormatPlayer playerData = players[i];
			if (!playerData) continue;
			
			PlayerBase player = PlayerBase.GetPlayerByUID(playerData.UID);
			if ( !player || !player.GetIdentity() )
				continue;

			ScriptRPC rpc = new ScriptRPC;
			rpc.Write(quickMarkers);
			rpc.Send(null, ExpansionPartyModuleRPC.SyncQuickMarkers, true, player.GetIdentity());
		}
	}
	
	// -----------------------------------------------------------
	// Expansion RPC_SyncQuickMarkers
	// -----------------------------------------------------------
	private void RPC_SyncQuickMarkers( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if (!IsMissionClient())
			return;
		
		array<ref ExpansionQuickMarker> quickMarkers;
		if ( !ctx.Read( quickMarkers ) )
			return;
		
		Exec_SyncQuickMarkers( quickMarkers );
	}
	
	// -----------------------------------------------------------
	// Expansion Exec_SyncQuickMarkers
	// -----------------------------------------------------------
	private void Exec_SyncQuickMarkers( ref array<ref ExpansionQuickMarker> quickMarkers )
	{
		if ( !IsMissionClient() || !m_Party || !quickMarkers)
			return;
		
		m_Party.SetQuickMarkers( quickMarkers );
		
		Expansion3DMarkerModule module;
		if ( Class.CastTo( module, GetModuleManager().GetModule( Expansion3DMarkerModule ) ) )
			module.RefreshMarkers();
	}

	// -----------------------------------------------------------
	// Expansion OnClientRespawn
	// -----------------------------------------------------------
	override void OnClientRespawn( PlayerBase player, PlayerIdentity identity )
	{
		OnInvokeConnect( player, identity );
	}
		
	// -----------------------------------------------------------
	// Expansion OnInvokeConnect
	// -----------------------------------------------------------
    override void OnInvokeConnect( PlayerBase player, PlayerIdentity identity )
    {
		if ( !identity || !player )
            return;
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionPartyModule::OnPlayerConnect - Start for " + identity.GetId() + " player : " + player );
		#endif
		
		FindAndSyncPlayerInvites( player, identity );

		for ( int i = 0; i < m_Parties.Count(); ++i )
		{
			ref ExpansionPartySaveFormat party = m_Parties.GetElement(i);
			if ( party )
			{
				ref ExpansionPartySaveFormatPlayer pFormat = party.GetPlayer( identity.GetId() );
				if ( pFormat )
				{
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("ExpansionPartyModule::OnPlayerConnect found a party : " + party.GetPartyName());
					#endif
					
					//Update all others clients
	    			UpdateClient( party.GetPartyID() );
					
					//Update our current client
					UpdateClient( party.GetPartyID(), player );
					
					SyncQuickMarkers(party);
				}
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionPartyModule::OnPlayerConnect - End");
		#endif
    }
	
	// -----------------------------------------------------------
	// Expansion OnInvokeDisconnect
	// -----------------------------------------------------------
	override void OnInvokeDisconnect( PlayerBase player )
	{
		if (!player)
			return;
		
		int partyId = GetPartyID(player);
		ref ExpansionPartySaveFormat party = m_Parties.Get( partyId );
		if ( !party || partyId == -1 )
		{
			return;
		}
		
		if (party.RemoveQuickMarker(player.GetIdentityUID()))
			SyncQuickMarkers(party);
	}
	
	// -----------------------------------------------------------
	// Expansion OnUpdate
	// -----------------------------------------------------------
	override void OnUpdate( float timeslice )
	{
		if (!IsMissionClient() || !m_Party)
			return;
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		if ( player && GetGame().GetInput() && GetGame().GetInput().LocalPress( "UAExpansionQuickMarker", false ) && GetExpansionSettings().GetParty() && GetExpansionSettings().GetParty().EnableQuickMarker )
		{
			vector pos = vector.Zero;
			
			vector rayStart = GetGame().GetCurrentCameraPosition();
			vector rayEnd = rayStart + GetGame().GetCurrentCameraDirection() * 10000;
			vector hitPos;
			vector hitNormal;
			int hitComponentIndex;
			
			if ( DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, null, null, null, false, true) )
			{
				pos = hitPos;
			}
			
			UpdateQuickMarker( pos );
		}
	}
	
	// -----------------------------------------------------------
	// Expansion SendNotificationToMembers
	// Send notification with given StringLocaliser data to all 
	// active members of the given party
	// -----------------------------------------------------------
	void SendNotificationToMembers( StringLocaliser text, ExpansionPartySaveFormat party, PlayerIdentity sender )
	{
		ref array< ref ExpansionPartySaveFormatPlayer > players = party.GetPlayers();	
	   	if ( !players ) return;
		
		for ( int i = 0; i < players.Count(); i++ )
	    {
			ref ExpansionPartySaveFormatPlayer playerData = players[i];
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
	}
	
	// -----------------------------------------------------------
	// Expansion HasParty
	// Called on client
	// -----------------------------------------------------------
	bool HasParty()
	{
		if ( !IsMissionClient() )
			return false;
		
		return ( m_Party != NULL );
	}
	
	// -----------------------------------------------------------
	// Expansion HasParty
	// Called on server
	// -----------------------------------------------------------
	bool HasParty( PlayerBase player )
	{
		if ( IsMissionHost() )
		{				
			for( int i = 0; i < m_Parties.Count(); ++i )
			{
				ref ExpansionPartySaveFormat party = m_Parties.GetElement(i);
				if ( party && party.GetPlayer( player.GetIdentityUID() ) )
				{
					return true;
				}
			}
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion GetPartyID
	// Called on client
	// ------------------------------------------------------------
	int GetPartyID()
	{
		if ( !IsMissionClient() || m_Party == NULL )
			return -1;
		
		return m_Party.GetPartyID();
	}
	
	// -----------------------------------------------------------
	// Expansion HasParty
	// Called on server
	// -----------------------------------------------------------
	int GetPartyID(PlayerBase player)
	{
		if (!IsMissionHost() || player.GetIdentityUID() == "") return -1;
		
		string id = player.GetIdentityUID();
		
		for (int i = 0; i < m_Parties.Count(); ++i)
		{
			ref ExpansionPartySaveFormat party = m_Parties.GetElement(i);
			if (party)
			{
				if (party.GetPlayer(id))
				{
					return party.GetPartyID();
				}
			}
		}
		
		return -1;
	}

	// ------------------------------------------------------------
	// Expansion GetParty
	// Called on client
	// ------------------------------------------------------------
	ref ExpansionPartySaveFormat GetParty()
	{
		if ( !IsMissionClient() )
			return NULL;
		
		return m_Party;
	}
	
	// ------------------------------------------------------------
	// Expansion GetPartyInvites
	// ------------------------------------------------------------
	ref array<ref ExpansionPartyInvite> GetPartyInvites()
	{
		if ( !IsMissionClient() )
			return NULL;
		
		return m_PartyInvites;
	}
	
	// ------------------------------------------------------------
	// Expansion SetPartyInvites
	// ------------------------------------------------------------
	void SetPartyInvites( ref array< ref ExpansionPartyInvite > invites )
	{
		if ( !IsMissionClient() )
			return;
		
		m_PartyInvites = invites;
	}
}