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
	static const int CONFIGVERSION = 3;

	int ConfigVersion;
	
	private int Humanity = 100;
	private int Sanity = 100;
	
	private int HeroKills = 0;
	private int BanditKills = 0;
	private int BambiKills = 0;
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
		return GetExpansionSettings().GetHardline().GetRank(Humanity);
	}
	
	int GetHumanity()
	{
		return Humanity;
	}
	
	void AddHumanity(int humanity)
	{
		Humanity += humanity;
	}
	
	void RemoveHumanity(int humanity)
	{
		Humanity -= humanity;
	}
	
	void ResetHumanity()
	{
		Humanity = 100;
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
	
	string GetRankDisplayName()
	{
		return "STR_EXPANSION_HARDLINE_RANK_" + typename.EnumToString(ExpansionHardlineRank, GetRank());
	}
	
	void OnKillBandit()
	{
		BanditKills++;
	}
	
	int GetBanditKills()
	{
		return BanditKills;
	}
	
	void OnKillHero()
	{
		HeroKills++;
	}
	
	int GetHeroKills()
	{
		return HeroKills;
	}
	
	void OnKillBambi()
	{
		BambiKills++;
	}
	
	int GetBambiKills()
	{
		return BambiKills;
	}
	
	int GetTotalPlayerKills()
	{
		return BanditKills + HeroKills + BambiKills;
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
			ConfigVersion = CONFIGVERSION;
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
		ctx.Write(Humanity);
		ctx.Write(Sanity);
		
		ctx.Write(HeroKills);
		ctx.Write(BanditKills);
		ctx.Write(BambiKills);
		ctx.Write(AIKills);
		ctx.Write(InfectedKills);
		ctx.Write(PlayerDeaths);
	}
	
	bool OnRecieve(ParamsReadContext ctx)
	{
		int rank;
		if (ConfigVersion < 3 && !ctx.Read(rank))
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
		
		int totalPlayerKills;
		if (ConfigVersion < 3 && !ctx.Read(totalPlayerKills))
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