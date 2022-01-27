/**
 * DayzExpansion.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
		#ifdef EXPANSIONEXPRINT
		EXPrint("[CORE] DayZExpansion::DayZExpansion - Start");
		#endif

		//! Version checking
		Expansion_LoadVersion();

		GetExpansionSettings();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("[CORE] DayZExpansion::DayZExpansion - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion Expansion_LoadVersion
	// ------------------------------------------------------------
	void Expansion_LoadVersion()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZExpansion::Expansion_LoadVersion - Start");
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

		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZExpansion::Expansion_LoadVersion - End");
		#endif
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

	override bool OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
		if ( g_exGlobalSettings && g_exGlobalSettings.OnRPC( sender, target, rpc_type, ctx ) )
			return true;

		return super.OnRPC( sender, target, rpc_type, ctx );
	}
}

static ref DayZExpansion g_exDayZ;

static DayZExpansion GetDayZExpansion()
{
	return g_exDayZ;
}

static void CreateDayZExpansion()
{
	#ifdef EXPANSIONEXPRINT
	EXPrint("CreateDayZExpansion - Start");
	#endif
	
	if ( g_exDayZ )
	{
		return;
	}

	g_exDayZ = new DayZExpansion;

	GetDayZGame().SetExpansionGame( g_exDayZ );
	
	#ifdef EXPANSIONEXPRINT
	EXPrint("CreateDayZExpansion - End");
	#endif
}

static void DestroyDayZExpansion()
{
	#ifdef EXPANSIONEXPRINT
	EXPrint("DestroyDayZExpansion - Start");
	#endif
	
	delete g_exDayZ;
	
	#ifdef EXPANSIONEXPRINT
	EXPrint("DestroyDayZExpansion - End");
	#endif
}