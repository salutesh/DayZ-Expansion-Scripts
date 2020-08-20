/**
 * ExpansionMarkerModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarkerModule: JMModuleBase
{
	protected ExpansionMarkerClientData m_CurrentData;
	protected ref array< ref ExpansionMarkerClientData > m_AllData;

	protected int m_Visibility[5];
	
	protected ExpansionPartyModule m_PartyModule;

	protected float m_TimeAccumulator = 0;

	private ref array< ref Expansion3DMarker > m_3DMarkers;

	void ~ExpansionMarkerModule()
	{
		delete m_3DMarkers;
	}

	override void OnInit()
	{
		super.OnInit();

		m_3DMarkers = new array< ref Expansion3DMarker>();
		m_AllData = new array< ref ExpansionMarkerClientData>();

		m_CurrentData = NULL;

		SetVisibility( ExpansionMapMarkerType.PERSONAL, EXPANSION_MARKER_VIS_WORLD | EXPANSION_MARKER_VIS_MAP );
		SetVisibility( ExpansionMapMarkerType.PARTY, EXPANSION_MARKER_VIS_WORLD | EXPANSION_MARKER_VIS_MAP );
		SetVisibility( ExpansionMapMarkerType.SERVER, EXPANSION_MARKER_VIS_WORLD | EXPANSION_MARKER_VIS_MAP );
		SetVisibility( ExpansionMapMarkerType.PLAYER, EXPANSION_MARKER_VIS_WORLD | EXPANSION_MARKER_VIS_MAP );
		SetVisibility( ExpansionMapMarkerType.PARTY_QUICK, EXPANSION_MARKER_VIS_WORLD | EXPANSION_MARKER_VIS_MAP );
	}
	
	override void OnMissionLoaded()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMarkerModule::OnSettingsUpdated - Start");
		#endif

		super.OnMissionLoaded();

		Class.CastTo( m_PartyModule, GetModuleManager().GetModule( ExpansionPartyModule ) );
		
		if ( !ReadLocalServerMarkers() )
		{
		}

		if ( m_AllData.Count() <= 0 )
		{
			m_AllData.Insert( new ExpansionMarkerClientData() );
		}

		if ( IsMissionOffline() )
		{
			m_CurrentData = m_AllData[0];
		} else if ( IsMissionClient() )
		{
			string address;
			int port;
			if ( GetGame().GetHostAddress( address, port ) )
			{
				bool found = false;

				for ( int i = 1; i < m_AllData.Count(); ++i )
				{
					if ( m_AllData[i].Equals( address, port ) )
					{
						m_CurrentData = m_AllData[i];

						found = true;
						break;
					}
				}

				if ( !found )
				{
					int index = m_AllData.Insert( new ExpansionMarkerClientData() );
					m_AllData[index].m_IP = address;
					m_AllData[index].m_Port = port;
					m_CurrentData = m_AllData[index];
				}
			} else
			{
				Error( "[ExpansionMarkerModule] Failed to get the host address." );
			}
		}

		Print( m_AllData );
		Print( m_AllData.Count() );
		Print( m_AllData[0] );

		SaveLocalServerMarkers();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMarkerModule::OnSettingsUpdated - Start");
		#endif
	}
	
	override void OnMissionFinish()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMarkerModule::OnMissionFinish - Start");
		#endif

		super.OnMissionFinish();

		SaveLocalServerMarkers();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMarkerModule::OnMissionFinish - Start");
		#endif
	}
	
	override void OnSettingsUpdated()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMarkerModule::OnSettingsUpdated - Start");
		#endif

		Refresh();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMarkerModule::OnSettingsUpdated - End");
		#endif
	}
		
	void SaveLocalServerMarkers()
	{
		FileSerializer file = new FileSerializer;
		
		if ( file.Open( EXPANSION_CLIENT_MARKERS, FileMode.WRITE ) )
		{
			file.Write( EXPANSION_VERSION_MAP_MARKER_SAVE );

			file.Write( 5 );
			file.Write( m_Visibility[0] );
			file.Write( m_Visibility[1] );
			file.Write( m_Visibility[2] );
			file.Write( m_Visibility[3] );
			file.Write( m_Visibility[4] );
			
			file.Write( m_AllData.Count() );
			
			for ( int i = 0; i < m_AllData.Count(); ++i )
				m_AllData[i].OnStoreSave( file );
			
			file.Close();
		}
	}
	
	bool ReadLocalServerMarkers()
	{		
		FileSerializer ctx = new FileSerializer;

		if ( ctx.Open( EXPANSION_CLIENT_MARKERS, FileMode.READ ) )
		{
			int version;
			if ( !ctx.Read( version ) )
			{
				ctx.Close();
				return false;
			}

			if ( version >= 8 )
			{
				int countVis;
				if ( Expansion_Assert_False( ctx.Read( countVis ), "[" + this + "] Failed reading m_Visibility array length" ) )
				{
					ctx.Close();
					return false;
				}

				for ( int j = 0; j < countVis; ++j )
				{
					int vis;
					if ( Expansion_Assert_False( ctx.Read( vis ), "[" + this + "] Failed reading m_Visibility[" + j + "]" ) )
					{
						ctx.Close();
						return false;
					}

					m_Visibility[j] = vis;
				}
			}

			int countArray;
			if ( Expansion_Assert_False( ctx.Read( countArray ), "[" + this + "] Failed reading countArray" ) )
			{
				ctx.Close();
				return false;
			}

			if ( version < 8 )
			{
				m_AllData.Insert( new ExpansionMarkerClientData() );
			}

			for ( int u = 0; u < countArray; ++u )
			{
				ExpansionMarkerClientData newServer = new ExpansionMarkerClientData();
				if ( !newServer.OnStoreLoad( ctx, version ) )
				{
					ctx.Close();
					return false;
				}

				bool found = false;

				for ( int i = 1; i < m_AllData.Count(); ++i )
				{
					if ( newServer.Equals( m_AllData[i] ) )
					{
						found = true;
					}
				}

				if ( found )
				{
					m_AllData.Insert( newServer );

					// TODO: Merge the markers, checking for duplicates within
				} else
				{
					m_AllData.Insert( newServer );
				}
			}
		}

		//! Override quick markers, they are always visible no matter what
		SetVisibility( ExpansionMapMarkerType.PARTY_QUICK, EXPANSION_MARKER_VIS_WORLD | EXPANSION_MARKER_VIS_MAP );

		return true;
	}
   
	override int GetRPCMin()
	{
		return ExpansionMarkerRPC.INVALID;
	}
	
	override int GetRPCMax()
	{
		return ExpansionMarkerRPC.COUNT;
	}
	
	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	{
		switch ( rpc_type )
		{
		}
	}

	ref ExpansionMarkerData CreateServerMarker( string name, string icon, vector position, int color, bool marker3D, string uid = "" )
	{
		if ( uid == "" )
			uid = name + Math.RandomInt( 0, int.MAX );

		ExpansionMarkerData marker = ExpansionMarkerData.Create( ExpansionMapMarkerType.SERVER, uid );

		marker.SetName( name );
		marker.SetIconName( icon );
		marker.SetPosition(position );
		marker.SetColor( color );
		marker.Set3D( marker3D );

		if ( IsMissionHost() )
		{
			if ( !GetExpansionSettings().GetMap() )
				return NULL;

			GetExpansionSettings().GetMap().AddServerMarker( marker );
			GetExpansionSettings().GetMap().Send( NULL );

			return marker;
		}

		// TODO: send rpc with permission system for creating on the client.
		
		return NULL;
	}
	
	void RemoveServerMarker( string uid )
	{
		if ( !IsMissionHost() || !GetExpansionSettings().GetMap() )
			return;

		GetExpansionSettings().GetMap().RemoveServerMarker( uid );
		GetExpansionSettings().GetMap().Send( NULL );
	}

	bool CreateMarker( ref ExpansionMarkerData data )
	{
		int type = data.GetType();
		switch ( type )
		{
		case ExpansionMapMarkerType.PERSONAL:
			m_CurrentData.PersonalInsert( data );
			break;
		case ExpansionMapMarkerType.PARTY:
			m_CurrentData.PartyInsert( data );
			break;
		default:
			Error( "Marker type " + typename.EnumToString( ExpansionMapMarkerType, type ) + " not supported." );
			return false;
		}

		Refresh();

		return true;
	}

	bool UpdateMarker( ref ExpansionMarkerData data )
	{
		int type = data.GetType();
		switch ( type )
		{
		case ExpansionMapMarkerType.PERSONAL:
			// doesn't have to do anything
			break;
		case ExpansionMapMarkerType.PARTY:
			m_CurrentData.PartyUpdate( data );
			break;
		default:
			Error( "Marker type " + typename.EnumToString( ExpansionMapMarkerType, type ) + " not supported." );
			return false;
		}

		Refresh();

		return true;
	}

	ref ExpansionMarkerClientData GetData()
	{
		return m_CurrentData;
	}
	
	override void OnUpdate( float timeslice )
	{
		super.OnUpdate( timeslice );

		m_TimeAccumulator += timeslice;
		if ( m_TimeAccumulator > 1.0 )
		{
			Refresh();
			
			m_TimeAccumulator = 0;
		}

		array< int > removing = new array< int >();
		//Print( m_3DMarkers.Count() );
		for ( int i = 0; i < m_3DMarkers.Count(); ++i )
		{
			if ( !m_3DMarkers[i].Update( timeslice ) )
			{
				//Print( "ExpansionMarkerModule::OnUpdate - Removing - " + m_3DMarkers[i] );

				removing.Insert( i );
			} else
			{
				//Print( "ExpansionMarkerModule::OnUpdate - Keeping - " + m_3DMarkers[i] );
			}
		}

		for ( i = 0; i < removing.Count(); ++i )
		{
			ref Expansion3DMarker marker = m_3DMarkers[i];

			m_3DMarkers.Remove( removing[i] );

			delete marker;
		}
	}

	private bool CanCreateMarker( ExpansionMarkerData other, out Expansion3DMarker marker )
	{
		if ( other.GetType() != ExpansionMapMarkerType.PARTY_QUICK )
		{
			if ( !other.Is3D() )
				return true;
			if ( !other.IsWorldVisible() )
				return true;
			if ( !IsWorldVisible( other.GetType() ) )
				return true;
		}

		for ( int i = 0; i < m_3DMarkers.Count(); ++i )
		{
			ExpansionMarkerData data = m_3DMarkers[i].GetMarkerData();
			if ( data != NULL && data == other )
			{
				marker = m_3DMarkers[i];
				return true;
			}
		}

		return false;
	}

	void Refresh()
	{
		if ( !m_CurrentData )
			return;

		//Print( "ExpansionMarkerModule::Refresh" );

		m_CurrentData.OnRefresh();

		array< ExpansionMarkerData > markers = m_CurrentData.GetAll();
		for ( int i = 0; i < markers.Count(); ++i )
		{
			Expansion3DMarker marker;
			if ( !CanCreateMarker( markers[i], marker ) )
			{
				marker = new Expansion3DMarker( markers[i] );
				m_3DMarkers.Insert( marker );
			}
		}

		UIScriptedMenu menu;
		if ( Class.CastTo( menu, GetGame().GetUIManager().FindMenu( MENU_EXPANSION_MAP ) ) )
		{
			menu.Refresh();
		}
	}

	int SetVisibility( ref ExpansionMarkerData data, int vis )
	{
		int type = data.GetType();

		switch ( type )
		{
		case ExpansionMapMarkerType.PERSONAL:
			return m_CurrentData.PersonalSetVisibility( data, vis );
		case ExpansionMapMarkerType.PARTY:
			return m_CurrentData.PartySetVisibility( data.GetUID(), vis );
		case ExpansionMapMarkerType.PLAYER:
			return m_CurrentData.PartyPlayerSetVisibility( data.GetUID(), vis );
		case ExpansionMapMarkerType.SERVER:
			return m_CurrentData.ServerSetVisibility( data.GetUID(), vis );
		}

		return 0;
	}

	int FlipVisibility( ref ExpansionMarkerData data, int vis )
	{
		int type = data.GetType();
		
		switch ( type )
		{
		case ExpansionMapMarkerType.PERSONAL:
			return m_CurrentData.PersonalFlipVisibility( data, vis );
		case ExpansionMapMarkerType.PARTY:
			return m_CurrentData.PartyFlipVisibility( data.GetUID(), vis );
		case ExpansionMapMarkerType.PLAYER:
			return m_CurrentData.PartyPlayerFlipVisibility( data.GetUID(), vis );
		case ExpansionMapMarkerType.SERVER:
			return m_CurrentData.ServerFlipVisibility( data.GetUID(), vis );
		}

		return 0;
	}

	int RemoveVisibility( ref ExpansionMarkerData data, int vis )
	{
		int type = data.GetType();
		
		switch ( type )
		{
		case ExpansionMapMarkerType.PERSONAL:
			return m_CurrentData.PersonalRemoveVisibility( data, vis );
		case ExpansionMapMarkerType.PARTY:
			return m_CurrentData.PartyRemoveVisibility( data.GetUID(), vis );
		case ExpansionMapMarkerType.PLAYER:
			return m_CurrentData.PartyPlayerRemoveVisibility( data.GetUID(), vis );
		case ExpansionMapMarkerType.SERVER:
			return m_CurrentData.ServerRemoveVisibility( data.GetUID(), vis );
		}

		return 0;
	}

	int SetVisibility( int type, int vis )
	{
		type -= 1;

		m_Visibility[type] = m_Visibility[type] | vis;

		return m_Visibility[type];
	}

	int FlipVisibility( int type, int vis )
	{
		type -= 1;

		if ( ( m_Visibility[type] & vis ) != 0 )
		{
			m_Visibility[type] = m_Visibility[type] & ~vis;
		} else
		{
			m_Visibility[type] = m_Visibility[type] | vis;
		}

		return m_Visibility[type];
	}

	int RemoveVisibility( int type, int vis )
	{
		type -= 1;
		
		m_Visibility[type] = m_Visibility[type] & ~vis;

		return m_Visibility[type];
	}

	int ClearVisibility( int type )
	{
		type -= 1;
		
		m_Visibility[type] = 0;
		return m_Visibility[type];
	}

	int GetVisibility( int type )
	{
		type -= 1;
		
		return m_Visibility[type];
	}

	bool IsVisible( int type )
	{
		type -= 1;
		
		return m_Visibility[type] != 0;
	}

	bool IsWorldVisible( int type )
	{
		type -= 1;
		
		return (m_Visibility[type] & EXPANSION_MARKER_VIS_WORLD) != 0;
	}

	bool IsMapVisible( int type )
	{
		type -= 1;
		
		return (m_Visibility[type] & EXPANSION_MARKER_VIS_MAP) != 0;
	}
}