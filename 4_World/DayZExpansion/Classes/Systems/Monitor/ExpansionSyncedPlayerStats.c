/**
 * ExpansionSyncedPlayerStats.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionSyncedPlayerStats
 * @brief		This class handles the sycronised player stats values
 **/

class ExpansionSyncedPlayerStats
{
	string m_PlayerUID;
	
	int m_Health;
	int m_Blood;
	int m_Water;
	int m_Energy;
	int m_Stamina;
	int m_Karma;
	
	float m_Distance;
	float m_Playtime;
	float m_PlayersKilled;
	float m_InfectedKilled;
	float m_AnimalsKilled;
	float m_LongestShot;
	
	int m_Weight;
}