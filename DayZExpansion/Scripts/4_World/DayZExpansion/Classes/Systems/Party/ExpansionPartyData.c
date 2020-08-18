/**
 * ExpansionPartyData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPartyData 
{
	protected int PartyID;

	protected string PartyName;
	protected string OwnerUID;
	protected string OwnerName;

	protected ref array< ref ExpansionPartyPlayerData > Players;
	protected ref array< ref ExpansionPartyInviteData > Invites;
	protected ref array< ref ExpansionMarkerData > Markers;
	
	protected ref map< string, ExpansionPartyPlayerData > PlayersMap;
	protected ref map< string, ExpansionPartyInviteData > InvitesMap;
	protected ref map< string, ExpansionMarkerData > MarkersMap;
		
	// ------------------------------------------------------------
	// Expansion ExpansionPartyData Consturctor
	// ------------------------------------------------------------
	void ExpansionPartyData( int partyId = -1 ) 
	{
		PartyID = partyId;

		Players = new array< ref ExpansionPartyPlayerData >;
		Invites = new array< ref ExpansionPartyInviteData >;
		Markers = new array< ref ExpansionMarkerData >;
		
		PlayersMap = new map< string, ExpansionPartyPlayerData >;
		InvitesMap = new map< string, ExpansionPartyInviteData >;
		MarkersMap = new map< string, ExpansionMarkerData >;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionPartyData Destructor
	// ------------------------------------------------------------
	void ~ExpansionPartyData() 
	{
		int i;

		for ( i = 0; i < Players.Count(); ++i )
			delete Players[i];
		for ( i = 0; i < Invites.Count(); ++i )
			delete Invites[i];
		for ( i = 0; i < Markers.Count(); ++i )
			delete Markers[i];

		delete Players;
		delete Invites;
		delete Markers;

		delete PlayersMap;
		delete InvitesMap;
		delete MarkersMap;
	}
	
	// ------------------------------------------------------------
	// Expansion InitMaps
	// ------------------------------------------------------------
	void InitMaps()
	{
		PlayersMap.Clear();
		InvitesMap.Clear();
		MarkersMap.Clear();

		int index = 0;
		
		for ( index = 0; index < Players.Count(); ++index )
			if ( Players[index] )
				PlayersMap.Insert( Players[index].UID, Players[index] );
		
		for ( index = 0; index < Invites.Count(); ++index )
			if ( Invites[index] )
				InvitesMap.Insert( Invites[index].UID, Invites[index] );
		
		for ( index = 0; index < Markers.Count(); ++index )
			if ( Markers[index] )
				MarkersMap.Insert( Markers[index].GetUID(), Markers[index] );
	}	
	// ------------------------------------------------------------
	// Expansion GetPartyID
	// ------------------------------------------------------------
	int GetPartyID()
	{
		return PartyID;
	}
	
	// ------------------------------------------------------------
	// Expansion GetPartyName
	// ------------------------------------------------------------
	string GetPartyName() 
	{
		return PartyName;
	}
	
	// ------------------------------------------------------------
	// Expansion GetPlayers
	// ------------------------------------------------------------
	ref array< ref ExpansionPartyPlayerData > GetPlayers()
	{
		return Players;
	}
	
	// ------------------------------------------------------------
	// Expansion GetAllMarkers
	// ------------------------------------------------------------
	ref array<ref ExpansionMarkerData> GetAllMarkers()
	{
		return Markers;
	}
	
	// ------------------------------------------------------------
	// Expansion SetupExpansionPartyData
	// ------------------------------------------------------------
	void SetupExpansionPartyData( PlayerBase pPb, string partyName )
	{
	   	ref ExpansionPartyPlayerData player = AddPlayer( pPb, true );

		PartyName = partyName;
		OwnerUID = player.UID;
		OwnerName = player.Name;
	}

	// ------------------------------------------------------------
	// Expansion AddPlayer
	// ------------------------------------------------------------
	private ref ExpansionPartyPlayerData AddPlayer( PlayerBase pPb, bool owner = false )
	{
		string uid = pPb.GetIdentityUID();
		string name = pPb.GetIdentityName();

		ExpansionPartyPlayerData player = new ExpansionPartyPlayerData;
		player.UID = uid;
		player.Name = name;
		player.Promoted = owner;
		player.OnJoin( pPb );
		player.CreateMarker();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("SetupExpansionPartyData::AddPlayer player.Promoted : " + player.Promoted);
		#endif

		Players.Insert( player );
		PlayersMap.Insert( player.UID, player );

		return player;
	}
	
	// ------------------------------------------------------------
	// Expansion AcceptInvite
	// ------------------------------------------------------------
	bool AcceptInvite( PlayerBase player ) 
	{
		if ( RemoveInvite( player.GetIdentityUID() ) )
		{
			AddPlayer( player );

			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// Expansion DeclineInvite
	// ------------------------------------------------------------
	bool DeclineInvite( string uid )
	{
		return RemoveInvite( uid );
	}

	// ------------------------------------------------------------
	// Expansion CancelInvite
	// ------------------------------------------------------------
	bool CancelInvite( string uid )
	{
		return RemoveInvite( uid );
	}

	// ------------------------------------------------------------
	// Expansion RemoveInvite
	// ------------------------------------------------------------
	private bool RemoveInvite( string uid )
	{
		ref ExpansionPartyInviteData invite = InvitesMap.Get( uid );
		if ( invite != NULL )
		{
			Invites.RemoveItem( invite );
			InvitesMap.Remove( uid );
			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion AddInvite
	// ------------------------------------------------------------
	ref ExpansionPartyInviteData AddInvite( string playerID ) 
	{
		ref ExpansionPartyInviteData invite = InvitesMap.Get( playerID );
		if ( invite != NULL )
		{
			Error("ExpansionPartyData::AddInvite Add a player already exist");
			DumpStack();
			return invite;
		}

		invite = new ExpansionPartyInviteData;
		invite.partyID = PartyID;
		invite.UID = playerID;
		invite.partyName = PartyName;
		
		Invites.Insert( invite );
		InvitesMap.Insert( playerID, invite );
		
		return invite;
	}
		
	// ------------------------------------------------------------
	// Expansion RemovePlayer
	// ------------------------------------------------------------
	bool RemovePlayer( string uid )
	{
		ref ExpansionPartyPlayerData player = PlayersMap.Get( uid );
		if ( player )
		{
			Players.RemoveItem( player );
			PlayersMap.Remove( uid );
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// Expansion GetPlayer
	// ------------------------------------------------------------
	ref ExpansionPartyPlayerData GetPlayer( string uid )
	{
		return PlayersMap.Get( uid );
	}

	// ------------------------------------------------------------
	// Expansion HasPlayerInvite
	// ------------------------------------------------------------
	bool HasPlayerInvite( string uid )
	{		
		return InvitesMap.Get( uid ) != NULL;
	}
	
	// ------------------------------------------------------------
	// Expansion GetPlayerInvite
	// ------------------------------------------------------------
	ref ExpansionPartyInviteData GetPlayerInvite( string uid )
	{
		return InvitesMap.Get( uid );
	}
		
	// ------------------------------------------------------------
	// Expansion AddMarker
	// ------------------------------------------------------------
	bool AddMarker( ref ExpansionMarkerData marker )
	{
		if ( marker.GetUID() == "" )
		{
			string newUid = "" + Math.RandomInt( 0, int.MAX );
			while ( MarkersMap.Get( newUid ) )
				newUid = "" + Math.RandomInt( 0, int.MAX );
			
			marker.SetUID( newUid );
		} else if ( MarkersMap.Get( marker.GetUID() ) )
			return false;

		Markers.Insert( marker );
		MarkersMap.Insert( marker.GetUID(), marker );

		return true;
	}

	ref ExpansionMarkerData GetMarker( string uid )
	{
		return MarkersMap.Get( uid );
	}
		
	// ------------------------------------------------------------
	// Expansion UpdateMarker
	// ------------------------------------------------------------
	bool UpdateMarker( ref ExpansionMarkerData marker )
	{
		ExpansionMarkerData orgi = MarkersMap.Get( marker.GetUID() );
		if ( !orgi )
			return false;

		int dummy_version = int.MAX;
		ScriptReadWriteContext ctx = new ScriptReadWriteContext;
		marker.OnStoreSave( ctx.GetWriteContext() );
		orgi.OnStoreLoad( ctx.GetReadContext(), dummy_version );

		return true;
	}

	// ------------------------------------------------------------
	// Expansion RemoveMarker
	// ------------------------------------------------------------
	bool RemoveMarker( string markerName )
	{
		ExpansionMarkerData marker = MarkersMap.Get( markerName );
		if ( marker )
		{
			int removeIndex = Markers.Find( marker );
			if ( removeIndex != -1 )
				Markers.Remove( removeIndex );

			MarkersMap.Remove( markerName );
			delete marker;
			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateMarkerPosition
	// ------------------------------------------------------------
	bool UpdateMarkerPosition( string markerName, vector markerPosition )
	{
		ExpansionMarkerData marker = MarkersMap.Get( markerName );
		if ( marker )
		{
			marker.SetPosition( markerPosition );
			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion GetPartyName
	// ------------------------------------------------------------
	string GetOwnerName() 
	{
		return OwnerName;
	}
		
	// ------------------------------------------------------------
	// Expansion GetOwnerUID
	// ------------------------------------------------------------
	string GetOwnerUID()
	{
		return OwnerUID;
	}
		
	// ------------------------------------------------------------
	// Expansion Save
	// ------------------------------------------------------------
	void Save() 
	{
		if ( !IsMissionHost() )
			return;
		
		FileSerializer file = new FileSerializer;
		
		if (file.Open(EXPANSION_GROUPS_FOLDER + PartyID + ".bin", FileMode.WRITE))
		{
			file.Write(EXPANSION_VERSION_CURRENT_SAVE);
			OnStoreSave(file);
			
			file.Close();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion Delete
	// ------------------------------------------------------------
	void Delete()
	{
		if ( !IsMissionHost() )
			return;
		
		if (EXPANSION_VERSION_CURRENT_SAVE >= 4)
		{
			DeleteFile( EXPANSION_GROUPS_FOLDER + PartyID + ".bin" );
		}
		else
		{
			DeleteFile( EXPANSION_GROUPS_FOLDER + PartyID + ".json" );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion CountPlayers
	// ------------------------------------------------------------
	private int CountPlayers()
	{
		int nmb = 0;
		
		for (int i = 0; i < Players.Count(); ++i)
		{
			if (!Players[i])
				continue;
			
			nmb++;
		}
		
		return nmb;
	}
	
	// ------------------------------------------------------------
	// Expansion CountInvites
	// ------------------------------------------------------------
	private int CountInvites()
	{
		int nmb = 0;
		
		for (int i = 0; i < Invites.Count(); ++i)
		{
			if (!Invites[i])
				continue;
			
			nmb++;
		}
		
		return nmb;
	}
	
	// ------------------------------------------------------------
	// Expansion CountMarkers
	// ------------------------------------------------------------
	private int CountMarkers()
	{
		int nmb = 0;
		
		for (int i = 0; i < Markers.Count(); ++i)
		{
			if ( !Markers[i] )
				continue;
			
			nmb++;
		}
		
		return Markers.Count();
	}

	void OnSend( ParamsWriteContext ctx )
	{
		ctx.Write( PartyID );
		ctx.Write( PartyName );
		ctx.Write( OwnerUID );
		ctx.Write( OwnerName );

		int count = 0;
		int index = 0;

		count = Players.Count();
		ctx.Write( count );
		for ( index = 0; index < count; ++index )
		{
			ctx.Write( Players[index].UID );
			ctx.Write( Players[index].Name );
			ctx.Write( Players[index].Promoted );

			if ( Players[index].Marker )
			{
				bool hasMarker = true;
				if ( !Players[index].Marker.GetObject() )
				{
					hasMarker = false;
					// todo: update marker, maybe player respawned?
				}

				ctx.Write( hasMarker );

				if ( hasMarker )
					Players[index].Marker.OnSend( ctx );
			} else
			{
				ctx.Write( false );
			}

			if ( Players[index].QuickMarker )
			{
				ctx.Write( true );

				Players[index].QuickMarker.OnSend( ctx );
			} else
			{
				ctx.Write( false );
			}
		}

		count = Invites.Count();
		ctx.Write( count );
		for ( index = 0; index < count; ++index )
		{
			ctx.Write( Invites[index].UID );
		}

		count = Markers.Count();
		ctx.Write( count );
		for ( index = 0; index < count; ++index )
		{
			ctx.Write( Markers[index].GetUID() );
			Markers[index].OnSend( ctx );
		}
	}

	bool OnRecieve( ParamsReadContext ctx )
	{
		// PartyID is read in ExpansionPartyModule
		
		if ( !ctx.Read( PartyName ) )
			return false;
		if ( !ctx.Read( OwnerUID ) )
			return false;
		if ( !ctx.Read( OwnerName ) )
			return false;

		string uid = "";

		int count = 0;
		int index = 0;
		int removeIndex = 0;
		array< string > checkArr;
		if ( !ctx.Read( count ) )
			return false;

		ExpansionPartyPlayerData player;
		checkArr = PlayersMap.GetKeyArray();
		for ( index = 0; index < count; ++index )
		{
			if ( !ctx.Read( uid ) )
				return false;

			removeIndex = checkArr.Find( uid );
			if ( removeIndex != -1 )
				checkArr.Remove( removeIndex );

			player = PlayersMap.Get( uid );
			if ( !player )
			{
				player = new ExpansionPartyPlayerData();
				PlayersMap.Insert( uid, player );
				Players.Insert( player );
			}

			player.UID = uid;

			if ( !ctx.Read( player.Name ) )
				return false;
			if ( !ctx.Read( player.Promoted ) )
				return false;

			bool hasMarker;
			if ( !ctx.Read( hasMarker ) )
				return false;

			if ( hasMarker )
			{
				if ( !player.Marker )
					player.Marker = new ExpansionPlayerMarkerData( uid );

				if ( !player.Marker.OnRecieve( ctx ) )
					return false;
			} else
			{
				if ( player.Marker )
					delete player.Marker;
			}

			bool hasQuickMarker;
			if ( !ctx.Read( hasQuickMarker ) )
				return false;

			if ( hasQuickMarker )
			{
				if ( !player.QuickMarker )
					player.QuickMarker = new ExpansionPartyQuickMarkerData( "QuickMarker" + uid );

				if ( !player.QuickMarker.OnRecieve( ctx ) )
					return false;
			} else
			{
				if ( player.QuickMarker )
					delete player.QuickMarker;
			}
		}
		for ( index = 0; index < checkArr.Count(); ++index )
		{
			player = PlayersMap.Get( checkArr[index] );
			if ( player )
			{
				removeIndex = Players.Find( player );
				if ( removeIndex != -1 )
					Players.Remove( removeIndex );

				PlayersMap.Remove( checkArr[index] );
				delete player;
			}
		}
		
		if ( !ctx.Read( count ) )
			return false;

		ExpansionPartyInviteData invite;
		checkArr = InvitesMap.GetKeyArray();
		for ( index = 0; index < count; ++index )
		{
			if ( !ctx.Read( uid ) )
				return false;

			removeIndex = checkArr.Find( uid );
			if ( removeIndex != -1 )
				checkArr.Remove( removeIndex );

			invite = InvitesMap.Get( uid );
			if ( !invite )
			{
				invite = new ExpansionPartyInviteData();
				InvitesMap.Insert( uid, invite );
				Invites.Insert( invite );
			}
		}
		for ( index = 0; index < checkArr.Count(); ++index )
		{
			invite = InvitesMap.Get( checkArr[index] );
			if ( invite )
			{
				removeIndex = Invites.Find( invite );
				if ( removeIndex != -1 )
					Invites.Remove( removeIndex );

				InvitesMap.Remove( checkArr[index] );
				delete invite;
			}
		}
		
		if ( !ctx.Read( count ) )
			return false;

		ExpansionMarkerData marker;
		checkArr = MarkersMap.GetKeyArray();
		for ( index = 0; index < count; ++index )
		{
			if ( !ctx.Read( uid ) )
				return false;

			removeIndex = checkArr.Find( uid );
			if ( removeIndex != -1 )
				checkArr.Remove( removeIndex );

			marker = MarkersMap.Get( uid );
			if ( !marker )
			{
				marker = ExpansionMarkerData.Create( ExpansionMapMarkerType.PARTY, uid );
				MarkersMap.Insert( uid, marker );
				Markers.Insert( marker );
			}

			if ( !marker.OnRecieve( ctx ) )
				return false;
		}
		for ( index = 0; index < checkArr.Count(); ++index )
		{
			marker = MarkersMap.Get( checkArr[index] );
			if ( marker )
			{
				removeIndex = Markers.Find( marker );
				if ( removeIndex != -1 )
					Markers.Remove( removeIndex );

				MarkersMap.Remove( checkArr[index] );
				delete marker;
			}
		}

		return true;
	}
	
	// ------------------------------------------------------------
	// Expansion OnStoreSave
	// ------------------------------------------------------------
	void OnStoreSave( ParamsWriteContext ctx )
	{
		ctx.Write(PartyID);
		ctx.Write(PartyName);
		ctx.Write(OwnerUID);
		ctx.Write(OwnerName);
		
		int i = 0;
		
		ctx.Write( Players.Count() );
		for ( i = 0; i < Players.Count(); ++i )
			Players[i].OnStoreSave( ctx );
		
		ctx.Write( Invites.Count() );
		for ( i = 0; i < Invites.Count(); ++i)
			Invites[i].OnStoreSave( ctx );
		
		ctx.Write( Markers.Count() );
		for ( i = 0; i < Markers.Count(); ++i )
		{
			ctx.Write( Markers[i].GetUID() );
			Markers[i].OnStoreSave( ctx );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion OnStoreLoad
	// ------------------------------------------------------------
	bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( Expansion_Assert_False( ctx.Read( PartyID ), "Failed reading party uid" ) )
			return false;
		
		if ( Expansion_Assert_False( ctx.Read( PartyName ), "Failed reading party name" ) )
			return false;
		
		if ( Expansion_Assert_False( ctx.Read( OwnerUID ), "Failed reading owner uid" ) )
			return false;
		
		if ( Expansion_Assert_False( ctx.Read( OwnerName ), "Failed reading owner name" ) )
			return false;
		
		int i;

		int countPlayers;
		if ( Expansion_Assert_False( ctx.Read( countPlayers ), "Failed reading player count" ) )
			return false;
		
		for ( i = 0; i < countPlayers; ++i )
			Players.Insert( new ExpansionPartyPlayerData );

		for ( i = 0; i < Players.Count(); ++i )
			if ( Expansion_Assert_False( Players[i].OnStoreLoad( ctx, version ), "Failed reading player [" + i + "]" ) )
				return false;
		
		int countInvites;
		if ( Expansion_Assert_False( ctx.Read( countInvites ), "Failed reading invite count" ) )
			return false;
		
		for ( i = 0; i < countInvites; ++i )
			Invites.Insert( new ExpansionPartyInviteData );

		for ( i = 0; i < Invites.Count(); ++i )
			if ( Expansion_Assert_False( Invites[i].OnStoreLoad( ctx, version ), "Failed reading invite [" + i + "]" ) )
				return false;

		int countMarkers;
		if ( Expansion_Assert_False( ctx.Read( countMarkers ), "Failed reading marker count" ) )
			return false;
		
		for ( i = 0; i < countMarkers; ++i )
			Markers.Insert( ExpansionMarkerData.Create( ExpansionMapMarkerType.PARTY ) );

		for ( i = 0; i < Markers.Count(); ++i )
		{
			string marker_uid;

			if ( version >= 8 )
			{
				if ( Expansion_Assert_False( ctx.Read( marker_uid ), "Failed reading marker_uid [" + i + "]" ) )
					return false;
			} else
			{
				marker_uid = "marker_old_" + i;
			}

			Markers[i].SetUID( marker_uid );

			if ( Expansion_Assert_False( Markers[i].OnStoreLoad( ctx, version ), "Failed reading marker [" + i + "]" ) )
				return false;
		}
		
		return true;
	}
}