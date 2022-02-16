/**
 * ExpansionKillfeedModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifndef JM_COT
//! TODO: This is a stub. Could in theory be extended to support other admin tools
class ExpansionPermissionsManager
{
	void ExpansionPermissionsManager()
	{
	}

	void ~ExpansionPermissionsManager()
	{
	}

	void RegisterPermission( string permission )
	{
	}

	bool HasPermission( string permission )
	{
		return false;
	}

	bool HasPermission( string permission, PlayerIdentity ihp )
	{
		return false;
	}
}

ref ExpansionPermissionsManager g_ExpansionPermissionsManager;

ref ExpansionPermissionsManager GetPermissionsManager()
{
	if ( !g_ExpansionPermissionsManager )
	{
		g_ExpansionPermissionsManager = new ExpansionPermissionsManager();
	}

	Assert_Null( g_ExpansionPermissionsManager );

	return g_ExpansionPermissionsManager;
}
#endif
