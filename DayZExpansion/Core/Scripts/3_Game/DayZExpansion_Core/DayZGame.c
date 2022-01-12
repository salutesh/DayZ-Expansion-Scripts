/**
 * ExpansionDayZGame.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
	
	// ------------------------------------------------------------
	// DayZGame Constructor
	// ------------------------------------------------------------
	void DayZGame()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZGame::DayZGame - Start");
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
			MakeDirectory(EXPANSION_FOLDER);
		}

		if (FileExist(EXPANSION_TEMP_INTERIORS))
		{
			DeleteFile(EXPANSION_TEMP_INTERIORS);
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZGame::DayZGame - End");
		#endif
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

		string path = "cfgVehicles";
		string child_name = "";
        for (int i = m_CharClassNames.Count() - 1; i >= 0; i--)
		{
            child_name = m_CharClassNames[i];

			if (child_name.Contains("Expansion") || child_name.Contains("Trader"))
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
   	static string GetExpansionClientVersion()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("LoadingScreen::GetExpansionClientVersion - Start");
		#endif

		array<ref ModInfo> mods = new array<ref ModInfo>;
		string version;
		
		GetDayZGame().GetModInfos( mods );
		for ( int i = 0; i < mods.Count(); ++i )
		{
			if ( mods[i].GetName().IndexOf( "DayZ" ) == 0 && mods[i].GetName().IndexOf( "Expansion" ) == 5 )
			{
				version = mods[i].GetVersion();
				break;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint( "LoadingScreen::GetExpansionClientVersion - Return: " + version );
		#endif

		return version;
	}

	int ExpansionGetStartTime()
	{
		return m_Expansion_StartTime;
	}

	protected void SetWorldCenterPosition()
	{
		string path = "CfgWorlds " + GetGame().GetWorldName();
		m_WorldCenterPosition = GetGame().ConfigGetVector( path + " centerPosition" );
	}

	//! Note: Usually not actual center, depends on what's in a world's config.cpp
	vector GetWorldCenterPosition()
	{
		if ( !m_WorldCenterPosition )
			SetWorldCenterPosition();

		return m_WorldCenterPosition;
	}

	float GetWorldSize()
	{
		string world_name = GetGame().GetWorldName();
		world_name.ToLower();

		float size;

		switch ( world_name )
		{
			case "chernarusplus":
			case "chernarusplusgloom":
				size = 15360.0;
				break;

			case "enoch":
			case "enochgloom":
			case "namalsk":
			case "namalskgloom":
			case "esseker":
			case "essekergloom":
				size = 12800.0;
				break;

			case "chiemsee":
			case "chiemseegloom":
				size = 10240.0;
				break;

			case "deerisle":
			case "deerislegloom":
				size = 16384.0;
				break;

			case "rostow":
			case "rostowgloom":
				size = 14336.0;
				break;

			case "sandbox":
			case "sandboxgloom":
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
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZGame::FirearmEffects - Start");
		#endif

		super.FirearmEffects(source, directHit, componentIndex, surface, pos, surfNormal, exitPos, inSpeed, outSpeed, isWater, deflected, ammoType);

		if (m_ExpansionGame != NULL)
			m_ExpansionGame.FirearmEffects(source, directHit, componentIndex, surface, pos, surfNormal, exitPos, inSpeed, outSpeed, isWater, deflected, ammoType);

		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZGame::FirearmEffects - End");
		#endif
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
};