/**
 * ExpansionGame.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionGame
{
#ifdef DABS_FRAMEWORK
	ref ExpansionUIManager m_ExUIManager;
#endif

	// ------------------------------------------------------------
	// ExpansionGame Constructor
	// ------------------------------------------------------------
	void ExpansionGame()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGame::ExpansionGame - Start");
		#endif
	#ifdef DABS_FRAMEWORK
		CreateExpansionUIManager();
	#endif

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGame::ExpansionGame - End");
		#endif
	}

	// ------------------------------------------------------------
	// ExpansionGame Destructor
	// ------------------------------------------------------------	
	void ~ExpansionGame()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGame::~ExpansionGame - Start");
		#endif
	
	#ifdef DABS_FRAMEWORK	
		DestroyExpansionUIManager();
	#endif
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGame::~ExpansionGame - End");
		#endif
	}
	
#ifdef DABS_FRAMEWORK
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
			delete m_ExUIManager;
	}
	
	// ------------------------------------------------------------
	// ExpansionGame GetExpansionUIManager
	// ------------------------------------------------------------
	ExpansionUIManager GetExpansionUIManager()
	{
		return m_ExUIManager;
	}
#endif

	// ------------------------------------------------------------
	// ExpansionGame FirearmEffects
	// ------------------------------------------------------------
	void FirearmEffects( Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal, vector exitPos, vector inSpeed, vector outSpeed, bool isWater, bool deflected, string ammoType ) 
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGame::FirearmEffects - Start");
		#endif	
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGame::FirearmEffects - End");
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
	
	// ------------------------------------------------------------
	// ExpansionGame OnRPC
	// ------------------------------------------------------------
	bool OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	{
		return false;
	}
};