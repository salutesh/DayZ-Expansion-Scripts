/**
 * SV_Abandoned_Sattelite_Antenna.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! @note: Can only be used when the BuildingsModPack mod by Starlv is loaded: https://steamcommunity.com/sharedfiles/filedetails/?id=2270098553
#ifdef EXPANSION_NAMALSK_ADVENTURE
class SV_Abandoned_Sattelite_Antenna extends House
{
#ifdef DIAG_DEVELOPER
#ifdef EXPANSIONMODNAVIGATION
	protected ExpansionMarkerData m_ServerMarker;
#endif
#endif
	
	protected bool m_IsActive;

	void SV_Abandoned_Sattelite_Antenna()
	{
		RegisterNetSyncVariableBool("m_IsActive");
	}
	
	void ~SV_Abandoned_Sattelite_Antenna()
	{
		#ifdef DIAG_DEVELOPER
		#ifdef EXPANSIONMODNAVIGATION
			if (!m_ServerMarker)
				return;

			ExpansionMarkerModule markerModule;
			CF_Modules<ExpansionMarkerModule>.Get(markerModule);
			if (markerModule)
				markerModule.RemoveServerMarker(m_ServerMarker.GetUID());
		#endif
		#endif
	}
	
	override void EEInit()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		super.EEInit();

		InitSatellite();
	}
	
	protected void InitSatellite()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		if (!GetGame().IsDedicatedServer())
		{
			InitSatelliteClient();
		}

		if (GetGame().IsServer())
		{
			InitSatelliteServer();
		}
	}
	
	void SetSatelliteActive(bool state)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		
		if (state)
		{
			OnSatelliteStateActiveStart();
		}
		else
		{
			OnSatelliteStateActiveEnd();
		}
		
		m_IsActive = state;
		SetSynchDirty();
	}
	
	void OnSatelliteStateActiveStart()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
	}
	
	void OnSatelliteStateActiveEnd()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
	}
	
	protected void InitSatelliteClient()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
	}

	protected void InitSatelliteServer()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

	#ifdef DIAG_DEVELOPER
	#ifdef EXPANSIONMODNAVIGATION
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CreateDebugMarker, 500, false);
	#endif
	#endif
	}

	bool GetActiveState()
	{
		return m_IsActive;
	}
	
#ifdef DIAG_DEVELOPER
#ifdef EXPANSIONMODNAVIGATION
	void CreateDebugMarker()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		ExpansionMarkerModule markerModule;
		CF_Modules<ExpansionMarkerModule>.Get(markerModule);
		if (markerModule)
			m_ServerMarker = markerModule.CreateServerMarker(GetType(), "Tent", Vector(GetPosition()[0], GetPosition()[1] + 1.0, GetPosition()[2]), ARGB(255, 44, 62, 80), true);
	}
#endif
#endif
};
#endif
