/**
 * MainMenuStats.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifndef EXPANSION_MAINMENU_NEW_DISABLE
modded class MainMenuStats
{
	override void UpdateStats()
	{
		PlayerBase player;
		MissionMainMenu mission_main_menu = MissionMainMenu.Cast(GetGame().GetMission());

	#ifdef PLATFORM_WINDOWS
		if (mission_main_menu.GetIntroSceneExpansion())
			player = mission_main_menu.GetIntroSceneExpansion().GetIntroCharacter().GetCharacterObj();

		if (mission_main_menu.GetIntroScenePC())
			player = mission_main_menu.GetIntroScenePC().GetIntroCharacter().GetCharacterObj();
	#endif

		if (player)
		{
			float stat_value;
			string stat_text;

			m_TimeSurvivedValue.SetText(ExpansionStatic.GetTimeString(player.StatGet(AnalyticsManagerServer.STAT_PLAYTIME)));
			m_PlayersKilledValue.SetText(ExpansionStatic.GetValueString(player.StatGet(AnalyticsManagerServer.STAT_PLAYERS_KILLED)));
			m_InfectedKilledValue.SetText(ExpansionStatic.GetValueString(player.StatGet(AnalyticsManagerServer.STAT_INFECTED_KILLED)));
			m_DistanceTraveledValue.SetText(ExpansionStatic.GetDistanceString(player.StatGet(AnalyticsManagerServer.STAT_DISTANCE)));
			m_LongRangeShotValue.SetText(ExpansionStatic.GetDistanceString(player.StatGet(AnalyticsManagerServer.STAT_LONGEST_SURVIVOR_HIT), true));
		}
	}
};
#endif
