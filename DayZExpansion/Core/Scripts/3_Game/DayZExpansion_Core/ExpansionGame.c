/**
 * ExpansionGame.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionGame
{
#ifdef EXPANSIONUI
	ref ExpansionUIManager m_ExUIManager;
#endif
	ref ExpansionRPCManager m_RPCManager = new ExpansionRPCManager(this);

	bool m_IsLoaded;
	int m_InstanceId;

	// ------------------------------------------------------------
	// ExpansionGame Constructor
	// ------------------------------------------------------------
	void ExpansionGame()
	{
#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.MISC, this);
#endif

	#ifdef EXPANSIONUI
		CreateExpansionUIManager();
	#endif
	}

	// ------------------------------------------------------------
	// ExpansionGame Destructor
	// ------------------------------------------------------------	
	void ~ExpansionGame()
	{
		if (!GetGame())
			return;
		
	#ifdef EXPANSIONUI
		DestroyExpansionUIManager();
	#endif

#ifdef DIAG_DEVELOPER
		Print("~ExpansionGame");
#endif
	}
	
#ifdef EXPANSIONUI
	// ------------------------------------------------------------
	// ExpansionGame CreateExpansionUIManager
	// ------------------------------------------------------------		
	void CreateExpansionUIManager()
	{
		if (!m_ExUIManager)
		{
			m_ExUIManager = new ExpansionUIManager();
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionGame DestroyExpansionUIManager
	// ------------------------------------------------------------	
	void DestroyExpansionUIManager()
	{
		if (m_ExUIManager)
			m_ExUIManager = null;
	}
	
	// ------------------------------------------------------------
	// ExpansionGame GetExpansionUIManager
	// ------------------------------------------------------------
	ExpansionUIManager GetExpansionUIManager()
	{
		return m_ExUIManager;
	}
#endif

	/**
	 * @brief like CreateObject, but won't create objects that have scope = 0 or no model
	 */
	static Object CreateObjectSafe(string type, vector pos, bool create_local = false, bool init_ai = false, bool create_physics = true)
	{
	/*
		int flags;

		if (create_local)
			flags |= ECE_LOCAL

		if (init_ai)
			flags |= ECE_INITAI;

		if (create_physics)
			flags |= ECE_CREATEPHYSICS;

		return CreateObjectExSafe(type, pos, flags);
	*/
		if (IsSafeToSpawn(type))
			return GetGame().CreateObject(type, pos, create_local, init_ai, create_physics);

		return null;
	}

	/**
	 * @brief like CreateObjectEx, but won't create objects that have scope = 0 or no model
	 */
	static Object CreateObjectExSafe(string type, vector pos, int iFlags, int iRotation = RF_DEFAULT)
	{
		if (IsSafeToSpawn(type))
			return GetGame().CreateObjectEx(type, pos, iFlags, iRotation);

		return null;
	}

	static bool IsSafeToSpawn(string type)
	{
		TStringArray paths = {CFG_VEHICLESPATH, CFG_WEAPONSPATH, CFG_MAGAZINESPATH};

		string model;

		foreach (string path: paths)
		{
			if (GetGame().ConfigIsExisting(path + " " + type))
			{
				if (!GetGame().ConfigGetInt(path + " " + type + " scope"))
					return false;

				if (!GetGame().ConfigGetText(path + " " + type + " model", model) || !model || model == "bmp")
					return false;

				return true;
			}
		}

		return false;
	}

	// ------------------------------------------------------------
	// ExpansionGame FirearmEffects
	// ------------------------------------------------------------
	void FirearmEffects( Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal, vector exitPos, vector inSpeed, vector outSpeed, bool isWater, bool deflected, string ammoType ) 
	{
	}
	
	// ------------------------------------------------------------
	// ExpansionGame OnStart
	// ------------------------------------------------------------	
	void OnStart()
	{
		//! @note ServerConfigGetInt will return zero in OnMissionFinish, so we cache the value
		m_InstanceId = GetGame().ServerConfigGetInt("instanceId");
	}
	
	// ------------------------------------------------------------
	// ExpansionGame OnLoaded
	// ------------------------------------------------------------
	void OnLoaded()
	{
		m_IsLoaded = true;
	}
	
	bool IsLoaded()
	{
		return m_IsLoaded;
	}

	// ------------------------------------------------------------
	// ExpansionGame OnFinish
	// ------------------------------------------------------------
	void OnFinish()
	{
	}

	// ------------------------------------------------------------
	// ExpansionGame OnUpdate
	// ------------------------------------------------------------
	void OnUpdate( bool doSim, float timeslice ) 
	{
	}
	
	void WriteRemovedWorldObjects(ParamsReadContext ctx)
	{
	}

	void ReadRemovedWorldObjects(ParamsReadContext ctx)
	{
	}

	void Expansion_SendNear(ExpansionScriptRPC rpc, vector position, float distance, Object target = null, bool guaranteed = false)
	{
	}

	void Expansion_SendFar(ExpansionScriptRPC rpc, vector position, Object target = null, bool guaranteed = false)
	{
	}

	static bool IsMultiplayerClient()
	{
		//! Check for `&& IsMultiplayer` is redundant here, comment for clarity
		return GetGame().IsClient();
	}

	static bool IsMultiplayerServer()
	{
		return GetGame().IsServer() && GetGame().IsMultiplayer();
	}

	static bool IsOffline()
	{
		return !GetGame().IsMultiplayer();
	}

	static bool IsClientOrOffline()
	{
		return !GetGame().IsDedicatedServer();
	}

	static bool IsServerOrOffline()
	{
		//! Check for `|| !IsMultiplayer` is redundant here, comment for clarity
		return GetGame().IsServer();
	}

	void Lobotomize(DayZCreatureAI creature)
	{
	}

	void LobotomySync(DayZCreatureAI creature, bool isLobotomized = false)
	{
	}
};
