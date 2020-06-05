/**
 * Expansion3DMarkerModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion3DMarkerModule: JMModuleBase
{
    protected autoptr array< ref Expansion3DMarker > 	m_Markers;
	protected bool										m_ShowMarkers;
	
	// ------------------------------------------------------------
	// Expansion3DMarkerModule Constructor
	// ------------------------------------------------------------
    void Expansion3DMarkerModule()
    {
        m_Markers = new array< ref Expansion3DMarker >;
		m_ShowMarkers = true;
    }
	
	void ToggleShowMaarkers()
	{
		m_ShowMarkers = !m_ShowMarkers;
	}
	
	bool GetShowMarkers()
	{
		return m_ShowMarkers;
	}
	
	override bool IsEnabled()
	{
		return (super.IsEnabled() && GetExpansionSettings().GetMap().EnableMap && GetExpansionSettings().GetMap().CanCreate3DMarker);
	}
	
	// ------------------------------------------------------------
	// Override OnSettingsUpdated
	// ------------------------------------------------------------
	override void OnSettingsUpdated()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion3DMarkerModule::OnSettingsUpdated Start");
		#endif
       
		RefreshMarkers();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion3DMarkerModule::OnSettingsUpdated End");
		#endif
    }
	
	// ------------------------------------------------------------
	// Expansion RefreshMarkers
	// ------------------------------------------------------------
    void RefreshMarkers()
    {
        if ( !IsMissionClient() )
            return;
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Expansion3DMarkerModule::RefreshMarkers Start");
		#endif
		
        CreateMarkers();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Expansion3DMarkerModule::RefreshMarkers End");
		#endif
    }
	
	// ------------------------------------------------------------
	// Expansion ClearMarkers
	// ------------------------------------------------------------
    private void ClearMarkers()
    {
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion3DMarkerModule::ClearMarkers - Start");
		#endif
		
        while ( m_Markers.Count() > 0 )
        {
			delete m_Markers.Get(m_Markers.Count() - 1);
            m_Markers.Remove(m_Markers.Count() - 1);
        }

        m_Markers.Clear();
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion3DMarkerModule::ClearMarkers - End");
		#endif
    }
	
	// ------------------------------------------------------------
	// Expansion CreateMarkers
	// ------------------------------------------------------------
    private void CreateMarkers()
    {
        ClearMarkers();

        ExpansionMapMarker marker = null;
        int idx = 0;
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Expansion3DMarkerModule::CreateMarkers Start");
		#endif

        ExpansionMapMarkerModule module;
        if ( !Class.CastTo( module, GetModuleManager().GetModule( ExpansionMapMarkerModule ) ) )
            return;

        if ( !GetExpansionClientSettings().Show3DMarkers )
            return;

        if ( GetExpansionClientSettings().Show3DClientMarkers )
        {
			if ( module.Count() > 0 )
			{
	            for ( idx = 0; idx < module.Count(); idx++ )
	            {
	                marker = module.GetMarker( idx );
	
	                if ( !marker || !marker.Is3DMarker() )
	                    continue;
	
	                Create3DMarker( marker.GetMarkerText(), ExpansionMarkerIcons.GetMarkerPath( marker.GetIconIndex() ), marker.GetMarkerColor(), marker.GetMarkerPosition() );
	            }
			}
        }

        if ( GetExpansionClientSettings().Show3DGlobalMarkers )
        {
			if ( module.ServerCount() > 0 )
			{
	            for ( idx = 0; idx < module.ServerCount(); idx++ )
	            {
	                marker = module.GetServerMarker( idx );
	
	                if ( !marker || !marker.Is3DMarker() )
	                    continue;
	
	                Create3DMarker( marker.GetMarkerText(), ExpansionMarkerIcons.GetMarkerPath( marker.GetIconIndex() ), marker.GetMarkerColor(), marker.GetMarkerPosition() );
	            }
			}
        }

        PlayerBase player;
        string selfUID;
        if ( !Class.CastTo( player, GetGame().GetPlayer() ) || !player.GetIdentity() )
            return;

        selfUID = player.GetIdentityUID();
		vector selfPosition = player.GetPosition();
		
		ref ExpansionPartyModule partyModule = ExpansionPartyModule.Cast(GetModuleManager().GetModule(ExpansionPartyModule));

        if ( GetExpansionClientSettings().Show3DPlayerMarkers && GetExpansionSettings().GetParty().ShowPartyMembers3DMarkers )
        {
			if ( partyModule.HasParty() && partyModule.GetParty() )
			{
	           	ref array< ref ExpansionPartySaveFormatPlayer > players = partyModule.GetParty().GetPlayers();
	            if ( players )
				{
					for ( idx = 0; idx < players.Count(); idx++ )
		            {
						ref ExpansionPartySaveFormatPlayer currPlayer = players[idx];
		                if ( !currPlayer || currPlayer.UID == selfUID ) continue;
		
						PlayerBase partyPlayer = PlayerBase.GetPlayerByUID(currPlayer.UID);
						if (partyPlayer && partyPlayer.IsAlive() && vector.Distance( partyPlayer.GetPosition(), selfPosition) <= GetExpansionSettings().GetMap().DistanceForPartyMarkers)
						{
							//TODO: add it back when exp hit : GetExpansionClientSettings().RedColorHUDOnTopOfHeadOfPlayers
							Create3DMarker( currPlayer.Name, "DayZExpansion\\GUI\\icons\\marker\\marker_mapmarker.paa", ARGB( 255, 255, 180, 24), vector.Zero, partyPlayer, "0 0.25 0", partyPlayer.GetBoneIndexByName( "Head" ) );
						}
		            }
				}
			}
        }

        if ( partyModule.HasParty() && GetExpansionClientSettings().Show3DPartyMarkers )
        {
			array<ref ExpansionMapMarker> partyMarkers = partyModule.GetParty().GetAllMarkers();
			if ( partyMarkers.Count() > 0 )
			{
				for ( idx = 0; idx < partyMarkers.Count(); idx++ )
		        {
					ExpansionMapMarker partyMarker = partyMarkers[idx];
	                if ( !partyMarker.Is3DMarker() )
	                    continue;
	
		            Create3DMarker( partyMarker.GetMarkerText(), ExpansionMarkerIcons.GetMarkerPath( partyMarker.GetIconIndex() ), partyMarker.GetMarkerColor(), partyMarker.GetMarkerPosition() );
				}
			}
        }
				
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Expansion3DMarkerModule::CreateMarkers End");
		#endif
    }
	
	// ------------------------------------------------------------
	// Expansion Create3DMarker
	// ------------------------------------------------------------
    private void Create3DMarker( string name, string iconpath, int color, vector position, Object linkedObject = NULL, vector offset = "0 0 0", int bone = -1 )
    {
        m_Markers.Insert( new Expansion3DMarker( name, iconpath, color, position, linkedObject, offset, bone ) );
    }
	
	// ------------------------------------------------------------
	// Override OnUpdate
	// ------------------------------------------------------------
    override void OnUpdate( float timeslice )
    {
        super.OnUpdate( timeslice );

		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion3DMarkerModule::OnUpdate - Start");
		#endif

		for ( int i = 0; i < m_Markers.Count(); i++ )
		{
			ref Expansion3DMarker marker = m_Markers[i];
			if (!marker)
			{
				m_Markers.RemoveOrdered(i);
				//We do i-- because we need to stay at the same position, because we have removed one marker
				i--;
				delete marker;
			}
			else
			{
				marker.Update( timeslice );
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion3DMarkerModule::OnUpdate - End");
		#endif
    }
}