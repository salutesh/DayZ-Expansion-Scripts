/**
 * DayZPlayerSyncJunctures.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZPlayerSyncJunctures
{
	static const int EXPANSION_SJ = 100;

	[Obsolete("Use DayZPlayerImplement::Expansion_Teleport")]
	static void ExpansionTeleport(DayZPlayer pPlayer, vector position, vector orientation = "0 0 0")
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PLAYER, DayZPlayerSyncJunctures, "" + pPlayer, "" + position, "" + orientation);
	#endif

		DayZPlayerImplement player;
		if (Class.CastTo(player, pPlayer))
			player.Expansion_Teleport(position, orientation);
	}
};
