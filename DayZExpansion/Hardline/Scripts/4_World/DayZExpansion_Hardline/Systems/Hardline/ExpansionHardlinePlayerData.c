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
	static const int CONFIGVERSION = 4;

	int ConfigVersion;
	
	private int Reputation = 0;	
	private int PlayerKills = 0;
	private int AIKills = 0;
	private int InfectedKills = 0;
	private int PlayerDeaths = 0;
	
	[NonSerialized()]
	ExpansionHardlineModule m_HardlineModule;

	void ExpansionHardlinePlayerData()
	{
		ConfigVersion = CONFIGVERSION;
	}
		
	int GetReputation()
	{
		return Reputation;
	}
	
	void AddReputation(int rep)
	{
		Reputation += rep;
	}
	
	void RemoveReputation(int rep)
	{
		Reputation -= rep;
	}
	
	void ResetReputation()
	{
		Reputation = 0;
	}
	
	void OnKillPlayer()
	{
		PlayerKills++;
	}
	
	int GetPlayerKills()
	{
		return PlayerKills;
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
		ctx.Write(Reputation);		
		ctx.Write(PlayerKills);
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
		
		if (!ctx.Read(Reputation))
		{
			Error(ToString() + "::OnRecieve Reputation");
			return false;
		}
		
		if (ConfigVersion < 4)
		{
			int sanity;
			if (!ctx.Read(sanity))
			{
				Error(ToString() + "::OnRecieve Sanity");
				return false;
			}
			
			int heroKills;
			if (!ctx.Read(heroKills))
			{
				Error(ToString() + "::OnRecieve HeroKills");
				return false;
			}
			
			int banditKills;
			if (!ctx.Read(banditKills))
			{
				Error(ToString() + "::OnRecieve BanditKills");
				return false;
			}
			
			int bambiKills;
			if (!ctx.Read(bambiKills))
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

			PlayerKills = heroKills + banditKills + bambiKills;
		}
		else
		{
			if (!ctx.Read(PlayerKills))
			{
				Error(ToString() + "::OnRecieve PlayerKills");
				return false;
			}
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