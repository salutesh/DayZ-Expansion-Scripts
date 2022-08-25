/**
 * AnalyticsManagerServer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class AnalyticsManagerServer
{
	override void OnPlayerConnect( Man player )
	{
		if (GetExpansionSettings().GetSpawn().EnableSpawnSelection)
		{
			//! Register vanilla stats except distance and playtime
			player.StatRegister( STAT_PLAYERS_KILLED );
			player.StatRegister( STAT_INFECTED_KILLED );
			player.StatRegister( STAT_LONGEST_SURVIVOR_HIT );

			//player.StatSyncToClient();
		}
		else
		{
			super.OnPlayerConnect( player );
		}
	}
}
