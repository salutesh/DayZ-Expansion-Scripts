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
	// ------------------------------------------------------------
	// Expansion Constructor
	// ------------------------------------------------------------
	void ExpansionGame()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGame::ExpansionGame - Start");
		#endif

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGame::ExpansionGame - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion FirearmEffects
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

	bool OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	{
		return false;
	}
}