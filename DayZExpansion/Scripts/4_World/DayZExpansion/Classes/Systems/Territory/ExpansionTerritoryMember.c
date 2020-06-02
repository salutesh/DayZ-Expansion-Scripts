/**
 * ExpansionTerritoryMember.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionTerritoryMember
{
	protected string m_ID;
	protected string m_Name;
	protected ExpansionTerritoryRank m_Rank;
	
	string GetID()
	{
		return m_ID;	
	}
	
	string GetName()
	{
		return m_Name;
	}
	
	string GetRankName()
	{
		switch (m_Rank)
		{
			case ExpansionTerritoryRank.ADMIN:
			{
				return "Admin";
			}
			break;
			
			case ExpansionTerritoryRank.MODERATOR:
			{
				return "Moderator";
			}
			break;
			
			case ExpansionTerritoryRank.MEMBER:
			{
				return "Member";
			}
			break;
		}
		
		return "Member";
	}
	
	ExpansionTerritoryRank GetRank()
	{
		return m_Rank;
	}
	
	void SetRank( ExpansionTerritoryRank rank )
	{
		m_Rank = rank;
	}
	
	void ExpansionTerritoryMember(string ID, string name, bool owner = false)
	{
		m_ID = ID;
		m_Name = name;
		
		if ( owner ) 
		{
			m_Rank = ExpansionTerritoryRank.ADMIN;
		}
		else
		{
			m_Rank = ExpansionTerritoryRank.MEMBER;
		}
	}
}