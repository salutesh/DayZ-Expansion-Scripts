/**
 * ExpansionTerritoryInvite.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionTerritoryInvite
{
	int TerritoryID;
	string TerritoryName;
	string UID;

	#ifdef CF_MODULE_MODSTORAGE
	// ------------------------------------------------------------
	// Expansion OnStoreSave
	// ------------------------------------------------------------
	void OnStoreSave( CF_ModStorage storage )
	{
		storage.Write( TerritoryID );
		storage.Write( TerritoryName );
		storage.Write( UID );
	}

	// ------------------------------------------------------------
	// Expansion OnStoreLoad
	// ------------------------------------------------------------
	bool OnStoreLoad( CF_ModStorage storage )
	{
		if ( Expansion_Assert_False( storage.Read( TerritoryID ), "[" + this + "] Failed reading TerritoryID" ) )
			return false;
		if ( Expansion_Assert_False( storage.Read( TerritoryName ), "[" + this + "] Failed reading TerritoryName" ) )
			return false;
		if ( Expansion_Assert_False( storage.Read( UID ), "[" + this + "] Failed reading UID" ) )
			return false;

		return true;
	}
	#endif
};