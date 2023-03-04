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
	ref ExpansionUIManager m_ExUIManager;

	bool m_IsLoaded;

	// ------------------------------------------------------------
	// ExpansionGame Constructor
	// ------------------------------------------------------------
	void ExpansionGame()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "ExpansionGame");
#endif

		CreateExpansionUIManager();
	}

	// ------------------------------------------------------------
	// ExpansionGame Destructor
	// ------------------------------------------------------------	
	void ~ExpansionGame()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "~ExpansionGame");
#endif
		
		DestroyExpansionUIManager();
	}
	
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

	// ------------------------------------------------------------
	// ExpansionGame FirearmEffects
	// ------------------------------------------------------------
	void FirearmEffects( Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal, vector exitPos, vector inSpeed, vector outSpeed, bool isWater, bool deflected, string ammoType ) 
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "FirearmEffects");
#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionGame OnStart
	// ------------------------------------------------------------	
	void OnStart()
	{
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

	// ------------------------------------------------------------
	// ExpansionGame OnRPC
	// ------------------------------------------------------------
	bool OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
		if ( rpc_type == ExpansionRPC.CreateNotification )
		{
			NotificationSystem.RPC_ExpansionCreateNotification( sender, target, ctx );

			return true;
		}

		return false;
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
};
