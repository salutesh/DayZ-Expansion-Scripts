/**
 * ExpansionTerritoryModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
	protected ref map<int, ref ExpansionTerritory>		m_Territories;  //! Contains only territories which a client is member of
	protected ref array<ref ExpansionTerritoryInvite> 	m_TerritoryInvites;
	
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
		
		foreach (int territoryID, TerritoryFlag currFlag: m_TerritoryFlags)
		{
			if (!currFlag)
			{
				toRemove.Insert(territoryID);
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
		SyncPlayerInvitesServer( player );
		
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
	#ifdef CF_BUGFIX_REF
	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	#else
	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	#endif
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
			EXLogPrint("ExpansionTerritoryModule::OnRPC - RPC_AcceptInvite");
			#endif
			
		case ExpansionTerritoryModuleRPC.DeclineInvite:
			RPC_DeclineInvite( ctx, sender, target );
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionTerritoryModule::OnRPC - RPC_DeclineInvite");
			#endif

			break;
		case ExpansionTerritoryModuleRPC.SyncPlayerInvites:
			RPC_SyncPlayerInvitesClient( ctx, sender, target );
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionTerritoryModule::OnRPC - RPC_SyncPlayerInvitesClient");
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
	private void Send_UpdateClient( int territoryID, ExpansionTerritory territory, notnull PlayerIdentity sendTo )
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
	private void RPC_UpdateClient( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
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
	private void Exec_UpdateClient( int territoryID, ExpansionTerritory territory )
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
				
				#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
				array<ref ExpansionTerritoryMember> members = territory.GetTerritoryMembers();
				for ( int i = 0; i < members.Count(); ++i )
				{
					EXLogPrint("ExpansionTerritoryModule::Exec_UpdateClient 3." + i + ": " + members[i] + " " + members[i].GetID() + " " + members[i].GetName() + " " + members[i].GetRank());
				}
				#endif
			}

			ExpansionUIScriptedMenu menu;
			if (Class.CastTo( menu, GetGame().GetUIManager().FindMenu(EXPANSION_MENU_BOOK)))
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
	void RPC_CreateTerritory( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
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
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NOPLAYER").Error(sender);
			return;
		}
		
		if ( GetExpansionSettings().GetTerritory().MaxTerritoryPerPlayer > 0 && GetNumberOfTerritory( sender.GetId() ) >= GetExpansionSettings().GetTerritory().MaxTerritoryPerPlayer )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_ERROR_MAX_TERRITORY_PER_PLAYER", GetExpansionSettings().GetTerritory().MaxTerritoryPerPlayer.ToString())).Error(sender);
			return;
		}
			
		//! Check if the territory name is not already used
		foreach ( int territoryID, TerritoryFlag currFlag: m_TerritoryFlags )
		{
			ExpansionTerritory territory = currFlag.GetTerritory();
			if ( territory.GetTerritoryName() == territoryName )
			{
				ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_NAMEEXISTS", territoryName)).Error(sender);
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

		if ( GetExpansionSettings().GetLog().Territory )
			GetExpansionSettings().GetLog().PrintLog( "[Territory] Player \"" + sender.GetName() + "\" (id=" + senderID + ")" + " created territory " + territoryName + " at " + position );
		
		ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_CREATED", territoryName), EXPANSION_NOTIFICATION_ICON_TERRITORY).Success(sender);
		
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
	void RPC_ChangeFlagTexture( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
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
	protected void RPC_DeleteTerritoryPlayer( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
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
			
			array< ref ExpansionTerritoryMember > members = currentTerritory.GetTerritoryMembers();
			for ( int i = 0; i < members.Count(); ++i )
			{
				if (!members[i])
					continue;
				
				PlayerBase currPlayer = PlayerBase.GetPlayerByUID( members[i].GetID() );
				if (!currPlayer || !currPlayer.GetIdentity())
					continue;
				
				Send_UpdateClient( territoryID, null, currPlayer.GetIdentity() );
			}
			
			array< ref ExpansionTerritoryInvite > invites = currentTerritory.GetTerritoryInvites();
			for ( int j = 0; j < invites.Count(); ++j )
			{
				if (!invites[j])
					continue;
				
				PlayerBase currPlayerInvite = PlayerBase.GetPlayerByUID( invites[j].UID );
				if (!currPlayerInvite)
					continue;
				
				SyncPlayerInvitesServer(currPlayerInvite);
			}
		
			if ( GetExpansionSettings().GetLog().Territory )
				GetExpansionSettings().GetLog().PrintLog( "[Territory] Player \"" + sender.GetName() + "\" (id=" + playerSteamID + ")" + " deleted territory " + currentTerritory.GetTerritoryName() + " at " + currentTerritory.GetPosition() );
			
			//Don't forget to set it as null before to delete, to not do a infinte loop
			flag.SetTerritory(null);

			GetGame().CreateObject( "TerritoryFlagKit", flag.GetPosition() );
			flag.Delete();
			
			m_TerritoryFlags.Remove( territoryID );
			
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_DELETED", currentTerritory.GetTerritoryName()), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE).Create(sender);
		} 
		else
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_DELETE_NORIGHTS", currentTerritory.GetTerritoryName()), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE).Create(sender);
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
	protected void RPC_DeleteTerritoryAdmin( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
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
		
		array< ref ExpansionTerritoryMember > members = currentTerritory.GetTerritoryMembers();
		for ( int i = 0; i < members.Count(); ++i )
		{
			if (!members[i])
				continue;
			
			PlayerBase currPlayer = PlayerBase.GetPlayerByUID( members[i].GetID() );
			if (!currPlayer || !currPlayer.GetIdentity())
				continue;
			
			Send_UpdateClient( territoryID, null, currPlayer.GetIdentity() );
		}
		
		array< ref ExpansionTerritoryInvite > invites = currentTerritory.GetTerritoryInvites();
		for ( int j = 0; j < invites.Count(); ++j )
		{
			if (!invites[j])
				continue;
			
			PlayerBase currPlayerInvite = PlayerBase.GetPlayerByUID( invites[j].UID );
			if (!currPlayerInvite)
				continue;
			
			SyncPlayerInvitesServer(currPlayerInvite);
		}
		
		if ( sender && GetExpansionSettings().GetLog().Territory )
			GetExpansionSettings().GetLog().PrintLog( "[Territory] Admin \"" + sender.GetName() + "\" (id=" + sender.GetId() + ")" + " deleted territory " + currentTerritory.GetTerritoryName() + " at " + currentTerritory.GetPosition() );
		
		//Don't forget to set it as null before to delete, to not do a infinte loop
		flag.SetTerritory( null );
		if (!flag.ToDelete())
			flag.Delete();
		
		m_TerritoryFlags.Remove( territoryID );
		
		if (sender)
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_DELETED", currentTerritory.GetTerritoryName()), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE).Create(sender);
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_DeleteTerritoryAdmin - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionPartyModule SyncPlayerInvitesServer
	// Called on server
	// ------------------------------------------------------------
	void SyncPlayerInvitesServer( PlayerBase sender )
	{
		if ( !IsMissionHost() || !sender || !sender.GetIdentity() )
			return;
		
		string id = sender.GetIdentityUID();
		array< ref ExpansionTerritoryInvite > invites = new array< ref ExpansionTerritoryInvite >;
		
		foreach (int territoryID, TerritoryFlag flag: m_TerritoryFlags)
		{
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
		rpcServer.Send( NULL, ExpansionTerritoryModuleRPC.SyncPlayerInvites, true, sender.GetIdentity() );
	}
	
	// ------------------------------------------------------------
	// ExpansionPartyModule RPC_SyncPlayerInvitesClient
	// Called on client
	// ------------------------------------------------------------
	private void RPC_SyncPlayerInvitesClient( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( !player )
			return;
		
		array<ref ExpansionTerritoryInvite> invites;
		if ( !ctx.Read( invites ) )
			return;

		SetTerritoryInvites( invites );
		
		ExpansionUIScriptedMenu menu;
		if (Class.CastTo(menu, GetGame().GetUIManager().FindMenu(EXPANSION_MENU_BOOK)))
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
	private void RPC_RequestInvitePlayer( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
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
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NOPLAYER").Error(sender);
			return;
		}
		
		if ( !flag )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NOFLAG").Error(sender);
			return;
		}
		
		ExpansionTerritory territory = flag.GetTerritory();
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_RequestInvitePlayer - 4 territory: " + territory);
		#endif

		if ( !territory )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NOTERRITORY").Error(sender);
			return;
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_RequestInvitePlayer - 5");
		#endif

		if ( territory.IsMember( targetID ) || territory.HasInvite(targetID) )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_ALREADY_MEMBER", targetPlayer.GetName())).Error(sender);
			return;
		}
		
		if ( GetExpansionSettings().GetTerritory() && GetExpansionSettings().GetTerritory().MaxMembersInTerritory > 1 && territory.NumberOfMembers() >= GetExpansionSettings().GetTerritory().MaxMembersInTerritory )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_ERROR_MAX_TERRITORY", GetExpansionSettings().GetTerritory().MaxMembersInTerritory.ToString())).Error(sender);
			return;
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_RequestInvitePlayer - 6");
		#endif

		ExpansionTerritoryInvite invite = new ExpansionTerritoryInvite;
		invite.TerritoryName = territory.GetTerritoryName();
		invite.TerritoryID = territory.GetTerritoryID();
		invite.UID = targetPlayer.GetIdentityUID();
		
		if (!territory.AddTerritoryInvite(invite))
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_ERROR_INVITED", targetPlayer.GetName())).Error(sender);
			return;
		}
		
		SyncPlayerInvitesServer(targetPlayer);
		
		if ( GetExpansionSettings().GetLog().Territory )
			GetExpansionSettings().GetLog().PrintLog("[Territory] Player \"" + targetPlayer.GetIdentity().GetName() + "\" (id=" + targetPlayer.GetIdentity().GetId() + " pos=" + targetPlayer.GetPosition() +") was invited to join the territory " + territory.GetTerritoryName() + " at " + territory.GetPosition() + " by the player \"" + sender.GetName() + "\" (id=" + sender.GetId() +")");
		
		//! Message to request sender
		ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_PLAYER_INVITE_REQUEST_SENDER", targetPlayer.GetName()), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE).Create(sender);
		
		//! Message to request target
		ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_PLAYER_INVITE_REQUEST_TARGET",  sender.GetName(), territory.GetTerritoryName()), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE).Create(targetPlayer.GetIdentity());
		
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
		EXLogPrint("ExpansionTerritoryModule::AcceptInvite - Start");
		#endif
		
		if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( territoryID );
			rpc.Send( NULL, ExpansionTerritoryModuleRPC.AcceptInvite, true, NULL );
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::AcceptInvite - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule RPC_AcceptInvite
	// Called on server
	// ------------------------------------------------------------
	private void RPC_AcceptInvite( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RPC_AcceptInvite - Start");
		#endif
		
		int territoryID;
		if ( !ctx.Read( territoryID ) )
			return;
		
		Exec_AcceptInvite( territoryID, senderRPC );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RPC_AcceptInvite - End");
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
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NOFLAG").Error(sender);
			return;
		}
		
		ExpansionTerritory territory = flag.GetTerritory();
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_AcceptInvite - 3 territory: " + territory);
		#endif

		if ( !territory )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NOTERRITORY").Error(sender);
			return;
		}
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID( sender.GetId() );
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_AcceptInvite - 4 senderPlayer: " + senderPlayer);
		#endif

		if ( !senderPlayer )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NOPLAYER").Error(sender);
			return;
		}
		
		if ( GetExpansionSettings().GetTerritory().MaxTerritoryPerPlayer > 0 && GetNumberOfTerritory( sender.GetId() ) >= GetExpansionSettings().GetTerritory().MaxTerritoryPerPlayer )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_ERROR_MAX_TERRITORY_PER_PLAYER", GetExpansionSettings().GetTerritory().MaxTerritoryPerPlayer.ToString())).Error(sender);
			return;
		}
		
		territory.RemoveTerritoryInvite( sender.GetId() );
		territory.AddMember( sender.GetId(), sender.GetName() );
		
		SyncPlayerInvitesServer( senderPlayer );
		UpdateClient( territoryID );
		
		ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_PLAYER_ADDED", territory.GetTerritoryName()), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE).Create(sender);
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_AcceptInvite - 5");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule DeclineInvite
	// Called on client
	// ------------------------------------------------------------
	void DeclineInvite( int territoryID )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::DeclineInvite - Start");
		#endif
		
		if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( territoryID );
			rpc.Send( NULL, ExpansionTerritoryModuleRPC.DeclineInvite, true, NULL );
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::DeclineInvite - End");
		#endif
	}
	
		
	// ------------------------------------------------------------
	// ExpansionTerritoryModule RPC_DeclineInvite
	// Called on server
	// ------------------------------------------------------------
	private void RPC_DeclineInvite( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RPC_DeclineInvite - Start");
		#endif
		
		int territoryID;
		if ( !ctx.Read( territoryID ) )
			return;
		
		Exec_DeclineInvite( territoryID, senderRPC );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::RPC_DeclineInvite - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule Exec_AcceptInvite
	// Called on server
	// ------------------------------------------------------------
	private void Exec_DeclineInvite( int territoryID, notnull PlayerIdentity sender )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_DeclineInvite - 1 territoryID: " + territoryID);
		#endif
		
		TerritoryFlag flag = m_TerritoryFlags.Get( territoryID );
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_DeclineInvite - 2 flag: " + flag);
		#endif

		if ( !flag )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NOFLAG").Error(sender);	
			return;
		}
		
		ExpansionTerritory territory = flag.GetTerritory();
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_DeclineInvite - 3 territory: " + territory);
		#endif

		if ( !territory )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NOTERRITORY").Error(sender);
			return;
		}
		
		PlayerBase senderPlayer = PlayerBase.GetPlayerByUID( sender.GetId() );
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_DeclineInvite - 4 senderPlayer: " + senderPlayer);
		#endif

		if ( !senderPlayer )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NOPLAYER").Error(sender);	
			return;
		}
		
		territory.RemoveTerritoryInvite( sender.GetId() );		
		SyncPlayerInvitesServer( senderPlayer );
		//UpdateClient( territoryID );
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_DeclineInvite - 5");
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
	private void RPC_PromoteMember( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
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
	private void Exec_PromoteMember( int territoryID, ExpansionTerritoryMember member, PlayerIdentity sender )
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
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NOFLAG").Error(sender);
			return;
		}
		
		ExpansionTerritory territory = flag.GetTerritory();
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_PromoteMember - 3 territory : " + territory);
		#endif

		if ( !territory )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NOTERRITORY").Error(sender);
			return;
		}
		
		ExpansionTerritoryMember senderTerritory = territory.GetMember( sender.GetId() );
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_PromoteMember - 4 senderTerritory : " + senderTerritory);
		#endif

		if ( !senderTerritory )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NOSENDERTERRITORY").Error(sender);
			return;
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_PromoteMember - 5");
		#endif
		
		if ( senderTerritory.GetRank() != ExpansionTerritoryRank.ADMIN )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_PROMOTE_NORIGHTS").Error(sender);
			return;
		}
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_PromoteMember - 6");
		#endif
		
		ExpansionTerritoryMember target = territory.GetMember( member.GetID() );
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_PromoteMember - 7 target : " + target);
		#endif

		if ( !target )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NORECIVEMEMBER").Error(sender);
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
		
		ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_PLAYER_PROMOTE", target.GetName(), target.GetRankName()), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE).Create(sender);
		
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
	private void RPC_DemoteMember( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
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
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NOFLAG").Error(sender);
			return;
		}
		
		ExpansionTerritory territory = flag.GetTerritory();
		if ( !territory )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NOTERRITORY").Error(sender);
			return;
		}
		
		ExpansionTerritoryMember senderTerritory = territory.GetMember( sender.GetId() );
		if ( !senderTerritory )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NOSENDERTERRITORY").Error(sender);
			return;
		}
		
		ExpansionTerritoryMember target = territory.GetMember( member.GetID() );
		if ( !target )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NORECIVEMEMBER").Error(sender);
			return;
		}
		
		if ( senderTerritory.GetRank() == ExpansionTerritoryRank.MEMBER )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser( "STR_EXPANSION_TERRITORY_PLAYER_DEMOTE_ALREADYMEMBER", target.GetName())).Error(sender);
			return;
		}
		
		if ( target.GetRank() == ExpansionTerritoryRank.MODERATOR )
		{
			target.SetRank( ExpansionTerritoryRank.MEMBER );
		}
		
		UpdateClient( territoryID );
		
		ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_PLAYER_DEMOTE", target.GetName()), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE).Create(sender);
		
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
	private void RPC_KickMember( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
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
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NOFLAG").Error(sender);
			return;
		}
		
		ExpansionTerritory territory = flag.GetTerritory();
		if ( !territory )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NOTERRITORY").Error(sender);
			return;
		}
		
		ExpansionTerritoryMember senderTerritory = territory.GetMember( sender.GetId() );
		if ( !senderTerritory )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NOSENDERTERRITORY").Error(sender);
			return;
		}
		
		ExpansionTerritoryMember target = territory.GetMember( member.GetID() );
		if ( !target )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NORECIVEMEMBER").Error(sender);
			return;
		}
		
		if ( senderTerritory.GetRank() != ExpansionTerritoryRank.ADMIN && senderTerritory.GetRank() != ExpansionTerritoryRank.MODERATOR )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_PLAYER_KICK_NORIGHTS", target.GetName())).Error(sender);
			return;
		}
		
		if ( target.GetRank() == ExpansionTerritoryRank.ADMIN )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_CANT_KICK_SELF", target.GetName())).Error(sender);
			return;
		}
		
		if ( target.GetRank() == ExpansionTerritoryRank.MODERATOR && senderTerritory.GetRank() == ExpansionTerritoryRank.MODERATOR )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_CANT_KICK_MODERATOR", target.GetName())).Error(sender);
			return;
		}
		
		territory.RemoveMember( target );
		
		PlayerBase playerTarget = PlayerBase.GetPlayerByUID( target.GetID() );
		if (playerTarget)
		{
			Send_UpdateClient( territoryID, NULL, playerTarget.GetIdentity() );
			
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_PLAYER_KICKED_TARGET", territory.GetTerritoryName(), sender.GetName()), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE).Create(playerTarget.GetIdentity());
		}
		
		UpdateClient( territoryID );
		
		ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_PLAYER_KICKED_SENDER", target.GetName()), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE).Create(sender );
		
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
	private void RPC_Leave( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
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
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NOFLAG").Error(sender);
			return;
		}
		
		ExpansionTerritory territory = flag.GetTerritory();
		if ( !territory )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NOTERRITORY").Error(sender);
			return;
		}
		
		ExpansionTerritoryMember senderTerritory = territory.GetMember( sender.GetId() );
		if ( !senderTerritory )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ERROR_NOSENDERTERRITORY").Error(sender);
			return;
		}
		
		if ( senderTerritory.GetRank() == ExpansionTerritoryRank.ADMIN )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_PLAYER_LEAVE_OWNER").Error(sender);
			return;
		}
		
		territory.RemoveMember( senderTerritory );
		
		Send_UpdateClient( territoryID, NULL, sender );
		
		UpdateClient( territoryID );
		
		// TODO: message
		ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_PLAYER_LEAVE", territory.GetTerritoryName()), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE).Create(sender);
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::Exec_Leave - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule RPC_PlayerEnteredTerritory
	// Called on server
	// ------------------------------------------------------------
	private void RPC_PlayerEnteredTerritory( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
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
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_ENTER_TERRITORY", territory.GetTerritoryName()), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE).Create(senderRPC );	
			return;
		} else if ( type == 1 )
		{
			ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", new StringLocaliser("STR_EXPANSION_TERRITORY_LEFT_TERRITORY", territory.GetTerritoryName()), EXPANSION_NOTIFICATION_ICON_TERRITORY, COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE).Create(senderRPC);
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
	map<int, ref ExpansionTerritory> GetTerritories()
	{
		return m_Territories;
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritoryModule GetTerritories
	// ------------------------------------------------------------
	ExpansionTerritory GetTerritory( int territoryID )
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
			
			foreach (int territoryID, TerritoryFlag flag: m_TerritoryFlags)
			{
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
	//! NOTE: Doesn't actually find the nearest flag, but checks if player is in a territory and returns that flag (or NULL).
	//! Method not renamed to not break other mods.
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
		
		TerritoryFlag flag = GetFlagAtPosition3D(player.GetPosition());
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::FindNearestTerritoryFlag - End and return NULL");
		#endif
		
		return flag;
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
	// Expansion IsInsideOwnTerritory
	// Can be called on client or server
	// Check if a position is in player's own territory
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
		
		float territorySizeSq;
		float dist;

		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXPrint("position " + position);
		EXPrint("territorySize " + territorySize);
		EXPrint("playerUID " + playerUID);
		#endif

		if (IsMissionHost())
		{
			territorySizeSq = territorySize * territorySize;

			foreach (int territoryID, TerritoryFlag flag: m_TerritoryFlags)
			{
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
					
					dist = vector.DistanceSq( currTerritory.GetPosition(), position );

					#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
					EXPrint("territory pos " + currTerritory.GetPosition());
					EXPrint("dist " + dist);
					EXPrint("isMember " + currTerritory.IsMember(playerUID));
					EXPrint("canEdit " + CanEditTerritory(playerUID));
					#endif

					if ( dist <= territorySizeSq && ( currTerritory.IsMember(playerUID) || CanEditTerritory(playerUID) ) )
					{
						#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
						EXLogPrint("ExpansionTerritoryModule::IsInsideOwnTerritory - ID " + flag.GetTerritoryID() + " \"" + currTerritory.GetTerritoryName() + "\"");
						#endif
						return true;
					}
				}
			}
		}
		else
		{
			if ( CanEditTerritory(playerUID) && IsInTerritory( position, territorySize ) )
				return true;

			territorySizeSq = territorySize * territorySize;

			foreach ( int id, ExpansionTerritory territory: m_Territories )
			{
				if (!territory)
				{
					#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
					EXLogPrint("ExpansionTerritoryModule::IsInsideOwnTerritory - [ERROR] Territory is NULL!");
					#endif
					continue;
				}

				dist = vector.DistanceSq( territory.GetPosition(), position );

				#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
				EXPrint("territory pos " + territory.GetPosition());
				EXPrint("dist " + dist);
				#endif

				if ( dist <= territorySizeSq )
				{
					#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
					EXLogPrint("ExpansionTerritoryModule::IsInsideOwnTerritory - ID " + territory.GetTerritoryID() + " \"" + territory.GetTerritoryName() + "\"");
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

	bool CanEditTerritory(string playerUID)
	{
		if (GetGame().IsClient())
			return GetPermissionsManager().IsAdminToolsToggledOn();

		PlayerBase player = PlayerBase.GetPlayerByUID(playerUID);
		if (player && player.GetIdentity())
			return GetPermissionsManager().HasPermission("Expansion.Territories.Edit", player.GetIdentity());

		return false;
	}

	// ------------------------------------------------------------
	// Expansion IsInsideOwnTerritoryOrPerimeter
	// Can be called on client or server
	// Check if a position is in player's own territory or within its perimeter
	// ------------------------------------------------------------
	bool IsInsideOwnTerritoryOrPerimeter( vector position, float territorySize = -1, float perimeterSize = -1, string playerUID = "" )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXPrint("ExpansionTerritoryModule::IsInsideOwnTerritoryOrPerimeter - Start");
		#endif

		if ( territorySize <= 0 )
			territorySize = GetExpansionSettings().GetTerritory().TerritorySize;

		if ( perimeterSize <= 0 )
			perimeterSize = GetExpansionSettings().GetTerritory().TerritoryPerimeterSize + 5;

		return IsInsideOwnTerritory( position, territorySize + perimeterSize, playerUID );
	}

	// ------------------------------------------------------------
	// Expansion IsInsideOwnPerimeter
	// Can be called on client or server
	// Check if a position is in player's own territory perimeter (but not within territory itself)
	// ------------------------------------------------------------
	bool IsInsideOwnPerimeter( vector position, float territorySize = -1, float perimeterSize = -1, string playerUID = "" )
	{
		return !IsInsideOwnTerritory( position, territorySize, playerUID ) && IsInsideOwnTerritoryOrPerimeter( position, territorySize, perimeterSize, playerUID );
	}
	
	TerritoryFlag GetFlagAtPosition3D( vector position, float territorySize = -1 )
	{
		if ( territorySize <= 0 )
		{
			territorySize = GetExpansionSettings().GetTerritory().TerritorySize;
		}

		if ( IsMissionHost() )
		{
			foreach (int territoryID, TerritoryFlag flag: m_TerritoryFlags)
			{
				if (IsInTerritory(position, territorySize, flag))
					return flag;
			}
		} else
		{
			set<TerritoryFlag> flags = TerritoryFlag.ExpansionGetAll();
			
			foreach (TerritoryFlag clientFlag: flags)
			{
				if (IsInTerritory(position, territorySize, clientFlag))
					return clientFlag;
			}
		}

		return NULL;
	}

	// ------------------------------------------------------------
	// Expansion IsInTerritory
	// Can be called on client or server
	// Check if a position is in territory
	// ------------------------------------------------------------
	bool IsInTerritory( vector position, float territorySize = -1 )
	{
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::IsInTerritory - Start");
		#endif
		
		if (GetFlagAtPosition3D(position, territorySize))
			return true;
		
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionTerritoryModule::IsInTerritory - End and return false!");
		#endif
		
		return false;
	}

	bool IsInTerritory(vector position, float territorySize, TerritoryFlag flag)
	{
		if ( !flag.HasExpansionTerritoryInformation() )
		{
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXPrint("ExpansionTerritoryModule::IsInTerritory - flag has no territory info");
			#endif
			return false;
		}

		float territorySizeSq = territorySize * territorySize;

		if ( vector.DistanceSq( flag.GetPosition(), position ) <= territorySizeSq )
		{
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionTerritoryModule::IsInTerritory - ID " + flag.GetTerritoryID() + " \"" + flag.GetTerritory().GetTerritoryName() + "\"");
			#endif
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// Expansion IsInTerritoryOrPerimeter
	// Can be called on client or server
	// Check if a position is in territory or within its perimeter
	// ------------------------------------------------------------
	bool IsInTerritoryOrPerimeter( vector position, float territorySize = -1, float perimeterSize = -1 )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXPrint("ExpansionTerritoryModule::IsInTerritoryOrPerimeter - Start");
		#endif

		if ( territorySize <= 0 )
			territorySize = GetExpansionSettings().GetTerritory().TerritorySize;

		if ( perimeterSize <= 0 )
			perimeterSize = GetExpansionSettings().GetTerritory().TerritoryPerimeterSize + 5;

		return IsInTerritory( position, territorySize + perimeterSize );
	}

	// ------------------------------------------------------------
	// Expansion IsInPerimeter
	// Can be called on client or server
	// Check if a position is in territory perimeter (but not within territory itself)
	// ------------------------------------------------------------
	bool IsInPerimeter( vector position, float territorySize = -1, float perimeterSize = -1 )
	{
		return !IsInTerritory( position, territorySize ) && IsInTerritoryOrPerimeter( position, territorySize, perimeterSize );
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
	array<ref ExpansionTerritoryInvite> GetTerritoryInvites()
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
	map<int, TerritoryFlag> GetAllTerritoryFlags()
	{
		return m_TerritoryFlags;
	}
}