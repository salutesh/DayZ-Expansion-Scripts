/**
 * ExpansionHardlinePlayerData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionHardlinePlayerData
{
	static const int CONFIGVERSION = 8;

	int ConfigVersion;
	int Reputation;

	ref map<int, int> FactionReputation; //! Hash map with all that faction IDs and their respective reputation. -1 is the key for the normal reputation.
	int FactionID;  //! Most recent faction the player was part of

	int PersonalStorageLevel = 1;

	bool m_FactionSynchDirty;

	void ExpansionHardlinePlayerData()
	{
		ConfigVersion = CONFIGVERSION;

		FactionReputation = new map<int, int>;
		FactionID = -1;
	}
	
	void Save(string fileName)
	{
		FileSerializer file = new FileSerializer();

		if (!FileExist(EXPANSION_HARDLINE_PLAYERDATA_FOLDER))
			ExpansionStatic.MakeDirectoryRecursive(EXPANSION_HARDLINE_PLAYERDATA_FOLDER);

		if (file.Open(EXPANSION_HARDLINE_PLAYERDATA_FOLDER + fileName + ".bin", FileMode.WRITE))
		{
			file.Write(ConfigVersion);

			OnWrite(file);

			file.Close();
		}
	}

	bool Load(string fileName)
	{
		string path = EXPANSION_HARDLINE_PLAYERDATA_FOLDER + fileName + ".bin";

		if (!FileExist(EXPANSION_HARDLINE_PLAYERDATA_FOLDER))
			ExpansionStatic.MakeDirectoryRecursive(EXPANSION_HARDLINE_PLAYERDATA_FOLDER);
		
		if (FileExist(path))
		{
			FileSerializer file = new FileSerializer();
			if (file.Open(path, FileMode.READ))
			{
				file.Read(ConfigVersion);

				OnRead(file);

				if (Reputation < 0)
					Reputation = 0;

				if (PersonalStorageLevel < 1)
					PersonalStorageLevel = 1;

				file.Close();
			}
			ConfigVersion = CONFIGVERSION;
			return true;
		}
		
		return false;
	}
	
	void OnWrite(ParamsWriteContext ctx)
	{
		ctx.Write(Reputation);
		EXTrace.Print(EXTrace.HARDLINE, this, "OnWrite Reputation " + Reputation);

		WriteFactionReputation(ctx);

		ctx.Write(FactionID);
		EXTrace.Print(EXTrace.HARDLINE, this, "OnWrite FactionID " + FactionID);

		ctx.Write(PersonalStorageLevel);
		EXTrace.Print(EXTrace.HARDLINE, this, "OnWrite PersonalStorageLevel " + PersonalStorageLevel);
	}

	void WriteFactionReputation(ParamsWriteContext ctx, bool includeCurrentFaction = true)
	{
		auto trace = EXTrace.Start(EXTrace.HARDLINE, this, "includeCurrentFaction " + includeCurrentFaction);

		int factionCount = FactionReputation.Count();

		//! If faction ID is identical to current faction, do not write separate entry since Reputation is faction rep
		//! (only used for sending to client, not when saving to disk)
		if (!includeCurrentFaction && FactionReputation.Contains(FactionID))
			factionCount--;

		ctx.Write(factionCount);
		
		foreach (int factionID, int factionRep: FactionReputation)
		{
			if (!includeCurrentFaction && factionID == FactionID)
				continue;

			ctx.Write(factionID);
			ctx.Write(factionRep);
			
		#ifdef EXPANSIONMODAI
		#ifdef DIAG
			typename factionType = eAIFaction.GetTypeByID(factionID);
			EXTrace.Print(EXTrace.HARDLINE, this, "WriteFactionReputation " + factionType + " " + factionRep);
		#endif
		#endif
		}
	}

	bool OnRead(ParamsReadContext ctx)
	{
		int rank;
		if (ConfigVersion < 3 && !ctx.Read(rank))
		{
			Error(ToString() + "::OnRead Rank");
			return false;
		}
		
		if (!ctx.Read(Reputation))
		{
			Error(ToString() + "::OnRead Reputation");
			return false;
		}

		EXTrace.Print(EXTrace.HARDLINE, this, "OnRead Reputation " + Reputation);

		if (ConfigVersion < 4)
		{
			int sanity;
			if (!ctx.Read(sanity))
			{
				Error(ToString() + "::OnRead Sanity");
				return false;
			}
			
			int heroKills;
			if (!ctx.Read(heroKills))
			{
				Error(ToString() + "::OnRead HeroKills");
				return false;
			}
			
			int banditKills;
			if (!ctx.Read(banditKills))
			{
				Error(ToString() + "::OnRead BanditKills");
				return false;
			}
			
			int bambiKills;
			if (!ctx.Read(bambiKills))
			{
				Error(ToString() + "::OnRead BambiKills");
				return false;
			}
			
			int totalPlayerKills;
			if (ConfigVersion < 3 && !ctx.Read(totalPlayerKills))
			{
				Error(ToString() + "::OnRead TotalPlayerKills");
				return false;
			}
		}
		else if (ConfigVersion == 4)
		{
			int playerKills;
			if (!ctx.Read(playerKills))
			{
				Error(ToString() + "::OnRead PlayerKills");
				return false;
			}
		}
		
		if (ConfigVersion < 5)
		{
			int aiKills;
			if (!ctx.Read(aiKills))
			{
				Error(ToString() + "::OnRead AIKills");
				return false;
			}
			
			int infectedKills;
			if (!ctx.Read(infectedKills))
			{
				Error(ToString() + "::OnRead InfectedKills");
				return false;
			}

			int playerDeaths;
			if (!ctx.Read(playerDeaths))
			{
				Error(ToString() + "::OnRead PlayerDeaths");
				return false;
			}
		}

		if (ConfigVersion < 6)
			return true;

		if (!ReadFactionReputation(ctx))
			return false;

		if (ConfigVersion < 7)
			return true;

		if (!ctx.Read(FactionID))
		{
			Error(ToString() + "::OnRead FactionID");
			return false;
		}

		EXTrace.Print(EXTrace.HARDLINE, this, "OnRead FactionID " + FactionID);

		if (FactionID != -1)
			SetFactionReputation(FactionID, Reputation);

		if (ConfigVersion < 8)
			return true;

		if (!ctx.Read(PersonalStorageLevel))
		{
			Error(ToString() + "::OnRead PersonalStorageLevel");
			return false;
		}

		EXTrace.Print(EXTrace.HARDLINE, this, "OnRead PersonalStorageLevel " + PersonalStorageLevel);

		return true;
	}
	
	bool ReadFactionReputation(ParamsReadContext ctx)
	{
		int factionRepCount;
		if (!ctx.Read(factionRepCount))
		{
			Error(ToString() + "::OnRead factionRepCount");
			return false;
		}
		
		for (int i = 0; i < factionRepCount; i++)
		{
			int factionID;
			if (!ctx.Read(factionID))
			{
				Error(ToString() + "::OnRead factionID");
				return false;
			}
			
			int factionRep;
			if (!ctx.Read(factionRep))
			{
				Error(ToString() + "::OnRead factionID");
				return false;
			}
			
		#ifdef EXPANSIONMODAI
		#ifdef DIAG
			typename factionType = eAIFaction.GetTypeByID(factionID);
			EXTrace.Print(EXTrace.HARDLINE, this, "ReadFactionReputation " + factionType + " " + factionRep);
		#endif
		#endif

			SetFactionReputation(factionID, factionRep);
		}

		return true;
	}

	int GetReputationByFactionID(int factionID)
	{
		return FactionReputation[factionID];
	}
	
	void SetFactionReputation(int factionID, int rep, bool forceSynchDirty = false)
	{
		if (forceSynchDirty || (factionID != FactionID && FactionReputation[factionID] != rep))
			m_FactionSynchDirty = true;
			
		if (rep > 0)
			FactionReputation[factionID] = rep;
		else
			FactionReputation.Remove(factionID);
	}

	//! Server
	void SendFactionReputation(PlayerIdentity identity)
	{
		auto trace = EXTrace.Start(EXTrace.HARDLINE, this, "synch dirty? " + m_FactionSynchDirty);

		if (!m_FactionSynchDirty)
			return;

		auto rpc = ExpansionScriptRPC.Create();
		WriteFactionReputation(rpc, false);
		rpc.Send(null, ExpansionHardlineModuleRPC.FactionReputationSync, true, identity);

		m_FactionSynchDirty = false;
	}
};
