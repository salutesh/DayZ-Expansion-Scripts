/**
 * ExpansionTerritory.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionOldTerritory
{
	int TerritoryID;
	string TerritoryName;
	vector TerritoryPosition;
	int TerritoryLevel;
	string TerritoryOwnerID;
	int TerritoryFlagTextureID;
	autoptr array< ref ExpansionTerritoryMember > TerritoryMembers;
	autoptr array< ref ExpansionTerritoryInvite > Invites;
}

class ExpansionTerritory
{
	private int TerritoryID;
	private string TerritoryName;
	private vector TerritoryPosition;
	private int TerritoryLevel;
	private string TerritoryOwnerID;
	private string TerritoryFlagTexturePath;
	private autoptr array< ref ExpansionTerritoryMember > TerritoryMembers;
	private autoptr array< ref ExpansionTerritoryInvite > Invites;
	
	// ------------------------------------------------------------
	// ExpansionTerritory Constructor
	// ------------------------------------------------------------
	void ExpansionTerritory(int id, string name, int level, string ownerID, vector position, string texturePath)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.TERRITORY, this, "ExpansionTerritory");
#endif

		TerritoryID = id;
		TerritoryName = name;
		TerritoryPosition = position;
		TerritoryLevel = level;
		TerritoryOwnerID = ownerID;
		TerritoryFlagTexturePath = texturePath;
		TerritoryMembers = new array<ref ExpansionTerritoryMember>;
		Invites = new array<ref ExpansionTerritoryInvite>;
	}
	
	// ------------------------------------------------------------
	// ExpansionTerritory Destructor
	// ------------------------------------------------------------
	void ~ExpansionTerritory()
	{
		
	}
	
	// ------------------------------------------------------------
	// Expansion NumberOfMembers
	// ------------------------------------------------------------
	int NumberOfMembers()
	{
		return TerritoryMembers.Count();
	}
	
	// ------------------------------------------------------------
	// Expansion SetMembers
	// ------------------------------------------------------------
	void SetMembers( array<ref ExpansionTerritoryMember> members)
	{
		TerritoryMembers = members;
	}
	
	// ------------------------------------------------------------
	// Expansion AddMember
	// ------------------------------------------------------------
	void AddMember(string id, string name, bool owner = false)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.TERRITORY, this, "AddMember");
#endif

		ExpansionTerritoryMember newMember = new ExpansionTerritoryMember(id, name, owner);
		
		TerritoryMembers.Insert(newMember);

		CF_Log.Debug("ExpansionTerritory::AddMember:: - Added new member [ID: " + id + ", Name: " + name + "] to territory [ID:" + TerritoryID.ToString() + ", Name: " + TerritoryName + "].");
	}
	
	// ------------------------------------------------------------
	// Expansion IsMember
	// ------------------------------------------------------------
	bool IsMember(string uid)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.TERRITORY, this, "IsMember");
#endif

		for (int i = 0; i < TerritoryMembers.Count(); ++i)
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.TERRITORY, this, "RemoveMember");
#endif

		int memberIndex = TerritoryMembers.Find(member);
		
		if ( memberIndex > -1 )
		{
			TerritoryMembers.Remove( memberIndex );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion GetMember
	// ------------------------------------------------------------	
	ExpansionTerritoryMember GetMember(string uid)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.TERRITORY, this, "GetMember");
#endif

		for(int i = 0; i < TerritoryMembers.Count(); ++i)
		{
			if (TerritoryMembers[i] && TerritoryMembers[i].GetID() == uid)
			{
				return TerritoryMembers[i];
			}
		}
		
		return null;
	}
	
	// ------------------------------------------------------------
	// Expansion SetInvites
	// ------------------------------------------------------------
	void SetInvites( array<ref ExpansionTerritoryInvite> invites)
	{
		Invites = invites;
	}
	
	// ------------------------------------------------------------
	// Expansion AddTerritoryInvite
	// ------------------------------------------------------------	
	bool AddTerritoryInvite(ExpansionTerritoryInvite invite)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.TERRITORY, this, "AddTerritoryInvite");
#endif

		if (!invite) return false;
		
		int idx = -1;
		for (int i = 0; i < Invites.Count(); ++i)
		{
			if (Invites[i] && Invites[i].UID == invite.UID)
			{
				idx = i;
				break;
			}
		}
		
		if (idx > -1)
		{
			return false;
		}
		
		Invites.Insert( invite );
		return true;
	}
	
	// ------------------------------------------------------------
	// Expansion RemoveTerritoryInvite
	// ------------------------------------------------------------		
	bool RemoveTerritoryInvite(string uid)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.TERRITORY, this, "RemoveTerritoryInvite");
#endif

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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.TERRITORY, this, "HasInvite");
#endif

		for (int i = 0; i < Invites.Count(); ++i)
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
	ExpansionTerritoryInvite GetInvite(string uid)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.TERRITORY, this, "GetInvite");
#endif

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
	array<ref ExpansionTerritoryInvite> GetTerritoryInvites()
	{
		return Invites;
	}
	
	// ------------------------------------------------------------
	// Expansion GetTerritoryMembers
	// ------------------------------------------------------------
	array<ref ExpansionTerritoryMember> GetTerritoryMembers()
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
	// Expansion GetFlagTexturePath
	// ------------------------------------------------------------
	string GetFlagTexturePath()
	{
		return TerritoryFlagTexturePath;
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
	
	// ------------------------------------------------------------
	// Expansion OnStoreSave
	// ------------------------------------------------------------
	void OnStoreSave_OLD( ParamsWriteContext ctx )
	{
		ctx.Write( TerritoryID );
		ctx.Write( TerritoryName );
		ctx.Write( TerritoryPosition );
		ctx.Write( TerritoryLevel );
		ctx.Write( TerritoryOwnerID );
		ctx.Write( TerritoryFlagTexturePath );
		ctx.Write( TerritoryMembers );
		ctx.Write( Invites );
	}
	
	// ------------------------------------------------------------
	// Expansion OnStoreLoad
	// ------------------------------------------------------------
	bool OnStoreLoad_OLD( ParamsReadContext ctx, int expansionVersion )
	{
		if ( Expansion_Assert_False( ctx.Read( TerritoryID ), "[" + this + "] Failed reading TerritoryID" ) )
			return false;
			
		if ( Expansion_Assert_False( ctx.Read( TerritoryName ), "[" + this + "] Failed reading TerritoryName" ) )
			return false;

		if ( Expansion_Assert_False( ctx.Read( TerritoryPosition ), "[" + this + "] Failed reading TerritoryPosition" ) )
			return false;

		if ( Expansion_Assert_False( ctx.Read( TerritoryLevel ), "[" + this + "] Failed reading TerritoryLevel" ) )
			return false;

		if ( Expansion_Assert_False( ctx.Read( TerritoryOwnerID ), "[" + this + "] Failed reading TerritoryOwnerID" ) )
			return false;

		if ( Expansion_Assert_False( ctx.Read( TerritoryFlagTexturePath ), "[" + this + "] Failed reading TerritoryFlagTexturePath" ) )
			return false;

		if ( Expansion_Assert_False( ctx.Read( TerritoryMembers ), "[" + this + "] Failed reading TerritoryMembers" ) )
			return false;

		if ( Expansion_Assert_False( ctx.Read( Invites ), "[" + this + "] Failed reading Invites" ) )
			return false;
		
		return true;
	}
	
	#ifdef EXPANSION_MODSTORAGE
	// ------------------------------------------------------------
	// Expansion OnStoreSave
	// ------------------------------------------------------------
	void OnStoreSave(CF_ModStorage ctx)
	{
		ctx.Write(TerritoryID);
		ctx.Write(TerritoryName);
		ctx.Write(TerritoryPosition);
		ctx.Write(TerritoryLevel);
		ctx.Write(TerritoryOwnerID);
		ctx.Write(TerritoryFlagTexturePath);

		int count;
		int index;

		index = 0;
		count = TerritoryMembers.Count();
		ctx.Write(count);
		while (index < count)
		{
			TerritoryMembers[index].OnStoreSave(ctx);
			index++;
		}

		index = 0;
		count = Invites.Count();
		ctx.Write(count);
		while (index < count)
		{
			Invites[index].OnStoreSave(ctx);
			index++;
		}
	}
	
	// ------------------------------------------------------------
	// Expansion OnStoreLoad
	// ------------------------------------------------------------
	bool OnStoreLoad(CF_ModStorage ctx)
	{
		if (!ctx.Read(TerritoryID))
			return false;
			
		if (!ctx.Read(TerritoryName))
			return false;

		if (!ctx.Read(TerritoryPosition))
			return false;

		if (!ctx.Read(TerritoryLevel))
			return false;

		if (!ctx.Read(TerritoryOwnerID))
			return false;

		if (!ctx.Read(TerritoryFlagTexturePath))
			return false;

		int count;
		int index;

		index = 0;
		if (!ctx.Read(count))
			return false;

		while ( index < count )
		{
			ExpansionTerritoryMember member = new ExpansionTerritoryMember();
			member.OnStoreLoad(ctx);
			TerritoryMembers.Insert(member); 
			index++;
		}

		index = 0;
		
		if (!ctx.Read(count))
			return false;

		while ( index < count )
		{
			ExpansionTerritoryInvite invite = new ExpansionTerritoryInvite();
			invite.OnStoreLoad(ctx);
			Invites.Insert(invite); 
			index++;
		}
		
		return true;
	}
	#endif
}