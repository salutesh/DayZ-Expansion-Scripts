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

	static const int EXPANSION_SJ_TELEPORT = 0;

	static void ExpansionTeleport(DayZPlayer pPlayer, vector position, vector orientation = "0 0 0")
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PLAYER, DayZPlayerSyncJunctures, "" + pPlayer, "" + position, "" + orientation);
	#endif

		auto rpc = new ScriptRPC;
		rpc.Write(EXPANSION_SJ_TELEPORT);

		rpc.Write(position);
		rpc.Write(orientation);

		rpc.Send(pPlayer, ExpansionScriptRPC.EXPANSION_RPC_SJ, true, pPlayer.GetIdentity());

		pPlayer.SetPosition(position);
		pPlayer.SetOrientation(orientation);

		DayZPlayerImplement player;
		if (Class.CastTo(player, pPlayer))
			player.Expansion_OnSyncJuncture(EXPANSION_SJ_TELEPORT, null);
	}

	static bool ExpansionReadTeleport(ParamsReadContext pCtx, out vector position, out vector orientation)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PLAYER, DayZPlayerSyncJunctures);
	#endif

		pCtx.Read(position);
		pCtx.Read(orientation);

		return true;
	}
};
