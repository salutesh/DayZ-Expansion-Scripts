/**
 * ExpansionMapMarkerModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionLocalServerMarkers
{
	string m_IP;
	int m_Port;
	autoptr array< ref ExpansionMapMarker > m_Markers;
	
	void ExpansionLocalServerMarkers()
	{
		m_Markers = new array< ref ExpansionMapMarker >;
	}
	
	private int Count()
	{
		int nmb = 0;
		
		for (int i = 0; i < m_Markers.Count(); ++i)
		{
			if (!m_Markers[i])
				continue;
			
			nmb++;
		}
		
		return nmb;
	}
	
	void OnStoreSave(FileSerializer file)
	{
		file.Write(m_IP);
		file.Write(m_Port);
		
		file.Write(Count());
		
		for (int i = 0; i < m_Markers.Count(); ++i)
		{
			if (!m_Markers[i])
				continue;
			
			m_Markers[i].OnStoreSave(file);
		}
	}
	
	bool OnStoreLoad(FileSerializer file, int version)
	{
		if (!file.Read(m_IP))
			return false;
		
		if (!file.Read(m_Port))
			return false;
		
		int count;
		if (!file.Read(count))
			return false;
		
		for (int i = 0; i < count; ++i)
		{
			ExpansionMapMarker newMarker = new ExpansionMapMarker("", -1, "0 0 0", -1, false);
			if (!newMarker.OnStoreLoad(file, version))
				return false;
			
			m_Markers.Insert(newMarker);
		}
		
		return true;
	}
}


class ExpansionMapMarkerModule: JMModuleBase
{
	protected autoptr array< ref ExpansionLocalServerMarkers > m_LocalServersMarkers;
	protected autoptr ExpansionLocalServerMarkers m_Markers;
	
	protected autoptr array< ref ExpansionMapMarker > m_ServerMarkers;
	
	protected static autoptr JsonSerializer m_Serializer = new JsonSerializer;
	
	// ------------------------------------------------------------
	// ExpansionMapMarkerModule Constructor
	// ------------------------------------------------------------
	void ExpansionMapMarkerModule()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapMarkerModule::ExpansionMapMarkerModule - Start");
		#endif
	   
		m_LocalServersMarkers = new array<ref ExpansionLocalServerMarkers>;
		m_Markers = null;
		m_ServerMarkers = new array< ref ExpansionMapMarker >;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapMarkerModule::ExpansionMapMarkerModule - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarkerModule OnMissionLoaded
	// ------------------------------------------------------------
	override void OnMissionLoaded()
	{
		super.OnMissionLoaded();
		
		if ( IsMissionHost() && GetExpansionSettings() && GetExpansionSettings().GetMap() && GetExpansionSettings().GetMap().ServerMarkers )
		{
			m_ServerMarkers = GetExpansionSettings().GetMap().ServerMarkers;
		}
		
		if (FileExist(EXPANSION_FOLDER + "markers.json"))
		{
			JsonFileLoader<ref array< ref ExpansionLocalServerMarkers >>.JsonLoadFile( EXPANSION_FOLDER + "markers.json", m_LocalServersMarkers );
			
			DeleteFile( EXPANSION_FOLDER + "markers.json" );
		}
		else if (FileExist(EXPANSION_CLIENT_MARKERS))
		{
			LoadClientMarkers();
		}
		
		SaveClientMarkers();
		
		GetCurrentLocalServerMarkers();
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarkerModule OnSettingsUpdated
	// ------------------------------------------------------------	
	override void OnSettingsUpdated()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapMarkerModule::OnSettingsUpdated - Start");
		#endif
		if ( GetExpansionSettings() && GetExpansionSettings().GetMap() && GetExpansionSettings().GetMap().ServerMarkers )
		{
			m_ServerMarkers = GetExpansionSettings().GetMap().ServerMarkers;
		}
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapMarkerModule::OnSettingsUpdated - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarkerModule GetCurrentLocalServerMarkers
	// ------------------------------------------------------------
	protected void GetCurrentLocalServerMarkers()
	{
		if (!GetGame().IsMultiplayer())
			return;
		
		string addr;
		int port;
		if ( !GetGame().GetHostAddress( addr, port ) )
			return;
		
		for (int i = 0; i < m_LocalServersMarkers.Count(); ++i)
		{
			ref ExpansionLocalServerMarkers currLocalServer = m_LocalServersMarkers[i];
			
			if (currLocalServer && currLocalServer.m_IP == addr && currLocalServer.m_Port == port)
			{
				m_Markers = currLocalServer;
				return;
			}
		}
		
		ExpansionLocalServerMarkers newServerMarkers = new ExpansionLocalServerMarkers;
		newServerMarkers.m_IP = addr;
		newServerMarkers.m_Port = port;
		
		m_LocalServersMarkers.Insert(newServerMarkers);
		
		m_Markers = newServerMarkers;
		
		SaveClientMarkers();
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarkerModule CountServerMarkers
	// ------------------------------------------------------------	
	private int CountServerMarkers()
	{
		int nmb = 0;
		
		for (int i = 0; i < m_LocalServersMarkers.Count(); ++i)
		{
			if (!m_LocalServersMarkers[i])
				continue;
			
			nmb++;
		}
		
		return nmb;
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarkerModule SaveClientMarkers
	// ------------------------------------------------------------	
	void SaveClientMarkers()
	{
		FileSerializer file = new FileSerializer;
		
		if (file.Open(EXPANSION_CLIENT_MARKERS, FileMode.WRITE))
		{
			file.Write(EXPANSION_VERSION_CURRENT_SAVE);
			
			file.Write(CountServerMarkers());
			
			for (int i = 0; i < m_LocalServersMarkers.Count(); ++i)
			{
				if (!m_LocalServersMarkers[i])
					continue;
				
				m_LocalServersMarkers[i].OnStoreSave(file);
			}
			
			file.Close();
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarkerModule LoadClientMarkers
	// ------------------------------------------------------------	
	private bool LoadClientMarkers()
	{
		if ( m_LocalServersMarkers.Count() )
        {
            // Clear the array so it doesn't grow in size each time
            m_LocalServersMarkers.Clear();
        }

        // Remove any duplicates that were in existing file
        autoptr array< ref ExpansionLocalServerMarkers > seenServersMarkers = new array<ref ExpansionLocalServerMarkers>;
        bool seen;
        for ( int i = m_LocalServersMarkers.Count() - 1; i >= 0; i-- )
        {
            if ( !m_LocalServersMarkers[i] )
				return false;

            seen = false;
            for ( int j = 0; j < seenServersMarkers.Count(); j++ )
            {
                if ( m_LocalServersMarkers[i].m_IP == seenServersMarkers[j].m_IP && m_LocalServersMarkers[i].m_Port == seenServersMarkers[j].m_Port )
                {
                    seen = true;
                    break;
                }
            }
            if ( seen )
            {
                m_LocalServersMarkers.Remove(i);
            }
            else
            {
                seenServersMarkers.Insert( m_LocalServersMarkers[i] );
            }
        }

        return true;
    }

	// ------------------------------------------------------------
	// Expansion AddServerMarker
	// ------------------------------------------------------------
	//return the index where the server marker has been added, so can be removed later
	// -1 is returned when failed, or index of inserted marker when not failed
	int AddServerMarker( string name, int iconIndex, vector position, int color, bool marker3D = false )
	{
		if ( !IsMissionHost() )
		{
			return -1;
		}

		int index = m_ServerMarkers.Insert( new ExpansionMapMarker( name, iconIndex, position, color, false, marker3D ) );
		SyncServerMarkers();

		return index;
	}
	
	// ------------------------------------------------------------
	// Expansion RemoveServerMarker
	// ------------------------------------------------------------
	void RemoveServerMarker( int index )
	{
	   	if ( !IsMissionHost() )
		{
			return;
		}

		if ( m_ServerMarkers.IsValidIndex( index ) )
		{
			m_ServerMarkers.Remove( index );
			SyncServerMarkers();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion OnInvokeConnect
	// ------------------------------------------------------------
	override void OnInvokeConnect( PlayerBase player, PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionPartyModule::OnInvokeConnect - Start" );
		#endif

		SyncServerMarkers( identity );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPartyModule::OnInvokeConnect - End");
		#endif
	}
   
	// ------------------------------------------------------------
	// Override GetRPCMin
	// ------------------------------------------------------------
	override int GetRPCMin()
	{
		return ExpansionMarkerRPC.INVALID;
	}
	
	// ------------------------------------------------------------
	// Override GetRPCMax
	// ------------------------------------------------------------
	override int GetRPCMax()
	{
		return ExpansionMarkerRPC.COUNT;
	}
	
	// ------------------------------------------------------------
	// Override OnRPC
	// ------------------------------------------------------------
	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapMarkerModule::OnRPC - Start");
		#endif
		switch ( rpc_type )
		{
		case ExpansionMarkerRPC.SyncServerMarkers:
			RPC_SyncServerMarkers( ctx, sender, target );
			break;
		}
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapMarkerModule::OnRPC - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion SyncServerMarkers
	// ------------------------------------------------------------
	void SyncServerMarkers()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapMarkerModule::SyncServerMarkers - Start");
		#endif

		if ( IsMissionHost() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( m_ServerMarkers );
			rpc.Send( NULL, ExpansionMarkerRPC.SyncServerMarkers, true, NULL );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapMarkerModule::SyncServerMarkers - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion SyncServerMarkers
	// ------------------------------------------------------------
	void SyncServerMarkers( PlayerIdentity playerIdent )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapMarkerModule::SyncServerMarkers - Start");
		#endif

		if ( IsMissionHost() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( m_ServerMarkers );
			rpc.Send( NULL, ExpansionMarkerRPC.SyncServerMarkers, true, playerIdent );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapMarkerModule::SyncServerMarkers - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_SyncServerMarkers
	// ------------------------------------------------------------
	private void RPC_SyncServerMarkers( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapMarkerModule::RPC_SyncServerMarkers - Start");
		#endif
		if ( !ctx.Read( m_ServerMarkers ) )
		{
			#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapMarkerModule::RPC_SyncServerMarkers - End");
		#endif
			return;
		}

		Expansion3DMarkerModule module;
		if ( Class.CastTo( module, GetModuleManager().GetModule( Expansion3DMarkerModule ) ) )
		{
			module.RefreshMarkers();
		}

		if ( GetGame().GetUIManager().IsMenuOpen( MENU_EXPANSION_MAP ) )
		{
			UIScriptedMenu mapM = GetGame().GetUIManager().FindMenu( MENU_EXPANSION_MAP );
			if ( mapM )
			{
				mapM.Refresh();
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapMarkerModule::RPC_SyncServerMarkers - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Count
	// ------------------------------------------------------------ 
	int ServerCount()
	{
		return m_ServerMarkers.Count();
	}
	
	// ------------------------------------------------------------
	// Expansion GetMarker
	// ------------------------------------------------------------ 
	ExpansionMapMarker GetServerMarker( int idx )
	{
		return m_ServerMarkers.Get( idx );
	}
	
	// ------------------------------------------------------------
	// Expansion Count
	// ------------------------------------------------------------ 
	int Count()
	{
		if (!m_Markers)
			return 0;
		
		return m_Markers.m_Markers.Count();
	}

	// ------------------------------------------------------------
	// Expansion GetMarker
	// ------------------------------------------------------------ 
   	ref ExpansionMapMarker GetMarker( int idx )
	{
		if (!m_Markers)
			return null;
		
		return m_Markers.m_Markers.Get( idx );
	}
	
	// ------------------------------------------------------------
	// Expansion GetMarkers
	// ------------------------------------------------------------ 
	autoptr array< ref ExpansionMapMarker > GetMarkers()
	{
		if (!m_Markers)
			return null;
		
		return m_Markers.m_Markers;
	}
	
	// ------------------------------------------------------------
	// Expansion AddMarker
	// ------------------------------------------------------------ 
	void AddMarker( ref ExpansionMapMarker marker )
	{
		if (!m_Markers)
			GetCurrentLocalServerMarkers();
		
		m_Markers.m_Markers.Insert(marker);
		
		SaveClientMarkers();
	}
	
	// ------------------------------------------------------------
	// Expansion RemoveMarker
	// ------------------------------------------------------------ 
	bool RemoveMarker( ref ExpansionMapMarker marker )
	{
		if (!m_Markers)
			GetCurrentLocalServerMarkers();
		
		int index = m_Markers.m_Markers.Find(marker);

		if ( index > -1 )
		{
			m_Markers.m_Markers.Remove( index );
			SaveClientMarkers();
			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion CreateClientMarker
	// ------------------------------------------------------------ 
	ExpansionMapMarker CreateClientMarker( string name, int iconIndex, vector position, int color, bool partyMarker, bool marker3D, Object linkedObject = NULL )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapMarkerModule::CreateClientMarker - Start");
		#endif
		ExpansionMapMarker marker;
		if ( partyMarker )
		{
			ExpansionPartyModule module;
			if ( Class.CastTo( module, GetModuleManager().GetModule( ExpansionPartyModule ) ) )
			{
				marker = new ExpansionMapMarker( name, iconIndex, position, color, true, marker3D );
				module.CreateMarker( marker );
			}
		} 
		else
		{
			marker = new ExpansionMapMarker( name, iconIndex, position, color, false, marker3D );
			AddMarker( marker );
			
			UIScriptedMenu mapM = GetGame().GetUIManager().FindMenu( MENU_EXPANSION_MAP );
			if ( mapM )
			{
				mapM.Refresh();
			}
			
			if ( marker3D )
			{
				Expansion3DMarkerModule marker3Dmodule;
				if ( Class.CastTo( marker3Dmodule, GetModuleManager().GetModule( Expansion3DMarkerModule ) ) )
				{
					marker3Dmodule.RefreshMarkers();
				}
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapMarkerModule::CreateClientMarker - End");
		#endif
		return marker;
	}
}