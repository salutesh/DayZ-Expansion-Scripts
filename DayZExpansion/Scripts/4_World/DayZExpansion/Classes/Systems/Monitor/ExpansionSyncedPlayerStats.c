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

class ExpansionSyncedPlayerStates
{
	int m_Bones;
	int m_Sick;
	int m_Cholera;
	int m_Influenza;
	int m_Salmonella;
	int m_Poison;
	int m_Infection;

	ref array<int> m_States = new array<int>;

	void OnSend( ParamsWriteContext ctx )
	{
		m_States = {m_Bones, m_Sick, m_Cholera, m_Influenza, m_Salmonella, m_Poison, m_Infection};

		ctx.Write( m_States );
	}

	bool OnRecieve( ParamsReadContext ctx )
	{
		ctx.Read( m_States );
		
		SetStatest();

		return true;
	}
	
	private void SetStatest()
	{
		if (m_States && m_States.Count() > 0)
		{
			m_Bones = m_States.Get(0);
			m_Sick = m_States.Get(1);
			m_Cholera = m_States.Get(2);
			m_Influenza = m_States.Get(3);
			m_Salmonella = m_States.Get(4);
			m_Poison = m_States.Get(5);
			m_Infection = m_States.Get(6);
		}
	}
};

class ExpansionSyncedPlayerStats
{
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