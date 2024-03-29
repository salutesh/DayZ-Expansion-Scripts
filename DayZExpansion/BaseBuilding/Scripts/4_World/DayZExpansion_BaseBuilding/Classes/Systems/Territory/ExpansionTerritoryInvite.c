/**
 * ExpansionTerritoryInvite.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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

	#ifdef EXPANSION_MODSTORAGE
	void OnStoreSave(CF_ModStorage ctx)
	{
		ctx.Write(TerritoryID);
		ctx.Write(TerritoryName);
		ctx.Write(UID);
	}

	bool OnStoreLoad( CF_ModStorage ctx )
	{
		if (!ctx.Read(TerritoryID))
			return false;
		if (!ctx.Read(TerritoryName))
			return false;
		if (!ctx.Read(UID))
			return false;

		return true;
	}
	#endif
	
	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(TerritoryID);
		ctx.Write(TerritoryName);
		ctx.Write(UID);
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(TerritoryID))
			return false;
		if (!ctx.Read(TerritoryName))
			return false;
		if (!ctx.Read(UID))
			return false;

		return true;
	}
};