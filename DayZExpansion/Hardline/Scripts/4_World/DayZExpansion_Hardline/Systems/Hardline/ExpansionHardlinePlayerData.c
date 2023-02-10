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

//! LEGACY: Only exists so we can read old hardline data
class ExpansionHardlinePlayerData
{
	static const int CONFIGVERSION = 4;

	int ConfigVersion;
	
	private int Reputation = 0;	
	private int PlayerKills = 0;
	private int AIKills = 0;
	private int InfectedKills = 0;
	private int PlayerDeaths = 0;

	void ExpansionHardlinePlayerData()
	{
		ConfigVersion = CONFIGVERSION;
	}

	int GetReputation()
	{
		return Reputation;
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
			DeleteFile(path);
			return true;
		}
		
		return false;
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

			PlayerKills = heroKills + banditKills + bambiKills;
		}
		else
		{
			if (!ctx.Read(PlayerKills))
			{
				Error(ToString() + "::OnRead PlayerKills");
				return false;
			}
		}
		
		if (!ctx.Read(AIKills))
		{
			Error(ToString() + "::OnRead AIKills");
			return false;
		}
		
		if (!ctx.Read(InfectedKills))
		{
			Error(ToString() + "::OnRead InfectedKills");
			return false;
		}

		if (!ctx.Read(PlayerDeaths))
		{
			Error(ToString() + "::OnRead PlayerDeaths");
			return false;
		}
		
		return true;
	}
};