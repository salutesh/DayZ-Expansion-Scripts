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

		ctx.Write(FactionReputation.Count());
		
		foreach (int factionID, int factionRep: FactionReputation)
		{
			ctx.Write(factionID);
			ctx.Write(factionRep);
		}

		ctx.Write(FactionID);

		ctx.Write(PersonalStorageLevel);
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
			
			FactionReputation[factionID] = factionRep;
		}

		if (ConfigVersion < 7)
			return true;

		if (!ctx.Read(FactionID))
		{
			Error(ToString() + "::OnRead FactionID");
			return false;
		}

		if (FactionID != -1)
			FactionReputation[FactionID] = Reputation;

		if (ConfigVersion < 8)
			return true;

		if (!ctx.Read(PersonalStorageLevel))
		{
			Error(ToString() + "::OnRead PersonalStorageLevel");
			return false;
		}

		return true;
	}
	
	protected int GetReputationByFactionID(int factionID)
	{
		return FactionReputation[factionID];
	}
};
