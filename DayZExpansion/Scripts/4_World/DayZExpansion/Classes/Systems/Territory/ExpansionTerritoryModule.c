/**
 * ExpansionTerritoryModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionTerritoryModule: JMModuleBase
{
	///////////////////////// STATIC VARS /////////////////////////////////
	static const int 									m_TerritorySize_Level_1 = 50;
	static const int 									m_TerritorySize_Level_2 = 100;
	static const int 									m_TerritorySize_Level_3 = 150;
	static const int 									m_TerritorySize_MAX = 150;
	
	/////////////////////// NON STATIC VARS ///////////////////////////////
	
	//Server
	protected ref map<int, TerritoryFlag> 				m_TerritoryFlags;
	protected int 										m_NextTerritoryID;
	protected float										m_TimeSliceCheckPlayer;
	
	//Client
	protected ref map<int, ref ExpansionTerritory>		m_Territories;
	protected ref array<ref ExpansionTerritoryInvite> 	m_TerritoryInvites;
	
	protected ref array<ref ExpansionTerritory>			m_ServerTerritories;
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule Constructor
	// Gets called on server and client
	// ------------------------------------------------------------
	void ExpansionTerritoryModule()
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::ExpansionTerritoryModule - Start");
		#endif
			
		//Server
		m_TerritoryFlags = new map<int, TerritoryFlag>;
		m_NextTerritoryID = 0;
		m_TimeSliceCheckPlayer = 0;
		
		//Client	
		m_Territories = new map<int, ref ExpansionTerritory>;
		m_TerritoryInvites = new array<ref ExpansionTerritoryInvite>;
		m_ServerTerritories = new array<ref ExpansionTerritory>;
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::ExpansionTerritoryModule - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule Destructor
	// Gets called on server and client
	// ------------------------------------------------------------
	void ~ExpansionTerritoryModule()
	{
	}
	
	// ------------------------------------------------------------
	// Override OnClientRespawn
	// Called on server
	// ------------------------------------------------------------
	override void OnClientRespawn( PlayerBase player, PlayerIdentity identity )
	{
		OnInvokeConnect( player, identity );
	}
	
	// ------------------------------------------------------------
	// Override OnPlayerConnect
	// Called on server
	// ------------------------------------------------------------
	override void OnInvokeConnect( PlayerBase player, PlayerIdentity identity )
	{
		if ( !player )
			return;

		string uid = identity.GetId();

		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::OnPlayerConnect - Start uid : " + uid + " m_TerritoryFlags.Count() : " + m_TerritoryFlags.Count());
		#endif
		
		TIntArray toRemove = new TIntArray;
		
		for(int i = 0; i < m_TerritoryFlags.Count(); ++i)
		{
			TerritoryFlag currFlag = m_TerritoryFlags.GetElement(i);
			if (!currFlag)
			{
				toRemove.Insert(m_TerritoryFlags.GetKey(i));
				continue;
			}
			
			ExpansionTerritory territory = currFlag.GetTerritory();
			if ( !territory )
				continue;

			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionTerritoryModule::OnPlayerConnect Current territory : " + territory.GetTerritoryName());
			#endif

			if ( territory && territory.IsMember( uid ) )
			{
				#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
				EXLogPrint("ExpansionTerritoryModule::OnPlayerConnect Found territory : " + territory.GetTerritoryName());
				#endif

				UpdateClient( territory.GetTerritoryID() );
				UpdateClient( territory.GetTerritoryID(), player );
			}
		}
		
		for (int j = 0; j < toRemove.Count(); ++j)
		{
			m_TerritoryFlags.Remove( toRemove[j] );
		}
		
		//Sync invites
		SyncPlayersInvites( player );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::OnPlayerConnect - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override OnUpdate
	// ------------------------------------------------------------
	override void OnUpdate( float timeslice )
	{
		if (!IsMissionClient() || (!GetExpansionSettings().GetNotification() && !GetExpansionSettings().GetNotification().ShowTerritoryNotifications))
			return;
			
		m_TimeSliceCheckPlayer += timeslice;
		
		if ( m_TimeSliceCheckPlayer > 2.5 )
		{
			Exec_CheckPlayer();
			m_TimeSliceCheckPlayer = 0;
		}
	}
	
	///////////////////////// RPCS /////////////////////////////////
	
	// ------------------------------------------------------------
	// Override GetRPCMin
	// ------------------------------------------------------------
	override int GetRPCMin()
	{
		return ExpansionTerritoryModuleRPC.INVALID;
	}
	
	// ------------------------------------------------------------
	// Override GetRPCMax
	// ------------------------------------------------------------
	override int GetRPCMax()
	{
		return ExpansionTerritoryModuleRPC.COUNT;
	}
	
	// ------------------------------------------------------------
	// Override OnRPC
	// ------------------------------------------------------------
	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::OnRPC - Start");
		#endif
		
		switch ( rpc_type )
		{
		case ExpansionTerritoryModuleRPC.OpenFlagMenu:
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionTerritoryModule::OnRPC - RPC_OpenFlagMenu");
			#endif
			TerritoryFlag flag;
			if ( Class.CastTo( flag, target ) )
			{
				GetGame().GetUIManager().EnterScriptedMenu( flag.GetTerritoryMenuID(), GetGame().GetUIManager().GetMenu() );
			}
			break;
		case ExpansionTerritoryModuleRPC.CreateTerritory:
			RPC_CreateTerritory( ctx, sender, target );
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionTerritoryModule::OnRPC - RPC_CreateTerritory");
			#endif

			break;
		case ExpansionTerritoryModuleRPC.ChangeTexture:
			RPC_ChangeFlagTexture( ctx, sender, target );
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionTerritoryModule::OnRPC - RPC_ChangeFlagTexture");
			#endif

			break;
		case ExpansionTerritoryModuleRPC.DeleteTerritoryPlayer:
			RPC_DeleteTerritoryPlayer( ctx, sender, target );
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionTerritoryModule::OnRPC - RPC_DeleteTerritoryPlayer");
			#endif

			break;
		case ExpansionTerritoryModuleRPC.DeleteTerritoryAdmin:
			RPC_DeleteTerritoryAdmin( ctx, sender, target );
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionTerritoryModule::OnRPC - RPC_DeleteTerritoryAdmin");
			#endif

			break;
		case ExpansionTerritoryModuleRPC.UpdateClient:
			RPC_UpdateClient( ctx, sender, target );
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionTerritoryModule::OnRPC - RPC_UpdateClient");
			#endif

			break;
		case ExpansionTerritoryModuleRPC.AcceptInvite:
			RPC_AcceptInvite( ctx, sender, target );
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionTerritoryModule::OnRPC - RPC_AddPlayer");
			#endif

			break;
		case ExpansionTerritoryModuleRPC.SyncPlayersInvites:
			RPC_SyncPlayersInvites( ctx, sender, target );
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionTerritoryModule::OnRPC - RPC_SyncPlayersInvites");
			#endif

			break;
		case ExpansionTerritoryModuleRPC.RequestInvitePlayer:
			RPC_RequestInvitePlayer( ctx, sender, target );
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionTerritoryModule::OnRPC - RPC_RequestInvitePlayer");
			#endif

			break;
		case ExpansionTerritoryModuleRPC.PromoteMember:
			RPC_PromoteMember( ctx, sender, target );
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionTerritoryModule::OnRPC - RPC_PromoteMember");
			#endif

			break;
		case ExpansionTerritoryModuleRPC.DemoteMember:
			RPC_DemoteMember( ctx, sender, target );
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionTerritoryModule::OnRPC - RPC_DemoteMember");
			#endif

			break;
		case ExpansionTerritoryModuleRPC.KickMember:
			RPC_KickMember( ctx, sender, target );
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionTerritoryModule::OnRPC - RPC_KickMember");
			#endif

			break;
		case ExpansionTerritoryModuleRPC.Leave:
			RPC_Leave( ctx, sender, target );
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionTerritoryModule::OnRPC - RPC_Leave");
			#endif

			break;
		case ExpansionTerritoryModuleRPC.PlayerEnteredTerritory:
			RPC_PlayerEnteredTerritory( ctx, sender, target );
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionTerritoryModule::OnRPC - RPC_PlayerEnteredTerritory");
			#endif

			break;
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::OnRPC - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateClient
	// Called on server
	// ------------------------------------------------------------
	void UpdateClient( int territoryID )
	{
		if ( !IsMissionHost() )
			return;
		TerritoryFlag flag = m_TerritoryFlags.Get( territoryID );
		if ( !flag )
			return;
		
		ExpansionTerritory territory = flag.GetTerritory();
		if ( !territory )
			return;
		
		array<ref ExpansionTerritoryMember> members = territory.GetTerritoryMembers();
		for ( int i = 0; i < members.Count(); ++i )
		{
			PlayerBase player = PlayerBase.GetPlayerByUID( members[i].GetID() );
			if ( player )
			{
				Send_UpdateClient(territory.GetTerritoryID(), territory, player.GetIdentity());
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateClient
	// Called on server
	// ------------------------------------------------------------
	void UpdateClient( int territoryID, PlayerBase player )
	{
		if ( !IsMissionHost() || !player || !player.GetIdentity() )
			return;
		
		TerritoryFlag flag = m_TerritoryFlags.Get( territoryID );
		if ( !flag )
			return;
		
		ExpansionTerritory territory = flag.GetTerritory();
		if ( !territory )
			return;
		
		Send_UpdateClient(territory.GetTerritoryID(), territory, player.GetIdentity());
	}

	// ------------------------------------------------------------
	// Expansion Send_UpdateClient
	// Called on server
	// ------------------------------------------------------------
	private void Send_UpdateClient( int territoryID, ref ExpansionTerritory territory, notnull PlayerIdentity sendTo )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Send_UpdateClient 1 territory : " + territory);
		#endif
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( territoryID );
		rpc.Write( territory );
		rpc.Send( NULL, ExpansionTerritoryModuleRPC.UpdateClient, true, sendTo );
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_UpdateClient
	// Called on client
	// ------------------------------------------------------------
	private void RPC_UpdateClient( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( !IsMissionClient() )
			return;
		
		int territoryID;
		if ( !ctx.Read( territoryID ) )
			return;

		ExpansionTerritory territory;
		if ( !ctx.Read( territory ))
			return;

		Exec_UpdateClient( territoryID, territory );
	}
		
	// ------------------------------------------------------------
	// Expansion Exec_UpdateClient
	// Called on client
	// ------------------------------------------------------------
	private void Exec_UpdateClient( int territoryID, ref ExpansionTerritory territory )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_UpdateClient 1 : " + territoryID + " " + territory + " " + m_Territories.Count() );
		#endif

		PlayerBase player;
		if ( Class.CastTo( player, GetGame().GetPlayer() ) )
		{
			ExpansionTerritory oldTerritory = m_Territories.Get( territoryID );
			if ( oldTerritory )
			{
				m_Territories.Remove( territoryID );
				delete oldTerritory;
			}
			
			if ( territory )
			{
				#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
				EXLogPrint("ExpansionTerritoryModule::Exec_UpdateClient 2 : " + territory + " " + m_Territories.Count());
				#endif
				
				m_Territories.Insert( territoryID, territory );
				
				array<ref ExpansionTerritoryMember> members = territory.GetTerritoryMembers();
				for ( int i = 0; i < members.Count(); ++i )
				{
					#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
					EXLogPrint("ExpansionTerritoryModule::Exec_UpdateClient 3." + i + ": " + members[i] + " " + members[i].GetID() + " " + members[i].GetName() + " " + members[i].GetRank());
					#endif
				}
			}

			UIScriptedMenu menu;
			if ( Class.CastTo( menu, GetGame().GetUIManager().FindMenu( MENU_EXPANSION_BOOK_MENU ) ) )
			{
				menu.Refresh();
			}
		}
	}
	
	///////////////////////// CreateTerritory /////////////////////////////////
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule CreateTerritory
	// Called from Client
	// Initiate remote procedure call for the creation off a new territory
	// ------------------------------------------------------------
	void CreateTerritory( string territoryName, TerritoryFlag flag )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::CreateTerritory - Start");
		#endif
		
		if ( !IsMissionClient() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( territoryName );
		rpc.Write( flag );
		rpc.Send( NULL, ExpansionTerritoryModuleRPC.CreateTerritory, true, NULL );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::CreateTerritory - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule RPC_CreateTerritory
	// Called on server
	// ------------------------------------------------------------
	void RPC_CreateTerritory( ref ParamsReadContext ctx, PlayerIdentity senderRPC, ref Object target )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RPC_CreateTerritory - Start");
		#endif
		
		string territoryName;
		if ( !ctx.Read( territoryName ) )	//! Get Territory name
			return;
		TerritoryFlag flag;
		if ( !ctx.Read( flag ) )			//! Get Flag object
			return;

		Exec_CreateTerritory( territoryName, flag, senderRPC );	//! Called from server side
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RPC_CreateTerritory - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule Exec_CreateTerritory
	// Called on server
	// Called on server/mission host
	// ------------------------------------------------------------
	private void Exec_CreateTerritory( string territoryName, TerritoryFlag flag, PlayerIdentity sender )
	{	
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_CreateTerritory - Start territoryName : " + territoryName + " flag : " + flag);
		#endif
		
		if ( !IsMissionHost() || !sender || !flag )
			return;
		
		//! Get player object from network by sender identity
		PlayerBase player = PlayerBase.GetPlayerByUID( sender.GetId() );
		if ( !player )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_NOPLAYER" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		if ( GetExpansionSettings().GetTerritory().MaxTerritoryPerPlayer > 0 && GetNumberOfTerritory( sender.GetId() ) >= GetExpansionSettings().GetTerritory().MaxTerritoryPerPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_MAX_TERRITORY_PER_PLAYER", GetExpansionSettings().GetTerritory().MaxTerritoryPerPlayer.ToString() ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
			
		//! Check if the territory name is not already used
		for ( int i = 0; i < m_TerritoryFlags.Count(); ++i )
		{
			ExpansionTerritory territory = m_TerritoryFlags.GetElement(i).GetTerritory();
			if ( territory.GetTerritoryName() == territoryName )
			{
				GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_NAMEEXISTS", territoryName ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
				return;
			}
		}
		
		bool territoryCreated = false;
		bool flagCreated = false;
		
		//! Get the needed data for territory creation
		vector position = flag.GetPosition();
		string texturePath = flag.GetFlagTexturePath();
		string senderID = sender.GetId();
		
		//! Set needed flag data
		flag.SetIsExpansionTerritoryFlag( true );
		flag.SetTerritoryID( m_NextTerritoryID );
		flag.SetFlagTexture( texturePath );
		#ifndef EXPANSION_TERRITORY_DEV_DEBUG
		flag.SetOwnerID( senderID );
		#endif
		
		//! Create new territory
		ExpansionTerritory newTerritory = new ExpansionTerritory( m_NextTerritoryID, territoryName, 1, senderID, position, texturePath );
		if ( !newTerritory )
			return;
		
		#ifndef EXPANSION_TERRITORY_DEV_DEBUG
		newTerritory.AddMember( senderID, sender.GetName(), true );
		#endif
		
		flag.SetTerritory( newTerritory );
		
		m_TerritoryFlags.Insert( m_NextTerritoryID, flag );
		
		UpdateClient( m_NextTerritoryID );
		
		m_NextTerritoryID++;
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_CREATED", territoryName ), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 5, sender );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_CreateTerritory - End");
		#endif
	}
	
	///////////////////////// ChangeFlagTexture /////////////////////////////////
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule ChangeFlagTexture
	// Called on client
	// ------------------------------------------------------------
	void ChangeFlagTexture( string texturePath, TerritoryFlag flag )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::ChangeFlagTexture - Start");
		#endif
		
		if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( texturePath );
			rpc.Write( flag );
			rpc.Send( NULL, ExpansionTerritoryModuleRPC.ChangeTexture, true, NULL );
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::ChangeFlagTexture - End");
		#endif
	}

	// ------------------------------------------------------------
	// ExpansionTerritoryModule RPC_ChangeFlagTexture
	// Called on server
	// ------------------------------------------------------------
	void RPC_ChangeFlagTexture( ref ParamsReadContext ctx, PlayerIdentity senderRPC, ref Object target )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RPC_ChangeFlagTexture - Start");
		#endif
		
		string texturePath;
		if ( !ctx.Read( texturePath ) )
			return;
		
		TerritoryFlag flag;
		if ( !ctx.Read( flag ) )
			return;
		
		Exec_ChangeFlagTexture( texturePath, flag, senderRPC );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RPC_ChangeFlagTexture - End");
		#endif
	}

	// ------------------------------------------------------------
	// ExpansionTerritoryModule RPC_ChangeFlagTexture
	// Called on server
	// ------------------------------------------------------------
	private void Exec_ChangeFlagTexture( string texturePath, TerritoryFlag flag, PlayerIdentity ident )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_ChangeFlagTexture - Start");
		#endif

		if ( texturePath == "" || !flag )
			return;
		
		flag.SetFlagTexture( texturePath );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_ChangeFlagTexture - End");
		#endif
	}

	///////////////////////// DeleteTerritory /////////////////////////////////
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule DeleteTerritoryPlayer
	// Called on client
	// ------------------------------------------------------------
	void DeleteTerritoryPlayer( TerritoryFlag flag )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::DeleteTerritoryPlayer - Start");
		#endif
		
		if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( flag );
			rpc.Send( NULL, ExpansionTerritoryModuleRPC.DeleteTerritoryPlayer, true, NULL );
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::DeleteTerritoryPlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule RPC_DeleteTerritory
	// Called on server
	// ------------------------------------------------------------
	protected void RPC_DeleteTerritoryPlayer( ref ParamsReadContext ctx, PlayerIdentity senderRPC, ref Object target )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::DeleteTerritoryPlayer - Start");
		#endif
		
		TerritoryFlag flag;
	
		if ( !ctx.Read( flag ) )
			return;

		Exec_DeleteTerritoryPlayer( flag, senderRPC );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::DeleteTerritoryPlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule Exec_DeleteTerritory
	// Called on server
	// ------------------------------------------------------------
	protected void Exec_DeleteTerritoryPlayer( TerritoryFlag flag, PlayerIdentity sender )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_DeleteTerritoryPlayer - Start");
		#endif	
		
		if ( !flag || !sender )
		{
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionTerritoryModule::Exec_DeleteTerritoryPlayer - [ERROR] Recived territory flag object is empty or NULL!");
			#endif

			return;
		}
		
		string playerSteamID = "";
		if (sender) playerSteamID = sender.GetId();
		string flagOwnerID = flag.GetOwnerID();
		
		if ( playerSteamID == flagOwnerID )
		{
			ExpansionTerritory currentTerritory = flag.GetTerritory();
			if (!currentTerritory) return;
			
			int territoryID = currentTerritory.GetTerritoryID();
			
			ref array< ref ExpansionTerritoryMember > members = currentTerritory.GetTerritoryMembers();
			for ( int i = 0; i < members.Count(); ++i )
			{
				if (!members[i])
					continue;
				
				PlayerBase currPlayer = PlayerBase.GetPlayerByUID( members[i].GetID() );
				if (!currPlayer || !currPlayer.GetIdentity())
					continue;
				
				Send_UpdateClient( territoryID, null, currPlayer.GetIdentity() );
			}
			
			ref array< ref ExpansionTerritoryInvite > invites = currentTerritory.GetTerritoryInvites();
			for ( int j = 0; j < invites.Count(); ++j )
			{
				if (!invites[j])
					continue;
				
				PlayerBase currPlayerInvite = PlayerBase.GetPlayerByUID( invites[j].UID );
				if (!currPlayerInvite)
					continue;
				
				SyncPlayersInvites(currPlayerInvite);
			}
			
			//Don't forget to set it as null before to delete, to not do a infinte loop
			flag.SetTerritory(null);

			GetGame().CreateObject( "TerritoryFlagKit", flag.GetPosition() );
			flag.Delete();
			
			m_TerritoryFlags.Remove( territoryID );
			
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_DELETED", currentTerritory.GetTerritoryName() ), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE, 5, sender );
		} 
		else
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_DELETE_NORIGHTS", currentTerritory.GetTerritoryName() ), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE, 5, sender );
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_DeleteTerritoryPlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule DeleteTerritoryPlayer
	// Called on client
	// ------------------------------------------------------------
	void DeleteTerritoryAdmin( int territoryID )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::DeleteTerritoryPlayer - Start");
		#endif
		
		if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( territoryID );
			rpc.Send( null, ExpansionTerritoryModuleRPC.DeleteTerritoryAdmin, true );
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::DeleteTerritoryPlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule RPC_DeleteTerritory
	// Called on server
	// ------------------------------------------------------------
	protected void RPC_DeleteTerritoryAdmin( ref ParamsReadContext ctx, PlayerIdentity senderRPC, ref Object target )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::DeleteTerritoryPlayer - Start");
		#endif
		
		int territoryID;
		if ( !ctx.Read( territoryID ) )
			return;

		Exec_DeleteTerritoryAdmin( territoryID, senderRPC );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::DeleteTerritoryPlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule Exec_DeleteTerritory
	// Called on server
	// ------------------------------------------------------------
	void Exec_DeleteTerritoryAdmin( int territoryID, PlayerIdentity sender )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_DeleteTerritoryAdmin - Start");
		#endif	
		
		TerritoryFlag flag = m_TerritoryFlags.Get( territoryID );
		
		if ( !flag )
		{
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionTerritoryModule::Exec_DeleteTerritoryAdmin - [ERROR] Recived territory flag object is empty or NULL!");
			#endif

			return;
		}
		
		ExpansionTerritory currentTerritory = flag.GetTerritory();
		if ( !currentTerritory )
			return;
		
		ref array< ref ExpansionTerritoryMember > members = currentTerritory.GetTerritoryMembers();
		for ( int i = 0; i < members.Count(); ++i )
		{
			if (!members[i])
				continue;
			
			PlayerBase currPlayer = PlayerBase.GetPlayerByUID( members[i].GetID() );
			if (!currPlayer || !currPlayer.GetIdentity())
				continue;
			
			Send_UpdateClient( territoryID, null, currPlayer.GetIdentity() );
		}
		
		ref array< ref ExpansionTerritoryInvite > invites = currentTerritory.GetTerritoryInvites();
		for ( int j = 0; j < invites.Count(); ++j )
		{
			if (!invites[j])
				continue;
			
			PlayerBase currPlayerInvite = PlayerBase.GetPlayerByUID( invites[j].UID );
			if (!currPlayerInvite)
				continue;
			
			SyncPlayersInvites(currPlayerInvite);
		}
		
		//Don't forget to set it as null before to delete, to not do a infinte loop
		flag.SetTerritory( null );
		if (!flag.ToDelete())
			flag.Delete();
		
		m_TerritoryFlags.Remove( territoryID );
		
		if (sender)
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_DELETED", currentTerritory.GetTerritoryName() ), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE, 5, sender );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_DeleteTerritoryAdmin - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionPartyModule SyncPlayersInvites
	// Called on server
	// ------------------------------------------------------------
	void SyncPlayersInvites( PlayerBase sender )
	{
		if ( !IsMissionHost() || !sender || !sender.GetIdentity() )
			return;
		
		string id = sender.GetIdentityUID();
		array< ref ExpansionTerritoryInvite > invites = new array< ref ExpansionTerritoryInvite >;
		
		for (int i = 0; i < m_TerritoryFlags.Count(); ++i)
		{
			ref TerritoryFlag flag = m_TerritoryFlags.GetElement(i);
			if ( !flag )
				continue;
			
			ExpansionTerritory territory = flag.GetTerritory();
			if ( !territory )
				continue;
			
			ExpansionTerritoryInvite invite = territory.GetInvite(id);
			if ( !invite )
				continue;
			
			invites.Insert(invite);
		}
		
		ScriptRPC rpcServer = new ScriptRPC();
		rpcServer.Write( invites );
		rpcServer.Send( NULL, ExpansionTerritoryModuleRPC.SyncPlayersInvites, true, sender.GetIdentity() );
	}
	
	// ------------------------------------------------------------
	// ExpansionPartyModule RPC_SyncPlayersInvites
	// Called on client
	// ------------------------------------------------------------
	private void RPC_SyncPlayersInvites( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( !player )
			return;
		
		array<ref ExpansionTerritoryInvite> invites;
		if ( !ctx.Read( invites ) )
			return;

		SetTerritoryInvites( invites );
		
		UIScriptedMenu menu;
		if ( Class.CastTo( menu, GetGame().GetUIManager().FindMenu( MENU_EXPANSION_BOOK_MENU ) ) )
		{
			menu.Refresh();
		}
	}
		
	// ------------------------------------------------------------
	// ExpansionTerritoryModule RequestInvitePlayer
	// Called on client
	// ------------------------------------------------------------
	void RequestInvitePlayer( string targetID, TerritoryFlag flag )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RequestInvitePlayer - Start");
		#endif
		
		if ( !IsMissionClient() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( targetID );
		rpc.Write( flag );
		rpc.Send( NULL, ExpansionTerritoryModuleRPC.RequestInvitePlayer, true, NULL );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RequestInvitePlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule RPC_RequestInvitePlayer
	// Called on server
	// ------------------------------------------------------------
	private void RPC_RequestInvitePlayer( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RPC_RequestInvitePlayer - Start");
		#endif

		if ( !IsMissionHost() )
			return;
		
		string targetID;
		if ( !ctx.Read( targetID ) )
			return;
		TerritoryFlag flag;
		if ( !ctx.Read( flag ) )
			return;
		
		Exec_RequestInvitePlayer( targetID, flag, senderRPC );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RPC_RequestInvitePlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule Exec_RequestInvitePlayer
	// Called on server
	// ------------------------------------------------------------
	private void Exec_RequestInvitePlayer( string targetID, TerritoryFlag flag, notnull PlayerIdentity sender )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_RequestInvitePlayer - 1");
		#endif
		
		if ( !IsMissionHost() )
			return;
		
		PlayerBase targetPlayer = PlayerBase.GetPlayerByUID( targetID );
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_RequestInvitePlayer - 2 targetID : " + targetID + " targetPlayer: " + targetPlayer);
		#endif

		if ( !targetPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_NOPLAYER" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		if ( !flag )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_NOFLAG" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		ExpansionTerritory territory = flag.GetTerritory();
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_RequestInvitePlayer - 4 territory: " + territory);
		#endif

		if ( !territory )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_NOTERRITORY" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_RequestInvitePlayer - 5");
		#endif

		if ( territory.IsMember( targetID ) || territory.HasInvite(targetID) )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ALREADY_MEMBER", targetPlayer.GetName() ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		if ( GetExpansionSettings().GetTerritory() && GetExpansionSettings().GetTerritory().MaxMembersInTerritory > 1 && territory.NumberOfMembers() >= GetExpansionSettings().GetTerritory().MaxMembersInTerritory )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_MAX_TERRITORY", GetExpansionSettings().GetTerritory().MaxMembersInTerritory.ToString() ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_RequestInvitePlayer - 6");
		#endif

		ExpansionTerritoryInvite invite = new ExpansionTerritoryInvite;
		invite.TerritoryName = territory.GetTerritoryName();
		invite.TerritoryID = territory.GetTerritoryID();
		invite.UID = targetPlayer.GetIdentityUID();
		
		territory.AddTerritoryInvite(invite);
		
		SyncPlayersInvites( targetPlayer );
		
		//! Message to request sender
		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_PLAYER_INVITE_REQUEST_SENDER", targetPlayer.GetName() ), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE, 5, sender );
		
		//! Message to request target
		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_PLAYER_INVITE_REQUEST_TARGET", sender.GetName(), territory.GetTerritoryName() ), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE, 5, targetPlayer.GetIdentity() );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_RequestInvitePlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule AcceptInvite
	// Called on client
	// ------------------------------------------------------------
	void AcceptInvite( int territoryID )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::AddPlayer - Start");
		#endif
		
		if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( territoryID );
			rpc.Send( NULL, ExpansionTerritoryModuleRPC.AcceptInvite, true, NULL );
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::AddPlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule RPC_AddPlayer
	// Called on server
	// ------------------------------------------------------------
	private void RPC_AcceptInvite( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RPC_AddPlayer - Start");
		#endif
		
		int territoryID;
		if ( !ctx.Read( territoryID ) )
			return;
		
		Exec_AcceptInvite( territoryID, senderRPC );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RPC_AddPlayer - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule Exec_AcceptInvite
	// Called on server
	// ------------------------------------------------------------
	private void Exec_AcceptInvite( int territoryID, notnull PlayerIdentity sender )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_AcceptInvite - 1 territoryID: " + territoryID);
		#endif
		
		TerritoryFlag flag = m_TerritoryFlags.Get( territoryID );
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_AcceptInvite - 2 flag: " + flag);
		#endif

		if ( !flag )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_NOFLAG" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		ExpansionTerritory territory = flag.GetTerritory();
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_AcceptInvite - 3 territory: " + territory);
		#endif

		if ( !territory )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_NOTERRITORY" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID( sender.GetId() );
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_AcceptInvite - 4 senderPlayer: " + senderPlayer);
		#endif

		if ( !senderPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_NOPLAYER" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		if ( GetExpansionSettings().GetTerritory().MaxTerritoryPerPlayer > 0 && GetNumberOfTerritory( sender.GetId() ) >= GetExpansionSettings().GetTerritory().MaxTerritoryPerPlayer )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_MAX_TERRITORY_PER_PLAYER", GetExpansionSettings().GetTerritory().MaxTerritoryPerPlayer.ToString() ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		territory.RemoveTerritoryInvite( sender.GetId() );
		territory.AddMember( sender.GetId(), sender.GetName() );
		
		SyncPlayersInvites( senderPlayer );
		UpdateClient( territoryID );
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_PLAYER_ADDED", territory.GetTerritoryName() ), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE, 5, sender );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_AcceptInvite - 5");
		#endif
	}
	
	///////////////////////// PromoteMember /////////////////////////////////
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule PromoteMember
	// Called on client
	// ------------------------------------------------------------
	void PromoteMember( int territoryID, ExpansionTerritoryMember member )
	{
		if ( !IsMissionClient() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( territoryID );
		rpc.Write( member );
		rpc.Send( NULL, ExpansionTerritoryModuleRPC.PromoteMember, true, NULL );
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule RPC_PromoteMember
	// Called on server
	// ------------------------------------------------------------
	private void RPC_PromoteMember( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RPC_PromoteMember - Start");
		#endif

		if ( !IsMissionHost() )
			return;
		
		int territoryID
		if ( !ctx.Read( territoryID ) )
			return;
		
		ExpansionTerritoryMember member;
		if ( !ctx.Read( member ) )
			return;
		
		Exec_PromoteMember( territoryID, member, senderRPC );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RPC_PromoteMember - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule Exec_PromoteMember
	// Called on server
	// ------------------------------------------------------------
	private void Exec_PromoteMember( int territoryID, ref ExpansionTerritoryMember member, PlayerIdentity sender )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_PromoteMember - 1 territoryID : " + territoryID + " member : " + member);
		#endif
		
		if ( !member )
			return;
		
		TerritoryFlag flag = m_TerritoryFlags.Get( territoryID );
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_PromoteMember - 2 flag : " + flag);
		#endif

		if ( !flag )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_NOFLAG" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		ExpansionTerritory territory = flag.GetTerritory();
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_PromoteMember - 3 territory : " + territory);
		#endif

		if ( !territory )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_NOTERRITORY" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		ExpansionTerritoryMember senderTerritory = territory.GetMember( sender.GetId() );
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_PromoteMember - 4 senderTerritory : " + senderTerritory);
		#endif

		if ( !senderTerritory )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_NOSENDERTERRITORY" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_PromoteMember - 5");
		#endif
		
		if ( senderTerritory.GetRank() != ExpansionTerritoryRank.ADMIN )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_PROMOTE_NORIGHTS" ), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE, 5, sender );
			return;
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_PromoteMember - 6");
		#endif
		
		ref ExpansionTerritoryMember target = territory.GetMember( member.GetID() );
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_PromoteMember - 7 target : " + target);
		#endif

		if ( !target )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_NORECIVEMEMBER" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_PromoteMember - 8");
		#endif

		if ( target.GetRank() == ExpansionTerritoryRank.MEMBER )
		{
			target.SetRank( ExpansionTerritoryRank.MODERATOR );
			
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionTerritoryModule::Exec_PromoteMember - 9");
			#endif
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_PromoteMember - 10");
		#endif
		
		UpdateClient( territoryID );
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_PLAYER_PROMOTE", target.GetName(), target.GetRankName() ), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE, 5, sender );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_PromoteMember - End");
		#endif
	}
	
	///////////////////////// DemoteMember /////////////////////////////////
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule DemoteMember
	// Called on client
	// ------------------------------------------------------------
	void DemoteMember( int territoryID, ExpansionTerritoryMember member )
	{
		if ( !IsMissionClient() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( territoryID );
		rpc.Write( member );
		rpc.Send( NULL, ExpansionTerritoryModuleRPC.DemoteMember, true, NULL );
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule RPC_DemoteMember
	// Called on server
	// ------------------------------------------------------------
	private void RPC_DemoteMember( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RPC_DemoteMember - Start");
		#endif
		
		int territoryID
		if ( !ctx.Read( territoryID ) )
			return;
		
		ExpansionTerritoryMember member;
		if ( !ctx.Read( member ) )
			return;
		
		Exec_DemoteMember( territoryID, member, senderRPC );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RPC_DemoteMember - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule Exec_DemoteMember
	// Called on server
	// ------------------------------------------------------------
	private void Exec_DemoteMember( int territoryID, ExpansionTerritoryMember member, notnull PlayerIdentity sender )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_DemoteMember - Start");
		#endif
		
		if ( !member )
			return;

		TerritoryFlag flag = m_TerritoryFlags.Get( territoryID );
		if ( !flag )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_NOFLAG" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		ExpansionTerritory territory = flag.GetTerritory();
		if ( !territory )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_NOTERRITORY" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		ExpansionTerritoryMember senderTerritory = territory.GetMember( sender.GetId() );
		if ( !senderTerritory )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_NOSENDERTERRITORY" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		ExpansionTerritoryMember target = territory.GetMember( member.GetID() );
		if ( !target )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_NORECIVEMEMBER" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		if ( senderTerritory.GetRank() == ExpansionTerritoryRank.MEMBER )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_PLAYER_DEMOTE_ALREADYMEMBER", target.GetName() ), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE, 5, sender );
			return;
		}
		
		if ( target.GetRank() == ExpansionTerritoryRank.MODERATOR )
		{
			target.SetRank( ExpansionTerritoryRank.MEMBER );
		}
		
		UpdateClient( territoryID );
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_PLAYER_DEMOTE", target.GetName() ), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE, 5, sender );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_DemoteMember - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule KickMember
	// Called on client
	// ------------------------------------------------------------
	void KickMember( int territoryID, ExpansionTerritoryMember member )
	{
		if ( !IsMissionClient() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( territoryID );
		rpc.Write( member );
		rpc.Send( NULL, ExpansionTerritoryModuleRPC.KickMember, true, NULL );
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule RPC_KickMember
	// Called on server
	// ------------------------------------------------------------
	private void RPC_KickMember( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RPC_KickMember - Start");
		#endif
		
		int territoryID
		if ( !ctx.Read( territoryID ) )
			return;
		
		ExpansionTerritoryMember member;
		if ( !ctx.Read( member ) )
			return;
		
		Exec_KickMember( territoryID, member, senderRPC );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RPC_KickMember - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule Exec_KickMember
	// Called on server
	// ------------------------------------------------------------
	private void Exec_KickMember( int territoryID, ExpansionTerritoryMember member, notnull PlayerIdentity sender )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_KickMember - Start");
		#endif
		
		if ( !member )
			return;
		
		TerritoryFlag flag = m_TerritoryFlags.Get( territoryID );
		if ( !flag )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_NOFLAG" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		ExpansionTerritory territory = flag.GetTerritory();
		if ( !territory )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_NOTERRITORY" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		ExpansionTerritoryMember senderTerritory = territory.GetMember( sender.GetId() );
		if ( !senderTerritory )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_NOSENDERTERRITORY" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		ExpansionTerritoryMember target = territory.GetMember( member.GetID() );
		if ( !target )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_NORECIVEMEMBER" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		if ( senderTerritory.GetRank() != ExpansionTerritoryRank.ADMIN && senderTerritory.GetRank() != ExpansionTerritoryRank.MODERATOR )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_PLAYER_KICK_NORIGHTS", target.GetName() ), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE, 5, sender );
			return;
		}
		
		if ( target.GetRank() == ExpansionTerritoryRank.ADMIN )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_CANT_KICK_SELF", target.GetName() ), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE, 5, sender );
			return;
		}
		
		if ( target.GetRank() == ExpansionTerritoryRank.MODERATOR && senderTerritory.GetRank() == ExpansionTerritoryRank.MODERATOR )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_CANT_KICK_MODERATOR", target.GetName() ), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE, 5, sender );
			return;
		}
		
		territory.RemoveMember( target );
		
		PlayerBase playerTarget = PlayerBase.GetPlayerByUID( target.GetID() );
		if (playerTarget)
		{
			Send_UpdateClient( territoryID, NULL, playerTarget.GetIdentity() );
			
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_PLAYER_KICKED_TARGET", territory.GetTerritoryName(), sender.GetName() ), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE, 5, playerTarget.GetIdentity());
		}
		
		UpdateClient( territoryID );
		
		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_PLAYER_KICKED_SENDER", target.GetName() ), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE, 5, sender );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_KickMember - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule Leave
	// Called on client
	// ------------------------------------------------------------
	void Leave( int territoryID )
	{
		if ( !IsMissionClient() )
			return;
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( territoryID );
		rpc.Send( NULL, ExpansionTerritoryModuleRPC.Leave, true, NULL );
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule RPC_Leave
	// Called on server
	// ------------------------------------------------------------
	private void RPC_Leave( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RPC_Leave - Start");
		#endif
		
		int territoryID
		if ( !ctx.Read( territoryID ) )
			return;
		
		Exec_Leave( territoryID, senderRPC );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RPC_Leave - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule Exec_Leave
	// Called on server
	// ------------------------------------------------------------
	private void Exec_Leave( int territoryID, notnull PlayerIdentity sender )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_Leave - Start");
		#endif
		
		TerritoryFlag flag = m_TerritoryFlags.Get( territoryID );
		if ( !flag )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_NOFLAG" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		ExpansionTerritory territory = flag.GetTerritory();
		if ( !territory )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_NOTERRITORY" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		ExpansionTerritoryMember senderTerritory = territory.GetMember( sender.GetId() );
		if ( !senderTerritory )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ERROR_NOSENDERTERRITORY" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		if ( senderTerritory.GetRank() == ExpansionTerritoryRank.ADMIN )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_PLAYER_LEAVE_OWNER" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 5, sender );
			return;
		}
		
		territory.RemoveMember( senderTerritory );
		
		Send_UpdateClient( territoryID, NULL, sender );
		
		UpdateClient( territoryID );
		
		// TODO: message
		GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_PLAYER_LEAVE", territory.GetTerritoryName() ), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE, 5, sender );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_Leave - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule RPC_PlayerEnteredTerritory
	// Called on server
	// ------------------------------------------------------------
	private void RPC_PlayerEnteredTerritory( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RPC_PlayerEnteredTerritory - Start");
		#endif
		
		int type;
		if ( !ctx.Read( type ) )
			return;
		
		int territoryID
		if ( !ctx.Read( territoryID ) )
			return;

		if ( !GetExpansionSettings().GetNotification() || !GetExpansionSettings().GetNotification().ShowTerritoryNotifications )
			return;
		
		TerritoryFlag flag = m_TerritoryFlags.Get( territoryID );
		if ( !flag )
			return;
		
		ExpansionTerritory territory = flag.GetTerritory();
		if ( !territory )
			return;

		if ( type == 0 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_ENTER_TERRITORY", territory.GetTerritoryName() ), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE, 5, senderRPC );	
			return;
		} else if ( type == 1 )
		{
			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_TERRITORY_TITLE" ), new StringLocaliser( "STR_EXPANSION_TERRITORY_LEFT_TERRITORY", territory.GetTerritoryName() ), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE, 5, senderRPC );
			return;
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RPC_PlayerEnteredTerritory - End");
		#endif
	}

	// ------------------------------------------------------------
	// ExpansionTerritoryModule Exec_CheckPlayer
	// Called on client
	// Check if player is inside territory, if so, send a notification
	// ------------------------------------------------------------
	private void Exec_CheckPlayer()
	{
		PlayerBase player;
		if ( !Class.CastTo( player, GetGame().GetPlayer() ) )
			return;

		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_CheckPlayer - Start");
		#endif
		
		TerritoryFlag flag = FindNearestTerritoryFlag( player );

		int type = -1;
		int territoryId;
		if ( flag )
		{
			territoryId = flag.GetTerritoryID();
			if ( territoryId != player.GetTerritoryIDInside() )
			{
				type = 0;
				player.SetTerritoryIDInside( territoryId );
			}
		}
		else
		{
			if ( player.GetTerritoryIDInside() != -1 )
				type = 1;

			territoryId = player.GetTerritoryIDInside();

			player.SetTerritoryIDInside( -1 );
		}

		if ( type > -1 )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( type );
			rpc.Write( territoryId );
 			rpc.Send( null, ExpansionTerritoryModuleRPC.PlayerEnteredTerritory, true );
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_CheckPlayer - End");
		#endif
	}
	
	///////////////////////// RPCS END /////////////////////////////////
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule GetTerritories
	// ------------------------------------------------------------
	ref map<int, ref ExpansionTerritory> GetTerritories()
	{
		return m_Territories;
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule GetTerritories
	// ------------------------------------------------------------
	ref ExpansionTerritory GetTerritory( int territoryID )
	{
		return m_Territories.Get( territoryID );
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule IsPlayerInsideTerritory
	// ------------------------------------------------------------
	bool IsPlayerInsideTerritory( notnull PlayerIdentity identity )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::IsPlayerInsideTerritory - Start");
		#endif
		
		for ( int i = 0; i < m_Territories.Count(); i++ )
		{
			ExpansionTerritory currentTerritory = ExpansionTerritory.Cast( m_Territories.Get(i) );
			if (!currentTerritory)
			{
				#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
				EXLogPrint("ExpansionTerritoryModule::IsPlayerInsideTerritory - [ERROR] ExpansionTerritory is NULL!");
				#endif
				continue;
			}
			
			array<ref ExpansionTerritoryMember> territoryMembers = currentTerritory.GetTerritoryMembers();
			
			for ( int j = 0; j < territoryMembers.Count(); j++ )
			{
				ExpansionTerritoryMember currentMember = ExpansionTerritoryMember.Cast( territoryMembers.Get(j) );
				
				if ( currentMember && currentMember.GetID() == identity.GetPlainId() )
				{
					#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
					EXLogPrint("ExpansionTerritoryModule::IsPlayerInsideTerritory - End and return true");
					#endif
					return true;
				}
			}
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::IsPlayerInsideTerritory - End and return false");
		#endif
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule GetNumberOfTerritory
	// ------------------------------------------------------------
	int GetNumberOfTerritory(string playerUID = "")
	{
		if (IsMissionClient())
		{
			return m_Territories.Count();
		}
		else
		{
			if (playerUID == "")
				return 0;
			
			int nmb = 0;
			
			for (int idx = 0; idx < m_TerritoryFlags.Count(); ++idx)
			{
				TerritoryFlag flag = m_TerritoryFlags.GetElement(idx);
				if (!flag)
					continue;
				
				ExpansionTerritory currTerritory = flag.GetTerritory();
				if (!currTerritory)
					continue;
				
				if ( currTerritory.IsMember(playerUID) )
				{
					nmb++;
				}
			}
			
			return nmb;
		}
		
		return 0;
	}
	
	// ------------------------------------------------------------
	// Expansion FindNearestTerritoryFlag
	// ------------------------------------------------------------
	TerritoryFlag FindNearestTerritoryFlag( PlayerBase player )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::FindNearestTerritoryFlag - Start");
		#endif
		
		if (!player)
		{
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionTerritoryModule::FindNearestTerritoryFlag - [ERROR] Player is NULL!");
			#endif
			return null;
		}
		
		vector position = player.GetPosition();
		array<Object> objects = new array<Object>;
		array<CargoBase> proxyCargos = new array<CargoBase> ;
		GetGame().GetObjectsAtPosition3D( position, GetExpansionSettings().GetTerritory().TerritorySize, objects, proxyCargos );
		
		if ( objects )
		{
			for ( int i = 0; i < objects.Count(); ++i )
			{
				TerritoryFlag flag = TerritoryFlag.Cast( objects.Get( i ) );
				if ( flag && flag.HasExpansionTerritoryInformation() )
				{
					#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
					EXLogPrint("ExpansionTerritoryModule::FindNearestTerritoryFlag - End and found flag: " + flag.ToString());
					#endif
					return flag;
				}
			}
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::FindNearestTerritoryFlag - End and return NULL");
		#endif
		
		return null;
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule AddTerritoryFlag
	// Called on server
	// Called from ExpansionTerritoryFlag initialisation when flag object is created.
	// In this way we fill the territory flag array (m_TerritoryFlags) automatically in this module.
	// ------------------------------------------------------------
	void AddTerritoryFlag( TerritoryFlag flag, int territoryID )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::AddTerritoryFlag - Start");
		#endif
		
		if ( !flag )
			return;
		
		m_TerritoryFlags.Insert( territoryID, flag );
		
		if ( m_NextTerritoryID <= territoryID )
		{
			m_NextTerritoryID = territoryID + 1;
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::AddTerritoryFlag - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule AddTerritoryFlag
	// Called on server
	// Called from ExpansionTerritoryFlag deconstructor when flag object is deleted.
	// In this way we remove the territory flag from the array (m_TerritoryFlags) automatically in this module.
	// ------------------------------------------------------------
	void RemoveTerritoryFlag( int territoryID )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RemoveTerritoryFlag - Start");
		#endif
		
		if ( territoryID <= -1 )
			return;
		
		m_TerritoryFlags.Remove( territoryID );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RemoveTerritoryFlag - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule GetServerTerritories
	// Only returns array on Client side
	// ------------------------------------------------------------
	ref array<ref ExpansionTerritory> GetServerTerritories()
	{
		return m_ServerTerritories;
	}
	
	// ------------------------------------------------------------
	// Expansion IsInsideOwnTerritory
	// Called only on client
	// Check if a posiition is in own territory
	// ------------------------------------------------------------
	bool IsInsideOwnTerritory( vector position = "-1 -1 -1", float territorySize = -1, string playerUID = "" )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::IsInsideOwnTerritory - Start");
		#endif
		
		if (playerUID != "" && IsMissionClient()) return false;
		
		if ( position == "-1 -1 -1" && IsMissionClient() )
		{
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			if ( !player )
			{
				#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
				EXLogPrint("ExpansionTerritoryModule::IsInsideOwnTerritory - [ERROR] Player is NULL! Return false.");
				#endif
				return false;
			}
			
			position = player.GetPosition();
		}

		if ( territorySize <= 0 )
		{
			territorySize = GetExpansionSettings().GetTerritory().TerritorySize;
		}
		
		if (IsMissionHost())
		{
			for (int idx = 0; idx < m_TerritoryFlags.Count(); ++idx)
			{
				TerritoryFlag flag = m_TerritoryFlags.GetElement(idx);
				if (!flag)
				{
					#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
					EXLogPrint("ExpansionTerritoryModule::IsInsideOwnTerritory - [ERROR] Flag is NULL!");
					#endif
					continue;
				}
				if ( flag.HasExpansionTerritoryInformation() )
				{
					ExpansionTerritory currTerritory = flag.GetTerritory();
					if (!currTerritory)
					{
						#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
						EXLogPrint("ExpansionTerritoryModule::IsInsideOwnTerritory - [ERROR] Territory is NULL!");
						#endif
						continue;
					}
					
					if ( vector.Distance( currTerritory.GetPosition(), position ) <= territorySize && currTerritory.IsMember(playerUID) )
					{
						#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
						EXLogPrint("ExpansionTerritoryModule::IsInsideOwnTerritory - [1] End and return true!");
						#endif
						return true;
					}
				}
			}
		}
		else
		{
			for ( int i = 0; i < m_Territories.Count(); ++i )
			{
				ref ExpansionTerritory territory = m_Territories.GetElement( i );
				if ( territory && vector.Distance( territory.GetPosition(), position ) <= territorySize )
				{
					#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
					EXLogPrint("ExpansionTerritoryModule::IsInsideOwnTerritory - [2] End and return true!");
					#endif
					return true;
				}
			}
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::IsInsideOwnTerritory - End and return false!");
		#endif

		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion IsInTerritory
	// Called server/client
	// Check if a position is in territory
	// ------------------------------------------------------------
	bool IsInTerritory( vector position, float territorySize = -1 )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::IsInTerritory - Start");
		#endif
		
		if ( territorySize <= 0 )
		{
			territorySize = GetExpansionSettings().GetTerritory().TerritorySize;
		}
		
		array<Object> objects = new array<Object>;
		array<CargoBase> proxyCargos = new array<CargoBase> ;
		GetGame().GetObjectsAtPosition3D( position, territorySize, objects, proxyCargos );
		
		for ( int i = 0; i < objects.Count(); ++i )
		{
			TerritoryFlag flag;
			if ( Class.CastTo( flag, objects[i] ) )
			{
				if ( flag.HasExpansionTerritoryInformation() )
				{
					#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
					EXLogPrint("ExpansionTerritoryModule::IsInTerritory - End and return true!");
					#endif
					return true;
				}
			}
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::IsInTerritory - End and return false!");
		#endif
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion SetTerritoryInvites
	// Called client side
	// ------------------------------------------------------------
	void SetTerritoryInvites(array<ref ExpansionTerritoryInvite> invites )
	{
		if ( !IsMissionClient() )
			return;
		
		m_TerritoryInvites = invites;
	}
	
	// ------------------------------------------------------------
	// Expansion GetTerritoryInvites
	// Called client side
	// ------------------------------------------------------------
	ref array<ref ExpansionTerritoryInvite> GetTerritoryInvites()
	{
		if ( !IsMissionClient() )
			return NULL;
		
		return m_TerritoryInvites;
	}
	
	// ------------------------------------------------------------
	// Expansion CloseMenus
	// Called client side
	// ------------------------------------------------------------
	void CloseMenus()
	{
		if ( GetGame().GetUIManager() && GetGame().GetUIManager().IsMenuOpen( MENU_EXPANSION_FLAG_MENU ) )
			GetGame().GetUIManager().FindMenu( MENU_EXPANSION_FLAG_MENU ).Close();
	}
	
	// ------------------------------------------------------------
	// Expansion GetAllTerritoryFlags
	// Called server side
	// ------------------------------------------------------------
	ref map<int, TerritoryFlag> GetAllTerritoryFlags()
	{
		return m_TerritoryFlags;
	}
}