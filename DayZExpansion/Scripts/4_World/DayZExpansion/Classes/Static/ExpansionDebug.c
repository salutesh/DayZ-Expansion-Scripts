/**
 * ExpansionDebug.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

static string Expansion_Debug_Player( Man man )
{
	PlayerBase player = PlayerBase.Cast( man );
	if ( player )
	{
		return "[" + player.GetIdentityName() + ":" + player.GetIdentityUID() + "] ";
	}

	if ( man )
	{
		return "[Unknown Player] ";
	}

	return "[NULL Player] ";
};