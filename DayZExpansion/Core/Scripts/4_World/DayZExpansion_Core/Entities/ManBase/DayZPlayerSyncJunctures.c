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

	static const int EXPANSION_SJ_NEXT_LINK = 1;
	static const int EXPANSION_SJ_GET_IN_TRANSPORT_UNLINK = 2;
	static const int EXPANSION_SJ_PERFORM_CLIMB = 3;
	static const int EXPANSION_SJ_UPDATE_TRANSFORM = 4;
	static const int EXPANSION_SJ_FORCE_UNLINK = 5;

	static void ExpansionTeleport(DayZPlayer pPlayer, vector position, vector orientation = "0 0 0")
	{
		auto trace = CF_Trace_0(EXTrace.PLAYER, null);

		ScriptJunctureData ctx = new ScriptJunctureData;
		ctx.Write(EXPANSION_SJ_TELEPORT);

		ctx.Write(position);
		ctx.Write(orientation);

		pPlayer.SendSyncJuncture(EXPANSION_SJ, ctx);
	}

	static bool ExpansionReadTeleport(ParamsReadContext pCtx, out vector position, out vector orientation)
	{
		auto trace = CF_Trace_0(EXTrace.PLAYER, null);

		pCtx.Read(position);
		pCtx.Read(orientation);

		return true;
	}

	static void ExpansionSendNextLink(DayZPlayer pPlayer)
	{
		auto trace = CF_Trace_0(EXTrace.PLAYER, null);

		ScriptJunctureData ctx = new ScriptJunctureData;
		ctx.Write(EXPANSION_SJ_NEXT_LINK);

		pPlayer.SendSyncJuncture(EXPANSION_SJ, ctx);
	}

	static bool ExpansionReadNextLink(ParamsReadContext pCtx)
	{
		auto trace = CF_Trace_0(EXTrace.PLAYER, null);

		return true;
	}

	static void ExpansionSendGetInTransportUnlink(DayZPlayer pPlayer)
	{
		auto trace = CF_Trace_0(EXTrace.PLAYER, null);

		ScriptJunctureData ctx = new ScriptJunctureData;
		ctx.Write(EXPANSION_SJ_GET_IN_TRANSPORT_UNLINK);

		pPlayer.SendSyncJuncture(EXPANSION_SJ, ctx);
	}

	static bool ExpansionReadGetInTransportUnlink(ParamsReadContext pCtx)
	{
		auto trace = CF_Trace_0(EXTrace.PLAYER, null);

		return true;
	}

	static void ExpansionSendPerformClimb(DayZPlayer pPlayer, bool performClimb, bool performClimbAttach)
	{
		auto trace = CF_Trace_0(EXTrace.PLAYER, null);

		ScriptJunctureData ctx = new ScriptJunctureData;
		ctx.Write(EXPANSION_SJ_PERFORM_CLIMB);

		ctx.Write(performClimb);
		ctx.Write(performClimbAttach);

		pPlayer.SendSyncJuncture(EXPANSION_SJ, ctx);
	}

	static bool ExpansionReadPerformClimb(ParamsReadContext pCtx, out bool performClimb, out bool performClimbAttach)
	{
		auto trace = CF_Trace_0(EXTrace.PLAYER, null);

		pCtx.Read(performClimb);
		pCtx.Read(performClimbAttach);
		return true;
	}

	static void ExpansionSendForceUnlink(DayZPlayer pPlayer)
	{
		auto trace = CF_Trace_0(EXTrace.PLAYER, null);

		ScriptJunctureData ctx = new ScriptJunctureData;
		ctx.Write(EXPANSION_SJ_FORCE_UNLINK);

		pPlayer.SendSyncJuncture(EXPANSION_SJ, ctx);
	}

	static bool ExpansionReadForceUnlink(ParamsReadContext pCtx)
	{
		auto trace = CF_Trace_0(EXTrace.PLAYER, null);

		return true;
	}
};
