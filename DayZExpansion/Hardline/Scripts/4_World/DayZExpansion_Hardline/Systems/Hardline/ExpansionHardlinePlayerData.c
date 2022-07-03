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

enum ExpansionPlayerRank
{
	BAMBI = 1,
	SURVIVOR,
	//! Hero player ranks
	SCOUT,
	PATHFINDER,
	HERO,
	SUPERHERO,
	LEGEND,
	//! Bandit player ranks
	KLEPTOMANIAC,
	BULLY,
	BANDIT,
	KILLER,
	MADMAN
};

class ExpansionHardlinePlayerData
{
	static const int CONFIGVERSION = 2;

	int ConfigVersion;
	
	private int Rank = ExpansionPlayerRank.BAMBI;
	
	private int Humanity = 100;
	private int Sanity = 100;
	
	private int HeroKills = 0;
	private int BanditKills = 0;
	private int BambiKills = 0;
	private int TotalPlayerKills = 0;
	private int AIKills = 0;
	private int InfectedKills = 0;
	private int PlayerDeaths = 0;
	
	[NonSerialized()]
	ExpansionHardlineModule m_HardlineModule;

	void ExpansionHardlinePlayerData()
	{
		ConfigVersion = CONFIGVERSION;
	}
	
	int GetRank()
	{
		return Rank;
	}
	
	int GetHumanity()
	{
		return Humanity;
	}
	
	void AddHumanity(int humanity)
	{
		Humanity += humanity;
		UpdateRankFromHumanity();
	}
	
	void RemoveHumanity(int humanity)
	{
		Humanity -= humanity;
		UpdateRankFromHumanity();
	}
	
	void ResetHumanity()
	{
		Humanity = 100;
		UpdateRankFromHumanity();
	}
	
	int GetSanity()
	{
		return Sanity;
	}
	
	void AddSanity(int sanity)
	{
		Sanity += sanity;
	}
	
	void RemoveSanity(int sanity)
	{
		Sanity -= sanity;
	}
	
	void UpdateRankFromHumanity()
	{
		int rank = 0;
		if (Humanity >= GetExpansionSettings().GetHardline().RankScout)
		{
			if (Humanity >= GetExpansionSettings().GetHardline().RankScout)
			{
				rank = ExpansionPlayerRank.SCOUT;
			}
			else if (Humanity >= GetExpansionSettings().GetHardline().RankPathfinder)
			{
				rank = ExpansionPlayerRank.PATHFINDER;
			}
			else if (Humanity >= GetExpansionSettings().GetHardline().RankHero)
			{
				rank = ExpansionPlayerRank.HERO;
			}
			else if (Humanity >= GetExpansionSettings().GetHardline().RankSuperhero)
			{
				rank = ExpansionPlayerRank.SUPERHERO;
			}
			else if (Humanity >= GetExpansionSettings().GetHardline().RankLegend)
			{
				rank = ExpansionPlayerRank.LEGEND;
			}
		}
		else if (Humanity <= GetExpansionSettings().GetHardline().RankKleptomaniac)
		{
			if (Humanity <= GetExpansionSettings().GetHardline().RankKleptomaniac)
			{
				rank = ExpansionPlayerRank.KLEPTOMANIAC;
			}
			else if (Humanity <= GetExpansionSettings().GetHardline().RankBully)
			{
				rank = ExpansionPlayerRank.BULLY;
			}
			else if (Humanity <= GetExpansionSettings().GetHardline().RankBandit)
			{
				rank = ExpansionPlayerRank.BANDIT;
			}
			else if (Humanity <= GetExpansionSettings().GetHardline().RankKiller)
			{
				rank = ExpansionPlayerRank.KILLER;
			}
			else if (Humanity <= GetExpansionSettings().GetHardline().RankMadman)
			{
				rank = ExpansionPlayerRank.MADMAN;
			}
		}
		
		Rank = rank;
	}
	
	string GetRankName()
	{
		string rankName;
		switch (Rank)
		{
			case ExpansionPlayerRank.BAMBI:
				rankName = "Bambi";
				break;
			case ExpansionPlayerRank.SURVIVOR:
				rankName = "Survivor";
				break;
			case ExpansionPlayerRank.SCOUT:
				rankName = "Scout";
				break;
			case ExpansionPlayerRank.PATHFINDER:
				rankName = "Pathfinder";
				break;
			case ExpansionPlayerRank.HERO:
				rankName = "Hero";
				break;
			case ExpansionPlayerRank.SUPERHERO:
				rankName = "Superhero";
				break;
			case ExpansionPlayerRank.LEGEND:
				rankName = "Legend";
				break;
			case ExpansionPlayerRank.KLEPTOMANIAC:
				rankName = "Kleptomaniac";
				break;
			case ExpansionPlayerRank.BULLY:
				rankName = "Bully";
				break;
			case ExpansionPlayerRank.BANDIT:
				rankName = "Bandit";
				break;
			case ExpansionPlayerRank.KILLER:
				rankName = "Killer";
				break;
			case ExpansionPlayerRank.MADMAN:
				rankName = "Madman";
				break;
		}
		return rankName;
	}
	
	void OnKillBandit()
	{
		BanditKills++;
		TotalPlayerKills++;
	}
	
	int GetBanditKills()
	{
		return BanditKills;
	}
	
	void OnKillHero()
	{
		HeroKills++;
		TotalPlayerKills++;
	}
	
	int GetHeroKills()
	{
		return HeroKills;
	}
	
	void OnKillBambi()
	{
		BambiKills++;
		TotalPlayerKills++;
	}
	
	int GetBambiKills()
	{
		return BambiKills;
	}
	
	int GetTotalPlayerKills()
	{
		return TotalPlayerKills;
	}
	
	void OnAIKilled()
	{
		AIKills++;
	}
	
	int GetAIKills()
	{
		return AIKills;
	}
	
	void OnInfectedKilled()
	{
		InfectedKills++;
	}
	
	int GetInfectedKills()
	{
		return InfectedKills;
	}
	
	void OnPlayerDeath()
	{
		PlayerDeaths++;
	}
	
	int GetPlayerDeaths()
	{
		return PlayerDeaths;
	}
	
	void Save(string fileName)
	{
		FileSerializer file = new FileSerializer();
		if (file.Open(EXPANSION_HARDLINE_PLAYERDATA_FOLDER + fileName + ".bin", FileMode.WRITE))
		{
			file.Write(ConfigVersion);

			OnSend(file);

			file.Close();
		}
	}
	
	bool Load(string fileName)
	{
		string legacyPath = EXPANSION_HARDLINE_PLAYERDATA_FOLDER + fileName + ".JSON";
		string path = EXPANSION_HARDLINE_PLAYERDATA_FOLDER + fileName + ".bin";
		if (FileExist(path))
		{
			FileSerializer file = new FileSerializer();
			if (file.Open(path, FileMode.READ))
			{
				file.Read(ConfigVersion);

				OnRecieve(file);

				file.Close();
			}
			return true;
		}
		else if (FileExist(legacyPath))
		{
			JsonFileLoader<ExpansionHardlinePlayerData>.JsonLoadFile(legacyPath, this);
			ConfigVersion = CONFIGVERSION;
			DeleteFile(legacyPath);
			Save(fileName);
			return true;
		}
		
		return false;
	}
	
	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(Rank);
		
		ctx.Write(Humanity);
		ctx.Write(Sanity);
		
		ctx.Write(HeroKills);
		ctx.Write(BanditKills);
		ctx.Write(BambiKills);
		ctx.Write(TotalPlayerKills);
		ctx.Write(AIKills);
		ctx.Write(InfectedKills);
		ctx.Write(PlayerDeaths);
	}
	
	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(Rank))
		{
			Error(ToString() + "::OnRecieve Rank");
			return false;
		}
		
		if (!ctx.Read(Humanity))
		{
			Error(ToString() + "::OnRecieve Humanity");
			return false;
		}
				
		if (!ctx.Read(Sanity))
		{
			Error(ToString() + "::OnRecieve Sanity");
			return false;
		}
		
		if (!ctx.Read(HeroKills))
		{
			Error(ToString() + "::OnRecieve HeroKills");
			return false;
		}
		
		if (!ctx.Read(BanditKills))
		{
			Error(ToString() + "::OnRecieve BanditKills");
			return false;
		}
		
		if (!ctx.Read(BambiKills))
		{
			Error(ToString() + "::OnRecieve BambiKills");
			return false;
		}
		
		if (!ctx.Read(TotalPlayerKills))
		{
			Error(ToString() + "::OnRecieve TotalPlayerKills");
			return false;
		}
		
		if (!ctx.Read(AIKills))
		{
			Error(ToString() + "::OnRecieve AIKills");
			return false;
		}
		
		if (!ctx.Read(InfectedKills))
		{
			Error(ToString() + "::OnRecieve InfectedKills");
			return false;
		}

		if (!ctx.Read(PlayerDeaths))
		{
			Error(ToString() + "::OnRecieve PlayerDeaths");
			return false;
		}
		
		return true;
	}
};