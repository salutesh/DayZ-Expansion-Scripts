/**
 * ExpansionPartyInviteData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPartyInviteData
{
	int partyID;
	string partyName;
	string UID;
	
	void OnStoreSave( ParamsWriteContext ctx )
	{
		ctx.Write(partyID);
		ctx.Write(partyName);
		ctx.Write(UID);
	}
	
	bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( Expansion_Assert_False( ctx.Read( partyID ), "[" + this + "] Failed reading partyID" ) )
			return false;

		if ( Expansion_Assert_False( ctx.Read( partyName ), "[" + this + "] Failed reading partyName" ) )
			return false;

		if ( Expansion_Assert_False( ctx.Read( UID ), "[" + this + "] Failed reading UID" ) )
			return false;
		
		return true;
	}
	
	string GetID()
	{
		return UID;
	}
};