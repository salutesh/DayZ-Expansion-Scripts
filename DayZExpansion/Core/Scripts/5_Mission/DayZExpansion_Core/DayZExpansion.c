/**
 * DayzExpansion.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		DayZExpansion
 * @brief		
 **/
class DayZExpansion: ExpansionWorld
{
	protected string m_Version;

	protected int m_MajorVersion;
	protected int m_MinorVersion;
	protected int m_BuildVersion;

	// ------------------------------------------------------------
	// DayZExpansion Constructor
	// ------------------------------------------------------------
	void DayZExpansion()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "DayZExpansion");
#endif
	
		//! Version checking
		Expansion_LoadVersion();

		//! Initialize settings
		GetExpansionSettings();
	}

	// ------------------------------------------------------------
	// Expansion Expansion_LoadVersion
	// ------------------------------------------------------------
	void Expansion_LoadVersion()
	{		
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "Expansion_LoadVersion");
#endif
	
		m_Version = GetDayZGame().GetExpansionClientVersion();

		array<string> values = new array<string>();
		m_Version.Split( ".", values );
		if ( values.Count() == 3 )
		{
			m_MajorVersion = values[0].ToInt();
			m_MinorVersion = values[1].ToInt();
			m_BuildVersion = values[2].ToInt();

			Print( "Expansion version: " + m_MajorVersion + "." + m_MinorVersion + "." + m_BuildVersion );
		}
		
		//string versionTest;
		//GetGame().ConfigGetText( "CfgMods DZ_Expansion version", versionTest );
		//Print( versionTest );
	}

	// ------------------------------------------------------------
	// Expansion GetVersion
	// ------------------------------------------------------------
	string GetVersion()
	{
		return m_Version;
	}

	// ------------------------------------------------------------
	// Expansion OnStart
	// ------------------------------------------------------------
	override void OnStart()
	{
		super.OnStart();
	}

	override void OnLoaded()
	{
		if ( IsMissionHost() )
		{
			ExpansionSettings.SI_SafeZone.Invoke();
		}

		super.OnLoaded();
	}
}

static ref DayZExpansion g_exDayZ;

static DayZExpansion GetDayZExpansion()
{
	return g_exDayZ;
}

static void CreateDayZExpansion()
{
#ifdef EXPANSIONTRACE
	auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, "CreateDayZExpansion");
#endif
	
	if ( g_exDayZ )
	{
		return;
	}

	g_exDayZ = new DayZExpansion;

	GetDayZGame().SetExpansionGame( g_exDayZ );
}

static void DestroyDayZExpansion()
{	
#ifdef EXPANSIONTRACE
	auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, "DestroyDayZExpansion");
#endif

	g_exDayZ = null;
}