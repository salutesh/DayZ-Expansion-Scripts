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
	static const int CONFIGVERSION = 5;

	int ConfigVersion;
	
	int Reputation;

	void ExpansionHardlinePlayerData()
	{
		ConfigVersion = CONFIGVERSION;
	}
	
	void Save(string fileName)
	{
		FileSerializer file = new FileSerializer();
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
		if (FileExist(path))
		{
			FileSerializer file = new FileSerializer();
			if (file.Open(path, FileMode.READ))
			{
				file.Read(ConfigVersion);

				OnRead(file);

				if (Reputation < 0)
					Reputation = 0;

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

		if (ConfigVersion >= 5)
			return true;

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
		else
		{
			int playerKills;
			if (!ctx.Read(playerKills))
			{
				Error(ToString() + "::OnRead PlayerKills");
				return false;
			}
		}
		
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
		
		return true;
	}
};