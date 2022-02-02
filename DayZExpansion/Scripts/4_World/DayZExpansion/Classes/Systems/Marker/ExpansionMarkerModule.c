/**
 * ExpansionMarkerModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule Destructor
	// ------------------------------------------------------------
	void ~ExpansionMarkerModule()
	{
		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMarkerModule::~ExpansionMarkerModule - Start");
		#endif
		
		delete m_3DMarkers;
		
		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMarkerModule::~ExpansionMarkerModule - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule OnInit
	// ------------------------------------------------------------
	override void OnInit()
	{
		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMarkerModule::OnInit - Start");
		#endif
		
		super.OnInit();

		m_3DMarkers = new array< ref Expansion3DMarker>();
		m_AllData = new array< ref ExpansionMarkerClientData>();

		m_CurrentData = NULL;

		//! Initialize all markers one by one
		//! Personal marker
		if ( GetExpansionSettings().GetMap().CanCreateMarker )
			SetVisibility( ExpansionMapMarkerType.PERSONAL, EXPANSION_MARKER_VIS_WORLD | EXPANSION_MARKER_VIS_MAP );

		//! Party marker
		if ( GetExpansionSettings().GetParty().CanCreatePartyMarkers )
			SetVisibility( ExpansionMapMarkerType.PARTY, EXPANSION_MARKER_VIS_WORLD | EXPANSION_MARKER_VIS_MAP );

		//! Server marker
		if ( GetExpansionSettings().GetMap().EnableServerMarkers )
			SetVisibility( ExpansionMapMarkerType.SERVER, EXPANSION_MARKER_VIS_WORLD | EXPANSION_MARKER_VIS_MAP );

		//! Party Member marker
		if ( GetExpansionSettings().GetParty().ShowPartyMember3DMarkers )
		{
			if ( GetExpansionSettings().GetMap().ShowPlayerPosition )
			{
				SetVisibility( ExpansionMapMarkerType.PLAYER, EXPANSION_MARKER_VIS_WORLD | EXPANSION_MARKER_VIS_MAP ); // 2D and 3D
			} else {
				SetVisibility( ExpansionMapMarkerType.PLAYER, EXPANSION_MARKER_VIS_WORLD ); // 3D only
			}
		} else {
			if ( GetExpansionSettings().GetMap().ShowPlayerPosition )
			{
				SetVisibility( ExpansionMapMarkerType.PLAYER, EXPANSION_MARKER_VIS_MAP ); // 2D only
			}
		}

		//! Party Quickmarker
		if ( GetExpansionSettings().GetParty().EnableQuickMarker )
			SetVisibility( ExpansionMapMarkerType.PARTY_QUICK, EXPANSION_MARKER_VIS_WORLD | EXPANSION_MARKER_VIS_MAP );
		
		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMarkerModule::OnInit - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule OnMissionLoaded
	// ------------------------------------------------------------
	override void OnMissionLoaded()
	{
		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMarkerModule::OnMissionLoaded - Start");
		#endif

		super.OnMissionLoaded();

		Class.CastTo( m_PartyModule, GetModuleManager().GetModule( ExpansionPartyModule ) );
		
		ReadLocalServerMarkers();

		if ( IsMissionOffline() )
		{
			if ( m_AllData.Count() <= 0 )
				m_AllData.Insert( new ExpansionMarkerClientData() );

			m_CurrentData = m_AllData[0];
		} else if ( IsMissionClient() )
		{
			string address;
			int port;
			if ( GetGame().GetHostAddress( address, port ) )
			{
				bool found = false;

				for ( int i = 0; i < m_AllData.Count(); ++i )
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

		SaveLocalServerMarkers();

		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMarkerModule::OnMissionLoaded - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule OnMissionFinish
	// ------------------------------------------------------------
	override void OnMissionFinish()
	{
		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMarkerModule::OnMissionFinish - Start");
		#endif

		super.OnMissionFinish();

		SaveLocalServerMarkers();

		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMarkerModule::OnMissionFinish - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule OnSettingsUpdated
	// ------------------------------------------------------------
	override void OnSettingsUpdated()
	{
		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMarkerModule::OnSettingsUpdated - Start");
		#endif

		Refresh();

		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMarkerModule::OnSettingsUpdated - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule SaveLocalServerMarkers
	// ------------------------------------------------------------
	void SaveLocalServerMarkers()
	{
		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMarkerModule::SaveLocalServerMarkers - Start");
		#endif
		
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
		
		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMarkerModule::SaveLocalServerMarkers - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule ReadLocalServerMarkers
	// ------------------------------------------------------------	
	bool ReadLocalServerMarkers()
	{
		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMarkerModule::ReadLocalServerMarkers - Start");
		#endif
		
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
					
					#ifdef EXPANSION_MARKER_MODULE_DEBUG
					EXPrint("ExpansionMarkerModule::ReadLocalServerMarkers - End and return false");
					#endif
					
					return false;
				}

				for ( int j = 0; j < countVis; ++j )
				{
					int vis;
					if ( Expansion_Assert_False( ctx.Read( vis ), "[" + this + "] Failed reading m_Visibility[" + j + "]" ) )
					{
						ctx.Close();
						
						#ifdef EXPANSION_MARKER_MODULE_DEBUG
						EXPrint("ExpansionMarkerModule::ReadLocalServerMarkers - End and return false");
						#endif
						
						return false;
					}

					m_Visibility[j] = vis;
				}
			}

			int countArray;
			if ( Expansion_Assert_False( ctx.Read( countArray ), "[" + this + "] Failed reading countArray" ) )
			{
				ctx.Close();
				
				#ifdef EXPANSION_MARKER_MODULE_DEBUG
				EXPrint("ExpansionMarkerModule::ReadLocalServerMarkers - End and return false");
				#endif
				
				return false;
			}

			if ( version < 8 )
			{
				m_AllData.Insert( new ExpansionMarkerClientData() );
			}

			for ( int u = 0; u < countArray; ++u )
			{
				string ip;
				int port;
				
				if ( Expansion_Assert_False( ctx.Read( ip ), "[" + this + "] Failed reading ip" ) )
				{
					#ifdef EXPANSION_MARKER_MODULE_DEBUG
					EXPrint("ExpansionMarkerModule::ReadLocalServerMarkers - End and return false");
					#endif
					
					return false;
				}
				
				if ( Expansion_Assert_False( ctx.Read( port ), "[" + this + "] Failed reading port" ) )
				{
					#ifdef EXPANSION_MARKER_MODULE_DEBUG
					EXPrint("ExpansionMarkerModule::ReadLocalServerMarkers - End and return false");
					#endif
					
					return false;
				}

				ExpansionMarkerClientData newServer = null;
				for ( int i = 0; i < m_AllData.Count(); ++i )
				{
					if ( m_AllData[i].Equals( ip, port ) )
					{
						newServer = m_AllData[i];
						break;
					}
				}
				
				if ( !newServer )
				{
					newServer = new ExpansionMarkerClientData();
					newServer.m_IP = ip;
					newServer.m_Port = port;
					m_AllData.Insert( newServer );
				}
				
				if ( !newServer.OnStoreLoad( ctx, version ) )
				{
					ctx.Close();
					
					#ifdef EXPANSION_MARKER_MODULE_DEBUG
					EXPrint("ExpansionMarkerModule::ReadLocalServerMarkers - End and return false");
					#endif
					
					return false;
				}
			}
			
			if ( version < 10 )
			{
				for ( int l1 = 1; l1 < m_AllData.Count(); ++l1 )
					for ( int l2 = l1; l2 < m_AllData.Count(); ++l2 )
						if ( m_AllData[l1].Equals( m_AllData[l2] ) )
						{
							m_AllData.Remove(l2);
							l2 -= 1;
						}
			}
		}

		//! Override quick markers, so they are always visible no matter what
		SetVisibility( ExpansionMapMarkerType.PARTY_QUICK, EXPANSION_MARKER_VIS_WORLD | EXPANSION_MARKER_VIS_MAP );

		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMarkerModule::ReadLocalServerMarkers - End and return true");
		#endif
		
		return true;
	}
 	
	// ------------------------------------------------------------
	// ExpansionMarkerModule GetRPCMin
	// ------------------------------------------------------------
	override int GetRPCMin()
	{
		return ExpansionMarkerRPC.INVALID;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule GetRPCMax
	// ------------------------------------------------------------
	override int GetRPCMax()
	{
		return ExpansionMarkerRPC.COUNT;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule OnRPC
	// ------------------------------------------------------------	
	#ifdef CF_BUGFIX_REF
	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	#else
	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	#endif
	{
		switch ( rpc_type )
		{
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule CreateServerMarker
	// ------------------------------------------------------------
	ExpansionMarkerData CreateServerMarker( string name, string icon, vector position, int color, bool marker3D, string uid = "" )
	{
		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMarkerModule::CreateServerMarker - Start");
		#endif
		
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
			{
				#ifdef EXPANSION_MARKER_MODULE_DEBUG
				EXPrint("ExpansionMarkerModule::CreateServerMarker - End and return NULL");
				#endif
				
				return NULL;
			}

			GetExpansionSettings().GetMap().AddServerMarker( marker );
			GetExpansionSettings().GetMap().Send( NULL );

			#ifdef EXPANSION_MARKER_MODULE_DEBUG
			EXPrint("ExpansionMarkerModule::CreateServerMarker - End and return marker: " + marker.ToString());
			#endif
			
			return marker;
		}

		// TODO: send rpc with permission system for creating on the client.
		
		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMarkerModule::CreateServerMarker - End and return NULL");
		#endif
		
		return NULL;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule RemoveServerMarker
	// ------------------------------------------------------------	
	void RemoveServerMarker( string uid )
	{
		if ( !IsMissionHost() || !GetExpansionSettings().GetMap() )		
			return;

		if ( GetExpansionSettings().GetMap().RemoveServerMarker( uid ) )
			GetExpansionSettings().GetMap().Send( NULL );
	}
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule CreateMarker
	// ------------------------------------------------------------
	bool CreateMarker(  ExpansionMarkerData data )
	{
		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMarkerModule::CreateMarker - Start");
		#endif
		
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
		
		#ifdef EXPANSION_MARKER_MODULE_DEBUG
		EXPrint("ExpansionMarkerModule::CreateMarker - End and return");
		#endif

		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule UpdateMarker
	// ------------------------------------------------------------
	bool UpdateMarker(  ExpansionMarkerData data )
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
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule GetData
	// ------------------------------------------------------------
	ExpansionMarkerClientData GetData()
	{
		return m_CurrentData;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule OnUpdate
	// ------------------------------------------------------------	
	override void OnUpdate( float timeslice )
	{
		super.OnUpdate( timeslice );

		if (!IsMissionClient())
			return;

		m_TimeAccumulator += timeslice;
		if ( m_TimeAccumulator > 1.0 )
		{
			Refresh();
			
			m_TimeAccumulator = 0;
		}

		for ( int i = m_3DMarkers.Count() - 1; i >= 0; i-- )
		{
			if ( !m_3DMarkers[i] || !m_3DMarkers[i].Update( timeslice ) )
			{
				Expansion3DMarker marker = m_3DMarkers[i];
				m_3DMarkers.Remove(i);
				delete marker;
			} 
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule CanCreateMarker
	// ------------------------------------------------------------
	private bool CanCreateMarker( ExpansionMarkerData other, out Expansion3DMarker marker )
	{		
		if ( other.GetType() != ExpansionMapMarkerType.PARTY_QUICK )
		{
			if ( !other.Is3D() )
			{
				#ifdef EXPANSION_MARKER_MODULE_DEBUG
				EXPrint("ExpansionMarkerModule::CanCreateMarker - End and return true");
				#endif
				return true;
			}
			
			if ( !other.IsWorldVisible() )
			{
				#ifdef EXPANSION_MARKER_MODULE_DEBUG
				EXPrint("ExpansionMarkerModule::CanCreateMarker - End and return true");
				#endif
				return true;
			}
			if ( !IsWorldVisible( other.GetType() ) )
			{
				#ifdef EXPANSION_MARKER_MODULE_DEBUG
				EXPrint("ExpansionMarkerModule::CanCreateMarker - End and return true");
				#endif
				return true;
			}
		}

		for ( int i = 0; i < m_3DMarkers.Count(); ++i )
		{
			if ( !m_3DMarkers[i] )
				continue;

			ExpansionMarkerData data = m_3DMarkers[i].GetMarkerData();
			if ( data != NULL && data == other )
			{
				marker = m_3DMarkers[i];
				
				#ifdef EXPANSION_MARKER_MODULE_DEBUG
				EXPrint("ExpansionMarkerModule::CanCreateMarker - End and return true");
				#endif
				
				return true;
			}
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule Refresh
	// ------------------------------------------------------------
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

		ExpansionUIScriptedMenu menu;
		if ( Class.CastTo( menu, GetGame().GetUIManager().FindMenu( MENU_EXPANSION_MAP ) ) )
		{
			menu.Refresh();
		}

		// CanCreateMarker != removal
		// CanCreate3DMarker != removal
	}
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule SetVisibility
	// ------------------------------------------------------------
	int SetVisibility(  ExpansionMarkerData data, int vis )
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
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule FlipVisibility
	// ------------------------------------------------------------
	int FlipVisibility(  ExpansionMarkerData data, int vis )
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
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule RemoveVisibility
	// ------------------------------------------------------------
	int RemoveVisibility(  ExpansionMarkerData data, int vis )
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
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule SetVisibility
	// ------------------------------------------------------------
	int SetVisibility( int type, int vis )
	{
		type -= 1;

		m_Visibility[type] = m_Visibility[type] | vis;

		return m_Visibility[type];
	}
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule FlipVisibility
	// ------------------------------------------------------------
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
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule RemoveVisibility
	// ------------------------------------------------------------
	int RemoveVisibility( int type, int vis )
	{
		type -= 1;
		
		m_Visibility[type] = m_Visibility[type] & ~vis;

		return m_Visibility[type];
	}
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule ClearVisibility
	// ------------------------------------------------------------
	int ClearVisibility( int type )
	{
		type -= 1;
		
		m_Visibility[type] = 0;
		return m_Visibility[type];
	}
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule GetVisibility
	// ------------------------------------------------------------
	int GetVisibility( int type )
	{
		type -= 1;
		
		return m_Visibility[type];
	}
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule IsVisible
	// ------------------------------------------------------------
	bool IsVisible( int type )
	{
		type -= 1;
		
		return m_Visibility[type] != 0;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule IsWorldVisible
	// ------------------------------------------------------------
	bool IsWorldVisible( int type )
	{
		type -= 1;

		if ( type == ExpansionMapMarkerType.SERVER && !GetExpansionSettings().GetMap().EnableServerMarkers )
			return false;
		if ( type == ExpansionMapMarkerType.PARTY_QUICK && !GetExpansionSettings().GetParty().EnableQuickMarker )
			return false;
		if ( type == ExpansionMapMarkerType.PLAYER && !GetExpansionSettings().GetParty().ShowPartyMember3DMarkers )
			return false;
		
		return (m_Visibility[type] & EXPANSION_MARKER_VIS_WORLD) != 0;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarkerModule IsMapVisible
	// ------------------------------------------------------------
	bool IsMapVisible( int type )
	{
		type -= 1;

		if ( type == ExpansionMapMarkerType.SERVER && !GetExpansionSettings().GetMap().EnableServerMarkers )
			return false;

		return (m_Visibility[type] & EXPANSION_MARKER_VIS_MAP) != 0;
	}
};