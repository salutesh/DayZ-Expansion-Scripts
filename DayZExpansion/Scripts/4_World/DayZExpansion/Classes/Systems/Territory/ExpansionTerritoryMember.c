/**
 * ExpansionTerritoryMember.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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

	#ifdef CF_MODULE_MODSTORAGE
	// ------------------------------------------------------------
	// Expansion OnStoreSave
	// ------------------------------------------------------------
	void OnStoreSave( CF_ModStorage storage )
	{
		storage.Write( m_ID );
		storage.Write( m_Name );
		storage.Write( m_Rank );
	}

	// ------------------------------------------------------------
	// Expansion OnStoreLoad
	// ------------------------------------------------------------
	bool OnStoreLoad( CF_ModStorage storage )
	{
		if ( Expansion_Assert_False( storage.Read( m_ID ), "[" + this + "] Failed reading m_ID" ) )
			return false;
		if ( Expansion_Assert_False( storage.Read( m_Name ), "[" + this + "] Failed reading m_Name" ) )
			return false;
		if ( Expansion_Assert_False( storage.Read( m_Rank ), "[" + this + "] Failed reading m_Rank" ) )
			return false;

		return true;
	}
	#endif
	
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
				return "#STR_EXPANSION_PARTY_RANK_ADMIN";
			}
			break;
			
			case ExpansionTerritoryRank.MODERATOR:
			{
				return "#STR_EXPANSION_PARTY_RANK_MOD";
			}
			break;
			
			case ExpansionTerritoryRank.MEMBER:
			{
				return "#STR_EXPANSION_PARTY_RANK_MEMBER";
			}
			break;
		}
		
		return "#STR_EXPANSION_PARTY_RANK_MEMBER";
	}
	
	ExpansionTerritoryRank GetRank()
	{
		return m_Rank;
	}
	
	void SetRank( ExpansionTerritoryRank rank )
	{
		m_Rank = rank;
	}
	
	void ExpansionTerritoryMember(string ID = "", string name = "", bool owner = false)
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
};