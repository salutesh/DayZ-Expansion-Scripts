/**
 * ExpansionMarkerClientData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarkerClientData : Managed
{
	string m_IP;
	int m_Port;

	int m_NextPersonalUID;

	ref array< ref ExpansionMarkerData > m_PersonalMarkers;

	ref map< string, ref ExpansionMarkerClientInfo > m_MarkerInfo_Server;

	protected int m_PartyUID;
	ref map< string, ref ExpansionMarkerClientInfo > m_MarkerInfo_Party;
	ref map< string, ref ExpansionMarkerClientInfo > m_MarkerInfo_PartyPlayers;
	
	protected ExpansionMarkerModule m_MarkerModule;
	protected ExpansionPartyModule m_PartyModule;

	void ExpansionMarkerClientData()
	{
		Class.CastTo( m_MarkerModule, GetModuleManager().GetModule( ExpansionMarkerModule ) );
		Class.CastTo( m_PartyModule, GetModuleManager().GetModule( ExpansionPartyModule ) );

		m_PersonalMarkers = new array< ref ExpansionMarkerData >();

		m_MarkerInfo_Server = new map< string, ref ExpansionMarkerClientInfo >();
		m_MarkerInfo_Party = new map< string, ref ExpansionMarkerClientInfo >();
		m_MarkerInfo_PartyPlayers = new map< string, ref ExpansionMarkerClientInfo >();
	}

	void ~ExpansionMarkerClientData()
	{
		delete m_PersonalMarkers;
	}
	
	void OnStoreSave( ParamsWriteContext file )
	{
		file.Write( m_IP );
		file.Write( m_Port );

		file.Write( m_NextPersonalUID );
		file.Write( m_PersonalMarkers.Count() );
		for ( int i = 0; i < m_PersonalMarkers.Count(); ++i )
		{
			file.Write( m_PersonalMarkers[i].GetUID() );
			m_PersonalMarkers[i].OnStoreSave( file );
		}

		file.Write( m_MarkerInfo_Server.Count() );
		for ( i = 0; i < m_MarkerInfo_Server.Count(); ++i )
		{
			m_MarkerInfo_Server.GetElement(i).OnStoreSave( file );
		}

		file.Write( m_PartyUID );
		file.Write( m_MarkerInfo_Party.Count() );
		for ( i = 0; i < m_MarkerInfo_Party.Count(); ++i )
		{
			m_MarkerInfo_Party.GetElement(i).OnStoreSave( file );
		}

		file.Write( m_MarkerInfo_PartyPlayers.Count() );
		for ( i = 0; i < m_MarkerInfo_PartyPlayers.Count(); ++i )
		{
			m_MarkerInfo_PartyPlayers.GetElement(i).OnStoreSave( file );
		}
	}
	
	bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		//if ( Expansion_Assert_False( ctx.Read( m_IP ), "[" + this + "] Failed reading m_IP" ) )
		//	return false;
		//if ( Expansion_Assert_False( ctx.Read( m_Port ), "[" + this + "] Failed reading m_Port" ) )
		//	return false;

		m_MarkerInfo_Server.Clear();
		m_MarkerInfo_Party.Clear();
		m_MarkerInfo_PartyPlayers.Clear();

		if ( version >= 8 )
		{
			if ( Expansion_Assert_False( ctx.Read( m_NextPersonalUID ), "[" + this + "] Failed reading m_NextPersonalUID" ) )
				return false;
		}

		int count = 0;
		int index = 0;

		string uid = "";

		if ( !Expansion_Assert_False( ctx.Read( count ), "[" + this + "] Failed reading personal count" ) )
		{
			for ( index = 0; index < count; ++index ) 
			{
				if ( Expansion_Assert_False( ctx.Read( uid ), "[" + this + "] Failed reading uid" ) )
					return false;

				ExpansionMarkerData newMarker = NULL;
				for ( int personal_marker = 0; personal_marker < m_PersonalMarkers.Count(); ++personal_marker )
					if ( m_PersonalMarkers[personal_marker].GetUID() == uid )
						newMarker = m_PersonalMarkers[personal_marker];

				if ( !newMarker )
				{
					newMarker = ExpansionMarkerData.Create( ExpansionMapMarkerType.PERSONAL, uid );

					m_PersonalMarkers.Insert( newMarker );
				}

				if ( !newMarker.OnStoreLoad( ctx, version ) )
					return false;
			}
		} else
		{
			return false;
		}
		
		ExpansionMarkerClientInfo newMarkerInfo;

		if ( !Expansion_Assert_False( ctx.Read( count ), "[" + this + "] Failed reading server count" ) )
		{
			for ( index = 0; index < count; ++index ) 
			{
				newMarkerInfo = new ExpansionMarkerClientInfo();
				if ( !newMarkerInfo.OnStoreLoad( ctx, version ) )
					return false;
				
				m_MarkerInfo_Server.Insert( newMarkerInfo.GetUID(), newMarkerInfo );
			}
		} else
		{
			return false;
		}

		
		if ( Expansion_Assert_False( ctx.Read( m_PartyUID ), "[" + this + "] Failed reading m_PartyUID" ) )
			return false;

		if ( !Expansion_Assert_False( ctx.Read( count ), "[" + this + "] Failed reading party count" ) )
		{
			for ( index = 0; index < count; ++index ) 
			{
				newMarkerInfo = new ExpansionMarkerClientInfo();
				if ( !newMarkerInfo.OnStoreLoad( ctx, version ) )
					return false;
				
				m_MarkerInfo_Party.Insert( newMarkerInfo.GetUID(), newMarkerInfo );
			}
		} else
		{
			return false;
		}

		if ( !Expansion_Assert_False( ctx.Read( count ), "[" + this + "] Failed reading party player count" ) )
		{
			for ( index = 0; index < count; ++index ) 
			{
				newMarkerInfo = new ExpansionMarkerClientInfo();
				if ( !newMarkerInfo.OnStoreLoad( ctx, version ) )
					return false;
				
				m_MarkerInfo_PartyPlayers.Insert( newMarkerInfo.GetUID(), newMarkerInfo );
			}
		} else
		{
			return false;
		}
		
		return true;
	}

	bool Equals( ExpansionMarkerClientData other )
	{
		return m_IP == other.m_IP && m_Port == other.m_Port;
	}

	bool Equals( string ip, int port )
	{
		return ip == m_IP && port == m_Port;
	}

	array< ExpansionMarkerData > GetAll()
	{
		array< ExpansionMarkerData > markers = new array< ExpansionMarkerData >();
		
		ExpansionMarkerData marker;
		
		int i = 0;

		if (GetExpansionSettings().GetMap().CanCreateMarker)
		{
			for ( i = 0; i < PersonalCount(); ++i )
			{
				marker = PersonalGet( i );
				if ( marker )
					markers.Insert( marker );
			}
		}
		
		for ( i = 0; i < PartyCount(); ++i )
		{
			marker = PartyGet( i );
			if ( marker )
				markers.Insert( marker );
		}

		PlayerBase localPlayer = PlayerBase.Cast( GetGame().GetPlayer() );
		string localUid = "";
		if ( localPlayer )
			localUid = localPlayer.GetIdentityUID();

		if (GetExpansionSettings().GetParty().ShowPartyMember3DMarkers)
		{
			for ( i = 0; i < PartyPlayerCount(); ++i )
			{
				marker = PartyPlayerGet( i );
				if ( marker && marker.GetUID() != localUid && localUid != "")
				{
					markers.Insert( marker );
				}
			}
		}

		if (GetExpansionSettings().GetMap().EnableServerMarkers)
		{
			for ( i = 0; i < ServerCount(); ++i )
			{
				marker = ServerGet( i );
				if ( marker )
					markers.Insert( marker );
			}
		}

		//! If enabled, will show Quickmarkers (the input is also disabled but it's just in case)
		if ( GetExpansionSettings().GetParty().EnableQuickMarker )
		{
			for ( i = 0; i < PartyQuickCount(); ++i )
			{
				marker = PartyQuickGet( i );
				if ( marker )
					markers.Insert( marker );
			}
		}

		return markers;
	}

	string NextPersonalUID()
	{
		return "" + m_NextPersonalUID;
	}

	int PersonalCount()
	{
		return m_PersonalMarkers.Count();
	}

	ExpansionMarkerData PersonalGet( int idx )
	{
		return m_PersonalMarkers[idx];
	}

	string PersonalGetUID( int idx )
	{
		return m_PersonalMarkers[idx].GetUID();
	}

	int PersonalSetVisibility(  ExpansionMarkerData data, int vis )
	{
		return data.SetVisibility( vis );
	}

	int PersonalRemoveVisibility(  ExpansionMarkerData data, int vis )
	{
		return data.RemoveVisibility( vis );
	}

	int PersonalFlipVisibility(  ExpansionMarkerData data, int vis )
	{
		return data.FlipVisibility( vis );
	}

	void PersonalInsert(  ExpansionMarkerData data )
	{
		m_NextPersonalUID++;
		data.SetUID( NextPersonalUID() );
		m_PersonalMarkers.Insert( data );
	}

	bool PersonalRemove( int idx )
	{
		delete m_PersonalMarkers[idx];
		m_PersonalMarkers.RemoveOrdered( idx );
		return true;
	}

	bool PersonalRemove( string uid )
	{
		for ( int i = 0; i < m_PersonalMarkers.Count(); ++i )
		{
			if ( m_PersonalMarkers[i].GetUID() == uid )
			{
				return PersonalRemove( i );
			}
		}
		return false;
	}

	array< ref ExpansionMarkerData > PersonalGet()
	{
		return m_PersonalMarkers;
	}

	int PartyCount()
	{
		if ( !m_PartyModule || !m_PartyModule.GetParty() )
			return 0;

		return m_PartyModule.GetParty().GetAllMarkers().Count();
	}
	
	ExpansionMarkerData PartyGet( int i )
	{
		return m_PartyModule.GetParty().GetAllMarkers()[i];
	}

	ExpansionMarkerData PartyGet( string uid )
	{
		array< ref ExpansionMarkerData > markers = m_PartyModule.GetParty().GetAllMarkers();
		for ( int i = 0; i < markers.Count(); ++i )
		{
			if ( markers[i].GetUID() == uid )
			{
				return markers[i];
			}
		}

		return NULL;
	}

	int PartySetVisibility( string uid, int vis )
	{
		ExpansionMarkerClientInfo info = m_MarkerInfo_Party.Get( uid );

		if ( info )
		{
			return info.SetVisibility( vis );
		}

		return 0;
	}

	int PartyRemoveVisibility( string uid, int vis )
	{
		ExpansionMarkerClientInfo info = m_MarkerInfo_Party.Get( uid );

		if ( info )
		{
			return info.RemoveVisibility( vis );
		}

		return 0;
	}

	int PartyFlipVisibility( string uid, int vis )
	{
		ExpansionMarkerClientInfo info = m_MarkerInfo_Party.Get( uid );

		if ( info )
		{
			return info.FlipVisibility( vis );
		}

		return 0;
	}

	void PartyInsert(  ExpansionMarkerData data )
	{
		m_PartyModule.CreateMarker( data );
	}

	void PartyUpdate(  ExpansionMarkerData data )
	{
		m_PartyModule.UpdateMarker( data );
	}

	bool PartyRemove( string uid )
	{
		if ( m_PartyModule )
		{
			m_PartyModule.DeleteMarker( uid );
			return true;
		}

		return false;
	}

	int PartyPlayerCount()
	{
		if ( !m_PartyModule || !m_PartyModule.GetParty() )
			return 0;

		return m_PartyModule.GetParty().GetPlayers().Count();
	}
	
	ExpansionPlayerMarkerData PartyPlayerGet( int i )
	{
		return m_PartyModule.GetParty().GetPlayers()[i].Marker;
	}
	
	ExpansionPlayerMarkerData PartyPlayerGet( string uid )
	{
		return m_PartyModule.GetParty().GetPlayer( uid ).Marker;
	}

	int PartyPlayerSetVisibility( string uid, int vis )
	{
		ExpansionMarkerClientInfo info = m_MarkerInfo_PartyPlayers.Get( uid );

		if ( info )
		{
			return info.SetVisibility( vis );
		}

		return 0;
	}

	int PartyPlayerRemoveVisibility( string uid, int vis )
	{
		ExpansionMarkerClientInfo info = m_MarkerInfo_PartyPlayers.Get( uid );

		if ( info )
		{
			return info.RemoveVisibility( vis );
		}

		return 0;
	}

	int PartyPlayerFlipVisibility( string uid, int vis )
	{
		ExpansionMarkerClientInfo info = m_MarkerInfo_PartyPlayers.Get( uid );

		if ( info )
		{
			return info.FlipVisibility( vis );
		}

		return 0;
	}
	
	int ServerCount()
	{
		return GetExpansionSettings().GetMap().ServerMarkers.Count();
	}
	
	ExpansionMarkerData ServerGet( int idx )
	{
		return GetExpansionSettings().GetMap().ServerMarkers[ idx ];
	}
	
	ExpansionMarkerData ServerGet( string uid )
	{
		array< ref ExpansionMarkerData > markers = GetExpansionSettings().GetMap().ServerMarkers;
		for ( int i = 0; i < markers.Count(); ++i )
		{
			if ( markers[i].GetUID() == uid )
			{
				return markers[i];
			}
		}

		return NULL;
	}

	int ServerSetVisibility( string uid, int vis )
	{
		ExpansionMarkerClientInfo info = m_MarkerInfo_Server.Get( uid );

		if ( info )
		{
			return info.SetVisibility( vis );
		}

		return 0;
	}

	int ServerRemoveVisibility( string uid, int vis )
	{
		ExpansionMarkerClientInfo info = m_MarkerInfo_Server.Get( uid );

		if ( info )
		{
			return info.RemoveVisibility( vis );
		}

		return 0;
	}

	int ServerFlipVisibility( string uid, int vis )
	{
		Print("ExpansionMarkerModule - ServerFlipVisibility");
		ExpansionMarkerClientInfo info = m_MarkerInfo_Server.Get( uid );

		if ( info )
		{
			return info.FlipVisibility( vis );
		}

		return 0;
	}

	int PartyQuickCount()
	{
		if ( !m_PartyModule || !m_PartyModule.GetParty() )
			return 0;

		return m_PartyModule.GetParty().GetPlayers().Count();
	}
	
	ExpansionPartyQuickMarkerData PartyQuickGet( int i )
	{
		return m_PartyModule.GetParty().GetPlayers()[i].QuickMarker;
	}

	ExpansionPartyQuickMarkerData PartyQuickGet( string uid )
	{
		return m_PartyModule.GetParty().GetPlayer( uid ).QuickMarker;
	}

	void OnRefresh()
	{
		//! IMPORTANT! We do not want to clear any marker client info if player is not yet loaded
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( !player || !player.IsPlayerLoaded() )
			return;

		RefreshServerMarkers();

		RefreshPartyMarkers();

		RefreshPersonalMarkers();
	}

	private void RefreshServerMarkers()
	{
		int index;
		string uid;
		int removeIndex;

		array< string > checkArr = m_MarkerInfo_Server.GetKeyArray();
		array< ref ExpansionMarkerData > markers = GetExpansionSettings().GetMap().ServerMarkers;

		for ( index = 0; index < markers.Count(); ++index )
		{
			uid = markers[index].GetUID();
			removeIndex = checkArr.Find( uid );
			if ( removeIndex != -1 )
			{
				//! Already have client info for this marker
				checkArr.Remove( removeIndex );
				markers[index].ApplyVisibility( m_MarkerInfo_Server[uid].GetVisibility() );
			} else
			{
				//! It's a new marker
				ExpansionMarkerClientInfo info = new ExpansionMarkerClientInfo( uid );
				m_MarkerInfo_Server.Insert( uid, info );
			}
		}

		for ( index = 0; index < checkArr.Count(); ++index )
		{
			//! Remove client info for markers that have been removed
			m_MarkerInfo_Server.Remove( checkArr[index] );
		}
	}

	private void RefreshPartyMarkers()
	{
		if ( !m_PartyModule || !m_PartyModule.HasParty() )
		{
			if ( m_PartyModule && m_PartyModule.IsClientInitialized() )
			{
				//! Player was in party and has left party, clear party marker info
				//! @note party marker info is *only* synched when changed, so *only* clear if party module is initialized!
				m_MarkerInfo_Party.Clear();
			}

			//! Player was in party and has left party, clear player marker info
			//! @note party player marker info is synched on each update, so it's ok to clear here
			m_MarkerInfo_PartyPlayers.Clear();

			return;
		}

		ExpansionMarkerData data;
		ExpansionMarkerClientInfo info;

		ExpansionPartyData party = m_PartyModule.GetParty();

		if (!party.m_MarkersSynced)
			return;

		int index;
		string uid;
		int removeIndex;

		//! Refresh client info for party markers
		array< string > checkArr = m_MarkerInfo_Party.GetKeyArray();
		array< ref ExpansionMarkerData > markers = party.GetAllMarkers();

		for ( index = 0; index < markers.Count(); ++index )
		{
			uid = markers[index].GetUID();
			removeIndex = checkArr.Find( uid );
			if ( removeIndex != -1 )
			{
				//! Already have client info for this marker
				checkArr.Remove( removeIndex );
				markers[index].ApplyVisibility( m_MarkerInfo_Party[uid].GetVisibility() );
			} else
			{
				//! It's a new marker
				info = new ExpansionMarkerClientInfo( uid );
				m_MarkerInfo_Party.Insert( uid, info );
			}
		}

		for ( index = 0; index < checkArr.Count(); ++index )
		{
			//! Remove client info for markers that have been removed
			m_MarkerInfo_Party.Remove( checkArr[index] );
		}

		//! Refresh client info for party member (player) markers
		checkArr = m_MarkerInfo_PartyPlayers.GetKeyArray();
		array< ref ExpansionPartyPlayerData > players = party.GetPlayers();

		for ( index = 0; index < players.Count(); ++index )
		{
			uid = players[index].UID;
			removeIndex = checkArr.Find( uid );
			if ( removeIndex != -1 )
			{
				//! Already have client info for this marker
				checkArr.Remove( removeIndex );
				if ( players[index].Marker )
					players[index].Marker.ApplyVisibility( m_MarkerInfo_PartyPlayers[uid].GetVisibility() );
			} else
			{
				//! It's a new marker
				info = new ExpansionMarkerClientInfo( uid );
				m_MarkerInfo_PartyPlayers.Insert( uid, info );
			}
		}

		for ( index = 0; index < checkArr.Count(); ++index )
		{
			//! Remove client info for markers that have been removed
			m_MarkerInfo_PartyPlayers.Remove( checkArr[index] );
		}
	}

	private void RefreshPersonalMarkers()
	{
		set< string > uids = new set< string >();
		int count_uids = 0;
		int count_uids_prev = 0;

		int index = m_PersonalMarkers.Count() - 1;
		int count_neg = 0;
		while ( index >= m_PersonalMarkers.Count() )
		{
			count_uids_prev = uids.Count();
			uids.Insert( m_PersonalMarkers[index].GetUID() );
			count_uids = uids.Count();

			if ( count_uids_prev != count_uids )
			{
				count_neg--;
			} else
			{
				m_PersonalMarkers.RemoveOrdered( index );
			}

			index = m_PersonalMarkers.Count() - 1 - count_neg;
		}
	}
}