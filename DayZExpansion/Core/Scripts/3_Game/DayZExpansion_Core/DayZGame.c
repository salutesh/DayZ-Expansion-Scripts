/**
 * ExpansionDayZGame.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZGame
{
	protected string m_ExpansionClientVersion;
	protected string m_ExpansionLastestVersion;
	protected ref ExpansionGame m_ExpansionGame;

	protected int m_Expansion_StartTime;

	protected vector m_WorldCenterPosition;
	protected bool m_Expansion_IsMissionMainMenu;
	
	// ------------------------------------------------------------
	// DayZGame Constructor
	// ------------------------------------------------------------
	void DayZGame()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "DayZGame");
#endif

		int year;
		int month;
		int day;
		int hour;
		int minute;
		int second;

		GetYearMonthDay(year, month, day);
		GetHourMinuteSecond( hour, minute, second );

		m_Expansion_StartTime = hour * 3600 + minute * 60 + second - GetTickTime();

		if (!FileExist(EXPANSION_FOLDER))
		{
			ExpansionStatic.MakeDirectoryRecursive(EXPANSION_FOLDER);
		}

		if (FileExist(EXPANSION_TEMP_INTERIORS))
		{
			DeleteFile(EXPANSION_TEMP_INTERIORS);
		}
	}

	// ------------------------------------------------------------
	// DayZGame Deconstructor
	// -------------------------------------------------------------
	void ~DayZGame()
	{
		if (FileExist(EXPANSION_TEMP_INTERIORS))
		{
			DeleteFile(EXPANSION_TEMP_INTERIORS);
		}
	}

    override void GlobalsInit()
    {
        super.GlobalsInit();

		string child_name;
        for (int i = m_CharClassNames.Count() - 1; i >= 0; i--)
		{
            child_name = m_CharClassNames[i];

			if (child_name.IndexOf("Expansion") == 0 || child_name.IndexOf("eAI") == 0)
			{
				m_CharClassNames.RemoveOrdered(i);
			}
		}
    }

	// ------------------------------------------------------------
	// Expansion SetExpansionGame
	// ------------------------------------------------------------
	void SetExpansionGame(ExpansionGame game)
	{
		m_ExpansionGame = game;
	}

	// ------------------------------------------------------------
	// Expansion GetExpansionGame
	// ------------------------------------------------------------
	ExpansionGame GetExpansionGame()
	{
		return m_ExpansionGame;
	}	

	// ------------------------------------------------------------
	// Expansion GetExpansionClientVersion
	// ------------------------------------------------------------
   	string GetExpansionClientVersion()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, "DayZGame", "GetExpansionClientVersion");
#endif

		if (m_ExpansionClientVersion)
			return m_ExpansionClientVersion;

		array<ref ModInfo> mods = new array<ref ModInfo>;
		string version;
		
		GetModInfos( mods );
		foreach (ModInfo mod: mods)
		{
			string modName = mod.GetName();
			if (modName[0] == "@")
				modName = modName.Substring(1, modName.Length() - 1);
			if ( modName.IndexOf( "DayZ-Expansion" ) == 0 && modName.IndexOf("DayZ-Expansion-Core") != 0 )
			{
				string modVersion = mod.GetVersion();
				if (modVersion > version)
					version = modVersion;
			}
		}

		if (!version)
			version = "[LOCAL BUILD]";

		m_ExpansionClientVersion = version;

		return version;
	}

	void Expansion_SetGameVersionText(TextWidget version_widget)
	{
		string version_info = "DayZ";

		#ifdef DIAG
			version_info += "_Diag";
		#endif

		string version;
		GetVersion(version);

		version_info += " #main_menu_version " + version;

		#ifdef BUILD_EXPERIMENTAL
			version_info += " Experimental";
		#endif

		string expansion_version = GetExpansionClientVersion();
		if (expansion_version)
			version_info += " | DayZ Expansion #main_menu_version " + expansion_version;

		version_widget.SetText(version_info);
	}

	int ExpansionGetStartTime()
	{
		return m_Expansion_StartTime;
	}

	protected void SetWorldCenterPosition()
	{
		string path = "CfgWorlds " + GetGame().GetWorldName();
		vector temp = GetGame().ConfigGetVector( path + " centerPosition" );
		m_WorldCenterPosition = Vector(temp[0], temp[2], temp[1]);
	}

	//! @note usually not actual center, depends on what's in a world's config.cpp
	vector GetWorldCenterPosition()
	{
		if ( !m_WorldCenterPosition )
			SetWorldCenterPosition();

		return m_WorldCenterPosition;
	}

	float GetWorldSize()
	{
		#ifndef DAYZ_1_18
		return GetGame().GetWorld().GetWorldSize();
		#endif
		
		float size;

		switch (ExpansionStatic.GetCanonicalWorldName())
		{
			case "chernarusplus":
				size = 15360.0;
				break;

			case "enoch":
			case "namalsk":
			case "esseker":
				size = 12800.0;
				break;

			case "chiemsee":
				size = 10240.0;
				break;

			case "deerisle":
				size = 16384.0;
				break;

			case "rostow":
				size = 14336.0;
				break;

			case "sandbox":
				size = 2048.0;
				break;
				
			default:
				//! Just fall back to Chernarus size
				size = 15360.0;
				break;
		}

		return size;
	}

	// ------------------------------------------------------------
	// Override FirearmEffects
	// ------------------------------------------------------------
	override void FirearmEffects(Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal, vector exitPos, vector inSpeed, vector outSpeed, bool isWater, bool deflected, string ammoType)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "FirearmEffects");
#endif

		if (m_ExpansionGame != NULL)
			m_ExpansionGame.FirearmEffects(source, directHit, componentIndex, surface, pos, surfNormal, exitPos, inSpeed, outSpeed, isWater, deflected, ammoType);

		super.FirearmEffects(source, directHit, componentIndex, surface, pos, surfNormal, exitPos, inSpeed, outSpeed, isWater, deflected, ammoType);
	}

	override void OnUpdate(bool doSim, float timeslice)
	{
		super.OnUpdate(doSim, timeslice);

		if (m_ExpansionGame != NULL)
			m_ExpansionGame.OnUpdate(doSim, timeslice);
	}

	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		//! Move below if there becomes some problems
		if (m_ExpansionGame != NULL && m_ExpansionGame.OnRPC(sender, target, rpc_type, ctx))
			return;
		
		super.OnRPC(sender, target, rpc_type, ctx);
	}

	void Expansion_SetIsMissionMainMenu(bool state)
	{
		m_Expansion_IsMissionMainMenu = state;
	}

	bool Expansion_IsMissionMainMenu()
	{
		return m_Expansion_IsMissionMainMenu;
	}
};
