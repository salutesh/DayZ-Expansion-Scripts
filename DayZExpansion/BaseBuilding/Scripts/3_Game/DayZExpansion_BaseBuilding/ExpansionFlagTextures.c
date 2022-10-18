/**
 * ExpansionFlagTextures.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionFlagTextures
{
	protected autoptr array<ref ExpansionFlagTexture> m_FlagTextures = new array<ref ExpansionFlagTexture>;

	void ExpansionFlagTextures()
	{
		Load();
	}

	array<ref ExpansionFlagTexture> GetAll()
	{
		return m_FlagTextures;
	}
	
	void Load()
	{
		Add(EXPANSION_FLAG_DAYZ_CHERNARUS, "Chernarus");
		Add(EXPANSION_FLAG_DAYZ_CHEDAKI, "Chedaki");
		Add(EXPANSION_FLAG_DAYZ_NAPA, "NAPA");
		Add(EXPANSION_FLAG_DAYZ_CDF, "CDF");
		Add(EXPANSION_FLAG_DAYZ_LIVONIA, "Livonia");
		Add(EXPANSION_FLAG_DAYZ_ALTIS, "Altis");
		Add(EXPANSION_FLAG_DAYZ_SSAHRANI, "SSahrani");
		Add(EXPANSION_FLAG_DAYZ_NSAHRANI, "NSahrani");
		Add(EXPANSION_FLAG_DAYZ_DAYZ, "DayZ");
		Add(EXPANSION_FLAG_DAYZ_LIVONIA_ARMY, "Livonia Army");
		Add(EXPANSION_FLAG_DAYZ_WHITE, "White");
		Add(EXPANSION_FLAG_DAYZ_BOHEMIA, "Bohemia");
		Add(EXPANSION_FLAG_DAYZ_APA, "APA");
		Add(EXPANSION_FLAG_DAYZ_UEC, "UEC");
		Add(EXPANSION_FLAG_DAYZ_PIRATES, "Pirates");
		Add(EXPANSION_FLAG_DAYZ_CANNIBALS, "Cannibals");
		Add(EXPANSION_FLAG_DAYZ_BEAR, "Bear");
		Add(EXPANSION_FLAG_DAYZ_WOLF, "Wolf");
		Add(EXPANSION_FLAG_DAYZ_BABY_DEER, "Baby Deer");
		Add(EXPANSION_FLAG_DAYZ_ROOSTER, "Rooster");
		Add(EXPANSION_FLAG_DAYZ_LIVONIA_POLICE, "Livonia Police");
		Add(EXPANSION_FLAG_DAYZ_CMC, "CMC");
		Add(EXPANSION_FLAG_DAYZ_TEC, "TEC");
		Add(EXPANSION_FLAG_DAYZ_CHEL, "CHEL");
		Add(EXPANSION_FLAG_DAYZ_ZENIT, "Zenit");
		Add(EXPANSION_FLAG_DAYZ_HUNTERZ, "HunterZ");
		Add(EXPANSION_FLAG_DAYZ_BRAINZ, "BrainZ");
		
		Add(EXPANSION_FLAG_LOGO_EXPANSION, "Expansion");
		Add(EXPANSION_FLAG_COLOR_WHITE, "White");
		Add(EXPANSION_FLAG_COUNTRIES_ARGENTINA, "Argentina");
		Add(EXPANSION_FLAG_COUNTRIES_AUSTRALIA, "Australia");
		Add(EXPANSION_FLAG_COUNTRIES_CANADA, "Canada");
		Add(EXPANSION_FLAG_COUNTRIES_CHERNARUS, "Chernarus");
		Add(EXPANSION_FLAG_COUNTRIES_FRANCE, "France");
		Add(EXPANSION_FLAG_COUNTRIES_GERMANY, "Germany");
		Add(EXPANSION_FLAG_COUNTRIES_LATVIA, "Latvia");
		Add(EXPANSION_FLAG_COUNTRIES_LUXEMBOURG, "Luxembourg");
		Add(EXPANSION_FLAG_COUNTRIES_MEXICO, "Mexico");
		Add(EXPANSION_FLAG_COUNTRIES_NETHERLANDS, "Netherlands");
		Add(EXPANSION_FLAG_COUNTRIES_NEWZEALAND, "New Zealand");
		Add(EXPANSION_FLAG_COUNTRIES_NORWAY, "Norway");
		Add(EXPANSION_FLAG_COUNTRIES_POLAND, "Poland");
		Add(EXPANSION_FLAG_COUNTRIES_RUSSIA, "Russia");
		Add(EXPANSION_FLAG_COUNTRIES_TURKEY, "Turkey");
		Add(EXPANSION_FLAG_COUNTRIES_UK, "UK");
		Add(EXPANSION_FLAG_COUNTRIES_UKRAINE, "Ukraine");
		Add(EXPANSION_FLAG_COUNTRIES_USA, "USA");
		
		Add(EXPANSION_FLAG_COUNTRIES_SCOTLAND, "Scotland");
		Add(EXPANSION_FLAG_COUNTRIES_FINLAND, "Finland");
		Add(EXPANSION_FLAG_COUNTRIES_SWEDEN, "Sweden");
		Add(EXPANSION_FLAG_COUNTRIES_SPAIN, "Spain");
		Add(EXPANSION_FLAG_COUNTRIES_BRAZIL, "Brazil");
		Add(EXPANSION_FLAG_COUNTRIES_PORTUGAL, "Portugal");
		Add(EXPANSION_FLAG_COUNTRIES_BELGIUM, "Belgium");
		Add(EXPANSION_FLAG_COUNTRIES_LIVONIA, "Livonia");
		Add(EXPANSION_FLAG_COUNTRIES_JAPAN, "Japan");
		Add(EXPANSION_FLAG_COUNTRIES_CHINA, "China");
		Add(EXPANSION_FLAG_COUNTRIES_SOUTHKOREA, "SouthKorea");
		Add(EXPANSION_FLAG_COUNTRIES_UN, "UN");
		Add(EXPANSION_FLAG_COUNTRIES_NATO, "NATO");
		Add(EXPANSION_FLAG_COUNTRIES_PIRATE, "Pirate");
		Add(EXPANSION_FLAG_COUNTRIES_CHEDAKI, "Chedaki");
		Add(EXPANSION_FLAG_COUNTRIES_NAPA, "NAPA");
		Add(EXPANSION_FLAG_COUNTRIES_CDF, "CDF");
		Add(EXPANSION_FLAG_COUNTRIES_NUEVORICO, "NuevoRico");
		Add(EXPANSION_FLAG_COUNTRIES_BORDURIA, "Borduria");
		Add(EXPANSION_FLAG_LOGO_BIOHAZARD, "Biohazard");
		Add(EXPANSION_FLAG_LOGO_ANYONEINCHERNO, "AnyoneInCherno");

		Add(EXPANSION_FLAG_COUNTRIES_IRELAND, "Ireland");
		Add(EXPANSION_FLAG_COUNTRIES_ITALY, "Italy");

		Add(EXPANSION_FLAG_COUNTRIES_WALES, "Wales");
		Add(EXPANSION_FLAG_COUNTRIES_SWITZERLAND, "Switzerland");
		Add(EXPANSION_FLAG_COUNTRIES_SKILAND, "SkiLand");
		Add(EXPANSION_FLAG_COUNTRIES_SOUTHAFRICA, "SouthAfrica");
		Add(EXPANSION_FLAG_COUNTRIES_SICILY, "Sicly");
		Add(EXPANSION_FLAG_COUNTRIES_OFFWITHHEAD, "OffWithHead");
		Add(EXPANSION_FLAG_COUNTRIES_GIBRALTAR, "Gibraltar");
		Add(EXPANSION_FLAG_COUNTRIES_CZECHIA, "Czechia");
		Add(EXPANSION_FLAG_COUNTRIES_FARI, "Fari");
		Add(EXPANSION_FLAG_LOGO_DAYZWHITE, "DayZWhite");
		Add(EXPANSION_FLAG_LOGO_DAYZBLACK, "DayZBlack");
		Add(EXPANSION_FLAG_LOGO_DOUBLEAXE, "DoubleAxe");
		Add(EXPANSION_FLAG_LOGO_GRENADE, "Grenade");
		Add(EXPANSION_FLAG_COLOR_RED, "Red");
		Add(EXPANSION_FLAG_COLOR_BLUE, "Blue");
		Add(EXPANSION_FLAG_COLOR_GREEN, "Green");
		Add(EXPANSION_FLAG_COLOR_YELLOW, "Yellow");
		Add(EXPANSION_FLAG_COLOR_ORANGE, "Orange");
		Add(EXPANSION_FLAG_COLOR_PINK, "Pink");
		Add(EXPANSION_FLAG_COLOR_PURPLE, "Purple");
		Add(EXPANSION_FLAG_COLOR_RAINBOW, "Rainbow");
	}
	
	void Add( string path, string name )
	{
		ExpansionFlagTexture flagTexture = new ExpansionFlagTexture( path, name );
		
		m_FlagTextures.Insert( flagTexture );
	}
	
	bool Remove( string path )
	{
		for (int i = 0; i < m_FlagTextures.Count(); ++i)
		{
			if (m_FlagTextures[i] && m_FlagTextures[i].Path == path)
			{
				m_FlagTextures.Remove(i);
				return true;
			}
		}
		
		return false;
	}
}

static ref ExpansionFlagTextures g_exGlobalFlagTextures;

static ExpansionFlagTextures GetExpansionFlagTextures()
{
	if ( !g_exGlobalFlagTextures )
	{
		g_exGlobalFlagTextures = new ExpansionFlagTextures();
	}

	return g_exGlobalFlagTextures;
}
