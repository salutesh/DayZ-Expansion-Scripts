/**
 * ExpansionMarkerClientData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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

	ref array< ref ExpansionMarkerClientInfo > m_MarkerInfo_Server;

	protected int m_PartyUID;
	ref array< ref ExpansionMarkerClientInfo > m_MarkerInfo_Party;
	ref array< ref ExpansionMarkerClientInfo > m_MarkerInfo_PartyPlayers;
	
	protected ExpansionMarkerModule m_MarkerModule;
	protected ExpansionPartyModule m_PartyModule;

	void ExpansionMarkerClientData()
	{
		Class.CastTo( m_MarkerModule, GetModuleManager().GetModule( ExpansionMarkerModule ) );
		Class.CastTo( m_PartyModule, GetModuleManager().GetModule( ExpansionPartyModule ) );

		m_PersonalMarkers = new array< ref ExpansionMarkerData >();

		m_MarkerInfo_Server = new array< ref ExpansionMarkerClientInfo >();
		m_MarkerInfo_Party = new array< ref ExpansionMarkerClientInfo >();
		m_MarkerInfo_PartyPlayers = new array< ref ExpansionMarkerClientInfo >();
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
			m_MarkerInfo_Server[i].OnStoreSave( file );
		}

		file.Write( m_PartyUID );
		file.Write( m_MarkerInfo_Party.Count() );
		for ( i = 0; i < m_MarkerInfo_Party.Count(); ++i )
		{
			m_MarkerInfo_Party[i].OnStoreSave( file );
		}

		file.Write( m_MarkerInfo_PartyPlayers.Count() );
		for ( i = 0; i < m_MarkerInfo_PartyPlayers.Count(); ++i )
		{
			m_MarkerInfo_PartyPlayers[i].OnStoreSave( file );
		}
	}
	
	bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( Expansion_Assert_False( ctx.Read( m_IP ), "[" + this + "] Failed reading m_IP" ) )
			return false;
		if ( Expansion_Assert_False( ctx.Read( m_Port ), "[" + this + "] Failed reading m_Port" ) )
			return false;

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

				ExpansionMarkerData newMarker = ExpansionMarkerData.Create( ExpansionMapMarkerType.PERSONAL, uid );
				if ( !newMarker.OnStoreLoad( ctx, version ) )
					return false;
				
				m_PersonalMarkers.Insert( newMarker );
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
				
				m_MarkerInfo_Server.Insert( newMarkerInfo );
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
				
				m_MarkerInfo_Party.Insert( newMarkerInfo );
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
				
				m_MarkerInfo_PartyPlayers.Insert( newMarkerInfo );
			}
		} else
		{
			return false;
		}
		
		return true;
	}

	bool Equals( ExpansionMarkerClientData other )
	{
		return m_IP != other.m_IP && m_Port != other.m_Port;
	}

	bool Equals( string ip, int port )
	{
		return ip == m_IP && port == m_Port;
	}

	ref array< ExpansionMarkerData > GetAll()
	{
		ref array< ExpansionMarkerData > markers = new array< ExpansionMarkerData >();
		
		ExpansionMarkerData marker;
		
		int i = 0;

		for ( i = 0; i < PersonalCount(); ++i )
		{
			marker = PersonalGet( i );
			if ( marker )
				markers.Insert( marker );
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

		for ( i = 0; i < PartyPlayerCount(); ++i )
		{
			marker = PartyPlayerGet( i );
			if ( marker && marker.GetUID() != localUid )
				markers.Insert( marker );
		}

		for ( i = 0; i < ServerCount(); ++i )
		{
			marker = ServerGet( i );
			if ( marker )
				markers.Insert( marker );
		}

		for ( i = 0; i < PartyQuickCount(); ++i )
		{
			marker = PartyQuickGet( i );
			if ( marker )
				markers.Insert( marker );
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

	ref ExpansionMarkerData PersonalGet( int idx )
	{
		return m_PersonalMarkers[idx];
	}

	string PersonalGetUID( int idx )
	{
		return m_PersonalMarkers[idx].GetUID();
	}

	int PersonalSetVisibility( ref ExpansionMarkerData data, int vis )
	{
		return data.SetVisibility( vis );
	}

	int PersonalRemoveVisibility( ref ExpansionMarkerData data, int vis )
	{
		return data.RemoveVisibility( vis );
	}

	int PersonalFlipVisibility( ref ExpansionMarkerData data, int vis )
	{
		return data.FlipVisibility( vis );
	}

	void PersonalInsert( ref ExpansionMarkerData data )
	{
		m_NextPersonalUID++;
		data.SetUID( NextPersonalUID() );
		m_PersonalMarkers.Insert( data );
	}

	bool PersonalRemove( int idx )
	{
		delete m_PersonalMarkers[idx];
		m_PersonalMarkers.Remove( idx );
		return true;
	}

	bool PersonalRemove( string uid )
	{
		for ( int i = 0; i < m_PersonalMarkers.Count(); ++i )
		{
			if ( m_PersonalMarkers[i].GetUID() == uid )
			{
				delete m_PersonalMarkers[i];
				m_PersonalMarkers.Remove( i );
				return true;
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
	
	ref ExpansionMarkerData PartyGet( int i )
	{
		return m_PartyModule.GetParty().GetAllMarkers()[i];
	}

	ref ExpansionMarkerData PartyGet( string uid )
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
		for ( int i = 0; i < m_MarkerInfo_Party.Count(); ++i )
		{
			if ( m_MarkerInfo_Party[i].GetUID() == uid )
			{
				return m_MarkerInfo_Party[i].SetVisibility( vis );
			}
		}

		return 0;
	}

	int PartyRemoveVisibility( string uid, int vis )
	{
		for ( int i = 0; i < m_MarkerInfo_Party.Count(); ++i )
		{
			if ( m_MarkerInfo_Party[i].GetUID() == uid )
			{
				return m_MarkerInfo_Party[i].RemoveVisibility( vis );
			}
		}

		return 0;
	}

	int PartyFlipVisibility( string uid, int vis )
	{
		for ( int i = 0; i < m_MarkerInfo_Party.Count(); ++i )
		{
			if ( m_MarkerInfo_Party[i].GetUID() == uid )
			{
				return m_MarkerInfo_Party[i].FlipVisibility( vis );
			}
		}

		return 0;
	}

	void PartyInsert( ref ExpansionMarkerData data )
	{
		m_PartyModule.CreateMarker( data );
	}

	void PartyUpdate( ref ExpansionMarkerData data )
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
	
	ref ExpansionPlayerMarkerData PartyPlayerGet( int i )
	{
		return m_PartyModule.GetParty().GetPlayers()[i].Marker;
	}
	
	ref ExpansionPlayerMarkerData PartyPlayerGet( string uid )
	{
		return m_PartyModule.GetParty().GetPlayer( uid ).Marker;
	}

	int PartyPlayerSetVisibility( string uid, int vis )
	{
		for ( int i = 0; i < m_MarkerInfo_PartyPlayers.Count(); ++i )
		{
			if ( m_MarkerInfo_PartyPlayers[i].GetUID() == uid )
			{
				return m_MarkerInfo_PartyPlayers[i].SetVisibility( vis );
			}
		}

		return 0;
	}

	int PartyPlayerRemoveVisibility( string uid, int vis )
	{
		for ( int i = 0; i < m_MarkerInfo_PartyPlayers.Count(); ++i )
		{
			if ( m_MarkerInfo_PartyPlayers[i].GetUID() == uid )
			{
				return m_MarkerInfo_PartyPlayers[i].RemoveVisibility( vis );
			}
		}

		return 0;
	}

	int PartyPlayerFlipVisibility( string uid, int vis )
	{
		for ( int i = 0; i < m_MarkerInfo_PartyPlayers.Count(); ++i )
		{
			if ( m_MarkerInfo_PartyPlayers[i].GetUID() == uid )
			{
				return m_MarkerInfo_PartyPlayers[i].FlipVisibility( vis );
			}
		}

		return 0;
	}
	
	int ServerCount()
	{
		return GetExpansionSettings().GetMap().ServerMarkers.Count();
	}
	
	ref ExpansionMarkerData ServerGet( int idx )
	{
		return GetExpansionSettings().GetMap().ServerMarkers[ idx ];
	}
	
	ref ExpansionMarkerData ServerGet( string uid )
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
		for ( int i = 0; i < m_MarkerInfo_Server.Count(); ++i )
		{
			if ( m_MarkerInfo_Server[i].GetUID() == uid )
			{
				return m_MarkerInfo_Server[i].SetVisibility( vis );
			}
		}

		return 0;
	}

	int ServerRemoveVisibility( string uid, int vis )
	{
		for ( int i = 0; i < m_MarkerInfo_Server.Count(); ++i )
		{
			if ( m_MarkerInfo_Server[i].GetUID() == uid )
			{
				return m_MarkerInfo_Server[i].RemoveVisibility( vis );
			}
		}

		return 0;
	}

	int ServerFlipVisibility( string uid, int vis )
	{
		for ( int i = 0; i < m_MarkerInfo_Server.Count(); ++i )
		{
			if ( m_MarkerInfo_Server[i].GetUID() == uid )
			{
				return m_MarkerInfo_Server[i].FlipVisibility( vis );
			}
		}

		return 0;
	}

	int PartyQuickCount()
	{
		if ( !m_PartyModule || !m_PartyModule.GetParty() )
			return 0;

		return m_PartyModule.GetParty().GetPlayers().Count();
	}
	
	ref ExpansionPartyQuickMarkerData PartyQuickGet( int i )
	{
		return m_PartyModule.GetParty().GetPlayers()[i].QuickMarker;
	}
	
	ref ExpansionPartyQuickMarkerData PartyQuickGet( string uid )
	{
		return m_PartyModule.GetParty().GetPlayer( uid ).QuickMarker;
	}

	void OnRefresh()
	{
		RefreshServerMarkers();

		RefreshPartyMarkers();

		RefreshPersonalMarkers();
	}

	private void RefreshServerMarkers()
	{
		if ( !m_PartyModule || !m_PartyModule.HasParty() )
			return;
		
		int count = m_MarkerInfo_Server.Count();
		int index = 0;

		for ( index = 0; index < count; ++index )
		{
			ExpansionMarkerData data = ServerGet( m_MarkerInfo_Server[index].GetUID() );
			if ( data )
			{
				data.ApplyVisibility( m_MarkerInfo_Server[index].GetVisibility() );
			}
		}

		m_MarkerInfo_Server.Clear();

		array< ref ExpansionMarkerData > markers = GetExpansionSettings().GetMap().ServerMarkers;

		count = markers.Count();
		for ( index = 0; index < count; ++index )
		{
			ref ExpansionMarkerClientInfo info = new ExpansionMarkerClientInfo( markers[index].GetUID() );
			m_MarkerInfo_Server.Insert( info );
		}
	}

	private void RefreshPartyMarkers()
	{
		if ( !m_PartyModule || !m_PartyModule.HasParty() )
		{
			m_MarkerInfo_Party.Clear();
			m_MarkerInfo_PartyPlayers.Clear();
			
			return;
		}

		ExpansionMarkerData data;
		ref ExpansionMarkerClientInfo info;

		ExpansionPartyData party = m_PartyModule.GetParty();

		int count = m_MarkerInfo_Party.Count();
		int index = 0;

		for ( index = 0; index < count; ++index )
		{
			data = PartyGet( m_MarkerInfo_Party[index].GetUID() );
			if ( data )
			{
				data.ApplyVisibility( m_MarkerInfo_Party[index].GetVisibility() );
			}
		}

		count = m_MarkerInfo_PartyPlayers.Count();

		for ( index = 0; index < count; ++index )
		{
			data = PartyPlayerGet( m_MarkerInfo_PartyPlayers[index].GetUID() );
			if ( data )
			{
				data.ApplyVisibility( m_MarkerInfo_PartyPlayers[index].GetVisibility() );
			}
		}
		
		m_MarkerInfo_Party.Clear();
		m_MarkerInfo_PartyPlayers.Clear();

		array< ref ExpansionMarkerData > markers = party.GetAllMarkers();

		count = markers.Count();
		for ( index = 0; index < count; ++index )
		{
			info = new ExpansionMarkerClientInfo( markers[index].GetUID() );
			m_MarkerInfo_Party.Insert( info );
		}

		array< ref ExpansionPartyPlayerData > players = party.GetPlayers();

		count = players.Count();
		for ( index = 0; index < count; ++index )
		{
			info = new ExpansionMarkerClientInfo( players[index].UID );
			m_MarkerInfo_PartyPlayers.Insert( info );
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