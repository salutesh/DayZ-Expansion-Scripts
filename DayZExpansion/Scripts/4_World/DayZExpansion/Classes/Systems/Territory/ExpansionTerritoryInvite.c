/**
 * ExpansionTerritoryInvite.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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

	// ------------------------------------------------------------
	// Expansion OnStoreSave
	// ------------------------------------------------------------
	void OnStoreSave( ModStorage storage )
	{
		storage.WriteInt( TerritoryID );
		storage.WriteString( TerritoryName );
		storage.WriteString( UID );
	}

	// ------------------------------------------------------------
	// Expansion OnStoreLoad
	// ------------------------------------------------------------
	bool OnStoreLoad( ModStorage storage )
	{
		if ( Expansion_Assert_False( storage.ReadInt( TerritoryID ), "[" + this + "] Failed reading TerritoryID" ) )
			return false;
		if ( Expansion_Assert_False( storage.ReadString( TerritoryName ), "[" + this + "] Failed reading TerritoryName" ) )
			return false;
		if ( Expansion_Assert_False( storage.ReadString( UID ), "[" + this + "] Failed reading UID" ) )
			return false;

		return true;
	}
};