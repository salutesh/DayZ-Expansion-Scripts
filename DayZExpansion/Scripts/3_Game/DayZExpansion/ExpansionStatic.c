/**
 * ExpansionStatic.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ExpansionStatic
{
	protected static autoptr array<ref ExpansionFlagTexture> m_FlagTextures;
	
	// ------------------------------------------------------------
	// ExpansionStatic GetFlagTextures
	// ------------------------------------------------------------
	static array<ref ExpansionFlagTexture> GetFlagTextures()
	{
		if (!m_FlagTextures)
		{
			m_FlagTextures = new array<ref ExpansionFlagTexture>;
			
			LoadFlagTextures();
		}

		return m_FlagTextures;
	}
	
	// ------------------------------------------------------------
	// Expansion LoadFlagTextures
	// Contains path and name information for each flag texture
	// ------------------------------------------------------------
	static void LoadFlagTextures()
	{
		AddFlagTexture(EXPANSION_FLAG_DAYZ_CHERNARUS, "Chernarus");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_CHEDAKI, "Chedaki");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_NAPA, "NAPA");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_CDF, "CDF");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_LIVONIA, "Livonia");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_ALTIS, "Altis");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_SSAHRANI, "SSahrani");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_NSAHRANI, "NSahrani");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_DAYZ, "DayZ");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_LIVONIA_ARMY, "Livonia Army");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_WHITE, "White");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_BOHEMIA, "Bohemia");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_APA, "APA");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_UEC, "UEC");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_PIRATES, "Pirates");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_CANNIBALS, "Cannibals");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_BEAR, "Bear");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_WOLF, "Wolf");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_BABY_DEER, "Baby Deer");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_ROOSTER, "Rooster");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_LIVONIA_POLICE, "Livonia Police");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_CMC, "CMC");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_TEC, "TEC");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_CHEL, "CHEL");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_ZENIT, "Zenit");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_HUNTERZ, "HunterZ");
		AddFlagTexture(EXPANSION_FLAG_DAYZ_BRAINZ, "BrainZ");
		
		AddFlagTexture(EXPANSION_FLAG_LOGO_EXPANSION, "Expansion");
		AddFlagTexture(EXPANSION_FLAG_COLOR_WHITE, "White");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_AUSTRALIA, "Australia");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_CANADA, "Canada");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_CHERNARUS, "Chernarus");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_FRANCE, "France");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_GERMANY, "Germany");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_LATVIA, "Latvia");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_LUXEMBOURG, "Luxembourg");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_MEXICO, "Mexico");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_NETHERLANDS, "Netherlands");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_NEWZEALAND, "New Zealand");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_NORWAY, "Norway");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_POLAND, "Poland");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_RUSSIA, "Russia");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_UK, "UK");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_USA, "USA");
		
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_SCOTTISH, "Scottish");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_FINNISH, "Finnish");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_SWEDISH, "Swedish");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_SPAIN, "Spain");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_BRAZIL, "Brazil");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_PORTUGAL, "Portugal");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_BELGIUM, "Belgium");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_LIVONIA, "Livonia");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_JAPAN, "Japan");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_CHINA, "China");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_SOUTHKOREA, "SouthKorea");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_UN, "UN");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_NATO, "NATO");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_PIRATE, "Pirate");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_CHEDAKI, "Chedaki");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_NAPA, "NAPA");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_CDF, "CDF");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_NUEVORICO, "NuevoRico");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_BORDURIENS, "Borduriens");
		AddFlagTexture(EXPANSION_FLAG_LOGO_BIOHAZARD, "Biohazard");
		AddFlagTexture(EXPANSION_FLAG_LOGO_ANYONEINCHERNO, "AnyoneInCherno");

		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_IRISH, "Irish");

		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_WALES, "Wales");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_SWITZERLAND, "Switzerland");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_SKILAND, "SkiLand");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_SOUTHAFRICA, "SouthAfrica");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_SICILY, "Sicly");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_OFFWITHHEAD, "OffWithHead");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_GIBRALTAR, "Gibraltar");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_CZECH, "Czech");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_FARI, "Fari");
		AddFlagTexture(EXPANSION_FLAG_COUNTRIES_FINLAND, "Finland");
		AddFlagTexture(EXPANSION_FLAG_LOGO_DAYZWHITE, "DayZWhite");
		AddFlagTexture(EXPANSION_FLAG_LOGO_DAYZBLACK, "DayZBlack");
		AddFlagTexture(EXPANSION_FLAG_LOGO_DOUBLEAXE, "DoubleAxe");
		AddFlagTexture(EXPANSION_FLAG_LOGO_GRENADE, "Grenade");
		AddFlagTexture(EXPANSION_FLAG_COLOR_RED, "Red");
		AddFlagTexture(EXPANSION_FLAG_COLOR_BLUE, "Blue");
		AddFlagTexture(EXPANSION_FLAG_COLOR_GREEN, "Green");
		AddFlagTexture(EXPANSION_FLAG_COLOR_YELLOW, "Yellow");
		AddFlagTexture(EXPANSION_FLAG_COLOR_ORANGE, "Orange");
		AddFlagTexture(EXPANSION_FLAG_COLOR_PINK, "Pink");
		AddFlagTexture(EXPANSION_FLAG_COLOR_PURPLE, "Purple");
		AddFlagTexture(EXPANSION_FLAG_COLOR_RAINBOW, "Rainbow");
	}
	
	// ------------------------------------------------------------
	// Expansion AddFlagTexture
	// ------------------------------------------------------------
	static void AddFlagTexture( string path, string name )
	{
		ExpansionFlagTexture flagTexture = new ExpansionFlagTexture( path, name );
		
		m_FlagTextures.Insert( flagTexture );
	}
};
