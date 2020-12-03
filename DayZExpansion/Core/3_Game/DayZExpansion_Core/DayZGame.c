/**
 * ExpansionDayZGame.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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

	// ------------------------------------------------------------
	// DayZGame Constructor
	// ------------------------------------------------------------
	void DayZGame()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZGame::DayZGame - Start");
		#endif

		Expansion_SetupDebugger();

		if (!FileExist(EXPANSION_FOLDER))
		{
			MakeDirectory(EXPANSION_FOLDER);
		}

		if (FileExist(EXPANSION_TEMP_INTERIORS))
		{
			DeleteFile(EXPANSION_TEMP_INTERIORS);
		}

		string path = "cfgVehicles";
		string child_name = "";
		int count = ConfigGetChildrenCount(path);
		m_CharClassNames.Clear();

		for (int p = 0; p < count; p++)
		{
			ConfigGetChildName(path, p, child_name);

			if (ConfigGetInt(path + " " + child_name + " scope") == 2 && IsKindOf(child_name, "SurvivorBase"))
			{
				if (child_name.Contains("Expansion") || child_name.Contains("Trader"))
					continue;

				m_CharClassNames.Insert(child_name);
			}
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

	// ------------------------------------------------------------
	// Expansion SetExpansionGame
	// ------------------------------------------------------------
	void SetExpansionGame(ref ExpansionGame game)
	{
		m_ExpansionGame = game;
	}

	// ------------------------------------------------------------
	// Expansion GetExpansionGame
	// ------------------------------------------------------------
	ref ExpansionGame GetExpansionGame()
	{
		return m_ExpansionGame;
	}

	// ------------------------------------------------------------
	// Expansion SetExpansionClientVersion
	// ------------------------------------------------------------
	void SetExpansionClientVersion(string version)
	{
		m_ExpansionClientVersion = version;
	}

	// ------------------------------------------------------------
	// Expansion SetExpansionLatestVersion
	// ------------------------------------------------------------
	void SetExpansionLatestVersion(string version)
	{
		m_ExpansionLastestVersion = version;
	}

	// ------------------------------------------------------------
	// Expansion GetExpansionClientVersion
	// ------------------------------------------------------------
	string GetExpansionClientVersion()
	{
		return m_ExpansionClientVersion;
	}

	// ------------------------------------------------------------
	// Expansion GetExpansionLatestVersion
	// ------------------------------------------------------------
	string GetExpansionLatestVersion()
	{
		return m_ExpansionLastestVersion;
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
		super.OnRPC(sender, target, rpc_type, ctx);

		//! Move below if there becomes some problems
		//! Steve moved below on 30.06.2020 to fix a NULL pointer
		if (m_ExpansionGame != NULL && m_ExpansionGame.OnRPC(sender, target, rpc_type, ctx))
			return;
	}
};