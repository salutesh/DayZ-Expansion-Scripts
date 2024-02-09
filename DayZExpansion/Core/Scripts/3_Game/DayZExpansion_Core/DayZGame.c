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
	protected float m_Expansion_ServerUpdateRateLimit_Interval;
	protected float m_Expansion_ServerUpdateRateLimit_LastUpdateTime;
	protected float m_Expansion_ServerUpdateRateLimit_TimeSlice;

	protected string m_ExpansionClientVersion;
	protected string m_ExpansionLastestVersion;
	protected ref ExpansionGame m_ExpansionGame;

	protected int m_Expansion_StartTime;
	protected int m_Expansion_StartTimeUTC;
	protected int m_Expansion_StartTimestamp;
	protected int m_Expansion_StartTimestampUTC;

	protected vector m_WorldCenterPosition;
	protected bool m_Expansion_IsMissionMainMenu;
	
	// ------------------------------------------------------------
	// DayZGame Constructor
	// ------------------------------------------------------------
	void DayZGame()
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(EXTrace.GLOBAL, this);
#endif

		auto now = CF_Date.Now();
		auto nowUTC = CF_Date.Now(true);

		m_Expansion_StartTime = ExpansionCalculateStartTime(now);
		m_Expansion_StartTimeUTC = ExpansionCalculateStartTime(nowUTC);
		m_Expansion_StartTimestamp = now.DateToEpoch() - (int) GetTickTime();
		m_Expansion_StartTimestampUTC = nowUTC.DateToEpoch() - (int) GetTickTime();
		EXLogPrint(ToString() + " Start time (local time) " + CF_Date.Epoch(m_Expansion_StartTimestamp).Format(CF_Date.DATETIME));
		EXLogPrint(ToString() + " Start time (UTC) " + CF_Date.Epoch(m_Expansion_StartTimestampUTC).Format(CF_Date.DATETIME));
		EXLogPrint(ToString() + " Runtime since start (seconds) " + GetTickTime());

		CF_Log.s_Expansion_Static = new ExpansionStatic;

		if (!FileExist(EXPANSION_FOLDER))
		{
			ExpansionStatic.MakeDirectoryRecursive(EXPANSION_FOLDER);
		}

		if (FileExist(EXPANSION_TEMP_INTERIORS))
		{
			DeleteFile(EXPANSION_TEMP_INTERIORS);
		}

		Expansion_FilterPlayerCharacters();
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

		if (EXTrace.PROFILING)
			EXTrace.DumpProfilingData();
	}

	void Expansion_FilterPlayerCharacters()
    {
		string child_name;
        for (int i = m_CharClassNames.Count() - 1; i >= 0; i--)
		{
            child_name = m_CharClassNames[i];

			if (child_name.IndexOf("Expansion") == 0 || child_name.IndexOf("eAI") == 0)
			{
				m_CharClassNames.RemoveOrdered(i);
				EXTrace.Print(EXTrace.MISC, this, "Removed " + child_name + " from player character class names");
			}
		}
    }

	void Expansion_SetFoodDecayModifier(float foodDecayModifier)
	{
		m_FoodDecayModifier = foodDecayModifier;
	}

	// ------------------------------------------------------------
	// Expansion SetExpansionGame
	// ------------------------------------------------------------
	void SetExpansionGame(ExpansionGame game)
	{
#ifdef DIAG
		auto trace = EXTrace.Start(EXTrace.MISC, this);
#endif

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

	int ExpansionCalculateStartTime(CF_Date date)
	{
		int time = date.GetHours() * 3600 + date.GetMinutes() * 60 + date.GetSeconds() - (int) GetTickTime();
		if (time < 0)
			time += 24 * 3600;
		return time;
	}

	int ExpansionGetStartTime(bool useUTC = false)
	{
		if (useUTC)
			return m_Expansion_StartTimeUTC;
		return m_Expansion_StartTime;
	}

	int ExpansionGetStartTimestamp(bool useUTC = false)
	{
		if (useUTC)
			return m_Expansion_StartTimestampUTC;
		return m_Expansion_StartTimestamp;
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
		return GetGame().GetWorld().GetWorldSize();
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
#ifdef SERVER
		//! @note timeslice isn't accurate for measuring actual elapsed time, so it's just passed to super to match vanilla
		float updateTime = GetTickTime();
		float elapsed = updateTime - m_Expansion_ServerUpdateRateLimit_LastUpdateTime;
		m_Expansion_ServerUpdateRateLimit_TimeSlice += timeslice;
		if (elapsed >= m_Expansion_ServerUpdateRateLimit_Interval)
		{
			super.OnUpdate(doSim, m_Expansion_ServerUpdateRateLimit_TimeSlice);

			if (m_ExpansionGame != NULL)
				m_ExpansionGame.OnUpdate(doSim, m_Expansion_ServerUpdateRateLimit_TimeSlice);

			m_Expansion_ServerUpdateRateLimit_LastUpdateTime = updateTime;
			m_Expansion_ServerUpdateRateLimit_TimeSlice = 0;
		}
#ifdef CFTOOLS
		else
		{
			gl_ticks++;
		}
#endif
#else
		super.OnUpdate(doSim, timeslice);

		if (m_ExpansionGame != NULL)
			m_ExpansionGame.OnUpdate(doSim, timeslice);
#endif
	}

	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		switch (rpc_type)
		{
			case ExpansionScriptRPC.EXPANSION_RPC_UNTARGETED:
				ExpansionRPCManager.OnRPC(sender, target, ctx);
				break;
			case ExpansionScriptRPC.EXPANSION_RPC_TARGETED:
				if (target)
					ExpansionRPCManager.OnRPC(sender, target, ctx);
				else
					EXTrace.Print(EXTrace.RPC, this, "Info: Targeted Expansion RPC received, but target doesn't exist");
				break;
			default:
				super.OnRPC(sender, target, rpc_type, ctx);
				break;
		}
	}

	void Expansion_SetIsMissionMainMenu(bool state)
	{
		m_Expansion_IsMissionMainMenu = state;
	}

	bool Expansion_IsMissionMainMenu()
	{
		return m_Expansion_IsMissionMainMenu;
	}

	bool Expansion_IsMissionSinglePlayer()
	{
		if (IsMultiplayer())
			return false;

		string mission;
		if (GetCLIParam("mission", mission))
			return true;

		return false;
	}

	void Expansion_SetServerUpdateRateLimit(int rate)
	{
		m_Expansion_ServerUpdateRateLimit_Interval = 1.0 / rate;
	}
};
