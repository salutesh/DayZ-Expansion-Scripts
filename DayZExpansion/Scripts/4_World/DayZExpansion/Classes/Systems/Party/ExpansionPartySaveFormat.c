/**
 * ExpansionPartySaveFormat.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPartyInvite
{
	int partyID;
	string partyName;
	string UID;
	
	void OnStoreSave(FileSerializer file)
	{
		file.Write(partyID);
		file.Write(partyName);
		file.Write(UID);
	}
	
	bool OnStoreLoad(FileSerializer file, int version)
	{
		if (!file.Read(partyID))
			return false;
		
		if (!file.Read(partyName))
			return false;
		
		if (!file.Read(UID))
			return false;
		
		return true;
	}
}

class ExpansionPartySaveFormatPlayer
{
	string UID;
	string Name;
	bool Promoted;
	
	void OnStoreSave(FileSerializer file)
	{
		file.Write(UID);
		file.Write(Name);
		file.Write(Promoted);
	}
	
	bool OnStoreLoad(FileSerializer file, int version)
	{
		if (!file.Read(UID))
			return false;
		
		if (!file.Read(Name))
			return false;
		
		if (!file.Read(Promoted))
			return false;
		
		return true;
	}
}

class ExpansionQuickMarker
{
	[NonSerialized()]
	string UID;
	
	vector Pos;
	int Color;
}

class ExpansionPartySaveFormat 
{
	protected int PartyID;

	protected string PartyName;
	protected string OwnerUID;
	protected string OwnerName;

	//string inside map is the uid of the player, so can easily search in log n instead n with array
	protected autoptr array< ref ExpansionPartySaveFormatPlayer > Players;
	protected autoptr array< ref ExpansionPartyInvite > Invites;
	
	protected autoptr array< ref ExpansionMapMarker > Markers;
	
	[NonSerialized()]
	protected autoptr map< string, ref ExpansionPartySaveFormatPlayer > PlayersMap;

	[NonSerialized()]
	protected autoptr map< string, ref ExpansionPartyInvite > InvitesMap;
	
	[NonSerialized()]
	protected autoptr array<ref ExpansionQuickMarker> m_QuickMarkers;
		
	// ------------------------------------------------------------
	// Expansion ExpansionPartySaveFormat Consturctor
	// ------------------------------------------------------------
	void ExpansionPartySaveFormat( int partyId = -1 ) 
	{
		PartyID = partyId;

		Players = new array< ref ExpansionPartySaveFormatPlayer >;
		Invites = new array< ref ExpansionPartyInvite >;
		
		Markers = new array< ref ExpansionMapMarker >;
		
		PlayersMap = new map< string, ref ExpansionPartySaveFormatPlayer >;
		InvitesMap = new map< string, ref ExpansionPartyInvite >;
		
		m_QuickMarkers = new array<ref ExpansionQuickMarker>;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionPartySaveFormat Destructor
	// ------------------------------------------------------------
	void ~ExpansionPartySaveFormat() 
	{
	}
	
	// ------------------------------------------------------------
	// Expansion InitMaps
	// ------------------------------------------------------------
	void InitMaps()
	{
		if (!PlayersMap)
			PlayersMap = new map< string, ref ExpansionPartySaveFormatPlayer >;
		
		if (!InvitesMap)
			InvitesMap = new map< string, ref ExpansionPartyInvite >;
		
		PlayersMap.Clear();
		InvitesMap.Clear();
		
		for ( int i = 0; i < Players.Count(); ++i )
		{
			ref ExpansionPartySaveFormatPlayer player = Players[i];
			if ( player )
			{
				PlayersMap.Insert( player.UID, player );
			}
		}
		
		for ( int j = 0; j < Invites.Count(); ++j )
		{
			ref ExpansionPartyInvite invite = Invites[i];
			if (invite)
			{
				InvitesMap.Insert( invite.UID, invite );
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion SetQuickMarkers
	// ------------------------------------------------------------
	void SetQuickMarkers(ref array<ref ExpansionQuickMarker> quickMarkers)
	{
		m_QuickMarkers = quickMarkers;
	}
	
	// ------------------------------------------------------------
	// Expansion GetQuickMarkers
	// ------------------------------------------------------------
	ref array<ref ExpansionQuickMarker> GetQuickMarkers()
	{
		return m_QuickMarkers;
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateQuickMarker
	// ------------------------------------------------------------
	void UpdateQuickMarker(ref ExpansionQuickMarker quickMarker)
	{
		if (!IsMissionHost())
			return;
		
		if (!m_QuickMarkers)
			m_QuickMarkers = new array<ref ExpansionQuickMarker>;
		
		for (int i = 0; i < m_QuickMarkers.Count(); ++i)
		{
			if (m_QuickMarkers[i] && m_QuickMarkers[i].UID == quickMarker.UID)
			{
				delete m_QuickMarkers[i];
				m_QuickMarkers[i] = quickMarker;
				return;
			}
		}
		
		m_QuickMarkers.Insert( quickMarker );
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateQuickMarker
	// ------------------------------------------------------------
	bool RemoveQuickMarker(string id)
	{
		if (!IsMissionHost())
			return false;
		
		if (!m_QuickMarkers)
			m_QuickMarkers = new array<ref ExpansionQuickMarker>;
		
		for (int i = 0; i < m_QuickMarkers.Count(); ++i)
		{
			if (m_QuickMarkers[i] && m_QuickMarkers[i].UID == id)
			{
				m_QuickMarkers.Remove(i);
				return true;
			}
		}
		
		return false;
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
	ref array< ref ExpansionPartySaveFormatPlayer > GetPlayers()
	{
		return Players;
	}
	
	// ------------------------------------------------------------
	// Expansion GetAllMarkers
	// ------------------------------------------------------------
	ref array<ref ExpansionMapMarker> GetAllMarkers()
	{
		return Markers;
	}
	
	// ------------------------------------------------------------
	// Expansion SetupExpansionPartySaveFormat
	// ------------------------------------------------------------
	void SetupExpansionPartySaveFormat( PlayerBase pPb, string partyName )
	{
	   	ref ExpansionPartySaveFormatPlayer player = AddPlayer( pPb, true );

		PartyName = partyName;
		OwnerUID = player.UID;
		OwnerName = player.Name;
	}

	// ------------------------------------------------------------
	// Expansion AddPlayer
	// ------------------------------------------------------------
	private ref ExpansionPartySaveFormatPlayer AddPlayer( PlayerBase pPb, bool owner = false )
	{
		string uid = pPb.GetIdentityUID();
		string name = pPb.GetIdentityName();

		ExpansionPartySaveFormatPlayer player = new ExpansionPartySaveFormatPlayer;
		player.UID = uid;
		player.Name = name;
		player.Promoted = owner;
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("SetupExpansionPartySaveFormat::AddPlayer player.Promoted : " + player.Promoted);
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
		ref ExpansionPartyInvite invite = InvitesMap.Get( uid );
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
	ref ExpansionPartyInvite AddInvite( string playerID ) 
	{
		ref ExpansionPartyInvite invite = InvitesMap.Get( playerID );
		if ( invite != NULL )
		{
			Error("ExpansionPartySaveFormat::AddInvite Add a player already exist");
			DumpStack();
			return invite;
		}

		invite = new ExpansionPartyInvite;
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
		ref ExpansionPartySaveFormatPlayer player = PlayersMap.Get( uid );
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
	ref ExpansionPartySaveFormatPlayer GetPlayer( string uid )
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
	ref ExpansionPartyInvite GetPlayerInvite( string uid )
	{
		return InvitesMap.Get( uid );
	}
		
	// ------------------------------------------------------------
	// Expansion AddMarker
	// ------------------------------------------------------------
	void AddMarker( ref ExpansionMapMarker marker )
	{
		Markers.Insert( marker );
	}
		
	// ------------------------------------------------------------
	// Expansion RemoveMarker
	// ------------------------------------------------------------
	bool RemoveMarker( string markerName )
	{
		for ( int i = 0; i < Markers.Count(); ++i )
		{
			if ( Markers.Get(i).GetMarkerText() == markerName )
			{
				Markers.Remove(i);
				return true;
			}
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateMarkerPosition
	// ------------------------------------------------------------
	bool UpdateMarkerPosition( string markerName, vector markerPosition )
	{
		for ( int i = 0; i < Markers.Count(); ++i )
		{
			if ( Markers[i] && Markers[i].GetMarkerText() == markerName )
			{
				Markers[i].SetPosition( markerPosition );
				return true;
			}
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
			if (!Markers[i])
				continue;
			
			nmb++;
		}
		
		return nmb;
	}
	
	// ------------------------------------------------------------
	// Expansion OnStoreSave
	// ------------------------------------------------------------
	protected void OnStoreSave(FileSerializer file)
	{
		file.Write(PartyID);
		file.Write(PartyName);
		file.Write(OwnerUID);
		file.Write(OwnerName);
		
		int i = 0;
		
		file.Write(CountPlayers());
		
		for (i = 0; i < Players.Count(); ++i)
		{
			if (!Players[i])
				continue;
			
			Players[i].OnStoreSave(file);
		}
		
		file.Write(CountInvites());
		
		for (i = 0; i < Invites.Count(); ++i)
		{
			if (!Invites[i])
				continue;
			
			Invites[i].OnStoreSave(file);
		}
		
		file.Write(CountMarkers());
		
		for (i = 0; i < Markers.Count(); ++i)
		{
			if (!Markers[i])
				continue;
			
			Markers[i].OnStoreSave(file);
		}
	}
	
	// ------------------------------------------------------------
	// Expansion OnStoreLoad
	// ------------------------------------------------------------
	bool OnStoreLoad(FileSerializer file, int version)
	{
		if (!file.Read(PartyID))
			return false;
		
		if (!file.Read(PartyName))
			return false;
		
		if (!file.Read(OwnerUID))
			return false;
		
		if (!file.Read(OwnerName))
			return false;
		
		int i;
		int countPlayers;
		if (!file.Read(countPlayers))
			return false;
		
		for (i = 0; i < countPlayers; ++i)
		{
			ExpansionPartySaveFormatPlayer newPlayer = new ExpansionPartySaveFormatPlayer;
			if (!newPlayer.OnStoreLoad(file, version))
				return false;
			
			Players.Insert(newPlayer);
		}
		
		int countInvites;
		if (!file.Read(countInvites))
			return false;
		
		for (i = 0; i < countInvites; ++i)
		{
			ExpansionPartyInvite newInvite = new ExpansionPartyInvite;
			if (!newInvite.OnStoreLoad(file, version))
				return false;
			
			Invites.Insert(newInvite);
		}
		
		int countMarkers;
		if (!file.Read(countMarkers))
			return false;
		
		for (i = 0; i < countMarkers; ++i)
		{
			ExpansionMapMarker newMarker = new ExpansionMapMarker("", -1, "0 0 0", -1, false);
			if (!newMarker.OnStoreLoad(file, version))
				return false;
			
			Markers.Insert(newMarker);
		}
		
		return true;
	}
}