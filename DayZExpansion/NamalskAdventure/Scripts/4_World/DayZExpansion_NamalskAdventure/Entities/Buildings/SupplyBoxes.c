/**
 * SupplyBoxes.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class StaticObj_Misc_SupplyBox1_DE extends House 
{
#ifdef DIAG
#ifdef EXPANSIONMODNAVIGATION
	protected ExpansionMarkerData m_ServerMarker;
#endif
#endif

    void ~StaticObj_Misc_SupplyBox1_DE()
	{
		#ifdef DIAG
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
        super.EEInit();
	#ifdef DIAG
	#ifdef EXPANSIONMODNAVIGATION
    #ifdef SERVER
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CreateDebugMarker, 500, false);
    #endif
	#endif
	#endif
	}

#ifdef DIAG
#ifdef EXPANSIONMODNAVIGATION
	void CreateDebugMarker()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		ExpansionMarkerModule markerModule;
		CF_Modules<ExpansionMarkerModule>.Get(markerModule);
		if (markerModule)
			m_ServerMarker = markerModule.CreateServerMarker(GetType(), "Tent", Vector(GetPosition()[0], GetPosition()[1] + 1.0, GetPosition()[2]), ARGB(255, 22, 160, 133), true);
	}
#endif
#endif
};

class StaticObj_Misc_SupplyBox2_DE extends House 
{
#ifdef DIAG
#ifdef EXPANSIONMODNAVIGATION
	protected ExpansionMarkerData m_ServerMarker;
#endif
#endif

    void ~StaticObj_Misc_SupplyBox2_DE()
	{
		#ifdef DIAG
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
        super.EEInit();
	#ifdef DIAG
	#ifdef EXPANSIONMODNAVIGATION
    #ifdef SERVER
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CreateDebugMarker, 500, false);
    #endif
	#endif
	#endif
	}

#ifdef DIAG
#ifdef EXPANSIONMODNAVIGATION
	void CreateDebugMarker()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		ExpansionMarkerModule markerModule;
		CF_Modules<ExpansionMarkerModule>.Get(markerModule);
		if (markerModule)
			m_ServerMarker = markerModule.CreateServerMarker(GetType(), "Tent", Vector(GetPosition()[0], GetPosition()[1] + 1.0, GetPosition()[2]), ARGB(255, 22, 160, 133), true);
	}
#endif
#endif
};

class StaticObj_Misc_SupplyBox3_DE extends House 
{
#ifdef DIAG
#ifdef EXPANSIONMODNAVIGATION
	protected ExpansionMarkerData m_ServerMarker;
#endif
#endif

    void ~StaticObj_Misc_SupplyBox3_DE()
	{
		#ifdef DIAG
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
        super.EEInit();
	#ifdef DIAG
	#ifdef EXPANSIONMODNAVIGATION
    #ifdef SERVER
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CreateDebugMarker, 500, false);
    #endif
	#endif
	#endif
	}

#ifdef DIAG
#ifdef EXPANSIONMODNAVIGATION
	void CreateDebugMarker()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		ExpansionMarkerModule markerModule;
		CF_Modules<ExpansionMarkerModule>.Get(markerModule);
		if (markerModule)
			m_ServerMarker = markerModule.CreateServerMarker(GetType(), "Tent", Vector(GetPosition()[0], GetPosition()[1] + 1.0, GetPosition()[2]), ARGB(255, 22, 160, 133), true);
	}
#endif
#endif
};
