/**
 * DayZPlayerSyncJunctures.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZPlayerSyncJunctures
{
	static const int EXPANSION_SJ_NEXT_LINK = 100;
	static const int EXPANSION_SJ_GET_IN_TRANSPORT_UNLINK = 101;
	static const int EXPANSION_SJ_PERFORM_CLIMB = 102;
	
	static void ExpansionSendNextLink( DayZPlayer pPlayer )
	{
		ScriptJunctureData ctx = new ScriptJunctureData;
		
		pPlayer.SendSyncJuncture( EXPANSION_SJ_NEXT_LINK, ctx );
	}
	
	static bool ExpansionReadNextLink( ParamsReadContext pCtx )
	{
		return true;
	}
	
	static void ExpansionSendGetInTransportUnlink( DayZPlayer pPlayer )
	{
		ScriptJunctureData ctx = new ScriptJunctureData;
		
		pPlayer.SendSyncJuncture( EXPANSION_SJ_GET_IN_TRANSPORT_UNLINK, ctx );
	}
	
	static bool ExpansionReadGetInTransportUnlink( ParamsReadContext pCtx )
	{
		return true;
	}
	
	static void ExpansionSendPerformClimb( DayZPlayer pPlayer, bool performClimb, bool performClimbAttach )
	{
		ScriptJunctureData ctx = new ScriptJunctureData;
		
		ctx.Write( performClimb );
		ctx.Write( performClimbAttach );

		pPlayer.SendSyncJuncture( EXPANSION_SJ_PERFORM_CLIMB, ctx );
	}
	
	static bool ExpansionReadPerformClimb( ParamsReadContext pCtx, out bool performClimb, out bool performClimbAttach )
	{
		pCtx.Read( performClimb );
		pCtx.Read( performClimbAttach );
		return true;
	}
};