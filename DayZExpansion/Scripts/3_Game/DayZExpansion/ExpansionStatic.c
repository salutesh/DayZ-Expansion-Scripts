/**
 * ExpansionStatic.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionStatic
{
	protected autoptr array<ref ExpansionFlagTexture> m_FlagTextures;
	
	// ------------------------------------------------------------
	// ExpansionStatic Constructor
	// ------------------------------------------------------------
	void ExpansionStatic()
	{
		m_FlagTextures = new array<ref ExpansionFlagTexture>;
		
		LoadFlagTextures();
	}
		
	// ------------------------------------------------------------
	// ExpansionStatic GetFlagTextures
	// ------------------------------------------------------------
	array<ref ExpansionFlagTexture> GetFlagTextures()
	{
		return m_FlagTextures;
	}
	
	// ------------------------------------------------------------
	// Expansion LoadFlagTextures
	// Contains path and name information for each flag texture
	// ------------------------------------------------------------
	void LoadFlagTextures()
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
	void AddFlagTexture( string path, string name )
	{
		ExpansionFlagTexture flagTexture = new ExpansionFlagTexture( path, name );
		
		m_FlagTextures.Insert( flagTexture );
	}
	
	// ------------------------------------------------------------
	// Expansion RemoveFlagTexture
	// ------------------------------------------------------------
	bool RemoveFlagTexture( string path )
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
	
	// ------------------------------------------------------------
	// Expansion GetFlagTexture
	// Hardcoded flag texture paths
	// Not used anymore
	// Will be removed in next update
	// ------------------------------------------------------------	
	string GetFlagTexturePath( int id )
	{
		string path = "";
		
		switch (id)
		{
			case 1:
			{
				path = EXPANSION_FLAG_LOGO_EXPANSION;
				break;
			}
			case 2:
			{
				path = EXPANSION_FLAG_COLOR_WHITE;
				break;
			}
			case 3:
			{
				path = EXPANSION_FLAG_COUNTRIES_AUSTRALIA;
				break;
			}
			case 4:
			{
				path = EXPANSION_FLAG_COUNTRIES_CANADA;
				break;
			}
			case 5:
			{
				path = EXPANSION_FLAG_COUNTRIES_CHERNARUS;
				break;
			}
			case 6:
			{
				path = EXPANSION_FLAG_COUNTRIES_FRANCE;
				break;
			}
			case 7:
			{
				path = EXPANSION_FLAG_COUNTRIES_GERMANY;
				break;
			}
			case 8:
			{
				path = EXPANSION_FLAG_COUNTRIES_LATVIA;
				break;
			}
			case 9:
			{
				path = EXPANSION_FLAG_COUNTRIES_LUXEMBOURG;
				break;
			}
			case 10:
			{
				path = EXPANSION_FLAG_COUNTRIES_MEXICO;
				break;
			}
			case 11:
			{
				path = EXPANSION_FLAG_COUNTRIES_NETHERLANDS;
				break;
			}
			case 12:
			{
				path = EXPANSION_FLAG_COUNTRIES_NEWZEALAND;
				break;
			}
			case 13:
			{
				path = EXPANSION_FLAG_COUNTRIES_NORWAY;
				break;
			}
			case 14:
			{
				path = EXPANSION_FLAG_COUNTRIES_POLAND;
				break;
			}
			case 15:
			{
				path = EXPANSION_FLAG_COUNTRIES_RUSSIA;
				break;
			}
			case 16:
			{
				path = EXPANSION_FLAG_COUNTRIES_UK;
				break;
			}
			case 17:
			{
				path = EXPANSION_FLAG_COUNTRIES_USA;
				break;
			}			
			case 18:
			{
				path = EXPANSION_FLAG_COUNTRIES_SCOTTISH;
				break;
			}
			case 19:
			{
				path = EXPANSION_FLAG_COUNTRIES_FINNISH;
				break;
			}
			case 20:
			{
				path = EXPANSION_FLAG_COUNTRIES_SWEDISH;
				break;
			}
			case 21:
			{
				path = EXPANSION_FLAG_COUNTRIES_SPAIN;
				break;
			}
			case 22:
			{
				path = EXPANSION_FLAG_COUNTRIES_BRAZIL;
				break;
			}
			case 23:
			{
				path = EXPANSION_FLAG_COUNTRIES_PORTUGAL;
				break;
			}
			case 24:
			{
				path = EXPANSION_FLAG_COUNTRIES_BELGIUM;
				break;
			}
			case 25:
			{
				path = EXPANSION_FLAG_COUNTRIES_LIVONIA;
				break;
			}
			case 26:
			{
				path = EXPANSION_FLAG_COUNTRIES_JAPAN;
				break;
			}
			case 27:
			{
				path = EXPANSION_FLAG_COUNTRIES_CHINA;
				break;
			}
			case 28:
			{
				path = EXPANSION_FLAG_COUNTRIES_SOUTHKOREA;
				break;
			}
			case 29:
			{
				path = EXPANSION_FLAG_COUNTRIES_UN;
				break;
			}
			case 30:
			{
				path = EXPANSION_FLAG_COUNTRIES_NATO;
				break;
			}
			case 31:
			{
				path = EXPANSION_FLAG_COUNTRIES_PIRATE;
				break;
			}
			case 32:
			{
				path = EXPANSION_FLAG_COUNTRIES_CHEDAKI;
				break;
			}
			case 33:
			{
				path = EXPANSION_FLAG_COUNTRIES_NAPA;
				break;
			}
			case 34:
			{
				path = EXPANSION_FLAG_COUNTRIES_CDF;
				break;
			}
			case 35:
			{
				path = EXPANSION_FLAG_COUNTRIES_NUEVORICO;
				break;
			}
			case 36:
			{
				path = EXPANSION_FLAG_COUNTRIES_BORDURIENS;
				break;
			}
			case 37:
			{
				path = EXPANSION_FLAG_LOGO_BIOHAZARD;
				break;
			}
			case 38:
			{
				path = EXPANSION_FLAG_LOGO_ANYONEINCHERNO;
				break;
			}
			case 39:
			{
				path = EXPANSION_FLAG_COUNTRIES_IRISH;
				break;
			}
			case 40:
			{
				path = EXPANSION_FLAG_COUNTRIES_WALES;
				break;
			}
			case 41:
			{
				path = EXPANSION_FLAG_COUNTRIES_SWITZERLAND;
				break;
			}
			case 42:
			{
				path = EXPANSION_FLAG_COUNTRIES_SKILAND;
				break;
			}
			case 43:
			{
				path = EXPANSION_FLAG_COUNTRIES_SOUTHAFRICA;
				break;
			}
			case 44:
			{
				path = EXPANSION_FLAG_COUNTRIES_SICILY;
				break;
			}
			case 45:
			{
				path = EXPANSION_FLAG_COUNTRIES_OFFWITHHEAD;
				break;
			}
			case 46:
			{
				path = EXPANSION_FLAG_COUNTRIES_GIBRALTAR;
				break;
			}
			case 47:
			{
				path = EXPANSION_FLAG_COUNTRIES_CZECH;
				break;
			}
			case 48:
			{
				path = EXPANSION_FLAG_COUNTRIES_FARI;
				break;
			}
			case 49:
			{
				path = EXPANSION_FLAG_COUNTRIES_FINLAND;
				break;
			}
			case 50:
			{
				path = EXPANSION_FLAG_LOGO_DAYZWHITE;
				break;
			}
			case 51:
			{
				path = EXPANSION_FLAG_LOGO_DAYZBLACK;
				break;
			}
			case 52:
			{
				path = EXPANSION_FLAG_LOGO_DOUBLEAXE;
				break;
			}
			case 53:
			{
				path = EXPANSION_FLAG_LOGO_GRENADE;
				break;
			}
			case 54:
			{
				path = EXPANSION_FLAG_COLOR_RED;
				break;
			}
			case 55:
			{
				path = EXPANSION_FLAG_COLOR_BLUE;
				break;
			}
			case 56:
			{
				path = EXPANSION_FLAG_COLOR_GREEN;
				break;
			}
			case 57:
			{
				path = EXPANSION_FLAG_COLOR_YELLOW;
				break;
			}
			case 58:
			{
				path = EXPANSION_FLAG_COLOR_ORANGE;
				break;
			}
			case 59:
			{
				path = EXPANSION_FLAG_COLOR_PINK;
				break;
			}
			case 60:
			{
				path = EXPANSION_FLAG_COLOR_PURPLE;
				break;
			}
			case 61:
			{
				path = EXPANSION_FLAG_COLOR_RAINBOW;
				break;
			}
			case 62:
			{
				path = EXPANSION_FLAG_DAYZ_CHERNARUS;
				break;
			}
			case 63:
			{
				path = EXPANSION_FLAG_DAYZ_CHEDAKI;
				break;
			}
			case 64:
			{
				path = EXPANSION_FLAG_DAYZ_NAPA;
				break;
			}
			case 65:
			{
				path = EXPANSION_FLAG_DAYZ_CDF;
				break;
			}
			case 66:
			{
				path = EXPANSION_FLAG_DAYZ_LIVONIA;
				break;
			}
			case 67:
			{
				path = EXPANSION_FLAG_DAYZ_ALTIS;
				break;
			}
			case 68:
			{
				path = EXPANSION_FLAG_DAYZ_SSAHRANI;
				break;
			}
			case 69:
			{
				path = EXPANSION_FLAG_DAYZ_NSAHRANI;
				break;
			}
			case 70:
			{
				path = EXPANSION_FLAG_DAYZ_NSAHRANI;
				break;
			}
			case 71:
			{
				path = EXPANSION_FLAG_DAYZ_DAYZ;
				break;
			}
			case 72:
			{
				path = EXPANSION_FLAG_DAYZ_LIVONIA_ARMY;
				break;
			}
			case 73:
			{
				path = EXPANSION_FLAG_DAYZ_WHITE;
				break;
			}
			case 74:
			{
				path = EXPANSION_FLAG_DAYZ_BOHEMIA;
				break;
			}
			case 75:
			{
				path = EXPANSION_FLAG_DAYZ_APA;
				break;
			}
			case 76:
			{
				path = EXPANSION_FLAG_DAYZ_UEC;
				break;
			}
			case 77:
			{
				path = EXPANSION_FLAG_DAYZ_PIRATES;
				break;
			}
			case 78:
			{
				path = EXPANSION_FLAG_DAYZ_CANNIBALS;
				break;
			}
			case 79:
			{
				path = EXPANSION_FLAG_DAYZ_BEAR;
				break;
			}
			case 80:
			{
				path = EXPANSION_FLAG_DAYZ_WOLF;
				break;
			}
			case 81:
			{
				path = EXPANSION_FLAG_DAYZ_BABY_DEER;
				break;
			}
			case 82:
			{
				path = EXPANSION_FLAG_DAYZ_ROOSTER;
				break;
			}
			case 83:
			{
				path = EXPANSION_FLAG_DAYZ_LIVONIA_POLICE;
				break;
			}
			case 83:
			{
				path = EXPANSION_FLAG_DAYZ_CMC;
				break;
			}
			case 84:
			{
				path = EXPANSION_FLAG_DAYZ_TEC;
				break;
			}
			case 85:
			{
				path = EXPANSION_FLAG_DAYZ_CHEL;
				break;
			}
			case 86:
			{
				path = EXPANSION_FLAG_DAYZ_ZENIT;
				break;
			}
			case 87:
			{
				path = EXPANSION_FLAG_DAYZ_HUNTERZ;
				break;
			}
			case 88:
			{
				path = EXPANSION_FLAG_DAYZ_BRAINZ;
				break;
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionStatic::GetFlagTexturePath - Return texture path: " + path + " for ID:" + id.ToString() );
		#endif
	
		return path;
	}
};

static ref ExpansionStatic g_exGlobalStatic;

static ExpansionStatic GetExpansionStatic()
{
	if ( !g_exGlobalStatic )
	{
		g_exGlobalStatic = new ExpansionStatic();
	}

	return g_exGlobalStatic;
}