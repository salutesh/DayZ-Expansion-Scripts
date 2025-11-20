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
#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.MISC, this);
#endif
	
		//! Version checking
		Expansion_LoadVersion();

		//! Initialize settings
		GetExpansionSettings();
	}

	void ~DayZExpansion()
	{
		if (!g_Game)
			return;

#ifdef DIAG_DEVELOPER
		Print("~DayZExpansion");
#endif
	}

	// ------------------------------------------------------------
	// Expansion Expansion_LoadVersion
	// ------------------------------------------------------------
	void Expansion_LoadVersion()
	{
#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.MISC, this);
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
		//g_Game.ConfigGetText( "CfgMods DZ_Expansion version", versionTest );
		//Print( versionTest );
	}

	// ------------------------------------------------------------
	// Expansion GetVersion
	// ------------------------------------------------------------
	string GetVersion()
	{
		return m_Version;
	}

	override void OnLoaded()
	{
		if ( IsMissionHost() )
		{
			ExpansionSettings.SI_SafeZone.Invoke();
		}

		super.OnLoaded();
	}

	override void OnSyncJuncture(Object target, int junctureID, ParamsReadContext ctx)
	{
		DayZPlayerImplement player;
		if (Class.CastTo(player, target))
			player.Expansion_OnSyncJuncture(junctureID, ctx);
		else
			EXError.Error(this, string.Format("Expansion Sync Juncture %1 received, but target %2 is not a player", junctureID, target));
	}
}

static ref DayZExpansion g_exDayZ;  //! Legacy, do not use!

static DayZExpansion GetDayZExpansion()
{
	DayZExpansion expansion;

	if (Class.CastTo(expansion, GetDayZGame().GetExpansionGame()))
		return expansion;

	return null;
}

static void CreateDayZExpansion()
{
#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.MISC);
#endif

	if (GetDayZGame().GetExpansionGame())
		return;

	GetDayZGame().SetExpansionGame(new DayZExpansion);
}

static void DestroyDayZExpansion()
{
#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.MISC);
#endif

	GetDayZGame().SetExpansionGame(null);
}