/**
 * ExpansionTerritory.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionTerritory
{
	private int TerritoryID;
	private string TerritoryName;
	private vector TerritoryPosition;
	private int TerritoryLevel;
	private string TerritoryOwnerID;
	private int TerritoryFlagTextureID;
	private autoptr array< ref ExpansionTerritoryMember > TerritoryMembers;
	private autoptr array< ref ExpansionTerritoryInvite > Invites;
	
	// ------------------------------------------------------------
	// ExpansionTerritory Constructor
	// ------------------------------------------------------------
	void ExpansionTerritory(int id, string name, int level, string ownerID, vector position, int textureID)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionTerritory::ExpansionTerritory:: - Start");
		#endif
		
		TerritoryID = id;
		TerritoryName = name;
		TerritoryPosition = position;
		TerritoryLevel = level;
		TerritoryOwnerID = ownerID;
		TerritoryFlagTextureID = textureID;
		TerritoryMembers = new array<ref ExpansionTerritoryMember>;
		Invites = new array<ref ExpansionTerritoryInvite>;
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionTerritory::ExpansionTerritory:: - Start");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritory Destructor
	// ------------------------------------------------------------
	void ~ExpansionTerritory()
	{
		
	}
	
	// ------------------------------------------------------------
	// Expansion AddMember
	// ------------------------------------------------------------
	void AddMember(string id, string name, bool owner = false)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionTerritory::AddMember:: - Start");
		#endif
		
		ExpansionTerritoryMember newMember = new ExpansionTerritoryMember(id, name, owner);
		
		TerritoryMembers.Insert(newMember);

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionTerritory::AddMember:: - Added new member [ID: " + id + ", Name: " + name + "] to territory [ID:" + TerritoryID.ToString() + ", Name: " + TerritoryName + "].");
		#endif
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionTerritory::AddMember:: - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion IsMember
	// ------------------------------------------------------------
	bool IsMember(string uid)
	{
		for(int i = 0; i < TerritoryMembers.Count(); ++i)
		{
			if (TerritoryMembers[i] && TerritoryMembers[i].GetID() == uid)
			{
				return true;
			}
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion AddMember
	// ------------------------------------------------------------
	void RemoveMember(ExpansionTerritoryMember member)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionTerritory::RemoveMember:: - Start");
		#endif
		
		int memberIndex = TerritoryMembers.Find(member);
		
		if ( memberIndex > -1 )
		{
			TerritoryMembers.Remove( memberIndex );
		}
	
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionTerritory::RemoveMember:: - End");
		#endif
	}
	
	ref ExpansionTerritoryMember GetMember(string uid)
	{
		for(int i = 0; i < TerritoryMembers.Count(); ++i)
		{
			if (TerritoryMembers[i] && TerritoryMembers[i].GetID() == uid)
			{
				return TerritoryMembers[i];
			}
		}
		
		return null;
	}
	
	void AddTerritoryInvite(ExpansionTerritoryInvite invite)
	{
		if (!invite) return;
		Invites.Insert( invite );
	}
	
	bool RemoveTerritoryInvite(string uid)
	{
		int idx = -1;
		for (int i = 0; i < Invites.Count(); ++i)
		{
			if (Invites[i] && Invites[i].UID == uid)
			{
				idx = i;
				break;
			}
		}
		
		if (idx > -1)
		{
			Invites.Remove(idx);
			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion HasInvite
	// ------------------------------------------------------------
	bool HasInvite(string uid)
	{
		for(int i = 0; i < Invites.Count(); ++i)
		{
			if (Invites[i] && Invites[i].UID == uid)
			{
				return true;
			}
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion HasInvite
	// ------------------------------------------------------------
	ref ExpansionTerritoryInvite GetInvite(string uid)
	{
		for (int i = 0; i < Invites.Count(); ++i)
		{
			if (Invites[i] && Invites[i].UID == uid)
			{
				return Invites[i];
			}
		}
		
		return null;
	}
	
	// ------------------------------------------------------------
	// Expansion GetTerritoryMembers
	// ------------------------------------------------------------
	ref array<ref ExpansionTerritoryInvite> GetTerritoryInvites()
	{
		return Invites;
	}
	
	// ------------------------------------------------------------
	// Expansion GetTerritoryMembers
	// ------------------------------------------------------------
	ref array<ref ExpansionTerritoryMember> GetTerritoryMembers()
	{
		return TerritoryMembers;
	}
	
	// ------------------------------------------------------------
	// Expansion GetTerritoryName
	// ------------------------------------------------------------
	string GetTerritoryName()
	{
		return TerritoryName;
	}
	
	// ------------------------------------------------------------
	// Expansion GetOwnerID
	// ------------------------------------------------------------
	string GetOwnerID()
	{
		return TerritoryOwnerID;
	}
	
	// ------------------------------------------------------------
	// Expansion GetPosition
	// ------------------------------------------------------------
	vector GetPosition()
	{
		return TerritoryPosition;
	}
	
	// ------------------------------------------------------------
	// Expansion GetFlagTextureID
	// ------------------------------------------------------------
	int GetFlagTextureID()
	{
		return TerritoryFlagTextureID;
	}
	
	// ------------------------------------------------------------
	// Expansion GetTerritoryID
	// ------------------------------------------------------------
	int GetTerritoryID()
	{
		return TerritoryID;
	}
	
	// ------------------------------------------------------------
	// Expansion GetTerritoryLevel
	// ------------------------------------------------------------
	int GetTerritoryLevel()
	{
		return TerritoryLevel;
	}
}