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

	void OnSend( ParamsWriteContext ctx )
	{
		ctx.Write( m_Health );
		ctx.Write( m_Blood );
		ctx.Write( m_Water );
		ctx.Write( m_Energy );
		ctx.Write( m_Stamina );
		ctx.Write( m_Karma );
		ctx.Write( m_Distance );
		ctx.Write( m_Playtime );
		ctx.Write( m_PlayersKilled );
		ctx.Write( m_InfectedKilled );
		ctx.Write( m_AnimalsKilled );
		ctx.Write( m_LongestShot );
		ctx.Write( m_Weight );
	}

	bool OnRecieve( ParamsReadContext ctx )
	{
		ctx.Read( m_Health );
		ctx.Read( m_Blood );
		ctx.Read( m_Water );
		ctx.Read( m_Energy );
		ctx.Read( m_Stamina );
		ctx.Read( m_Karma );
		ctx.Read( m_Distance );
		ctx.Read( m_Playtime );
		ctx.Read( m_PlayersKilled );
		ctx.Read( m_InfectedKilled );
		ctx.Read( m_AnimalsKilled );
		ctx.Read( m_LongestShot );
		ctx.Read( m_Weight );

		return true;
	}
};