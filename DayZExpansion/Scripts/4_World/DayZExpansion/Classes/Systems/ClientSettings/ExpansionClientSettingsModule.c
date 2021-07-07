/**
 * ExpansionClientSettingsModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionClientSettingsModule: JMModuleBase
{	
	static ref ScriptInvoker m_SettingChanged = new ScriptInvoker;
	
	private bool m_ColorGrading;
	private bool m_ColorVignette;
	
	private float m_Distance;
	private vector m_Offset;
	
	override bool IsServer()
	{
		return false;
	}
	
	override bool IsClient()
	{
		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionClientSettingsModule Constructor
	// ------------------------------------------------------------	
	void ExpansionClientSettingsModule()
	{
		
	}
	
	// ------------------------------------------------------------
	// ExpansionClientSettingsModule OnInvokeConnect
	// ------------------------------------------------------------
	override void OnInvokeConnect( PlayerBase player, PlayerIdentity identity )
	{
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXPrint( "ExpansionClientSettingsModule::OnInvokeConnect - Start" );
		#endif

		Print("ExpansionClientSettingsModule::OnInvokeConnect player : " + player);
		
		if ( player )
		{
			GetExpansionClientSettings().Load();
		}
			
			
		m_ColorGrading = GetExpansionClientSettings().ColorGrading;
		m_ColorVignette = GetExpansionClientSettings().ColorVignette;
		m_Distance = GetTransportCameraDistance();
		m_Offset = GetTransportCameraOffset();
		
		UpdateColorGradingFromSettings();
		UpdateColorVignetteFromSettings();
		UpdateMarkersFromSettings();	
		
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXPrint("ExpansionClientSettingsModule::OnInvokeConnect - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override OnClientLogout
	// ------------------------------------------------------------
	override void OnClientLogout( PlayerBase player, PlayerIdentity identity, int logoutTime, bool authFailed )
	{
		if ( player )
		{
			GetExpansionClientSettings().Save();
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionClientSettingsModule OnSettingsUpdated
	// ------------------------------------------------------------
	override void OnSettingsUpdated()
	{
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXPrint( "ExpansionClientSettingsModule::OnSettingsUpdated - Start" );
		#endif
		
		UpdateColorGradingFromSettings();
		UpdateColorVignetteFromSettings();
		UpdateMarkersFromSettings();
		UpdateCameraFromSettings();
		
		#ifdef EXPANSION_CLIENT_SETTINGS_DEBUG
		EXPrint( "ExpansionClientSettingsModule::OnSettingsUpdated - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionClientSettingsModule UpdateColorGradingFromSettings
	// ------------------------------------------------------------
	private void UpdateColorGradingFromSettings()
	{
		if ( GetGame().GetPlayer() && m_ColorGrading != GetExpansionClientSettings().ColorGrading )
		{
			m_ColorGrading = GetExpansionClientSettings().ColorGrading;
			
			PPEffects.UpdateSaturation();
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionClientSettingsModule UpdateColorVignetteFromSettings
	// ------------------------------------------------------------
	private void UpdateColorVignetteFromSettings()
	{
		if ( GetGame().GetPlayer() && m_ColorVignette != GetExpansionClientSettings().ColorVignette )
		{
			PPEffects.UpdateVignette();
		
			m_ColorVignette = GetExpansionClientSettings().ColorVignette;
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionClientSettingsModule UpdateMarkersFromSettings
	// ------------------------------------------------------------
	private void UpdateMarkersFromSettings()
	{
		ExpansionMarkerModule module = ExpansionMarkerModule.Cast(GetModuleManager().GetModule(ExpansionMarkerModule));
		if (module)
		{
			// PERSONAL MARKERS
			if ( GetExpansionClientSettings().Show2DClientMarkers )
				module.SetVisibility( ExpansionMapMarkerType.PERSONAL, EXPANSION_MARKER_VIS_MAP );
			else
				module.RemoveVisibility( ExpansionMapMarkerType.PERSONAL, EXPANSION_MARKER_VIS_MAP );
			
			if ( GetExpansionClientSettings().Show3DClientMarkers  )
				module.SetVisibility( ExpansionMapMarkerType.PERSONAL, EXPANSION_MARKER_VIS_WORLD );
			else
				module.RemoveVisibility( ExpansionMapMarkerType.PERSONAL, EXPANSION_MARKER_VIS_WORLD );
			
			// PARTY MARKERS
			if ( GetExpansionClientSettings().Show2DPartyMarkers )
				module.SetVisibility( ExpansionMapMarkerType.PARTY, EXPANSION_MARKER_VIS_MAP );
			else
				module.RemoveVisibility( ExpansionMapMarkerType.PARTY, EXPANSION_MARKER_VIS_MAP );
			
			if ( GetExpansionClientSettings().Show3DPartyMarkers )
				module.SetVisibility( ExpansionMapMarkerType.PARTY, EXPANSION_MARKER_VIS_WORLD );
			else
				module.RemoveVisibility( ExpansionMapMarkerType.PARTY, EXPANSION_MARKER_VIS_WORLD );
			
			// SERVER MARKERS
			if ( GetExpansionClientSettings().Show2DGlobalMarkers )
				module.SetVisibility( ExpansionMapMarkerType.SERVER, EXPANSION_MARKER_VIS_MAP );
			else
				module.RemoveVisibility( ExpansionMapMarkerType.SERVER, EXPANSION_MARKER_VIS_MAP );
			
			if ( GetExpansionClientSettings().Show3DGlobalMarkers )
				module.SetVisibility( ExpansionMapMarkerType.SERVER, EXPANSION_MARKER_VIS_WORLD );
			else
				module.RemoveVisibility( ExpansionMapMarkerType.SERVER, EXPANSION_MARKER_VIS_WORLD );
			
			// MEMBER MARKERS
			if ( GetExpansionClientSettings().Show2DPlayerMarkers )
				module.SetVisibility( ExpansionMapMarkerType.PLAYER, EXPANSION_MARKER_VIS_MAP );
			else
				module.RemoveVisibility( ExpansionMapMarkerType.PLAYER, EXPANSION_MARKER_VIS_MAP );
			
			if ( GetExpansionClientSettings().Show3DPlayerMarkers )
				module.SetVisibility( ExpansionMapMarkerType.PLAYER, EXPANSION_MARKER_VIS_WORLD );
			else
				module.RemoveVisibility( ExpansionMapMarkerType.PLAYER, EXPANSION_MARKER_VIS_WORLD );
		}
	}
	
	private void UpdateCameraFromSettings()
	{
		if (GetGame() && GetGame().GetPlayer())
		{
			PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
			if (player && player.IsAlive() && player.IsInTransport())
			{
				DayZPlayerCamera3rdPersonVehicle camera = DayZPlayerCamera3rdPersonVehicle.Cast(player.GetCurrentCamera());
				if (camera)
				{
					m_Distance = GetTransportCameraDistance();
					m_Offset = GetTransportCameraOffset();
					
					camera.SetViewDistance(m_Distance);
					camera.SetCameraOffset(m_Offset);
				}
			}
		}
	}

	private float GetCameraHeight()
	{
		return GetExpansionClientSettings().VehicleCameraHeight;
	}

	private float GetCameraDistance()
	{
		return GetExpansionClientSettings().VehicleCameraDistance;
	}
	
	private float GetCameraOffsetX()
	{
		return GetExpansionClientSettings().VehicleCameraOffsetY;
	}
	
	private float GetTransportCameraDistance()
	{
		return GetCameraDistance();
	}
	
	private vector GetTransportCameraOffset()
	{
		return Vector(0, GetCameraHeight(), GetCameraOffsetX());
	}
}